#include "lcd.h"
#include "menu.h"
#include <string.h>
#include <stdlib.h>


char *_Item(volatile item _menu[], volatile uint8_t pointer){
    //This function that selects an item from the menu list (char_set) and return a string for display
    return _menu[pointer].item_name;
}

void Show(volatile item _menu[], volatile uint8_t Pointer,volatile int menu_size){
    //This function that accepts a menu list and a pointer to pick out an item from the list and present it on the display.
    //The menu_size variable is used to make the menu wrap around from end back to begining and reverse.
    lcd_clrscr();
    lcd_home();
    char *str = _Item(_menu, Pointer%(menu_size));
    lcd_puts(str);
    lcd_putc(0b01111111);
    lcd_puts("\n");
    str = _Item(_menu, ((Pointer+1)%menu_size));
    lcd_puts(str);
}

int Scroll(int up, int down, volatile uint8_t Pointer){
    //This function increments or decrements the menu the pointer variable to scroll through a menu.
    if(up == 1 && down == 0) return Pointer + 1;
    else if(up ==0 && down == 1) return Pointer - 1;
    else return 0;
    
}

int Select(volatile item _menu[], volatile uint8_t Pointer, volatile uint8_t menu_size){
    //This function returns an interger that could be used to make a decision based on an item in the
    //context of a menu (i.e. when an item is selected using perhaps a select button.)
    return _menu[Pointer%(menu_size)].action;   
}

item *Menu_maker(char *buffer[],int *action,int size,item *target){
    // item *tmp_menu;
    target = (item*)malloc(size * sizeof(item));
    for(int i=0; i<size;i++){
        strcpy ((char*)target[i].item_name, buffer[i]);
        target[i].action    =  action[i];
    }
    return target;
}