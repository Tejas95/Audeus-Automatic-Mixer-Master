#include "audiowrapper.h"

extern CAudeus AUD;

audioWrapper::audioWrapper(QObject *parent) : QObject(parent)
{
    mStopAudio = false;
}

int audioWrapper::startAudioFreeze(void)
{
    PaError err;

    mStopAudio = false;

    PaStreamParameters inputParameters, outputParameters;

      err = Pa_Initialize();
     if( err != paNoError ) goto error;

     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
     if (inputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default input device.\n");
         goto error;
     }
     inputParameters.channelCount = NUM_CHANNELS;
     inputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
     inputParameters.hostApiSpecificStreamInfo = NULL;

     outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
     if (outputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default output device.\n");
         goto error;
     }

     outputParameters.channelCount = NUM_CHANNELS;
     outputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
     outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0,
                        AUD.AudioCallbackFreeze,
                        (void*)&AUD);


    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if(mStopAudio) return 0;
    if( err != paNoError ) goto error;

    printf("Hit ENTER to stop program.\n");
    getchar();
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    return 0;

  error:

      Pa_Terminate();
      fprintf( stderr, "An error occured while using the portaudio stream\n" );
      fprintf( stderr, "Error number: %d\n", err );
      fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
      return -1;
}

int audioWrapper::startAudioNoise(void)
{
    PaError err;

    mStopAudio = false;

    PaStreamParameters inputParameters, outputParameters;

      err = Pa_Initialize();
     if( err != paNoError )
         goto error;

     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
     if (inputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default input device.\n");
         goto error;
     }
     inputParameters.channelCount = NUM_CHANNELS;
     inputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
     inputParameters.hostApiSpecificStreamInfo = NULL;

     outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
     if (outputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default output device.\n");
         goto error;
     }

     outputParameters.channelCount = NUM_CHANNELS;
     outputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
     outputParameters.hostApiSpecificStreamInfo = NULL;


    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0,
                        AUD.AudioCallbackNoiseDetection,
                        (void*)&AUD);


    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );


    //if(mStopAudio) return -1;

    //printf("Hit ENTER to stop program.\n");
    //getchar();
    //err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    //Pa_Terminate();
    //return 0;

  error:

    err = Pa_CloseStream( stream );

      Pa_Terminate();
//      fprintf( stderr, "An error occured while using the portaudio stream\n" );
//      fprintf( stderr, "Error number: %d\n", err );
//      fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
      return -1;
}


int audioWrapper::startAudioInput(void)
{

    PaError err;

    PaStreamParameters inputParameters, outputParameters;

    mStopAudio = false;

      err = Pa_Initialize();
     if( err != paNoError ) goto error;

     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
     if (inputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default input device.\n");
         goto error;
     }
     inputParameters.channelCount = NUM_CHANNELS;
     inputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
     inputParameters.hostApiSpecificStreamInfo = NULL;

     outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
     if (outputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default output device.\n");
         goto error;
     }

     outputParameters.channelCount = NUM_CHANNELS;
     outputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
     outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
     outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0,
                        AUD.AudioCallbackInput,
                        (void*)&AUD);


    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if(mStopAudio) return 0;
    if( err != paNoError )
        stopAudio();

    printf("Hit ENTER to stop program.\n");
    getchar();
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    return 0;

  error:

    err = Pa_CloseStream( stream );
      Pa_Terminate();
      fprintf( stderr, "An error occured while using the portaudio stream\n" );
      fprintf( stderr, "Error number: %d\n", err );
      fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
      return -1;
}


int audioWrapper::startAudio(void)
{

   AUD.startChannel = 0;
   AUD.endChannel = NUM_CHANNELS;
   AUD.count = 0;
   AUD.BlocksizeTemp = 0;

   AUD.gainToBe = AUD.gain;


  PaError err;


  PaStreamParameters inputParameters, outputParameters;

    err = Pa_Initialize();
   if( err != paNoError ) goto error;

   inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
   if (inputParameters.device == paNoDevice) {
       fprintf(stderr,"Error: No default input device.\n");
       goto error;
   }
   inputParameters.channelCount = NUM_CHANNELS;
   inputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
   inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency;
   inputParameters.hostApiSpecificStreamInfo = NULL;

   outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
   if (outputParameters.device == paNoDevice) {
       fprintf(stderr,"Error: No default output device.\n");
       goto error;
   }

   outputParameters.channelCount = NUM_CHANNELS;
   outputParameters.sampleFormat = PA_SAMPLE_TYPE | paNonInterleaved;
   outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
   outputParameters.hostApiSpecificStreamInfo = NULL;

  err = Pa_OpenStream(&stream,
                      &inputParameters,
                      &outputParameters,
                      SAMPLE_RATE,
                      FRAMES_PER_BUFFER,
                      0,
                      AUD.AudioCallback,
                      (void*)&AUD);


  if( err != paNoError ) goto error;


    emit on_gain(AUD.gain);
    err = Pa_StartStream( stream );



  if( err != paNoError ) goto error;

  printf("Hit ENTER to stop program.\n");
  getchar();
  err = Pa_CloseStream( stream );
  if( err != paNoError ) goto error;

  Pa_Terminate();
  return 0;

error:

    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;

}


void audioWrapper::stopAudio()
{

    mStopAudio = true;

    //emit noiseIsComplete("Done");

    int err=-1;

    err = Pa_AbortStream( stream );

    //if( err != paNoError ) return err;

    err = Pa_CloseStream( stream );

    Pa_Terminate();

  //return err;
}

