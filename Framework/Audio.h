/* Audio.h: abstract audio input and output.
 * Philippe.Bekaert@uhasselt.be - 20170728,31,0801,0802 */

#pragma once

#include <vector>
#include <list>

#include <stdint.h>
#include <atomic>

#include <memory>
#include "Framework/Properties/Properties.h"

#include <jack/jack.h>
class AudioConnector {

public:
    AudioConnector();
    ~AudioConnector();
    void updateFrames(int nFrames);

    float* samples;
};

// Terminology:
// - Physical capture resp. playback port (e.g. stereo left, ADAT 1, AES L, ...)
// correspond one-to-one with audio input/output connections on the audio
// interface.
// - Software ports are endpoints for communication of audio
// samples between applications. Studio.One e.g. creates a set of input
// and output ports connected with resp. capture and playback physical ports.
// - The set of audio samples from all ports at a given time instance is
// called an audio frame.
// - The number of frames per second is called the frame rate, typically 48KHz.
// Each ports sample rate is equal to the frame rate (each port provides/
// accepts one sample at any given time instance).
// - There is a one-to-one correspondence between ports and audio channels.
// Channels are per definition mono.
// - Channels are organised in tracks. A track can be mono (single
// channel), stereo (two channels with meaning left and right), a.s.o.
// - Audio is processed in chunks called periods. The number of frames
// per period (or samples on each channel per period) is a parameter of
// the audio hardware/processing engine, and typically 128 ... 1024.
// The period duration in seconds is the period size divided by the frame
// rate, e.g. 1024/48000.
// - The necessity to process audio samples in such chunks is a major saource
// of latency in digital audio processors.
// The latency of an input port is the time between arrival
// of an audio signal on some physical capture port and the presentation of
// its samples in the software. The latency of an output port is the time
// between submission of samples in the software and the appearance of the
// audio signal on some physical playback port. The latency of each port is
// available as a min-max range, expressed in number of samples. It is a
// range because there may be multiple paths to physical ports.

// Abstract audio port: essentially a container of a period of audio samples
// and a callback function 'process' to process it (send or receive to/from
// audio port).
class AudioPort {
public:
  AudioPort()
    :  latency(0,0), samples(0), portId(-1), connected(0)
  {}
  
  virtual ~AudioPort() {}

  // returns a name for the audio port. For GUI, recorder file names, etc...
  virtual const char* getName(void) =0;

  // Called when a period of samples is available on the port.
  // The samples themselves are pointed to by the 'samples' member
  // variable, set right before calling this callback.
  // 'nrSamples' is the number of samples to process on this port.
  // It is the same for all audio ports, and corresponds wit the engines
  // period.
  // Note: nrSamples is always equal to the engines bufferSize.
  virtual void process(int nrSamples) {}

  // called when the ports latency range has changed. The range itself is
  // available in the 'latency' member variable, set right before calling
  // the callback. This callback can add some number of frames to the
  // min-max range. After returning, the new range is set and propagated
  // in the audio processing graph. The callback shall return true if
  // latency is added, and false if it does not change the latency range.
  virtual bool latencyChanged(void) { return false; }
  struct LATENCY_RANGE {
    unsigned min, max;       // latency range expressed in nr of samples.
    LATENCY_RANGE(unsigned m =0, unsigned M =0) : min(m), max(M) {}
  } latency;

  // called when the buffer size changes
  virtual void bufferSizeChanged(int newBufferSize) {}

  float *samples;           // buffer containing current audio period samples

 protected:
  friend class JackPort;
  friend class JackClient;
  
  int portId;                // device port index number, set when registering
  int connected;             // number of connections to this port
};

// Abstract audio filter: callbacks are invoked in order by which the
// filters are registered to the AudioEngine.
// AudioFilters have access to the samples in the AudioPorts through the
// AudioEngine they are connected to. They operate in-place, or can change
// the samples pointer for input or playback AudioPorts. Not so for output
// AudioPorts however: the audio driver expects to see the samples to be
// output in the memory area pointed to.
class AudioFilter {
 public:
  class AudioEngine *engine;   // AudioEngine managing the filter

  AudioFilter(AudioEngine *_engine, const std::string &name, int numberOfInputs = 0, int numberOfOutputs = 0);
  virtual ~AudioFilter();

  virtual std::string getDescription(void);

  //Update nr of connector frames
  void updateConnectorFrames(int nFrames);

  //Connects the specified output port of this filter to an input port of the next filter
  void connectOutputPort(int outputPort, AudioFilter* nextFilter, int inputPort);

protected:
  // audio engine callbacks
  friend class AudioEngine;

  // called right before driver activation
  // construct list of engine inputs, outputs and playbacks. These lists
  // are not yet available.
  virtual void activate(void)  {}
  // called right after driver activation
  // list of engine inputs, outputs and playbacks is available and the
  // process cycle is active.
  virtual void activated(void) {}

  // called right before driver deactivation - undoes activated().
  // list of engine inputs, outputs and playbacks is available. The
  // process cycle is still active.
  virtual void deactivate(void) {}
  // called right after driver deactivation - undoes activate().
  // list of engine inputs, outputs and playbacks are no longer all available
  // The process cycle has been suspended. 
  virtual void deactivated(void) {}

  // the work horse - throws Exception in case of serious error.
  // Note: this callback is run in a real-time thread.
  // Called by engine to process a period of audio frames. The audio
  // frames are available in 'samples' member of the AudioPorts of the engine
  // at the time process() gets called. Right before process()
  // gets called, also the frame count and microsecond period start time
  // since the start of the process cycle are set, as well
  // as estimates for the start of the next period.
  virtual void process(int nrFrames) {}

  // called when number of samples per period changes
  virtual void bufferSizeChanged(unsigned int newBufferSize) {}

  // called when audio frame rate changes
  virtual void frameRateChanged(unsigned int newFrameRate) {}

  // called at each engine time code update.
  // Updated time code and corresponding frame count (in the period to
  // be processed next) are available in engine member variables tc and
  // tcFrameCount.
  virtual void timeCodeChanged(void) {}

  //Initializes the connector buffers
  void initializeConnectors(int numberOfInputs, int numberOfOutputs);

  std::vector< std::shared_ptr<AudioConnector> > inputs, outputs;
  std::string name;
};

// Abstract audio engine, maintains driver, list of input/playback/output
// AudioPorts, and a doubly linked list of AudioFilters, invoked in the
// order they are added to the engine.
class AudioEngine {
 protected:
  // list of audioFilters.
  std::list<AudioFilter*> filterlist;

 public:
  // audio driver handle - created and destroyed in the constructor/destructor
  friend class JackClient;   // see Jack.h
  class JackClient *jack;

  unsigned frameRate;       // audio engine frame rate
  unsigned bufferSize;      // buffer size in frames, for process() - usually
                            // the nr of frames per period.

  // input and output ports - available when the engine is active
  // (see activated() and deactivated() callbacks below).
  std::vector<AudioPort*> input, playback, output;

  // Current period time information, set right before the engine calls the
  // process() callback.
  // Period start as nr of frames since start of process cycle. Unless
  // xruns have occurred, this is the previous periods frameCount plus
  // the number of frames per period (argument to process()).
  int64_t frameCount;
  jack_time_t current_usecs, next_usecs;
  // Current period time best estimate in microseconds 
  float period_usecs;
  
  // nextFrameCount: frameCount + nFrames during process.
  // If at the start of process(), nrDrops=frameCount-nextFrameCounts is larger
  // than zero, nrDrops frames have been dropped.
  // dropCount is the total number of dropped frames since the start
  // of the process cycle. syncCount is usually zero, meaning all is
  // in sync, but will deviate near frame drops (first be positive, then
  // level out to zero again).
  int64_t  nextFrameCount, nrDrops, dropCount, syncCount;

  int previousNrFrames;

  // Current maximum capture and playback port latency, in number of
  // frames. Derived from list of input and output ports before processing
  // each period.
  unsigned maxCaptureLatency, maxPlaybackLatency;

  AudioEngine();
  ~AudioEngine();

  // initialize the driver - it is closed by the destructor.
  void init(const char* engineName);

  // Register/Unregister AudioFilters. Merely appends of prepends to
  // filterlist, resp. unlinks.
  // You can also directly prepend or append new filters to any other
  // filter known to be in the list, or unlink direcly.
  void prepend(AudioFilter *f);
  void append(AudioFilter *f);
  void unlink(AudioFilter *f);

  // no filter or port registration of unregistration once activated!
  void activate(void);
  void deactivate(void);
  bool isActive(void);

  std::shared_ptr<Properties> getProperties() const { return properties; }

 protected:
  // These callback return 0 for succes, or nonzero in case of failure.

  // The return nonzero in case on of the invoked AudioFilter callbacks
  // throws an Exception.
  int process(int nrFrames);

  // Called by the driver when the buffer size changes. Invokes AudioFilter
  // callbacks and sets our own bufferSize if none throws an Exception.
  int bufferSizeChanged(unsigned newBufferSize);

  // Called by the driver when the frame rate changes.
  int frameRateChanged(unsigned newFrameRate);

  // for debugging
  void listFilters(void);  

  // flags indicating first period is to be processed.
  bool first;

  // capture/playback latency monitoring
  void getLatencies(void);

  //Is each linked filter activated?
  std::atomic_bool allActivated;

  //A list of properties for the engine and filters
  std::shared_ptr<Properties> properties;
};



