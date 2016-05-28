


#include "RTTask.h"
#include "RTSystemManager.h"
#include "RTCondition.h"

void KeepConnectedTask::init(const std::string& path0, const std::string& path1) {
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

KeepConnectedTask::KeepConnectedTask(RTSystemManager* pManager, const std::string& path0, const std::string& path1) : m_pManager(pManager),
m_ns0(m_pManager->naming(m_pManager->digestPathUri(path0)[0])),
m_ns1(m_pManager->naming(m_pManager->digestPathUri(path1)[0])) {
	init(path0, path1);
	m_NamedValues = m_pManager->defaultDataPortConnectorNV;
}

KeepConnectedTask::KeepConnectedTask(RTSystemManager* pManager, const std::string& path0, const std::string& path1, std::map<std::string, std::string> nv) : m_pManager(pManager),
m_ns0(m_pManager->naming(m_pManager->digestPathUri(path0)[0])),
m_ns1(m_pManager->naming(m_pManager->digestPathUri(path1)[0])) {
	init(path0, path1);
	m_NamedValues = nv;
}

KeepConnectedTask::~KeepConnectedTask() {}

void KeepConnectedTask::operator()() {
	RTConsumer rtc0 = m_pManager->resolve(m_ns0, m_path0);
	RTConsumer rtc1 = m_pManager->resolve(m_ns1, m_path1);
	PortConsumer port0 = m_pManager->getPort(rtc0, m_PortName0);
	PortConsumer port1 = m_pManager->getPort(rtc1, m_PortName1);
	if (!m_pManager->isConnectedBetweenDataPorts(port0, port1)) {
		m_pManager->connectDataPorts(port0, port1, m_NamedValues);
	}
}


ActivateWhenTask::ActivateWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path) : m_pManager(pManager), m_pCondition(pCondition),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
}
void ActivateWhenTask::operator()() {
	if ((*m_pCondition)()) {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		m_pManager->activateRTC(rtc, m_pManager->getEC(rtc));
	}
}

DeactivateWhenTask::DeactivateWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path) : m_pManager(pManager), m_pCondition(pCondition),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
}

void DeactivateWhenTask::operator()() {
	if ((*m_pCondition)()) {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		m_pManager->deactivateRTC(rtc, m_pManager->getEC(rtc));
	}
}


ResetWhenTask::ResetWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path) : m_pManager(pManager), m_pCondition(pCondition),
m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
}

void ResetWhenTask::operator()() {
	if ((*m_pCondition)()) {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		m_pManager->resetRTC(rtc, m_pManager->getEC(rtc));
	}
}
