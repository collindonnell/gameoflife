#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    bool alive;
} cell_t;

int main(int argc, char *argv[]) {
    int max_y = 0, max_x = 0;
    int num_rows, num_cols;
    int delay = 150000;

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, true);

    getmaxyx(stdscr, max_y, max_x);
    num_cols = max_x-2, num_rows = max_y-3;
    cell_t cells[num_cols][num_rows];

    srand((unsigned)time(NULL));
    for (int x = 0; x < num_cols; x++) {
        for (int y = 0; y < num_rows; y++) {
            bool alive = (rand() % 12) == 1;
            cells[x][y].alive = alive;
        }
    }

    int gen = 0;

    while(1) {
        clear();
        int living = 0;
        for (int x = 0; x < num_cols; x++) {
            for (int y = 0; y < num_rows; y++) {
                cell_t* cell = &cells[x][y];

                cell_t neighbors[8];
                int ncount = 0;
                for (int nx = x-1; nx < x+2; nx++) {
                    for (int ny = y-1; ny < y+2; ny++) {
                        if (nx == x && ny == y) {
                            continue;
                        }
                        if ((nx > -1 && ny > -1) && (nx < num_cols && ny < num_rows)) {
                            neighbors[ncount++] = cells[nx][ny];
                        }
                    }
                }

                int nalive = 0;
                for (int i = 0; i < ncount; i++) {
                    if (neighbors[i].alive) {
                        nalive++;
                    }
                }

                /* Any live cell with two or three live neighbors survives. */
                if (cell->alive) {
                    cell->alive = (nalive > 1 && nalive < 4);
                } else {
				/* Any dead cell with three live neighbors becomes a live cell. */
                    cell->alive = (nalive == 3);
                }
				/* All other live cells die in the next generation. Similarly, all other dead cells stay dead. */

                if (cell->alive == true) {
                    living++;
                    mvwaddch(stdscr, y+1, x+1, '*');
                }
            }
        }

        mvprintw(max_y-1, 0, "Living: %d\tGeneration: %d", living, gen);
        char* control_string = "(F)aster/(S)lower";
        mvprintw(max_y-1, max_x-(strlen(control_string)), control_string);
        usleep(delay);
        gen = gen + 1;

        refresh();

		int c = getch();
		switch (c) {
			case 'f': {
				if (delay >= 25000) {
					delay -= 25000;
				}
				break;
			}
			case 's': {
				if (delay <= 500000) {
					delay += 25000;
				}
				break;
			}
		}
    }

    endwin();
    return 0;
}
