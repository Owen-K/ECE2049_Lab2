#include <msp430.h> 
#include "peripherals.h"
#include "notes.h"
#include "timer.h"

/**
 * main.c
 */

// Function Prototypes
void swDelay(char numLoops);
void setupTimerA2();

// Declare globals here
enum GameState{Welcome, CheckStart, CountDown, PlayNotes};
static const int COUNTDOWN_DELAY = 1000;

struct Song{
    unsigned int NoteIndex;
    unsigned int NoteDuration;
};

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	_BIS_SR(GIE);
	
	enum GameState state;
	state = Welcome;

	configDisplay();
	configKeypad();
	configButtons();
	initLeds();

	Graphics_clearDisplay(&g_sContext); // Clear the display


	setupTimerA2();
	unsigned long int start_time;
	unsigned char countdown;
	unsigned int CurrSongIndex = 0;
	unsigned char leds = 0;

	//temporary
	struct Song song1[] = {{7, 100}, {5, 50}, {3,200}, {7,100}, {5, 20}, {3,60},
	                       {8,400}, {7,150}, {5,99}, {8,50}, {7,500}, {5,10}, {32000,0}};


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
                    countdown = 0;
                    start_time = 0;
                    state = CountDown;
                }
                leds = getButtons();
                setLeds(leds);
                break;

            case CountDown:
                if (timer_cnt > (start_time + COUNTDOWN_DELAY/5)) {
                    switch(countdown) {
                        case 0:
                            Graphics_clearDisplay(&g_sContext);
                            Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                            Graphics_flushBuffer(&g_sContext);
                            setLeds(0x8);
                            break;
                        case 1:
                            Graphics_clearDisplay(&g_sContext);
                            Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                            Graphics_flushBuffer(&g_sContext);
                            setLeds(0xC);
                            break;
                        case 2:
                            Graphics_clearDisplay(&g_sContext);
                            Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                            Graphics_flushBuffer(&g_sContext);
                            setLeds(0xE);
                            break;
                        case 3:
                            Graphics_clearDisplay(&g_sContext);
                            Graphics_drawStringCentered(&g_sContext, "GO", AUTO_STRING_LENGTH, 48, 48, TRANSPARENT_TEXT);
                            Graphics_flushBuffer(&g_sContext);
                            setLeds(0xF);
                            break;
                        case 4:
                            state = PlayNotes;
                            start_time = timer_cnt;
                            BuzzerOn();
                            playNote(notes[song1[CurrSongIndex].NoteIndex].pitch);
                            setLeds(notes[song1[CurrSongIndex].NoteIndex].led);
                            break;
                    }

                    countdown++;
                    start_time = timer_cnt;
                }
                break;

            case PlayNotes:
                if(timer_cnt > (start_time + song1[CurrSongIndex].NoteDuration)){
                    start_time = timer_cnt;
                    CurrSongIndex++;
                    if(song1[CurrSongIndex].NoteIndex == 32000){
                        BuzzerOff();
                        setLeds(0x0);
                        CurrSongIndex = 0;
                        state = Welcome;
                    }else{
                        playNote(notes[song1[CurrSongIndex].NoteIndex].pitch);
                        setLeds(notes[song1[CurrSongIndex].NoteIndex].led);
                    }
                }



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
