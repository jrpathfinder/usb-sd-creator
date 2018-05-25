#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <QLinkedList>
#include <limits>

class MovingAverage
{
public:
    MovingAverage(const uint numSamples = 10);
    void AddValue(double val);
    double AverageValue();

private:
    QLinkedList<double> samples;
    int size;
    double total;
};

#endif // MOVINGAVERAGE_H
