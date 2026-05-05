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

    Gain<> cont;                        // Gain converting position in rad, to a velocity in rad/s, in order to set a velocity for the motor
    Saturation<> qdMax;                // Saturation block to limit the velocity to a maximum value
    Gain<> i;                         // reduction ratio of the gearbox
    Gain<> kM;                       // Motor constant to convert velocity in rad/s to a voltage in V
    PeripheralOutput<> M1;          // Motor 1

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP