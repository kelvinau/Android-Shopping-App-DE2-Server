/*
 * vga.h
 *
 *	Created: 2014-01-18
 *		Author: Brittaney Geisler, Isaac Cheng, Kelvin Au, Kevin Hui
 */

#include <system.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <math.h>
#include <string.h>

typedef struct player{
	int x;
	int y;
	int hp;
	int colour;
	int power;
} player;

void initScreen(void);
void clearScreen(void);
void updateScreen(void);
void printLines(void);
void printMsg(unsigned char*,int,int);


void displayHighScore(char *,char *,char *,char *);
void searchScore(char*);
