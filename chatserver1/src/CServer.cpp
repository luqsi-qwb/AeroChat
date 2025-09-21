#include "CServer.h"
#include <iostream>
#include "AsioIOServicePool.h"
#include "UserMgr.h"
#include "RedisMgr.h"

CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(),port)), _timer(_io_context, std::chrono::seconds(60))
{
	cout << "Server start success, listen on port : " << _port << endl;
	////启动定时器
	//_timer.async_wait([this](boost::system::error_code ec) {
	//		on_timer(ec);
	//	});
	StartAccept();
}

CServer::~CServer() {
	cout << "Server destruct listen on port : " << _port << endl;
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		//将这个会话启动
		new_session->Start();
		lock_guard<mutex> lock(_mutex);
		//将这个会话根据uid添加进去
		_sessions.insert(make_pair(new_session->GetSessionId(), new_session));
	}
	else {
		cout << "session accept failed, error is " << error.what() << endl;
	}
	//继续监听连接
	StartAccept();
}

void CServer::StartAccept() {
	auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
	shared_ptr<CSession> new_session = make_shared<CSession>(io_context, this);
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}

//根据session 的id删除session，并移除用户和session的关联
void CServer::ClearSession(std::string session_id) {
	
	lock_guard<mutex> lock(_mutex);
	if (_sessions.find(session_id) != _sessions.end()) {
		auto uid = _sessions[session_id]->GetUserId();

		//移除用户和session的关联
		UserMgr::GetInstance()->RmvUserSession(uid, session_id);
	}

	_sessions.erase(session_id);
	
}

//根据用户获取session
shared_ptr<CSession> CServer::GetSession(std::string uuid) {
	lock_guard<mutex> lock(_mutex);
	auto it = _sessions.find(uuid);
	if (it != _sessions.end()) {
		return it->second;
	}
	return nullptr;
}

bool CServer::CheckValid(std::string uuid)
{
	lock_guard<mutex> lock(_mutex);
	auto it = _sessions.find(uuid);
	if (it != _sessions.end()) {
		return true;
	}
	return false;
}

void CServer::on_timer(const boost::system::error_code& ec) {
	std::vector<std::shared_ptr<CSession>> _expired_sessions;
	//此处加锁遍历session
	{
		lock_guard<mutex> lock(_mutex);
		time_t now = std::time(nullptr);
		for (auto iter = _sessions.begin(); iter != _sessions.end(); iter++) {
			auto b_expired = iter->second->IsHeartbeatExpired(now);
			if (b_expired) {
				//关闭socket, 其实这里也会触发async_read的错误处理
				iter->second->Close();
				//收集过期信息
				_expired_sessions.push_back(iter->second);
				continue;
			}
		}
	}

	//处理过期session, 单独提出，防止死锁
	for (auto &session : _expired_sessions) {
		session->DealExceptionSession();
	}
	
	//再次设置，下一个60s检测
	_timer.expires_after(std::chrono::seconds(60));
	_timer.async_wait([this](boost::system::error_code ec) {
		on_timer(ec);
	});
}
