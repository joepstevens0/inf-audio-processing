#include "fileinputpatch.h"

FileInputPatch::FileInputPatch(QWidget* parent) : PatchDialog(parent)
{
    setWindowTitle("File input patch");

    initializePatch();
}

FileInputPatch::~FileInputPatch()
{
    engine->deactivate();

    delete audioinput;
    delete audioOutput;
}

void FileInputPatch::createPatch()
{
    audioinput = new AudioFileInput(engine, "Audio File Input");

    audioOutput = new AudioFileOutput(engine, "Audio File Output");
    audioOutput->createChannels(2);   // create two default output channels

    engine->append(audioinput);
    engine->append(audioOutput);

    audioinput->connectOutputPort(0, audioOutput, 0);
}
