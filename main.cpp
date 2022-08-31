#include <iostream>
#include <string>

// Conway's Game of Life

// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.             LIVE -> DEAD
// 2. Any live cell with two or three live neighbours lives on to the next generation.              LIVE -> LIVE
// 3. Any live cell with more than three live neighbours dies, as if by overpopulation.             LIVE -> DEAD
// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.  DEAD -> LIVE

// 1. Any live cell with two or three live neighbours survives.                                     LIVE -> LIVE
// 2. Any dead cell with three live neighbours becomes a live cell.                                 DEAD -> LIVE
// 3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.   LIVE,DEAD -> DEAD

using namespace std;

const int SIZE = 10;
const string DEAD = "◻︎", LIVE = "◼︎";

// Patterns
const int tub_pattern[4][2] = {{3, 5}, {4, 4}, {4, 6}, {5, 5}};
const int oscillator_pattern[3][2] = {{4, 4}, {4, 5}, {4, 6}};
const int glider_pattern[5][2] = {{2, 2}, {3, 3}, {4, 1}, {4, 2}, {4, 3}};

// Matrix steps
const signed around_cells[8][2] = {{1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1}};

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
    x = calc_coordinate(cell_x, around_cells[p][0]);
    y = calc_coordinate(cell_y, around_cells[p][1]);

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

      if (grid->values[i][j] == LIVE && live_neighbours_number < 2)
      {
        new_grid.values[i][j] = DEAD;
      }
      else if (grid->values[i][j] == LIVE && (live_neighbours_number == 2 || live_neighbours_number == 3))
      {
        new_grid.values[i][j] = LIVE;
      }
      else if (grid->values[i][j] == LIVE && live_neighbours_number > 3)
      {
        new_grid.values[i][j] = DEAD;
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
      for (int p = 0; p < 5; p++)
      {
        if (i == glider_pattern[p][0] && j == glider_pattern[p][1])
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

  cout << "\nStadio iniziale:" << endl;
  print_grid(&grid);

  for (int i = 0; i < 10; i++)
  {
    cout << "Generazione " << i + 1 << endl;
    grid = new_generation(&grid);
    print_grid(&grid);
  }

  return 0;
}
