#ifndef AUDIOFILEINPUT_H
#define AUDIOFILEINPUT_H

#include "Framework/Audio.h"
#include "sndfile.h"


class AudioFileInput : public AudioFilter
{
public:
    AudioFileInput(AudioEngine* engine, const std::string &name);
    ~AudioFileInput();

    std::string getDescription(void) override;

protected:
    void activated(void) override;
    void deactivated() override;
    void process(int nFrames) override;

private:
    std::string m_filename;
    OpenFilePropertyPtr p_filename;
    bool m_loop;
    BooleanPropertyPtr p_loop;

    SF_INFO sfinfo;
    SNDFILE* m_sf = nullptr;

    int m_bufferP;
    std::vector<float> m_buffer;
};

#endif // AUDIOFILEINPUT_H
