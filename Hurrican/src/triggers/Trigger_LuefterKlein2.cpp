// --------------------------------------------------------------------------------------
// Der kleine, kaputte Luefter
// --------------------------------------------------------------------------------------

#include "Trigger_LuefterKlein2.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerLuefterKlein2::GegnerLuefterKlein2(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_STEHEN;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = false;
    ForceLight = true;
    OwnDraw = true;
    Energy = 100;
    AnimEnde = 11;
    BackGround = true;

    if (Value1 == 0)
        AnimSpeed = 0.5f;
    else
        AnimSpeed = Value1 / 10.0f;
}

// --------------------------------------------------------------------------------------
// Rendern
// --------------------------------------------------------------------------------------

void GegnerLuefterKlein2::DoDraw() {
    D3DCOLOR Color = TileEngine.LightValue(xPos, yPos, GegnerRect[GegnerArt], ForceLight);
    pGegnerGrafix[GegnerArt]->RenderSprite(static_cast<float>(xPos - TileEngine.XOffset),
                                           static_cast<float>(yPos - TileEngine.YOffset), AnimPhase, Color, false);
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerLuefterKlein2::DoKI() {
    SimpleAnimation();
}

// --------------------------------------------------------------------------------------
// LuefterKlein2 explodiert
// --------------------------------------------------------------------------------------

void GegnerLuefterKlein2::GegnerExplode() {}
