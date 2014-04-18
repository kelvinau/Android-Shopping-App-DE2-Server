#include <stdio.h>
#include <string.h>
#include "altera_up_avalon_rs232.h"

#include "values.h"

void initUart();
void clearUart();
void receiveFromMiddleman();
void send_upc(item);
void sendConfirmation(int);
void sendMsg(unsigned char[]);
int receiveMsg(int);
