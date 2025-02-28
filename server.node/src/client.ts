import { stringify } from "querystring";
import Server from "./server";
import { Log, LogLevel, RandInt } from "./sys";


export default class Client {
  private static clientList: Map<string, Client> = new Map();
  private static onlineList: Map<string, string> = new Map();

  private ws: WebSocket | null;

  readonly addr: string = "(unknownAddr)";
  readonly clientID: string;

  private _pubkey: string = "";
  private _uid: string = "";
  private _status: string = "logout";

  get status(): string {
    return this._status;
  }
  get uid(): string {
    return this._uid;
  }
  get pubkey(): string {
    return this._pubkey;
  }
  private set pubkey(value: string) {
    this._pubkey = value;
  }
  private set uid(value: string) {
    this._uid = value;
  }
  private set status(value: string) {
    this._status = value;
  }

  constructor(ws: WebSocket, addr: string) {
    this.ws = ws;
    this.addr = addr;
    this.clientID = RandInt(10000, 99999).toString();
    ws.onmessage = (e) => {
      try {
        const message = JSON.parse(e.data);
        this.Recv_Message(message);
      } catch (error) {
        Log(`(Client/RECV/ERR) ${e.data}:${error}`, LogLevel.ERROR);
      }
    };
    Client.clientList.set(this.clientID, this);
  }

  Recv_Message(message: { type: string; data: any }) {
    if (this.status === "login") {
      if (message["type"] === "msg")
        this.Server_ForwardMessage(message["data"]);
      else if (message["type"] === "req") {
        if (message["data"] === "online") {
          const list = Array.from(Client.onlineList.keys());
          const reply = {
            type: "online",
            data: list
          };
          this.ws?.send(JSON.stringify(reply));
        }
      }
      else if (message["type"] === "echo")
        this.ws?.send(JSON.stringify({ type: "echo", data: message["data"] }));
    } else if (this.status === "logout") {
      if (message["type"] === "login") {
        const data = message["data"];
        this.Server_Authenticate(data);
      }
    }
  }


  Server_ForwardMessage(message: { msg: string; from: string; to: string }) {
    if (message.from === this.uid) {
      const target = Client.onlineList.get(message.to);
      if (target) {
        const client = Client.clientList.get(target);

        if (client) {
          client.ws?.send(JSON.stringify({ type: "msg", data: message }));
        }
      }
    }

  }

  OnlineListChanged() {
    let onlineClients = new Array<Client>();
    let list = new Array<{ uid: string, publicKey: string }>();
    for (const [uid, cid] of Client.onlineList.entries()) {
      const client = Client.clientList.get(cid);
      if (client) {
        onlineClients.push(client);
        list.push({ uid: uid, publicKey: client.pubkey });
      }
    }
    for (const client of onlineClients)
      client.ws?.send(JSON.stringify({ type: "online", data: list }));
  }

  Server_Authenticate(data: { uid: string; pwd: string, pubkey: string }) {
    const AuthResult = (authResult: boolean | any) => {
      Log(`AuthResult(${this.clientID}/${data.uid})${authResult ? "OK" : "FAILED"}`);
      let r = false;
      let reply = {
        type: "login",
        data: {
          result: "AUTH_FAILED",
          info: ""
        }
      };
      if (typeof authResult === "boolean") {
        r = authResult;

      }
      else {
        reply.data.info = authResult as string;
      }
      if (r) {
        reply.data.result = "AUTH_OK";
        reply.data.info = data.uid;
        this.uid = data.uid;
        this.status = "login";
        this.pubkey = data.pubkey;

      }
      else {
        reply.data.info = "UID or Password incorrect."
      }
      this.ws?.send(JSON.stringify(reply));
      if (r) {
        const online = Client.onlineList.get(data.uid);
        if (online) {
          const client = Client.clientList.get(online);
          const logout = {
            type: "logout",
            data: "LOGIN_ELSEWHERE"
          }
          client?.ws?.send(JSON.stringify(logout));
        }
        Client.onlineList.set(data.uid, this.clientID);
        this.OnlineListChanged();
      }
    };
    const result = Server.Auth(data.uid, data.pwd);
    result.then(AuthResult).catch(AuthResult);
  }

  static NewConnection(ws: WebSocket, req: any) {
    const addr = `${req.socket.remoteAddress}:${req.socket.remotePort}`;
    const client = new Client(ws, addr);
    Log(`WSConnection(${client.clientID})New:${addr}`);
    ws.onclose = (e) => {
      Log(
        `WSConnection(${client.clientID})Closed:${addr};reason=${e.reason}`
      );
      this.Disconnect(client.clientID);
    };
  }

  static Disconnect(clientID: string) {
    const client = this.clientList.get(clientID);
    Client.clientList.delete(clientID);
    if (client && client.ws) {
      Client.onlineList.delete(client.uid);
      client.OnlineListChanged();
      client.ws.onmessage = null;
      client.ws.onerror = null;
      client.ws.onclose = null;
      client.ws = null;
    }

  }
}

Server;