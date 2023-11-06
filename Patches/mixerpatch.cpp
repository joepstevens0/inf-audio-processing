#include "mixerpatch.h"

MixerPatch::MixerPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Mixer patch");

    initializePatch();
}

MixerPatch::~MixerPatch()
{
    engine->deactivate();

    delete sine;
    delete sine2;
    delete mixer;
    delete audioOutput;
}

void MixerPatch::createPatch()
{
    sine = new SineWavetable(engine, "Sine wavetable oscillator 1", 100);
    sine->setFrequency(500);
    sine2 = new SineWavetable(engine, "Sine wavetable oscillator 2", 100);
    sine2->setFrequency(500);

    mixer = new Mixer(engine, "Sine mixer");

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(sine);
    engine->append(sine2);
    engine->append(mixer);
    engine->append(audioOutput);

    sine->connectOutputPort(0, mixer, 0);
    sine2->connectOutputPort(0, mixer, 1);
    mixer->connectOutputPort(0,audioOutput,0);
}
