#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>
#include <altera_up_sd_card_avalon_interface.h>
#include <time.h>
#include "values.h"
#include <unistd.h>

void initSD(void);
void findDatabaseVersion(void);
void setDatabase(int);

int getItems();
item scan(unsigned char[]);
void updateDatabase();

void transaction(unsigned char,unsigned char[]);

item getItemFromMsg(unsigned char[]);
int checkItem(unsigned char[],int);
int getQuantityFromMsg(unsigned char[]);

int checkEmployee(char*);

int addItem(item);
int buyItem(item,int);
int changeItem(item);

void showChanges();
void sortItems(item[]);


char* concat(char*,char*);
char* getUPC(unsigned char[]);

void wait(int);
