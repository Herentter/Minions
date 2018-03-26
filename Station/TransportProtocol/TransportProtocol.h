#pragma once
#include <iostream>

using namespace std;

/*
        Following notes are Chinese for Windows:

        传输协议由两部分组成
	分别是确认字段和正文
	确认字段为第一部分，为本机IP地址
	正文字段为第二部分，为条形码内容
	确认字段与正文字段之间以英文逗号（ASCII:44)分隔
	报文长度可能不确定，但肯定以  ASCII:10  结尾
	协议中 _MAX_SIZE 变量标记了数据包的最大长度
	超过最大长度的数据将被丢弃
	_MAX_SIZE 默认值为100
	协议默认端口为9526


	English Notes:

	Data packet includs two parts
	which are confirm words and main information
	The first part of data packet is confirm words
	which is the IP address of packet sender
	The second part of packet is main data
	which is the information of the bar code.
	Confirm words and main information are seperated by English comma(ASCII:44)
	The length of data packet may be vary , but must end up with ASCII:10
	_MAX_SIZE marked the max length of data
	datas that our range of _MAX_SIZE will be droped

	default value of _MAX_SIZE is 1000
	default port of this protocol is 9526
*/
class TransportProtocol {
private:
/*
	_MAX_SIZE 表示数据包的最大长度，单位为字节
	这个值不宜小于70
	因为二维码信息的长度大概在50个字节左右
	协议头需占用不多于16个字节
	因此，最大长度最好在70以上

	_MAX_SIZE means max length of data packet
	better don't make it lower than 70
	default value of _MAX_SIZE is 100

        TCP portocol will offer every client a idependent port to transport data
        this tool box will not save those new generated port
        users must save them by themself
*/
        int _MAX_SIZE;

        string data;//数据包正文字段
        string conf;//数据包确认字段

        char *IPaddr;//服务器IP地址
        int Port;//default value : 9526
        int sockfd;
        string Linkemode;//UDP,TCP,break
        int generateHeadInfor(char *data);//初始化本地IP地址
        bool IPconfiged;
public:
    //Common functions
    TransportProtocol();//Initialization _MAX_SIZE=100 bool-IPseted and bool-Portseted,and local IP address
    bool closeSocket();
    int getMaxDataSize();
    char *getIPaddr();
    int getPort();

    //TCP functions
    bool TCPLink(char *IPaddr, int Port);
    bool TCPsend(char *data);
    int TCPAcceptor();//This function will return a port,users must save them by them self.
    char *TCPReceive(int connfd,int &datalen);//接收数据
	bool TCPStartServer(int Port);
	bool TCPStartServer();

    //UDP functions
    bool UDPsend(char *Data, char *IPaddr, int Port);
    char *UDPReceive();//接收数据	
	bool UDPStartServer(int Port);	
	bool UDPStartServer();


/*
        Following notes are Chinese for Windows

	这里给出了TCP和UDP链接的重载方法
	这些方法存在的意义是提供更加多样化的初始化方案
	方便团队中其他成员的使用


	English Notes

	Here are extra function for TCP and UDP links
	for peosonization use

*/
    bool setPort(int Port);
    bool setAddr(int Port);
    bool TCPLink(char *IPaddr);
    bool TCPLink();
    bool UDPSend(char *Data);
    bool _setMaxDataSize(int MaxSize);//Usage is Not Recommend
};
