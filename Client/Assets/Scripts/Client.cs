using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class Client : MonoBehaviour {
    public const string IP = "127.0.0.1";
    public const int Port = 6000;

    public string message;

    public SocketTcp socketTcp;

    private void Start() {
        Debug.Log("Start");
    }

    [ContextMenu("Send")]
    private void Send() {
        //创建socket
        var tcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        //连接服务器
        tcpSocket.Connect(IPAddress.Parse(IP), Port);
        Debug.Log("连接服务器");
        // //接收消息
        // byte[] bt = new byte[1024];
        // int messgeLength = tcpSocket.Receive(bt);
        // Debug.Log(Encoding.UTF8.GetString(bt));
        //发送消息
        Debug.Log($"发送消息 {message}");
        tcpSocket.Send(Encoding.UTF8.GetBytes(message));
        // //接收消息
        // byte[] bt = new byte[1024];
        // int messgeLength = tcpSocket.Receive(bt);
        // Debug.Log($"接受消息 {Encoding.UTF8.GetString(bt)}");
        
        tcpSocket.Disconnect(false);
    }


    [ContextMenu("Send2")]
    private void Send2() {
        TcpClient client = new(IP, Port);
        var stream = client.GetStream();
        var msg = Encoding.UTF8.GetBytes(message);
        stream.Write(msg, 0, msg.Length);
        stream.Close();
        client.Close();
    }
}
