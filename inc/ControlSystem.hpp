#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/SignalChecker.hpp>
#include "customBlocks/arcsinus.hpp"
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/Saturation.hpp>

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    PeripheralInput<> E2;               //Encoder 2

    // Parameters for angular velocity control 
    /*
    Gain<> cont;                        // Gain converting position in rad, to a velocity in rad/s, in order to set a velocity for the motor
    Saturation<> qdMax;                // Saturation block to limit the velocity to a maximum value
    Gain<> i;                         // reduction ratio of the gearbox
    Gain<> kM;                       // Motor constant to convert velocity in rad/s to a voltage in V
    PeripheralOutput<> M1;          // Motor 1
    */

    Gain<> cont;                        // Gain converting position in rad, to a torque in Nm, in order to set nominal torque of the motor after one revolution of wheel2
    Saturation<> qMax;                  // Saturation block to limit the torque
    Gain<> iInv;                        // inverse of reduction ratio of the gearbox
    Gain<> kMInv;                       // inverse of Motor constant to convert torque to a current in A
    Gain<> R;                          // Resistance of the motor, to convert current in A to a voltage in V           
    PeripheralOutput<> M1;             // Motor 1

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP