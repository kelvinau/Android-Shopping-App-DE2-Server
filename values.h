#ifndef VALUES_INCLUDE_H
#define VALUES_INCLUDE_H

#define UPC_NUM_CHARACTER 12
#define TOTAL_EMPLOYEE 2
#define TOTAL_ITEMS 278

static char const EMPLOYEE_ID[TOTAL_EMPLOYEE][10] ={
		"123456789",
		"111111111"
};


typedef struct _item_{
	unsigned char upc[UPC_NUM_CHARACTER+1];
	unsigned char name[200];
	//unsigned char description[50];
	double price;
	//unsigned char category[50];
	int stock;
} item;
#endif
extern item* list;
extern int total_items;
