/*
 * vga.c
 *
 *	Created: 2014-01-18
 *		Author: Brittaney Geisler, Isaac Cheng, Kelvin Au, Kevin Hui
 */
#include "screen.h"
#include "values.h"
#include <stdlib.h>
#include <stdio.h>
#include "sd_card.h"
#include <string.h>

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev * char_buffer;

int i = 0;
int numPlayers = 4;

player p[2];



void initScreen() {
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + PIXEL_BUFFER_SPAN/2;// Set the 1st buffer address


	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(PIXEL_BUFFER_DMA_NAME);

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr2);

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);
}

void clearScreen(){
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
	alt_up_char_buffer_clear(char_buffer);
}

void updateScreen(){
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
		//printf("is it white?"); 	for (count = 0; count < wait; ++count) {	}
}


void printMsg(unsigned char* msg,int x, int y) {

		alt_up_char_buffer_string(char_buffer, msg, x, y);
}




















