#include "etapatch.h"

ETAPatch::ETAPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Exponential time averaging patch");

    initializePatch();
}

ETAPatch::~ETAPatch()
{
    engine->deactivate();

    delete input;
    delete eta;
    delete audioOutput;
}

void ETAPatch::createPatch()
{
    input = new AudioFileInput(engine, "File input 1");

    eta = new ETAFilter(engine, "ETA filter");

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(input);
    engine->append(eta);
    engine->append(audioOutput);

    input->connectOutputPort(0, eta, 0);
    eta->connectOutputPort(0,audioOutput,0);
}
