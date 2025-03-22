using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using server.src.Data;
using static server.src.utils.Logger;

namespace server.src.utils
{
    internal class MessageAnalyzer
    {
        private readonly Message message;
        public Client Target { get; private set; }
        public JsonObject Data { get; private set; }
        public Message.MessageType Type { get; private set; }
        public string Attach { get; private set; }
        public bool MessageForward_OK { get; private set; } = false;

#pragma warning disable CS8618
        private MessageAnalyzer(Message source)
#pragma warning restore CS8618
        {
            message = source;
        }

        private async Task AnalyzeMessage()
        {
            switch(message.Type)
            {
                case Message.MessageType.MSG:
                    await Message_Forward();
                    break;
                case Message.MessageType.LOGIN:
                    await Message_Login(true);
                    break;
                case Message.MessageType.LOGOUT:
                    await Message_Login(false);
                    break;
                case Message.MessageType.REGISTER:
                    await Message_Register();
                    break;
            }
        }

        private async Task Message_Forward()
        {
            Log("_fwd");
        }

        private async Task Message_Login(bool login)
        {
            Target = message.Sender;
            Log("_login");
        }

        private async Task Message_Register()
        {
            Target = message.Sender;
            Log("_reg");
        }

        public static async Task<MessageAnalyzer> Analyze(Message message)
        {
            var ins = new MessageAnalyzer(message);
            await ins.AnalyzeMessage();
            return ins;
        }
    }
}
