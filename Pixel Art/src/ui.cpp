// ui.cpp
#include "ui.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "fileManager.h"
#include "raylib.h"
#include "config.h"
#include <stack>
#include <vector>


void DrawColorWheel(Color& selectedColor, bool& eraserMode, int scaleFactor) {
    Vector2 wheelCenter = { 100 * scaleFactor, 300 * scaleFactor };
    int wheelRadius = 80 * scaleFactor;
    int segments = 60;

    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, wheelCenter, wheelRadius) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        float angle = atan2f(mousePos.y - wheelCenter.y, mousePos.x - wheelCenter.x);
        if (angle < 0) angle += 2 * PI;

        float hue = angle * (180.0f / PI) / 360.0f;
        selectedColor = ColorFromHSV(hue * 360, 1.0f, 1.0f);
        eraserMode = false;
    }

    for (int i = 0; i < segments; i++) {
        float startAngle = (360.0f / segments) * i;
        float endAngle = startAngle + (360.0f / segments);
        Color segmentColor = ColorFromHSV(i * 6.0f, 1.0f, 1.0f);
        DrawCircleSector(wheelCenter, wheelRadius, startAngle, endAngle, 32, segmentColor);
    }
}

void DrawColorSwatches(Color& selectedColor, bool& eraserMode, int scaleFactor) {
    Rectangle whiteSwatch = { 10 * scaleFactor, 400 * scaleFactor, 80 * scaleFactor, 40 * scaleFactor };
    Rectangle blackSwatch = { 110 * scaleFactor, 400 * scaleFactor, 80 * scaleFactor, 40 * scaleFactor };
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, whiteSwatch) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedColor = WHITE;
        eraserMode = false;
    }
    if (CheckCollisionPointRec(mousePos, blackSwatch) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedColor = BLACK;
        eraserMode = false;
    }

    DrawRectangleRec(whiteSwatch, WHITE);
    DrawRectangleLinesEx(whiteSwatch, 2, BLACK);
    DrawText("White", whiteSwatch.x + 15 * scaleFactor, whiteSwatch.y + 10 * scaleFactor, 10 * scaleFactor, BLACK);

    DrawRectangleRec(blackSwatch, BLACK);
    DrawRectangleLinesEx(blackSwatch, 2, WHITE);
    DrawText("Black", blackSwatch.x + 15 * scaleFactor, blackSwatch.y + 10 * scaleFactor, 10 * scaleFactor, WHITE);
}

void DrawEraserButton(bool& eraserMode, int scaleFactor) {
    Rectangle eraserButton = { 10 * scaleFactor, 460 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    if (GuiButton(eraserButton, "Eraser")) {
        eraserMode = true;
    }
}

void DrawModeButtons(DrawingMode& mode, int scaleFactor) {
    Rectangle pencilButton = { 10 * scaleFactor, 520 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle filledBoxButton = { 10 * scaleFactor, 580 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle outlineBoxButton = { 10 * scaleFactor, 640 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle selectBoxButton = { 10 * scaleFactor, 700 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };

    if (GuiButton(pencilButton, "Pencil")) mode = MODE_PENCIL;
    if (GuiButton(filledBoxButton, "Filled Box")) mode = MODE_FILLED_BOX;
    if (GuiButton(outlineBoxButton, "Outline Box")) mode = MODE_OUTLINE_BOX;
    if (GuiButton(selectBoxButton, "Select")) mode = MODE_SELECT;
}

void DrawSaveLoadButtons(Color grid[GRID_SIZE][GRID_SIZE], int scaleFactor) {
    Rectangle saveButton = { 10 * scaleFactor, 770 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle loadButton = { 10 * scaleFactor, 830 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle saveAsButton = { 10 * scaleFactor, 890 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
    Rectangle loadByNameButton = { 10 * scaleFactor, 950 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };

    static char fileNameInput[256] = "";
    static bool showSaveAsInput = false;
    static bool showLoadByNameInput = false;

    if (GuiButton(saveButton, "Save")) {
        if (strlen(currentFileName) == 0) {
            strncpy(currentFileName, fileNameInput, 255);
        }
        SaveGridAsPNG(grid);
    }

    if (GuiButton(loadButton, "Load Current")) {
        LoadGridFromPNG(grid);
    }

    if (GuiButton(saveAsButton, "Save As")) {
        showSaveAsInput = true;
        showLoadByNameInput = false; // Close load input if open
    }

    if (GuiButton(loadByNameButton, "Load by Name")) {
        showLoadByNameInput = true;
        showSaveAsInput = false; // Close save input if open
    }

    // Display and handle "Save As" input
    if (showSaveAsInput) {
        Rectangle inputBox = { 200 * scaleFactor, 890 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
        GuiTextBox(inputBox, fileNameInput, 256, true);

        Rectangle confirmSaveButton = { 400 * scaleFactor, 890 * scaleFactor, 80 * scaleFactor, 50 * scaleFactor };
        if (GuiButton(confirmSaveButton, "OK")) {
            if (strlen(fileNameInput) > 0) {
                SaveGridAs(grid, fileNameInput);
                strncpy(currentFileName, fileNameInput, 255); // Update current file name
            }
            showSaveAsInput = false;
        }

        Rectangle cancelButton = { 490 * scaleFactor, 890 * scaleFactor, 80 * scaleFactor, 50 * scaleFactor };
        if (GuiButton(cancelButton, "Cancel")) {
            showSaveAsInput = false;
        }
    }

    // Display and handle "Load by Name" input
    if (showLoadByNameInput) {
        Rectangle inputBox = { 200 * scaleFactor, 950 * scaleFactor, 180 * scaleFactor, 50 * scaleFactor };
        GuiTextBox(inputBox, fileNameInput, 256, true);

        Rectangle confirmLoadButton = { 400 * scaleFactor, 950 * scaleFactor, 80 * scaleFactor, 50 * scaleFactor };
        if (GuiButton(confirmLoadButton, "OK")) {
            if (strlen(fileNameInput) > 0) {
                LoadGridByName(grid, fileNameInput);
                strncpy(currentFileName, fileNameInput, 255); // Update current file name
            }
            showLoadByNameInput = false;
        }

        Rectangle cancelButton = { 490 * scaleFactor, 950 * scaleFactor, 80 * scaleFactor, 50 * scaleFactor };
        if (GuiButton(cancelButton, "Cancel")) {
            showLoadByNameInput = false;
        }
    }
}
void DrawUndoRedoButtons(Color grid[GRID_SIZE][GRID_SIZE], std::stack<std::vector<Color>>& undoStack, std::stack<std::vector<Color>>& redoStack, int scaleFactor) {
    Rectangle undoButton = { 10 * scaleFactor, 10 * scaleFactor, 80 * scaleFactor, 40 * scaleFactor };
    Rectangle redoButton = { 100 * scaleFactor, 10 * scaleFactor, 80 * scaleFactor, 40 * scaleFactor };

    if (GuiButton(undoButton, "Undo") && !undoStack.empty()) {
        std::vector<Color> currentState(GRID_SIZE * GRID_SIZE);
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                currentState[x * GRID_SIZE + y] = grid[x][y];
            }
        }
        redoStack.push(currentState);

        const std::vector<Color>& lastState = undoStack.top();
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                grid[x][y] = lastState[x * GRID_SIZE + y];
            }
        }
        undoStack.pop();
    }

    if (GuiButton(redoButton, "Redo") && !redoStack.empty()) {
        std::vector<Color> currentState(GRID_SIZE * GRID_SIZE);
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                currentState[x * GRID_SIZE + y] = grid[x][y];
            }
        }
        undoStack.push(currentState);

        const std::vector<Color>& lastState = redoStack.top();
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                grid[x][y] = lastState[x * GRID_SIZE + y];
            }
        }
        redoStack.pop();
    }
}
