#ifndef DELAY_H
#define DELAY_H

#include "Framework/Audio.h"
#include "Framework/Properties/PropertyObserver.h"

class Delay : public AudioFilter, public PropertyObserver
{
public:
    Delay(AudioEngine* engine, const std::string &name);
    ~Delay();

    std::string getDescription() override;

    void setDelay(int ms);
protected:
    void activated() override;
    void deactivated() override;
    void process(int nFrames) override;

    IntegerPropertyPtr p_delay;
private:
    void update() override;

    int m_delay;
    int m_bufferP;
    std::vector<float> m_buffer;
};

#endif // DELAY_H
