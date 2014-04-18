#include "UART.h"

void sendMessage(unsigned char barcode[]){
	 int i;
	 for (i = 0; i < strlen(barcode); i++){
		alt_up_rs232_write_data(uart, barcode[i]);
	 }
}

_Bool ID_p = 0;
_Bool size_p =0;
_Bool type_p = 0;
_Bool pc_p = 0;
_Bool bool_p = 0;

unsigned char ID_r;
unsigned char num_r;
unsigned char type_r;
unsigned char pc_r;
unsigned char bool_r;
unsigned char IDmessage[10];


void recieveMessage(){

	int i;

	if (alt_up_rs232_get_used_space_in_read_FIFO(uart) > 0){

		alt_up_rs232_read_data(uart, &data, &parity);

		if (ID_p == 0){
			ID_r = data;
			ID_p = 1;
		}
		else if(size_p == 0){
			num_r = data;
			size_p = 1;
		}
		else if(type_p == 0){
			type_r = data;
			type_p = 1;
			if (type_r == 'I'){
				printf("ID %d,NUM %d,TYPE: %c\n",ID_r,num_r,type_r);
				ID_p = 0;
				size_p = 0;
				type_p = 0;
				IDmessage[0] = ID_r;
				IDmessage[1] = 4;
				IDmessage[2] = 'I';//type
				IDmessage[3] = '1';//this should be packetCount
				IDmessage[4] = ID_r;//'1';
				IDmessage[5] = 'C';//this should be checksum
				IDmessage[6] = '\0';
				sendMessage(IDmessage);
			}
		}
		else if ((type_r == 'S') && (pc_p == 0)){
			pc_r = data;
			pc_p = 1;
		}
		else if ((type_r == 'S') && (bool_p == 0)){
			bool_p = 1;
			bool_r = data;
			printf("ID %i,num %i, type %c, pc %c, bool %c",ID_r,num_r,type_r,pc_r,bool_r);
			ID_p = 0;
			size_p = 0;
			type_p = 0;
			pc_p = 0;
			bool_p = 0;
		}
	}
}
