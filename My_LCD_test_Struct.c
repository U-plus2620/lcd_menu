#define BOUNCE 200
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
#include "menu.h"
#include <avr/interrupt.h>

#define MILK   PD5
#define MEAT   PD6
#define UP     PD2 
#define DOWN   PD3
#define SELECT PD4

int DeBounce(int Pin_reg, int Pin){
    if(bit_is_clear(Pin_reg,Pin)) _delay_ms(BOUNCE);
    if(bit_is_clear(Pin_reg,Pin)) return 1;
    else return 0;
}

uint8_t pointer = 0;
uint8_t dimmer  = 0;
item *current_menu;
int current_menu_size;
enum selection{milk,meat,RGB,on,off,dim,red,green,blue,white,back}selection;

// Various menus in the form of lists
    char *main_menu[] = {"milk sink","meat sink","RGB's(INOP)"};
    int main_menu_size;// = sizeof(main_menu) / sizeof(main_menu[0]);
    // item menuBuff[main_menu_size];
    int main_menu_actions[] = {0,1,2};
    item *_main_menu;// = Menu_maker(main_menu,main_menu_actions,main_menu_size,menuBuff);

    char *sink_menu[] = {"on","off","dim","back"};
    int sink_size;// = sizeof(sink_menu) / sizeof(sink_menu[0]);
    // item sinkBuff[sink_size];
    int sink_actions[] = {3,4,5,10};
    item *milky_sink_menu;// = Menu_maker(sink_menu,sink_actions,sink_size,sinkBuff);
    item *meaty_sink_menu;// = Menu_maker(sink_menu,sink_actions,sink_size,sinkBuff);

    char *RGB_menu[] = {"on","off","dim","red","grn","blu","wht","back"};
    int RGB_size;// = sizeof(RGB_menu) / sizeof(RGB_menu[0]);
    // item RGBBuff[RGB_size];
    int RGB_actions[] = {3,4,5,6,7,8,9,10}; 
    item *_RGB_menu;// = Menu_maker(RGB_menu,RGB_actions,RGB_size,RGBBuff);
    
   

   
ISR (PCINT2_vect){
    if (DeBounce(PIND,UP)){ //Scroll up
            pointer = Scroll(1,0,pointer);
            Show(current_menu,pointer,current_menu_size);
        }
        else if (DeBounce(PIND,DOWN)){ //Scroll down
            pointer = Scroll(0,1,pointer);
            Show(current_menu,pointer,current_menu_size);
        }
        else if (DeBounce(PIND,SELECT)){ //Select item
      
        selection = Select(current_menu,pointer,current_menu_size);
           switch(selection){
                case milk:
                    lcd_clrscr();
                    lcd_home();
                    _delay_ms(100);
                    current_menu = milky_sink_menu;
                    current_menu_size = sink_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case meat:
                    lcd_clrscr();
                    lcd_home();
                    _delay_ms(100);
                    current_menu = meaty_sink_menu;
                    current_menu_size = sink_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case RGB:
                    lcd_clrscr();
                    lcd_home();
                    _delay_ms(100);
                    current_menu = _RGB_menu;
                    current_menu_size = RGB_size;
                    Show(current_menu, pointer,current_menu_size);
                    break;
                case on:
                    if(current_menu == milky_sink_menu){
                        TCCR0A |= (1<<COM0B1);
                        OCR0B = 0xFF;
                    }
                    else if (current_menu == meaty_sink_menu){
                        TCCR0A |= (1<<COM0A1);
                        OCR0A = 0xFF;
                    }
                    // else if (current_menu == _RGB_menu){
                    //      send on signal
                    // }
                    break;
                case off:
                     if(current_menu == milky_sink_menu){
                        TCCR0A &= ~(1<<COM0B1);
                        PORTD &= ~(1<< MILK);
                    }
                    else if (current_menu == meaty_sink_menu){
                       TCCR0A &= ~(1<<COM0A1);
                       PORTD &= ~(1<< MEAT);
                    }
                     // else if (current_menu == _RGB_menu){
                    //      send off signal
                    // }
                    break;
                case dim:
                        lcd_clrscr();
                        char intbuff[10];
                        itoa((dimmer/(float)0)*100,intbuff,10);
                        lcd_puts("DIM\n");
                        lcd_puts(intbuff);
                        lcd_putc('%');
                        do{
                            if(DeBounce(PIND,UP)){
                                dimmer += (dimmer!=255) ? 51 : 1; 
                                itoa((dimmer/(float)255)*100,intbuff,10);
                                lcd_clrscr();
                                lcd_puts("DIM\n");
                                lcd_puts(intbuff);
                                lcd_putc('%');
                                }
                            else if (DeBounce(PIND,DOWN)){
                                dimmer -= (dimmer!=0) ? 51 : 1; 
                                itoa((dimmer/(float)255)*100,intbuff,10);
                                lcd_clrscr();
                                lcd_puts("DIM\n");
                                lcd_puts(intbuff);
                                lcd_putc('%');
                                }
                            if(current_menu == milky_sink_menu){
                                OCR0B = dimmer;
                                }
                            else if (current_menu == meaty_sink_menu){
                                OCR0A = dimmer;
                                }
                             // else if (current_menu == _RGB_menu){
                             //      send dim signal
                             // }
                        }while (!DeBounce(PIND, SELECT));
                        Show(current_menu,pointer,current_menu_size);
                    break;
                case red:
                    lcd_clrscr();
                    lcd_home();
                    lcd_puts("INOP");
                    _delay_ms(100);
                    Show(current_menu,pointer,current_menu_size);
                    break;
                case green:
                    lcd_clrscr();
                    lcd_home();
                    lcd_puts("INOP");
                    _delay_ms(3000);
                    Show(current_menu,pointer,current_menu_size);
                    break;
                case blue:
                    lcd_clrscr();
                    lcd_home();
                    lcd_puts("INOP");
                    _delay_ms(3000);
                    Show(current_menu,pointer,current_menu_size);
                    break;
                case white:
                    lcd_clrscr();
                    lcd_home();
                    lcd_puts("INOP");
                    _delay_ms(3000);
                    Show(current_menu,pointer,current_menu_size);
                    break;
                case back:
                    current_menu = _main_menu;
                    current_menu_size = main_menu_size;
                    Show(current_menu, pointer,current_menu_size);
                default:
                    break;
                }
        }   

}
int main(void){ //Super loop
    
    //Pin configurations
    DDRD  &= ~(1 << UP) | ~(1<<DOWN) | ~(1<<SELECT);        /* Pin PD2 input              */
    PORTD |=  (1 << UP);          /* Pin PD2 pull-up enabled    */
    PORTD |=  (1 << DOWN);          /* Pin PD3 pull-up enabled    */
    PORTD |=  (1 << SELECT);          /* Pin PD4 pull-up enabled    */
    DDRD  |=  (1<<MILK) | (1<<MEAT); /*output pin*/   
    PORTD &= ~(1<<MILK) | ~(1<<MEAT); /*output pin*/
    
    PCICR |= (1<<PCIE2);
    PCMSK2 |= (1<<PCINT18) | (1<<PCINT19) | (1<<PCINT20);

    //Initialize timer for dimming
    TCCR0A  |= (1<<WGM00) | (1<<WGM01); //(1<<COM0B1) | (1<<COM0A1) |
    TCCR0B  |= (1<<CS00);
    
    // // Various menus in the form of lists
    // char *main_menu[] = {"milk sink","meat sink","RGB's(INOP)"};
     int main_menu_size = sizeof(main_menu) / sizeof(main_menu[0]);
     item menuBuff[main_menu_size];
    // int main_menu_actions[] = {0,1,2};
    /*item **/_main_menu = Menu_maker(main_menu,main_menu_actions,main_menu_size,menuBuff);

    // char *sink_menu[] = {"on","off","dim","back"};
     int sink_size = sizeof(sink_menu) / sizeof(sink_menu[0]);
     item sinkBuff[sink_size];
    // int sink_actions[] = {3,4,5,10};
    /* item**/ milky_sink_menu = Menu_maker(sink_menu,sink_actions,sink_size,sinkBuff);
    /* item**/ meaty_sink_menu = Menu_maker(sink_menu,sink_actions,sink_size,sinkBuff);

    // char *RGB_menu[] = {"on","off","dim","red","grn","blu","wht","back"};
     int RGB_size = sizeof(RGB_menu) / sizeof(RGB_menu[0]);
     item RGBBuff[RGB_size];
    // int RGB_actions[] = {3,4,5,6,7,8,9,10}; 
    /* item**/ _RGB_menu = Menu_maker(RGB_menu,RGB_actions,RGB_size,RGBBuff);
    
    //Initialization of the screen and main program
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_home();
    lcd_puts("Light controller\n");
    lcd_puts("V3.0");
    _delay_ms(500);
    lcd_clrscr();
    lcd_home();
    lcd_puts("hello there! how can");
    lcd_puts("\n");
    lcd_puts("i help you?");
    _delay_ms(500 );

    sei();
    
    current_menu = _main_menu;
    current_menu_size = main_menu_size;
    Show(current_menu, pointer,current_menu_size);

    while(1){
        // if (DeBounce(PIND,UP)){ //Scroll up
        //     pointer = Scroll(1,0,pointer);
        //     Show(current_menu,pointer,current_menu_size);
        // }
        // else if (DeBounce(PIND,DOWN)){ //Scroll down
        //     pointer = Scroll(0,1,pointer);
        //     Show(current_menu,pointer,current_menu_size);
        // }
        // else if (DeBounce(PIND,SELECT)){ //Select item
      
        // selection = Select(current_menu,pointer,current_menu_size);
        //    switch(selection){
        //         case milk:
        //             lcd_clrscr();
        //             lcd_home();
        //             _delay_ms(100);
        //             current_menu = milky_sink_menu;
        //             current_menu_size = sink_size;
        //             Show(current_menu, pointer,current_menu_size);
        //             break;
        //         case meat:
        //             lcd_clrscr();
        //             lcd_home();
        //             _delay_ms(100);
        //             current_menu = meaty_sink_menu;
        //             current_menu_size = sink_size;
        //             Show(current_menu, pointer,current_menu_size);
        //             break;
        //         case RGB:
        //             lcd_clrscr();
        //             lcd_home();
        //             _delay_ms(100);
        //             current_menu = _RGB_menu;
        //             current_menu_size = RGB_size;
        //             Show(current_menu, pointer,current_menu_size);
        //             break;
        //         case on:
        //             if(current_menu == milky_sink_menu){
        //                 TCCR0A |= (1<<COM0B1);
        //                 OCR0B = 0xFF;
        //             }
        //             else if (current_menu == meaty_sink_menu){
        //                 TCCR0A |= (1<<COM0A1);
        //                 OCR0A = 0xFF;
        //             }
        //             // else if (current_menu == _RGB_menu){
        //             //      send on signal
        //             // }
        //             break;
        //         case off:
        //              if(current_menu == milky_sink_menu){
        //                 TCCR0A &= ~(1<<COM0B1);
        //                 PORTD &= ~(1<< MILK);
        //             }
        //             else if (current_menu == meaty_sink_menu){
        //                TCCR0A &= ~(1<<COM0A1);
        //                PORTD &= ~(1<< MEAT);
        //             }
        //              // else if (current_menu == _RGB_menu){
        //             //      send off signal
        //             // }
        //             break;
        //         case dim:
        //                 lcd_clrscr();
        //                 char intbuff[10];
        //                 itoa((dimmer/(float)0)*100,intbuff,10);
        //                 lcd_puts("DIM\n");
        //                 lcd_puts(intbuff);
        //                 lcd_putc('%');
        //                 do{
        //                     if(DeBounce(PIND,UP)){
        //                         dimmer += (dimmer!=255) ? 51 : 1; 
        //                         itoa((dimmer/(float)255)*100,intbuff,10);
        //                         lcd_clrscr();
        //                         lcd_puts("DIM\n");
        //                         lcd_puts(intbuff);
        //                         lcd_putc('%');
        //                         }
        //                     else if (DeBounce(PIND,DOWN)){
        //                         dimmer -= (dimmer!=0) ? 51 : 1; 
        //                         itoa((dimmer/(float)255)*100,intbuff,10);
        //                         lcd_clrscr();
        //                         lcd_puts("DIM\n");
        //                         lcd_puts(intbuff);
        //                         lcd_putc('%');
        //                         }
        //                     if(current_menu == milky_sink_menu){
        //                         OCR0B = dimmer;
        //                         }
        //                     else if (current_menu == meaty_sink_menu){
        //                         OCR0A = dimmer;
        //                         }
        //                      // else if (current_menu == _RGB_menu){
        //                      //      send dim signal
        //                      // }
        //                 }while (!DeBounce(PIND, SELECT));
        //                 Show(current_menu,pointer,current_menu_size);
        //             break;
        //         case red:
        //             lcd_clrscr();
        //             lcd_home();
        //             lcd_puts("INOP");
        //             _delay_ms(100);
        //             Show(current_menu,pointer,current_menu_size);
        //             break;
        //         case green:
        //             lcd_clrscr();
        //             lcd_home();
        //             lcd_puts("INOP");
        //             _delay_ms(3000);
        //             Show(current_menu,pointer,current_menu_size);
        //             break;
        //         case blue:
        //             lcd_clrscr();
        //             lcd_home();
        //             lcd_puts("INOP");
        //             _delay_ms(3000);
        //             Show(current_menu,pointer,current_menu_size);
        //             break;
        //         case white:
        //             lcd_clrscr();
        //             lcd_home();
        //             lcd_puts("INOP");
        //             _delay_ms(3000);
        //             Show(current_menu,pointer,current_menu_size);
        //             break;
        //         case back:
        //             current_menu = _main_menu;
        //             current_menu_size = main_menu_size;
        //             Show(current_menu, pointer,current_menu_size);
        //         default:
        //             break;
        //         }
        // }   
    }
}