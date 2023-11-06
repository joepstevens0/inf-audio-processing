#include "SineOscilatorPatch.h"

SineOscilatorPatch::SineOscilatorPatch(QWidget* parent) : PatchDialog(parent)
{
    //Set the title of the window
    setWindowTitle("Sine Oscilator Patch");

    //Initialize the patch
    initializePatch();
}

SineOscilatorPatch::~SineOscilatorPatch()
{
    //Deactivate the engine upon destruction
    engine->deactivate();

    //Remove filters
    delete sineOscilator;
    delete audioOutput;
}

void SineOscilatorPatch::createPatch()
{
    // create AudioOutput filter - appended to the audio engine processing chain
    // as the last filter (see below).
    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    // create the sine oscilator and set Frequency and Phase Shift
    sineOscilator = new SineOscilator(engine, "Sine Oscilator");
    sineOscilator->setFrequency(500);
    sineOscilator->setPhaseShift(0.5); //180 degrees phase shift

    //Append filters to engine. This determines the execution order in each processing loop. Choose it carefully.
    engine->append(sineOscilator);
    engine->append(audioOutput);

    //Connect output port 0 from the oscilator to input port 0 of the output device
    sineOscilator->connectOutputPort(0, audioOutput, 0);
}

