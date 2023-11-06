#ifndef ETAPATCH_H
#define ETAPATCH_H

#include "PatchDialog.h"

#include "Filters/audiofileinput.h"
#include "Filters/etafilter.h"
#include "Framework/AudioFileOutput.h"

class ETAPatch: public PatchDialog
{
public:
    ETAPatch(QWidget *parent);
    ~ETAPatch();

    void createPatch() override;
private:

    // filters
    AudioFileInput* input = nullptr;
    ETAFilter* eta = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
