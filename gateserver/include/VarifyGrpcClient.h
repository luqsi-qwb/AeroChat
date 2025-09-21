#pragma once
//grpc中的客户端
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"const.h"
#include"Singleton.h"


using grpc::Channel;
using grpc:: Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;


class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port);

	~RPConPool();

	void Close();

	std::unique_ptr<VarifyService::Stub> getConnection();

	void returnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


class VarifyGrpcClient:public Singleton<VarifyGrpcClient>
{
	friend class Singleton<VarifyGrpcClient>;
public:
	//这个函数接口是提供getserver使用的，用来验证验证码是否发送到邮箱，这个只是grpc给上层的接口
	GetVarifyRsp GetVarifyCode(std::string email) {
		//客户端的一个上下文
		ClientContext context;
		//还要设置一个回包
		GetVarifyRsp reply;

		GetVarifyReq request;
		request.set_email(email);
		auto stub = pool_->getConnection();
		//发送请求
		Status status = stub->GetVarifyCode(&context, request, &reply);
		if (status.ok()) {
			pool_->returnConnection(std::move(stub));
			return reply;
		}
		else {
			pool_->returnConnection(std::move(stub));
			reply.set_error(ErrorCodes::RPCFailed);
			return reply;
		}
		
	}

private:
	VarifyGrpcClient();
	//一种媒介，只有通关他才可以和别人通信,后续改为多线程模式下就需要多个这个媒介，因此也将这个媒介改为线程池
	std::unique_ptr<RPConPool> pool_;
};

