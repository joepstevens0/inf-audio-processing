#include "reverballpasspatch.h"

ReverbAllpassPatch::ReverbAllpassPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Reverb allpass patch");

    initializePatch();
}

ReverbAllpassPatch::~ReverbAllpassPatch()
{
    engine->deactivate();

    delete audioinput;
    delete allpass1;
    delete allpass2;
    delete allpass3;
    delete allpass4;
    delete audioOutput;
}

void ReverbAllpassPatch::createPatch()
{
    //// create filters
    // input
    audioinput = new AudioFileInput(engine, "Audio File Input");
    // allpass 1
    allpass1 = new AllPass(engine, "Allpass 1");
    allpass1->setDelay(19);
    allpass1->setGValue(90);
    // allpass 2
    allpass2 = new AllPass(engine, "Allpass 2");
    allpass2->setDelay(39);
    allpass2->setGValue(90);
    // allpass 3
    allpass3 = new AllPass(engine, "Allpass 3");
    allpass3->setDelay(59);
    allpass3->setGValue(90);
    // allpass 4
    allpass4 = new AllPass(engine, "Allpass 4");
    allpass4->setDelay(69);
    allpass4->setGValue(90);
    // output
    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    //// add filters
    engine->append(audioinput);
    engine->append(allpass1);
    engine->append(allpass2);
    engine->append(allpass3);
    engine->append(allpass4);
    engine->append(audioOutput);

    //// connect filters
    // comb 1
    audioinput->connectOutputPort(0, allpass1, 0);
    allpass1->connectOutputPort(0,allpass2,0);
    allpass2->connectOutputPort(0,allpass3,0);
    allpass3->connectOutputPort(0,allpass4,0);
    allpass4->connectOutputPort(0,audioOutput,0);
}
