#include "screen.h"

static u8 *BUFFER = (u8 *) 0xA0000;

//screen buffer
u8 _sbuffer[SCREEN_SIZE];

#define CURRENT (_sbuffer)

// VGA control port addresses
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void setMode13();

void screen_swap() {
    memcpy(BUFFER, &CURRENT, SCREEN_SIZE);
}

void screen_clear(u8 color) {
    memset(&CURRENT, color, SCREEN_SIZE);
}

void screen_init() {
    setMode13();

    // configure palette with 8-bit RRRGGGBB color
    outportb(PALETTE_MASK, 0xFF);
    outportb(PALETTE_WRITE, 0);
    for (u8 i = 0; i < 255; i++) {
        outportb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
}

static const u8 hor_regs [] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x13 };

static const u8 width_320[] = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };

static const u8 ver_regs  [] = { 0x06, 0x07, 0x09, 0x10, 0x11, 0x12, 0x15, 0x16 };

static const u8 height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };

void setMode13()
{
    outportb(0x3c2, 0x63);
    outportb(0x3d4, 0x0e11);

    for (int i = 0; i < 7; i++)
    {
        outportw(0x3d4, (u16) ((width_320[i] << 8) | hor_regs[i]));
    }

    for (int i = 0; i < 8; i++)
    {
        outportw(0x3d4, (u16) ((height_200[i] << 8) | ver_regs[i]));
    }

    outportw(0x3d4, 0x0008);

    outportw(0x3d4, 0x4014);
    outportw(0x3d4, 0xa317);
    outportw(0x3c4, 0x0e04);

    outportw(0x3c4, 0x0101);
    outportw(0x3c4, 0x0f02);
    outportw(0x3ce, 0x4005);
    outportw(0x3ce, 0x0506);

    inportb(0x3da);
    outportb(0x3c0, 0x30);
    outportb(0x3c0, 0x41);
    outportb(0x3c0, 0x33);
    outportb(0x3c0, 0x00);

    outportb(0x3c0, 0x20);
}