#ifndef MIXERPATCH_H
#define MIXERPATCH_H

#include "PatchDialog.h"

#include "Filters/sinewavetable.h"
#include "Filters/mixer.h"
#include "Framework/AudioFileOutput.h"

class MixerPatch : public PatchDialog
{
public:
    MixerPatch(QWidget *parent);
    ~MixerPatch ();

    void createPatch() override;
private:

    // filters
    SineWavetable* sine = nullptr;
    SineWavetable* sine2 = nullptr;
    Mixer* mixer = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
