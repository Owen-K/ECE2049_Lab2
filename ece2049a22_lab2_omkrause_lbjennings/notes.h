/*
 * notes.h
 *
 *  Created on: Sep 14, 2022
 *      Author: okrause
 */

#ifndef NOTES_H_
#define NOTES_H_

struct Note {
    unsigned int pitch;
    unsigned char led;
};

struct Note notes[13];

enum Notes{

notes[0] = {440, 0x0F};
notes[1] = {};
notes[2] = {};
notes[3] = {};

#endif /* NOTES_H_ */
