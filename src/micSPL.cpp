#include <micSPL.h>

ExponentialFilter<long> ADC_Filter(5,0); // instantiate the filter class for smoothing the raw audio signal

String micSPL::handleMic(){
  int n;
  n = analogRead(MIC_PIN_SPL);
  n = abs(1023 - n);  // remove the MX9614 bias of 1.25VDC
  n = (n <= NOISE_SPL) ? 0 : abs(n - NOISE_SPL);  // hard limit noise/hum
  ADC_Filter.Filter(n);  // apply the exponential filter to smooth the raw signal
  lvl = ADC_Filter.Current();
  String sound_level = String(lvl);
  return sound_level;
}