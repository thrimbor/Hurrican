#ifndef _GEGNER_LAFASS_HPP_
#define _GEGNER_LAFASS_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerLaFass : public GegnerClass {
  public:
    GegnerLaFass(int Wert1,
                 int Wert2,  // Konstruktor
                 bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
};

#endif
