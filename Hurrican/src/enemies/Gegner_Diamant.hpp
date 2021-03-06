#ifndef _GEGNER_DIAMANT_H
#define _GEGNER_DIAMANT_H

#include "GegnerClass.hpp"
#include "Gegner_Stuff.hpp"

class GegnerDiamant : public GegnerClass {
  public:
    GegnerDiamant(int Wert1,
                  int Wert2,  // Konstruktor
                  bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
};

#endif
