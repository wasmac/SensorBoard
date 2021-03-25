#include <Filter.h>
#include <MegunoLink.h>

#define MIC_PIN_SPL 34
#define NOISE_SPL 500

class micSPL
{
public:
    String handleMic();
private:
    int lvl = 0, minLvl = 0, maxLvl = 300; // define the variables needed for the audio levels
    
};