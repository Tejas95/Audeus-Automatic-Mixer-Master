#include "input_loudness.h"
#include "ui_input_loudness.h"
#include "noise_6.h"
#include "audiowrapper.h"
#include "Audeus.h"
#include "mainwindow.h"

extern CAudeus AUD;

Input_Loudness::Input_Loudness(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Input_Loudness)
{
    ui->setupUi(this);

}

Input_Loudness::~Input_Loudness()
{
    delete ui;
}

void Input_Loudness::on_PBNext_clicked(void)
{
    this->close();
}

void Input_Loudness::on_PBStart_clicked()
{
    CAudeus AUDIn;

    AUD.startChannel = 0;
    AUD.endChannel = NUM_CHANNELS;
    AUD.BlocksizeTemp = 0;
    AUD.inputLoudnessLevelTemp = 0;

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
                        AUDIn.AudioCallbackInput,
                        (void*)&AUDIn);

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


}

void Input_Loudness::onInputLoudnessIsComplete(QString status)
{
    ui->label->setText(status);
}
