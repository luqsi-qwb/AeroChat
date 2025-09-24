#pragma once
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include   <memory>
//项目采取了spdlog日志库，在这里面封装一下日志库的初始化和写日志的接口


namespace AeroChat {
	class Logger {
	public:
		//初始化日志库
		static void Init(const std::string& servicename);
		//获取日志实例
		static std::shared_ptr<spdlog::logger> GetInstance();


	private:
		static std::shared_ptr<spdlog::logger> s_logger;//日志库实例
	}; 
}

// 宏封装，方便使用
#define LOG_TRACE(...)  ::AeroChat::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)  ::AeroChat::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)   ::AeroChat::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)   ::AeroChat::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  ::AeroChat::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::AeroChat::Logger::GetLogger()->critical(__VA_ARGS__)

