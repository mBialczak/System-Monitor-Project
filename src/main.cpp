#include <iostream>  // TODO: remove

#include "ncurses_display.h"
#include "system.h"
int main() {
  System system;
  NCursesDisplay::Display(system);
}