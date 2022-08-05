using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MS461xx_RawSockets_example
{
    interface SCPIConnection
    {
        void Connect(int timeOutMs);
        void SetTimeout(int timeOutMs);
        void Write(string wCommand);
        string Query(string qCommand);
        void Disconnect();
        
    }
}
