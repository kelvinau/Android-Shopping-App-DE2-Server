#include "middleman.h"

int i;
unsigned char data;
unsigned char parity;
alt_up_rs232_dev* uart;

void initUart(){
	printf("UART Initialization\n");
	uart = alt_up_rs232_open_dev("/dev/rs232");
	clearUart();
}

void clearUart(){
	printf("Clearing read buffer\n");
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart)) {
		alt_up_rs232_read_data(uart, &data, &parity);
	}
}

/*
void receiveFromMiddleman(){
   printf("Waiting for data from the Middleman\n");
   while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);

   unsigned char msg[100];
   // First byte is the clientID
   alt_up_rs232_read_data(uart, &data, &parity);
   int clientID = (int)data;
   printf("clientID: %d\n", clientID);
   msg[0] = (unsigned char)(((int)'0')+clientID);
   msg[1] = ':';
   // Second byte is the number of characters in our message
   while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
   alt_up_rs232_read_data(uart, &data, &parity);


   int num_to_receive = (int)data;

   printf("About to receive %d characters:\n", num_to_receive);
   while (alt_up_rs232_get_used_space_in_read_FIFO(uart) <num_to_receive);
   for (i = 2; i < num_to_receive + 2; i++) {
	   printf("i:%d ",i);
	   while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0){
		   //printf("in while loop");
	   }
	   alt_up_rs232_read_data(uart, &data, &parity);
	   msg[i] = data;
	   printf("data:%c\n",data);
	   //printf("data: %c\n", data);
   }
   msg[i] = '\0';
   printf("Message: %s\n",msg);

   if (msg[4] == '-'){
	   if (msg[3] == 'E')
		   checkEmployee(msg);

	   else if (msg[3] == 'A');
	   	   //addItem(msg);
	   else if (msg[3] == 'C');
	   	   //changeItemDetail(msg);
   }
}
*/

void send_upc(item to_send){
	int i=0,j=0,k=0;
	unsigned char message[100];
	unsigned char tmp[20] = {'\0'};
	int clientID = receiveMsg(1); // 0xFF for all clients
	printf("Client connected:%d\n",clientID);

	while(i<2){
		if (i==0){
			message[j] = to_send.upc[k];
			j++;k++;
			if (to_send.upc[k] == '\0'){
				k=0;i++;
				message[j] = ',';
				j++;
			}
		}
		else if (i==1){
		    sprintf( tmp, "%.2f", to_send.price);
		   // printf( "%s\n", tmp);
		    message[j] = tmp[k];
		    j++;k++;
			if (tmp[k] == '\0'){
				k=0;i++;
				//message[j] = ',';
				//j++;
			}
		}
		else{
			message[j] = to_send.name[k];
			j++;k++;
			if (to_send.name[k] == '\0'){
				k=0;i++;
				message[j] = ';';
				j++;
			}
		}

	}

	message[j] = '\0';

	printf("Sending the message to the Middleman\n");
	printf("Message to send: %s\n",message);


	// Start with the clientID
	alt_up_rs232_write_data(uart, (unsigned char) clientID);

	// Then the number of bytes in the message
	alt_up_rs232_write_data(uart, (unsigned char) strlen(message));
	printf("message length: %d\n", strlen(message));
	// Now send the reversed message to the Middleman
	for (i = 0 ; message[i]!='\0' ; i++) {
		alt_up_rs232_write_data(uart, message[i]);
	}


}




void sendMsg(unsigned char msg[]){
	//clearUart();
	int msg_length = strlen(msg);
	unsigned char tmp[129];
	int segment = 0;

	while (msg_length > 128){
		strncpy(tmp, &msg[0+segment*128], 128);
		tmp[128] = '\0';
		sendMsg(tmp);
		msg_length -= 128;
		segment++;
	}

	if (msg_length < 128){
		strncpy(msg, &msg[strlen(msg) - msg_length], msg_length);
		msg[msg_length] = '\0';
	}
/*
	int clientID = 1; //  Server
	while(alt_up_rs232_get_available_space_in_write_FIFO(uart)<10);
	printf("Sending to user %d\n",clientID);
	alt_up_rs232_write_data(uart, clientID);

	while(alt_up_rs232_get_available_space_in_write_FIFO(uart)<10);
	alt_up_rs232_write_data(uart, (unsigned char)strlen(msg));
	printf("Message to be sent:\n%s\n",msg);
	printMsg("Sending request to the server...",0,26);
	printf("Sending the message to middleman...\n");
	printf("strlen(msg):%d\n",strlen(msg));
*/
	while(alt_up_rs232_get_available_space_in_write_FIFO(uart)< strlen(msg));
	for (i = 0 ; msg[i]!='\0' ; i++) {
		//while(alt_up_rs232_get_available_space_in_write_FIFO(uart)<10);
		alt_up_rs232_write_data(uart, msg[i]);
	}
	printf("Message Sent.\n");
}

// if type == 0 ,  get confirmation
// if type == 1 , get clientID
int receiveMsg(int type){
	int i = 0;
	printMsg("Waiting for confirmation from the server...",0,28);
	printf("Waiting for confirmation from the Middleman\n");
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0){
		i++;
		if (i > 30000000)
			break;
	}
	if (i < 30000000){
		unsigned char msg[100];
		int clientID = 1;
/*
		// First byte is the clientID
		alt_up_rs232_read_data(uart, &data, &parity);
		clientID = (int)data;
		printf("Receive confirmation from clientID: %d\n", clientID);
*/
		// Second byte is the number of characters in our message
		while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
		alt_up_rs232_read_data(uart, &data, &parity);
		int num_to_receive = (int)data;
		//printf("About to receive %d characters:\n", num_to_receive);

		while (alt_up_rs232_get_used_space_in_read_FIFO(uart) < num_to_receive);
		for (i = 0; i < num_to_receive; i++) {
			printf("i:%d ",i);
			//while (alt_up_rs232_get_used_space_in_read_FIFO(uart) < 10);
			alt_up_rs232_read_data(uart, &data, &parity);
			msg[i] = data;
			printf("data:%c\n",data);
		}
		msg[i] = '\0';
		printf("Message received: %s\n",msg);
		if (type == 0)
			return (msg[1] - '0');
		else
			clientID;
	}
	else{
		printMsg("Cannot get a response from the server!",0,30);
		printMsg("Exiting back to the transaction page.",0,32);
	}
}


