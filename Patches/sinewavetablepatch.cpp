#include "sinewavetablepatch.h"

SineWaveTablePatch::SineWaveTablePatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Sine wavetable patch");

    initializePatch();
}

SineWaveTablePatch::~SineWaveTablePatch()
{
    engine->deactivate();

    delete sineOscilator;
    delete audioOutput;
}

void SineWaveTablePatch::createPatch()
{

    sineOscilator = new SineWavetable(engine, "Sine Oscilator", 50);
    sineOscilator->setFrequency(500);
    sineOscilator->setPhaseShift(50); //180 degrees phase shift

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(sineOscilator);
    engine->append(audioOutput);

    sineOscilator->connectOutputPort(0, audioOutput, 0);
}
