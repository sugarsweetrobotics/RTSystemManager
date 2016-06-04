// -*- C++ -*-
/*!
 * @file RTSystemManagerComp.cpp
 * @brief Standalone component
 * @date $Date$
 *
 * $Id$
 */

#include <map>
#include <rtm/Manager.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "RTSystemManager.h"

#include "RTTask.h"
#include "RTCondition.h"

static RTSystemManager_ptr _instance = RTSystemManager_nullptr;


RTSystemManager_ptr RTSystemManager::init(int argc, char** argv) {
	if (_instance == nullptr) {
		_instance = std::shared_ptr<RTSystemManager>(new RTSystemManager(argc, argv));
	}
	return _instance;
}

RTSystemManager_ptr RTSystemManager::instance() {
	return _instance;
}


RTSystemManager::RTSystemManager(int argc, char** argv) {
	orb = CORBA::ORB_init(argc, argv);
	defaultDataPortConnectorNV["dataport.interface_type"] = "corba_cdr";
	defaultDataPortConnectorNV["dataport.dataflow_type"] = "push";
	defaultDataPortConnectorNV["dataport.subscription_type"] = "flush";
}

RTSystemManager::~RTSystemManager() {
	orb->shutdown(true);
}

void ssplit(std::vector<std::string>& buf, const std::string &str, char delim) {
	size_t current = 0;
	size_t found;

	while ((found = str.find_first_of(delim, current)) != std::string::npos){
		buf.push_back(std::string(str, current, found - current));
		current = found + 1;
	}

	buf.push_back(std::string(str, current, str.size() - current));
}


std::vector<std::string> RTSystemManager::digestPathUri(const std::string& pathUri) {
	std::vector<std::string> buf;
	if (pathUri.c_str()[0] == '/') {
		ssplit(buf, pathUri.substr(1), '/');
	}
	else {
		ssplit(buf, pathUri, '/');
	}
	return buf;
}

std::string concat(const std::vector<std::string>& strs, const char delim, const int start, const int end) {
	std::string buf = strs[start];
	for (int i = start + 1; i < strs.size(); i++) {
		if (end > 0 && i >= end) {
			break;
		}
		buf += delim;
		buf += strs[i];
	}
	return buf;
}

RTC::CorbaNaming RTSystemManager::naming(const std::string& pathUri) {
	std::vector<std::string> fullpath = digestPathUri(pathUri);
	return RTC::CorbaNaming(orb, fullpath[0].c_str());
}


RTConsumer RTSystemManager::resolve(RTC::CorbaNaming& naming, const std::string& pathUri) 		throw (RTC::CorbaNaming::NotFound, RTC::CorbaNaming::SystemException, RTC::CorbaNaming::InvalidName, RTC::CorbaNaming::CannotProceed) {
	RTConsumer rtc;
	std::vector<std::string> fullpath = digestPathUri(pathUri);
	//RTC::Manager &m = RTC::Manager::instance();

	std::string subPath = concat(fullpath, '/', 1);
	rtc.setObject(naming.resolve(subPath.c_str()));

	return rtc;
}

PortConsumer RTSystemManager::getPort(RTConsumer& rtc, const std::string& name) {
	PortConsumer port;
	RTC::ComponentProfile* cp = rtc->get_component_profile();
	std::string comp_name = (const char*)cp->instance_name;
	std::string combined_name = comp_name + '.' + name;
	std::string java_version_error_name = '.' + name;
	RTC::PortServiceList* plist = rtc->get_ports();


	for (int i = 0; i < plist->length(); i++) {
		RTC::PortProfile* pp = (*plist)[i]->get_port_profile();
		if (combined_name == (const char*)(pp->name)) {
			port.setObject((*plist)[i]->_duplicate((*plist)[i]));
		}
		else if (name == (const char*)(pp->name)) {
			port.setObject((*plist)[i]->_duplicate((*plist)[i]));
		}
		else if (java_version_error_name == (const char*)(pp->name)) {
			port.setObject((*plist)[i]->_duplicate((*plist)[i]));
		}
		delete pp;
	}
	delete cp;
	delete plist;
	return port;
}

RTC::ExecutionContext_ptr RTSystemManager::getEC(RTConsumer& rtc, int ec_id) {
	RTC::ExecutionContextList* ecs = rtc->get_owned_contexts();
	for (int i = 0; i < ecs->length(); i++) {
		if (i == ec_id) {
			return ((*ecs)[i]);
		}
	}
	return NULL;
}

bool RTSystemManager::activateRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec)
{
	if (ec->_is_nil()) { return false; }
	ec->activate_component(rtc._ptr());
	return true;
}

bool RTSystemManager::deactivateRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec)
{
	if (ec->_is_nil()) { return false; }
	ec->deactivate_component(rtc._ptr());
	return true;
}

bool RTSystemManager::resetRTC(RTConsumer& rtc, RTC::ExecutionContext_ptr ec)
{
	if (ec->_is_nil()) { return false; }
	ec->reset_component(rtc._ptr());
	return true;
}

bool RTSystemManager::exitRTC(RTConsumer& rtc)
{
	rtc->exit();
	return true;
}


RTC::LifeCycleState RTSystemManager::getState(RTConsumer& rtc, RTC::ExecutionContext_ptr ec)
{
	return ec->get_component_state(rtc._ptr());
}




bool RTSystemManager::connectDataPorts(PortConsumer& in, PortConsumer& out, std::map<std::string, std::string>& nv, std::string id, std::string name) {
	// 接続のためのコネクションプロファイルを作成
	RTC::ConnectorProfile prof;
	prof.connector_id = CORBA::string_dup(id.c_str());
	prof.name = CORBA::string_dup(name.c_str());
	prof.ports.length(2);
	prof.ports[0] = in._ptr();// > _duplicate(in._ptr());
	prof.ports[1] = out._ptr();// ->_duplicate(out._ptr());
	in.setObject(in._ptr()->_duplicate(in._ptr()));
	out.setObject(out._ptr()->_duplicate(out._ptr()));
	std::map<std::string, std::string>::iterator i = nv.begin();
	for (; i != nv.end(); ++i) {
		std::string key = i->first;
		std::string value = i->second;
		if (nv.find("dataport.interface_type") == nv.end()) {
			CORBA_SeqUtil::push_back(prof.properties, NVUtil::newNV(key.c_str(), value.c_str()));
		}
	}

	std::map<std::string, std::string>::iterator i2 = nv.begin();
	for (; i2 != nv.end(); ++i2) {
		std::string key = i2->first;
		std::string value = i2->second;
		CORBA_SeqUtil::push_back(prof.properties, NVUtil::newNV(key.c_str(), value.c_str()));
	}

	// 接続を確立
	if (in->connect(prof) != RTC::RTC_OK) {
		return false;
	}

	return true;
}


bool RTSystemManager::hasConnection(PortConsumer& port) {
	RTC::PortProfile* pp = port->get_port_profile();
	bool flag = (pp->connector_profiles.length() > 0);
	delete pp;
	return flag;
}

bool RTSystemManager::isConnectedBetweenDataPorts(PortConsumer& in, PortConsumer& out, std::string id, std::string name) {
	bool retval = false;
	if (in->_is_nil()) {
		return false;
	}

	if (out->_is_nil()) {
		return false;
	}

	RTC::PortProfile* outp = out->get_port_profile();
	RTC::ComponentProfile* outOwnp = outp->owner->get_component_profile();

	std::string outRTCName = (const char*)outOwnp->instance_name;
	std::string outPortName = (const char*)outp->name;

	//RTC::ConnectorProfileList* clistIn = ;
	RTC::ConnectorProfileList* inconplist = in->get_connector_profiles();
	for (int i = 0; i < inconplist->length(); i++) {
		RTC::ConnectorProfile* cpi = &(*inconplist)[i];
		if (id.size() > 0) {
			if (id != (const char*)cpi->connector_id) {
				continue;
			}
		}

		if (name.size() > 0) {
			if (name != (const char*)cpi->name) {
				continue;
			}
		}

		RTC::PortProfile* p0 = cpi->ports[0]->get_port_profile();
		RTC::PortProfile* p1 = cpi->ports[1]->get_port_profile();
		RTC::ComponentProfile* pc0 = p0->owner->get_component_profile();
		RTC::ComponentProfile* pc1 = p1->owner->get_component_profile();
		std::string inst_name0 = (const char*)pc0->instance_name;
		std::string inst_name1 = (const char*)pc1->instance_name;
		std::string port_name0 = (const char*)p0->name;
		std::string port_name1 = (const char*)p1->name;

		delete p0;
		delete p1;
		delete pc0;
		delete pc1;
		if (inst_name0 != outRTCName && inst_name1 != outRTCName) {
			continue;
		}
		if (port_name0 != outPortName && port_name1 != outPortName) {
			continue;
		}

		retval = true;
		break;
	}


	delete inconplist;
	delete outOwnp;
	delete outp;
	return false;
}

bool RTSystemManager::disconnectDataPorts(PortConsumer& in, PortConsumer& out, std::string id, std::string name)
{
	return 0;
}



void RTSystemManager::main(void) {
	while (true) {

		std::vector<RTTask_ptr>::iterator i = taskList.begin();
		for (; i != taskList.end(); ++i) {
			try {
				(**i)();
				coil::usleep(1000);
			}
			catch (RTC::CorbaNaming::NotFound& ex) {
				std::cout << "NotFound" << std::endl;
			}
			catch (CORBA::BAD_INV_ORDER& ex) {
				std::cout << "BadInvOrder" << std::endl;
			}
			catch (CORBA::TRANSIENT& ex) {
				std::cout << "Trasient" << std::endl;
			}
			catch (CORBA::UNKNOWN& ex) {
				std::cout << "Trasient" << std::endl;
			}
			catch (CORBA::COMM_FAILURE& ex) {
				std::cout << "COM_FAILURE" << std::endl;
			}
		}
		coil::usleep(1000 * 1000);
	}

}