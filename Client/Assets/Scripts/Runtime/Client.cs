using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Google.Protobuf;
using Protobuf;
using TMPro;
using UnityEngine;

public class Client : MonoBehaviour {
    [Header("UI")]
    public TMP_InputField ipInput;
    public TMP_InputField portInput;

    [Header("Value")]
    public string IP = "127.0.0.1";
    public int Port = 6000;
    public string message;

    private AKIRA.Net.Network network;

    private async void Start() {
        network = new(IP, Port);
        await network.Connect();
    }

    // [ContextMenu("Send")]
    // private void Send() {
    //     //创建socket
    //     var tcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    //     //连接服务器
    //     tcpSocket.Connect(IPAddress.Parse(IP), Port);
    //     Debug.Log("连接服务器");
    //     // //接收消息
    //     // byte[] bt = new byte[1024];
    //     // int messgeLength = tcpSocket.Receive(bt);
    //     // Debug.Log(Encoding.UTF8.GetString(bt));
    //     //发送消息
    //     Debug.Log($"发送消息 {message}");
    //     tcpSocket.Send(Encoding.UTF8.GetBytes(message));
    //     // //接收消息
    //     // byte[] bt = new byte[1024];
    //     // int messgeLength = tcpSocket.Receive(bt);
    //     // Debug.Log($"接受消息 {Encoding.UTF8.GetString(bt)}");
        
    //     tcpSocket.Disconnect(false);
    // }


    [ContextMenu("Send")]
    private async void Send() {
        var request = new TestRequest
        {
            Id = 10,
            Msg = message,
            Params = "this is a test params"
        };

        var buffer = await (await network.Send(request.ToByteArray())).Read();
        var response = TestRequest.Parser.ParseFrom(buffer);
        Debug.Log(response);
    }
    
    private void OnDestroy() {
        network.Dispose();
    }
}
