// ui.h
#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "canvas.h"
#include <stack>
#include <vector>

// UI Functions
void DrawColorWheel(Color& selectedColor, bool& eraserMode, int scaleFactor);
void DrawColorSwatches(Color& selectedColor, bool& eraserMode, int scaleFactor);
void DrawModeButtons(DrawingMode& mode, int scaleFactor);
void DrawEraserButton(bool& eraserMode, int scaleFactor);
void DrawSaveLoadButtons(Color grid[GRID_SIZE][GRID_SIZE], int scaleFactor);
void DrawUndoRedoButtons(Color grid[GRID_SIZE][GRID_SIZE], std::stack<std::vector<Color>>& undoStack, std::stack<std::vector<Color>>& redoStack, int scaleFactor);

#endif
