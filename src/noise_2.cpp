#include "noise_2.h"
#include "ui_noise_2.h"
#include "noise_3.h"
#include "noise_1.h"
#include "Audeus.h"
#include "audiowrapper.h"

extern CAudeus AUD;

Noise_2::Noise_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Noise_2)
{
    ui->setupUi(this);

//        AUD.noiseLoudnessLevelsBuffer = new float*[NUM_CHANNELS];
//        for (int i = 0; i < NUM_CHANNELS; i++)
//        {
//            AUD.noiseLoudnessLevelsBuffer[i] = new float[AUD.numberOfNoiseLoudnessValues];
//            //memset(AUD.noiseLoudnessLevelsBuffer, 0, sizeof(float) * NUM_CHANNELS);
//        }
}

Noise_2::~Noise_2()
{
    delete ui;
}

void Noise_2::on_PBSkip_clicked()
{
    this ->close();

    Noise_3 noise3;
    noise3.setModal(true);
    noise3.exec();
}

void Noise_2::on_PBStart_clicked()
{
    CAudeus AUD2;

    AUD.startChannel = 1;
    AUD.endChannel = 2;

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
                        AUD2.AudioCallbackNoiseDetection,
                        (void*)&AUD2);

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

void Noise_2::on_PBNext_clicked()
{

    this ->close();

    Noise_3 noise3;
    noise3.setModal(true);
    noise3.exec();

}

void Noise_2::onNoise2IsComplete(QString status)
{
    ui->label->setText(status);
}
