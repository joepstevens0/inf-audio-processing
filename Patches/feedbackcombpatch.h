#ifndef FEEDBACKCOMBPATCH_H
#define FEEDBACKCOMBPATCH_H

#include "PatchDialog.h"

#include "Filters/mixer.h"
#include "Filters/delay.h"
#include "Filters/audiofileinput.h"
#include "Framework/AudioOutput.h"
#include "Framework/AudioFileOutput.h"

class FeedbackCombPatch : public PatchDialog
{
public:
    FeedbackCombPatch(QWidget *parent);
    ~FeedbackCombPatch();

    void createPatch() override;
private:

    // filters
    AudioFileInput* audioinput = nullptr;
    Delay* delay = nullptr;
    Mixer* mixer = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
