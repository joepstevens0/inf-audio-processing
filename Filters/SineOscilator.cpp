#include "SineOscilator.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <cmath>

#include "Framework/Properties/Properties.h"
SineOscilator::SineOscilator(AudioEngine* _engine, const std::string &name)
  : AudioFilter(_engine, name, 0, 1) // Tell the filter that it has no input ports and 1 output port
{
    m_PI = 0;
    m_frequency = 1000.0f;
    m_phaseShift = 0.0f;

    //Create properties for setting the frequency and phaseshift
    //These properties show up in the GUI
    //values can be read using m_frequency and m_phaseShift
    //Setting the values should be done using p_frequency->setValue and p_phaseshift->setValue to update the GUI
    p_frequency = Properties::createFloatProperty("Frequency", name, &m_frequency);
    _engine->getProperties()->addProperty(p_frequency);

    p_phaseshift = Properties::createFloatProperty("Phase Shift (0-1)", name, &m_phaseShift);
    _engine->getProperties()->addProperty(p_phaseshift);
}

SineOscilator::~SineOscilator()
{
}

void SineOscilator::activated()
{
    //Initialize PI when the parent patch is started
    m_PI = std::atan(1)*4;
}

void SineOscilator::process(int nFrames)
{
    //Process samples: in this case fill the samples with a continuous sine wave
    float shift = 2.0*m_PI*m_phaseShift;

    float *out = outputs[0]->samples; //The sample buffer of the first output port

    double frameStart = engine->frameCount;
    double framerate = engine->frameRate;

    for (int i = 0; i < nFrames; ++i)
    {
        double frame = frameStart + i;
        double time = frame/framerate;

        *out++ = std::sin(2*m_PI*time*m_frequency+shift);
    }
}

std::string SineOscilator::getDescription(void)
{
    //Some Description that can be used in the GUI
    return "SineOscilator";
}

void SineOscilator::setFrequency(float frequency)
{
    //Wrapper function for setting the frequency
    p_frequency->setValue(frequency);
}

void SineOscilator::setPhaseShift(float phase)
{
    //Wrapper function for setting the phase shift
    p_phaseshift->setValue(phase);
}
