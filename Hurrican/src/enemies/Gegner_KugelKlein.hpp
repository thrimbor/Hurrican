#ifndef _GEGNER_KUGELKLEIN_HPP_
#define _GEGNER_KUGELKLEIN_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerKugelKlein : public GegnerClass {
  public:
    GegnerKugelKlein(int Wert1,
                     int Wert2,  // Konstruktor
                     bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
};

#endif
