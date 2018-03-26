#pragma once
#include <iostream>

using namespace std;

/*
        Following notes are Chinese for Windows:

        ����Э�������������
	�ֱ���ȷ���ֶκ�����
	ȷ���ֶ�Ϊ��һ���֣�Ϊ����IP��ַ
	�����ֶ�Ϊ�ڶ����֣�Ϊ����������
	ȷ���ֶ��������ֶ�֮����Ӣ�Ķ��ţ�ASCII:44)�ָ�
	���ĳ��ȿ��ܲ�ȷ�������϶���  ASCII:10  ��β
	Э���� _MAX_SIZE ������������ݰ�����󳤶�
	������󳤶ȵ����ݽ�������
	_MAX_SIZE Ĭ��ֵΪ100
	Э��Ĭ�϶˿�Ϊ9526


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
	_MAX_SIZE ��ʾ���ݰ�����󳤶ȣ���λΪ�ֽ�
	���ֵ����С��70
	��Ϊ��ά����Ϣ�ĳ��ȴ����50���ֽ�����
	Э��ͷ��ռ�ò�����16���ֽ�
	��ˣ���󳤶������70����

	_MAX_SIZE means max length of data packet
	better don't make it lower than 70
	default value of _MAX_SIZE is 100

        TCP portocol will offer every client a idependent port to transport data
        this tool box will not save those new generated port
        users must save them by themself
*/
        int _MAX_SIZE;

        string data;//���ݰ������ֶ�
        string conf;//���ݰ�ȷ���ֶ�

        char *IPaddr;//������IP��ַ
        int Port;//default value : 9526
        int sockfd;
        string Linkemode;//UDP,TCP,break
        int generateHeadInfor(char *data);//��ʼ������IP��ַ
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
    char *TCPReceive(int connfd,int &datalen);//��������
	bool TCPStartServer(int Port);
	bool TCPStartServer();

    //UDP functions
    bool UDPsend(char *Data, char *IPaddr, int Port);
    char *UDPReceive();//��������	
	bool UDPStartServer(int Port);	
	bool UDPStartServer();


/*
        Following notes are Chinese for Windows

	���������TCP��UDP���ӵ����ط���
	��Щ�������ڵ��������ṩ���Ӷ������ĳ�ʼ������
	�����Ŷ���������Ա��ʹ��


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
