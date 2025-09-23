#include <citro2d.h>

#include <3ds.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct //definiowane struktury sprite'a, czyli nie wiem o co biega
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite spritee[1]; //jedna instacja sprite'a

int main(int argc, char **argv)
{
  	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
 	consoleInit(GFX_BOTTOM, NULL);

 	printf("\x1b[1;1HProgram start");

 	u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF);

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

 	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	!spriteSheet ? svcBreak(USERBREAK_PANIC) : printf("\x1b[2;1HSpriteSheet loaded");

 	Sprite* sprite = &spritee[0];

 	C2D_SpriteFromSheet(&sprite->spr, spriteSheet, 0);
	C2D_SpriteSetCenter(&sprite->spr, 0.5f, 0.5f);
	C2D_SpriteSetPos(&sprite->spr, 100, 100);

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

  		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrBlack);
		C2D_SceneBegin(top);

  		C2D_DrawSprite(spritee[0].spr);

  		C3D_FrameEnd(0);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}
 	C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
    romfsExit();
	gfxExit();
	return 0;
}
