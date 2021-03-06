#ifndef _GEGNER_FIRESPIRDER_HPP_
#define _GEGNER_FIRESPIRDER_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerFireSpider : public GegnerClass {
  private:
    float rot;
    float shotdelay;

  public:
    GegnerFireSpider(int Wert1,
                     int Wert2,  // Konstruktor
                     bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
    void DoDraw() override;         // Gegner individuell rendern
};

#endif
