// WL_ATMOS.H

#include "version.h"

#if defined(USE_STARSKY) || defined(USE_RAIN) || defined(USE_SNOW)

#include "wl_def.h"

#if defined(USE_RAIN) || defined(USE_SNOW)
uint32_t rainpos;
#endif

typedef struct
{
    fixed x, y, z;
} point3d_t;

#define MAXPOINTS 400
point3d_t points[MAXPOINTS];

const float BASE_CENTERY = 100.0f;

byte moon[100] =
    {
        0,
        0,
        27,
        18,
        15,
        16,
        19,
        29,
        0,
        0,
        0,
        22,
        16,
        15,
        15,
        16,
        16,
        18,
        24,
        0,
        27,
        17,
        15,
        17,
        16,
        16,
        17,
        17,
        18,
        29,
        18,
        15,
        15,
        15,
        16,
        16,
        17,
        17,
        18,
        20,
        16,
        15,
        15,
        16,
        16,
        17,
        17,
        18,
        19,
        21,
        16,
        15,
        17,
        20,
        18,
        17,
        18,
        18,
        20,
        22,
        19,
        16,
        18,
        19,
        17,
        17,
        18,
        19,
        22,
        24,
        28,
        19,
        17,
        17,
        17,
        18,
        19,
        21,
        25,
        31,
        0,
        23,
        18,
        19,
        18,
        20,
        22,
        24,
        28,
        0,
        0,
        0,
        28,
        21,
        20,
        22,
        28,
        30,
        0,
        0,
};

/*
====================
=
= Init3DPoints
=
====================
*/

void Init3DPoints(void)
{
    int i, j;
    float length;
    point3d_t* pt;

    for (i = 0; i < MAXPOINTS; i++)
    {
        pt = &points[i];

        pt->x = 16384 - (rand() & 32767);
        pt->z = 16384 - (rand() & 32767);

        length = (float)sqrt((double)pt->x * pt->x + (double)pt->z * pt->z);
        j = 50;

        do
        {
            pt->y = 1024 + (rand() & 8191);
            j--;

        } while (j > 0 && ((float)pt->y * 256.0f) / length >= BASE_CENTERY);
    }
}

#endif

#ifdef USE_STARSKY

/*
====================
=
= DrawStarSky
=
====================
*/

void DrawStarSky(void)
{
    int i, j;
    point3d_t* pt;
    byte* dest;
    byte shade;
    int16_t stopx, starty, stopy;
    fixed x, y, z;
    fixed xx, yy;

    const int starSize = (int)(1.0f * scaleFactor);

    dest = vbuf;
    for (i = 0; i < centery; i++, dest += bufferPitch)
        memset(dest, 0, viewwidth);

    for (i = 0; i < MAXPOINTS; i++)
    {
        pt = &points[i];

        x = pt->x * viewcos + pt->z * viewsin;

        y = (int32_t)((pt->y << 16) * scaleFactor);
        z = ((pt->z * viewcos - pt->x * viewsin)) >> 8;

        if (z <= 0)
            continue;

        shade = (byte)(z >> 18);

        if (shade > 15)
            continue;

        xx = ((x / z) * scaleFactor) + (centerx + 1);
        yy = centery - (y / z);

        if (xx >= 0 && xx < viewwidth - starSize && yy >= 0 && yy < centery - starSize)
        {
            for (int dy = 0; dy < starSize; dy++)
            {
                for (int dx = 0; dx < starSize; dx++)
                {
                    vbuf[ylookup[yy + dy] + (xx + dx)] = shade + 15;
                }
            }
        }
    }

    x = (16384 * viewcos) + (16384 * viewsin);
    z = ((16384 * viewcos) - (16384 * viewsin)) >> 8;

    if (z <= 0)
        return;

    xx = ((x / z) * scaleFactor) + (centerx + 1);

    int32_t rawMoonY = ((((int32_t)BASE_CENTERY - ((int32_t)BASE_CENTERY >> 3)) << 22) / z);
    yy = centery - (int32_t)(rawMoonY * scaleFactor);

    int moonScale = scaleFactor * (int)scaleFactor;
    if (moonScale < 1) moonScale = 1;

    if (xx > (moonScale * -10) && xx < viewwidth)
    {
        stopx = 10 * moonScale;
        starty = 0;
        stopy = 10 * moonScale;
        i = 0;

        if (xx < 0)
            i = -xx;
        if (xx >= viewwidth - (10 * moonScale))
            stopx = viewwidth - xx;

        if (yy < 0)
            starty = -yy;
        if (yy >= viewheight - (10 * moonScale))
            stopy = viewheight - yy;

        while (i < stopx)
        {
            for (j = starty; j < stopy; j++)
            {
                int srcX = i / moonScale;
                int srcY = j / moonScale;
                if (srcX < 10 && srcY < 10)
                {
                    byte col = moon[(srcY * 10) + srcX];
                    if (col)
                        vbuf[ylookup[yy + j] + xx + i] = col;
                }
            }
            i++;
        }
    }
}

#endif

#ifdef USE_RAIN

/*
====================
=
= DrawRain
=
====================
*/

void DrawRain(void)
{
#if defined(USE_FLOORCEILINGTEX) && defined(FIXRAINSNOWLEAKS)
    byte tilex, tiley;
    int16_t prestep;
    fixed basedist, stepscale;
    fixed xfrac, yfrac;
    fixed xstep, ystep;
#endif

    int i;
    point3d_t* pt;
    byte shade;
    int32_t ax, az, x, y, z, xx, yy, height, actheight;
    fixed px, pz;

    const int rainStreakSize = (int)(1.0f * scaleFactor);

    px = (player->y + FixedMul(0x7900, viewsin)) >> 6;
    pz = (player->x - FixedMul(0x7900, viewcos)) >> 6;

    rainpos -= tics * 900;

    for (i = 0; i < MAXPOINTS; i++)
    {
        pt = &points[i];

        ax = pt->x + px;
        ax = 0x1fff - (ax & 0x3fff);
        az = pt->z + pz;
        az = 0x1fff - (az & 0x3fff);
        x = (ax * viewcos) + (az * viewsin);

        int32_t rawY = (((pt->y << 6) + rainpos) & 0x0ffff);
        y = -(heightnumerator << 7) + (int32_t)(rawY * 2048.0f * scaleFactor);

        z = ((az * viewcos) - (ax * viewsin)) >> 8;

        if (z <= 0)
            continue;

        shade = (byte)(z >> 17);

        if (shade > 13)
            continue;

        xx = (x / z) + (centerx + 1);

        if (xx < 0 || xx >= viewwidth)
            continue;

        actheight = y / z;
        yy = centery - actheight;
        height = (heightnumerator << 10) / z;

        if (actheight < 0)
            actheight = -actheight;

        int32_t wallThresh = (int32_t)((wallheight[xx] >> 3) * scaleFactor);
        if (actheight < wallThresh && height < wallheight[xx])
            continue;

        if (xx >= 0 && xx < viewwidth && (yy - (rainStreakSize * 3)) >= 0 && yy < viewheight)
        {
#if defined(USE_FLOORCEILINGTEX) && defined(FIXRAINSNOWLEAKS)
            prestep = centerx - xx + 1;
            basedist = FixedDiv(scale, (height >> 3) + 1) >> 1;
            stepscale = basedist / scale;

            xstep = FixedMul(stepscale, viewsin);
            ystep = -FixedMul(stepscale, viewcos);

            xfrac = (viewx + FixedMul(basedist, viewcos)) - (xstep * prestep);
            yfrac = -(viewy - FixedMul(basedist, viewsin)) - (ystep * prestep);

            int calcTileX = (xfrac >> TILESHIFT);
            int calcTileY = (~yfrac >> TILESHIFT);

            if (calcTileX >= 0 && calcTileX < mapwidth && calcTileY >= 0 && calcTileY < mapheight)
            {
                tilex = (byte)calcTileX;
                tiley = (byte)calcTileY;

                if (MAPSPOT(tilex, tiley, 2) >> 8)
                    continue;
            }
#endif

            for (int s = 0; s < rainStreakSize; s++)
            {
                int ypos = yy - s;
                if (ypos >= 0 && ypos < viewheight)
                    vbuf[ylookup[ypos] + xx] = shade + 15;
            }

            for (int s = 0; s < rainStreakSize; s++)
            {
                int ypos = yy - rainStreakSize - s;
                if (ypos >= 0 && ypos < viewheight)
                    vbuf[ylookup[ypos] + xx] = shade + 16;
            }

            for (int s = 0; s < rainStreakSize; s++)
            {
                int ypos = yy - (rainStreakSize * 2) - s;
                if (ypos >= 0 && ypos < viewheight)
                    vbuf[ylookup[ypos] + xx] = shade + 17;
            }
        }
    }
}

#endif

#ifdef USE_SNOW

/*
====================
=
= DrawSnow
=
====================
*/

void DrawSnow(void)
{
#if defined(USE_FLOORCEILINGTEX) && defined(FIXRAINSNOWLEAKS)
    byte tilex, tiley;
    int16_t prestep;
    fixed basedist, stepscale;
    fixed xfrac, yfrac;
    fixed xstep, ystep;
#endif

    int i;
    point3d_t* pt;
    byte shade;
    int32_t ax, az, x, y, z, xx, yy, height, actheight;
    fixed px, pz;

    const int snowFlakeSize = (int)(1.0f * scaleFactor);

    px = (player->y + FixedMul(0x7900, viewsin)) >> 6;
    pz = (player->x - FixedMul(0x7900, viewcos)) >> 6;

    rainpos -= tics * 256;

    for (i = 0; i < MAXPOINTS; i++)
    {
        pt = &points[i];

        ax = pt->x + px;
        ax = 0x1fff - (ax & 0x3fff);
        az = pt->z + pz;
        az = 0x1fff - (az & 0x3fff);
        x = (ax * viewcos) + (az * viewsin);

        int32_t rawY = (((pt->y << 6) + rainpos) & 0x0ffff);
        y = -(heightnumerator << 7) + (int32_t)(rawY * 2048.0f * scaleFactor);

        z = ((az * viewcos) - (ax * viewsin)) >> 8;

        if (z <= 0)
            continue;

        shade = (byte)(z >> 17);

        if (shade > 13)
            continue;

        xx = (x / z) + (centerx + 1);

        if (xx < 0 || xx >= viewwidth)
            continue;

        actheight = y / z;
        yy = centery - actheight;
        height = (heightnumerator << 10) / z;

        if (actheight < 0)
            actheight = -actheight;

        int32_t wallThresh = (int32_t)((wallheight[xx] >> 3) * scaleFactor);
        if (actheight < wallThresh && height < wallheight[xx])
            continue;

        int maxOffset = (shade < 10) ? (snowFlakeSize * 2) : snowFlakeSize;

        if (xx >= maxOffset && xx < viewwidth - maxOffset && yy >= maxOffset && yy < viewheight - maxOffset)
        {
#if defined(USE_FLOORCEILINGTEX) && defined(FIXRAINSNOWLEAKS)
            prestep = centerx - xx + 1;
            basedist = FixedDiv(scale, (height >> 3) + 1) >> 1;
            stepscale = basedist / scale;

            xstep = FixedMul(stepscale, viewsin);
            ystep = -FixedMul(stepscale, viewcos);

            xfrac = (viewx + FixedMul(basedist, viewcos)) - (xstep * prestep);
            yfrac = -(viewy - FixedMul(basedist, viewsin)) - (ystep * prestep);

            int calcTileX = (xfrac >> TILESHIFT);
            int calcTileY = (~yfrac >> TILESHIFT);

            if (calcTileX >= 0 && calcTileX < mapwidth && calcTileY >= 0 && calcTileY < mapheight)
            {
                tilex = (byte)calcTileX;
                tiley = (byte)calcTileY;

                if (MAPSPOT(tilex, tiley, 2) >> 8)
                    continue;
            }
#endif

            if (shade < 10)
            {
                for (int dy = 0; dy < snowFlakeSize; dy++)
                {
                    for (int dx = 0; dx < snowFlakeSize; dx++)
                    {
                        int y1 = yy - dy;
                        int y2 = yy - snowFlakeSize - dy;
                        int x1 = xx - dx;
                        int x2 = xx - snowFlakeSize - dx;

                        if (y1 >= 0 && y1 < viewheight && y2 >= 0 && y2 < viewheight &&
                            x1 >= 0 && x1 < viewwidth && x2 >= 0 && x2 < viewwidth)
                        {
                            vbuf[ylookup[y1] + x1] = shade + 17;
                            vbuf[ylookup[y1] + x2] = shade + 16;
                            vbuf[ylookup[y2] + x1] = shade + 16;
                            vbuf[ylookup[y2] + x2] = shade + 15;
                        }
                    }
                }
            }
            else
            {
                for (int dy = 0; dy < snowFlakeSize; dy++)
                {
                    for (int dx = 0; dx < snowFlakeSize; dx++)
                    {
                        int y1 = yy - dy;
                        int x1 = xx - dx;

                        if (y1 >= 0 && y1 < viewheight && x1 >= 0 && x1 < viewwidth)
                        {
                            vbuf[ylookup[y1] + x1] = shade + 15;
                        }
                    }
                }
            }
        }
    }
}

#endif
