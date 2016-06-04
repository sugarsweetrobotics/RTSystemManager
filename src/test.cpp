#include "RTSystemManager.h"
#include "RTTask.h"
#include "RTCondition.h"



void MyModuleInit(RTC::Manager* manager)
{
	return;
}

int main(int argc, char** argv) {
  RTSystemManager_ptr systemManager = RTSystemManager::init(argc, argv);
  std::string ns = "localhost:2809/MSI.host_cxt/";
  std::string joy = ns + "SFMLJoystick0.rtc";
  std::string conv = ns + "JoystickConverter0.rtc";
  std::string can0 = ns + "CANOpenMotorController0.rtc";
  std::string can1 = ns + "CANOpenMotorController1.rtc";
  std::string arms = ns + "ArmIntegrator0.rtc";

  std::string joyout0 = joy + ":axis";
  std::string convin0 = conv + ":axis";
  std::string joyout1 = joy + ":buttons";
  std::string convin1 = conv + ":buttons";

  std::string armsrcom = arms + ":rightManipulatorCommon";
  std::string convrcom = conv + ":rightManipulatorCommon";
  std::string armsrmid = arms + ":rightManipulatorMiddle";
  std::string convrmid = conv + ":rightManipulatorMiddle";



  std::string armslcom = arms + ":leftManipulatorCommon";
  std::string convlcom = conv + ":leftManipulatorCommon";
  std::string armslmid = arms + ":leftManipulatorMiddle";
  std::string convlmid = conv + ":leftManipulatorMiddle";

  std::string can0in0 = can0 + ":target";
  std::string armsout0 = arms + ":rightEPOSTarget";
  std::string can0out0 = can0 + ":current";
  std::string armsin0 = arms + ":rightEPOSActual";
  std::string can1in0 = can1 + ":target";
  std::string armsout1 = arms + ":leftEPOSTarget";
  std::string can1out0 = can1 + ":current";
  std::string armsin1 = arms + ":leftEPOSActual";

  systemManager->pushTask(UNLESS(ISCONNECTED(joyout0, convin0), CONNECT(joyout0, convin0)));
  systemManager->pushTask(UNLESS(ISCONNECTED(joyout1, convin1), CONNECT(joyout1, convin1)));

  systemManager->pushTask(UNLESS(ISCONNECTED(armsrcom, convrcom), CONNECT(armsrcom, convrcom)));
  systemManager->pushTask(UNLESS(ISCONNECTED(armsrmid, convrmid), CONNECT(armsrmid, convrmid)));
  systemManager->pushTask(UNLESS(ISCONNECTED(armslcom, convlcom), CONNECT(armslcom, convlcom)));
  systemManager->pushTask(UNLESS(ISCONNECTED(armslmid, convlmid), CONNECT(armslmid, convlmid)));

  systemManager->pushTask(UNLESS(ISCONNECTED(can0in0, armsout0), CONNECT(can0in0, armsout0)));
  systemManager->pushTask(UNLESS(ISCONNECTED(can0out0, armsin0), CONNECT(can0out0, armsin0)));
  systemManager->pushTask(UNLESS(ISCONNECTED(can1in0, armsout1), CONNECT(can1in0, armsout1)));
  systemManager->pushTask(UNLESS(ISCONNECTED(can1out0, armsin1), CONNECT(can1out0, armsin1)));

  systemManager->pushTask(IF(OR(ISRTCNOTFOUND(conv), ISRTCZOMBIE(conv)), DEACTIVATE(joy), ACTIVATE(joy)));
  systemManager->pushTask(UNLESS(ISRTCACTIVE(conv), ACTIVATE(conv)));
  systemManager->pushTask(UNLESS(ISRTCACTIVE(arms), ACTIVATE(arms)));
  systemManager->pushTask(UNLESS(ISRTCACTIVE(can0), ACTIVATE(can0)));
  systemManager->pushTask(UNLESS(ISRTCACTIVE(can1), ACTIVATE(can1)));

  systemManager->main();
  return 0;
}
