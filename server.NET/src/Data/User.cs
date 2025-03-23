using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;

namespace server.src.Data
{
    public class User(Guid guid, string uid, string email, JsonObject data)
    {
        public Guid Guid { get; } = guid;
        public string Uid { get; } = uid;
        public string Email { get; } = email;
        public JsonObject Data { get; } = data;
    }
}
