#include "../include/ansi_wrapper.hpp"
#include "../include/levenshtein.h"
#include "../include/util.hpp"
#include "../include/sources.hpp"
#include "../include/input_handler.hpp"
#include <chrono>
#include <locale>
#include <vector>
#include <iostream>
#include <fstream>

int          width;
int          height;
int          col_width;
Sources      sources;
InputHandler input_handler;

void draw_sources(int offset) {
    if (offset >= height) {
        offset = height - 1;
    }

    int end = sources.size() > height - 1 ? height - 1 : sources.size();
    for (int i = 0; i < end; i++) {
        if (i == offset) {
            set_color(15);
            // style = curses.A_BOLD | curses.color_pair(4)
        }
        else if (sources.get(i)->getColor() == -1) {
            reset_color();
        }
        else {
            set_color(sources.get(i)->getColor());
        }
        move(0, height - i - 2);
        printf("%s", sources.get(i)->getName().c_str());
    }

    int x = col_width + 5;
    int w = width - x;
    if (w >= 15) {
        for (int i = 0; i < end; i++) {
            if (i == offset) {
                set_color(15);
            }
            else {
                set_color(8);
            }
            if (sources.get(i)->getPath().size() > w) {
                const char *str = sources.get(i)->getPath().c_str();
                int idx = sources.get(i)->getPath().size() - w + 1;
                while (str[idx] != '/') {
                   idx++;
                }
                move(width - sources.get(i)->getPath().size()
                        + idx - 1, height - i - 2);
                printf("…%s", str + idx);
            }
            else {
                move(x, height - i - 2);
                printf("%*s\n", w, sources.get(i)->getPath().c_str());
            }
        }
    }
}

void draw_query(std::string query) {
    reset_color();
    move(0, height - 1);

    if (query.size() + 3 >= width) {
        int idx = query.size() - width + 3;
        printf("> %.*s", width - 3, query.c_str() + idx);
    }
    else {
        printf("> %.*s", (int)query.size(), query.c_str());
    }
}

void draw_screen() {
    clear_term();
    draw_sources(input_handler.get_offset());
    draw_query(input_handler.get_query());
}

void on_resize(int w, int h) {
    width = w;
    height = h;
    draw_screen();
}

int main(int argc, char **argv) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    char *src_path = argv[1];
    char *dest_path = argv[2];

    if (!sources.read(src_path)) {
        fprintf(stderr, "Error reading %s\n", src_path);
        return 1;
    }

    col_width = sources.get_largest_source_length();

    register_resize_callback(on_resize);
    init_term();

    while (input_handler.is_active()) {
        sources.sort(input_handler.get_query());
        draw_screen();

        char input_c;
        read(1, &input_c, 1);
        input_handler.handle(input_c);
    }

    if (input_handler.is_selected()) {
        std::ofstream file;
        file.open(dest_path);
        file << sources.get(input_handler.get_offset())->getPath();
        file.close();
    }

    return 0;
}

