#include<libtransistor/nx.h>
#include <stdio.h>
#include "atari.h"
#include "input.h"
#include "akey.h"
#include "screen.h"
#include "colours.h"
#include<SDL2/SDL.h>

#define SWITCH_WIDTH 1280
#define SWITCH_HEIGHT 720
#define PIXEL_SIZE 3
#define VIEW_WIDTH Screen_WIDTH * PIXEL_SIZE
#define VIEW_HEIGHT Screen_HEIGHT * PIXEL_SIZE

// center the render
#define VIEW_OFFSET_WIDTH (SWITCH_WIDTH - VIEW_WIDTH) / 2
#define VIEW_OFFSET_HEIGHT (SWITCH_HEIGHT - VIEW_HEIGHT) / 2

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;


int PLATFORM_Initialise(int *argc, char *argv[])
{
    result_t r;

    if((r = sm_init()) != RESULT_OK){
        printf("failed to init sm: 0x%x\n", r);
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        exit(-1);
        return 0;
    }

    printf("initialized SDL\n");

    window = SDL_CreateWindow("n/a", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("window could not be created: %s\n", SDL_GetError());
        exit(-1);
        return 0;
    }
    surface = SDL_GetWindowSurface(window);
    return 1;
}

int PLATFORM_Exit(int run_monitor)
{
    if (run_monitor)
        return 1;

    SDL_DestroyWindow(window);
    SDL_Quit();
    // exit stuff here
    sm_finalize();
    return 0;
}

int PLATFORM_Keyboard(void)
{
    return AKEY_NONE;
}

void Switch_SetPixel(int32_t x, int32_t y, uint32_t pixel)
{

    SDL_Rect rect;
    rect.x = VIEW_OFFSET_WIDTH + x * PIXEL_SIZE;
    rect.y = VIEW_OFFSET_HEIGHT + y * PIXEL_SIZE;
    rect.w = PIXEL_SIZE;
    rect.h = PIXEL_SIZE;

    SDL_FillRect(surface, &rect, pixel);
}

void PLATFORM_DisplayScreen(void)
{
    for (int x = 0; x < Screen_WIDTH; x++)
    {
        for (int y = 0; y < Screen_HEIGHT; y++)
        {
                uint32_t rgb = Screen_atari[y * Screen_WIDTH + x];
                Switch_SetPixel(x, y, SDL_MapRGBA(surface->format, rgb > 16, rgb >> 8, rgb, 0xFF));
                //Switch_SetPixel(x, y, SDL_MapRGBA(surface->format, Colours_GetR(rgb), Colours_GetG(rgb), Colours_GetB(rgb), 0xFF));
        }
    }
    SDL_UpdateWindowSurface(window);
}

int PLATFORM_PORT(int num)
{
    return 0xff;
}

int PLATFORM_TRIG(int num)
{
    return 1;
}

void PLATFORM_Sleep(double s)
{
    if (s > 0) {
        svcSleepThread(s * 1e9);
    }
}

int main(int argc, char **argv)
{
    int ticks = 500;

    /* initialise Atari800 core */
    if (!Atari800_Initialise(&argc, argv)) {
        printf("Atari initialisation failed!\n");
        return 3;
    }

    /* main loop */
    while (--ticks) {
        INPUT_key_code = PLATFORM_Keyboard();
        Atari800_Frame();
        if (Atari800_display_screen)
            PLATFORM_DisplayScreen();
        printf("LOOP %d\n", ticks);
    }
    PLATFORM_Exit(0);
    return 0;
}