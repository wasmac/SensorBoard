#include "Arduino.h"
#include <arduinoFFT.h>
#include <micFFT.h>

void micFFT::setup()
{
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}

void micFFT::reset_samples()
{
  // Reset bandValues[]
  for (int i = 0; i<NUM_BANDS; i++){
    bandValues[i] = 0;
  }
}

void micFFT::sample()
{
  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }
}

void micFFT::calc_FFT()
{
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
}

void micFFT::FFT_to_bands()
{
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter

    /*//8 bands, 12kHz top band
      if (i<=3 )           bandValues[0]  += (int)vReal[i];
      if (i>3   && i<=6  ) bandValues[1]  += (int)vReal[i];
      if (i>6   && i<=13 ) bandValues[2]  += (int)vReal[i];
      if (i>13  && i<=27 ) bandValues[3]  += (int)vReal[i];
      if (i>27  && i<=55 ) bandValues[4]  += (int)vReal[i];
      if (i>55  && i<=112) bandValues[5]  += (int)vReal[i];
      if (i>112 && i<=229) bandValues[6]  += (int)vReal[i];
      if (i>229          ) bandValues[7]  += (int)vReal[i];*/

    //16 bands, 12kHz top band
      if (i==1 )           bandValues[0]  += (int)vReal[i]; //40
      if (i>1   && i<=2  ) bandValues[1]  += (int)vReal[i]; //59
      if (i>2   && i<=3  ) bandValues[2]  += (int)vReal[i]; //86
      if (i>3   && i<=4  ) bandValues[3]  += (int)vReal[i]; //125
      if (i>4   && i<=6  ) bandValues[4]  += (int)vReal[i]; //183
      if (i>6   && i<=8  ) bandValues[5]  += (int)vReal[i]; //268
      if (i>8  && i<=12  ) bandValues[6]  += (int)vReal[i]; //392
      if (i>12  && i<=18 ) bandValues[7]  += (int)vReal[i]; //576
      if (i>18  && i<=26 ) bandValues[8]  += (int)vReal[i]; //838
      if (i>26  && i<=39 ) bandValues[9]  += (int)vReal[i]; //1226
      if (i>39  && i<=57 ) bandValues[10] += (int)vReal[i]; //1793
      if (i>57  && i<=83 ) bandValues[11] += (int)vReal[i]; //2622
      if (i>83  && i<=121) bandValues[12] += (int)vReal[i]; //3835
      if (i>121 && i<=177) bandValues[13] += (int)vReal[i]; //5609
      if (i>177 && i<=259) bandValues[14] += (int)vReal[i]; //8204
      if (i>259          ) bandValues[15] += (int)vReal[i]; //12000 and more
    }
  }
}

DynamicJsonDocument micFFT::FFT_to_bands_height()
{
  DynamicJsonDocument doc(512);
  for (byte band = 0; band < NUM_BANDS; band++) {

    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 1; //

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

    doc["band"][band] = band;
    doc["barHeight"][band] = barHeight;



    Serial.print("band:");
    Serial.print(band);
    Serial.print(" height:");
    Serial.print(barHeight);
    Serial.print(" ");

    }
    Serial.println(" ");
    return doc;
}


void micFFT::FFT_bands_decay()
{
    for (byte band = 0; band < NUM_BANDS; band++)
      if (peak[band] > 0) peak[band] -= 1;
}