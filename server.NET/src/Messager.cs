using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using static server.src.utils.Logger;

namespace server.src
{
    internal class Messager
    {
        enum MessageType
        {
            MSG,
            LOGIN,
            LOGOUT,
            REGISTER,
            ERROR
        }

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
            Client from = e.Client;
            try
            {
                var message = JsonNode.Parse(e.Message);
                //
            }
            catch (System.Text.Json.JsonException)
            {
                SendMessage(from, MessageType.ERROR, new JsonObject { { "json", "NOT_A_VALID_JSON" } });
            }
            catch (NullReferenceException ex)
            {
                SendMessage(from, MessageType.ERROR, new JsonObject { { "ref", $"VALUE_INVALID:{ex.Message}" } });
            }
            catch (Exception ex)
            {
                SendMessage(from, MessageType.ERROR, new JsonObject { { "exception", ex.Message } });
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

        private async void SendMessage(Client client, MessageType type, string child, string attach = "")
        {
            string reply = new JsonObject
            {
                { "type", type.ToString()},
                { "data", child },
                { "attach", attach }
            }.ToJsonString();
            if (!await Server.SendTextAsync(client, reply))
            {
                Log($"ERROR send message to client:{reply}");
            }
        }
        private async void SendMessage(Client client, MessageType type, JsonObject child, string attach = "")
        {
            string reply = new JsonObject
            {
                { "type", type.ToString().ToLower() },
                { "data", child },
                { "attach", attach }
            }.ToJsonString();
            if(!await Server.SendTextAsync(client, reply))
            {
                Log($"ERROR send message to client:{reply}");
            }
        }
    }
}
