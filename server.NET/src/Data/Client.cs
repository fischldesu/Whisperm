using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.WebSockets;
using System.Text;
using System.Threading.Tasks;

namespace server.src.Data
{
    internal class Client(WebSocket socket, string remoteEndpoint)
    {
        public Guid ID { get; } = Guid.NewGuid();
        public WebSocket Socket { get; } = socket;
        public DateTime ConnectedTime { get; } = DateTime.UtcNow;
        public string RemoteEndpoint { get; } = remoteEndpoint;
    }

}
