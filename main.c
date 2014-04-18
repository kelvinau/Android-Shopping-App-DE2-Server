#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <assert.h>
#include "values.h"
#include "sd_card.h"
#include "middleman.h"
#include "io.h"
#include "nios2_ctrl_reg_macros.h"
#include "altera_up_avalon_rs232.h"
#include "input.h"
#include "UART.h"
#include "screen.h"

alt_up_ps2_dev *ps2;
KB_CODE_TYPE decode_mode;
char ascii;
alt_u8 buf[12];
unsigned char barcode[12];
unsigned char buffer[200];
unsigned char message[17];
unsigned char ID[2];
alt_up_rs232_dev *uart;
unsigned char data;
unsigned char parity;
float start_time;
float current_time;
unsigned char checksumChar;
int count;
_Bool ready_flag;
int packetBuffer[10];
int packetCount;
int packetHead;
int packetTail;
char packetCountC[1];
int currentID;
int buffer_length = 0;

item *list;
int total_items = 0;

unsigned char *input;
int i;
unsigned char msg[600];

int main()
{
	initScanner();
	initUart();
	initSD(); // ******************* Have to be called after initUart
	initScreen();
	clearScreen();


	//updateScreen();

	list = malloc(sizeof(item));
	if (getItems(list) == -1)
		printf("Error in reading SD card.\n");
	else
		printf("All items are retrieved.\n");


/*
	item result = scan("058716989744");
	printf("name: %s\n",result.name);
	printf("Stock: %d\n",result.stock);
	buyItem(result,8);
	//result = scan("058716989744");
	//printf("After decreasing - Stock: %d\n",result.stock);


		result = scan("678523040249");
		printf("name: %s\n",result.name);
		printf("Stock: %d\n",result.stock);
		buyItem(result,6);
		//result = scan("678523040249");
		//printf("After decreasing - Stock: %d\n",result.stock);


		result = scan("066721002501");
		printf("name: %s\n",result.name);
		printf("Stock: %d\n",result.stock);
		buyItem(result,6);
		result = scan("066721002501");
		printf("After decreasing - Stock: %d\n",result.stock);


		result = scan("041143120033");
		printf("name: %s\n",result.name);
		printf("Stock: %d\n",result.stock);
		buyItem(result,5);
		result = scan("041143120033");
		printf("After decreasing - Stock: %d\n",result.stock);


		result = scan("073141551342");
		printf("name: %s\n",result.name);
			printf("Stock: %d\n",result.stock);

			buyItem(result,2);
			//result = scan("073141551342");
			//printf("After decreasing - Stock: %d\n",result.stock);

			//updateDatabase();

*/


	int printed;
while(1){
	clearScreen();
	printMsg("Enter the password for connecting to the server:",0,0);
	printed = 0;
	 alt_up_ps2_clear_fifo(ps2);
	 memset(buffer,0,sizeof(buffer));
	 memset(barcode,0,sizeof(barcode));

	 buffer_length = 0;
	 while ((count == 0) || (buffer[count-1] != '\0')){
		 if (printed==0){
			 printf("Enter the password to connect to the server OR Scan an item\n");
			 printed = 1;
		 }
		// recieveMessage();
		 if (buffer_length != strlen(buffer)){
			 printMsg(buffer,0,1);
			 printf("%c",buffer[count-1]);
			 buffer_length = strlen(buffer);
		 }
	 }
	 printf("\n");
	 count=0;

	 printf("BUFFER: %s\n",buffer);
	 printf("BUFFER LENGTH: %i\n",strlen(buffer));

	 if (strcmp(buffer,"PASSWORD") == 0){
		 while(!(buffer[0] == '4' && strlen(buffer) == 1)){
		 clearScreen();
		 memset(buffer,0,sizeof(buffer));
		 printMsg("Three options of Transaction:",0,0);
		 printMsg("1. Adding a new item",0,2);
		 printMsg("2. Buying an item",0,4);
		 printMsg("3. Changing an existing item's details",0,6);
		 printMsg("4. Exit",0,8);
		 printMsg("Enter 1, 2, 3, or 4:",0,10);

		 printf("Three options of Transaction: \n1. Adding a new item\n2. Buying an item\n3. Changing an existing item's details\n");
		 printf("Enter 1, 2, or 3:");
		 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,20,10);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
		 printf("\n");
		 count=0;

		 if (buffer[0] == '1' && strlen(buffer) == 1){
			 clearScreen();
			printf("Adding a new item...\n");
			getItemFromInput('A',msg);
			printf("here");
			transaction('A',msg);
		 }
		 else if (buffer[0] == '2' && strlen(buffer) == 1){
			 clearScreen();
			 printf("Buying an item...\n");
			 getItemFromInput('B',msg);
			 transaction('B',msg);
		 }
		 else if (buffer[0] == '3' && strlen(buffer) == 1){
			 clearScreen();
			printMsg("Changing an existing item's details...",0,0);
			 printf("Changing an existing item's details...\n");
			getItemFromInput('C',msg);
			transaction('C',msg);
		 }
		}
	 }
	 else if(buffer[0] == 'S' && strlen(buffer) == 1){
		 while(!(buffer[0] == 'E' && strlen(buffer) == 1)){
			 clearScreen();
			 memset(buffer,0,sizeof(buffer));

			 showChanges();
			 printMsg("Enter E to exit:",0,30);
			 while ((count == 0) || (buffer[count-1] != '\0')){
				 //recieveMessage();
				 if (buffer_length != strlen(buffer)){
					 printMsg(buffer,16,30);
					 printf("%c",buffer[count-1]);
					 buffer_length = strlen(buffer);
				 }
			 }
			 printf("\n");
			 count=0;
		 }
	 }
	 else if ((strlen(buffer) == 2) || (strlen(buffer) == 1)){
		 //ID code
		 for (i=0;i<strlen(buffer);i++){
			 ID[i] = buffer[i];
		 }
		 ID[strlen(buffer)] = '\0';
		 ready_flag = 1;
		 currentID = atoi(ID);
		 printf("ID: %i\n",currentID);
	 }
	 else if (strlen(buffer) == 12){
		 if (ready_flag == 1){

			 for (i=0; i<12; i++){
				 barcode[i] = buffer[i];
			 }

			 checksumChar = checksum(barcode);

			 barcode[12] = '\0';

			 item result = scan(barcode);

			 if (result.upc[0] == '\0'){
				 printf("BARCODE: %s\n",barcode);
				 printf("ITEM NOT FOUND!\n");
			 	 //add an enter item function
			 }
			 else{
				 printf("BARCODE: %s\n",barcode);
			 	 printf("ITEM FOUND: %s\n",result.upc);
			 	 printf("ITEM NAME: %s\n",result.name);
			 	 printf("PRICE: %.2f\n",result.price);
			 	 printf("STOCK: %d\n",result.stock);
			 	 if (result.stock > 0) {
			 		 //ID + SIZE + TYPE + PC + UPC + CS
			 		 message[0] = currentID;//ID
			 		 message[1] = 15;//SIZE
			 		 message[2] = 'U';//TYPE
			 		 message[3] = 3;//PacketCount
			 		 for (i=4; i<16; i++){
			 			 message[i] = barcode[i-4];
			 		 }
			 		 message[16] = checksumChar;
			 		 message[17] = '\0';
			 		 printf("MESSAGE: %s\n",message);
			 		 sendMessage(message);

			 		 int newstock = buyItem(result,1);
			 		 printf("UPDATED STOCK: %d\n",newstock);
			 		 //updateDatabase(list);
			 	 }
			 	 else {
			 		 printf("STOCK EMPTY - BARCODE NOT SENT TO APP\n");
			 	  	 printf("INCREASING STOCK\n");
			 	  	//` int newstock = increaseStockMemory(list,result);
			 	  	// printf("UPDATED STOCK: %d\n",newstock);
			 	  	 //updateDatabase(list);
			 	 }
			 }
		 }
	 }

}


/*
	if (getItems(list) == -1)
		printf("Error in reading SD card.\n");
	else
		printf("All items are retrieved.\n");

	printf("UPC: %s\n",list[TOTAL_ITEMS-1].upc);
	printf("Name: %s\n",list[TOTAL_ITEMS-1].name);
	//printf("Description: %s\n",list[0].description);
	printf("Price: %.2f\n",list[TOTAL_ITEMS-1].price);
	//printf("Category: %s\n",list[0].category);
	printf("Stock: %d\n",list[TOTAL_ITEMS-1].stock);

	unsigned char upc[] = "058716989744";
	item result = scan(upc);

	printf("check UPC: %s\n",upc);

	if (result.upc[0] == '\0')
		printf("Item not found.\n");
	else{
		printf("Item found.\n");
		printf("Item with UPC: %s\n",result.upc);
		printf("Name: %s\n",result.name);
		//printf("Description: %s\n",result.description);
		printf("Price: %.2f\n",result.price);
		//printf("Category: %s\n",result.category);
		printf("Stock: %d\n",result.stock);
	}

	send_upc(result);
	// Check getItemFromMsg
	unsigned char msg[] = "999999999999,\"ITEM_TEST1\",2.47,100,\"This is a description\",\"This is the image link\"";
	unsigned char msg2[] = "999999999999,\"ITEM_TEST1\",2.47,100,\"This is a loooffkfkfkfkfkfk loooooooongooooooooooooooooooooooooooooooooooooong description\",\"This is the image link\"";
	//transaction('A',msg);

	unsigned char msg3[] = "073141552325,7";
	//transaction('B',msg3);

	unsigned char msg4[] = "058716989744,\"SUPER Planters - Praline Nut Mix - 575g\",7.99,1000,\"This is a description\",\"This is the image link\"";
	//transaction('C',msg4);
*/
	//sendToMiddleman(result);
	//receiveFromMiddleman();
/*
	//************** Don't call updateDatabase for the first time!!!!!!!!!!!!!!!!!!!! ********************************
	decreaseStockMemory(result);
	decrresult = scan(upc);easeStockMemory(result);
	result = scan(upc);
	printf("After decreasing - Stock: %d\n",result.stock);
	//updateDatabase(list);
	decreaseStockMemory(result);
	decreaseStockMemory(result);
	result = scan(upc);
	printf("After decreasing - Stock: %d\n",result.stock);
	//updateDatabase(list);
*/
/*	-- Check Employee
	item tmp;
	strcpy(tmp.upc, "358716989744");
	strcpy(tmp.name, "\"Planters - Praline Nut Mix - 575g\"");
	tmp.price = 5.97;
	tmp.stock = 10;

	send_upc(tmp);


	while(1){
		receiveFromMiddleman();
	}*/

	//free(list);
  return 0;
 }
