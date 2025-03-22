using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Buffers;
using static server.src.utils.Logger;
using server.src.Data;

namespace server.src
{
    internal class Server : IDisposable
    {

        private const int BufferSize = 4096;
        private const int MaxMessageSize = 8 * 1024 * 1024; // 8MB

        public static readonly int DefaultPort = 5941;
        public string Host { get; }
        public int Port { get; }
        public bool Online => listener?.IsListening ?? false;

        private readonly ConcurrentDictionary<Guid, Client> clients = new();
        private CancellationTokenSource cancellationTokenSource = new();
        private HttpListener listener;
        private bool isDisposed;

        public event EventHandler<MessageReceivedEventArgs> MessageReceived;
        public event EventHandler<ClientEventArgs> ClientConnected;
        public event EventHandler<ClientEventArgs> ClientDisconnected;

        public class MessageReceivedEventArgs(Client client, string message) : EventArgs
        {
            public Client Client { get; } = client;
            public string Message { get; } = message;
        }

        public class ClientEventArgs(Client client) : EventArgs
        {
            public Client Client { get; } = client;
        }

        public Server(int port)
        {
            Host = "127.0.0.1";
            Port = port;
            Initialize();
        }

        private void Initialize()
        {
            try
            {
                if (!HttpListener.IsSupported)
                    throw new PlatformNotSupportedException("HTTP Listener not supported on this platform");
                if(Port < 0 || Port > 65535)
                    throw new ArgumentOutOfRangeException(nameof(Port), "Port must be between 0 and 65535");

                listener = new HttpListener();
                listener.Prefixes.Add($"http://{Host}:{Port}/");
                listener.Start();
                Log($"Server running on port: {Port}", Level.INFO);
            }
            catch (Exception e)
            {
                Log($"Failed to start server: {e.Message}", Level.ERROR);
                throw;
            }
        }

        public async Task Start()
        {
            if (!Online)
            {
                Log($"Server initialization error: {Host}:{Port}", Level.ERROR);
                return;
            }

            try
            {
                while (!cancellationTokenSource.IsCancellationRequested)
                {
                    try
                    {
                        var context = await listener.GetContextAsync().ConfigureAwait(false);
                        if (context.Request.IsWebSocketRequest)
                        {
                            _ = Task.Run(() => ProcessWebSocketRequest(context));
                        }
                        else
                        {
                            await HandleNonWebSocketRequest(context).ConfigureAwait(false);
                        }
                    }
                    catch (Exception ex) when (!cancellationTokenSource.IsCancellationRequested)
                    {
                        Log($"Error accepting client: {ex.Message}", Level.ERROR);
                    }
                }
            }
            catch (ObjectDisposedException)
            {
                
            }
        }

        private async Task HandleNonWebSocketRequest(HttpListenerContext context)
        {
            try
            {
                context.Response.StatusCode = 400;
                context.Response.ContentType = "text/plain";
                var responseBytes = Encoding.UTF8.GetBytes("WebSocket connections only");
                await context.Response.OutputStream.WriteAsync(responseBytes, cancellationTokenSource.Token);
            }
            finally
            {
                context.Response.Close();
            }
        }

        private async Task ProcessWebSocketRequest(HttpListenerContext context)
        {
            WebSocket? webSocket = null;
            Client? client = null;

            try
            {
                WebSocketContext webSocketContext = await context.AcceptWebSocketAsync(subProtocol: null).ConfigureAwait(false);

                webSocket = webSocketContext.WebSocket;
                client = new Client(webSocket, context.Request.RemoteEndPoint.ToString());

                if (clients.TryAdd(client.ID, client))
                {
                    try
                    {
                        OnClientConnected(client);
                        await HandleWebSocketMessages(client).ConfigureAwait(false);
                    }
                    finally
                    {
                        RemoveClient(client);
                    }
                }
                else
                {
                    Log($"Failed to add client: {client.ID}", Level.ERROR);
                    await webSocket.CloseAsync(
                        WebSocketCloseStatus.InternalServerError,
                        "Server error",
                        CancellationToken.None).ConfigureAwait(false);
                }
            }
            catch (Exception ex)
            {
                Log($"WebSocket Request Handling error: {ex.Message}", Level.ERROR);
                if (client != null && !clients.ContainsKey(client.ID) && webSocket?.State == WebSocketState.Open)
                {
                    try
                    {
                        await webSocket.CloseAsync(
                            WebSocketCloseStatus.InternalServerError,
                            "Server error",
                            CancellationToken.None).ConfigureAwait(false);
                    }
                    catch{}
                }
            }
        }

        private async Task HandleWebSocketMessages(Client client)
        {
            byte[] buffer = ArrayPool<byte>.Shared.Rent(BufferSize);
            try
            {
                while (client.Socket.State == WebSocketState.Open && !cancellationTokenSource.IsCancellationRequested)
                {
                    using MemoryStream messageStream = new MemoryStream();
                    WebSocketMessageType messageType = WebSocketMessageType.Close;
                    bool endOfMessage = false;

                    while (!endOfMessage)
                    {
                        WebSocketReceiveResult result = await client.Socket.ReceiveAsync(
                            new ArraySegment<byte>(buffer),
                            cancellationTokenSource.Token).ConfigureAwait(false);

                        messageType = result.MessageType;
                        endOfMessage = result.EndOfMessage;

                        if (messageType == WebSocketMessageType.Close)
                        {
                            await client.Socket.CloseAsync(
                                WebSocketCloseStatus.NormalClosure,
                                "Client requested close",
                                cancellationTokenSource.Token).ConfigureAwait(false);
                            return;
                        }

                        await messageStream.WriteAsync(
                            buffer.AsMemory(0, result.Count),
                            cancellationTokenSource.Token).ConfigureAwait(false);

                        if (messageStream.Length > MaxMessageSize)
                        {
                            Log($"Message too large from client: {client.ID}", Level.WARNING);
                            await client.Socket.CloseAsync(
                                WebSocketCloseStatus.MessageTooBig,
                                "Message size exceeds limit",
                                cancellationTokenSource.Token).ConfigureAwait(false);
                            return;
                        }
                    }

                    if (messageType == WebSocketMessageType.Text)
                    {
                        string message = Encoding.UTF8.GetString(messageStream.ToArray());
                        await ProcessTextMessage(client, message).ConfigureAwait(false);
                    }
                    else if (messageType == WebSocketMessageType.Binary)
                    {
                        Log($"Binary{client.ID} (not processed)", Level.INFO);
                    }
                }
            }
            catch (OperationCanceledException)
            {
                
            }
            catch (WebSocketException ex)
            {
                Log($"WebSocket error for client {client.ID}: {ex.Message}", Level.ERROR);
            }
            catch (Exception ex)
            {
                Log($"Error processing messages from {client.ID}: {ex.Message}", Level.ERROR);
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(buffer);
            }
        }

        private async Task ProcessTextMessage(Client client, string message)
        {
            try
            {
                await Task.Run(() =>
                {
                    MessageReceived?.Invoke(this, new MessageReceivedEventArgs(client, message));
                }).ConfigureAwait(false);
            }
            catch (Exception ex)
            {
                Log($"Error processing message: {ex.Message}", Level.ERROR);
            }
        }

        private void RemoveClient(Client client)
        {
            if (clients.TryRemove(client.ID, out _))
            {
                OnClientDisconnected(client);
            }

            if (client.Socket.State != WebSocketState.Closed)
            {
                try
                {
                    var closeTask = client.Socket.CloseAsync(
                        WebSocketCloseStatus.NormalClosure,
                        "Server closing connection",
                        CancellationToken.None);

                    if (!closeTask.Wait(TimeSpan.FromSeconds(2)))
                    {
                        Log($"Socket close operation timed out for client: {client.ID}", Level.WARNING);
                    }
                }
                catch (Exception ex)
                {
                    Log($"Error closing socket for client {client.ID}: {ex.Message}", Level.WARNING);
                }
            }
        }

        public async Task<bool> SendTextAsync(Client target, string message)
        {
            ArgumentNullException.ThrowIfNull(target);

            if (string.IsNullOrEmpty(message))
                throw new ArgumentException("Message cannot be null or empty", nameof(message));

            if (target.Socket.State != WebSocketState.Open)
            {
                Log($"Cannot send message to client {target.ID}: WebSocket not open", Level.WARNING);
                return false;
            }

            try
            {
                byte[] messageBytes = Encoding.UTF8.GetBytes(message);
                await target.Socket.SendAsync(
                    messageBytes,
                    WebSocketMessageType.Text,
                    true,
                    cancellationTokenSource.Token).ConfigureAwait(false);
                return true;
            }
            catch (WebSocketException ex)
            {
                Log($"Error sending message to client {target.ID}: {ex.Message}", Level.ERROR);
                return false;
            }
            catch (Exception ex)
            {
                Log($"Unexpected error sending message to client {target.ID}: {ex.Message}", Level.ERROR);
                return false;
            }
        }

        public async Task BroadcastAsync(string message, Client except = null)
        {
            var tasks = new List<Task>();

            foreach (var client in clients.Values)
            {
                if (except != null && client.ID == except.ID)
                    continue;

                tasks.Add(SendTextAsync(client, message));
            }

            await Task.WhenAll(tasks).ConfigureAwait(false);
        }

        public IReadOnlyCollection<Client> GetClients() => clients.Values.ToList().AsReadOnly();

        public void Stop()
        {
            try
            {
                cancellationTokenSource.Cancel();
                var clientList = clients.Values.ToList();
                foreach (var client in clientList)
                {
                    try
                    {
                        var closeTask = client.Socket.CloseOutputAsync(
                            WebSocketCloseStatus.NormalClosure,
                            "Server is shutting down",
                            CancellationToken.None);
                        if (!closeTask.Wait(TimeSpan.FromSeconds(1)))
                        {
                            Log($"Socket close timed out for client: {client.ID}", Level.WARNING);
                        }
                    }
                    catch (Exception e)
                    {
                        Log($"Error closing client connection: {e.Message}", Level.ERROR);
                    }
                }

                clients.Clear();
                if (listener?.IsListening == true)
                {
                    listener.Stop();
                    listener.Close();
                }

                Log("Server stopped", Level.INFO);
            }
            catch (Exception ex)
            {
                Log($"Error stopping server: {ex.Message}", Level.ERROR);
            }
        }

        protected virtual void OnClientConnected(Client client)
        {
            try
            {
                ClientConnected?.Invoke(this, new ClientEventArgs(client));
            }
            catch (Exception ex)
            {
                Log($"Error in client connected event handler: {ex.Message}", Level.ERROR);
            }
        }

        protected virtual void OnClientDisconnected(Client client)
        {
            try
            {
                ClientDisconnected?.Invoke(this, new ClientEventArgs(client));
            }
            catch (Exception ex)
            {
                Log($"Error in client disconnected event handler: {ex.Message}", Level.ERROR);
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (isDisposed)
                return;

            if (disposing)
            {
                Stop();
                cancellationTokenSource?.Dispose();
                listener?.Close();
            }

            cancellationTokenSource = null;
            listener = null;
            isDisposed = true;
        }

        ~Server()
        {
            Dispose(false);
        }
    }
}
