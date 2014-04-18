#include "sd_card.h"

char database_name[12];
int version = 0;

void initSD(){
	int connected = 0;
	alt_up_sd_card_dev *device_reference = NULL;
	device_reference = alt_up_sd_card_open_dev(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME);
	if (device_reference != NULL) {

			if ((connected == 0) && (alt_up_sd_card_is_Present())) {
				//printf("Card connected. \n");
				//printf("fat16: %d",alt_up_sd_card_is_FAT16());
				if (alt_up_sd_card_is_FAT16()); //printf("FAT16 file system detected. \n");
				else printf("Unknown file system. \n");
				connected = 1;
			} else if ((connected == 1) && (alt_up_sd_card_is_Present()
					== false)) {
				printf("Card disconnected. \n");
				connected = 0;
			}
	}
	findDatabaseVersion();
	setDatabase(0);
}

void findDatabaseVersion(){
	if(alt_up_sd_card_find_first("",database_name)==0){
		while(alt_up_sd_card_find_next(database_name)==0){
				//printf("%s \n",database_name);
				version++;
		}
	}
	else if (alt_up_sd_card_find_first("",database_name)== -1)
		printf("No files. \n");
}

void setDatabase(int type){
	if (type == 1)
		version++;
	if (type == 2)
		version--;
	if (version < 10)
		database_name[6] = (char)(((int)'0')+version);
	else if (version >= 10 && version <100){
		database_name[5] = (char)(((int)'0')+version/10);
		database_name[6] = (char)(((int)'0')+version%10);
	}
	else{
		database_name[4] = (char)(((int)'0')+version/100);
		database_name[5] = (char)(((int)'0')+(version/10)%10);
		database_name[6] = (char)(((int)'0')+version%10);
	}
	printf("Database: %s\n",database_name);
}

int getItems(){
	short int handler = alt_up_sd_card_fopen(database_name, false);
	if (handler == -1) return -1;
	int i=0 ,j=0; // j = index of items
	char word = alt_up_sd_card_read(handler);
	unsigned char tmp_string[10];
	item *tmp;


	while (word != EOF){
		//printf("j=%d\n", j);
		if (j == 0){
			// Get rid of UTF-8 BOM
			if (strcmp(database_name,"data000.txt") == 0){
				word = alt_up_sd_card_read(handler);
				word = alt_up_sd_card_read(handler);
				word = alt_up_sd_card_read(handler);
			}
		}
		// UPC
		for (i=0;i<UPC_NUM_CHARACTER;i++){
			//printf("word: %c\n",word);
			list[j].upc[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		list[j].upc[i] = '\0';
		//printf("upc: %s\n",list[j].upc);
		word = alt_up_sd_card_read(handler); // Pass the Comma

		// Name - up to quotation mark '"'
		i=0;
		list[j].name[i] = word;
		i++;
		word = alt_up_sd_card_read(handler);
		for (;(int)word != 34;i++){
			list[j].name[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		list[j].name[i++] = word;
		list[j].name[i] = '\0';
		word = alt_up_sd_card_read(handler);
		word = alt_up_sd_card_read(handler);
		//printf("name: %s\n",list[j].name);

/*
		// Description
		i=0;
		list[j].description[i] = word;
		i++;
		word = alt_up_sd_card_read(handler);
		for (;(int)word != 34;i++){
			list[j].description[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		list[j].description[i++] = word;
		list[j].description[i] = '\0';
		word = alt_up_sd_card_read(handler);
		word = alt_up_sd_card_read(handler);
*/
		// Price
		for (i=0; (int)word != 44 ; i++){
			tmp_string[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		tmp_string[i] = '\0';
		//printf("price_string: %s\n",price_string);
		list[j].price = atof(tmp_string);
		word = alt_up_sd_card_read(handler);
		//printf("price: %.2f\n",list[j].price);
/*
		// Category
		i=0;
		list[j].category[i] = word;
		i++;
		word = alt_up_sd_card_read(handler);
		for (;(int)word != 34;i++){
			list[j].category[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		list[j].category[i++] = word;
		list[j].category[i] = '\0';
		word = alt_up_sd_card_read(handler);
		word = alt_up_sd_card_read(handler);
*/
		// Stock
		for (i=0;(int)word != 10;i++){
			//printf("word: %c\n",word);
			tmp_string[i] = word;
			word = alt_up_sd_card_read(handler);
		}
		tmp_string[i] = '\0';
		list[j].stock = atoi(tmp_string);
		//printf("stock: %d\n",list[j].stock);

		word = alt_up_sd_card_read(handler);
		tmp=realloc(list,(j+2)*sizeof(item));

		if ( tmp != NULL )
			list=tmp;
		else {
			free(list);
			printf("Error allocating memory!\n");
			return -1;
		}

		// Next item
		j++;
	}

	total_items = j;
	printf("Total Number of Items: %d\n",total_items);
	alt_up_sd_card_fclose(handler);

	return 0;
}


item scan(unsigned char upc[]){
	item result;
	result.upc[0] = '\0';
	int i = 0;
	while (i < total_items){
		if (strcmp(list[i].upc,upc) == 0){
			result = list[i];
			//printf("Matched! Price:%.2f\n",result.price);
			break;
		}
		i++;
	}

	return result;
}


void updateDatabase(){
		setDatabase(1);
		short int handler = alt_up_sd_card_fopen(database_name, true);
		unsigned char tmp[5] = {'\0'};
		int i,j;
		printf("Updating to %s...\n",database_name);
		if (handler == -1){
			printf("Error occurred when updating database.");
		}

		// Write data into the file
		for (i=0;i<total_items;i++){
			for (j=0;j<UPC_NUM_CHARACTER;j++){
				alt_up_sd_card_write(handler, list[i].upc[j]);
			}
			alt_up_sd_card_write(handler, ',');

			for (j=0;list[i].name[j] != '\0';j++){
				alt_up_sd_card_write(handler, list[i].name[j]);
			}
			alt_up_sd_card_write(handler, ',');

			sprintf( tmp, "%.2f", list[i].price);
			for (j=0;tmp[j] != '\0';j++){
				alt_up_sd_card_write(handler, tmp[j]);
			}
			alt_up_sd_card_write(handler, ',');

			sprintf( tmp, "%d", list[i].stock);
			for (j=0;tmp[j] != '\0';j++){
				alt_up_sd_card_write(handler, tmp[j]);
			}
			alt_up_sd_card_write(handler, 13); // CR
			alt_up_sd_card_write(handler, 10); // LF
		}
		alt_up_sd_card_fclose(handler);

		printf("Database updated.\n");
}
/*
int checkEmployee(char* msg){
	char tmp[10];
	int i;
	strncpy(tmp, &msg[5],9);
	tmp[9] = '\0';
	printf("tmp:%s\n",tmp);
	for (i = 0 ; i < TOTAL_EMPLOYEE;i++){
		if (strcmp(tmp,EMPLOYEE_ID[i]) == 0){
			printf("Employee identified.\n");
			sendConfirmation(0);
			return 1;
		}
	}
	sendConfirmation(1);
	return 0;
}
*/
//***** Quantity should be checked when inputing it
void transaction(unsigned char type,unsigned char msg[]){
	int status;
	int quantity;
	item msg_item;
	// A-upc,name,price,stock,description,imagelink,category
	if (type == 'A'){
		status = checkItem(msg,-1);
		if (status == 0){
			msg_item = getItemFromMsg(msg);
			sendMsg(concat("A,",msg));
			//status = receiveMsg(0);
			status = 1;
			if (status == 1){
				addItem(msg_item);
				updateDatabase();
			}
				else{
				printMsg("Transaction failed!",0,30);
				printf("Transaction failed!\n\n");
			}
		}
		else{
			printMsg("Cannot add the item!",0,30);
			printf("Cannot add the item!\n\n");
		}
		wait(30000000);
	}
	// B-upc,quantity
	else if (type == 'B'){
		quantity = getQuantityFromMsg(msg);
		status = checkItem(msg,quantity);

		if (status == 1){
			sendMsg(concat("B,",msg));
			//status = receiveMsg(0);
			status = 1;
			if (status == 1){
				buyItem(msg_item,quantity);
				updateDatabase();
			}
			else{
				printMsg("Transaction failed!",0,30);
				printf("Transaction failed!\n\n");
			}
		}
		else{
			printMsg("Cannot buy the item!",0,30);
			printf("Cannot buy the item!\n\n");
		}
		wait(30000000);
	}
	// C-upc,name,price,stock,description,imagelink
	else if (type == 'C'){
		status = checkItem(msg,-1);
		if (status == 1){
			msg_item = getItemFromMsg(msg);
			sendMsg(concat("C,",msg));
			//status = receiveMsg(0);
			status = 1;
			if (status == 1){
				changeItem(msg_item);
				msg_item = scan(msg_item.upc);
				printf("Price after:%.2f\n",msg_item.price);
				updateDatabase();
			}
			else{
				printMsg("Transaction failed!",0,30);
				printf("Transaction failed!\n\n");
			}
		}
		else{
			printMsg("Cannot change the item's information!",0,30);
			printf("Cannot change the item's information!\n\n");
		}
		wait(30000000);
	}
}

item getItemFromMsg(unsigned char msg[]){
	int i;
	int name_length = 1;
	int stock_length = 1;
	item msg_item;
	unsigned char tmp[150];
	unsigned char price_str[5];
	unsigned char stock_str[6];


	strncpy(msg_item.upc, msg,UPC_NUM_CHARACTER);
	msg_item.upc[UPC_NUM_CHARACTER] = '\0';
	printf("msg_item's UPC:%s\n",msg_item.upc);

	for (i=UPC_NUM_CHARACTER+2; msg[i] != '"'; i++)
		name_length++;
	name_length++;

	strncpy(msg_item.name, &msg[UPC_NUM_CHARACTER+1],name_length);
	msg_item.name[name_length] = '\0';
	printf("msg_item's name:%s\n",msg_item.name);

	strncpy(price_str, &msg[UPC_NUM_CHARACTER+1+name_length+1],4);
	price_str[4] = '\0';
	msg_item.price = atof(price_str);
	printf("msg_item's price:%.2f\n",msg_item.price);

	for (i=UPC_NUM_CHARACTER+1+name_length+1+5; msg[i] != ','; i++)
		stock_length++;
	strncpy(stock_str, &msg[UPC_NUM_CHARACTER+1+name_length+1+5],stock_length);
	stock_str[stock_length] = '\0';
	msg_item.stock = atoi(stock_str);
	printf("msg_item's stock:%d\n",msg_item.stock);

	return msg_item;
}


// If quantity > 0 , return 1 if result.stock >= quantity, return 0 if item doesn't exist or doesn't have enough quantity
// else , quantity <= 0,  return 1 if item exists, 0 if not

int checkItem(unsigned char msg[],int quantity){
	item result = scan(getUPC(msg));
	// Does Not Exist
	if (result.upc[0] == '\0'){
		printMsg("Item does not exist!",0,22);
		printf("Item does not exist!\n");
		return 0;
	}
	else{
	// Exist
		printMsg("Item exists!",0,22);
		printf("Item exists!\n");
		if (quantity > 0){
			//printf("item's stock:%d\n",result.stock);
			if (result.stock >= quantity){
				printMsg("There are enough items in the stock!",0,24);
				printf("There are enough items in the stock!\n");
				return 1;
			}
			else{
				printMsg("There are not enough items in the stock!",0,24);
				printf("There are not enough items in the stock!\n");
				return 0;
			}
		}
		else
			return 1;
	}
}

int getQuantityFromMsg(unsigned char msg[]){
	int quantity;
	unsigned char quantity_str[10];
	strncpy(quantity_str, &msg[UPC_NUM_CHARACTER+1],strlen(msg)-(UPC_NUM_CHARACTER+1));
	quantity_str[strlen(msg)-(UPC_NUM_CHARACTER+1)] = '\0';
	//printf("quantity_str:%s\n",quantity_str);

	quantity = atoi(quantity_str);
	//printf("quantity:%d\n",quantity);
	return quantity;
}


int addItem(item msg_item){
		printf("Adding a new item...\n");

		item *tmp=realloc(list,(total_items+1)*sizeof(item));

		if ( tmp != NULL ){
			tmp[total_items] = msg_item;
			list=tmp;
			total_items++;
			printMsg("The new item is added!",0,30);
			printf("The new item is added!\n\n");
			return 1;
		}
		else {
			free(list);
			printf("Error allocating memory!\n\n");
			return 0;
		}
}

int buyItem(item to_decrease,int quantity){
	int i = 0;
	for (i=0;i<total_items;i++){
		if (strcmp(list[i].upc,to_decrease.upc) == 0){
			list[i].stock -= quantity;
			printMsg("The item is bought!",0,30);
			printf("Number of the item in the stock is decreased!\n\n");
			return 1;
		}
	}
	return 0;
}


int changeItem(item msg_item){
	int i;

	for (i=0;i<total_items;i++){
		if (strcmp(msg_item.upc,list[i].upc) == 0){
			list[i] = msg_item;
			printMsg("Item's info is changed!",0,30);
			printf("Item's info is changed!\n\n");
			return 1;
		}
	}

	return 0;
}

void showChanges(){
	item items[total_items];
	int i,j;
	unsigned char tmp[20];
	for (i=0,j=0 ; i < total_items ; i++){
		if (list[i].stock != 10){
			items[j] = list[i];
			/*
			printf("name:%s\n",list[i].name);
			printf("stock:%d\n",list[i].stock);
			printf("stock difference:%d\n",10-list[i].stock);
			printMsg(list[i].name,20,8+j*2);
			sprintf(tmp, "%d", 10-list[i].stock);
			printMsg(tmp,30,8+j*2);*/
			j++;
		}
	}
	items[j].name[0] = '\0';

	sortItems(items);
	printMsg("Top 5 Sales of last week:",20,6);
	for (i=0;i < 5;i++){
		printf("name:%s\n",items[i].name);
		printf("stock:%d\n",items[i].stock);
		printf("stock difference:%d\n",10-items[i].stock);
		printMsg(items[i].name,20,8+j*2);
		sprintf(tmp, "%d", 10-items[i].stock);
		printMsg(tmp,30,8+j*2);
	}
}

void sortItems(item items[]){
	int i;
	for (i=0;items[i].name[0] != '\0' ; i++){
		// sort() function
		i++;
	}
}

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}



char* getUPC(unsigned char *msg){
    unsigned char *upc = malloc(UPC_NUM_CHARACTER+1);//+1 for the zero-terminator
	strncpy(upc, msg,UPC_NUM_CHARACTER);
	upc[UPC_NUM_CHARACTER] = '\0';

    return upc;
}

void wait(int s){
	int i;
	for (i = 0 ; i < s ; i++);
}
