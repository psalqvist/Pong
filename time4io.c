#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
int btns;
int sw;


int getsw() {

  sw = ((PORTD & 0x0f00) >> 8);       // mask bits 8 through 11, shift to LSB
  return sw;
}
