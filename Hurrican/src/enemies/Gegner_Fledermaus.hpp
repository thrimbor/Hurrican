#ifndef _GEGNER_FLEDERMAUS_HPP_
#define _GEGNER_FLEDERMAUS_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerFledermaus : public GegnerClass {
  public:
    GegnerFledermaus(int Wert1,
                     int Wert2,  // Konstruktor
                     bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
};

#endif
