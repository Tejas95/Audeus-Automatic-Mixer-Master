#include "noise_5.h"
#include "ui_noise_5.h"
#include "noise_6.h"
#include "noise_4.h"
#include "Audeus.h"
#include "audiowrapper.h"

extern CAudeus AUD;

Noise_5::Noise_5(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Noise_5)
{
    ui->setupUi(this);

}

Noise_5::~Noise_5()
{
    delete ui;
}

void Noise_5::on_PBSkip_clicked()
{
    this->close();

    Noise_6 noise6;
    noise6.setModal(true);
    noise6.exec();
}

void Noise_5::on_PBStart_clicked()
{
    CAudeus AUD5;

    AUD.startChannel = 4;
    AUD.endChannel = 5;

    AUD.BlocksizeTemp = 0;
    AUD.noiseLoudnessLevelTemp = 0;

    PaError err;

    //mStopAudio = false;

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
                        AUD5.AudioCallbackNoiseDetection,
                        (void*)&AUD5);

    if( err != paNoError ) goto error;



    err = Pa_StartStream(stream);
    if( err != paNoError ) goto error;

    Pa_Sleep( 8 * 1000 );

    err = Pa_StopStream(stream);
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    ui->label->setText("Done");


    //connect(&AUD, &CAudeus::on_noise, this,  &Noise_1::onNoise1IsComplete);
    //QFuture<PaError> err1 = QtConcurrent::run(Pa_StartStream(), PaStream(stream));

    //err = Pa_StartStream( stream );

    //emit on_noise1("Done");


    //if(mStopAudio) return -1;

    //printf("Hit ENTER to stop program.\n");
    //getchar();
    //err = Pa_CloseStream( stream );

    //if( err != paNoError ) goto error;

    Pa_Terminate();
    //return 0;

  error:

      Pa_Terminate();
//      fprintf( stderr, "An error occured while using the portaudio stream\n" );
//      fprintf( stderr, "Error number: %d\n", err );
//      fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
     // return -1;


    //connect(this, &Noise_1::on_stop, &ap, &audioWrapper::stopAudio);

}

void Noise_5::on_PBNext_clicked()
{ 
    this ->close();

    Noise_6 noise6;
    noise6.setModal(true);
    noise6.exec();

}

void Noise_5::onNoise5IsComplete(QString status)
{
    ui->label->setText(status);
}
