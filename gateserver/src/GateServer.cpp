//网关服务器
#include <iostream>
#include"Cserver.h"
#include"ConfigMgr.h"
#include"const.h"
#include"RedisMgr.h"
#include"MysqlMgr.h"


int main()
{
    try
    {
        RedisMgr::GetInstance();
        MysqlMgr::GetInstance();
        auto& gCfgMgr = ConfigMgr::Inst();
        std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
        unsigned short gate_port = atoi(gate_port_str.c_str());
        net::io_context ioc{ 1 };
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop();
            });
        std::make_shared<CServer>(ioc, gate_port)->Start();
        ioc.run();
        RedisMgr::GetInstance()->Close();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        RedisMgr::GetInstance()->Close();
        return EXIT_FAILURE;
    }
}
