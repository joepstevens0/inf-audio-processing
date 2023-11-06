/* AudioOutput.cpp: audio output. Essentially creates jack output ports
 * and manages connections with the system audio interface playback ports.
 * Philippe.Bekaert@uhasselt.be - 20170801,0829 */

#include "AudioIO.h"
#include "AudioOutput.h"

class AudioOutputChannel: public AudioIOChannel {
public:
  AudioOutputChannel(char* _name, AudioEngine* _engine)
    : AudioIOChannel(_name, _engine)
  {
    AudioIOChannel::description = "AudioOutputChannel";
    engine->jack->registerOutput(name, this);
  }
};

AudioOutput::AudioOutput(AudioEngine* _engine, const std::string& name)
  : AudioFilter(_engine, name, 1, 0)
  , channel(0), nrChannels(0)
{
}

AudioOutput::~AudioOutput()
{
  for (int i=0; i<nrChannels; i++)
    delete channel[i];
  delete [] channel;
}

void AudioOutput::createChannels(int _nrChannels)
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

void AudioOutput::activated(void)
{
  // connect output ports. Autoconnect with physical ports if so
  // requested. Or connect with earlier specified ports.
  const char** sysOutPorts = engine->jack->getPhysicalPlaybackPorts();
  const char** autoPort = sysOutPorts;
  for (int i=0; i<nrChannels; i++)
    channel[i]->activated(autoPort&&(*autoPort) ? *autoPort++ : 0);
  engine->jack->free(sysOutPorts);
}

void AudioOutput::process(int nFrames)
{
  for (int i=0; i<nrChannels; i++)
  {
    memcpy(channel[i]->samples, inputs[0]->samples, sizeof(float)*nFrames);
    channel[i]->process(nFrames);
  }
}

std::string AudioOutput::getDescription(void)
{
  return "AudioOutput";
}
