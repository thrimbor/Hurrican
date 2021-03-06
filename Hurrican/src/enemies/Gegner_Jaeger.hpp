#ifndef _GEGNER_JAEGER_HPP_
#define _GEGNER_JAEGER_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerJaeger : public GegnerClass {
  private:
    float smokeDelay;

  public:
    GegnerJaeger(int Wert1,
                 int Wert2,  // Konstruktor
                 bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
    void DoDraw() override;         // Gegner individuell rendern
};

#endif
