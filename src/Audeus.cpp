// Audeus.cpp : Defines the entry point for the console application.
//


/** @file main.cpp
@ingroup Audeus
@brief Estimate Loudness and adjust gain
@author Tejas Manjunath
*/
/*
* $Id$
*
* This program uses the PortAudio Portable Audio Library.
* For more information see: http://www.portaudio.com
* Copyright (c) 1999-2000 Ross Bencina and Phil Burk
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
* ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
* The text above constitutes the entire PortAudio license; however,
* the PortAudio community also makes the following non-binding requests:
*cd D
* Any person wishing to distribute modifications to the Software is
* requested to send the modifications to the original developer so that
* they can be incorporated into the canonical version. It is also
* requested that these non-binding requests be included along with the
* license above.
*/

#include "Audeus.h"
#include <QDebug>
#include "noise_1.h"

CAudeus AUD;


CAudeus::CAudeus()
{
    numBlocks = BLOCK_SIZE / FRAMES_PER_BUFFER;

    BlocksizeTemp = 0;
    noiseLoudnessLevelTemp = 0;
    inputLoudnessLevelTemp = 0;
    gainLevelTemp = 0;
    channelTemp = 0;
    startChannel = 0;
    endChannel = 0;
    count = 0;

    output = new float *[NUM_CHANNELS]; //num_channels x blocksize
    input = new float *[NUM_CHANNELS];

    gain = new float[NUM_CHANNELS];
    gainToBe = new float[NUM_CHANNELS];
    levelMeterGain = new float[NUM_CHANNELS];

    loud_I = new float[NUM_CHANNELS];
    memset(loud_I, 0, sizeof(float) * NUM_CHANNELS);    

    numberOfNoiseLoudnessValues = 20;
    numberOfInputLoudnessValues = 20;
    numberOfGainValues = 20;

    noiseLoudnessLevelsBuffer = new float*[NUM_CHANNELS];
    noiseLoudnessLevelBufferAvg = new float[NUM_CHANNELS];
    //noiseArray = new float[numberOfNoiseLoudnessValues];
    inputLoudnessLevelsBuffer = new float*[NUM_CHANNELS];
    inputLoudnessLevelBufferAvg = new float[NUM_CHANNELS];
    memset(noiseLoudnessLevelBufferAvg, 0, sizeof(float) * NUM_CHANNELS);
    //memset(inputLoudnessLevelBufferAvg, 0, sizeof(float) * NUM_CHANNELS);


    noiseLoudnessLevelStart = "YES";
    noiseLoudnessLevelIsDone = "NO";
    inputLoudnessLevelStart = "NO";
    inputLoudnessLevelIsDone = "NO";
    gainLevelStart = "NO";
    gainLevelIsDone = "NO";

    loudnessReferenceValues = new float[NUM_CHANNELS];
    limiterThreshold = new float[NUM_CHANNELS];
    gateThreshold = new float[NUM_CHANNELS];
    loudnessValues = new float[NUM_CHANNELS];

    SLOW_TIME_CONSTANT = 0.0001;

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        output[i] = new float[BLOCK_SIZE];
        input[i] = new float[BLOCK_SIZE];
        //inputBufferForGain[i] = new float[BLOCK_SIZE];
        noiseLoudnessLevelsBuffer[i] = new float[numberOfNoiseLoudnessValues];
        inputLoudnessLevelsBuffer[i] = new float[numberOfInputLoudnessValues];
        memset(output[i], 0, sizeof(float) * BLOCK_SIZE);
        memset(input[i], 0, sizeof(float) * BLOCK_SIZE);
        //memset(noiseLoudnessLevelsBuffer[i], 0, sizeof(float) * numberOfNoiseLoudnessValues);
        //memset(inputLoudnessLevelsBuffer[i], 0, sizeof(float) * numberOfInputLoudnessValues);

        loudnessReferenceValues[i] = -24;
        loudnessValues[i] = -24;
        limiterThreshold[i] = loudnessReferenceValues[i];// + 15;
        gateThreshold[i] = loudnessReferenceValues[i] - 20;
        gain[i] = 1;
        gainToBe[i] = 1;
    }
}




void CAudeus::calculateAverage(float **input, float *output, int numberOfNoiseValues)
{
    for (int i = AUD.startChannel; i < AUD.endChannel; i++)
    {
        for (int j = 0; j < numberOfNoiseValues; j++)
        {
            output[i] += input[i][j];
            qDebug() << input[i][j];
        }

        output[i] = output[i] / numberOfNoiseValues;

    }
}


int CAudeus::AudioCallbackNoiseDetection(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    float **out = (float**)outputBuffer;
    const float **in = (const float**)inputBuffer;

    //unsigned int i;
    unsigned int j;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;

    CAudeus* AudNoise = (CAudeus*)userData;

    AUD.BlocksizeTemp = AUD.BlocksizeTemp + 1;

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        if (inputBuffer == NULL)
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                out[i][j] = 0;  // silent
            }
        }

        else
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                AUD.input[AUD.startChannel][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = in[AUD.startChannel][j];
                AUD.output[AUD.startChannel][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = in[AUD.startChannel][j];

                out[i][j] = in[AUD.startChannel][j];

            }
        }
    }

    /***************************************** THIS PART IS THE STEPS ***********************************************/


    //Check if number of samples required to calculate loudness is reached

    if (AUD.BlocksizeTemp == AUD.numBlocks)
    {
        //qDebug() << AUD.noiseLoudnessLevelTemp;

        // START WITH NOISE
        if (AUD.noiseLoudnessLevelTemp != AUD.numberOfNoiseLoudnessValues - 1)
        {
            //Calculates input loudness
            AUD.Cloud.compute_loudness(AUD.input, AUD.loud_I);

            //Store each channel loudness in temp buffer -> 10 values
            AudNoise->noiseLoudnessLevelsBuffer[AUD.startChannel][AUD.noiseLoudnessLevelTemp] = AUD.loud_I[AUD.startChannel];

            AUD.noiseLoudnessLevelTemp += 1;
        }


        //Calculate average of 20 values and move on
        else
        {
            AUD.calculateAverage(AudNoise->noiseLoudnessLevelsBuffer, AUD.noiseLoudnessLevelBufferAvg, AUD.numberOfNoiseLoudnessValues);
            qDebug() << "0" << AUD.noiseLoudnessLevelBufferAvg[0];
            qDebug() << "1" << AUD.noiseLoudnessLevelBufferAvg[1];
        }

        AUD.BlocksizeTemp = 0;
    }

    return paContinue;
}



int CAudeus::AudioCallbackInput(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    float **out = (float**)outputBuffer;
    const float **in = (const float**)inputBuffer;

    unsigned int i;
    unsigned int j;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;

    CAudeus* AudInput = (CAudeus*)userData;

    AUD.BlocksizeTemp = AUD.BlocksizeTemp + 1;

    for (i = 0; i < NUM_CHANNELS; i++)
    {
        if (inputBuffer == NULL)
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                out[i][j] = 0;  // silent
            }
        }

        else
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                //AUD->gain[i] = (AUD->SLOW_TIME_CONSTANT * AUD->gainToBe[i]) + ((1 - AUD->SLOW_TIME_CONSTANT) * AUD->gain[i]); //Moving Average filter to smooth gain

                AUD.input[i][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = in[i][j];
                //AUD.output[i][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = AUD.gain[i] * in[i][j];

                out[i][j] = in[i][j];
            }
        }
    }


    /***************************************** THIS PART IS THE STEPS ***********************************************/


    //Check if number of samples required to calculate loudness is reached

    if (AUD.BlocksizeTemp == AUD.numBlocks)
    {
        qDebug() << AUD.inputLoudnessLevelTemp;
        //CALCULATE INPUT LOUDNESS

        if (AUD.inputLoudnessLevelTemp < AUD.numberOfInputLoudnessValues - 1)
        {
            AUD.Cloud.compute_loudness(AUD.input, AUD.loud_I);

            //Store each channel loudness and input in temp buffer -> 20 values
            for (int channel = 0; channel < NUM_CHANNELS; channel++)
            {
                if (AUD.loud_I[channel] > AUD.noiseLoudnessLevelBufferAvg[channel])
                {
                    AudInput->inputLoudnessLevelsBuffer[channel][AUD.inputLoudnessLevelTemp] = AUD.loud_I[channel];
                    AUD.inputLoudnessLevelTemp += 1;
                }
            }
        }

        //Calculate average of 10 values and move on
        else
        {
            AUD.calculateAverage(AudInput->inputLoudnessLevelsBuffer, AUD.inputLoudnessLevelBufferAvg, AUD.numberOfInputLoudnessValues);
            qDebug() << "HI";

            AUD.gainLevelStart = "YES";
        }


        //CALCULATE GAIN

        if (AUD.gainLevelStart.compare("YES") == 0)
        {
            memcpy(AUD.loudnessValues, AUD.loudnessReferenceValues, sizeof(float) * NUM_CHANNELS);

            qDebug() << "Loudness Values 0 " << AUD.inputLoudnessLevelBufferAvg[0];
            qDebug() << "Loudness Values 1 " << AUD.inputLoudnessLevelBufferAvg[1];

            AUD.Cgain.cal_gain(AUD.inputLoudnessLevelBufferAvg, AUD.loudnessValues, AUD.gain);

            qDebug() << "Gain 0" << AUD.gain[0];
            qDebug() << "Gain 1" << AUD.gain[1];


            AUD.gainLevelIsDone = "YES";
        }


        AUD.BlocksizeTemp = 0;
    }

    return paContinue;
}


int CAudeus::AudioCallback(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    float **out = (float**)outputBuffer;
    const float **in = (const float**)inputBuffer;

    unsigned int i;
    unsigned int j;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;

    CAudeus* AudMain = (CAudeus*)userData;

    if (AUD.count > 300)
        AUD.BlocksizeTemp = AUD.BlocksizeTemp + 1;

    for (i = 0; i < NUM_CHANNELS; i++)
    {
        if (inputBuffer == NULL)
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                out[i][j] = 0;  // silent
            }
        }

        else
        {            
            if (AUD.count > 300 && AUD.BlocksizeTemp == AUD.numBlocks)
            {
                AUD.Climiter.compute_limiter(AUD.output, AUD.loudnessReferenceValues, AUD.limiterThreshold, AUD.gain, AUD.gainToBe);
                qDebug() << "GAIN" << AUD.gain[i];

                AUD.BlocksizeTemp = 0;

            }


            for (j = 0; j < framesPerBuffer; j++)
            {
                AUD.gain[i] = (AUD.SLOW_TIME_CONSTANT * AUD.gainToBe[i]) + ((1 - AUD.SLOW_TIME_CONSTANT) * AUD.gain[i]); //Moving Average filter to smooth gain


                //AUD.input[i][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = in[i][j];
                if (AUD.count > 300)
                    AUD.output[i][(j + (framesPerBuffer * AUD.BlocksizeTemp))] = AUD.gain[i] * in[i][j];

                out[i][j] = AUD.gain[i] * in[i][j];

            }
        }
    }

    if (AUD.count < 302)
        AUD.count = AUD.count + 1;

    return paContinue;
}



int CAudeus::AudioCallbackFreeze(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    float **out = (float**)outputBuffer;
    const float **in = (const float**)inputBuffer;

    unsigned int i;
    unsigned int j;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;

    CAudeus* AudFreeze = (CAudeus*)userData;

    AudFreeze->BlocksizeTemp = AudFreeze->BlocksizeTemp + 1;

    for (i = 0; i < NUM_CHANNELS; i++)
    {
        if (inputBuffer == NULL)
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                out[i][j] = 0;  // silent
            }
        }

        else
        {
            for (j = 0; j < framesPerBuffer; j++)
            {
                out[i][j] = AUD.gain[i] * in[i][j];
            }
        }
    }

    return paContinue;
}
