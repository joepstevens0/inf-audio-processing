#include "mixer.h"

Mixer::Mixer(AudioEngine* _engine, const std::string &name)
    : AudioFilter(_engine, name, 2, 1)
{
    m_alpha1 = 50;
    m_alpha2 = 50;
    p_alpha1 = Properties::createFloatProperty("Alpha1(%)", name, &m_alpha1);
    _engine->getProperties()->addProperty(p_alpha1);
    p_alpha2 = Properties::createFloatProperty("Alpha2(%)", name, &m_alpha2);
    _engine->getProperties()->addProperty(p_alpha2);
}

Mixer::~Mixer(){

}
void Mixer::activated(){

}

void Mixer::process(int nFrames){
    float *out = outputs[0]->samples;
    float* in0 = inputs[0]->samples;
    float* in1 = inputs[1]->samples;

    for (int i = 0; i < nFrames; ++i)
    {
        *out = (*in0++ * (m_alpha1/100.f)) + (*in1++ * (m_alpha2/100.f));
        *out = std::clamp(*out, -1.f,1.f);
        ++out;
    }
}

std::string Mixer::getDescription()
{
    return "Mixer";
}

void Mixer::setAlpha1(float alpha1){
    p_alpha1->setValue(alpha1);
}
void Mixer::setAlpha2(float alpha2){
    p_alpha2->setValue(alpha2);
}
