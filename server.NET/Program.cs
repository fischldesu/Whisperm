using server.src;

Console.WriteLine("\n");
Console.WriteLine("          fischldesu/whisperm.server");
Console.WriteLine("     WebSocket Server v0.0.1p1 .NET " + Environment.Version);
Console.WriteLine("\n");

int port = Server.DefaultPort;
foreach (string arg in args)
{
    if (arg.StartsWith("--port="))
        port = int.Parse(arg[7..]);
    else if(arg.StartsWith("-p="))
        port = int.Parse(arg[3..]);

}

Messager messager = new(new Server(port));
await messager.Server.Start();
messager.Server.Stop();
