#ifndef _GEGNER_SCHLEIMMAUL_HPP_
#define _GEGNER_SCHLEIMMAUL_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerSchleimMaul : public GegnerClass {
  private:
    float SpawnCount;
    float ShotDelay;
    GegnerClass *pChildren[10];

  public:
    GegnerSchleimMaul(float x, float y, int Wert1, int Wert2, bool Light);
    void GegnerExplode() override;  // Gegner explodiert
    void DoKI() override;           // Gegner individuell mit seiner eigenen kleinen KI bewegen
    void DoDraw() override;         // Gegner individuell rendern
};

#endif
