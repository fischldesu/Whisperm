using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace server.src.utils
{
    public class Logger
    {
        public enum Level
        {
            INFO,
            WARNING,
            ERROR,
        }
        public static void Log(string logMessage, Level level = Level.INFO)
        {
            Console.WriteLine($"{DateTime.Now} [{level}] {logMessage}");
        }
    }
}
