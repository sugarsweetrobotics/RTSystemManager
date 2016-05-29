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


class IsRTCInactive : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCInactive(RTSystemManager* pManager, const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

class IsRTCActive : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCActive(RTSystemManager* pManager, const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

class IsRTCError : public RTCondition {

private:
	RTSystemManager* m_pManager;
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCError(RTSystemManager* pManager, const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};