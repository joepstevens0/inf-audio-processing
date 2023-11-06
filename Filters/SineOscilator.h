#ifndef SINEOSCILATOR_H
#define SINEOSCILATOR_H

#include <vector>
#include <stdio.h>

#include "Framework/Audio.h"

class SineOscilator: public AudioFilter {
 protected:

 public:
  SineOscilator(AudioEngine* engine, const std::string &name);
  ~SineOscilator();

  std::string getDescription(void);

  //Update properties through code
  void setFrequency(float frequency);
  void setPhaseShift(float phase);

private:
  float m_frequency, m_phaseShift;
  double m_PI;

protected:

  // AudioFilter callbacks
  void activated();
  void process(int nFrames);

  //Properties
  FloatPropertyPtr p_frequency;
  FloatPropertyPtr p_phaseshift;
};

#endif // SINEOSCILATOR_H

