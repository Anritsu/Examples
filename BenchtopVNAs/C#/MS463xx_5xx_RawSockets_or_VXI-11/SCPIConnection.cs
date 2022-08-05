using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MS463xx_5xx_RawSockets_or_VXI_11_ex
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
