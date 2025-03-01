import Bcrypt from "bcrypt";
import mysql2, { QueryResult } from "mysql2";
import { Log, LogLevel } from "./sys";

const salt_rounds: number = 10;

class _Server {
  private static Instance: _Server;
  private readonly db: mysql2.Connection | null = null;

  private users: Map<string, string> = new Map();
  
  private constructor() {
    try {
      //this is an internal host database
      const dbinfo = {
        host: "1Panel-mysql-sAYK",
        port: 3306,
        user: "fischldesu",
        database: "whisperm",
        password: "A6hmBHB58i4Y7kYr",
      };
      this.db = mysql2.createConnection(dbinfo);
      this.db.ping();
      Log("MySQL Connected (UserInfoDB).\n", LogLevel.INFO);
      this.InitData();
    } catch (err) {
      Log(`MySQL Connect Failed: ${err}`, LogLevel.ERROR);
    }
  }

  private InitData() {
    function StoreUser(this: _Server, table: QueryResult) {
        const users = table as {name:string, data:string, nickname:string}[];
        for (let index = 0; index < users.length; index++) {
            const user = users[index];
            const pwd = user.data.split('|')[1];
            if(pwd) this.users.set(user.name, pwd);
        }
        this.db?.end((err)=>{
          if(err)
            Log(`MySQL Connection Close Error: ${err}`, LogLevel.ERROR);
        });
    }
    if (this.db) {
      this.db.execute("SELECT * FROM user", (err, results: QueryResult) => {
        if (err) {
          Log(`MySQL QueryFailed: ${err}`, LogLevel.ERROR);
        } else
          try {
            StoreUser.call(this, results);
          } catch (err) {
            Log(`MySQL DataException: ${err}`, LogLevel.ERROR);
          }
      });
    }
  }

  public static get Server() {
    if (!this.Instance) {
      this.Instance = new _Server();
    }
    return this.Instance;
  }

  public static async Encrypt(data:string){
    return await Bcrypt.hash(data, salt_rounds)
  }

  public async Auth(uid: string, pwd: string): Promise<boolean> {
    const userpwd = this.users.get(uid);
    if (userpwd) return await Bcrypt.compare(pwd, userpwd);
    return false;
  }
}

const Server = _Server.Server;
export default Server;
