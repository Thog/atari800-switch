#include<libtransistor/nx.h>
#include <stdio.h>
#include "atari.h"
#include "input.h"
#include "akey.h"

int PLATFORM_Initialise(int *argc, char *argv[])
{
	return 1;
}

int PLATFORM_Exit(int run_monitor)
{
    if (run_monitor)
        return 1;

    // exit stuff here
    return 0;
}

int PLATFORM_Keyboard(void)
{
    return AKEY_NONE;
}

void PLATFORM_DisplayScreen(void)
{

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
    result_t r;

    if((r = sm_init()) != RESULT_OK){
        printf("failed to init sm: 0x%x\n", r);
        return 1;
    }

	/* initialise Atari800 core */
    if (!Atari800_Initialise(&argc, argv)) {
        printf("Atari initialisation failed!\n");
		return 3;
    }

	/* main loop */
	if (true) {
		INPUT_key_code = PLATFORM_Keyboard();
		Atari800_Frame();
		if (Atari800_display_screen)
            PLATFORM_DisplayScreen();
        // yield
	    svcSleepThread(1000*1000*1000);
    }
    sm_finalize();
    return 0;
}