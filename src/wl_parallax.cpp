// WL_PARALLAX.C

#include "version.h"

#ifdef USE_PARALLAX

#include "wl_def.h"

#ifdef USE_FEATUREFLAGS

// The lower left tile of every map determines the start texture of the parallax sky.
int GetParallaxStartTexture(void)
{
    int startTex = ffDataBottomLeft;

    assert(startTex >= 0 && startTex < PMSpriteStart);

    return startTex;
}

#else

int GetParallaxStartTexture(void)
{
    int startTex;

    switch (gamestate.episode * 10 + gamestate.mapon)
    {
    case 0:
        startTex = 20;
        break;
    default:
        startTex = 0;
        break;
    }

    assert(startTex >= 0 && startTex < PMSpriteStart);

    return startTex;
}

#endif

/*
====================
=
= DrawParallax
=
====================
*/

void DrawParallax(void)
{
    int x, y;
    byte* dest, * skysource;
    int16_t angle;
    int16_t skypage, curskypage;
    int16_t lastskypage;
    int16_t xtex;
    int16_t toppix;
    int texX, texY;

    if (!vbuf)
        return;

    skypage = GetParallaxStartTexture();
    lastskypage = -1;
    skysource = NULL;

    for (x = 0; x < viewwidth; x++)
    {
        toppix = centery - (wallheight[x] >> 3);

        if (toppix <= 0)
            continue;
        if (toppix > viewheight)
            toppix = viewheight;

        angle = pixelangle[x] + midangle;

        if (angle < 0)
            angle += FINEANGLES;
        else if (angle >= FINEANGLES)
            angle -= FINEANGLES;

        xtex = ((int32_t)angle * USE_PARALLAX * TEXTURESIZE) / FINEANGLES;
        curskypage = (xtex / TEXTURESIZE) % USE_PARALLAX;

        if (curskypage < 0)
            curskypage += USE_PARALLAX;

        if (lastskypage != curskypage)
        {
            lastskypage = curskypage;
            skysource = PM_GetPage(skypage + curskypage);
        }

        if (!skysource)
            continue;

        texX = xtex & (TEXTURESIZE - 1);

        dest = &vbuf[ylookup[0] + x];

        for (y = 0; y < toppix; y++)
        {
            texY = (y * TEXTURESIZE) / centery;
            if (texY >= TEXTURESIZE)
                texY = TEXTURESIZE - 1;

            *dest = skysource[(texX << TEXTURESHIFT) + texY];
            dest += bufferPitch;
        }
    }
}

#endif
