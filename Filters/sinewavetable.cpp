#include "sinewavetable.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <cmath>

#include "Framework/Properties/Properties.h"
SineWavetable::SineWavetable(AudioEngine* _engine, const std::string &name, int tablesize)
  : AudioFilter(_engine, name, 0, 1) // Tell the filter that it has no input ports and 1 output port
{
    m_PI = 0;
    m_frequency = 1000.0f;
    m_phaseShift = 0.0f;
    m_tablesize = tablesize;

    //Create properties for setting the frequency and phaseshift
    //These properties show up in the GUI
    //values can be read using m_frequency and m_phaseShift
    //Setting the values should be done using p_frequency->setValue and p_phaseshift->setValue to update the GUI
    p_frequency = Properties::createFloatProperty("Frequency", name, &m_frequency);
    _engine->getProperties()->addProperty(p_frequency);

    p_phaseshift = Properties::createFloatProperty("Phase Shift (0%-100%)", name, &m_phaseShift);
    _engine->getProperties()->addProperty(p_phaseshift);
}

SineWavetable::~SineWavetable()
{
}

void SineWavetable::activated()
{
    //Initialize PI when the parent patch is started
    m_PI = std::atan(1)*4;

    // Initialize wavetable
    m_wavetable.resize(m_tablesize);

    for (int i = 0; i < m_tablesize;++i){

        double t = 2*m_PI*((double)i/m_tablesize);
        m_wavetable[i] = std::sin(t);
    }

}

void SineWavetable::process(int nFrames)
{

    float *out = outputs[0]->samples; //The sample buffer of the first output port

    double frameStart = engine->frameCount;
    double framerate = engine->frameRate;

    for (int i = 0; i < nFrames; ++i)
    {
        double frame = frameStart + i;
        double time = frame/framerate;

        double r = time*m_frequency+m_phaseShift/100.f;
        r = m_tablesize*r;
        int lower = (int)std::floor(r);
        int upper = (int)std::ceil(r);
        double d = r-lower;
        lower= lower%m_tablesize;
        upper = upper%m_tablesize;
        *out++ = m_wavetable[lower]*(1-d) + m_wavetable[upper]*d;
    }
}

std::string SineWavetable::getDescription(void)
{
    //Some Description that can be used in the GUI
    return "SineOscilator using wavetable";
}

void SineWavetable::setFrequency(float frequency)
{
    //Wrapper function for setting the frequency
    p_frequency->setValue(frequency);
}

void SineWavetable::setPhaseShift(float phase)
{
    //Wrapper function for setting the phase shift
    p_phaseshift->setValue(phase);
}
