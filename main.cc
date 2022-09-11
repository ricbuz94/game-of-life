#include <iostream>
#include <string>
#include <string_view>
#include <vector>
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

constexpr int SIZE = 36;
constexpr int GENERATIONS = 400;

constexpr std::string_view DEAD = "□", LIVE = "■";

constexpr int pattern[81][2] = {{0, 0}, {1, 0}, {12, 0}, {13, 0}, {21, 0}, {22, 0}, {34, 0}, {35, 0}, {0, 1}, {12, 1}, {22, 1}, {35, 1}, {7, 7}, {8, 7}, {17, 7}, {27, 7}, {8, 8}, {9, 8}, {16, 8}, {17, 8}, {18, 8}, {25, 8}, {26, 8}, {27, 8}, {8, 9}, {17, 9}, {26, 9}, {0, 12}, {1, 12}, {34, 12}, {35, 12}, {0, 13}, {35, 13}, {8, 16}, {17, 16}, {26, 16}, {7, 17}, {8, 17}, {9, 17}, {16, 17}, {17, 17}, {18, 17}, {25, 17}, {26, 17}, {27, 17}, {8, 18}, {17, 18}, {26, 18}, {0, 21}, {35, 21}, {0, 22}, {1, 22}, {34, 22}, {35, 22}, {8, 25}, {17, 25}, {26, 25}, {7, 26}, {8, 26}, {9, 26}, {16, 26}, {17, 26}, {18, 26}, {25, 26}, {26, 26}, {7, 27}, {17, 27}, {26, 27}, {27, 27}, {0, 34}, {12, 34}, {22, 34}, {35, 34}, {0, 35}, {1, 35}, {12, 35}, {13, 35}, {21, 35}, {22, 35}, {34, 35}, {35, 35}};

// Matrix steps (row/col operations)
constexpr signed neighbours_cells[8][2] = {{1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1}};

struct Matrix
{
  std::string values[SIZE][SIZE];
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

  return sum;
}

// Check all living neighbours
int check_live_neighbours(struct Matrix *matrix, int cell_x, int cell_y)
{
  int neighbours = 0;
  int x, y;

  for (int p = 0; p < 8; p++)
  {
    x = calc_coordinate(cell_x, neighbours_cells[p][0]);
    y = calc_coordinate(cell_y, neighbours_cells[p][1]);

    if (matrix->values[x][y] == LIVE)
    {
      neighbours++;
    }
  }

  return neighbours;
}

// Cycle a new generation
Matrix new_generation(Matrix *matrix)
{
  Matrix new_matrix;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      const int live_neighbours_number = check_live_neighbours(matrix, i, j);

      if (matrix->values[i][j] == LIVE && (live_neighbours_number == 2 || live_neighbours_number == 3))
      {
        new_matrix.values[i][j] = LIVE;
      }
      else if (matrix->values[i][j] == DEAD && live_neighbours_number == 3)
      {
        new_matrix.values[i][j] = LIVE;
      }
      else
      {
        new_matrix.values[i][j] = DEAD;
      }
    }
  }

  return new_matrix;
}

// Print matrix
void print_matrix(Matrix *matrix)
{
  std::cout << std::endl;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      std::cout << " " << matrix->values[i][j];
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

// Main
int main(int argc, char const *argv[])
{
  Matrix matrix;

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      // Pattern
      bool found = false;
      for (int p = 0; p < sizeof(pattern) / (sizeof(int) * 2); p++)
      {
        if (i == pattern[p][0] && j == pattern[p][1])
        {
          matrix.values[i][j] = LIVE;
          found = true;
          break;
        }
      }

      if (!found)
      {
        matrix.values[i][j] = DEAD;
      }
    }
  }

  system("clear");
  unsigned int microsecond = 1000000;

  std::cout << "\nInitial stage:" << std::endl;
  print_matrix(&matrix);
  usleep(2 * microsecond);

  for (int i = 0; i < GENERATIONS; i++)
  {
    matrix = new_generation(&matrix);

    usleep(0.05 * microsecond);
    system("clear");
    std::cout << "\nGeneration n. " << i + 1 << std::endl;
    print_matrix(&matrix);
  }

  return 0;
}
