#include "delaypatch.h"

DelayPatch::DelayPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Delay patch");

    initializePatch();
}

DelayPatch::~DelayPatch()
{
    engine->deactivate();

    delete input1;
    delete input2;
    delete delay;
    delete mixer;
    delete audioOutput;
}

void DelayPatch::createPatch()
{
    input1 = new AudioFileInput(engine, "File input 1");
    input2 = new AudioFileInput(engine, "File input 2 (delayed)");

    delay = new Delay(engine, "File input 2 delay");
    delay->setDelay(1000);

    mixer = new Mixer(engine, "Audio mixer");
    mixer->setAlpha1(100);
    mixer->setAlpha2(100);

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(input1);
    engine->append(input2);
    engine->append(delay);
    engine->append(mixer);
    engine->append(audioOutput);

    input1->connectOutputPort(0, mixer, 0);
    input2->connectOutputPort(0, delay, 0);
    delay->connectOutputPort(0,mixer,1);
    mixer->connectOutputPort(0,audioOutput,0);
}
