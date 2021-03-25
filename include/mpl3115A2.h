#include <Wire.h>

#define Addr 0x60

class mpl3115A2
{
public:
    void setup();
    String handleTemp();
    double handlePressure() ;
    
};

