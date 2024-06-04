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
        public int WaitConnectMillionSeconds = 3000;

        /// <summary>
        /// send and receive buff size
        /// </summary>
        private const int MaxBufferSize = 8192;
        /// <summary>
        /// connect time out, million seconds
        /// </summary>
        private const int Timeout = 3000;

        /// <summary>
        /// is connected
        /// </summary>
        public bool IsConnected => client != null && client.Connected;

        public Network(string ip, int port) {
            IP = ip;
            Port = port;
        }

        /// <summary>
        /// Connect, auto connect when new()
        /// </summary>
        /// <returns></returns>
        public async Task<Network> Connect() {
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
                }
            } catch {
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
            } catch {
                Disconnect();
                var network = await Connect();
                await network.Send(buffer);
            }

            return this;
        }

        public void Dispose() {
            Disconnect();
        }

        private void Log(object msg) {
#if ENABLE_AKIRA_LOG
#else
            UnityEngine.Debug.Log(msg);
#endif
        }
    }
}