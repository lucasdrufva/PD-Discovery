#pragma once

struct Menu_item{
  const char *title;
  void (*fn_ptr)();
} ;

void start_menu(const Menu_item menu_items[], int size);
