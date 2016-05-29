// -*- C++ -*-
/*!
 * @file  RTSystemManager.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef RTSYSTEMMANAGER_H
#define RTSYSTEMMANAGER_H

#include <vector>
#include <memory>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>



#include <rtm/CorbaNaming.h>
#include <rtm/CorbaConsumer.h>

class RTTask;
class RTCondition;
class RTSystemManager;
typedef std::shared_ptr<RTTask> RTTask_ptr;
typedef std::shared_ptr<RTCondition> RTCondition_ptr;
typedef std::shared_ptr<RTSystemManager> RTSystemManager_ptr;
const static std::shared_ptr<RTTask> RTTask_nullptr(nullptr);
const static std::shared_ptr<RTCondition> RTCondition_nullptr(nullptr);
const static std::shared_ptr<RTSystemManager> RTSystemManager_nullptr(nullptr);

void ssplit(std::vector<std::string>& buf, const std::string &str, char delim);
std::string concat(const std::vector<std::string>& strs, const char delim, const int start = 0, const int end = -1);
typedef RTC::CorbaConsumer<RTC::RTObject> RTConsumer;
typedef RTC::CorbaConsumer<RTC::PortService> PortConsumer;
/*!
 * @class RTSystemManager
 * @brief ModuleDescription
 *
 */
class RTSystemManager {
private:


	CORBA::ORB_ptr orb;

public:

	std::map<std::string, std::string> defaultDataPortConnectorNV;

	std::vector<RTTask_ptr> taskList;
  
  static RTSystemManager_ptr init(int argc, char** argv);

  static RTSystemManager_ptr instance();
private:
	RTSystemManager(int argc, char** argv);


public:
	~RTSystemManager();

public:
	void pushTask(RTTask_ptr task) {
		taskList.push_back(task);
	}

public:

	void main();

	std::vector<std::string> digestPathUri(const std::string& pathUri);

	RTConsumer resolve(RTC::CorbaNaming& naming, const std::string& pathUri)
		throw (RTC::CorbaNaming::NotFound, RTC::CorbaNaming::SystemException, RTC::CorbaNaming::InvalidName, RTC::CorbaNaming::CannotProceed);

	RTC::CorbaNaming naming(const std::string& pathUri);

	PortConsumer getPort(RTConsumer& rtc, const std::string& name);

	bool connectDataPorts(PortConsumer& in, PortConsumer& out, std::string id = "connector0", std::string name = "connector0")  {
		return connectDataPorts(in, out, defaultDataPortConnectorNV, id, name);
	}

	bool connectDataPorts(PortConsumer& in, PortConsumer& out, std::map<std::string, std::string>& nv, std::string id = "connector0", std::string name = "connector0");
	bool disconnectDataPorts(PortConsumer& in, PortConsumer& out, std::string id = "", std::string name = "");
	bool isConnectedBetweenDataPorts(PortConsumer& in, PortConsumer& out, std::string id = "", std::string name = "");

	RTC::ExecutionContext_ptr getEC(RTConsumer& rtc, int ec_id = 0);

	bool activateRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec);
	bool deactivateRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec);
	bool resetRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec);
	bool exitRTC(RTConsumer& rtc);

	RTC::LifeCycleState getState(RTConsumer& rtc, RTC::ExecutionContext_ptr ec);

	bool isActiveRTC(RTConsumer& rtc, const int ec_id = 0) {
		return getState(rtc, getEC(rtc, ec_id)) == RTC::ACTIVE_STATE;
	}

	bool isInactiveRTC(RTConsumer& rtc, const int ec_id = 0) {
		return getState(rtc, getEC(rtc, ec_id)) == RTC::INACTIVE_STATE;
	}

	bool isErrorRTC(RTConsumer& rtc, const int ec_id = 0) {
		return getState(rtc, getEC(rtc, ec_id)) == RTC::ERROR_STATE;
	}
};



#endif // RTSYSTEMMANAGER_H

