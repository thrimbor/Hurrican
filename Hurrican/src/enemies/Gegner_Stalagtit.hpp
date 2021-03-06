#ifndef _GEGNER_STALAKTIT_HPP_
#define _GEGNER_STALAKTIT_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerStalagtit : public GegnerClass {
  public:
    GegnerStalagtit(int Wert1,
                    int Wert2,  // Konstruktor
                    bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
};

#endif
