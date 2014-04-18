#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>
#include "altera_up_avalon_rs232.h"

void sendMessage(unsigned char[]);
void recieveMessage(void);

extern alt_up_rs232_dev *uart;
extern unsigned char data;
extern unsigned char parity;
