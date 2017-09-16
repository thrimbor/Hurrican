#ifndef _GEGNER_SPITTER_H
#define _GEGNER_SPITTER_H

#include "GegnerClass.hpp"
#include "Gegner_Stuff.hpp"

class GegnerSpitter : public GegnerClass
{
public:

    GegnerSpitter		(int Wert1, int Wert2,			// Konstruktor
                         bool			Light);
    void GegnerExplode  (void);							// Gegner explodiert
    void DoKI			(void);							// Gegner individuell mit seiner
    // eigenen kleinen KI bewegen
};

#endif