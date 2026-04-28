#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : g(2.0),
      timedomain("Main time domain", dt, true),
      q1("quat1"),
      arcsinus_block(),
      motor("motor1"),
      motorVoltageSetpoint(0.0)
{
    // Name all blocks
    g.setName("g");
    q1.setName("quaternion 1");
    arcsinus_block.setName("arcsinus");
    motorVoltageSetpoint.setName("motorVoltageSetpoint");
    motor.setName("motor");

    // Name all signals
    q1.getOut().getSignal().setName("sin(alpha/2)");     // q1 is alpha/2, that's why we multiply it with a gain of 2 to get alpha
    g.getOut().getSignal().setName("alpha");
    arcsinus_block.getOut().getSignal().setName("alpha/2");
    motorVoltageSetpoint.getOut().getSignal().setName("Motor voltage setpoint [V]");

    // Connect signals
    arcsinus_block.getIn().connect(q1.getOut());              // output of Gain g block is input of the arcsinus block
    g.getIn().connect(arcsinus_block.getOut());                 //q1 is input of the Gain g block
    motor.getIn().connect(motorVoltageSetpoint.getOut());     // output of the motor voltage setpoint block is input of the motor block

    // Add blocks to timedomain
    timedomain.addBlock(q1);
    timedomain.addBlock(arcsinus_block);
    timedomain.addBlock(g);
    timedomain.addBlock(motorVoltageSetpoint);
    timedomain.addBlock(motor);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}