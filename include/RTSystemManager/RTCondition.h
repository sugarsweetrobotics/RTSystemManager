#pragma once

#include <memory>
#include "RTSystemManager.h"


class RTCondition {

public:

	RTCondition() {}

	virtual ~RTCondition() {}

public:
	virtual bool operator()() = 0;
};


typedef std::shared_ptr<RTCondition> RTCondition_ptr;



class IsRTCInactive : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCInactive(RTSystemManager* pManager, const std::string& path, const int ec_id = 0) : m_pManager(pManager),
		m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
		m_ec_id = 0;
	}

public:
	virtual bool operator()() {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		return m_pManager->isInactiveRTC(rtc, m_ec_id);
	}
};

class IsRTCActive : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCActive(RTSystemManager* pManager, const std::string& path, const int ec_id = 0) : m_pManager(pManager),
		m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
		m_ec_id = 0;
	}

public:
	virtual bool operator()() {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		return m_pManager->isActiveRTC(rtc, m_ec_id);
	}
};

class IsRTCError : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCError(RTSystemManager* pManager, const std::string& path, const int ec_id = 0) : m_pManager(pManager),
		m_ns(m_pManager->naming(m_pManager->digestPathUri(path)[0])), m_path(path) {
		m_ec_id = 0;
	}

public:
	virtual bool operator()() {
		RTConsumer rtc = m_pManager->resolve(m_ns, m_path);
		return m_pManager->isErrorRTC(rtc, m_ec_id);
	}
};