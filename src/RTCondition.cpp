#include "RTCondition.h"
#include "RTSystemManager.h"

Or::Or(RTCondition_ptr condition0, RTCondition_ptr condition1) : cond0(condition0), cond1(condition1) {
}

Or::~Or() {}

bool Or::operator()() {
  return ((*cond0)() || (*cond1)());
}


Not::Not(RTCondition_ptr condition_) : condition(condition_) {
}

Not::~Not() {}

bool Not::operator()() {
  return !(*condition)();
}

IsRTCInactive::IsRTCInactive(const std::string& path, const int ec_id) : 
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
	m_ec_id = 0;
}
bool IsRTCInactive::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  return RTSystemManager::instance()->isInactiveRTC(rtc, m_ec_id);
}

IsRTCActive::IsRTCActive(const std::string& path, const int ec_id):
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
  m_ec_id = 0;
}
bool IsRTCActive::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  return RTSystemManager::instance()->isActiveRTC(rtc, m_ec_id);
}

IsRTCZombie::IsRTCZombie(const std::string& path, const int ec_id):
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
  m_ec_id = 0;
}
bool IsRTCZombie::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  try {
    rtc->get_component_profile();
  } catch (CORBA::TRANSIENT& ex) {
    return true;
  }
  return false;
}

IsRTCNotFound::IsRTCNotFound(const std::string& path, const int ec_id):
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
  m_ec_id = 0;
}
bool IsRTCNotFound::operator()() {
  try {
    RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  } catch (RTC::CorbaNaming::NotFound& ex) {
    return true;
  }
  return false;
}




IsRTCError::IsRTCError(const std::string& path, const int ec_id) : 
  m_ns(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path)[0])), m_path(path) {
  m_ec_id = 0;
}
bool IsRTCError::operator()() {
  RTConsumer rtc = RTSystemManager::instance()->resolve(m_ns, m_path);
  return RTSystemManager::instance()->isErrorRTC(rtc, m_ec_id);
}

IsConnected::IsConnected(const std::string& path0, const std::string& path1) :
  m_ns0(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path0)[0])),
  m_ns1(RTSystemManager::instance()->naming(RTSystemManager::instance()->digestPathUri(path1)[0])) {
  init(path0, path1);
}

IsConnected::~IsConnected() {}


void IsConnected::init(const std::string& path0, const std::string& path1) {
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

bool IsConnected::operator()() {
  RTConsumer rtc0 = RTSystemManager::instance()->resolve(m_ns0, m_path0);
  RTConsumer rtc1 = RTSystemManager::instance()->resolve(m_ns1, m_path1);
  PortConsumer port0 = RTSystemManager::instance()->getPort(rtc0, m_PortName0);
  PortConsumer port1 = RTSystemManager::instance()->getPort(rtc1, m_PortName1);
  return (RTSystemManager::instance()->isConnectedBetweenDataPorts(port0, port1));
}

