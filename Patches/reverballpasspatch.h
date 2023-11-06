#ifndef REVERBALLPASSPATCH_H
#define REVERBALLPASSPATCH_H

#include "PatchDialog.h"

#include "Filters/allpass.h"
#include "Framework/AudioOutput.h"
#include "Filters/audiofileinput.h"
#include "Framework/AudioFileOutput.h"

class ReverbAllpassPatch : public PatchDialog
{
public:
    ReverbAllpassPatch(QWidget *parent);
    ~ReverbAllpassPatch();

    void createPatch() override;
private:

    // filters

    AudioFileInput* audioinput = nullptr;

    AllPass* allpass1 = nullptr;
    AllPass* allpass2 = nullptr;
    AllPass* allpass3 = nullptr;
    AllPass* allpass4 = nullptr;

    AudioFileOutput* audioOutput = nullptr;
};

#endif // REVERBALLPASSPATCH_H
