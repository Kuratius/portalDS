#include "common/general.h"

#include <nds/arm9/image.h>
#include <nds/arm9/pcx.h>
#include <nds/arm9/video.h>

#define FONT_T1 NORMAL_PACK(-32,-32,0)
#define FONT_T2 NORMAL_PACK(-32,32,0)
#define FONT_T3 NORMAL_PACK(32,32,0)
#define FONT_T4 NORMAL_PACK(32,-32,0)

static font_struct hudFont;
static font_struct* currentFont;

void initText(void)
{
	loadFont(&hudFont,16,8);
}

void setFont(font_struct* f)
{
	currentFont=f;
}



bool myloadPCX(const unsigned char *pcx, sImage *image)
{
    // struct rgb {
    //     unsigned char b,g,r;
    // };

    const RGB_24 *pal;

    const PCXHeader *hdr = (const PCXHeader *)pcx;

    pcx += sizeof(PCXHeader);

    int scansize = hdr->bytesPerLine;

    int width = image->width = hdr->xmax - hdr->xmin + 1;
    int height = image->height = hdr->ymax - hdr->ymin + 1;

    int size = image->width * image->height;

    if (hdr->bitsPerPixel != 8)
        return false;

    unsigned char *scanline = image->image.data8 = malloc(size);
    if (scanline == NULL)
        return false;

    image->palette = malloc(256 * 2);
    if (image->palette == NULL)
    {
        free(scanline);
        return false;
    }

    int count = 0;

    for (int iy = 0; iy < height; iy++)
    {
        count = 0;
        while (count < scansize)
        {
            unsigned char c = *pcx++;

            if (c < 192)
            {
                scanline[count++] = c;
            }
            else
            {
                int run = c - 192;
                c = *pcx++;

                for (int i = 0; i < run && count < scansize; i++)
                    scanline[count++] = c;
            }
        }
        scanline += width;
    }

    // check for the palette marker.
    // I have seen PCX files without this, but the docs don't seem ambiguous--it must be
    // here. Anyway, the support among other apps is poor, so we're going to reject it.
    if (*pcx != 0x0C)
    {
        free(image->image.data8);
        image->image.data8 = 0;
        free(image->palette);
        image->palette = 0;
        return false;
    }

    pcx++;

    pal = (const RGB_24 *)(pcx);

    image->bpp = 8;

    for (int i = 0; i < 256; i++)
    {
        u8 r = (pal[i].r + 4 > 255) ? 255 : (pal[i].r + 4);
        u8 g = (pal[i].g + 4 > 255) ? 255 : (pal[i].g + 4);
        u8 b = (pal[i].b + 4 > 255) ? 255 : (pal[i].b + 4);
        image->palette[i] = RGB15(r >> 3, g >> 3, b >> 3);
    }

    return true;
}


void myimageDestroy(sImage *img)
{
    if (img->image.data8)
        free(img->image.data8);

    if (img->palette && img->bpp == 8)
        free(img->palette);
}

void loadFont(font_struct* f, u8 charsize, u8 rendersize)
{
    if (!f){
        return;
    }
	int i, j;
	int param;
	uint8 texX, texy;
	//u8 buffer[512*64/4];

	u8 * buffer=malloc(512*64/4*sizeof(u8));
	sImage pcx;
	u8 *buffer2;
	u16 palette[4];

	buffer2=bufferizeFile("HUD.pcx", "font", NULL, true);
	if(!buffer2){
        free(buffer);
        return;
    }
	myloadPCX((u8*)buffer2, &pcx);

	palette[0]=RGB15(31,0,31);
	palette[1]=RGB15(31,31,31);
	palette[2]=RGB15(0,0,0);
	palette[3]=RGB15(0,0,0);
    unsigned int size=pcx.width*pcx.height*pcx.bpp/8;

	for(j=0;j<64;j++)
	{
		for(i=0;i<512/4;i++)
		{

            if((i*4+3+j*512)<size)
            {
			buffer[i+j*512/4]=(pcx.image.data8[i*4+j*512])|((pcx.image.data8[i*4+1+j*512])<<2)|((pcx.image.data8[i*4+2+j*512])<<4)|((pcx.image.data8[i*4+3+j*512])<<6);
            } else
            {
                NOGBA("OUT OF BOUNDS IMAGE ACCESS\n");
            }
		}
	}
	myimageDestroy(&pcx);
	free(buffer2);

	f->tex.used=true;

	f->tex.width=512;
	f->tex.height=64;
	f->tex.size=f->tex.width*f->tex.height/4;
	addPaletteToBank(&f->tex, palette, 8*2);
	param = TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | (1<<29);

	getTextureAddress(&f->tex);
	getGlWL(f->tex.width, f->tex.height, &texX, &texy);

	setTextureParameter(&f->tex, texX, texy, f->tex.addr, GL_RGB4, param);

	addToBank(&f->tex, buffer, f->tex.bank);

	f->charsize=charsize;
	f->rendersize=rendersize;
	f->charsizef32=inttof32(charsize);

	setFont(f);
    free(buffer);
}

void drawCharRelative(char c)
{
	c-=32;

	int tx=(c*16)%512;
	int ty=16*(c*16-tx)/512;

	glBegin(GL_QUADS);
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx), inttot16(ty));
		GFX_VERTEX10 = FONT_T1;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx), inttot16(ty+16));
		GFX_VERTEX10 = FONT_T2;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx+16), inttot16(ty+16));
		GFX_VERTEX10 = FONT_T3;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx+16), inttot16(ty));
		GFX_VERTEX10 = FONT_T4;

	glTranslatef32(inttof32(1)/2, 0, 0);
}

void drawChar(char c, u16 color, int32 x, int32 y)
{
	c-=32;

	int tx=(c*16)%512;
	int ty=16*(c*16-tx)/512;

	glPushMatrix();

	glColor(color);

	bindTexture(&currentFont->tex);
	bindPalette4(&currentFont->tex);

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_ID(63));

	glTranslatef32(x, y, 0);

	glScalef32((currentFont->charsizef32),(currentFont->charsizef32),inttof32(1));

	glBegin(GL_QUADS);

		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx), inttot16(ty));
		GFX_VERTEX10 = FONT_T1;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx), inttot16(ty+16));
		GFX_VERTEX10 = FONT_T2;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx+16), inttot16(ty+16));
		GFX_VERTEX10 = FONT_T3;
		GFX_TEX_COORD = TEXTURE_PACK(inttot16(tx+16), inttot16(ty));
		GFX_VERTEX10 = FONT_T4;

	glEnd();
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

	glPopMatrix(1);
}

void drawString(char* s, u16 color, int32 size, int32 x, int32 y)
{
	int n=strlen(s);
	int i;

	glColor(color);
	bindTexture(&currentFont->tex);
	bindPalette4(&currentFont->tex);
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_ID(48));

	glPushMatrix();
		glTranslatef32(x, y, 0);
		glScalef32((currentFont->charsizef32),(currentFont->charsizef32),inttof32(1));
		glScalef32(size, size, inttof32(1));
		glTranslatef32(inttof32(1)/4, inttof32(1)/2, 0);
		for(i=0;i<n;i++)
		{
			drawCharRelative(s[i]);
		}
	glPopMatrix(1);
	unbindMtl();
}
