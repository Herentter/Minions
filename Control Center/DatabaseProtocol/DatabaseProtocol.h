#include <cstdlib>

#include <memory>
#include <iostream>

using namespace std;

#include "mysqlpp.hpp"

class DatabaseProtocol{
private:
    //User Data
    string IPaddr;
    string user;
    string pwd;
    string dbname;

    //Link infor
    using conn_ptr = std::unique_ptr<mysqlpp::connection>;
    conn_ptr conn;

    conn_ptr GenerateLink();
    bool linkReturn();
    int selectHoleLocationnum(string ordernum);//return the best holelocationnum
public:
    //Basic Function
    DatabaseProtocol();
    bool LinkDB(string IPaddr,string user,string pwd,string dbname);

    //Data Transform
    bool OrdernumWrite(string ordernum,int stationid);//this function will select locationnum automanticly
    bool OrdernumConferm(string ordernum);

    //Overload Function
    bool LinkDB(string dbname);

};
