using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Text.Json;
using System.Threading.Tasks;
using System.Data;

namespace server.src.utils
{
    public class Database
    {
        private readonly MySqlConnection db;
        private readonly string connectionInfo;

        public readonly string host;
        public readonly int port;
        public readonly string dbname;
        public readonly string dbuser;
        public readonly string dbpassword;

        private const string sql_user = "SELECT * FROM `users` WHERE uid = @uid";
        private const string sql_reg = "INSERT INTO `users` (uid, email, password, userdata) VALUES (@uid, @email, @pwdhash, @dataJson)";
        private const string sql_updateLoginTime = "UPDATE users SET last_login_time = CURRENT_TIMESTAMP WHERE uid = @uid";
        private Database(string host, int port, string dbname, string dbuser, string dbpassword)
        {
            this.host = host;
            this.port = port;
            this.dbname = dbname;
            this.dbuser = dbuser;
            this.dbpassword = dbpassword;

            connectionInfo =
                $"server={host};" +
                $"port={port};" +
                $"database={dbname};" +
                $"uid={dbuser};" +
                $"password={dbpassword};";

            db = new MySqlConnection(connectionInfo);

        }

        public async Task<KeyValuePair<Data.User?, string>> QueryUserAsync(string uid)
        {
            try
            {
                if (db.State != ConnectionState.Open)
                    await db.OpenAsync();

                using var query_user = new MySqlCommand(sql_user, db);
                query_user.Parameters.AddWithValue("@uid", uid);
                using var reader_ = await query_user.ExecuteReaderAsync();
                if (reader_ is not MySqlDataReader reader || !reader.Read())
                    throw new InvalidOperationException();

                var data = JsonSerializer.Deserialize<JsonObject>(reader.GetString("userdata")) ??
                    throw new JsonException();

                var guid = reader.GetGuid("guid");
                var email = reader.GetString("email");
                var hashed_password = reader.GetString("password");

                await db.CloseAsync();

                return new(new Data.User(guid, uid, email, data), hashed_password);
            }
            catch
            {
                return new(null, "");
            }
        }

        public async Task<bool> QueryRegisterAsync(string uid, string email, string hashed_password, JsonObject userData)
        {
            try
            {
                if (db.State != ConnectionState.Open)
                    await db.OpenAsync();

                using var query = new MySqlCommand(sql_reg, db);

                query.Parameters.AddWithValue("@uid", uid);
                query.Parameters.AddWithValue("@email", email);
                query.Parameters.AddWithValue("@pwdhash", hashed_password);
                query.Parameters.AddWithValue("@dataJson", JsonSerializer.Serialize(userData));

                await db.CloseAsync();

                var result = await query.ExecuteNonQueryAsync();
                return result > 0;
            }
            catch
            {
                return false;
            }
        }

        public async Task QueryUserLoginAsync(string uid)
        {
            if (db.State != ConnectionState.Open)
                await db.OpenAsync();

            using var query = new MySqlCommand(sql_updateLoginTime, db);
            query.Parameters.AddWithValue("@uid", uid);
            await query.ExecuteNonQueryAsync();
        }

        public static Database? Make(string host, int port, string dbname, string dbuser, string dbpassword)
        {
            try
            {
                if(port < 0 ||  port > 65535)
                    throw new ArgumentOutOfRangeException(nameof(port));
                return new Database(host, port, dbname, dbuser, dbpassword);
            }
            catch
            {
                return null;
            }
        }

    }
}
