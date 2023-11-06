#ifndef ETAFILTER_H
#define ETAFILTER_H

#include "Framework/Audio.h"

class ETAFilter : public AudioFilter
{
public:
    ETAFilter(AudioEngine* engine, const std::string &name);
    ~ETAFilter() override;

    std::string getDescription() override;

    void setAlpha(float alpha);
protected:

    void activated() override;
    void process(int nFrames) override;
    FloatPropertyPtr p_alpha;
private:
    float m_lastoutput;
    float m_alpha;

};

#endif // MIXER_H
