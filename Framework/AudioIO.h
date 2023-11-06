/* AudioIO.h : audio input/output.
 * Philippe.Bekaert@uhasselt.be - 20170829 */

#pragma once

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "Audio.h"
#include "Jack.h"

#include <iostream>

//////////////////////////////////////////////////////////////////

// base class for AudioInputChannel and AudioOutputChannel - these are almost
// entirely the same.
class AudioIOChannel: public AudioPort {
protected:
  const char* description;
  
public:
  AudioEngine* engine;
  char* name;
  char *connectedPortName;
  float gain;
  
  AudioIOChannel(const char* _name, AudioEngine* _engine)
    : engine(_engine), name(0), connectedPortName(0), gain(1.0)
  {
    name = strdup(_name);
    connectedPortName = strdup("AUTO");
  }

  ~AudioIOChannel()
  {
    free(name);
    free(connectedPortName);
  }

  const char* getName(void)
  {
    return name;
  }

  void connect(const char* _portname =0)
  {
    if (_portname) {
      free(connectedPortName);
      connectedPortName = strdup(_portname);
    }
    try {
      if (engine->jack->isActive()) {
	// disconnect from current connected ports and connect to the new one
	  if (connected)
	    engine->jack->disconnect(portId);
	  if (connectedPortName[0])
	    engine->jack->connect(portId, connectedPortName);
      }    
    } catch (...) {
      std::cerr << description << " " << name << "connect to '" << connectedPortName << "' failed." << std::endl;
      free(connectedPortName);
      connectedPortName = strdup("");
    }
  }

  // handles automatic capture port connect : autoPort is the name
  // of a physical capture port to connect to is the connected port
  // name of this AudioIOChannel equals "AUTO".
  void activated(const char* autoPort)
  {
    if (strcmp(connectedPortName, "AUTO") == 0)
      connectedPortName = autoPort ? strdup(autoPort) : strdup("");
    connect();
  }

  // called by real-time audio processing thread.
  // applies gain.
  void process(int nrSamples)
  {
    float *s = samples;
    for (int i=0; i<nrSamples; i++)
      *s++ *= gain;
  }
};

