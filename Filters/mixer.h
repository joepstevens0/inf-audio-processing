#ifndef MIXER_H
#define MIXER_H

#include "Framework/Audio.h"

class Mixer : public AudioFilter
{
public:
    Mixer(AudioEngine* engine, const std::string &name);
    ~Mixer() override;

    std::string getDescription() override;

    void setAlpha1(float alpha1);
    void setAlpha2(float alpha2);
protected:

  void activated() override;
  void process(int nFrames) override;

  FloatPropertyPtr p_alpha1;
  FloatPropertyPtr p_alpha2;
private:
    float m_alpha1;
    float m_alpha2;
};

#endif // MIXER_H
