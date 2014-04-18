#include "input.h"

extern alt_up_ps2_dev *ps2;
extern KB_CODE_TYPE decode_mode;
extern char ascii;
extern alt_u8 buf[12];
extern int count=0;
extern unsigned char buffer[50];
extern float start_time;

extern unsigned char msg[50];
static void keyboard_ISR( void *c, alt_u32 id )
{
	decode_scancode(ps2,&decode_mode, buf, &ascii);
	if (decode_mode == 1 || (decode_mode == 2) ){
		if (buf[0] == 'Z'){
			buffer[count] = '\0';
		}
		else{
			buffer[count] = ascii;
		}
		count++;
		//printf("ascii:%d\n",ascii);
	}

}

void initScanner(){
	//for scanner
	 ps2 = alt_up_ps2_open_dev(KEYBOARD_NAME);
	 alt_up_ps2_init(ps2);
	 alt_up_ps2_clear_fifo(ps2);
	 void* keyboard_control_register_ptr = (void*) (KEYBOARD_BASE + 4);
	 alt_irq_register(KEYBOARD_IRQ, keyboard_control_register_ptr, keyboard_ISR);
	 alt_up_ps2_enable_read_interrupt(ps2);
}

unsigned char checksum(unsigned char barcode []){
	unsigned char sum=0;
	int i=0;
	for (i = 0; i < 12; i++){
		sum = barcode[i] + sum;
	}
	return sum;
}

void getItemFromInput(unsigned char type,unsigned char msg[]){
	int i,j;
	unsigned char *head_msg;

	if (type == 'A')
		head_msg = "Adding a new item...";
	else if (type == 'B')
		head_msg = "Buying an item...";
	else
		head_msg = "Changing an existing item's details...";
	printMsg(head_msg,0,0);

	memset(buffer,0,sizeof(buffer));
	printMsg("Enter the upc:",0,2);
	 printf("Enter the upc:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,14,2);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;
	while (strlen(buffer) != 12){
		clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg("Error:Invalid input for UPC!",52,0);
		memset(buffer,0,sizeof(buffer));
		printf("UPC has 12 digits! Re-enter the UPC:");
		 while ((count == 0) || (buffer[count-1] != '\0')){
				 //recieveMessage();
				 if (buffer_length != strlen(buffer)){
					 printMsg(buffer,14,2);
					 printf("%c",buffer[count-1]);
					 buffer_length = strlen(buffer);
				 }
			 }
		printf("\n");
		count=0;
	}

	for (i=0;buffer[i] != '\0';i++){
		msg[i] = buffer[i];
	}
	msg[i++] = ',';
	unsigned char upc_str[strlen(buffer)+1];
	strncpy(upc_str,buffer,strlen(buffer));
	upc_str[strlen(buffer)] = '\0';

	if (type == 'A' || type == 'C'){
		clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg(upc_str,14,2);
		memset(buffer,0,sizeof(buffer));

		printMsg("Enter the name:",0,4);
		printf("Enter the name:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,15,4);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;

	 msg[i++] = '"';
	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = '"';
	msg[i++] = ',';

	unsigned char name_str[strlen(buffer)+1];
	strncpy(name_str,buffer,strlen(buffer));
	name_str[strlen(buffer)] = '\0';

	clearScreen();
	printMsg(head_msg,0,0);
	printMsg("Enter the upc:",0,2);
	printMsg(upc_str,14,2);
	printMsg("Enter the name:",0,4);
	printMsg(name_str,15,4);
	memset(buffer,0,sizeof(buffer));

	printMsg("Enter the price:",0,6);
	printf("Enter the price(2 decimal places):");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,16,6);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;
	 while (atof(buffer) <= 0){
			clearScreen();
			printMsg(head_msg,0,0);
			printMsg("Enter the upc:",0,2);
			printMsg(upc_str,14,2);
			printMsg("Enter the name:",0,4);
			printMsg(name_str,15,4);
			printMsg("Enter the price:",0,6);
		 memset(buffer,0,sizeof(buffer));
		 printMsg("Error:Invalid input for Price!",50,0);
			printf("Invalid input for Price! Re-enter the Price:");
			 while ((count == 0) || (buffer[count-1] != '\0')){
					 //recieveMessage();
					 if (buffer_length != strlen(buffer)){
						 printMsg(buffer,16,6);
						 printf("%c",buffer[count-1]);
						 buffer_length = strlen(buffer);
					 }
				 }
			printf("\n");
			count=0;
	 }

	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = ',';
	unsigned char price_str[strlen(buffer)+1];
	strncpy(price_str,buffer,strlen(buffer));
	price_str[strlen(buffer)] = '\0';

	clearScreen();
	printMsg(head_msg,0,0);
	printMsg("Enter the upc:",0,2);
	printMsg(upc_str,14,2);
	printMsg("Enter the name:",0,4);
	printMsg(name_str,15,4);
	printMsg("Enter the price:",0,6);
	printMsg(price_str,16,6);

	memset(buffer,0,sizeof(buffer));
	printMsg("Enter the stock number:",0,8);
	printf("Enter the stock number:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,23,8);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;
	 while (atoi(buffer) <= 0){
		 clearScreen();
		 	printMsg(head_msg,0,0);
		 	printMsg("Enter the upc:",0,2);
		 	printMsg(upc_str,14,2);
		 	printMsg("Enter the name:",0,4);
		 	printMsg(name_str,15,4);
		 	printMsg("Enter the price:",0,6);
		 	printMsg(price_str,16,6);
		 	printMsg("Enter the stock number:",0,8);
		 memset(buffer,0,sizeof(buffer));
		 printMsg("Error:Invalid input for Stock Number!",45,0);
			printf("Invalid input of Stock! Re-enter the Stock:\n");
			 while ((count == 0) || (buffer[count-1] != '\0')){
					 //recieveMessage();
					 if (buffer_length != strlen(buffer)){
						 printMsg(buffer,23,8);
						 printf("%c",buffer[count-1]);
						 buffer_length = strlen(buffer);
					 }
				 }
			printf("\n");
			count=0;
	 }

	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = ',';
	unsigned char stock_str[strlen(buffer)+1];
	strncpy(stock_str,buffer,strlen(buffer));
	stock_str[strlen(buffer)] = '\0';

	clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg(upc_str,14,2);
		printMsg("Enter the name:",0,4);
		printMsg(name_str,15,4);
		printMsg("Enter the price:",0,6);
		printMsg(price_str,16,6);
		printMsg("Enter the stock number:",0,8);
		 printMsg(stock_str,23,8);
	memset(buffer,0,sizeof(buffer));
	printMsg("Enter the Description:",0,10);
	printf("Enter the Description:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,22,10);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;

	msg[i++] = '"';
	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = '"';
	msg[i++] = ',';

	unsigned char desc_str[strlen(buffer)+1];
	strncpy(desc_str,buffer,strlen(buffer));
	desc_str[strlen(buffer)] = '\0';

	clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg(upc_str,14,2);
		printMsg("Enter the name:",0,4);
		printMsg(name_str,15,4);
		printMsg("Enter the price:",0,6);
		printMsg(price_str,16,6);
		printMsg("Enter the stock number:",0,8);
		 printMsg(stock_str,23,8);
		 printMsg("Enter the description:",0,10);
		 printMsg(desc_str,22,10);

	memset(buffer,0,sizeof(buffer));
	printMsg("Enter the image link of the item:",0,12);
	printf("Enter the image link of the item:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,33,12);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;

	 msg[i++] = '"';

	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = '"';
	msg[i++] = ',';

	unsigned char link_str[strlen(buffer)+1];
	strncpy(link_str,buffer,strlen(buffer));
	link_str[strlen(buffer)] = '\0';

	clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg(upc_str,14,2);
		printMsg("Enter the name:",0,4);
		printMsg(name_str,15,4);
		printMsg("Enter the price:",0,6);
		printMsg(price_str,16,6);
		printMsg("Enter the stock number:",0,8);
		 printMsg(stock_str,23,8);
		 printMsg("Enter the description:",0,10);
		 printMsg(desc_str,22,10);
		 printMsg("Enter the image link of the item:",0,12);
		 printMsg(link_str,33,12);

	memset(buffer,0,sizeof(buffer));
	printMsg("Enter the category of the item:",0,14);
	printf("Enter the category of the item:");
	 while ((count == 0) || (buffer[count-1] != '\0')){
			 //recieveMessage();
			 if (buffer_length != strlen(buffer)){
				 printMsg(buffer,31,14);
				 printf("%c",buffer[count-1]);
				 buffer_length = strlen(buffer);
			 }
		 }
	 printf("\n");
	 count=0;

	 msg[i++] = '"';

	for (j=0;buffer[j] != '\0';j++,i++){
		msg[i] = buffer[j];
	}
	msg[i++] = '"';













	msg[i] = '\0';




	printf("Input Message:%s\n",msg);
	//return msg;
	}



	else{
		clearScreen();
		printMsg(head_msg,0,0);
		printMsg("Enter the upc:",0,2);
		printMsg(upc_str,14,2);

		memset(buffer,0,sizeof(buffer));
		printMsg("Enter the quantity:",0,4);
		printf("Enter the quantity:");
		 while ((count == 0) || (buffer[count-1] != '\0')){
				 //recieveMessage();
				 if (buffer_length != strlen(buffer)){
					 printMsg(buffer,19,4);
					 printf("%c",buffer[count-1]);
					 buffer_length = strlen(buffer);
				 }
			 }
		 printf("\n");
		 count=0;
		 while (atoi(buffer) <= 0){
			 	 clearScreen();
			 	printMsg(head_msg,0,0);
			 	printMsg("Enter the upc:",0,2);
			 	printMsg(upc_str,14,2);
				printMsg("Enter the quantity:",0,4);

			 memset(buffer,0,sizeof(buffer));
			 printMsg("Invalid input for quantity!",50,0);
				printf("Invalid input of Stock! Re-enter the Stock:\n");
				 while ((count == 0) || (buffer[count-1] != '\0')){
						 //recieveMessage();
						 if (buffer_length != strlen(buffer)){
							 printMsg(buffer,19,4);
							 printf("%c",buffer[count-1]);
							 buffer_length = strlen(buffer);
						 }
					 }
				printf("\n");
				count=0;
		 }

		for (j=0;buffer[j] != '\0';j++,i++){
			msg[i] = buffer[j];
		}
		msg[i] = '\0';

		//msg = concat(concat(upc_str,","),quan_str);
		printf("Input Message:%s\n",msg);
		//return msg;
	}
}
