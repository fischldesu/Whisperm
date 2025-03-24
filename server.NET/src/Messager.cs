using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using server.src.Data;
using server.src.utils;
using static server.src.utils.Logger;

namespace server.src
{
    internal class Messager
    {
        public Server Server { get; }

        private readonly ConcurrentDictionary<string, Client> online = new();

        public Messager(Server server)
        {
            Server = server;
            Server.MessageReceived += OnMessageReceived;
            Server.ClientConnected += OnClientConnected;
            Server.ClientDisconnected += OnClientDisconnected;
        }
        
        private void OnMessageReceived(object? sender, Server.MessageReceivedEventArgs e)
        {
            var from = e.Client;
            try
            {
                var sourceMessage = e.Message;
                var type = Message.MessageType.UNKNOWN;

                var message = JsonNode.Parse(sourceMessage) ?? throw new NullReferenceException();
                switch (message.ToString())
                {
                    case "msg":
                        type = Message.MessageType.MSG;
                        break;
                    case "login":
                        type = Message.MessageType.LOGIN;
                        break;
                    case "logout":
                        type = Message.MessageType.LOGOUT;
                        break;
                    case "register":
                        type = Message.MessageType.REGISTER;
                        break;
                }

                var data = message["data"];
                var attach = message["attach"];
                if (data != null && type != Message.MessageType.UNKNOWN)
                {
                    _ = ReplyMessage(new Message(from, type, data, attach?.ToString()));
                }
                else
                {
                    SendMessage(from, Message.MessageType.ERROR, new JsonObject { { "errtype", "err" }, { "err", "UNKNOWN_MESSAGE" } });
                }
            }
            catch (System.Text.Json.JsonException)
            {
                SendMessage(from, Message.MessageType.ERROR, new JsonObject { { "errtype", "json" }, { "json", "NOT_A_VALID_JSON" } });
            }
            catch (NullReferenceException ex)
            {
                SendMessage(from, Message.MessageType.ERROR, new JsonObject { { "errtype", "ref" }, { "ref", $"VALUE_INVALID:{ex.Message}" } });
            }
            catch (Exception ex)
            {
                SendMessage(from, Message.MessageType.ERROR, new JsonObject { { "errtype", "unknown" }, { "unknown", ex.Message } });
            }
        }

        private void OnClientConnected(object? sender, Server.ClientEventArgs e)
        {
            Log($"(NewConnection) {e.Client.ID}");
        }

        private void OnClientDisconnected(object? sender, Server.ClientEventArgs e)
        {
            Log($"(ClosedConnection) {e.Client.ID}");
        }

        private async Task ReplyMessage(Message message)
        {
            MessageAnalyzer result = await MessageAnalyzer.Analyze(message);
            SendMessage(result.Target, result.Type, result.Data, result.Attach);
            if (result.MessageForwardOk)
                SendMessage(message.Sender, result.Type, new JsonObject{ { "reply", "ok" }, { "stamp", message.Attach } });
        }

        private async void SendMessage(Client target, Message.MessageType type, JsonObject child, string attach = "")
        {
            string reply = new JsonObject
            {
                { "type", type.ToString().ToLower() },
                { "data", child },
                { "attach", attach }
            }.ToJsonString();
            
            if(!await Server.SendTextAsync(target, reply))
                Log($"ERROR send message to client:{reply}");
            
        }

    }
}
