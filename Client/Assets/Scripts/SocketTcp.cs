using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class TransData {
    public TransData(Action<bool, string> action, int buffsize = 8192) {
        recvbuff = new byte[1000000];
        tmpbuff = new byte[buffsize];
        istrans = false;
        actionResult = action;
        recvsize = 0;
    }

    public byte[] tmpbuff;
    public byte[] recvbuff;
    public int recvsize;
    public bool istrans;
    public Action<bool, string> actionResult;
}

public class SocketTcp : MonoBehaviour {
    private TcpClient client;
    private NetworkStream stream;

    public const string IP = "127.0.0.1";
    public const int Port = 6000;

    public const int MaxBuffer = 8192;

    public SocketTcp Connect() {
        try {
            if (client == null || !client.Connected) {
                client = new(new IPEndPoint(IPAddress.Parse(IP), Port))
                {
                    ReceiveBufferSize = MaxBuffer,
                    SendBufferSize = MaxBuffer
                };
                stream = client.GetStream();
            }
        } catch {
            client.Close();
            client.Dispose();
            Connect();
        }

        return this;
    }

    public SocketTcp Send(string msg = "") {
        try {
            if (stream.CanWrite) {
                var sendBytes = Encoding.UTF8.GetBytes(msg);
                stream.Write(sendBytes, 0, sendBytes.Length);
                stream.Flush();
            }
        } catch {
            client.Close();
            client.Dispose();
            Connect().Send(msg);
        }

        return this;
    }

    public SocketTcp Recv(Action<bool, string> action = null) {
        TransData trans = new TransData(action, client.ReceiveBufferSize);
        try {
            stream.BeginRead(trans.tmpbuff, 0, trans.tmpbuff.Length, TcpDataReceived, trans);
        } catch {
            client.Close();
            client.Dispose();
            Connect().Recv(action);
        }
        return this;
    }

    private void TcpDataReceived(IAsyncResult ar) {
        int recv = 0;
        TransData transData = (TransData)ar.AsyncState;
        try {
            recv = stream.EndRead(ar);
        } catch {
            recv = 0;
        }

        //判断接收数为0，并且未在接收中
        if (recv == 0 && !transData.istrans)
        {
            transData = new TransData(transData.actionResult, client.ReceiveBufferSize);
            stream.BeginRead(transData.tmpbuff, 0, transData.tmpbuff.Length,
                TcpDataReceived, transData);
        }
        //已在接收了，再次接收为0，说明接收完了，直接调用
        else if (recv == 0)
        {
            //执行回调函数
            string resstr = Encoding.UTF8.GetString(transData.recvbuff);
            transData.actionResult(transData.istrans, resstr);
        }
        //如果recv大于0,说明接收到了数据，修改接收值
        else
        {
            transData.istrans = true;

            byte[] buff = new byte[recv];
            Buffer.BlockCopy(transData.tmpbuff, 0, buff, 0, recv);
            Buffer.BlockCopy(buff, 0, transData.recvbuff, transData.recvsize, recv);
            transData.recvsize += recv;

            if (recv < transData.tmpbuff.Length)
            {
                //执行回调函数
                string resstr = Encoding.UTF8.GetString(transData.recvbuff);
                int strlen = resstr.IndexOf('\0');
                if (strlen > 0)
                {
                    resstr = resstr.Substring(0, strlen);
                }
                transData.actionResult(transData.istrans, resstr);
                //执行完回调后重新初始化接收参数
                transData = new TransData(transData.actionResult, client.ReceiveBufferSize);
            }

            stream.BeginRead(transData.tmpbuff, 0, transData.tmpbuff.Length,
                TcpDataReceived, transData);
        }
    }
}
