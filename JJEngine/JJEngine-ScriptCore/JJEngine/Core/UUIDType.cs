using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine
{
    public struct UUIDType
    {
        public UInt64 LowBytes;
        public UInt64 HighBytes;

        public UUIDType(UInt64 low, UInt64 high)
        {
            LowBytes = low;
            HighBytes = high;
        }

        public byte[] ToBytes()
        {
            byte[] bytes=new byte[16];

            Buffer.BlockCopy(BitConverter.GetBytes(LowBytes), 0, bytes, 0, 8);
            Buffer.BlockCopy(BitConverter.GetBytes(HighBytes), 0, bytes, 8, 8);
            return bytes;
        }

        public override string ToString()
        {
            byte[] bytes = ToBytes();
            string plain = BitConverter.ToString(bytes).Replace("-", string.Empty).ToLower();
            string uuid = 
                $"{plain.Substring(0, 8)}" +
                $"-{plain.Substring(8, 4)}" +
                $"-{plain.Substring(12, 4)}" +
                $"-{plain.Substring(16, 4)}" +
                $"-{plain.Substring(20, 12)}";
            return uuid;
        }
    }
}
