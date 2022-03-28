#ifndef ANSI_WRAPPER_H
#define ANSI_WRAPPER_H

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

static termios initial;
static void (*resize_callback)(int, int) = nullptr;

void register_resize_callback(void (*func)(int, int));

void move(int x, int y);
void move_home();

void set_color(uint8_t r, uint8_t g, uint8_t b);
void set_color(int code);
void reset_color();

void clear_term();

void set_alternate_buffer(bool value);
void set_cursor(bool value);

void init_term(void);
void restore_term(void);
void on_sig_term(int i);
void on_sig_resize(int i);

#endif
