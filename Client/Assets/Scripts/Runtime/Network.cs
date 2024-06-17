// #define ENABLE_AKIRA_LOG
using System;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace AKIRA.Net {
    public class Network : IDisposable {

        // ip
        public string IP { get; private set; }
        // port
        public int Port { get; private set; }

        // client
        private TcpClient client;
        private NetworkStream stream;

        /// <summary>
        /// Disconnect and reconnected time, million seconds
        /// </summary>
        public static int WaitConnectMillionSeconds = 3000;

        /// <summary>
        /// send and receive buff size
        /// </summary>
        private const int MaxBufferSize = 4096;
        /// <summary>
        /// connect time out, million seconds
        /// </summary>
        private const int Timeout = 3000;

        /// <summary>
        /// is connected
        /// </summary>
        public bool IsConnected => client != null && client.Connected;

        private static bool isTryConnected = false;


        public Network(string ip, int port) {
            IP = ip;
            Port = port;
        }

        /// <summary>
        /// Connect, auto connect when new()
        /// </summary>
        /// <returns></returns>
        public async Task<Network> Connect() {
            lock (this) {
                if (isTryConnected)
                    return default;
                isTryConnected = true;
            }

            try {
                if (!IsConnected) {
                    client = new(IP, Port) {
                        SendBufferSize = MaxBufferSize,
                        ReceiveBufferSize = MaxBufferSize,
                        SendTimeout = Timeout,
                        ReceiveTimeout = Timeout,
                    };
                    stream = client.GetStream();
                    Log($"Connect server {IP}:{Port} successfull");
                    isTryConnected = false;
                }
            } catch {
                isTryConnected = false;
                Disconnect();
                await Task.Delay(WaitConnectMillionSeconds);
                Log($"Try to connect {IP}:{Port}");
                await Connect();
            }

            return this;
        }

        /// <summary>
        /// Disconnect
        /// </summary>
        public void Disconnect() {
            stream?.Close();
            client?.Close();
            Log($"Disconnect Server {IP}:{Port}");
        }

        public async Task<Network> Send(string buffer) => await Send(Encoding.UTF8.GetBytes(buffer));
        public async Task<Network> Send(byte[] buffer) {
            try {
                await stream.WriteAsync(buffer, 0, buffer.Length);
                await stream.FlushAsync();
                Log("Send Server buffer successfully");
            } catch {
                Disconnect();
                Log("Try reconnect server and send buffer...");
                await (await Connect())?.Send(buffer);
            }

            return this;
        }

        public async Task<byte[]> Read() {
            byte[] buffer = new byte[MaxBufferSize];
            byte[] res = default;
            try {
                var length = await stream.ReadAsync(buffer, 0, buffer.Length);
                // retur buffer caused error: InvalidProtocolBufferException: Protocol message contained an invalid tag (zero).
                // create a new byte[] and copy the right length of buffer
                res = new byte[length];
                Array.Copy(buffer, res, length);
            } catch {
                Disconnect();
                Log("Try reconnect server and read buffer...");
                await (await Connect())?.Read();
            }

            // return Encoding.UTF8.GetString(buffer);
            return res;
        }

        public void Dispose() {
            Disconnect();
            isTryConnected = true;
            stream = null;
            client = null;
        }

        private void Log(object msg) {
#if ENABLE_AKIRA_LOG
#else
            UnityEngine.Debug.Log(msg);
#endif
        }
    }
}