#include <msp430.h> 
#include "peripherals.h"
#include "notes.h"

/**
 * main.c
 */

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
enum GameState{Welcome, CheckStart, CountDown, PlayNotes};



void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	enum GameState state;
	state = Welcome;

	configDisplay();
	configKeypad();
	configButtons();
	initLeds();

	Graphics_clearDisplay(&g_sContext); // Clear the display

	//temporary
	unsigned char song[] = {7, 5, 3, 7, 5, 3, 8, 7, 5, 8, 7, 5};

	unsigned int i, j;
	while(1)
	{
        switch(state)
        {
            case Welcome://welcome screen
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "MSP430 Hero", AUTO_STRING_LENGTH, 48, 44, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press * to begin", AUTO_STRING_LENGTH, 48, 52, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                state = CheckStart;

                break;

            case CheckStart:
                if(getKey() == '*'){
                    //loopCounter = 0;
                    state = CountDown;
                }
                break;

            case CountDown:
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                setLeds(0x8);
                swDelay(1);
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                setLeds(0xC);
                swDelay(1);
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                setLeds(0xE);
                swDelay(1);
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "GO", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                setLeds(0xF);

                state = PlayNotes;

                break;
            case PlayNotes:
                for(i = 0; i < sizeof(song); i++){
                    playNote(notes[song[i]].pitch);
                    setLeds(notes[song[i]].led);
                    swDelay(1);
                }
                BuzzerOff();
                state = Welcome;
                break;
        }
	}

}


void swDelay(char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2021

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}
