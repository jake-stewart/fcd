#include "../include/ansi_wrapper.hpp"
#include "../include/util.hpp"
#include "../include/sources.hpp"
#include "../include/options.hpp"
#include "../include/input_handler.hpp"
#include "../include/project_source_parser.hpp"
#include "../include/directory_source_parser.hpp"

#include <fstream>

const int MIN_INFO_WIDTH = 25;
const int MIN_INFO_SPACING = 5;
const int MAX_INFO_WIDTH = 60;

int          width;
int          height;
Sources      sources;
int          n_visible_sources;
InputHandler input_handler;

void drawSources(int offset) {
    if (offset >= height) {
        offset = height - 1;
    }

    n_visible_sources = sources.size() > height - 1
        ? height - 1
        : sources.size();

    int col_width = 0;
    for (int i = 0; i < n_visible_sources; i++) {
        if (sources.hasHeuristic() &&
                sources.get(i).getHeuristic() == INT_MAX) {
            n_visible_sources = i;
            break;
        }

        std::string name = sources.get(i).getName();

        if (name.size() > col_width) {
            col_width = name.size();
        }
        if (i == offset) {
            set_color(15);
            // style = curses.A_BOLD | curses.color_pair(4)
        }
        else if (sources.get(i).getColor() == -1) {
            reset_color();
        }
        else {
            set_color(sources.get(i).getColor());
        }
        move(0, height - i - 2);
        printf("%s", name.c_str());
    }

    int x = col_width + MIN_INFO_SPACING;
    int w = width - x;
    if (w > MAX_INFO_WIDTH) {
        x += w - MAX_INFO_WIDTH;
        w = MAX_INFO_WIDTH;
    }

    if (w >= MIN_INFO_WIDTH) {
        for (int i = 0; i < n_visible_sources; i++) {
            if (sources.hasHeuristic() &&
                    sources.get(i).getHeuristic() == INT_MAX) {
                break;
            }
            if (i == offset) {
                set_color(15);
            }
            else {
                set_color(8);
            }
            if (sources.get(i).getPath().size() > w) {
                const char *str = sources.get(i).getPath().c_str();
                int start_idx = sources.get(i).getPath().size() - w + 1;
                int idx = start_idx;
                while (str[idx] != '/') {
                   idx++;
                   if (idx == sources.get(i).getPath().size()) {
                       idx = start_idx;
                       break;
                   }
                }
                move(width - sources.get(i).getPath().size()
                        + idx - 1, height - i - 2);
                printf("…%s", str + idx);
            }
            else {
                move(x, height - i - 2);
                printf("%*s\n", w, sources.get(i).getPath().c_str());
            }
        }
    }
}

void drawQuery(std::string query) {
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

void drawScreen() {
    clear_term();
    drawSources(input_handler.getOffset());
    drawQuery(input_handler.getQuery());
}

void onResize(int w, int h) {
    width = w;
    height = h;
    drawScreen();
}

int main(int argc, char **argv) {
    Options options;
    options.parseArgs(argc, argv);
    remove(options.getOutputPath().c_str());

    SourceParser* source_parser;
    std::string cwd;

    if (options.isProject()) {
        source_parser = new ProjectSourceParser(
                options.getConfigPath(), options.getProjectPath());
    }
    else {
        source_parser = new DirectorySourceParser(options.getConfigPath());
    }

    int status = source_parser->readSources();
    if (status) {
        return status;
    }

    sources = source_parser->getSources();

    register_resize_callback(onResize);
    init_term();

    while (input_handler.isActive()) {
        sources.sort(input_handler.getQuery());
        input_handler.setOffsetRange(n_visible_sources);

        drawScreen();

        char input_c;
        read(1, &input_c, 1);
        input_handler.handle(input_c);
    }

    if (input_handler.isSelected() && n_visible_sources) {
        sources.updateHistory(input_handler.getOffset());
        source_parser->writeHistory(sources);
        std::ofstream file;
        file.open(options.getOutputPath());
        file << sources.get(input_handler.getOffset()).getPath();
        file.close();
    }

    return 0;
}

