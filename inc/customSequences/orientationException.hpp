#ifndef ORIENTATIONEXCEPTION_HPP_
#define ORIENTATIONEXCEPTION_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Condition.hpp>
#include "ControlSystem.hpp"


class CheckOrientation : public eeros::sequencer::Condition
{
public:
    CheckOrientation(double angle, ControlSystem &cs)
        : angle(angle), cs(cs)
    {
        
    }

    bool validate()
    {
        return abs(cs.g.getOut().getSignal().getValue()) > angle;  // Check if the absolute value of the output signal of the Gain g block is greater than the specified angle
    }

private:    
    double angle;
    ControlSystem &cs;  
};




class OrientationException : public eeros::sequencer::Sequence
{
public:
    OrientationException(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs, CheckOrientation checkOrientation)
        : eeros::sequencer::Sequence(name, caller, true), cs(cs), checkOrientation(checkOrientation)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        log.warn() << "Orientation around x is outside of the allowed range!";
        log.warn() << "Stoping all motors!";
        cs.motorVoltageSetpoint.setValue(0);
        return 0;
    }

    bool checkExitCondition()
    {
        return !checkOrientation.validate();  // The exception sequence will be active as long as the orientation is outside 
                                              // of the allowed range, once the orientation is back in the allowed range, 
                                              // the exception sequence will be aborted and the system can continue with its 
                                              // normal operation
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
    CheckOrientation checkOrientation;
};

#endif // ORIENTATIONEXCEPTION_HPP_
