#ifndef SINEWAVETABLE_H
#define SINEWAVETABLE_H

#include <vector>
#include <stdio.h>

#include "Framework/Audio.h"

class SineWavetable : public AudioFilter
{
public:
    SineWavetable(AudioEngine* engine, const std::string &name, int tablesize);
    ~SineWavetable();

    std::string getDescription(void);

    //Update properties through code
    void setFrequency(float frequency);
    void setPhaseShift(float phase);

private:
    float m_frequency, m_phaseShift;
    double m_PI;
    std::vector<float> m_wavetable;
    int m_tablesize;


protected:

    // AudioFilter callbacks
    void activated();
    void process(int nFrames);

    //Properties
    FloatPropertyPtr p_frequency;
    FloatPropertyPtr p_phaseshift;
};

#endif // SINEWAVETABLE_H
