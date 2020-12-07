//
//  Gate.cpp
//  Audeus
//
//  Created by Tejas Manjunath on 10/21/19.
//  Copyright © 2019 Tejas Manjunath. All rights reserved.
//

#include "limiter.h"
#include <iostream>
#include "Gate.h"
#include "Audeus.h"

using namespace std;

CGate::CGate()
{
    loudness_current = new float[NUM_CHANNELS];
}


void CGate::compute_gate(float** input, float** output, float* threshold, float* gain)
{
    CLoudness cloudness;

    cloudness.compute_loudness(input, loudness_current);

    //Gain Calculation for limiter
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        for (int j = 0; j < FRAMES_PER_BUFFER; j++)
        {
            if (input[i][j] < threshold[i])
                output[i][j] = 0;
            else
                output[i][j] = input[i][j] * gain[i];
        }
    }
}
