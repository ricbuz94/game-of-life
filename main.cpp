#include <iostream>
#include <string>
#include <unistd.h>

// Conway's Game of Life

// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.             LIVE -> DEAD
// 2. Any live cell with two or three live neighbours lives on to the next generation.              LIVE -> LIVE
// 3. Any live cell with more than three live neighbours dies, as if by overpopulation.             LIVE -> DEAD
// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.  DEAD -> LIVE

// ...or more concise:

// 1. Any live cell with two or three live neighbours survives.                                     LIVE -> LIVE
// 2. Any dead cell with three live neighbours becomes a live cell.                                 DEAD -> LIVE
// 3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.   LIVE,DEAD -> DEAD

using namespace std;

const int SIZE = 36;
const string DEAD = "◻︎", LIVE = "◼︎";

// Patterns

// - Tub
const int tub_pattern[4][2] = {{3, 5}, {4, 4}, {4, 6}, {5, 5}};

// - Oscillator
const int oscillator_pattern[3][2] = {{4, 4}, {4, 5}, {4, 6}};

// - Gliders
const int glider_pattern_north[5][2] = {{15, 20}, {14, 19}, {13, 19}, {13, 20}, {13, 21}};
const int glider_pattern_south[5][2] = {{19, 14}, {20, 15}, {21, 13}, {21, 14}, {21, 15}};
const int glider_pattern_east[5][2] = {{20, 19}, {19, 20}, {19, 21}, {20, 21}, {21, 21}};
const int glider_pattern_west[5][2] = {{14, 15}, {15, 14}, {15, 13}, {14, 13}, {13, 13}};

// Matrix steps
const signed neighbours_cells[8][2] = {{1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1}};

struct Grid
{
  string values[SIZE][SIZE];
};

int calc_coordinate(int coordinate, signed sub_value)
{
  const int sum = coordinate + sub_value;

  if (sum == SIZE)
  {
    return 0;
  }
  if (sum < 0)
  {
    return SIZE - 1;
  }
  else
  {
    return sum;
  }
}

// Check all living neighbours
int check_live_neighbours(struct Grid *grid, int cell_x, int cell_y)
{
  int neighbours = 0;
  int x, y;

  for (int p = 0; p < 8; p++)
  {
    x = calc_coordinate(cell_x, neighbours_cells[p][0]);
    y = calc_coordinate(cell_y, neighbours_cells[p][1]);

    if (grid->values[x][y] == LIVE)
    {
      neighbours++;
    }
  }

  return neighbours;
}

// Cycle a new generation
Grid new_generation(Grid *grid)
{
  Grid new_grid;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      const int live_neighbours_number = check_live_neighbours(grid, i, j);

      if (grid->values[i][j] == LIVE && (live_neighbours_number == 2 || live_neighbours_number == 3))
      {
        new_grid.values[i][j] = LIVE;
      }
      else if (grid->values[i][j] == DEAD && live_neighbours_number == 3)
      {
        new_grid.values[i][j] = LIVE;
      }
      else
      {
        new_grid.values[i][j] = DEAD;
      }
    }
  }

  return new_grid;
}

// Print grid
void print_grid(Grid *grid)
{
  cout << endl;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      cout << " " << grid->values[i][j];
    }
    cout << endl;
  }

  cout << endl;
}

// Main
int main(int argc, char const *argv[])
{
  Grid grid;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      // Pattern
      bool found = false;
      for (int p = 0; p < sizeof(glider_pattern_north) / (sizeof(int) * 2); p++)
      {
        if (
            (i == glider_pattern_north[p][0] && j == glider_pattern_north[p][1]) ||
            (i == glider_pattern_south[p][0] && j == glider_pattern_south[p][1]) ||
            (i == glider_pattern_east[p][0] && j == glider_pattern_east[p][1]) ||
            (i == glider_pattern_west[p][0] && j == glider_pattern_west[p][1]))
        {
          grid.values[i][j] = LIVE;
          found = true;
          break;
        }
      }

      if (!found)
      {
        grid.values[i][j] = DEAD;
      }
    }
  }

  system("clear");

  cout << "\nStadio iniziale:" << endl;
  print_grid(&grid);

  for (int i = 0; i < 160; i++)
  {
    grid = new_generation(&grid);

    unsigned int microsecond = 1000000;
    usleep(0.25 * microsecond);
    system("clear");
    cout << "Generazione " << i + 1 << endl;
    print_grid(&grid);
  }

  return 0;
}
