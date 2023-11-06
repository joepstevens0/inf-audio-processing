#ifndef SINEWAVETABLEPATCH_H
#define SINEWAVETABLEPATCH_H

#include "PatchDialog.h"

#include "Filters/sinewavetable.h"
#include "Framework/AudioOutput.h"
#include "Framework/AudioFileOutput.h"

class SineWaveTablePatch : public PatchDialog
{
public:
    SineWaveTablePatch(QWidget *parent);
    ~SineWaveTablePatch();

    void createPatch() override;
private:

    // filters

    SineWavetable* sineOscilator = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
