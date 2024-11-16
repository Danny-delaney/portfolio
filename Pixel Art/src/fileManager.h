#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "raylib.h"
#include "canvas.h"

// Function declarations with default arguments for saving and loading
void SaveGridAsPNG(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName = nullptr);
void LoadGridFromPNG(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName = nullptr);
void SaveGridAs(Color grid[GRID_SIZE][GRID_SIZE], const char* newFileName);
void LoadGridByName(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName);

#endif
