#ifndef ALLPASSPATCH_H
#define ALLPASSPATCH_H

#include "PatchDialog.h"

#include "Filters/allpass.h"
#include "Filters/audiofileinput.h"
#include "Framework/AudioFileOutput.h"

class AllPassPatch : public PatchDialog
{
public:
    AllPassPatch(QWidget *parent);
    ~AllPassPatch();

    void createPatch() override;
private:

    // filters
    AudioFileInput* audioinput = nullptr;
    AllPass* allpass = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // ALLPASSPATCH_H
