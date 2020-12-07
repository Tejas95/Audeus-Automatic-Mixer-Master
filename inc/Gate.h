#ifndef GATE_H
#define GATE_H

//
//  Gate.hpp
//  Audeus
//
//  Created by Tejas Manjunath on 10/21/19.
//  Copyright © 2019 Tejas Manjunath. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "Definitions.h"

using namespace std;

class CGate
{
public:

    CGate();

    void compute_gate(float** input, float** output, float* threshold, float* gain);

    float* loudness_current;

};


#endif // GATE_H
