#include "RTSystemManager.h"
#include "RTTask.h"
#include "RTCondition.h"



void MyModuleInit(RTC::Manager* manager)
{
	return;
}

int main(int argc, char** argv) {
  RTSystemManager_ptr systemManager = RTSystemManager::init(argc, argv);
  std::string out = "localhost:2809/ConsoleOut0.rtc";
  std::string in = "localhost:2809/ConsoleIn0.rtc";
  std::string outport = "localhost:2809/ConsoleIn0.rtc:out";
  std::string inport = "localhost:2809/ConsoleOut0.rtc:in";
  systemManager->pushTask(UNLESS(ISCONNECTED(inport, outport), CONNECT(inport, outport)));
  systemManager->pushTask(IF(OR(ISRTCNOTFOUND(in), ISRTCZOMBIE(in)), DEACTIVATE(out), ACTIVATE(out)));
  systemManager->pushTask(IF(OR(ISRTCNOTFOUND(out), ISRTCZOMBIE(out)), DEACTIVATE(in), ACTIVATE(in)));
  systemManager->main();
  return 0;
}
