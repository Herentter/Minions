#include "TransportProtocol.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stddef.h>

bool TransportProtocol::TCPLink(char *IPaddr, int Port) {
	this->IPaddr = IPaddr;
	this->IPconfiged = true;
	this->Port = Port;
	return TCPLink();
}

bool TransportProtocol::TCPLink(char *IPaddr) {
	this->IPaddr = IPaddr;
	this->IPconfiged = true;
	return TCPLink();
}

bool TransportProtocol::TCPLink() {
    if(!this->IPconfiged)
	    return false;//IP address of server must be set firstly

    struct sockaddr_in servaddr;
    
	bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	inet_pton(AF_INET,this->IPaddr,&servaddr.sin_addr);
    servaddr.sin_port = htons(this->Port);

    if(connect(this->sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
	    return false;//Faill to link
    }
    this->Linkemode="TCP";
	return true;
}

bool TransportProtocol::TCPsend(char *data) {
    cout<<"Plan to send: "<<data<<endl;
    /*
	    ע�⣬����ٶ����͵�����Ϊû�����ͷ��Ϣ������������
		��ˣ��ڷ���֮ǰ��Ҫ���ͷ��Ϣ
		��������״̬����ΪTCP

		Head was not inserted yet.
	*/
    char *buf = (char*)malloc(this->_MAX_SIZE*sizeof(char));
	for(int i=0;i<this->_MAX_SIZE;i++)
	    buf[i] = 10;
	int count = 0;
	for(;data[count] != '\n';count++);
	write(this->sockfd,data,count);
    return true;
}	

char *TransportProtocol::TCPReceive(int connfd,int &datalen) {
    char *buf;
    buf = (char*)malloc(this->_MAX_SIZE*sizeof(char));
    datalen = read(connfd,buf,this->_MAX_SIZE);
  
    return buf;
}

//This function will return a port,users must save them by them self.
int TransportProtocol::TCPAcceptor() {
    struct sockaddr_in cliaddr;
    socklen_t cliaddr_len;
    cliaddr_len = sizeof(cliaddr);
    int connfd;
    connfd = accept(this->sockfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
    return connfd;
}

bool TransportProtocol::TCPStartServer(int Port) {
    this->Port = Port;
    return TCPStartServer();
}

bool TransportProtocol::TCPStartServer() {
    //change listenfd to this->sockfd
    struct sockaddr_in servaddr;
    socklen_t cliaddr_len;

    //int listenfd = socket(AF_INET,SOCK_STREAM,0);

    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(this->Port);

    bind(this->sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    listen(this->sockfd,20);

    return true;
}

bool TransportProtocol::closeSocket() {
    /*
	    ������״̬����Ϊbreak
	*/
	close(this->sockfd);
	this->Linkemode="Break";
	return true;
}


bool TransportProtocol::UDPsend(char *Data, char *IPaddr, int Port) {}
bool TransportProtocol::UDPSend(char *Data) {}
char *TransportProtocol::UDPReceive() {}
bool TransportProtocol::UDPStartServer(int Port) {}	
bool TransportProtocol::UDPStartServer() {}

/*
    ��ȡ���ݰ����ߴ�
	Get the max size of data packet
*/
int TransportProtocol::getMaxDataSize() {
	return _MAX_SIZE;
}

/*
    ���÷�������ַ
	Set IP address if server
*/
bool TransportProtocol::setPort(int Port) {
	this->Port = Port;
	return true;
}

/*
    ���ö˿ں�
	Set port num of server
*/
bool TransportProtocol::setAddr(int Port) {
	this->Port = Port;
	return true;
}

/*
    ��ȡ������IP��ַ����ֵΪ0.0.0.0
	Get IP address of server��default:0.0.0.0)
*/
char *TransportProtocol::getIPaddr() {
	return IPaddr;
}

/*
    ��ȡ�������˿ںţ�Ĭ��9526��
	Get port num of server(default 9526)
*/
int TransportProtocol::getPort() {
	return Port;
}

/*
    Not Recommend
    �޸�������ݰ��ߴ�
	Change max size of data packet
*/
bool TransportProtocol::_setMaxDataSize(int MaxSize) {
	this->_MAX_SIZE = MaxSize;
	return true;
}

//���캯��
//Constructor
TransportProtocol::TransportProtocol() {
	this->_MAX_SIZE = 80;
	this->IPaddr = "0.0.0.0";
	this->Port = 8000;
	this->sockfd = socket(AF_INET,SOCK_STREAM,0);
	this->IPconfiged = false;
}

//Private:
/*
    ����ͷ��Ϣ
*/
int TransportProtocol::generateHeadInfor(char *data) {
	return data[0] + data[4];
}
