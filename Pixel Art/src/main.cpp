// main.cpp
#include "raylib.h"
#include "ui.h"
#include "canvas.h"
#include "config.h"
#include <stack>
#include <vector>

int main() {
    int screenWidth = (CANVAS_X + (GRID_SIZE * SQUARE_SIZE))  * SCALE_FACTOR;
    const int screenHeight = (GRID_SIZE * SQUARE_SIZE) * SCALE_FACTOR;
    InitWindow(screenWidth, screenHeight, "Scaled Canvas");

    Color grid[GRID_SIZE][GRID_SIZE];
    InitGrid(grid);

    Color selectedColor = BLACK;
    bool eraserMode = false;
    DrawingMode mode = MODE_PENCIL;
    Vector2 startMousePos;
    bool drawing = false;

    // Stacks for undo and redo operations
    std::stack<std::vector<Color>> undoStack;
    std::stack<std::vector<Color>> redoStack;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the grid and UI components with scaled sizes
        DrawGrid(grid, SQUARE_SIZE * SCALE_FACTOR);
        DrawUndoRedoButtons(grid, undoStack, redoStack, SCALE_FACTOR);
        DrawColorWheel(selectedColor, eraserMode, SCALE_FACTOR);
        DrawColorSwatches(selectedColor, eraserMode, SCALE_FACTOR);
        DrawEraserButton(eraserMode, SCALE_FACTOR);
        DrawSaveLoadButtons(grid, SCALE_FACTOR);
        DrawModeButtons(mode, SCALE_FACTOR);

        // Get the mouse position, scaling it down for the grid coordinate calculations
        Vector2 mousePos = GetMousePosition();
        mousePos.x = (mousePos.x - CANVAS_X) / SCALE_FACTOR;
        mousePos.y /= SCALE_FACTOR;

        // Start drawing (rectangle or pencil) when the left mouse button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseOnCanvas(mousePos, SCALE_FACTOR)) {
            startMousePos = mousePos;
            drawing = true;

            // Save the current state to the undo stack
            std::vector<Color> currentState(GRID_SIZE * GRID_SIZE);
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    currentState[x * GRID_SIZE + y] = grid[x][y];
                }
            }
            undoStack.push(currentState);

            // Clear the redo stack whenever a new action starts
            while (!redoStack.empty()) redoStack.pop();
        }

        // Continue drawing or show the temporary rectangle outline
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && drawing) {
            if (mode == MODE_FILLED_BOX || mode == MODE_OUTLINE_BOX || mode == MODE_SELECT) {
                DrawRectangleLines((startMousePos.x * SCALE_FACTOR) + CANVAS_X,
                    startMousePos.y * SCALE_FACTOR,
                    (mousePos.x - startMousePos.x) * SCALE_FACTOR,
                    (mousePos.y - startMousePos.y) * SCALE_FACTOR,
                    selectedColor);
            }
            else if (mode == MODE_PENCIL) {
                DrawOnCanvas(grid, mode, selectedColor, eraserMode, mousePos, mousePos);
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && mode == MODE_SELECT) {
            PasteSelection(grid, mousePos);
        }

        // End drawing and apply the changes to the grid
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && drawing) {
            DrawOnCanvas(grid, mode, selectedColor, eraserMode, startMousePos, mousePos);
            drawing = false;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
