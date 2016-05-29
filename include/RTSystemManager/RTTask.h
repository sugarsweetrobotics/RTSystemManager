#pragma once

///#include "RTSystemManager.h"

#include <string>
#include <map>
#include <memory>

#include <rtm/CorbaNaming.h>
#include "RTSystemManager.h"


#include "RTCondition.h"

class RTTask {

public:
	RTTask() {}
	virtual ~RTTask() {}

public:
	virtual void operator()() = 0;
};

class If : public RTTask {
 protected:
	RTTask_ptr trueTask;
	RTTask_ptr falseTask;
	RTCondition_ptr condition;
public:

	If(RTCondition_ptr condition, RTTask_ptr taskWhenTrue, RTTask_ptr taskWhenFalse = RTTask_nullptr);

	~If();

	virtual void operator()();
};

inline RTTask_ptr IF(RTCondition_ptr condition, RTTask_ptr taskWhenTrue, RTTask_ptr taskWhenFalse = RTTask_nullptr) {
  return RTTask_ptr(new If(condition, taskWhenTrue, taskWhenFalse));
}

class Unless : public If {
 protected:
 public:
 Unless(RTCondition_ptr cond, RTTask_ptr trueTask, RTTask_ptr falseTask = RTTask_nullptr) : If(RTCondition_ptr(new Not(cond)), trueTask, falseTask) {}
  virtual ~Unless() {}
};

inline RTTask_ptr UNLESS(RTCondition_ptr cond, RTTask_ptr ttask, RTTask_ptr ftask = RTTask_nullptr) {
  return RTTask_ptr(new Unless(cond, ttask, ftask));
}

typedef std::shared_ptr<Unless> Unless_ptr;

class Connect : public RTTask {
private:
	std::string m_path0;
	std::string m_path1;

	RTC::CorbaNaming m_ns0;
	RTC::CorbaNaming m_ns1;

	std::string m_PortName0;
	std::string m_PortName1;

	std::map<std::string, std::string> m_NamedValues;
public:

	void init(const std::string& path0, const std::string& path1);

	Connect(const std::string& path0, const std::string& path1);

	Connect(const std::string& path0, const std::string& path1, std::map<std::string, std::string> nv);

	virtual ~Connect();

	virtual void operator()();
};

inline RTTask_ptr CONNECT(const std::string& path0, const std::string& path1) {
  return RTTask_ptr(new Connect(path0, path1));
}

inline RTTask_ptr CONNECT(const std::string& path0, const std::string& path1, std::map<std::string, std::string> nv) {
  return RTTask_ptr(new Connect(path0, path1, nv));
}

class Activate : public RTTask {

public:
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	Activate(const std::string& path);

public:
	virtual void operator()();
};

inline RTTask_ptr ACTIVATE(const std::string& path) {
  return RTTask_ptr(new Activate(path));
}

class Deactivate : public RTTask {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	Deactivate(const std::string& path);

public:
	virtual void operator()();
};

inline RTTask_ptr DEACTIVATE(const std::string& path) {
  return RTTask_ptr(new Deactivate(path));
}

class Reset : public RTTask {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
public:
	Reset(const std::string& path);

public:
	virtual void operator()();
};

inline RTTask_ptr RESET(const std::string& path) {
  return RTTask_ptr(new Reset(path));
}
