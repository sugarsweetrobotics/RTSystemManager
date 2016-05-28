#include "RTSystemManager.h"
#include "RTTask.h"
#include "RTCondition.h"



void MyModuleInit(RTC::Manager* manager)
{
	return;
}

int main(int argc, char** argv) {

	RTSystemManager systemManager(argc, argv);

	systemManager.pushTask(RTTask_ptr(new KeepConnectedTask(&systemManager, "localhost:2809/ConsoleIn0.rtc:out", "localhost:2809/ConsoleOut0.rtc:in")));
	systemManager.main();

	return 0;
}
