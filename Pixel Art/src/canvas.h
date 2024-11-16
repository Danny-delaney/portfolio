// canvas.h
#ifndef CANVAS_H
#define CANVAS_H

#include "raylib.h"
#include "config.h"

// Modes for finite state machine
typedef enum {
    MODE_PENCIL,
    MODE_FILLED_BOX,
    MODE_OUTLINE_BOX,
    MODE_SELECT       // New mode for selecting and copying grid sections
} DrawingMode;

// Function declarations
void DrawCheckerboardBackground(int squareSize);
void InitGrid(Color grid[GRID_SIZE][GRID_SIZE]);
void DrawGrid(Color grid[GRID_SIZE][GRID_SIZE], int squareSize);
bool IsMouseOnCanvas(Vector2 mousePos, int scaleFactor);
void DrawOnCanvas(Color grid[GRID_SIZE][GRID_SIZE], DrawingMode mode, Color selectedColor, bool eraserMode, Vector2 startPos, Vector2 endPos);
void CopySelection(Color grid[GRID_SIZE][GRID_SIZE], Vector2 startPos, Vector2 endPos);
void PasteSelection(Color grid[GRID_SIZE][GRID_SIZE], Vector2 pastePos);

#endif
