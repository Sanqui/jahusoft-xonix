/************************************************************************
 *                              X O N I X                               *
 *                  Copyright (c) 1991, Jan Hubicka                     *
 *                          All Rights Reserved                         *
 *			  see README for details			*
 *                                                                      *
 * Authors:                                                             *
 *        Jan Hubicka(JAHUSOFT)                                         *
 *                                                                      *
 * Adress:                                                              *
 *        JAHUSOFT                                                      *
 *        Jan Hubicka                                                   *
 *        Dukelskych bojovniku 1944                                     *
 *        Tabor                                                         *
 *        390 03                                                        *
 *        CR                                                            *
 *                                                                      *
 * Versions:                                                            *
 *    V1.0 7.8.1991                                                	*
 *    V1.1 29.6.1995 revised and ported for linux			*
 *                   sorry for stupid code...It was my forst work on    *
 *								  unix  *
 *                                                                      *
 ************************************************************************/
/*
 * xonix.c
 *
 * This is first game from jahusoft.Created for X-Window
 * in K&R C (compile using cc) on HP_UX
 * run only in 256 posixcolor mode(sorry)
 */
/*#define MEM_DEBUG *//*For JAHUSOFT dynamic memory */
/*#include "memory.h" *//*debugger                   */
struct main
  {
    char ahoj:1, nazdar:1;
  }
p[5];
#define GIFDIR GifDIR.addr
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <sys/signal.h>
#include <signal.h>
#include <time.h>
#include <dirent.h>
#define MAIN
#include "xgif.h"
#include <math.h>
#include "xonix.bm"
#include "xgifload.c" 
                        /*ugh ugh!!???????????!!!!!!!!very sstupid!!!!!!!!
			(I did it 5 year ago........)*/
#define FREE 2
#define HRAJEME 0
#define DEMO 3
#define UKAZUJEM 4
#define DALSI 1
#define SIRKAS /*(Xkrok>Ykrok?Ykrok/2:Xkrok/2)*/2
#define PREKRESLI 1
#define NORMAL 0
#define DODRAW 2
Display *dp;
Window wi;
XFontStruct *fs;
int screen, live = 5, SIRKA = 20, klavesa = 0;
Bool pokus ();
int smer = 0, smer1 = 0, x = 0, y = 0, score = 0, stav = HRAJEME;
XColor *colors;
int ncolor = 0;
long border, back;
#define KeyUp 134
#define KeyDown 133
#define KeyLeft 132
#define KeyRight 135
#define VELX 80
#define VELY 50
#define PANACEK 255
#define PRISERA 254
#define ZABRANO 253
#define MAXNECARA 252
#define KONCIME 2
#define ROZMERX 40
#define ROZMERY 14
char logo[ROZMERY][ROZMERX] =
{0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0,
 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

int CASTO = 30000;
int CASTO1 = 0;
XrmValue backcolor, forecolor, leftshadow, rightshadow, font1;
XrmValue Man, Text, UseGif, GifDIR, display, Priserac;
#define maxprisery 100
int pocprisery = 2, pocvybarv = 0;
int ps[maxprisery][2];
int pp[maxprisery][2];
int Xkrok = 10;
int Ykrok = 10;
int level = 1;
int xs, ys, minule = 0, vyhral = 0;
int zabrano = 0, limit = (int) ((75.0 / 100.0) * VELX * VELY);
/*int zabrano=0,limit=(int)((0.0/100.0)*VELX*VELY); */
int usegif = 0, image1 = 0, image2 = 0;
int pocetgifu = 0;
XWindowAttributes xwa;
float vo = 0;			/*VOLUME=1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
GC gc;
int bezi = 0;
int prekresli = 0;
unsigned char pole[VELX][VELY], pole2, lpole[VELX][VELY];
struct zabrano
  {
    unsigned char u:1, l:1, d:1, r:1, c1:1, c2:1, c3:1, c4:1;
  }
lpol[VELX][VELY];
int pole1[VELX][VELY];
XEvent myEvent;
unsigned long Panacek, Vyplneno, Textc, stin, stin1, obsah, stin2, stin3;
#define JePole(x,y) (((x)>=0&&(y)>=0&&(x)<VELX&&(y)<VELY)?\
                    ((pole[x][y]==PANACEK)?pole2:pole[x][y]):0)
unsigned long ColorRGBToPixel ();
char *data1, *data2;
XImage *koule1, *obrazek;
DIR *dir;


zjistipocet ()
{
  chdir (GIFDIR);
  dir = opendir (".");
  while (readdir (dir) != NULL)
    pocetgifu++;
}
char file[255];

int nahraj ();
nactidalsi ()
{
  int i;
  struct dirent *di;
  file[0] = 0;
  strcpy (file, GIFDIR);
  strcat (file, "/");
  do
    {
      i = rand () % pocetgifu;
      for (; i >= 0; i--)
	if ((di = readdir (dir)) == NULL)
	  rewinddir (dir), di = readdir (di);
    }
  while (!(((di->d_name[strlen (di->d_name) - 1] == 'F' || di->d_name[strlen (di->d_name) - 1] == 'f')) &&
	   ((di->d_name[strlen (di->d_name) - 2] == 'I' || di->d_name[strlen (di->d_name) - 2] == 'i')) &&
	   ((di->d_name[strlen (di->d_name) - 3] == 'G' || di->d_name[strlen (di->d_name) - 3] == 'g')) &&
	   (di->d_name[strlen (di->d_name) - 4] == '.')));
  strcat (file, di->d_name);
}

FatalError (c)
     char *c;
{
  printf ("FatalError in loading file:%s\n", c);	/*usegif=0; */
}

nahraj (name)
     char *name;
{
  theDisp = dp;
  BitOffset = 0, XC = 0, YC = 0, Pass = 0, OutCount = 0, Verbose = False;
  theScreen = DefaultScreen (theDisp);
  rootW = RootWindow (theDisp, theScreen);
  theGC = gc;
  fcol = WhitePixel (theDisp, theScreen);
  bcol = BlackPixel (theDisp, theScreen);
  theVisual = DefaultVisual (theDisp, theScreen);
  dispcells = DisplayCells (theDisp, theScreen);
  mainW = wi;
  if (image1)
    {
      register int poc = 0, i;
      register long *pixels;
      pixels = (long *) malloc (numused * sizeof (long));
      for (i = 0; i < numused; i++)
	if (used[i] && defs[i].pixel != 0xffff && used[i] != 6)
	  pixels[poc] = (long) defs[i].pixel, poc++;
      XFreeColors (dp, theCmap, pixels, poc, 0);
      XFlush (dp);
      free (pixels);

      XDestroyImage (theImage);	/*free(Image); */
    }
  theCmap = DefaultColormap (theDisp, theScreen);
  image1 = 1;
  usegif = 1;
  nostrip = 0;
  LoadGIF (name);
  resizni ();
}

resizni ()
{
  register int x, y;
  int xv, yv;
  char *data3;
  if (usegif)
    {
      xv = VELX * Xkrok;
      yv = VELY * Ykrok;
      if (image2)
	free (data2);
      image2 = 1;
      if ((data2 = (char *) malloc (xv * yv)) == NULL)
	perror ("Memory Error"), exit (2);
      obrazek = XCreateImage (dp, DefaultVisual (dp, screen), 8
			      ,ZPixmap, 0, data2, xv,
			      yv, 8, xv);
      if (obrazek == NULL)
	exit ();
      data3 = data2;
      {
	register char *data, *data1;	/*bresenham */
	register int ddx, sp, e;
	register int ddy, spy, ey;
	register int ddx1, ddy1;
	ddx1 = xv + xv;
	ddx = theImage->width + theImage->width;
	if (ddx1 < ddx)
	  sp = ddx / ddx1, ddx = ddx % ddx1;
	else
	  sp = 0;
	e = ddx1 / 2;

	ddy1 = (yv + yv);
	ddy = theImage->height + theImage->height - 2;
	if (ddy1 < ddy)
	  spy = ddy / ddy1, ddy = ddy % ddy1;
	else
	  spy = 0;
	ey = ddy1 / 2;
	spy *= theImage->width;

	data = data1 = (char *) (Image);
	for (y = 0; y < yv; y++)
	  {
	    for (x = 0; x < xv; x++)
	      {
		*data3 = *data;
		data3++;
		data += sp;
		if (e > 0)
		  {
		    data++;
		    e -= ddx1;
		  }
		e += ddx;
	      }
	    data1 += spy;
	    if (ey > 0)
	      {
		data1 += theImage->width;
		ey -= ddy1;
	      }
	    ey += ddy;
	    data = data1;

	  }
      }
    }
}
koule ()
{
  register int x, y, m;
  register long r, g, b, r1 = 65535, g1 = 65535 / 8, b1 = 65535 / 8;
  Colormap colormap;
  XColor color;

  colormap = XDefaultColormap (dp, DefaultScreen (dp));
  XParseColor (dp, colormap, Priserac.addr, &color);
  r1 = color.red;
  g1 = color.green;
  b1 = color.blue;
  if ((data1 = (char *) malloc (Xkrok * Ykrok * DefaultDepth (dp, screen) / 8)) == NULL)
    perror ("Memory Error"), exit (2);
  koule1 = XCreateImage (dp, DefaultVisual (dp, screen), DefaultDepth (dp, screen)
			 ,ZPixmap, 0, data1, Xkrok,
   Ykrok, DefaultDepth (dp, screen), Xkrok * DefaultDepth (dp, screen) / 8);
  if (Ykrok > Xkrok)
    m = Xkrok;
  else
    m = Ykrok;
  for (x = 0; x < Xkrok; x++)
    for (y = 0; y < Ykrok; y++)
      {
	if (((x - Xkrok / 2) * (x - Xkrok / 2) + (y - Ykrok / 2) * (y - Ykrok / 2)) < (m / 2 * m / 2))
	  {
	    r = 65535 - pow ((double) (((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)) / (float)
				       ((1.3 * m / 4 * 1.3 * m / 4) == 0 ? 1 : (1.3 * m / 4 * 1.3 * m / 4))), 0.4) * 65535.0;
	    g = 65535 - pow ((double) (((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)) / (float)
				       ((1.3 * m / 4 * 1.3 * m / 4) == 0 ? 1 : (1.3 * m / 4 * 1.3 * m / 4))), 0.4) * 65535.0;
	    b = 65535 - pow ((double) (((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)) / (float)
				       ((1.3 * m / 4 * 1.3 * m / 4) == 0 ? 1 : (1.3 * m / 4 * 1.3 * m / 4))), 0.4) * 65535.0;
	    if (r < 0)
	      r = 0;
	    if (r > 65535)
	      r = 65535;
	    if (g < 0)
	      g = 0;
	    if (g > 65535)
	      g = 65535;
	    if (b < 0)
	      b = 0;
	    if (b > 65535)
	      b = 65535;
	    r += fabs (pow ((1.0 - sqrt ((double) ((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)))
			     / (float) m), 1.0) * r1);
	    g += fabs (pow ((1.0 - sqrt ((double) ((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)))
			     / (float) m), 1.0) * g1);
	    b += fabs (pow ((1.0 - sqrt ((double) ((x - 1.3 * Xkrok / 4) * (x - 1.3 * Xkrok / 4) + (y - 1.3 * Ykrok / 4) * (y - 1.3 * Ykrok / 4)))
			     / (float) m), 1.0) * b1);
	    if (r < 0)
	      r = 0;
	    if (r > 65535)
	      r = 65535;
	    if (g < 0)
	      g = 0;
	    if (g > 65535)
	      g = 65535;
	    if (b < 0)
	      b = 0;
	    if (b > 65535)
	      b = 65535;
	    XPutPixel (koule1, x, y, ColorRGBToPixel (dp, (short) r, (short) g, (short) b, 256 * 16));
	  }
	else
	  XPutPixel (koule1, x, y, back);
      }
}

unsigned long 
ColorNameToPixel (disp, name)
     char *name;
     Display *disp;
{
  Colormap colormap;
  XColor color;

  colormap = XDefaultColormap (disp, DefaultScreen (disp));
  XParseColor (disp, colormap, name, &color);
  XAllocColor (disp, colormap, &color);
  return (color.pixel);
}
unsigned long 
ColorRGBToPixel (disp, r, g, b, strip)
     unsigned short r, g, b, strip;
     Display *disp;
{
  Colormap colormap;
  int ncol1 = ncolor;
  colormap = XDefaultColormap (disp, DefaultScreen (disp));
  colors[ncolor].red = (unsigned short) (r / strip) * strip;
  colors[ncolor].green = (unsigned short) (g / strip) * strip;
  colors[ncolor].blue = (unsigned short) (b / strip) * strip;
  if (!XAllocColor (disp, colormap, &colors[ncolor]))
    {
      register i;
      register long md = 65535 * 4;
      for (i = 0; i < ncolor; i++)
	if ((abs (colors[i].red - r) + abs (colors[i].green - g) + abs (colors[i].blue - b)) < md)
	  ncol1 = md, md = abs (colors[i].red - r) + abs (colors[i].green - g) + abs (colors[i].blue - b);
    }
  else
    {
      register i;
      for (i = 0; i < ncolor; i++)
	if (colors[ncolor].pixel == colors[i].pixel)
	  ncolor--, i = ncolor;
      ncolor++;
    }
  return (colors[ncol1].pixel);
}
nastavprisery ()
{
  register int i;
  for (i = 0; i < maxprisery; i++)
    {
      ps[i][0] = random () % VELX, ps[i][1] = random () % VELY,
	pp[i][0] = (random () % 2) * 2 - 1, pp[i][1] = (random () % 2) * 2 - 1;
    }
}
napis (text)
     char *text;
{
  XSetForeground (dp, gc, back);
  XFillRectangle (dp, wi, gc, 0, (int) VELY * Ykrok, (int) VELX * Xkrok, SIRKA);
  XSetForeground (dp, gc, Textc);
  XDrawString (dp, wi, gc, 0, (int) VELY * Ykrok + SIRKA, text, strlen (text));
}
displaytext ()
{
  char i[255];
  sprintf (i, "Lives%3i   Percent%3i   Limit%3i   Level%3i   Score%6i",
	   live, zabrano / (VELX * VELY / 100), limit / (VELX * VELY / 100), level, score);
  napis (i);
}
draw (x, y, mode)
     int x, y;
{
  int x1, y1, c1, c2, x2, y2, st;
  XPoint points[4];
  struct zabrano kresli;
  int ddd = 0;
  if (((mode == NORMAL && lpole[x][y] != pole[x][y]) || mode == PREKRESLI || pole[x][y] == ZABRANO))
    switch (pole[x][y])
      {
      case 0:
	XSetForeground (dp, gc, back);
	XFillRectangle (dp, wi, gc, (int) x * Xkrok, (int) y * Ykrok, Xkrok, (int) Ykrok);
	break;
      case ZABRANO:
	x1 = x * Xkrok, y1 = y * Ykrok, x2 = x * Xkrok + Xkrok, y2 = y * Ykrok + Ykrok;
	if (JePole (x + 1, y) != ZABRANO)
	  {
	    x2 -= SIRKAS;
	    if (!lpol[x][y].u)
	      kresli.u = 1;
	    else
	      kresli.u = 0;
	    lpol[x][y].u = 1;
	  }
	else
	  {
	    if (lpol[x][y].u)
	      kresli.u = 1;
	    else
	      kresli.u = 0;
	    lpol[x][y].u = 0;
	  }

	if (JePole (x - 1, y) != ZABRANO)
	  {
	    x1 += SIRKAS;
	    if (!lpol[x][y].d)
	      kresli.d = 1;
	    else
	      kresli.d = 0;
	    lpol[x][y].d = 1;
	  }
	else
	  {
	    if (lpol[x][y].d)
	      kresli.d = 1;
	    else
	      kresli.d = 0;
	    lpol[x][y].d = 0;
	  }

	if (JePole (x, y + 1) != ZABRANO)
	  {
	    y2 -= SIRKAS;
	    if (!lpol[x][y].r)
	      kresli.r = 1;
	    else
	      kresli.r = 0;
	    lpol[x][y].r = 1;
	  }
	else
	  {
	    if (lpol[x][y].r)
	      kresli.r = 1;
	    else
	      kresli.r = 0;
	    lpol[x][y].r = 0;
	  }

	if (JePole (x, y - 1) != ZABRANO)
	  {
	    y1 += SIRKAS;
	    if (!lpol[x][y].l)
	      kresli.l = 1;
	    else
	      kresli.l = 0;
	    lpol[x][y].l = 1;
	  }
	else
	  {
	    if (lpol[x][y].l)
	      kresli.l = 1;
	    else
	      kresli.l = 0;
	    lpol[x][y].l = 0;
	  }

	if (mode == PREKRESLI || lpole[x][y] != ZABRANO)
	  kresli.u = 1, kresli.d = 1,
	    kresli.l = 1, kresli.r = 1;
	else
	  {
	    if (lpol[x][y].c1)
	      {
		if (x2 != x * Xkrok + Xkrok || y2 != y * Ykrok + Ykrok)
		  goto neprezkoc;
		else if (JePole (x + 1, y + 1) == ZABRANO)
		  goto neprezkoc;
	      }
	    if (lpol[x][y].c2)
	      {
		if (x2 != x * Xkrok + Xkrok || y1 != y * Ykrok)
		  goto neprezkoc;
		else if (JePole (x + 1, y - 1) == ZABRANO)
		  goto neprezkoc;
	      }
	    if (lpol[x][y].c3)
	      {
		if (x1 != x * Xkrok || y1 != y * Ykrok)
		  goto neprezkoc;
		else if (JePole (x - 1, y - 1) == ZABRANO)
		  goto neprezkoc;
	      }
	    if (lpol[x][y].c4)
	      {
		if (x1 != x * Xkrok || y2 != y * Ykrok + Ykrok)
		  goto neprezkoc;
		else if (JePole (x - 1, y + 1) == ZABRANO)
		  goto neprezkoc;
	      }
	    if (!kresli.u && !kresli.l && !kresli.r && !kresli.d)
	      goto preskoc;
	  neprezkoc:;
	  }
	if (usegif)
	  XPutImage (dp, wi, gc, obrazek, x1, y1, (int) x1, (int) y1, x2 - x1, y2 - y1);
	else
	  {
	    XSetForeground (dp, gc, Vyplneno);
	    XFillRectangle (dp, wi, gc, x1, y1, x2 - x1, y2 - y1);
	  }
	ddd = 1;
	if (!kresli.u && !kresli.l && !kresli.r && !kresli.d)
	  goto preskoc;
	st = 0;
	if (JePole (x - 1, y) != ZABRANO /*&&(kresli.u||kresli.l||kresli.r */ )
	  {
	    XSetForeground (dp, gc, stin);
	    st = 1;
	    points[0].x = x * Xkrok;
	    points[0].y = y * Ykrok;
	    points[1].x = x1;
	    points[1].y = y1;
	    points[2].x = x1, points[2].y = y2, points[3].x = x * Xkrok,
	      points[3].y = y * Ykrok + Ykrok;
	    XFillPolygon (dp, wi, gc, points, 4, Convex, CoordModeOrigin);
	  }
	if (JePole (x, y - 1) != ZABRANO /*&&(kresli.d||kresli.l||kresli.r */ )
	  {
	    if (!st)
	      XSetForeground (dp, gc, stin);
	    points[0].x = x * Xkrok;
	    points[0].y = y * Ykrok;
	    points[1].x = x1;
	    points[1].y = y1;
	    points[2].x = x2, points[2].y = y1,
	      points[3].x = x * Xkrok + Xkrok,
	      points[3].y = y * Ykrok;
	    XFillPolygon (dp, wi, gc, points, 4, Convex, CoordModeOrigin);
	  }
	st = 0;
	if (JePole (x + 1, y) != ZABRANO /*&&(kresli.r||kresli.u||kresli.d */ )
	  {
	    XSetForeground (dp, gc, stin1);
	    st = 1;
	    points[0].x = x * Xkrok + Xkrok;
	    points[0].y = y * Ykrok;
	    points[1].x = x2;
	    points[1].y = y1;
	    points[2].x = x2, points[2].y = y2,
	      points[3].x = x * Xkrok + Xkrok,
	      points[3].y = y * Ykrok + Ykrok;
	    XFillPolygon (dp, wi, gc, points, 4, Convex, CoordModeOrigin);
	  }
	if (JePole (x, y + 1) != ZABRANO /*&&(kresli.l||kresli.u||kresli.d) */ )
	  {
	    if (!st)
	      XSetForeground (dp, gc, stin1);
	    st = 1;
	    points[0].x = x * Xkrok;
	    points[0].y = y * Ykrok + Ykrok;
	    points[1].x = x1;
	    points[1].y = y2;
	    points[2].x = x2, points[2].y = y2,
	      points[3].x = x * Xkrok + Xkrok,
	      points[3].y = y * Ykrok + Ykrok;
	    XFillPolygon (dp, wi, gc, points, 4, Convex, CoordModeOrigin);
	  }
      preskoc:;
	kresli = lpol[x][y];
	lpol[x][y].c1 = 0;
	lpol[x][y].c2 = 0;
	lpol[x][y].c3 = 0;
	lpol[x][y].c4 = 0;
	if (x2 == x * Xkrok + Xkrok && y2 == y * Ykrok + Ykrok)
	  if (JePole (x + 1, y + 1) != ZABRANO)
	    {
	      lpol[x][y].c1 = 1;
	      if ((!kresli.c1 || mode == PREKRESLI || ddd))
		{
		  if (!st)
		    XSetForeground (dp, gc, stin1);
		  st = 1;
		  XFillRectangle (dp, wi, gc, x2 - SIRKAS, y2 - SIRKAS, SIRKAS, SIRKAS);
		}
	    }
	if (x2 == x * Xkrok + Xkrok && y1 == y * Ykrok)
	  if (JePole (x + 1, y - 1) != ZABRANO)
	    {
	      lpol[x][y].c2 = 1;
	      if ((!kresli.c2 || mode == PREKRESLI || ddd))
		{
		  if (!st)
		    XSetForeground (dp, gc, stin1);
		  points[0].x = x2, points[0].y = y1, points[1].x = x2 - SIRKAS,
		    points[1].y = y1 + SIRKAS, points[2].x = x2 - SIRKAS, points[2].y = y1;
		  XFillPolygon (dp, wi, gc, points, 3, Convex, CoordModeOrigin);
		  XSetForeground (dp, gc, stin), st = 1;
		  points[2].x = x2, points[2].y = y1 + SIRKAS;
		  XFillPolygon (dp, wi, gc, points, 3, Convex, CoordModeOrigin);
		}
	    }
	  else
	    st = 0;
	if (x1 == x * Xkrok && y1 == y * Ykrok)
	  if (JePole (x - 1, y - 1) != ZABRANO)
	    {
	      lpol[x][y].c3 = 1;	/*if(!st) */
	      if ((!kresli.c3 || mode == PREKRESLI || ddd))
		{
		  XSetForeground (dp, gc, stin), st = 1;
		  XFillRectangle (dp, wi, gc, x1, y1, SIRKAS, SIRKAS);
		}
	    }
	if (x1 == x * Xkrok && y2 == y * Ykrok + Ykrok)
	  if (JePole (x - 1, y + 1) != ZABRANO)
	    {
	      lpol[x][y].c4 = 1;
	      if ((!kresli.c4 || mode == PREKRESLI || ddd))
		{
		  points[0].x = x1, points[0].y = y2, points[1].x = x1 + SIRKAS;
		  if (!st)
		    XSetForeground (dp, gc, stin), st = 1;
		  points[1].y = y2 - SIRKAS, points[2].x = x1 + SIRKAS, points[2].y = y2;
		  XFillPolygon (dp, wi, gc, points, 3, Convex, CoordModeOrigin);
		  XSetForeground (dp, gc, stin1);
		  points[2].x = x1, points[2].y = y2 - SIRKAS;
		  XFillPolygon (dp, wi, gc, points, 3, Convex, CoordModeOrigin);
		}
	    }
	if (lpole[x][y] != ZABRANO)
	  {
	    lpole[x][y] = ZABRANO;
	    for (x1 = x - 1; x1 <= x + 1; x1++)
	      for (y1 = y - 1; y1 <= y + 1; y1++)
		if (y1 >= 0 && y1 < VELY && x1 >= 0 && x1 < VELX && x1 >= 0)
		  if (x1 != x || y1 != y)
		    if (pole[x1][y1] == ZABRANO)
		      {
			draw (x1, y1, DODRAW);
		      }
	  }
	break;
      case PANACEK:
	XSetForeground (dp, gc, Panacek);
	XFillArc (dp, wi, gc, (int) x * Xkrok, (int) y * Ykrok, (int) Xkrok - 2, (int) Ykrok - 2
		  ,0, 360 * 64);
	XSetForeground (dp, gc, stin);
	XDrawArc (dp, wi, gc, (int) x * Xkrok, (int) y * Ykrok, (int) Xkrok - 2, (int) Ykrok - 2
		  ,0, 360 * 64);
	break;
      case PRISERA:
	XPutImage (dp, wi, gc, koule1, 0, 0, (int) x * Xkrok, (int) y * Ykrok, Xkrok, Ykrok);
	break;
      default:
	XSetForeground (dp, gc, Textc);
	c1 = (int) pole[x][y] % 8;
	c2 = (int) pole[x][y] / 8;
	switch (c1)
	  {
	  case 0:
	    x1 = (int) x *Xkrok + (int) Xkrok / 2;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  case 1:
	    x1 = (int) x *Xkrok + (int) Xkrok / 2;
	    y1 = y * Ykrok + Ykrok;
	    break;
	  case 2:
	    x1 = x * Xkrok;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  case 3:
	    x1 = x * Xkrok + Xkrok / 2;
	    y1 = y * Ykrok;
	    break;
	  case 4:
	    x1 = x * Xkrok + Xkrok;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  default:
	    goto ko;
	  }
	XDrawLine (dp, wi, gc, x1, y1, (int) x * Xkrok + (int) Xkrok / 2, (int) y * Ykrok + (int) Ykrok / 2);
	switch (c2)
	  {
	  case 0:
	    x1 = x * Xkrok + Xkrok / 2;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  case 3:
	    x1 = x * Xkrok + Xkrok / 2;
	    y1 = y * Ykrok + Ykrok;
	    break;
	  case 4:
	    x1 = x * Xkrok;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  case 1:
	    x1 = x * Xkrok + Xkrok / 2;
	    y1 = y * Ykrok;
	    break;
	  case 2:
	    x1 = x * Xkrok + Xkrok;
	    y1 = y * Ykrok + Ykrok / 2;
	    break;
	  default:
	    goto ko;
	  }
	XDrawLine (dp, wi, gc, x1, y1, (int) x * Xkrok + (int) Xkrok / 2, (int) y * Ykrok + (int) Ykrok / 2);
      }
ko:;
  lpole[x][y] = pole[x][y];
}

int vybarvovat, pocetvyb;
vypln (x, y)
     int x, y;
{
  if (x >= 0 && x < VELX && y >= 0 && y < VELY)
    {
      if (pole[x][y] == PRISERA)
	vybarvovat = 0;
      if (pole[x][y] == 0 && !pole1[x][y])
	{
	  pole1[x][y] = pocetvyb;
	  vypln (x + 1, y);
	  vypln (x - 1, y);
	  vypln (x, y + 1);
	  vypln (x, y - 1);
	}
    }
}
kresli ()
{
  register int x, y, x1, y1, vy = 0;
  for (x = 0; x < VELX; x++)
    for (y = 0; y < VELY; y++)
      pole1[x][y] = 0;
  vybarvovat = 1;
  pocetvyb = 1;
  for (x = 0; x < VELX; x++)
    for (y = 0; y < VELY; y++)
      {
	if (!pole[x][y] && !pole1[x][y])
	  {
	    vybarvovat = 1;
	    vypln (x, y);
	    if (vybarvovat)
	      {
		for (x1 = 0; x1 < VELX; x1++)
		  for (y1 = 0; y1 < VELY; y1++)
		    if (pole1[x1][y1] == pocetvyb)
		      pole[x1][y1] = ZABRANO,
			vy = 1;
	      }
	    pocetvyb++;
	  }
      }
  zabrano = 0;
  for (x = 0; x < VELX; x++)
    for (y = 0; y < VELY; y++)
      {
	if (pole[x][y] < MAXNECARA && pole[x][y])
	  pole[x][y] = ZABRANO, vy = 1;
	if (pole[x][y] == ZABRANO)
	  zabrano++;
      }
  if (vy)
    for (x = 0; x < VELX; x++)
      for (y = 0; y < VELY; y++)
	if (pole[x][y] != lpole[x][y])
	  draw (x, y, NORMAL);
  if (vy)
    displaytext (), pocvybarv++;
  if (zabrano > limit)
    vyhral = 1;
}
konec1 ()
{
  int x1, y1;
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      if (pole[x1][y1] < MAXNECARA || pole[x1][y1] == PANACEK)
	pole[x1][y1] = 0, draw (x1, y1, NORMAL);
  live--;
  pole2 = pole[xs][ys];
  x = xs, y = ys;
  pole[xs][ys] = PANACEK;
  draw (xs, ys, NORMAL);
  pole[xs][ys] = pole2;
  displaytext ();
  if (live < 0)
    GameOver ();
}
drawgame ()
{
  char s[255];
  XSetForeground (dp, gc, Textc);
  sprintf (s, "Total score:%3i", score);
  XDrawString (dp, wi, gc, (int) VELX * Xkrok / 2 - XTextWidth (fs, s, strlen (s)) / 2,
	       VELY * Ykrok / 2 + SIRKA * 2, s, strlen (s));
  XFlush (dp);
}

drawdalsi ()
{
  char s[255];
  XSetForeground (dp, gc, Textc);
  sprintf (s, "Up to limit:%3i score:%3i", (zabrano - limit) * 100 / VELX / VELY, (
						      zabrano - limit) * 2);
  XDrawString (dp, wi, gc, (int) VELX * Xkrok / 2 - XTextWidth (fs, s, strlen (s)) / 2,
	       VELY * Ykrok / 2 - SIRKA * 2, s, strlen (s));
  sprintf (s, "      limit:%3i score:%3i", (limit) * 100 / VELX / VELY, (
							limit) / pocvybarv);
  XDrawString (dp, wi, gc, (int) VELX * Xkrok / 2 - XTextWidth (fs, s, strlen (s)) / 2,
	       VELY * Ykrok / 2, s, strlen (s));
  sprintf (s, "Total score:%3i   now:%3i", score, (score + limit / pocvybarv + zabrano -
						   limit));
  XDrawString (dp, wi, gc, (int) VELX * Xkrok / 2 - XTextWidth (fs, s, strlen (s)) / 2,
	       VELY * Ykrok / 2 + SIRKA * 2, s, strlen (s));
  XFlush (dp);
}
dalsi ()
{
  int x1, y1;
  napis ("Next Level");
  stav = DALSI;
  drawdalsi ();
  XFlush (dp);
  /*XBell (dp, 100);*/
  pocprisery++;
  XFlush (dp);
  klavesa = 0;
  if (usegif)
    {
      stav = UKAZUJEM;
      XPutImage (dp, wi, gc, obrazek, 0, 0, 0, 0, Xkrok * VELX, Ykrok * VELY);
      napis ("Press Any Key");
      pokus (FREE);
      pokus (NORMAL);
    }
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      pole[x1][y1] = 0, draw (x1, y1, PREKRESLI);
  napis ("Loading next level");
  stav = DALSI;
  drawdalsi ();
  if (usegif)
    nactidalsi (),
      nahraj (file);
  napis ("Press any key");
  pokus (FREE);
  napis ("Press any key");
  pokus (NORMAL);
  score += limit / pocvybarv + zabrano - limit;
  live++;
  limit += VELX / 3;
  if (limit > VELX * VELY - 3 * VELX)
    limit = 75 * VELX * VELY / 100, CASTO /= 2, CASTO1 /= 2;
  stav = HRAJEME;
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      pole[x1][y1] = 0, draw (x1, y1, PREKRESLI);
  x = 0, y = 0, pole2 = 0, xs = 0, ys = 0, minule = 0, pocvybarv = 0, smer = 0, smer1 = 1, stav = HRAJEME;
  level++;
  nastavprisery ();
  displaytext ();
}
GameOver ()
{
  int x1, y1;
  napis ("Game Over");
  drawgame ();
  score = 0;
  live = 5;
  limit = 75 * VELX * VELY / 100;
  level = 1;
  XFlush (dp);
  klavesa = 0;
  pocprisery = 2;
  XFlush (dp);
  klavesa = 0;
  stav = KONCIME;
  pokus (FREE);
  pokus (NORMAL);
  nakresli_demo ();
  nastavprisery ();
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      pole[x1][y1] = 0, draw (x1, y1, PREKRESLI);
  x = 0, y = 0, pole2 = 0, xs = 0, ys = 0, minule = 0, pocvybarv = 0, smer = 0, smer1 = 1, stav = HRAJEME;
  displaytext ();
  pole2 = 0;
}

pohyb ()
{
  int x1, y1, vykresli = 0, i;
  struct itimerval rttimer, oldrttimer;
  signal (SIGALRM, pohyb);
  if (bezi == 0)
    {
      bezi = 1;
    restart:;
      if (prekresli)
	{
	  for (x1 = 0; x1 < VELX; x1++)
	    for (y1 = 0; y1 < VELY; y1++)
	      draw (x1, y1, NORMAL);
	  prekresli = 0;
	}
      for (i = 0; i < pocprisery; i++)
	{
	  x1 = ps[i][0] + pp[i][0];
	  y1 = ps[i][1] + pp[i][1];
	  pole[ps[i][0]][ps[i][1]] = 0;
	  if (x1 >= 0 && y1 >= 0 && x1 < VELX && y1 < VELY && pole2 != ZABRANO)
	    {
	      int poc1 = pocprisery;
	      if (pole[x1][y1] == PANACEK)
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	      if (pole[ps[i][0]][y1] == PANACEK)
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	      if (pole[x1][ps[i][1]] == PANACEK)
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	      if (pole[x1][y1] < MAXNECARA && pole[x1][y1])
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	      if (pole[ps[i][0]][y1] < MAXNECARA && pole[ps[i][0]][y1])
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	      if (pole[x1][ps[i][1]] < MAXNECARA && pole[x1][ps[i][1]])
		{
		  konec1 ();
		  if (poc1 != pocprisery)
		    goto restart;
		}
	    }
	  if (y1 < 0 || y1 >= VELY)
	    pp[i][1] *= -1, y1 = ps[i][1], x1 = ps[i][0];
	  else if (pole[ps[i][0]][y1])
	    pp[i][1] *= -1, y1 = ps[i][1], x1 = ps[i][0];
	  if (x1 < 0 || x1 >= VELX)
	    pp[i][0] *= -1, y1 = ps[i][1], x1 = ps[i][0];
	  else if (pole[x1][ps[i][1]])
	    pp[i][0] *= -1, y1 = ps[i][1], x1 = ps[i][0];
	  if (pole[x1][y1])
	    pp[i][0] *= -1, y1 = ps[i][1], pp[i][1] *= -1, x1 = ps[i][0];
	  pole[ps[i][0]][ps[i][1]] = 0;
	  pole[x1][y1] = PRISERA;
	  draw (ps[i][0], ps[i][1], NORMAL);
	  ps[i][0] = x1;
	  ps[i][1] = y1;
	  draw (x1, y1, NORMAL);
	}
      if (smer)
	{

	  x1 = x;
	  y1 = y;
	  pole[x1][y1] = pole2;
	  draw (x1, y1, NORMAL);
	  if (!pole[x1][y])
	    pole[x1][y1] = smer1 + smer * 8;
	  smer1 = smer;
	  if (pole[x][y] == ZABRANO)
	    xs = x, ys = y;
	  switch (smer)
	    {
	    case 1:
	      y--;
	      if (y < 0)
		y = 0, vykresli = 1;
	      break;
	    case 2:
	      x++;
	      if (x >= VELX)
		x = VELX - 1, vykresli = 1;
	      break;
	    case 3:
	      y++;
	      if (y >= VELY)
		y = VELY - 1, vykresli = 1;
	      break;
	    case 4:
	      x--;
	      if (x < 0)
		x = 0, vykresli = 1;
	      break;
	    }
	  if (pole[x][y] == ZABRANO)
	    vykresli = 1, xs = x, ys = y;
	  if (pole[x][y] == PRISERA)
	    konec1 ();
	  if (pole[x][y] < MAXNECARA && pole[x][y] && (x1 != x || y1 != y))
	    konec1 ();
	  if (x1 != x || y1 != y)
	    pole2 = pole[x][y];
	  pole[x][y] = PANACEK;
	  draw (x1, y1, NORMAL);
	  draw (x, y, NORMAL);
	  if (x != x1 || y != y1)
	    minule = 0;
	  if (vykresli && pole[x1][y1] != ZABRANO && !minule)
	    {
	      pole[x][y] = pole2 == ZABRANO ? ZABRANO : 1
		;
	      kresli ();
	      pole2 = pole[x][y];
	      pole[x][y] = PANACEK, draw (x, y, NORMAL);
	      minule = 1;
	    }
	  XFlush (dp);
	}
      if (vyhral)
	dalsi (), vyhral = 0;
      XFlush (dp);
      bezi = 0;
      XFlush (dp);
    }
  rttimer.it_value.tv_sec = CASTO1;
  rttimer.it_value.tv_usec = CASTO;
  rttimer.it_interval.tv_sec = 0;
  rttimer.it_interval.tv_usec = 0;
  setitimer (ITIMER_REAL, &rttimer, &oldrttimer);
}
int pocstisknutych = 0;
int klavreag = 0;

Bool 
pokus (mode)
{
  int x, y, xs, ys, xk, yk;
  for (;;)
    {
      if (mode == FREE)
	if (!XEventsQueued (dp, QueuedAfterFlush))
	  return (True);
      XNextEvent (dp, &myEvent);
      switch (myEvent.type)
	{
	case KeyRelease:
	  if (klavreag == myEvent.xkey.keycode)
	    smer = 0;
	  pocstisknutych--;
	  klavesa++;
	  break;
	case KeyPress:
	  pocstisknutych++;
	  if (stav != HRAJEME && mode == NORMAL /*&&smer==0&&klavesa/*&&pocstisknutych==1 */ )
	    {
	      return (True);
	    }
	  /* klavesa=1; */
	  switch (XLookupKeysym((XKeyEvent *)&myEvent,((XKeyEvent *)&myEvent)->state))
	    {
	    case XK_Up:
	      smer = 1, klavreag = myEvent.xkey.keycode;
	      break;
	    case XK_Right:
	      smer = 2;
	      klavreag = myEvent.xkey.keycode;
	      break;
	    case XK_Down:
	      smer = 3;
	      klavreag = myEvent.xkey.keycode;
	      break;
	    case XK_Left:
	      smer = 4;
	      klavreag = myEvent.xkey.keycode;
	      break;
	    }
	  break;
	default:
	  if (usegif && stav == UKAZUJEM)
	    {
	      int Xkrok1 = Xkrok, Ykrok1 = Ykrok;
	      if (XGetWindowAttributes (dp, wi, &xwa) == 0)
		goto ko1;
	      Xkrok = xwa.width / VELX;
	      Ykrok = (xwa.height - SIRKA) / VELY;
	      if (Xkrok < 2)
		Xkrok = 2;
	      if (Ykrok < 2)
		Ykrok = 2;
	      if (Xkrok1 != Xkrok || Ykrok1 != Ykrok)
		{
		  XFlush (dp);
								/*XDestroyImage(koule1); */ free (data1);
								/*generate momery fault why? */
		  koule ();
		  resizni ();
		}
	      XPutImage (dp, wi, gc, obrazek, myEvent.xexpose.x, myEvent.xexpose.y,
			 myEvent.xexpose.x, myEvent.xexpose.y,
			 myEvent.xexpose.width, myEvent.xexpose.height);
	      napis ("Press any key");
	      goto ko;
	    }
	  {
	    int Xkrok1 = Xkrok, Ykrok1 = Ykrok;
	    bezi = 1;
	    if (XGetWindowAttributes (dp, wi, &xwa) == 0)
	      goto ko1;
	    Xkrok = xwa.width / VELX;
	    Ykrok = (xwa.height - SIRKA) / VELY;
	    if (Xkrok < 2)
	      Xkrok = 2;
	    if (Xkrok < 5)
	      Ykrok = 5;
	    if (Ykrok < 5)
	      Ykrok = 5;
	    if (Xkrok1 != Xkrok || Ykrok1 != Ykrok)
	      {
		XFlush (dp);
		XSetForeground (dp, gc, back);
		XFillRectangle (dp, wi, gc, 0, 0, xwa.width, xwa.height);
		XDestroyImage (koule1);		/*free(data1); */
		koule ();
		resizni ();
		xs = 0, ys = 0, xk = VELX, yk = VELY;
	      }
	    else
	      {
		xs = (myEvent.xexpose.x) / Xkrok;
		ys = (myEvent.xexpose.y) / Ykrok;
		xk = (myEvent.xexpose.x + myEvent.xexpose.width) / Xkrok + 1;
		yk = (myEvent.xexpose.y + myEvent.xexpose.height) / Ykrok + 1;
		if (xs > VELX)
		  xs = VELX;
		if (ys > VELY)
		  ys = VELY;
		if (xk > VELX)
		  xk = VELX;
		if (yk > VELY)
		  yk = VELY;
	      }
	    for (x = xs; x < xk; x++)
	      for (y = ys; y < yk; y++)
		{
		  if (pole[x][y] != 0)
		    draw (x, y, PREKRESLI);
		  if (pole[x][y] == PANACEK)
		    pole[x][y] = pole2, draw (x, y, PREKRESLI), pole[x][y] = PANACEK,
		      draw (x, y, NORMAL);
		}
	    displaytext ();
	    XFlush (dp);
	  ko:;
	    if (stav == DALSI)
	      drawdalsi ();
	    if (stav == KONCIME)
	      drawgame ();
	    bezi = 0;
	  }
	}
    ko1:;
    }
}
#define MEZERA 2
#define dopole(x,y,z) ((x>0)&&(x<VELX)&&(y>0)&&(y<VELY)?(pole[x][y]=z):0)
nakresli_demo ()
{
  int x, y, x1, y1;
  x = (VELX - ROZMERX) / 2;
  y = (VELY - ROZMERY) / 2;
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      pole[x1][y1] = 0;
  for (x1 = x - MEZERA; x1 < x + ROZMERX + MEZERA; x1++)
    {
      dopole (x1, (y - MEZERA), ZABRANO);
      dopole (x1, (y + ROZMERY + MEZERA), ZABRANO);
    }
  for (y1 = y - MEZERA; y1 < y + ROZMERY + MEZERA + 1; y1++)
    {
      dopole ((x - MEZERA), y1, ZABRANO);
      dopole ((x + ROZMERX + MEZERA), y1, ZABRANO);
    }
  for (x1 = 0; x1 < ROZMERX; x1++)
    for (y1 = 0; y1 < ROZMERY; y1++)
      switch (logo[y1][x1])
	{
	case 1:
	  dopole ((x1 + x), (y1 + y), PRISERA);
	  break;
	case 2:
	  dopole ((x1 + x), (y1 + y), 0);
	  if (x1 + x >= 0 && x1 + x < VELX && y1 + y >= 0 && y1 + y < VELY)
	    draw (x1 + x, y1 + y, PREKRESLI);
	  dopole ((x1 + x), (y1 + y), PANACEK);
	  break;
	}
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      draw (x1, y1, PREKRESLI);
  XFlush (dp);
  stav = DEMO;
  napis ("Copyright (C) Jan Hubicka 1993");
  klavesa = 0;
  pokus (FREE);
  pokus (NORMAL);
  klavesa = 0;
  for (x1 = 0; x1 < VELX; x1++)
    for (y1 = 0; y1 < VELY; y1++)
      pole[x1][y1] = 0, draw (x1, y1, NORMAL);
  stav = HRAJEME;
}
/*#include "color.c" */
struct itimerval rttimer, oldrttimer;
main (argc, argv)
     int argc;
     char **argv;
{
  XWMHints *XS;
  XSizeHints SH;
  Font f;
  XrmDatabase Database = NULL;
  static XrmOptionDescRec opTable[] =
  {
    {"-background", ".background", XrmoptionSepArg, (caddr_t) NULL},
    {"-bg", ".background", XrmoptionSepArg, (caddr_t) NULL},
    {"-fg", ".foreground", XrmoptionSepArg, (caddr_t) NULL},
    {"-foreground", ".foreground", XrmoptionSepArg, (caddr_t) NULL},
    {"-display", ".display", XrmoptionSepArg, (caddr_t) NULL},
    {"-leftshadow", ".topleftshade", XrmoptionSepArg, (caddr_t) NULL},
    {"-rightshadow", ".bottomrightshade", XrmoptionSepArg, (caddr_t) NULL},
    {"-text", ".text", XrmoptionSepArg, (caddr_t) NULL},
    {"-man", ".man", XrmoptionSepArg, (caddr_t) NULL},
    {"-ball", ".ball", XrmoptionSepArg, (caddr_t) NULL},
    {"-gifdir", ".gifdir", XrmoptionSepArg, (caddr_t) NULL},
    {"-usegif", ".usegif", XrmoptionNoArg, (caddr_t) "on"},
    {"-nousegif", ".usegif", XrmoptionNoArg, (caddr_t) "off"},
  };
  char *type;

  XrmInitialize ();
  if ((Database = XrmGetFileDatabase ("/usr/lib/X11/app-defaults/Xonix")) == NULL)
    {
      printf ("       X O N I X\n");
      printf ("JAHUSOFT shareware game\n"
	      "copyright(c) 1993 Jan Hubicka(JAHUSOFT)\n\n"
	      "Database file not found.\n"
	      "Please create database file \"Xonix\" \n");
      exit (1);
    }
  XrmParseCommand (&Database, opTable, 13, "xonix", &argc, argv);
  if (argc != 1)
    {
      printf ("       X O N I X\n"
	      "JAHUSOFT shareware game\n"
	      "copyright(c) 1993 Jan Hubicka(JAHUSOFT)\n\n"
	      "Usage:%s [-bg <color>] [--background <color>]\n"
	      "     [-fg <color] [-foreground <color>]\n"
	      "     [-help] [-display <display>] [-font <font>]\n"
	      "     [-man <color>] [-text <color>] [-ball <color>]\n"
	      "     [-usegif] [-nousegif] [-gifdir <directory>]\n"
	      "copyright(c) 1993 Jan Hubicka(JAHUSOFT)\n\n", argv[0]);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.background", "Xonix.Background",
		       &type, &backcolor))
    {
      printf ("No background color defined\n"
	      "define please *Background in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.foreground", "Xonix.Foreground",
		       &type, &forecolor))
    {
      printf ("No foreground color defined\n"
	      "define please *Foreground in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.man", "Xonix.Man",
		       &type, &Man))
    {
      printf ("No man color defined\n"
	      "define please *Man in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.topleftshade", "Xonix.TopLeftShade",
		       &type, &leftshadow))
    {
      printf ("No topleftshade color defined\n"
	      "define please *TopLeftShade in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.bottomrightshade", "Xonix.BottomRightShade",
		       &type, &rightshadow))
    {
      printf ("No bottomrightshade color defined\n"
	      "define please *BottomRightShade in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.text", "Xonix.Text",
		       &type, &Text))
    {
      printf ("No text color defined\n"
	      "define please *Text in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.usegif", "Xonix.UseGIF",
		       &type, &UseGif))
    usegif = 0;
  else
    {
      if (strncmp (UseGif.addr, "on", 2))
	usegif = 0;
      else
	{
	  usegif = 1;
	  if (!XrmGetResource (Database, "xonix.gifdir", "Xonix.GifDir",
			       &type, &GifDIR))
	    {
	      printf ("No \n"
		      "define please *GifDir in Xonix database file\n"
		);
	      exit (1);
	    }
	}
    }
  if (!XrmGetResource (Database, "xonix.font", "Xonix.Font",
		       &type, &font1))
    {
      printf ("No font defined\n"
	      "define please *Font in Xonix database file\n"
	);
      exit (1);
    }
  if (!XrmGetResource (Database, "xonix.ball", "Xonix.Ball",
		       &type, &Priserac))
    {
      printf ("No ball color defined\n"
	      "define please *Ball in Xonix database file\n"
	);
      exit (1);
    }

  if (!XrmGetResource (Database, "xonix.display", "Xonix.Display",
		       &type, &display))
    {
      if ((dp = XOpenDisplay (0)) == 0)
	{
	  printf ("could not open display");
	  exit ();
	}
    }
  else if ((dp = XOpenDisplay (display.addr)) == 0)
    {
      printf ("could not open display");
      exit ();
    }
  if ((f = XLoadFont (dp, font1.addr)) == 0)
    {
      printf ("Font %s does not exist", font1);
      /*  napisy=9999; */
    }
  fs = XQueryFont (dp, f);
  SIRKA = fs->max_bounds.ascent + fs->max_bounds.descent;
  screen = DefaultScreen (dp);
  nastavprisery ();
  border = BlackPixel (dp, screen);
/* back=WhitePixel(dp,screen); */
  back = ColorNameToPixel (dp, backcolor.addr);
  wi = XCreateSimpleWindow (dp, RootWindow (dp, screen), 50, 50, (int) VELX * Xkrok,
			    (int) VELY * Ykrok + SIRKA, 3, border, back);
  XS = XGetWMHints (dp, wi);
  SH.flags |= PResizeInc | PMinSize;
  SH.width_inc = VELX;
  SH.height_inc = VELY;
  SH.min_width = VELX * 5;
  SH.min_height = VELY * 5 + SIRKA;
  XSetStandardProperties (dp, wi, "JAHUSOFT XONIX", "Xonix",
			  XCreateBitmapFromData (dp, wi, xonix_bits, xonix_width, xonix_height), argv, argc, XS);
  XSetNormalHints (dp, wi, &SH);
  XMapWindow (dp, wi);
  XFlush (dp);
  gc = XCreateGC (dp, wi, 0, NULL);
  /*Prisera=ColorNameToPixel(dp,"Black"); */
  Panacek = ColorNameToPixel (dp, Man.addr);
  Vyplneno = ColorNameToPixel (dp, forecolor.addr);
  stin = ColorNameToPixel (dp, leftshadow.addr);
  stin1 = ColorNameToPixel (dp, rightshadow.addr);
  Textc = ColorNameToPixel (dp, Text.addr);
  XSetFont (dp, gc, f);
  signal (SIGALRM, pohyb);
  colors = (XColor *) malloc (DisplayCells (dp, screen) * sizeof (XColor));
  koule ();
  srand ((int) time (NULL));
  if (usegif)
    {
      zjistipocet ();
      nactidalsi ();
      nahraj (file);
    }
  rttimer.it_value.tv_sec = CASTO1;
  rttimer.it_value.tv_usec = CASTO;
  rttimer.it_interval.tv_sec = 0;
  rttimer.it_interval.tv_usec = 0;
  XSelectInput (dp, wi, ExposureMask | KeyPress | KeyRelease | ConfigureRequest);
  nakresli_demo ();
  setitimer (ITIMER_REAL, &rttimer, &oldrttimer);
  while (1)
    pokus (NORMAL);
}
