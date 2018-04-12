using System.Linq;
​
namespace ADCMessageParser
{
    public class Parser
    {
        public static byte[] GetSubMessage(byte[] fullMessage)
        {
            if (fullMessage == null)
            {
                return new byte[] { };
            }
            else
            {
                int strippedLength = fullMessage.Length - headerLength - checksumLength;
                if(((strippedLength != validMessageLength) && (strippedLength != validMessageLength2)) //Per Chris, the sub-messages can either be 5 or 10 bytes long
                || (fullMessage[3] != 0x77)
                || (fullMessage[4] != 0x00)
                || (fullMessage[5] != 0x0A))
                {
                    return new byte[] { };
                }
​
                byte[] output = fullMessage.Skip(6).ToArray();
                return output.Take(output.Count() - 1).ToArray();
            }
        }
​
        private const int headerLength = 3;
        private const int checksumLength = 1;
        private const int validMessageLength = 13; // 3 bytes of message type + 10 bytes of data
        private const int validMessageLength2 = 8; // 3 bytes of message type + 5 bytes of data
    }
}
