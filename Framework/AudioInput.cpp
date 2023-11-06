/* AudioInput.cpp: live audio capture, using jack.
 * Philippe.Bekaert@uhasselt.be - 20170731,0801,0828 */

#include "AudioIO.h"
#include "AudioInput.h"

class AudioInputChannel: public AudioIOChannel {
public:
  AudioInputChannel(char* _name, AudioEngine* _engine)
    : AudioIOChannel(_name, _engine)
  {
    AudioIOChannel::description = "AudioInputChannel";
    engine->jack->registerInput(name, this);
  }
};

AudioInput::AudioInput(AudioEngine* _engine)
  : AudioFilter(_engine, "AudioInput", 0, 1)
  , channel(0), nrChannels(0)
{
}

AudioInput::~AudioInput()
{
  for (int i=0; i<nrChannels; i++)
    delete channel[i];
  delete [] channel;
}

void AudioInput::createChannels(int _nrChannels)
{
  if (nrChannels != 0) {
    std::cerr << "AudioInput: already have " << nrChannels <<  " channels." << std::endl;
    return;
  }
  nrChannels = _nrChannels;
  channel = new AudioInputChannel* [nrChannels];
  for (int i=0; i<nrChannels; i++) channel[i] = 0;  // initialize to null
  for (int i=0; i<nrChannels; i++) {
    char name[10];
    snprintf(name, 10, "in%d", i+1);
    channel[i] = new AudioInputChannel(name, engine);
  }
}

void AudioInput::activated(void)
{
  // connect input ports. Autoconnect with physical ports if so
  // requested. Or connect with earlier specified ports.
  const char** sysInPorts = engine->jack->getPhysicalCapturePorts();
  const char** autoPort = sysInPorts;
  for (int i=0; i<nrChannels; i++)
    channel[i]->activated(autoPort&&(*autoPort) ? *autoPort++ : 0);
  engine->jack->free(sysInPorts);
}

void AudioInput::process(int nFrames)
{
  for (int i=0; i<nrChannels; i++)
    channel[i]->process(nFrames);
  //At the moment we only use a single channel
  memcpy(outputs[0]->samples, channel[0]->samples, sizeof(float)*nFrames);
}

std::string AudioInput::getDescription(void)
{
  return "AudioInput";
}
