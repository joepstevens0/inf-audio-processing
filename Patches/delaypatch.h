#ifndef DELAYPATCH_H
#define DELAYPATCH_H

#include "PatchDialog.h"

#include "Filters/audiofileinput.h"
#include "Filters/delay.h"
#include "Filters/mixer.h"
#include "Framework/AudioFileOutput.h"

class DelayPatch : public PatchDialog
{
public:
    DelayPatch(QWidget *parent);
    ~DelayPatch ();

    void createPatch() override;
private:

    // filters
    AudioFileInput* input1 = nullptr;
    AudioFileInput* input2 = nullptr;
    Delay* delay = nullptr;
    Mixer* mixer = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
