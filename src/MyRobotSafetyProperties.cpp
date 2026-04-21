#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slShuttingDown("Shuting down"),
      slBraking("Braking"),
      slStartingUp("Starting up"),
      slEmergency("Emergency"),
      slEmergencyBraking("Emergency Braking"),
      slSystemOn("System On"),
      slMotorPowerOn("Motor Power On"),
      slSystemMoving("System Moving"),

      doSystemOn("Startup the system"),
      shutdown("Shutdown the system"),
      motorsHalted("Motors halted"),
      abort("abort"),
      emergency("emergency"),
      powerOff("Power off"),
      stopMoving("Stop Moving"),
      startMoving("Start moving"),
      powerOn("Power On"),
      resetEmergency("Reset Emergency"),
      systemStarted("System started")

{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
     LED_green = hal.getLogicOutput("onBoardLEDgreen");
     LED_red = hal.getLogicOutput("onBoardLEDred");

     criticalOutputs = {LED_green, LED_red};

    // Declare and add critical inputs
     Button_Pause = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause");
     Button_Mode  = eeros::hal::HAL::instance().getLogicInput("onBoardButtonMode");

     criticalInputs = {Button_Pause, Button_Mode};

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slShuttingDown);
    addLevel(slBraking);
    addLevel(slStartingUp);
    addLevel(slEmergency);
    addLevel(slEmergencyBraking);
    addLevel(slSystemOn);
    addLevel(slMotorPowerOn);
    addLevel(slSystemMoving);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slStartingUp, kPublicEvent);
    slStartingUp.addEvent(systemStarted, slSystemOn, kPrivateEvent);
    slSystemOn.addEvent(powerOn, slMotorPowerOn, kPublicEvent);
    slMotorPowerOn.addEvent(startMoving, slSystemMoving, kPublicEvent);

    slEmergency.addEvent(resetEmergency, slSystemOn, kPrivateEvent);

    slSystemMoving.addEvent(stopMoving, slMotorPowerOn, kPublicEvent);
    slSystemMoving.addEvent(emergency, slEmergencyBraking, kPublicEvent);
    slSystemMoving.addEvent(abort, slBraking, kPublicEvent);

    slMotorPowerOn.addEvent(powerOff, slSystemOn, kPublicEvent);
    slEmergencyBraking.addEvent(motorsHalted, slEmergency, kPrivateEvent);

    slBraking.addEvent(motorsHalted, slShuttingDown, kPrivateEvent);
    slShuttingDown.addEvent(shutdown, slSystemOff, kPrivateEvent);




    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slSystemOn, slMotorPowerOn, emergency, slEmergency, kPublicEvent);
    addEventToAllLevelsBetween(slEmergency, slMotorPowerOn, abort, slShuttingDown, kPublicEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({                       ignore(Button_Pause),                                     ignore(Button_Mode)});
    slShuttingDown.setInputActions({                    ignore(Button_Pause),                                     ignore(Button_Mode)});
    slBraking.setInputActions({                         ignore(Button_Pause),                                     ignore(Button_Mode)});
    slStartingUp.setInputActions({                      ignore(Button_Pause),                                     ignore(Button_Mode)});
    slEmergency.setInputActions({                       ignore(Button_Pause),                                     check(Button_Mode, false, resetEmergency)});
    slEmergencyBraking.setInputActions({                ignore(Button_Pause),                                     ignore(Button_Mode)});
    slSystemOn.setInputActions({                        check(Button_Pause, false, emergency),                    ignore(Button_Mode)});
    slMotorPowerOn.setInputActions({                    check(Button_Pause, false, emergency),                    ignore(Button_Mode)});
    slSystemMoving.setInputActions({                    check(Button_Pause, false, emergency),                    ignore(Button_Mode)});

    // Define output actions for all levels
    slSystemOff.setOutputActions({                      set(LED_green, false),                                     set(LED_red, false)});
    slShuttingDown.setOutputActions({                   set(LED_green, false),                                     set(LED_red, true)});
    slBraking.setOutputActions({                        set(LED_green, false),                                     set(LED_red, true)});
    slStartingUp.setOutputActions({                     set(LED_green, true),                                     set(LED_red, false)});
    slEmergency.setOutputActions({                      set(LED_green, true),                                     set(LED_red, true)});
    slEmergencyBraking.setOutputActions({               set(LED_green, true),                                     set(LED_red, true)});
    slSystemOn.setOutputActions({                       set(LED_green, true),                                     set(LED_red, false)});
    slMotorPowerOn.setOutputActions({                   set(LED_green, true),                                     set(LED_red, false)});
    slSystemMoving.setOutputActions({                   set(LED_green, true),                                     set(LED_red, false)});

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        eeros::Executor::stop();
    });

    slShuttingDown.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        privateContext->triggerEvent(shutdown);
    });

     slBraking.setLevelAction([&](SafetyContext *privateContext) {
        //check if motors are standing still
        privateContext->triggerEvent(motorsHalted);
    });

    slStartingUp.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        privateContext->triggerEvent(systemStarted);
    });

    slEmergency.setLevelAction([&](SafetyContext *privateContext) {
    
    });

    slEmergencyBraking.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(motorsHalted);
    
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        if (slSystemOn.getNofActivations()*dt >= 1)   // wait 1 sec
        {
            privateContext->triggerEvent(powerOn);
        }
    });

    slMotorPowerOn.setLevelAction([&](SafetyContext *privateContext) {
        if (slMotorPowerOn.getNofActivations()*dt >= 5)   // wait 5 sec
        {
            privateContext->triggerEvent(startMoving);
        }
    });

    slSystemMoving.setLevelAction([&](SafetyContext *privateContext) {
        if (slSystemMoving.getNofActivations()*dt >= 5)   // wait 5 sec
        {
            privateContext->triggerEvent(stopMoving);
        }
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(abort);
    });
}
