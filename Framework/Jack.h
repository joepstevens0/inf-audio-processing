/* Jack.h: audio input and output with jack
 * Philippe.Bekaert@uhasselt.be - 20170728 */

#pragma once

#include <vector>
#include "Audio.h"

#include <jack/jack.h>

// Associates generic AudioPort with jack input or output port.
class JackPort {
 public:
  jack_client_t *client;
  jack_port_t *p;
  class AudioPort *aport;
  
  JackPort(jack_client_t *_c =0, jack_port_t *_p =0, AudioPort *_aport =0)
    : client(_c), p(_p), aport(_aport)
  {}
  
  ~JackPort()
  {}
  
  const char *getName(void);  
  bool isInput(void);
  bool isOutput(void);
  int nrConnections(void);
  const char** getConnections(void);
  static void free(const char **portNames);
  void connect(const char* otherport);
  void disconnect(const char* otherport);
  void disconnect(void);

 protected:
  friend class JackClient;
  void checkerr(int rc, const char* routine);
  void getBuffer(jack_nframes_t nframes);
  bool latencyChangedCB(jack_latency_callback_mode_t mode);
};

class JackClient {
 public:
  class AudioEngine *engine;   // associated generic audio engine object
  jack_client_t *client;
  std::vector<JackPort> port;

  JackClient(AudioEngine* _engine, const char* jackClientName =0)
    : engine(_engine), client(0), name(0), active(false)
    , frameCount(0), frameCountCycle(0)
  {
    if (jackClientName)
      open(jackClientName);
  }

  ~JackClient()
  {
    close();
    delete [] name;
  }

  // connect to system-wide jack audio server
  void open(const char* jackClientName ="Studio.One");
  void close(void);
  bool isOpen(void);
  char* getName(void);   // returns client name (copy of argument to open())

  // Register input/out port with given short name. Returns port index.
  // Port index is also filled in AudioPort passed as an argument.
  int registerInput (const char *portname, class AudioPort *aport);
  int registerOutput(const char *portname, class AudioPort *aport);

  // list physical capture and playback ports - use free() below to
  // delete the returned data structure.
  const char **getPhysicalCapturePorts(void);
  const char **getPhysicalPlaybackPorts(void);
  const char **getPorts(const char* portNamePattern, const char *typeNamePattern, unsigned long flags);
  static void free(const char **portNames);

  // connect and disconnect ports
  void connect   (int portId, const char* otherPortName);
  void disconnect(int portId, const char* otherPortName);
  void disconnect(int portId);

  // start running the client - no port registrations allowed when active
  void activate(void);
  void deactivate(void);
  bool isActive(void);
  
 protected:
  friend class JackPort;
  char *name;
  bool active;
  unsigned frameCount, frameCountCycle;
  
  void portIndexRangeCheck(int i);  
  int registerPort(const char *portname, class AudioPort *aport, unsigned portflags);

  static void print_status(jack_status_t status);
  void checkerr(int rc, const char* routine);

 public:
  // jack callbacks - called by jack client thread, and in turn calling
  // the connected abstract AudioEngine and/or taking other appropriate
  // actions. Do not call yourself.
  int processCB(jack_nframes_t nframes);
  int xrunCB(void);
  int bufferSizeChangedCB(jack_nframes_t nframes);
  int frameRateChangedCB(jack_nframes_t nframes);
  int latencyChangedCB(jack_latency_callback_mode_t mode);
  void shutdownCB(void);
}; // class JackClient

