#include "feedbackcombpatch.h"

FeedbackCombPatch::FeedbackCombPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Feedback comb patch");

    initializePatch();
}

FeedbackCombPatch::~FeedbackCombPatch()
{
    engine->deactivate();

    delete audioinput;
    delete mixer;
    delete delay;
    delete audioOutput;
}

void FeedbackCombPatch::createPatch()
{
    audioinput = new AudioFileInput(engine, "Audio File Input");
    delay = new Delay(engine, "Delay");
    delay->setDelay(500);
    mixer = new Mixer(engine, "Mixer");
    mixer->setAlpha1(75);
    mixer->setAlpha2(25);
    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(audioinput);
    engine->append(delay);
    engine->append(mixer);
    engine->append(audioOutput);

    audioinput->connectOutputPort(0, mixer, 0);
    delay->connectOutputPort(0,mixer,1);

    mixer->connectOutputPort(0,delay,0);
    mixer->connectOutputPort(0,audioOutput,0);
}
