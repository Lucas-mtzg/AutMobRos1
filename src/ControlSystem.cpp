#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : E2("enc2"), cont(0.03/2.0/M_PI), qMax(0.1), iInv(104/3441), kMInv(1.0/8.44e-3), R(8), M1("motor1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    E2.setName("encoder 2");
    cont.setName("cont");
    qMax.setName("qMax");
    iInv.setName("iInv");
    kMInv.setName("kMInv");
    R.setName("R");
    M1.setName("M1");


    // Name all signals
    E2.getOut().getSignal().setName("q2 [rad]");
    cont.getOut().getSignal().setName("q1 [Nm]");
    qMax.getOut().getSignal().setName("q1 [Nm]");
    iInv.getOut().getSignal().setName("T1 [Nm]");
    kMInv.getOut().getSignal().setName("I1 [A]");
    R.getOut().getSignal().setName("V1 [V]");

    // Connect signals
    cont.getIn().connect(E2.getOut());
    qMax.getIn().connect(cont.getOut());
    iInv.getIn().connect(qMax.getOut());
    kMInv.getIn().connect(iInv.getOut());
    R.getIn().connect(kMInv.getOut());
    M1.getIn().connect(R.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(E2);
    timedomain.addBlock(cont);
    timedomain.addBlock(qMax);
    timedomain.addBlock(iInv);
    timedomain.addBlock(kMInv);
    timedomain.addBlock(R);
    timedomain.addBlock(M1);
    

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}