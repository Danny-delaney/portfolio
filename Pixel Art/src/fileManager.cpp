#include "fileManager.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include "config.h"

#define DRAWINGS_FOLDER "drawings/"

void EnsurePNGExtension(char* fileName, size_t maxLen) {
    size_t len = strlen(fileName);
    if (len > 4 && strcmp(&fileName[len - 4], ".png") == 0) {
        return; // Already has .png extension
    }
    strncat(fileName, ".png", maxLen - len - 1);
}

void SaveGridAsPNG(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName) {
    Image image = GenImageColor(GRID_SIZE, GRID_SIZE, BLANK);

    char adjustedFileName[256];
    strncpy(adjustedFileName, fileName ? fileName : currentFileName, sizeof(adjustedFileName) - 1);
    adjustedFileName[sizeof(adjustedFileName) - 1] = '\0';

    EnsurePNGExtension(adjustedFileName, sizeof(adjustedFileName));

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", DRAWINGS_FOLDER, adjustedFileName);

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            ImageDrawPixel(&image, x, y, grid[x][y]);
        }
    }

    ExportImage(image, fullPath);
    UnloadImage(image);

    if (fileName) strcpy(currentFileName, adjustedFileName);
}

void LoadGridFromPNG(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName) {
    char adjustedFileName[256];
    strncpy(adjustedFileName, fileName ? fileName : currentFileName, sizeof(adjustedFileName) - 1);
    adjustedFileName[sizeof(adjustedFileName) - 1] = '\0';

    EnsurePNGExtension(adjustedFileName, sizeof(adjustedFileName));

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", DRAWINGS_FOLDER, adjustedFileName);

    if (FileExists(fullPath)) {
        Image image = LoadImage(fullPath);
        if (image.width == GRID_SIZE && image.height == GRID_SIZE) {
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    grid[x][y] = GetImageColor(image, x, y);
                }
            }
        }
        UnloadImage(image);

        if (fileName) strcpy(currentFileName, adjustedFileName);
    }
}

void SaveGridAs(Color grid[GRID_SIZE][GRID_SIZE], const char* newFileName) {
    SaveGridAsPNG(grid, newFileName);
}

void LoadGridByName(Color grid[GRID_SIZE][GRID_SIZE], const char* fileName) {
    LoadGridFromPNG(grid, fileName);
}
