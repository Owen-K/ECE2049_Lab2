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

struct Note notes[13] = {

{440, 0x08},   //A
{466, 0x04},   //B flat
{494, 0x02},   //B
{523, 0x01},   //C
{554, 0x08},   //C sharp
{587, 0x04},   //D
{622, 0x02},   //E flat
{659, 0x01},   //E
{698, 0x08},   //F
{740, 0x04},   //F sharp
{784, 0x02},   //G
{831, 0x01},   //A flat
{880, 0x08}};  //A

#endif /* NOTES_H_ */
