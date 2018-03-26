#include"DatabaseProtocol.h"

using conn_ptr = std::unique_ptr<mysqlpp::connection>;

conn_ptr DatabaseProtocol::GenerateLink(){
        mysql_library_init(0, 0, 0);
        atexit(mysql_library_end);
    	conn_ptr c(new mysqlpp::connection(this->IPaddr, this->user, this->pwd, this->dbname));
        return c;
}

bool DatabaseProtocol::linkReturn(){
        this->conn = GenerateLink();
        return true;
}

int DatabaseProtocol::selectHoleLocationnum(string ordernum){
    //select DB and return the best HoleLocationnum
    auto stmt = this->conn->prepare("select * from locationmapping,ordermapping where locationmapping.reallocation = ordermapping.reallocation and ordernum = ?");
    stmt->param(ordernum);
    //cout<<"ordernum :"<<ordernum<<endl;
    auto rs = stmt->query();
    //cout<<"rs->num_rows() : "<<rs->num_rows()<<endl;
    if(rs->num_rows() ==0)
        return -1;
    rs->fetch();//select result has a head node,so we need to go to next node firstly.

    return rs->get<uint32_t>("holelocation");
}

DatabaseProtocol::DatabaseProtocol(){
        //Default SQL information
        this->IPaddr = "127.0.0.1";
        this->user = "root";
        this->pwd = "000000";
        this->dbname = "mysqlpptest";
}

bool DatabaseProtocol::LinkDB(string IPaddr,string user,string pwd,string dbname){
        this->IPaddr = IPaddr;
        this->user = user;
        this->pwd = pwd;
        this->dbname = dbname;
        return linkReturn();
}

bool DatabaseProtocol::OrdernumWrite(string ordernum,int stationid){
    //insert reigon,ordernum,stationid and locationid into DB

    /**
     *    First step:
     *    Select region by ordernum
     **/
    cout<<"ordernum select:|"<<ordernum<<"|end|"<<endl;
    cout<<"stationid select:|"<<stationid<<"|end|"<<endl;
    auto stmt = this->conn->prepare("select reallocation from ordermapping where ordernum = ?");
    stmt->param(ordernum);
    //cout<<"ordernum :"<<ordernum<<endl;
    auto rs = stmt->query();
    //cout<<"rs->num_rows() : "<<rs->num_rows()<<endl;
    if(rs->num_rows() ==0)
        return false;//fail to find the reigon
    rs->fetch();
    string reigon = rs->get<std::string>(0);
    cout<<"find the reigon : "<<reigon<<endl;
    int holelocation = this->selectHoleLocationnum(ordernum);
    stmt = this->conn->prepare("insert into transportationinfor(ordernum,statu,stationid,robotid,holelocation,region) values (?,0,?,0,?,?)");
    stmt->param(ordernum);
    stmt->param(stationid);
    stmt->param(holelocation);
    stmt->param(reigon);
    //Insert data
    uint64_t affected_rows = stmt->execute();
    if(affected_rows == 0)
        return false;//fale to insert data
    cout<<"insert succed"<<endl;
    return true;
}

bool DatabaseProtocol::OrdernumConferm(string ordernum){
    //select DB to conferm whether given ordernum exist
    cout<<"Ordernum Conferm:|"<<ordernum<<"|end|"<<endl;
    auto stmt = this->conn->prepare("select count(holelocation) from locationmapping,ordermapping where locationmapping.reallocation = ordermapping.reallocation and ordernum = ?");
    stmt->param(ordernum);
    //cout<<"ordernum :"<<ordernum<<endl;
    auto rs = stmt->query();
    //cout<<"rs->num_rows() : "<<rs->num_rows()<<endl;
    cout<<"rs->num_rows() is "<<rs->num_rows()<<endl;
    if(rs->num_rows() ==0)
        return false;
    cout<<"rs->num_rows() is not 0"<<endl;
    rs->fetch();//select result has a head node,so we need to go to next node firstly.
    cout<<"rs->get<uint32_t>(0) is "<<rs->get<uint32_t>(0)<<endl;
    if(rs->get<uint32_t>(0)==0)
        return false;
    cout<<"count holelocation succed"<<endl;
    return true;
}

bool DatabaseProtocol::LinkDB(string dbname){
        this->dbname = dbname;
        return linkReturn();
}
