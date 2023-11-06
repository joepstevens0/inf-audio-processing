/* AudioOutput.h: audio output. Essentially creates jack output ports
 * and manages connections with the system audio interface playback ports.
 * Philippe.Bekaert@uhasselt.be - 20170801,0829 */

#pragma once


#include <stdio.h>

#include "Audio.h"

class AudioOutput: public AudioFilter {
 protected:
  class AudioOutputChannel **channel;
  int nrChannels;

 public:
  AudioOutput(AudioEngine* engine, const std::string &name);
  ~AudioOutput();

  // initialize manually
  void createChannels(int _nrChannels);

  // access output ports in AudioEngine object, output member variable.
  std::string getDescription(void);

 protected:
  // AudioFilter callbacks
  void activated(void);
  void process(int nFrames);
};
