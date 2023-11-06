#include "reverbcombpatch.h"

ReverbCombPatch::ReverbCombPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("Reverb comb patch");

    initializePatch();
}

ReverbCombPatch::~ReverbCombPatch()
{
    engine->deactivate();

    delete audioinput;
    delete comb1delay;
    delete comb1mixer;
    delete comb2delay;
    delete comb2mixer;
    delete comb3delay;
    delete comb3mixer;
    delete comb4delay;
    delete comb4mixer;
    delete mix1and2;
    delete mix3and4;
    delete mixer;
    delete allpass1;
    delete allpass2;
    delete audioOutput;
}

void ReverbCombPatch::createPatch()
{
    //// create filters
    // input
    audioinput = new AudioFileInput(engine, "Audio File Input");
    // comb filter 1
    comb1delay = new Delay(engine, "Comb1Delay");
    comb1delay->setDelay(100);
    comb1mixer = new Mixer(engine, "Comb1Mixer");
    comb1mixer->setAlpha1(75);
    comb1mixer->setAlpha2(25);
    // comb filter 2
    comb2delay = new Delay(engine, "Comb2Delay");
    comb2delay->setDelay(123);
    comb2mixer = new Mixer(engine, "Comb2Mixer");
    comb2mixer->setAlpha1(75);
    comb2mixer->setAlpha2(25);
    // comb filter 3
    comb3delay = new Delay(engine, "Comb3Delay");
    comb3delay->setDelay(149);
    comb3mixer = new Mixer(engine, "Comb3Mixer");
    comb3mixer->setAlpha1(75);
    comb3mixer->setAlpha2(25);
    // comb filter 4
    comb4delay = new Delay(engine, "Comb4Delay");
    comb4delay->setDelay(171);
    comb4mixer = new Mixer(engine, "Comb4Mixer");
    comb4mixer->setAlpha1(75);
    comb4mixer->setAlpha2(25);
    // mixer 1 and 2
    mix1and2 = new Mixer(engine, "Mixer comb 1 and 2");
    mix1and2->setAlpha1(50);
    mix1and2->setAlpha2(50);
    // mixer 3 and 4
    mix3and4 = new Mixer(engine, "Mixer comb 3 and 4");
    mix3and4->setAlpha1(50);
    mix3and4->setAlpha2(50);
    // comb result mixer
    mixer = new Mixer(engine, "Comb result mixer");
    mixer->setAlpha1(50);
    mixer->setAlpha2(50);
    // allpass 1
    allpass1 = new AllPass(engine, "Allpass 1");
    allpass1->setDelay(30);
    // allpass 2
    allpass2 = new AllPass(engine, "Allpass 2");
    allpass2->setDelay(30);
    // output
    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    //// add filters
    engine->append(audioinput);
    engine->append(comb1delay);
    engine->append(comb1mixer);
    engine->append(comb2delay);
    engine->append(comb2mixer);
    engine->append(comb3delay);
    engine->append(comb3mixer);
    engine->append(comb4delay);
    engine->append(comb4mixer);
    engine->append(mix1and2);
    engine->append(mix3and4);
    engine->append(mixer);
    engine->append(allpass1);
    engine->append(allpass2);
    engine->append(audioOutput);

    //// connect filters
    // comb 1
    audioinput->connectOutputPort(0, comb1delay, 0);
    audioinput->connectOutputPort(0, comb1mixer, 0);
    comb1delay->connectOutputPort(0, comb1mixer, 1);
    // comb 2
    audioinput->connectOutputPort(0, comb2delay, 0);
    audioinput->connectOutputPort(0, comb2mixer, 0);
    comb1delay->connectOutputPort(0, comb2mixer, 1);
    // comb 3
    audioinput->connectOutputPort(0, comb3delay, 0);
    audioinput->connectOutputPort(0, comb3mixer, 0);
    comb1delay->connectOutputPort(0, comb3mixer, 1);
    // comb 4
    audioinput->connectOutputPort(0, comb4delay, 0);
    audioinput->connectOutputPort(0, comb4mixer, 0);
    comb1delay->connectOutputPort(0, comb4mixer, 1);
    // mix comb results
    comb1mixer->connectOutputPort(0, mix1and2, 0);
    comb2mixer->connectOutputPort(0, mix1and2, 1);
    comb3mixer->connectOutputPort(0, mix3and4, 0);
    comb4mixer->connectOutputPort(0, mix3and4, 1);
    mix1and2->connectOutputPort(0, mixer, 0);
    mix3and4->connectOutputPort(0, mixer, 1);
    // allpass
    mixer->connectOutputPort(0, allpass1, 0);
    allpass1->connectOutputPort(0,allpass2,0);
    allpass2->connectOutputPort(0,audioOutput,0);
}
