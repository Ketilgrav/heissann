#ifndef IO_H
#define IO_H
#include "channels.h"

#include <math.h>
#include <comedilib.h>


// Returns 0 on init failure

int io_init(void);

void io_set_bit(int channel);
void io_clear_bit(int channel);
void io_write_bit(int channel, int val);

int io_read_bit(int channel);

int io_read_analog(int channel);
void io_write_analog(int channel, int value);

#endif
