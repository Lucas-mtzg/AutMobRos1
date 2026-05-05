#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent shutdown;
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent motorsHalted;
    eeros::safety::SafetyEvent abort;
    eeros::safety::SafetyEvent emergency;
    eeros::safety::SafetyEvent powerOff;
    eeros::safety::SafetyEvent stopMoving;
    eeros::safety::SafetyEvent startMoving;
    eeros::safety::SafetyEvent powerOn;
    eeros::safety::SafetyEvent resetEmergency;
    eeros::safety::SafetyEvent systemStarted;


    // Defina all possible (safety) levels
    eeros::safety::SafetyLevel slSystemOff;             // level 0
    eeros::safety::SafetyLevel slShuttingDown;          // level 1
    eeros::safety::SafetyLevel slBraking;               // level 2
    eeros::safety::SafetyLevel slStartingUp;            // level 3
    eeros::safety::SafetyLevel slEmergency;             // level 4
    eeros::safety::SafetyLevel slEmergencyBraking;      // level 5
    eeros::safety::SafetyLevel slSystemOn;              // level 6
    eeros::safety::SafetyLevel slMotorPowerOn;          // level 7
    eeros::safety::SafetyLevel slSystemMoving;          // level 8
    
private:
    // Define all critical outputs
    eeros::hal::Output<bool>* LED_green;
    eeros::hal::Output<bool>* LED_red;

    // Define all critical inputs
    eeros::hal::Input<bool>* Button_Pause;
    eeros::hal::Input<bool>* Button_Mode;

    ControlSystem &cs;

    
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
