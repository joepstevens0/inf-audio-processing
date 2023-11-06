/* AudioInput.h: audio capture. Essentially creates jack input ports and
 * manages connections of system audio interface capture ports.
 * Philippe.Bekaert@uhasselt.be - 20170731,0801,828 */

#pragma once


#include <stdio.h>
#include "Audio.h"

class AudioInput: public AudioFilter {
 protected:
  class AudioInputChannel** channel;
  int nrChannels;
  
 public:
  AudioInput(AudioEngine* engine);
  ~AudioInput();

  // initialize manually
  void createChannels(int nrChannels);

  // access capture ports in AudioEngine object input member variable.  

  std::string getDescription(void);
  
 protected:
  // AudioFilter callbacks
  void activated(void);  
  void process(int nFrames);
};


    
