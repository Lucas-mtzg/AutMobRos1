#ifndef ARCSINUS_HPP_
#define ARCSINUS_HPP_

#include <eeros/control/Blockio.hpp>
// Include header files for the subblocks

using namespace eeros::control;

template <typename T = double>
class arcsinus : public Blockio<1,1,T>   // Set the number of inputs and outputs
{
public:
    arcsinus() : alpha{0.0}
    {
    
    }

    // Implement getter functions for the subsystem inputs


    virtual void run()
    {
        // Calculate output values, set timestamps and 
        // call the run method of the subblocks
        alpha = asin(this->in.getSignal().getValue());  // Calculate arcsinus of the input signal
        this->out.getSignal().setValue(alpha);        // Set the output signal to the calculated arcsinus value
        this->out.getSignal().setTimestamp(this->in.getSignal().getTimestamp()); // Set the timestamp of the output signal to the same as the input signal
    }

protected:
    // Define intermediate variables and subblocks
    double alpha;
};

#endif //ARCSINUS_HPP_
