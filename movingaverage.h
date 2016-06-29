////////////////////////////////////////////////////////////////////////////////
//      This file is part of LibreELEC - http://www.libreelec.tv
//      Copyright (C) 2016 Team LibreELEC
//
//  LibreELEC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  LibreELEC is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

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
