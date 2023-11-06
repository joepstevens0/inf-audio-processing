#include "allpass.h"
#include <cstring>
#include <cmath>

AllPass::AllPass(AudioEngine* _engine, const std::string &name)
: AudioFilter(_engine, name, 1, 1){
    m_delay = 0;
    m_g = 50.f;
    p_delay = Properties::createIntegerProperty("Delay(ms)", name, &m_delay);
    _engine->getProperties()->addProperty(p_delay);
    p_delay->addObserver(this);
    p_g = Properties::createFloatProperty("g(%)", name, &m_g);
    _engine->getProperties()->addProperty(p_g);
}
AllPass::~AllPass(){

}

std::string AllPass::getDescription(){
    return "Allpass";
}

void AllPass::setDelay(int ms){
    p_delay->setValue(ms);
}
void AllPass::setGValue(float g){
    p_g->setValue(g);
}

void AllPass::activated() {
    update();   // resize buffer
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
    m_bufferP = 0;

    std::cout << "Delay started with buffer of " << m_buffer.size() << "\n";
}
void AllPass::deactivated(){
    m_buffer.clear();
}
void AllPass::process(int nFrames) {

    float g = m_g/100.f;
    for (int i = 0; i < nFrames;++i){
        float delayinput = inputs[0]->samples[i] + m_buffer[m_bufferP]*g;
        outputs[0]->samples[i] = std::clamp((1-g*g)*m_buffer[m_bufferP] + (-g*inputs[0]->samples[i]), -1.f,1.f);
        m_buffer[m_bufferP] = delayinput;
        m_bufferP = (m_bufferP + 1) % m_buffer.size();
    }
}

void AllPass::update(){
    // delay update, resize buffer
    unsigned int bufferedFrames =  std::ceil(engine->frameRate*m_delay/1000);
    m_buffer.resize(bufferedFrames + 1);
    m_bufferP = m_bufferP % m_buffer.size();
}
