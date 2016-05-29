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

class Or : public RTCondition {
 private:
  RTCondition_ptr cond0;
  RTCondition_ptr cond1;

 public:
  Or(RTCondition_ptr condition0, RTCondition_ptr condition1);
  virtual ~Or();

 public:
  virtual bool operator()();
};

inline RTCondition_ptr OR(RTCondition_ptr cond0, RTCondition_ptr cond1) {
  return RTCondition_ptr(new Or(cond0, cond1));
}

class Not : public RTCondition {
 private:
  RTCondition_ptr condition;
 public:
  Not(RTCondition_ptr condition_);

  virtual ~Not();

 public:
  virtual bool operator()();
};

inline RTCondition_ptr NOT(RTCondition_ptr cond) {
  return RTCondition_ptr(new Not(cond));
}

class IsRTCInactive : public RTCondition {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCInactive(const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

inline RTCondition_ptr ISRTCINACTIVE(const std::string& path, const int ec_id = 0) {
  return RTCondition_ptr(new IsRTCInactive(path, ec_id));
}

class IsRTCActive : public RTCondition {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCActive(const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

inline RTCondition_ptr ISRTCACTIVE(const std::string& path, const int ec_id = 0) {
  return RTCondition_ptr(new IsRTCActive(path, ec_id));
}

class IsRTCZombie : public RTCondition {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCZombie(const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

inline RTCondition_ptr ISRTCZOMBIE(const std::string& path, const int ec_id = 0) {
  return RTCondition_ptr(new IsRTCZombie(path, ec_id));
}

class IsRTCNotFound : public RTCondition {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCNotFound(const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

inline RTCondition_ptr ISRTCNOTFOUND(const std::string& path, const int ec_id = 0) {
  return RTCondition_ptr(new IsRTCNotFound(path, ec_id));
}



class IsRTCError : public RTCondition {

private:
	RTC::CorbaNaming m_ns;
	std::string m_path;
	int m_ec_id;
public:
	IsRTCError(const std::string& path, const int ec_id = 0);

public:
	virtual bool operator()();
};

inline RTCondition_ptr ISRTCERROR(const std::string& path, const int ec_id = 0) {
  return RTCondition_ptr(new IsRTCError(path, ec_id));
}

class IsConnected : public RTCondition {
	std::string m_path0;
	std::string m_path1;

	RTC::CorbaNaming m_ns0;
	RTC::CorbaNaming m_ns1;

	std::string m_PortName0;
	std::string m_PortName1;

 public:
	IsConnected(const std::string& path0, const std::string& path1);
	virtual ~IsConnected();

	void init(const std::string& path0, const std::string& path1);
 public:
	virtual bool operator()();
};

inline RTCondition_ptr ISCONNECTED(const std::string& path0, const std::string& path1) {
  return RTCondition_ptr(new IsConnected(path0, path1));
}
