#include "etafilter.h"

ETAFilter::ETAFilter(AudioEngine* _engine, const std::string &name)
: AudioFilter(_engine, name, 1, 1){
    m_lastoutput = 0;
    setAlpha(50);
    p_alpha = Properties::createFloatProperty("Alpha(0%-100% of new)", name, &m_alpha);
    _engine->getProperties()->addProperty(p_alpha);
}
ETAFilter::~ETAFilter(){

}

std::string ETAFilter::getDescription(){
    return "Exponential time averaging filter";
}


void ETAFilter::activated() {
}

void ETAFilter::process(int nFrames) {
    float* out = outputs[0]->samples;
    float* in = inputs[0]->samples;

    for (int i = 0; i < nFrames; ++i){
        *out = (m_alpha/100.f)*(*in) + (1-(m_alpha/100.f))*m_lastoutput;
        m_lastoutput = *out;
        ++out;
        ++in;
    }
}

void ETAFilter::setAlpha(float alpha){
    m_alpha = alpha;
}
