#include "../include/ansi_wrapper.hpp"

#define ANSI_ESC "\x1b["

void move(int x, int y) {
    printf(ANSI_ESC "%u;%uH", y + 1, x + 1);
}

void move_home() {
    printf(ANSI_ESC "H");
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
    printf(ANSI_ESC "38;2;%u;%u;%um", r, g, b);
}

void set_color(int code) {
    printf(ANSI_ESC "38;5;%dm", code);
}

void reset_color() {
    printf(ANSI_ESC "0m");
}

void set_alternate_buffer(bool value) {
    printf(ANSI_ESC "?1049%c", value ? 'h' : 'l');
}

void clear_term() {
    printf(ANSI_ESC "2J");
}

void set_cursor(bool value) {
    printf(ANSI_ESC "?25%c", value ? 'h' : 'l');
}

int start_x, start_y;

void restore_term(void) {
    set_alternate_buffer(true);       // enter alternate buffer
    clear_term();                     // clean up the buffer
    set_cursor(true);                 // show the cursor
    set_alternate_buffer(false);      // return to the main buffer
    // move(start_x, start_y);
    tcsetattr(STDIN_FILENO, TCSANOW, &initial);  // restore original termios params
    // printf("\n");
}

void on_sig_term(int i) {
    exit(1);
    // since atexit has already registered a handler,
    // a call to exit(3) is all we actually need
}

void on_sig_resize(int i) {
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    if (resize_callback != nullptr)
        resize_callback(ws.ws_col, ws.ws_row);
}

struct termios orig_termios;

void die(const char *s) {
  perror(s);
  exit(1);
}

void disable_raw_mode() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
}

void enable_raw_mode() {
    // set_alternate_buffer(true);       // enter alternate buffer
    // clear_term();                     // clean up the buffer
    // set_cursor(true);                 // show the cursor
    // set_alternate_buffer(false);      // return to the main buffer

    tcgetattr(STDIN_FILENO, &orig_termios);
    // atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void init_term(void) {
    // since we're using printf here, which doesn't play nicely
    // with non-canonical mode, we need to turn off buffering.
    // setvbuf(stdout, NULL, _IONBF, 0);

    enable_raw_mode();
    atexit(disable_raw_mode);

    signal(SIGWINCH, on_sig_resize);
    signal(SIGTERM, on_sig_term);
    signal(SIGINT, on_sig_term);

    set_alternate_buffer(true);
    clear_term();
    move_home();
    on_sig_resize(0);
}

void register_resize_callback(void (*func)(int, int)) {
    resize_callback = func;
}

// credit: https://stackoverflow.com/a/50888457
int get_pos(int *x, int *y) {
    char buf[30]={0};
    int ret, i, pow;
    char ch;

    *x = 0;
    *y = 0;

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for( i = 0, ch = 0; ch != 'R'; i++ )
    {
        ret = read(0, &ch, 1);

        if ( !ret ) {
            tcsetattr(0, TCSANOW, &restore);
            return 1;
        }
        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        return(1);
    }

    for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + ( buf[i] - '0' ) * pow;

    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + ( buf[i] - '0' ) * pow;

    tcsetattr(0, TCSANOW, &restore);
    return 0;
}
