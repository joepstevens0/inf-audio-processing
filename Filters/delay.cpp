#include "delay.h"
#include <cstring>
#include <cmath>

Delay::Delay(AudioEngine* _engine, const std::string &name)
: AudioFilter(_engine, name, 1, 1){
    m_delay = 0;
    p_delay = Properties::createIntegerProperty("Delay(ms)", name, &m_delay);
    _engine->getProperties()->addProperty(p_delay);
    p_delay->addObserver(this);
}
Delay::~Delay(){

}

std::string Delay::getDescription(){
    return "Delay";
}

void Delay::setDelay(int ms){
    p_delay->setValue(ms);
}

void Delay::activated() {
    update();   // resize buffer
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
    m_bufferP = 0;

    std::cout << "Delay started with buffer of " << m_buffer.size() << "\n";
}
void Delay::deactivated(){
    m_buffer.clear();
}
void Delay::process(int nFrames) {

    for (int i = 0; i < nFrames;++i){
        outputs[0]->samples[i] = m_buffer[m_bufferP];
        m_buffer[m_bufferP] = inputs[0]->samples[i];
        m_bufferP = (m_bufferP + 1) % m_buffer.size();
    }
}

void Delay::update(){
    // delay update, resize buffer
    unsigned int bufferedFrames =  std::ceil(engine->frameRate*m_delay/1000);
    m_buffer.resize(bufferedFrames + 1);
    m_bufferP = m_bufferP % m_buffer.size();
}
