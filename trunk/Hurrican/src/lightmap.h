// Datei : lightmap.h

// --------------------------------------------------------------------------------------
//
// Lightmaps f�r das Hurrican Level
//
// (c) 2005 J�rg M. Winterstein
//
// --------------------------------------------------------------------------------------

#ifndef _LIGHTMAP_H_
#define _LIGHTMAP_H_

// --------------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------------

#include "Globals.h"


// --------------------------------------------------------------------------------------
// LigthMap Klasse
// --------------------------------------------------------------------------------------

class CLightMap
{

public:

    int			xsize, ysize;
    D3DCOLOR*	map;

    CLightMap(void);
    ~CLightMap(void);

    void	  Load(const char *filename);
};

#endif
