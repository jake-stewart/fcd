#include "../include/util.hpp"
#include "../include/sources.hpp"
#include "../include/options.hpp"
#include "../include/input_handler.hpp"
#include "../include/project_source_parser.hpp"
#include "../include/directory_source_parser.hpp"

#include <curses.h>
#include <fstream>

const int MIN_INFO_WIDTH = 25;
const int MIN_INFO_SPACING = 5;
const int MAX_INFO_WIDTH = 60;

WINDOW *window;

int          width;
int          height;
Sources      sources;
int          n_visible_sources;
InputHandler input_handler;

enum Colors {
    COLOR_SOURCE = 1,
    COLOR_SELECTED_SOURCE,
    COLOR_PATH,
    COLOR_SELECTED_PATH,
    COLOR_SEARCH,
};

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
            color_set(COLOR_SELECTED_SOURCE, nullptr);
        }
        else {
            color_set(COLOR_SOURCE, nullptr);
        }
        move(height - i - 2, 0);
        addstr(name.c_str());
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
                color_set(COLOR_SELECTED_PATH, nullptr);
            }
            else {
                color_set(COLOR_PATH, nullptr);
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
                move(height - i - 2, width - sources.get(i).getPath().size() + idx - 1);
                addstr("…");
                addstr(str + idx);
            }
            else {
                move(height - i - 2, width - sources.get(i).getPath().size());
                addstr(sources.get(i).getPath().c_str());
            }
        }
    }
}

void drawQuery(std::string query) {
    color_set(COLOR_SEARCH, nullptr);
    move(height - 1, 0);
    addstr("> ");

    if (query.size() + 3 >= width) {
        int idx = query.size() - width + 3;
        addstr(query.c_str() + idx);
    }
    else {
        addstr(query.c_str());
    }
}

void drawScreen() {
    clear();
    drawSources(input_handler.getOffset());
    drawQuery(input_handler.getQuery());
}

void onResize() {
    getmaxyx(window, height, width);
    drawScreen();
}

int main(int argc, char **argv) {
    // enable unicode
    setlocale(LC_ALL, "");

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

    window = initscr();

    noecho();

    start_color();
    use_default_colors();
    init_pair(COLOR_SOURCE, COLOR_BLUE, -1);
    init_pair(COLOR_SELECTED_SOURCE, COLOR_WHITE, -1);
    init_pair(COLOR_PATH, COLOR_BLACK + 8, -1);
    init_pair(COLOR_SELECTED_PATH, -1, -1);
    init_pair(COLOR_SEARCH, -1, -1);
    onResize();

    while (input_handler.isActive()) {
        sources.sort(input_handler.getQuery());
        input_handler.setOffsetRange(n_visible_sources);

        drawScreen();

        int input_c = getch();

        if (input_c == KEY_RESIZE) {
            onResize();
        }
        else {
            input_handler.handle(input_c);
        }
    }

    if (input_handler.isSelected() && n_visible_sources) {
        sources.updateHistory(input_handler.getOffset());
        source_parser->writeHistory(sources);
        std::ofstream file;
        file.open(options.getOutputPath());
        file << sources.get(input_handler.getOffset()).getPath();
        file.close();
    }

    endwin();
    return 0;
}

