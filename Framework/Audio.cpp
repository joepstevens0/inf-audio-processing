/* Audio.cpp : audio processing engine
 * Philippe.Bekaert@uhasselt.be - 20170802 */

#include "Audio.h"
#include "Jack.h"

#include <iostream>
#include <unistd.h>

AudioConnector::AudioConnector()
{
    samples = nullptr;
}

AudioConnector::~AudioConnector()
{
    delete[] samples;
}

void AudioConnector::updateFrames(int nFrames)
{
    delete[] samples;
    samples = new float[nFrames]();
}


//////////////////////////////////////////////////////////

AudioFilter::AudioFilter(AudioEngine *_engine, const std::string &name, int numberOfInputs, int numberOfOutputs)
    : engine(_engine)
{
    this->name = name;
    initializeConnectors(numberOfInputs, numberOfOutputs);
}

void AudioFilter::initializeConnectors(int numberOfInputs, int numberOfOutputs)
{
    inputs.resize(numberOfInputs);
    outputs.resize(numberOfOutputs);

    for (unsigned int i = 0; i < inputs.size(); ++i)
        inputs[i] = std::shared_ptr<AudioConnector>(new AudioConnector());

    for (unsigned int i = 0; i < outputs.size(); ++i)
        outputs[i] = std::shared_ptr<AudioConnector>(new AudioConnector());
}


void AudioFilter::updateConnectorFrames(int nFrames)
{
    for (unsigned int i = 0; i < inputs.size(); ++i)
        inputs[i]->updateFrames(nFrames);

    for (unsigned int i = 0; i < outputs.size(); ++i)
        outputs[i]->updateFrames(nFrames);
}


AudioFilter::~AudioFilter()
{
}


std::string AudioFilter::getDescription(void)
{
    return "STUB";
}

void AudioFilter::connectOutputPort(int outputPort, AudioFilter *nextFilter, int inputPort)
{
    if (inputPort >= nextFilter->inputs.size())
    {
        std::cerr << "Invalid input port number in connectOutputPort. Index should be between 0 and "
                  << (int)(nextFilter->inputs.size())-1 << "." << std::endl;
    }

    if (outputPort >= this->outputs.size())
    {
        std::cerr << "Invalid output port number in connectOutputPort. Index should be between 0 and "
                  << (int)(this->outputs.size())-1 << "." << std::endl;
    }


    nextFilter->inputs[inputPort] = this->outputs[outputPort];
}

//////////////////////////////////////////////////////////

AudioEngine::AudioEngine()
    : jack(0), frameRate(0), bufferSize(0)
    , frameCount(0), current_usecs(0), next_usecs(0), period_usecs(0.0)
    , nextFrameCount(-1), nrDrops(0), dropCount(0), syncCount(0)
    , maxCaptureLatency(0), maxPlaybackLatency(0)
    , first(true)
{
    jack = new JackClient(this);
    allActivated = false;

    properties = std::shared_ptr<Properties>(new Properties());
}

AudioEngine::~AudioEngine()
{
    if (isActive())
        deactivate();

    // delete filter list.
    filterlist.clear();

    jack->close();
    delete jack;
}

void AudioEngine::init(const char* engineName)
{
    jack->open(engineName);
    previousNrFrames = 0;
}

void AudioEngine::prepend(AudioFilter* f)
{
    filterlist.push_front(f);
}

void AudioEngine::append(AudioFilter *f)
{
    filterlist.push_back(f);
}

void AudioEngine::unlink(AudioFilter *f)
{
    filterlist.remove(f);
}

#define INVOKE(cb) {							\
    try { \
    for (auto f = filterlist.begin(); f != filterlist.end(); ++f) \
    (*f)->cb();								\
    } catch (...) {						\
    rc = -1;								\
    }									\
}

#define INVOKE_WITH_ARG(cb,arg) {					\
    try {									\
    for (auto f = filterlist.begin(); f != filterlist.end(); ++f) \
    (*f)->cb(arg);							\
    } catch (...) {						\
    rc = -1;								\
    }									\
    }

void AudioEngine::listFilters(void)
{
    for (auto f = filterlist.begin(); f != filterlist.end(); ++f) {
        std::cout << "AudioEngine filter: " << (*f)->getDescription() << std::endl;
    }
}

void AudioEngine::activate(void)
{
    frameCount = dropCount = nrDrops = syncCount = 0;
    nextFrameCount = -1;

    int rc = 0;
    INVOKE(activate);
    jack->activate();
    std::cout << "Audio engine activated: " << input.size()
              << " inputs, " << input.size()
              << " playbacks, " << output.size()
              << " outputs." << std::endl;
    INVOKE(activated);

    first = true;
    allActivated = true;
}

void AudioEngine::deactivate(void)
{
    allActivated = false;

    int rc = 0;
    INVOKE(deactivate);
    jack->deactivate();
    std::cout << "Audio engine deactivated." << std::endl;
    INVOKE(deactivated);
}

bool AudioEngine::isActive(void)
{
    return jack->isActive();
}

int AudioEngine::process(int nrFrames)
{
    if (allActivated)
    {
        if (first) {
            first = false;
        }

        if (nrFrames != previousNrFrames)
        {
            previousNrFrames = nrFrames;

            for (auto f = filterlist.begin(); f != filterlist.end(); ++f)
                (*f)->updateConnectorFrames(nrFrames);
        }


        int rc=0;
        INVOKE_WITH_ARG(process, nrFrames);
        return rc;
    }
    return 0;
}

int AudioEngine::bufferSizeChanged(unsigned newBufferSize)
{
    int rc=0;
    INVOKE_WITH_ARG(bufferSizeChanged, newBufferSize);
    bufferSize = newBufferSize;
    return rc;
}

int AudioEngine::frameRateChanged(unsigned newFrameRate)
{
    int rc=0;
    INVOKE_WITH_ARG(frameRateChanged, newFrameRate);
    frameRate = newFrameRate;
    return rc;
}
