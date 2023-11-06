#ifndef ALLPASS_H
#define ALLPASS_H

#include "Framework/Audio.h"
#include "Framework/Properties/PropertyObserver.h"

class AllPass : public AudioFilter, public PropertyObserver
{
public:
    AllPass(AudioEngine* engine, const std::string &name);
    ~AllPass();

    std::string getDescription() override;

    void setDelay(int ms);
    void setGValue(float g);
protected:
    void activated() override;
    void deactivated() override;
    void process(int nFrames) override;

    IntegerPropertyPtr p_delay;
    FloatPropertyPtr p_g;
private:
    void update() override;

    int m_delay;
    float m_g;
    int m_bufferP;
    std::vector<float> m_buffer;
};

#endif // ALLPASS_H
