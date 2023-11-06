#include "allpasspatch.h"

AllPassPatch::AllPassPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("AllPass patch");

    initializePatch();
}

AllPassPatch::~AllPassPatch()
{
    engine->deactivate();

    delete audioinput;
    delete allpass;
    delete audioOutput;
}

void AllPassPatch::createPatch()
{
    audioinput = new AudioFileInput(engine, "Audio File Input");


    allpass = new AllPass(engine, "Allpass");
    allpass->setDelay(75);
    allpass->setGValue(75);

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(audioinput);
    engine->append(allpass);
    engine->append(audioOutput);

    audioinput->connectOutputPort(0, allpass, 0);
    allpass->connectOutputPort(0,audioOutput,0);
}
