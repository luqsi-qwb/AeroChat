#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include<memory>
#include<iostream>
#include"Singleton.h"
#include<functional>
#include<map>
#include<unordered_map>
#include<jsoncpp/json/json.h>
#include<jsoncpp/json/value.h>
#include<jsoncpp/json/reader.h>
#include <boost/filesystem.hpp> 
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree//ini_parser.hpp>
#include<atomic>
#include<queue>
#include<mutex>
#include<condition_variable>
#include <hiredis/hiredis.h>
#include<cassert>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


enum ErrorCodes
{
    Success = 0,
    Error_Json = 1001,  //Json解析错误
    RPCFailed = 1002,  //RPC请求错误
    VarifyExpired = 1003, //验证码过期
    VarifyCodeErr = 1004, //验证码错误
    UserExit = 1005, //用户已存在
    PassWdErr = 1006,//密码错误
    EmailNotMatch = 1007,//邮箱不匹配
    PassWdUpFailed = 1008, //更新密码失败
    PassWdInvaild = 1009, //密码更新失败

};

class Defer {
public:
    //接收一个lambda表达式或者函数指针
    Defer(std::function<void()> func) : func_(func) {}

    ~Defer() {
        func_();
    }
private:
    std::function<void()> func_;
};


#define CODEPREFIX "code_"



