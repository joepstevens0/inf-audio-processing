#ifndef SINEOSCILATORPATCH_H
#define SINEOSCILATORPATCH_H

#include "PatchDialog.h"

#include "Filters/SineOscilator.h"
#include "Framework/AudioOutput.h"

#include "Framework/AudioFileOutput.h"

//Derive from PatchDialog
class SineOscilatorPatch : public PatchDialog
{
public:
    //Constructor/Destructor
    SineOscilatorPatch(QWidget *parent);
    ~SineOscilatorPatch();

    //Build the patch
    void createPatch();
private:

    //The filters
    SineOscilator* sineOscilator;
    AudioFileOutput* audioOutput;
};

#endif // SINEOSCILATORPATCH_H
