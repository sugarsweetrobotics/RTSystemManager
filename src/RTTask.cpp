


#include "RTTask.h"
#include "RTSystemManager.h"
#include "RTCondition.h"

If::If(RTCondition_ptr condition_, RTTask_ptr taskWhenTrue, RTTask_ptr taskWhenFalse):
  condition(condition_), trueTask(taskWhenTrue), falseTask(taskWhenFalse)
{
}

If::~If() 
{
}

void If::operator()()
{
  if ((*condition)()) {
    (*trueTask)();
  } else {
    if (falseTask != nullptr) {
      (*falseTask)();
    }
  }
}


void Connect::init(const std::string& path0, const std::string& path1) {
	std::vector<std::string> buf;
	ssplit(buf, path0, ':');
	if (buf.size() == 2) {
		m_path0 = buf[0];
		m_PortName0 = buf[1];
	}
	else if (buf.size() == 3) {
		m_path0 = concat(buf, ':', 0, 2);
		m_PortName0 = buf[2];
	}
	buf.clear();
	ssplit(buf, path1, ':');
	if (buf.size() == 2) {
		m_path1 = buf[0];
		m_PortName1 = buf[1];
	}
	else if (buf.size() == 3) {
		m_path1 = concat(buf, ':', 0, 2);
		m_PortName1 = buf[2];
	}
}

Connect::Connect(const std::string& path0, const std::string& path1) : 
  m_ns0(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path0)[0])),
  m_ns1(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path1)[0])) {
  init(path0, path1);
  m_NamedValues = RTSystemManager::instance()->defaultDataPortConnectorNV;
}

Connect::Connect(const std::string& path0, const std::string& path1, std::map<std::string, std::string> nv) : 
  m_ns0(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path0)[0])),
  m_ns1(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path1)[0])) {
  init(path0, path1);
  m_NamedValues = nv;
}

Connect::~Connect() {}

void Connect::operator()() {
	RTConsumer rtc0 = RTSystemManager::instance()->resolve(m_ns0, m_path0);
	RTConsumer rtc1 = RTSystemManager::instance()->resolve(m_ns1, m_path1);
	PortConsumer port0 = RTSystemManager::instance()->getPort(rtc0, m_PortName0);
	PortConsumer port1 = RTSystemManager::instance()->getPort(rtc1, m_PortName1);
	if (!RTSystemManager::instance()->isConnectedBetweenDataPorts(port0, port1)) {
		RTSystemManager::instance()->connectDataPorts(port0, port1, m_NamedValues);
	}
}


Activate::Activate(const std::string& path) : 
m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
}
void Activate::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  RTSystemManager::instance()->activateRTC(rtc, RTSystemManager::instance()->getEC(rtc));
}

Deactivate::Deactivate(const std::string& path) :
m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
}

void Deactivate::operator()() {
		RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
		RTSystemManager::instance()->deactivateRTC(rtc, RTSystemManager::instance()->getEC(rtc));
}


Reset::Reset(const std::string& path) : 
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) 
{
}

void Reset::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  RTSystemManager::instance()->resetRTC(rtc, RTSystemManager::instance()->getEC(rtc));
}
