#include "noise_1.h"
#include "ui_noise_1.h"
#include "noise_2.h"
#include "Audeus.h"
#include "mainwindow.h"

extern CAudeus AUD;

Noise_1::Noise_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Noise_1)
{
    ui->setupUi(this);

}

Noise_1::~Noise_1()
{
    delete ui;
}

void Noise_1::on_PBSkip_clicked()
{
    this->close();

    Noise_2 noise2;
    noise2.setModal(true);
    noise2.exec();
}

void Noise_1::on_PBStart_clicked()
{
    CAudeus AUD1;

    AUD.startChannel = 0;
    AUD.endChannel = 1;
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
                        AUD1.AudioCallbackNoiseDetection,
                        (void*)&AUD1);

    if( err != paNoError ) goto error;



    err = Pa_StartStream(stream);
    if( err != paNoError ) goto error;

    Pa_Sleep( 8 * 1000 );

    err = Pa_StopStream(stream);
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    ui->label->setText("Done");


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

void Noise_1::on_PBNext_clicked()
{
    //emit on_stop();

    this->close();

    Noise_2 noise2;
    noise2.setModal(true);
    noise2.exec();



}

void Noise_1::onNoise1IsComplete(QString status)
{
    ui->label->setText(status);
}
