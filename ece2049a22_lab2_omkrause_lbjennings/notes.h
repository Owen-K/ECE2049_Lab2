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


notes[0] = {440, 0x08};   //A
notes[1] = {466, 0x04};   //B flat
notes[2] = {494, 0x02};   //B
notes[3] = {523, 0x01};   //C
notes[4] = {554, 0x08};   //C sharp
notes[5] = {587, 0x04};   //D
notes[6] = {622, 0x02};   //E flat
notes[7] = {659, 0x01};   //E
notes[8] = {698, 0x08};   //F
notes[9] = {740, 0x04};   //F sharp
notes[10] = {784, 0x02};  //G
notes[11] = {831, 0x01};  //A flat
notes[12] = {880, 0x08};  //A

#endif /* NOTES_H_ */
