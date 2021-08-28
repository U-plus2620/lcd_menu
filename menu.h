#ifndef M_1545_MENU_H
#define M_1545_MENU_H
 /*
    ###########################################################################################################################
   # This file provides functions for a specific implementation of a menu used on an LCD part number hd44780.                  #
   # Most functions accept an array containing pointers to structs of type Item.                                               #      
   # The Item struct contains a "item_name" string variable and "action" int variable.                                         #
   # The "item_name" is used to present the menu item to be printed to the screen.                                             #
   # An "action" interger is to be returned to a switch-case which will select an action,                                      #
   # an enum containing diferrent selection names is used in conjunction with the switch-case to make a decision.              #  
    ########################################################################################################################### 
 */
#define MAX_NAME_SIZE 16

 typedef struct _Item{
        char item_name[MAX_NAME_SIZE];
        int action;
        //int output;
    }item;
    
item *Menu_maker(char *buffer[],int *action,int size, item *target);
char *_Item(item _menu[], int pointer);
void Show(item _menu[], int Pointer, int menu_size);
int Scroll(int up, int down, int Pointer);
int Select(item _menu[], int Pointer, int menu_size);
int Wrap(item _menu,int Pointer);

#endif

