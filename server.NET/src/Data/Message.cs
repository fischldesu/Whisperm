using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using static server.src.Data.Message;

namespace server.src.Data
{
    internal class Message(Client sender, Message.MessageType type, JsonNode data, string? attacch)
    {
        public enum MessageType
        {
            MSG,
            LOGIN,
            LOGOUT,
            REGISTER,
            ERROR,
            UNKNOWN
        }
        public JsonNode Data { get; } = data;
        public Client Sender { get; } = sender;
        public MessageType Type { get; } = type;
        public string? Attach { get; } = attacch;
    }
}
