#ifndef REVERBCOMBPATCH_H
#define REVERBCOMBPATCH_H

#include "PatchDialog.h"

#include "Filters/delay.h"
#include "Filters/mixer.h"
#include "Filters/allpass.h"
#include "Framework/AudioOutput.h"
#include "Filters/audiofileinput.h"
#include "Framework/AudioFileOutput.h"

class ReverbCombPatch : public PatchDialog
{
public:
    ReverbCombPatch(QWidget *parent);
    ~ReverbCombPatch();

    void createPatch() override;
private:

    // filters

    AudioFileInput* audioinput = nullptr;

    Delay* comb1delay = nullptr;
    Mixer* comb1mixer = nullptr;
    Delay* comb2delay = nullptr;
    Mixer* comb2mixer = nullptr;
    Delay* comb3delay = nullptr;
    Mixer* comb3mixer = nullptr;
    Delay* comb4delay = nullptr;
    Mixer* comb4mixer = nullptr;

    Mixer* mix1and2 = nullptr;
    Mixer* mix3and4 = nullptr;
    Mixer* mixer = nullptr;

    AllPass* allpass1 = nullptr;
    AllPass* allpass2 = nullptr;

    AudioFileOutput* audioOutput = nullptr;
};

#endif // REVERBCOMBPATCH_H
