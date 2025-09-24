#include "Logger.h"
#include<iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;


namespace AeroChat {
	std::shared_ptr<spdlog::logger> Logger::s_logger;//静态变量需要初始化一样

	//获取实例
	std::shared_ptr<spdlog::logger> Logger::GetInstance() {
		return s_logger;//静态成员函数只可以访问静态成员变量
	}

	//日志的初始化
	void Logger::Init(const std::string& servicename) {
		try {
			
			//日志目录名称
			std::string logDir = "logs" + servicename;
			if (!fs::exists(logDir)) {//如果当前目录不存在便创建目录
				fs::create_directories(logDir);
			}

			//设置异步日志队列：队列大小，线程数量
			spdlog::init_thread_pool(8192, 2);

			//控制台sink(彩色)
			auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
			console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

			//文件sink(每天切割)
			auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
				logDir+"/"+servicename+".log",0,0);

			file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

			//组合sink
			std::vector < spdlog::sink_ptr > sinks{ console_sink,file_sink };//日志既输出在终端也输出在文件当中

			//创建异步logger
			s_logger = std::make_shared < spdlog::async_logger>(
				servicename,
				sinks.begin(),sinks.end(),
				spdlog::thread_pool(),
				spdlog::async_overflow_policy::overrun_oldest//当日志插入速度大于日志写的速度就将老的日志删除
			);

			s_logger->set_level(spdlog::level::debug);
			s_logger->flush_on(spdlog::level::warn);

			s_logger->info("===== {} started (async logging enabled) =====", servicename);

		}
		catch (const spdlog::spdlog_ex& ex) {
			std::cout << "log init faild：" << ex.what() << std::endl;
		}

	}

}
