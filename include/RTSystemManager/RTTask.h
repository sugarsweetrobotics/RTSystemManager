#pragma once

///#include "RTSystemManager.h"

#include <string>
#include <map>
#include <memory>

#include <rtm/CorbaNaming.h>
class RTSystemManager;
class RTCondition;

class RTTask {

public:
	RTTask() {}
	virtual ~RTTask() {}

public:
	virtual void operator()() = 0;
};

typedef std::shared_ptr<RTTask> RTTask_ptr;

class KeepConnectedTask : public RTTask {
private:
	RTSystemManager* m_pManager;
	std::string m_path0;
	std::string m_path1;

	RTC::CorbaNaming m_ns0;
	RTC::CorbaNaming m_ns1;

	std::string m_PortName0;
	std::string m_PortName1;

	std::map<std::string, std::string> m_NamedValues;
public:

	void init(const std::string& path0, const std::string& path1);

	KeepConnectedTask(RTSystemManager* pManager, const std::string& path0, const std::string& path1);

	KeepConnectedTask(RTSystemManager* pManager, const std::string& path0, const std::string& path1, std::map<std::string, std::string> nv);

	virtual ~KeepConnectedTask();

	virtual void operator()();
};

class ActivateWhenTask : public RTTask {

public:
	RTSystemManager* m_pManager;
	RTCondition_ptr m_pCondition;
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	ActivateWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path);

public:
	virtual void operator()();
};

class DeactivateWhenTask : public RTTask {

private:
	RTSystemManager* m_pManager;
	RTCondition_ptr m_pCondition;
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	DeactivateWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path);

public:
	virtual void operator()();
};

class ResetWhenTask : public RTTask {

private:
	RTSystemManager* m_pManager;
	RTCondition_ptr m_pCondition;
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	ResetWhenTask(RTSystemManager* pManager, RTCondition_ptr pCondition, const std::string& path);

public:
	virtual void operator()();
};