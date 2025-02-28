import { WebSocketServer } from "ws";
import { IncomingMessage } from "http";

import { Log } from "./sys";
import Client from "./client";

function main() {
  const webSocketServer = new WebSocketServer({ port: 5941 });
  Log(`Server Running On Port ${webSocketServer.options.port}`);
  webSocketServer.on("connection", OnConnection);
}

function OnConnection(ws: WebSocket, req: IncomingMessage) {
  Client.NewConnection(ws, req);
}

main();
