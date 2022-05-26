#pragma once

struct Menu_item{
  char *title;
  void (*fn_ptr)();
} ;

void start_menu(Menu_item menu_items[], int size);
