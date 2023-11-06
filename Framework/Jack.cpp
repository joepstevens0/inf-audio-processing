/* Jack.cpp : audio input and output with jack.
 * Philippe.Bekaert@uhasselt.be - 20170728 */

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <errno.h>
#include <vector>

#include "Jack.h"
#include <sstream>
#include <iostream>

const char *JackPort::getName(void)
{
  return jack_port_name(p);
}
  
bool JackPort::isInput(void)
{
  return (jack_port_flags(p) & JackPortIsInput);
}

bool JackPort::isOutput(void)
{
  return (jack_port_flags(p) & JackPortIsOutput);
}

int JackPort::nrConnections(void)
{
  return jack_port_connected(p);
}

const char** JackPort::getConnections(void)
{
  return jack_port_get_connections(p);
}

void JackPort::free(const char **portNames)
{
  jack_free(portNames);
}

void JackPort::connect(const char* otherport)
{
  const char *thisport = getName();
  const char *outport, *inport;
  const char *kind;
  if (isOutput()) {
    outport = thisport; inport = otherport; kind = "output";
  } else {
    outport = otherport; inport = thisport; kind = "input";
  }
  
  int err = 0;
  switch (err = jack_connect(client, outport, inport)) {
      case 0:
        std::cout << "Jack " << kind << " port " << thisport << " connected to " << otherport << std::endl;
        break;
      case EEXIST:
        std::cout << "Jack " << kind << " port " << thisport << "  was already connected to " << otherport << std::endl;
        break;
      default:
      {
      std::ostringstream oss("");
      oss << "Jack "
          << kind
          << " port "
          << thisport
          << " failed to connect with "
          << otherport
          << "." << std::endl;
      throw std::runtime_error(oss.str());
      }
  }
  
  aport->connected = nrConnections();
}

void JackPort::disconnect(const char* otherport)
{
  checkerr(jack_disconnect(client, getName(), otherport), "disconnect");
  aport->connected = nrConnections();
}

void JackPort::disconnect(void)
{
  checkerr(jack_port_disconnect(client, p), "port_disconnect");
  aport->connected = nrConnections();
}

void JackPort::getBuffer(jack_nframes_t nframes)
{
  aport->samples = (float*)jack_port_get_buffer(p, nframes);
}

bool JackPort::latencyChangedCB(jack_latency_callback_mode_t mode)
{
  // For input  ports, only the capture  latency is relevant.
  // For output ports, only the playback latency is relevant.
  switch (mode) {
  case JackCaptureLatency : if (!isInput() ) return false; break;
  case JackPlaybackLatency: if (!isOutput()) return false; break;
  default:
    std::cerr << "Jack port " << getName() << " latencyChangedCB(): invalid mode " << (int)mode << std::endl;
    return false;
  }
  
  jack_latency_range_t range;
  jack_port_get_latency_range(p, mode, &range);
  aport->latency.min = range.min;
  aport->latency.max = range.max;

  /*
  Debug("Jack %s port %s %s latency changing to min %d max %d\n",
	isInput() ? "input" : "output",
	getName(),
	mode==JackCaptureLatency ? "capture" : mode==JackPlaybackLatency ? "playback" : "unknown",
	aport->latency.min, aport->latency.max);
*/

  if (aport->latencyChanged()) {
    range.min = aport->latency.min;
    range.max = aport->latency.max;
    jack_port_set_latency_range(p, mode, &range);
    return true;
  }
  return false;
}

void JackPort::checkerr(int rc, const char* routine)
{
  if (rc!=0)
  {
      std::ostringstream oss("");
      oss << "Jack port "
          << getName() << "jack_"
          << routine << "() failed with error code " << rc << "." << std::endl;
      throw std::runtime_error(oss.str());
  }
}

//////////////////////////////////////////////////////////////////

void JackClient::close(void)
{
  if (client)
    jack_client_close(client);
  client = 0;
}

char* JackClient::getName(void)
{
  return name;
}

bool JackClient::isOpen(void)
{
  return client!=0;
}

bool JackClient::isActive(void)
{
  return active;
}

// C-style callbacks - called by jack server.
static int _process(jack_nframes_t nframes, void* arg)
{
  JackClient* c = (JackClient*)arg;
  return c->processCB(nframes);
}

static void _shutdown(void* arg)
{
  JackClient* c = (JackClient*)arg;
  c->shutdownCB();
}

static void _error(const char* msg)
{
  std::cerr << "Jack error: " << msg << std::endl;
}

static void _info(const char* msg)
{
  std::cout << "Jack info: " << msg << std::endl;
}

static int _xrun(void *arg)
{
  JackClient* c = (JackClient*)arg;
  return c->xrunCB();
}

static int _samplerate(jack_nframes_t nframes, void* arg)
{
  JackClient* c = (JackClient*)arg;
  return c->frameRateChangedCB(nframes);
}  

static int _bufsz(jack_nframes_t nframes, void* arg)
{
  JackClient* c = (JackClient*)arg;
  return c->bufferSizeChangedCB(nframes);
}

static void _latency(jack_latency_callback_mode_t mode, void* arg)
{
  JackClient* c = (JackClient*)arg;
  c->latencyChangedCB(mode);
}

void JackClient::open(const char* jackClientName)
{
  if (isOpen()) {
    std::cout << "Jack client " << name << " open(): already open." << std::endl;
    return;
  }
  
  int maxnsz = jack_client_name_size();
  name = new char [maxnsz];
  if (snprintf(name, maxnsz-1, "%s", jackClientName) >= maxnsz)
    std::cout << "JackClient::open(): client name "
              << jackClientName
              << " too long - truncating to "
              << maxnsz
              << " characters."
              << std::endl;
  
  // create client
  jack_options_t opt = JackNullOption;
  jack_status_t status;
  client = jack_client_open(name, opt, &status);
  if (!client) {
    print_status(status);
    throw std::runtime_error("Failed to open jack client.\n");
  }
  
  // set callbacks
  jack_set_error_function(_error);
  jack_set_info_function(_info);
  jack_on_shutdown (client, _shutdown, this);
  checkerr(jack_set_process_callback (client, _process, this), "set_process_callback");
  checkerr(jack_set_buffer_size_callback(client, _bufsz, this), "set_buffer_size_callback");
  checkerr(jack_set_latency_callback(client, _latency, this), "set_latency_callback");
  checkerr(jack_set_sample_rate_callback(client, _samplerate, this), "set_sample_rate_callback");
  checkerr(jack_set_xrun_callback(client, _xrun, this), "set_xrun_callback");
  
  engine->frameRate = jack_get_sample_rate(client);
  engine->bufferSize = jack_get_buffer_size(client);
  std::cout << "Jack client " << name << " opened. Engine sample rate "
            << engine->frameRate << ". Buffer size "
            << engine->bufferSize << std::endl;
}

int JackClient::registerPort(const char *portname, class AudioPort *aport, unsigned portflags)
{
  if (isActive())
    throw std::runtime_error("Jack client: can't register ports while active.\n");
  
  jack_port_t *p = jack_port_register(client, portname, JACK_DEFAULT_AUDIO_TYPE, portflags, 0);
  if (!p)
    throw std::runtime_error("Jack client failed to create port.\n");
  port.push_back(JackPort(client, p, aport));
  return aport->portId = port.size()-1;
}

int JackClient::registerInput(const char *portname, class AudioPort *aport)
{
  registerPort(portname, aport, JackPortIsInput|JackPortIsTerminal);
  engine->input.push_back(aport);
  return aport->portId;
}

int JackClient::registerOutput(const char *portname, class AudioPort *aport)
{
  registerPort(portname, aport, JackPortIsOutput|JackPortIsTerminal);
  engine->output.push_back(aport);
  return aport->portId;
}

const char **JackClient::getPorts(const char* portNamePattern, const char *typeNamePattern, unsigned long flags)
{
  return jack_get_ports(client, portNamePattern, typeNamePattern, flags);
}

const char **JackClient::getPhysicalCapturePorts(void)
{
  return getPorts(NULL, NULL, JackPortIsPhysical|JackPortIsOutput);
}

const char **JackClient::getPhysicalPlaybackPorts(void)
{
  return getPorts(NULL, NULL, JackPortIsPhysical|JackPortIsInput);
}

void JackClient::free(const char **portNames)
{
  jack_free(portNames);
}

void JackClient::portIndexRangeCheck(int i)
{
  if (i<0 || i>= (int)port.size())
      throw std::runtime_error("Jack client: invalid port index.\n");
}

void JackClient::connect(int i, const char* otherPortName)
{
  portIndexRangeCheck(i);
  port[i].connect(otherPortName);
}

void JackClient::disconnect(int i, const char* otherPortName)
{
  portIndexRangeCheck(i);
  port[i].disconnect(otherPortName);
}

void JackClient::disconnect(int i)
{
  portIndexRangeCheck(i);
  port[i].disconnect();
}

void JackClient::activate(void)
{
  if (active) {
    std::cout << "Jack client " << name << " activate(): already active." << std::endl;
    return;
  }

  frameCountCycle = frameCount = 0;
  checkerr(jack_activate(client), "activate");
  active = true;
}

void JackClient::deactivate(void)
{
  if (!active) {
    std::cout << "Jack client " << name << " deactivate(): not active." << std::endl;
    return;
  }
  
  checkerr(jack_deactivate(client), "deactivate");
  active = false;
}

void JackClient::checkerr(int rc, const char* routine)
{
  if (rc!=0)
  {
      std::ostringstream oss("");
      oss << "Jack client "
          << name
          << " jack_" << routine
          << " failed with error code "
          << rc << std::endl;
      throw std::runtime_error(oss.str());

  }
}

int JackClient::processCB(jack_nframes_t nframes)
{
  // get frame count and time for period from the jack engine.
  uint32_t newFrameCount;
  checkerr(jack_get_cycle_times(client,
				&newFrameCount,
				&engine->current_usecs, &engine->next_usecs,
				&engine->period_usecs), "get_cycle_times");

  if (newFrameCount<frameCount) {    // overflow (once about a day at 48KHz)
    frameCountCycle++;
    std::cout << "JackClient::processCB(): new frame count cycle: old frame count " << frameCount << ", new frame count " << newFrameCount << ", cycle " << frameCountCycle << std::endl;
  }
  frameCount = newFrameCount;

  // engine frame count - no overflows in 64 bit counter.
  engine->frameCount = (((int64_t)frameCountCycle)<<32) + (int64_t)frameCount;

  // we assume no frame drops at first processCB invocation.
  if (engine->nextFrameCount<0)
    engine->nextFrameCount = engine->frameCount;

  // new frame count minus (old frame count + old nr frames) shall be
  // zero. If positive, there are new dropped frames. The difference
  // can also be negative: if N*bufferSize frames have been dropped, the
  // frame count is not increased for the next N process callback invocations,
  // causing a negative difference. The accumulated difference tells us
  // how much late we are with our processing.
  int64_t nrDrops = engine->frameCount - engine->nextFrameCount;
  engine->syncCount += nrDrops;
  engine->nrDrops = 0;
  if (nrDrops>0) {
    engine->nrDrops = nrDrops;
    engine->dropCount += nrDrops;
  }

  // expected frame counter for the next process callback invocation
  engine->nextFrameCount = engine->frameCount + nframes;
  
  // get pointers to the sample buffers for all ports
  for (unsigned int i=0; i<port.size(); i++)
    port[i].getBuffer(nframes);

  // and finally do the real work
  int rc = engine->process(nframes);

  return rc;
}

int JackClient::xrunCB(void)
{
  //  float delay = jack_get_max_delayed_usecs(client);
  //  Warning("Jack client %s xrun - %g usecs delay since startup or reset.\n", name, delay);
  return 0;  // don't give up
}

int JackClient::bufferSizeChangedCB(jack_nframes_t nframes)
{
  return engine->bufferSizeChanged(nframes);
}

int JackClient::frameRateChangedCB(jack_nframes_t nframes)
{
  return engine->frameRateChanged(nframes);
}

int JackClient::latencyChangedCB(jack_latency_callback_mode_t mode)
{
  bool latencyNeedsRecomputing =false;
  for (unsigned int i=0; i<port.size(); i++) {
    latencyNeedsRecomputing = latencyNeedsRecomputing ||
      port[i].latencyChangedCB(mode);
  }
  /*Debug("Jack client %s %s latency changed. %s\n", name,
       mode==JackCaptureLatency ? "capture" : mode==JackPlaybackLatency ? "playback" : "unknown",
       latencyNeedsRecomputing ? "Need recomputing." : "No need to recompute.");*/
  if (latencyNeedsRecomputing)
    checkerr(jack_recompute_total_latencies(client), "recompute_total_latencies");
 // tstats.tick(__LINE__, "JackClient() leaving latencyChangedCB.");
  return 0;
}

void JackClient::shutdownCB(void)
{
  std::cout << "Jack client " << name << " shut down." << std::endl;
}

void JackClient::print_status(jack_status_t status)
{
  struct { JackStatus bit; const char *msg; } statusmsgs[] = {
    JackFailure, 
    "Overall operation failed.",
    
    JackInvalidOption, 
    "The operation contained an invalid or unsupported option.",
    
    JackNameNotUnique,
    "The desired client name was not unique. With the JackUseExactName option this situation is fatal. Otherwise, the name was modified by appending a dash and a two-digit number in the range '-01' to '-99'. The jack_get_client_name() function will return the exact string that was used. If the specified client_name plus these extra characters would be too long, the open fails instead.",
    
    JackServerStarted,
    "The JACK server was started as a result of this operation. Otherwise, it was running already. In either case the caller is now connected to jackd, so there is no race condition. When the server shuts down, the client will find out.",
    
    JackServerFailed, 	
    "Unable to connect to the JACK server.",
    
    JackServerError, 	
    "Communication error with the JACK server.",
    
    JackNoSuchClient, 	
    "Requested client does not exist.",
    
    JackLoadFailure, 	
    "Unable to load internal client",
    
    JackInitFailure, 	
    "Unable to initialize client.",
    
    JackShmFailure, 	
    "Unable to access shared memory.",
    
    JackVersionError,
    "Client's protocol version does not match.",
    
    JackBackendError,
    "Backend error.",
    
    JackClientZombie,
    "Client zombie error.",
    
    // sentinel: bit doesn't matter - message is a zero pointer.
    JackClientZombie,
    0
  };
  
  int n=0;
  for (int i=0; statusmsgs[i].msg!=0; i++) {
    if (status & statusmsgs[i].bit) {
      std::cerr << "Jack error: " << statusmsgs[i].msg << std::endl;
      n++;
    }
  }
  
  if (n==0)
    std::cerr << "Unknown jack error." << std::endl;
}

