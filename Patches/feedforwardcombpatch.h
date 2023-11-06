#ifndef FEEDFORWARDCOMBPATCH_H
#define FEEDFORWARDCOMBPATCH_H

#include "PatchDialog.h"

#include "Filters/mixer.h"
#include "Filters/delay.h"
#include "Filters/audiofileinput.h"
#include "Framework/AudioOutput.h"
#include "Framework/AudioFileOutput.h"

class FeedforwardCombPatch : public PatchDialog
{
public:
    FeedforwardCombPatch(QWidget *parent);
    ~FeedforwardCombPatch();

    void createPatch() override;
private:

    // filters
    AudioFileInput* audioinput = nullptr;
    Delay* delay = nullptr;
    Mixer* mixer = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
