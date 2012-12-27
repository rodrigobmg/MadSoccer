//------------------------------------------------------------------------
// Name:    s2c_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

static const int s2c_ProtocolListener_ID = 200;

// ProtocolListener
class s2c_ProtocolListener : public network::IProtocolListener
{
public:
	s2c_ProtocolListener() : IProtocolListener(s2c_ProtocolListener_ID) {}
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
	virtual void func1(netid senderId){}
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
	virtual void func4(netid senderId){}
};


// Protocol
class s2c_Protocol : public network::IProtocol
{
public:
	s2c_Protocol() : IProtocol(s2c_ProtocolListener_ID) {}
	void func1(netid targetId);
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, const float &value);
	void func4(netid targetId);
};
