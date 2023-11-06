#include "AudioFileOutput.h"

#include "AudioIO.h"
#include "AudioOutput.h"

#include <sndfile.h>
#include <string.h>

class AudioOutputChannel: public AudioIOChannel {
public:
    AudioOutputChannel(char* _name, AudioEngine* _engine)
        : AudioIOChannel(_name, _engine)
    {
        AudioIOChannel::description = "AudioOutputChannel";
        engine->jack->registerOutput(name, this);
    }
};


AudioFileOutput::AudioFileOutput(AudioEngine *engine, const std::string &name) : AudioFilter(engine, name, 1, 0)
  , channel(0), nrChannels(0)
{
    m_filename = "out.wav";
    p_filename = Properties::createSaveFileProperty("File", name, &m_filename);
    engine->getProperties()->addProperty(p_filename);
}

AudioFileOutput::~AudioFileOutput()
{
    for (int i=0; i<nrChannels; i++)
        delete channel[i];
    delete [] channel;


    if (m_sf)
    {
        sf_close(m_sf);
        m_sf = nullptr;
    }
}

std::string AudioFileOutput::getDescription()
{
    return "AudioFileOutput";
}

void AudioFileOutput::activated()
{
    // connect output ports. Autoconnect with physical ports if so
    // requested. Or connect with earlier specified ports.
    const char** sysOutPorts = engine->jack->getPhysicalPlaybackPorts();
    const char** autoPort = sysOutPorts;
    for (int i=0; i<nrChannels; i++)
        channel[i]->activated(autoPort&&(*autoPort) ? *autoPort++ : 0);
    engine->jack->free(sysOutPorts);


    //File output: open stream
    memset(&sfinfo, 0, sizeof(sfinfo));
    sfinfo.channels = 1;

    sfinfo.samplerate = engine->frameRate;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    if (!sf_format_check(&sfinfo)) {
        std::cerr << "Output to file '" << m_filename << "': invalid audio format - can't log audio." << std::endl;
        return;
    }

    m_sf = sf_open(m_filename.c_str(), SFM_WRITE, &sfinfo);
    std::cout << m_sf << std::endl;
    if (!m_sf)
        std::cerr << "Output to file '" << m_filename << "': failed to open file for logging audio." << std::endl;
    else
        std::cerr << "Output to file '" << m_filename << "': logging audio to file." << std::endl;
}

void AudioFileOutput::createChannels(int _nrChannels)
{
    if (nrChannels != 0) {
        std::cerr << "AudioOutput: " << nrChannels << " channels have been allocated already." << std::endl;
        return;
    }
    nrChannels = _nrChannels;
    channel = new AudioOutputChannel* [nrChannels];
    for (int i=0; i<nrChannels; i++) channel[i] = 0;  // initialize to null
    for (int i=0; i<nrChannels; i++) {
        char name[10];
        snprintf(name, 10, "out%d", i+1);
        channel[i] = new AudioOutputChannel(name, engine);
    }
}


void AudioFileOutput::deactivated()
{
    if (m_sf)
    {
        sf_close(m_sf);
        m_sf = nullptr;
        std::cout << "Audio logging finished." << std::endl;
    }
}

void AudioFileOutput::process(int nFrames)
{
    // Audio output to channels
    for (int i=0; i<nrChannels; i++)
    {
        memcpy(channel[i]->samples, inputs[0]->samples, sizeof(float)*nFrames);
        channel[i]->process(nFrames);
    }

    //Audio output to file
    if (m_sf)
    {
        if (sf_writef_float(m_sf, inputs[0]->samples, nFrames) != nFrames)
        {
            std::cerr << "Output to file '" << m_filename << "': error logging audio." << std::endl;
            sf_close(m_sf);
            m_sf = nullptr;
        }
    }
}
