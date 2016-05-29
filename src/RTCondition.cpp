#include "RTCondition.h"
#include "RTSystemManager.h"




IsRTCInactive::IsRTCInactive(RTSystemManager* pManager, const std::string& path, const int ec_id) : m_pManager(pManager),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
	m_ec_id = 0;
}
bool IsRTCInactive::operator()() {
	RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
	return m_pManager->isInactiveRTC(rtc, m_ec_id);
}

IsRTCActive::IsRTCActive(RTSystemManager* pManager, const std::string& path, const int ec_id) : m_pManager(pManager),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
	m_ec_id = 0;
}
bool IsRTCActive::operator()() {
	RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
	return m_pManager->isActiveRTC(rtc, m_ec_id);
}

IsRTCError::IsRTCError(RTSystemManager* pManager, const std::string& path, const int ec_id) : m_pManager(pManager),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
	m_ec_id = 0;
}
bool IsRTCError::operator()() {
	RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
	return m_pManager->isErrorRTC(rtc, m_ec_id);
}
