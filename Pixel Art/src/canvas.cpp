// canvas.cpp
#include "canvas.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

static Color copiedSelection[GRID_SIZE][GRID_SIZE];
static bool selectionAvailable = false;

// Custom Clamp function for integer values
int Clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Draw the checkerboard background as part of the canvas
void DrawCheckerboardBackground(int squareSize) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            // Alternate colors based on the position to create a checker pattern
            Color color = ((x + y) % 2 == 0) ? LIGHTGRAY : WHITE;
            DrawRectangle((x * squareSize) + CANVAS_X, y * squareSize, squareSize, squareSize, color);
        }
    }
}

// Initializes the grid with a checkerboard pattern
void InitGrid(Color grid[GRID_SIZE][GRID_SIZE]) {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            grid[x][y] = BLANK;
        }
    }
}

// Draws the grid on the screen
void DrawGrid(Color grid[GRID_SIZE][GRID_SIZE], int squareSize) {
    // Draw the checkerboard background first
    DrawCheckerboardBackground(squareSize);

    // Draw the main grid on top of the checkerboard
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            Color color = grid[x][y];
            if (color.a != 0) { // Only draw non-transparent colors
                DrawRectangle((x * squareSize) + CANVAS_X, y * squareSize, squareSize, squareSize, color);
            }
        }
    }
}


// Checks if the mouse is within the scaled canvas area
bool IsMouseOnCanvas(Vector2 mousePos, int scaleFactor) {
    int canvasWidth = GRID_SIZE * SQUARE_SIZE;
    int canvasHeight = GRID_SIZE * SQUARE_SIZE;
    return mousePos.x >= 0 && mousePos.x < canvasWidth &&
        mousePos.y >= 0 && mousePos.y < canvasHeight;
}


// Copy selected area into the buffer
void CopySelection(Color grid[GRID_SIZE][GRID_SIZE], Vector2 startPos, Vector2 endPos) {
    int startX = Clamp((int)startPos.x / SQUARE_SIZE, 0, GRID_SIZE - 1);
    int startY = Clamp((int)startPos.y / SQUARE_SIZE, 0, GRID_SIZE - 1);
    int endX = Clamp((int)endPos.x / SQUARE_SIZE, 0, GRID_SIZE - 1);
    int endY = Clamp((int)endPos.y / SQUARE_SIZE, 0, GRID_SIZE - 1);

    // Clear previous selection and copy new selection
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            copiedSelection[x][y] = BLANK;
        }
    }

    for (int x = fmin(startX, endX); x <= fmax(startX, endX); x++) {
        for (int y = fmin(startY, endY); y <= fmax(startY, endY); y++) {
            copiedSelection[x - startX][y - startY] = grid[x][y];
        }
    }

    selectionAvailable = true;  // Set flag indicating a selection is available
}

// Paste copied selection to a new location
void PasteSelection(Color grid[GRID_SIZE][GRID_SIZE], Vector2 pastePos) {
    if (!selectionAvailable) return;  // Exit if no selection to paste

    int pasteX = Clamp((int)pastePos.x / SQUARE_SIZE, 0, GRID_SIZE - 1);
    int pasteY = Clamp((int)pastePos.y / SQUARE_SIZE, 0, GRID_SIZE - 1);

    // Loop through the selection buffer and paste it onto the grid
    for (int x = 0; x < GRID_SIZE && pasteX + x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE && pasteY + y < GRID_SIZE; y++) {
            if (copiedSelection[x][y].a != 0) {  // Only paste non-transparent pixels
                grid[pasteX + x][pasteY + y] = copiedSelection[x][y];
            }
        }
    }
}

// Update DrawOnCanvas to handle selection and paste operations
void DrawOnCanvas(Color grid[GRID_SIZE][GRID_SIZE], DrawingMode mode, Color selectedColor, bool eraserMode, Vector2 startPos, Vector2 endPos) {
    int squareSize = SQUARE_SIZE;

    // Convert mouse coordinates to grid coordinates
    int startX = Clamp((int)(startPos.x / squareSize), 0, GRID_SIZE - 1);
    int startY = Clamp((int)(startPos.y / squareSize), 0, GRID_SIZE - 1);
    int endX = Clamp((int)(endPos.x / squareSize), 0, GRID_SIZE - 1);
    int endY = Clamp((int)(endPos.y / squareSize), 0, GRID_SIZE - 1);

    if (mode == MODE_SELECT) {
        CopySelection(grid, startPos, endPos);
    }
    else if (mode == MODE_FILLED_BOX) {
        for (int x = fmin(startX, endX); x <= fmax(startX, endX); x++) {
            for (int y = fmin(startY, endY); y <= fmax(startY, endY); y++) {
                grid[x][y] = eraserMode ? BLANK : selectedColor;
            }
        }
    }
    else if (mode == MODE_OUTLINE_BOX) {
        for (int x = fmin(startX, endX); x <= fmax(startX, endX); x++) {
            grid[x][startY] = eraserMode ? BLANK : selectedColor;
            grid[x][endY] = eraserMode ? BLANK : selectedColor;
        }
        for (int y = fmin(startY, endY); y <= fmax(startY, endY); y++) {
            grid[startX][y] = eraserMode ? BLANK : selectedColor;
            grid[endX][y] = eraserMode ? BLANK : selectedColor;
        }
    }
    else if (mode == MODE_PENCIL) {
        grid[endX][endY] = eraserMode ? BLANK : selectedColor;
    }
}