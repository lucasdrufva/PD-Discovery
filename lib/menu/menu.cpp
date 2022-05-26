#include "menu.h"

#include "pin-definitions.h"
#include "buttons.h"

// Forward declaration
class Display{
public:
    void drawMenu(Menu_item items[], int index, int length);
};

extern Display display;

Menu_item *g_current_menu;
int g_menu_index = 0;
int g_menu_size = 0;

void menu_draw()
{
    display.drawMenu(g_current_menu, g_menu_index, g_menu_size);
}

void menu_select()
{
    g_current_menu[g_menu_index].fn_ptr();
}

void menu_up()
{
    if( g_menu_index > 0)
        g_menu_index--;
    
    menu_draw();
}

void menu_down()
{
    if( g_menu_index < g_menu_size - 1 )
        g_menu_index++;
    
    menu_draw();
}

void start_menu(Menu_item menu_items[], int size)
{
    g_current_menu = menu_items;
    g_menu_size = size;

    button_register_cb(PIN_UP_2, SHORT, *menu_up);
    button_register_cb(PIN_DOWN_2, SHORT, *menu_down);
    button_register_cb(PIN_SELECT, SHORT, *menu_select);

    menu_draw();
}