#define BOUNCE 200
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
#include "menu.h"


int DeBounce(int Pin_reg, int Pin){
    if(bit_is_clear(Pin_reg,Pin)) _delay_ms(BOUNCE);
    if(bit_is_clear(Pin_reg,Pin)) return 1;
    else return 0;
}

int main(void){ //Super loop
    
    //Pin configurations
    DDRD  &= ~(1 << PD2) | ~(1<<PD3) | ~(1<<PD4);        /* Pin PD2 input              */
    PORTD |=  (1 << PD2);        /* Pin PD2 pull-up enabled    */
    PORTD |=  (1 << PD3);        /* Pin PD3 pull-up enabled    */
    PORTD |=  (1 << PD4);        /* Pin PD4 pull-up enabled    */
    DDRB  |=  (1<<PB7);    /* Pin connected to LED backlight*/
    PORTB &= ~(1<<PB7);

   
    
    // Various menus in the form of lists
    char *main_menu[] = {"milk sink","meat sink","RGB's"};
    int main_menu_size = sizeof(main_menu) / sizeof(main_menu[0]);
    item menuBuff[main_menu_size];
    int main_menu_actions[] = {0,1,2};
    item *_main_menu = Menu_maker(main_menu,main_menu_actions,main_menu_size,menuBuff);

    char *sink_menu[] = {"on","off","dim","back"};
    int sink_size = sizeof(sink_menu) / sizeof(sink_menu[0]);
    item sinkBuff[sink_size];
    int sink_actions[] = {3,4,5,9};
    item *_sink_menu = Menu_maker(sink_menu,sink_actions,sink_size,sinkBuff);

    char *RGB_menu[] = {"on","off","dim","red","green","blue","back"};
    int RGB_size = sizeof(RGB_menu) / sizeof(RGB_menu[0]);
    item RGBBuff[RGB_size];
    int RGB_actions[] = {3,4,5,6,7,8,9}; 
    item *_RGB_menu = Menu_maker(RGB_menu,RGB_actions,RGB_size,RGBBuff);
    
   
   

    enum selection{milk,meat,RGB,on,off,dim,red,green,blue,back}selection;

    int pointer = 0;
    item *current_menu;
    int current_menu_size;


    //Initialization of the screen and main program
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_home();
    lcd_puts("Light controller\n");
    lcd_puts("V2.0");
    _delay_ms(500);
    lcd_clrscr();
    lcd_home();
    lcd_puts("hello there! how can");
    lcd_puts("\n");
    lcd_puts("i help you?");
    _delay_ms(500 );

   

    current_menu = _main_menu;
    current_menu_size = main_menu_size;
    Show(current_menu, pointer,current_menu_size);
    
    while(1){
        if (DeBounce(PIND,PD2)){ //Scroll up
        //    PORTB |= (1<<PB7);
            pointer = Scroll(1,0,pointer);
            Show(current_menu,pointer,current_menu_size);
        }
        else if (DeBounce(PIND,PD3)){ //Scroll down
            //PORTB &= ~(1<<PB7);         
            pointer = Scroll(0,1,pointer);
            Show(current_menu,pointer,current_menu_size);
        }
        else if (DeBounce(PIND,PD4)){ //Select item
        lcd_home();
        lcd_puts("item selected");
        _delay_ms(500);
        lcd_clrscr();
        selection = Select(current_menu,pointer,current_menu_size);
           switch(selection){
                case milk:
                    lcd_home();
                    lcd_puts("milky sink");
                    _delay_ms(700);
                    current_menu = _sink_menu;
                    current_menu_size = sink_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case meat:
                    lcd_home();
                    lcd_puts("meaty sink");
                    _delay_ms(700);
                    current_menu = _sink_menu;
                    current_menu_size = sink_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case RGB:
                    lcd_home();
                    lcd_puts("RGB's");
                    _delay_ms(700);
                    current_menu = _RGB_menu;
                    current_menu_size = RGB_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case on:
                    break;
                case off:
                    break;
                case dim:
                    break;
                case red:
                    break;
                case green:
                    break;
                case blue:
                    break;
                case back:
                    lcd_home();
                    lcd_puts("Going back");
                    _delay_ms(700);
                    current_menu = _main_menu;
                    current_menu_size = main_menu_size;
                    Show(current_menu, pointer,current_menu_size);
                default:
                    break;
                }
            }
        
    }
}