#include "movingaverage.h"

// idea from http://www.codeproject.com/Articles/17860/A-Simple-Moving-Average-Algorithm

// initialize the sample size to the specified number
MovingAverage::MovingAverage(const uint numSamples)
{
    size = numSamples;
    total = 0;
}

// add sample to a list
void MovingAverage::AddValue(double val)
{
    if (samples.count() == size) {
        // substract the oldest value and remove it from the list
        total -= samples.takeFirst();
    }

    samples.append(val);  // add new value to the list
    total += val;
}

// get the average value
double MovingAverage::AverageValue()
{
    //if (samples.count() < size / 10)
    //    return std::numeric_limits<double>::max();

    return total / samples.count();
}
