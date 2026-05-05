#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : g(2.0),
      timedomain("Main time domain", dt, true),
      q1("quat1"),
      signalchecker(-0.2, 0.2),
      arcsinus_block()
{
    // Name all blocks
    g.setName("g");
    q1.setName("quaternion 1");
    signalchecker.setName("signal checker");
    arcsinus_block.setName("arcsinus");

    // Name all signals
    q1.getOut().getSignal().setName("sin(alpha/2)");     // q1 is alpha/2, that's why we multiply it with a gain of 2 to get alpha
    g.getOut().getSignal().setName("alpha");
    arcsinus_block.getOut().getSignal().setName("alpha/2");

    // Connect signals
    arcsinus_block.getIn().connect(q1.getOut());              // output of Gain g block is input of the arcsinus block
    g.getIn().connect(arcsinus_block.getOut());                 //q1 is input of the Gain g block
    signalchecker.getIn().connect(g.getOut());      // output of Gain g block is input of the signalchecker 

    // Add blocks to timedomain
    timedomain.addBlock(q1);
    timedomain.addBlock(arcsinus_block);
    timedomain.addBlock(g);
    timedomain.addBlock(signalchecker);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}