#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int GRID_SIZE = 36;
const int GENERATIONS = 400;

const int pattern[81][2] = {
    {0, 0},   {1, 0},   {12, 0},  {13, 0},  {21, 0},  {22, 0},  {34, 0},
    {35, 0},  {0, 1},   {12, 1},  {22, 1},  {35, 1},  {7, 7},   {8, 7},
    {17, 7},  {27, 7},  {8, 8},   {9, 8},   {16, 8},  {17, 8},  {18, 8},
    {25, 8},  {26, 8},  {27, 8},  {8, 9},   {17, 9},  {26, 9},  {0, 12},
    {1, 12},  {34, 12}, {35, 12}, {0, 13},  {35, 13}, {8, 16},  {17, 16},
    {26, 16}, {7, 17},  {8, 17},  {9, 17},  {16, 17}, {17, 17}, {18, 17},
    {25, 17}, {26, 17}, {27, 17}, {8, 18},  {17, 18}, {26, 18}, {0, 21},
    {35, 21}, {0, 22},  {1, 22},  {34, 22}, {35, 22}, {8, 25},  {17, 25},
    {26, 25}, {7, 26},  {8, 26},  {9, 26},  {16, 26}, {17, 26}, {18, 26},
    {25, 26}, {26, 26}, {7, 27},  {17, 27}, {26, 27}, {27, 27}, {0, 34},
    {12, 34}, {22, 34}, {35, 34}, {0, 35},  {1, 35},  {12, 35}, {13, 35},
    {21, 35}, {22, 35}, {34, 35}, {35, 35}};

const int glider[5][2] = {{1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}};
const signed neighbours_cells_offset[8][2] = {
    {1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1}};

enum State { DEAD, LIVE };

int calc_coordinate(int coordinate, signed offset) {
  const int sum = coordinate + offset;

  if (sum == GRID_SIZE) {
    return 0;
  }
  if (sum < 0) {
    return GRID_SIZE - 1;
  }
  return sum;
}

int get_neighbours(enum State grid[GRID_SIZE][GRID_SIZE], int cell_x,
                   int cell_y) {
  int neighbours = 0;
  int x, y;

  size_t n_length =
      sizeof(neighbours_cells_offset) / sizeof(neighbours_cells_offset[0]);

  for (size_t i = 0; i < n_length; ++i) {
    x = calc_coordinate(cell_x, neighbours_cells_offset[i][0]);
    y = calc_coordinate(cell_y, neighbours_cells_offset[i][1]);

    if (grid[x][y] == LIVE) {
      neighbours++;
    }
  }

  return neighbours;
}

void new_generation(enum State grid[GRID_SIZE][GRID_SIZE]) {
  enum State buff[GRID_SIZE][GRID_SIZE];

  for (int i = 0; i < GRID_SIZE; ++i) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      const int neighbours = get_neighbours(grid, i, j);

      if (grid[i][j] == LIVE && (neighbours == 2 || neighbours == 3)) {
        buff[i][j] = LIVE;
      } else if (grid[i][j] == DEAD && neighbours == 3) {
        buff[i][j] = LIVE;
      } else {
        buff[i][j] = DEAD;
      }
    }
  }

  memcpy(grid, buff, sizeof(buff));
}

void print_grid(enum State grid[GRID_SIZE][GRID_SIZE]) {
  printf("\n");

  for (int i = 0; i < GRID_SIZE; ++i) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      printf(" %s", grid[i][j] == LIVE ? "■" : "□");
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char const *argv[]) {
  enum State grid[GRID_SIZE][GRID_SIZE];

  // Populate the grid
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; ++j) {
      int found = 0;
      for (int k = 0; k < sizeof(pattern) / (sizeof(int) * 2); ++k) {
        if (i == pattern[k][0] && j == pattern[k][1]) {
          grid[i][j] = LIVE;
          found = 1;
          break;
        }
      }
      if (found == 0) {
        grid[i][j] = DEAD;
      }
    }
  }

  // Print the initial state and wait before start
  system("clear");
  unsigned int microseconds = 10 * 100000;
  printf("\n Initial stage\n");
  print_grid(grid);
  usleep(microseconds);

  // Start printing each generation
  for (int i = 0; i < GENERATIONS; ++i) {
    new_generation(grid);

    usleep(0.05 * microseconds);
    system("clear");
    printf("\n Generazion n. %d\n", i + 1);
    print_grid(grid);
  }

  return 0;
}
