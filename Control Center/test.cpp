#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include"TransportProtocol.h"
#include"DatabaseProtocol.h"
//#include"TransportProtocol/TransportProtocol.h"
//#include"DatabaseProtocol/DatabaseProtocol.h"

using namespace std;

void SocketTest(){
    string i;
    TransportProtocol TP;
	cout<<"Server starting"<<endl;
    TP.TCPStartServer();
	cout<<"Server Lestening"<<endl;
    int connfd = TP.TCPAcceptor();
	cout<<"connfd:"<<connfd<<endl;
	int n;
	while(1){
	    cout<<"Server is reading"<<endl;
        cout<<TP.TCPReceive(connfd,n)<<endl;
		cout<<"n is "<<n<<endl;
	}
	cin>>i;
	TP.closeSocket();
}

void DBTest(){
    DatabaseProtocol DB;
    DB.LinkDB("mysqlpptest");
    bool b;
    if(DB.OrdernumConferm("926752379919"))
        b = DB.OrdernumWrite("926752379919",1);
    cout<<"DataInsert :"<<b<<endl;
}

void *thread(void *arg)
{
        int connfd = *(int *)arg;
        TransportProtocol TP;
    	DatabaseProtocol DB;
    	DB.LinkDB("mysqlpptest");
	int n;
    	bool b;
	while(1){
	    	cout<<"Server is reading"<<endl;
                //Warning: potential memory leakage risk
        	char *rec = TP.TCPReceive(connfd,n);
		cout<<"rec :"<<rec<<endl;
		cout<<"n is "<<n<<endl;
                int stationid = (int)(rec[0]-48);
                string ordernum;
                for(int i=2;i<n;i++)
                    if(rec[i]>31 && rec[i]<127)
                        ordernum += rec[i];
		//ordernum += '\0';//do not make it end up with '\n'
		cout<<"ordernum is "<<ordernum<<endl;
		cout<<"stationid is "<<stationid<<endl;
		b = false;
                if(DB.OrdernumConferm(ordernum)){
    		        cout<<"Ordernum Confermed"<<endl;
        		b = DB.OrdernumWrite(ordernum,stationid);
                }
    		cout<<"DataInsert :"<<b<<endl;
	}
    return 0;
}

void CCTest(){
    	string i;
    	TransportProtocol TP;
	cout<<"Server starting"<<endl;
    	TP.TCPStartServer();
        while(1){
	    cout<<"Server Lestening"<<endl;
            int connfd = TP.TCPAcceptor();
	    cout<<"connfd:"<<connfd<<endl;
            pthread_t id;
            pthread_create(&id, NULL, thread, &connfd);
        }
	cin>>i;
	TP.closeSocket();
}

int main(int argc,char *argv[]){
    CCTest();
    return 0;
}
