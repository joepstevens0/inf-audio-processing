#include "audiofileinput.h"

#include "Framework/AudioIO.h"
#include "Framework/AudioInput.h"

#include <sndfile.h>

AudioFileInput::AudioFileInput(AudioEngine* engine, const std::string &name)
    : AudioFilter(engine, name, 0, 1)
{
    m_filename = "in.wav";
    p_filename = Properties::createOpenFileProperty("File", name, &m_filename);
    engine->getProperties()->addProperty(p_filename);

    m_loop = true;
    p_loop = Properties::createBooleanProperty("Loop", name, &m_loop);
    engine->getProperties()->addProperty(p_loop);
}

AudioFileInput::~AudioFileInput()
{
    if (m_sf)
    {
        sf_close(m_sf);
        m_sf = nullptr;
    }
}


std::string AudioFileInput::getDescription()
{
    return "AudioFileInput";
}

void AudioFileInput::activated()
{
    //File input: open
    memset(&sfinfo, 0, sizeof(sfinfo));
    m_sf = sf_open(m_filename.c_str(), SFM_READ, &sfinfo);
    std::cout << m_sf << std::endl;
    if (!m_sf)
        std::cerr << "Input from file '" << m_filename << "': failed to open file." << std::endl;
    else
        std::cerr << "Input from file '" << m_filename << "': playing audio from file." << std::endl;

    m_buffer.resize(engine->frameRate*sfinfo.channels);
}

void AudioFileInput::deactivated()
{
    if (m_sf){
        sf_close(m_sf);
        m_sf = nullptr;
        std::cout << "Audio playing finished." << std::endl;
    }
}

void AudioFileInput::process(int nFrames)
{
    // read from audio file
    if (m_sf){
        if(sf_readf_float(m_sf, m_buffer.data(), nFrames) != nFrames){

            std::cout << "Input from file '" << m_filename << "': finished playing audio." << std::endl;
            if (m_loop){
                std::cout << "Looping audio" << std::endl;
                sf_seek(m_sf, 0, SEEK_SET);
            }
            else{
                sf_close(m_sf);
                m_sf = nullptr;
            }
        }
        int channels = sfinfo.channels;
        std::fill(&outputs[0]->samples[0], &outputs[0]->samples[nFrames],0);
        for (int i = 0; i < nFrames;++i){
            // take the average of all input channels
            for (int channel = 0; channel < channels;++channel)
                outputs[0]->samples[i] += m_buffer[i*channels + channel];
            outputs[0]->samples[i] /= channels;
        }
    } else{
        std::fill(&outputs[0]->samples[0], &outputs[0]->samples[nFrames], 0);
    }
}
