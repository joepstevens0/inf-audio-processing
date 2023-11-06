#ifndef FILEINPUTPATCH_H
#define FILEINPUTPATCH_H

#include "PatchDialog.h"

#include "Filters/audiofileinput.h"
#include "Framework/AudioFileOutput.h"

class FileInputPatch : public PatchDialog
{
public:
    FileInputPatch(QWidget *parent);
    ~FileInputPatch ();

    void createPatch() override;
private:

    // filters
    AudioFileInput* audioinput = nullptr;
    AudioFileOutput* audioOutput = nullptr;
};

#endif // TESTPATCH_H
