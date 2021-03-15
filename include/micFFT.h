#include <arduinoFFT.h>

#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN    34            // Signal in on this pin
#define NOISE           500           // Used as a crude noise filter, values below this are ignored
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
#define NUM_LEDS       (kMatrixWidth * kMatrixHeight)     // Total number of LEDs
#define BAR_WIDTH      (kMatrixWidth  / (NUM_BANDS - 1))  // If width >= 8 light 1 LED width per bar, >= 16 light 2 LEDs width bar etc
#define TOP            (kMatrixHeight - 0)                // Don't allow the bars to go offscreen
#define SERPENTINE     true                               // Set to false if you're LEDS are connected end to end, true if serpentine
#define NUM_BANDS       8            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands


class micFFT
{
public:
    void setup();
    void reset_samples();
    void sample();
    void calc_FFT();
    void FFT_to_bands();
    void FFT_to_bands_height();
    void FFT_bands_decay();
private:
    unsigned int sampling_period_us;
    byte peak[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};              // The length of these arrays must be >= NUM_BANDS
    int oldBarHeights[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int bandValues[8] = {0,0,0,0,0,0,0,0};
    double vReal[SAMPLES];
    double vImag[SAMPLES];
    unsigned long newTime;
    arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
    
};