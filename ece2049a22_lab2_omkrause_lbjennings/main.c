#include <msp430.h> 
#include "peripherals.h"
#include "notes.h"
#include "timer.h"


#define COUNTDOWN_DELAY  1000
#define NOTE_A 0
#define NOTE_B 1
#define NOTE_C 3
#define NOTE_CS 4
#define NOTE_D 5
#define NOTE_Eb 6
#define NOTE_E 7
#define NOTE_F 8
#define NOTE_G 10
#define TERMINATOR 32000
/**
 * main.c
 */

// Function Prototypes
void setupTimerA2();

// Declare globals here
enum GameState{Welcome, CheckStart, CountDown, PlayNotes, WinGame, LoseGame, EndGame};

struct Song{
    unsigned int NoteIndex;
    unsigned int NoteDuration;
};

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	_BIS_SR(GIE); //enable interrupts
	
	enum GameState state;
	state = Welcome; //state machine state

	configDisplay();
	configKeypad();
	configButtons();
	initLeds(); //configuring periphrials, see periphrials.c

	Graphics_clearDisplay(&g_sContext); // Clear the display


	setupTimerA2();
	unsigned long int start_time; // used to calclate delays
	unsigned char countdown; //current countdown value
	unsigned int CurrSongIndex = 0; //current note in song

	int numMissed = 0; // numbers of errors
	bool hitNote = false; //has the player hit the button corresponding to the current note?

	//struct containing index of corresponding note and duration in timer increments
	//TERMINATOR is a value outside the note range that represents the end of the song
	struct Song song1[] = {{NOTE_CS, 100}, {NOTE_C, 100}, {NOTE_CS, 100}, {NOTE_C, 100}, {NOTE_B, 100}, {NOTE_A, 100},
	                       {NOTE_B, 100}, {NOTE_C, 100}, {NOTE_D, 100}, {NOTE_Eb, 100}, {NOTE_E, 100}, {NOTE_Eb, 100},
	                       {NOTE_E, 100}, {NOTE_D, 100}, {NOTE_C, 100}, {NOTE_D, 100}, {NOTE_E, 100}, {NOTE_F,100},
	                       {NOTE_G, 200}, {NOTE_F, 100}, {NOTE_G, 100}, {NOTE_E, 100}, {NOTE_D, 100}, {NOTE_CS, 400},
	                       {NOTE_C, 100}, {NOTE_A, 200}, {NOTE_F, 100}, {NOTE_G, 100}, {TERMINATOR, 0}};


	while(1)
	{
	    //reset switch checker, outside of standard state machine
	    if(getKey() == '#') {
	        BuzzerOff();
	        setLeds(0x0);
	        CurrSongIndex = 0;
            numMissed = 0;
            hitNote = false;
            state = Welcome;
	    }
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
                //seperate from Welcome state so it doesnt make screen flicker from redrawing
                if(getKey() == '*'){
                    //reset game state, start
                    countdown = 0;
                    start_time = 0;
                    CurrSongIndex = 0;
                    state = CountDown;
                }
                break;

            case CountDown:
                //display countdown timer, standard delay is .2 seconds between screens
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
                            playNote(notes[song1[CurrSongIndex].NoteIndex].pitch);
                            setLeds(notes[song1[CurrSongIndex].NoteIndex].led);
                            break;
                    }

                    countdown++;
                    start_time = timer_cnt;
                }
                break;

            case PlayNotes:
                //plays each note, checks to see if reached end state
                //updates based on each note's duration
                if(timer_cnt > (start_time + song1[CurrSongIndex].NoteDuration)){
                    start_time = timer_cnt;
                    CurrSongIndex++;
                    if(song1[CurrSongIndex].NoteIndex == TERMINATOR){
                        //if reached end of song, player wins
                        BuzzerOff();
                        setLeds(0x0);
                        state = WinGame;
                    }else if(numMissed > 2){
                        //if has missed enough notes, player loses
                        BuzzerOff();
                        setLeds(0x0);
                        state = LoseGame;
                    }else{
                        if(hitNote == false){
                            //if hasn't played note yet, count is increased
                            numMissed++;
                        }
                        //plays next note and sets corresponding LED
                        playNote(notes[song1[CurrSongIndex].NoteIndex].pitch);
                        setLeds(notes[song1[CurrSongIndex].NoteIndex].led);
                    }
                }
                if(getButtons() == notes[song1[CurrSongIndex].NoteIndex].led){
                    //to hit a note, player must hit the same button as the led that is
                    //currently lit, only needs to happen once during the period
                    //does not count other notes played.
                    hitNote = true;
                }

                break;

            case WinGame:
                //display winning screen
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "YOU ARE WINNER", AUTO_STRING_LENGTH, 48, 44, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                state = EndGame;
                break;

            case LoseGame:
                //display losing screen
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "YOU LOSE", AUTO_STRING_LENGTH, 48, 44, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "YOU SUCK", AUTO_STRING_LENGTH, 48, 52, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                state = EndGame;
                break;

            case EndGame:
                //doesn't do anything, only here to make sure screen doesnt flash from redrawing
                //game will stay in this state until reset switch (#) is hit
                break;

        }
	}

}
