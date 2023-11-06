#ifndef AUDIOFILEOUTPUT_H
#define AUDIOFILEOUTPUT_H

#include "Audio.h"
#include "sndfile.h"

class AudioFileOutput : public AudioFilter
{
    class AudioOutputChannel **channel;
    int nrChannels;

public:
    AudioFileOutput(AudioEngine* engine, const std::string &name);
    ~AudioFileOutput();

    // access output ports in AudioEngine object, output member variable.
    std::string getDescription(void);


    // initialize manually
    void createChannels(int nrChannels);

protected:
    // AudioFilter callbacks
    void activated(void);
    void deactivated();
    void process(int nFrames);

private:
    std::string m_filename;
    SaveFilePropertyPtr p_filename;

    SF_INFO sfinfo;
    SNDFILE* m_sf;
};


#endif // AUDIOFILEOUTPUT_H
