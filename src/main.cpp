// main.cpp - Raylib Icon Maker for iOS by Demi 💖 PRO MAX

#include "raylib.h"
#include <vector>
#include <stack>
#include <sstream>
#include <cmath>
#include <cstring>
#include <algorithm>

const int WIDTH = 512, HEIGHT = 512;
int brushSize = 8;
bool brushIsCircle = true;
int currentFrame = 0;
const int NUM_FRAMES = 4;

Color drawColor = BLACK;
std::vector<Color> colorHistory;
std::stack<unsigned char*> undoStack, redoStack;
bool showAI = false;

enum InputMode { NONE, RGB_MODE, HSV_MODE, HEX_MODE };
InputMode inputMode = NONE;
char inputBuffer[64] = "";
int inputLength = 0;
bool inputActive = false;
bool rubberActive = false;

char keyLayout[] = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM#";
int selectedKey = -1;

Color HSVtoRGB(float h, float s, float v) {
    float r, g, b;
    int i = (int)(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }

    return (Color){ (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), 255 };
}

void RGBtoHSV(Color c, float &h, float &s, float &v) {
    float r = c.r / 255.0f, g = c.g / 255.0f, b = c.b / 255.0f;
    float max = fmaxf(fmaxf(r, g), b);
    float min = fminf(fminf(r, g), b);
    v = max;
    float d = max - min;
    s = max == 0 ? 0 : d / max;
    if (max == min) h = 0;
    else if (max == r) h = (g - b) / d + (g < b ? 6 : 0);
    else if (max == g) h = (b - r) / d + 2;
    else h = (r - g) / d + 4;
    h /= 6.0f;
}

void AddColor(Color c) {
    colorHistory.insert(colorHistory.begin(), c);
    if (colorHistory.size() > 5) colorHistory.pop_back();
}

void PushUndo(Image frames[]) {
    unsigned char* snap = (unsigned char*)malloc(WIDTH * HEIGHT * 4);
    memcpy(snap, frames[currentFrame].data, WIDTH * HEIGHT * 4);
    undoStack.push(snap);
    while (!redoStack.empty()) {
        free(redoStack.top());
        redoStack.pop();
    }
}

void ApplyBrush(int x, int y, Image frames[]) {
    for (int dy = -brushSize; dy <= brushSize; dy++) {
        for (int dx = -brushSize; dx <= brushSize; dx++) {
            if (dx * dx + dy * dy > brushSize * brushSize && brushIsCircle) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                ImageDrawPixel(&frames[currentFrame], nx, ny, drawColor);
            }
        }
    }
}

void ProcessInput(Image frames[]) {
    std::istringstream iss(inputBuffer);
    if (inputMode == RGB_MODE) {
        int r, g, b;
        if (iss >> r >> g >> b) {
            drawColor = (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
            AddColor(drawColor);
        }
    } else if (inputMode == HSV_MODE) {
        float h, s, v;
        if (iss >> h >> s >> v) {
            drawColor = HSVtoRGB(h, s, v);
            AddColor(drawColor);
        }
    } else if (inputMode == HEX_MODE) {
        std::string hex;
        iss >> hex;
        if (hex[0] == '#') hex = hex.substr(1);
        if (hex.length() == 3) {
            std::string expanded;
            for (char c : hex) expanded += std::string(2, c);
            hex = expanded;
        }
        if (hex.length() == 6 || hex.length() == 8) {
            try {
                unsigned char r = std::stoi(hex.substr(0, 2), nullptr, 16);
                unsigned char g = std::stoi(hex.substr(2, 2), nullptr, 16);
                unsigned char b = std::stoi(hex.substr(4, 2), nullptr, 16);
                drawColor = (Color){ r, g, b, 255 };
                AddColor(drawColor);
            } catch (...) {}
        }
    }
    inputActive = false;
    inputBuffer[0] = '\0';
    inputLength = 0;
    inputMode = NONE;
}

std::vector<Color> GeneratePalette(Color base) {
    float h, s, v;
    RGBtoHSV(base, h, s, v);
    std::vector<Color> palette;
    for (int i = 0; i < 5; i++) {
        float newHue = fmod(h + i * 0.1f, 1.0f);
        palette.push_back(HSVtoRGB(newHue, s, v));
    }
    return palette;
}

void DrawColorWheel(int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            float dist = sqrtf(x * x + y * y);
            if (dist <= radius) {
                float angle = atan2f(y, x);
                if (angle < 0) angle += 2 * PI;
                float hue = angle / (2 * PI);
                Color c = HSVtoRGB(hue, 1, 1);
                DrawPixel(cx + x, cy + y, c);
            }
        }
    }
}

void DrawKeyboard() {
    int keyWidth = 40, keyHeight = 40;
    int startX = 10, startY = HEIGHT - 200;
    for (int i = 0; i < (int)strlen(keyLayout); i++) {
        int x = startX + (i % 10) * (keyWidth + 5);
        int y = startY + (i / 10) * (keyHeight + 5);
        DrawRectangle(x, y, keyWidth, keyHeight, GRAY);
        DrawTextCodepoint(GetFontDefault(), keyLayout[i], (Vector2){ (float)(x + 12), (float)(y + 8) }, 20, BLACK);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, (Rectangle){ (float)x, (float)y, (float)keyWidth, (float)keyHeight })) {
                if (inputLength < 63) {
                    inputBuffer[inputLength++] = keyLayout[i];
                    inputBuffer[inputLength] = '\0';
                }
            }
        }
    }

    int bx = startX, by = startY + 4 * (keyHeight + 5);
    DrawRectangle(bx, by, 80, keyHeight, DARKGRAY);
    DrawText("BKSP", bx + 10, by + 10, 20, WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, (Rectangle){ (float)bx, (float)by, 80, (float)keyHeight }) && inputLength > 0) {
            inputLength--;
            inputBuffer[inputLength] = '\0';
        }
    }

    int ex = bx + 100;
    DrawRectangle(ex, by, 80, keyHeight, DARKGREEN);
    DrawText("ENTER", ex + 10, by + 10, 20, WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, (Rectangle){ (float)ex, (float)by, 80, (float)keyHeight })) {
            ProcessInput(nullptr); // Placeholder, would be passed properly
        }
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "iOS Icon Maker Pro by Demi 💖");
    SetTargetFPS(60);

    Image frames[NUM_FRAMES];
    for (int i = 0; i < NUM_FRAMES; i++) frames[i] = GenImageColor(WIDTH, HEIGHT, WHITE);
    Texture2D canvas = LoadTextureFromImage(frames[currentFrame]);

    bool drawing = false;

    while (!WindowShouldClose()) {
        int mx = GetMouseX(), my = GetMouseY();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !inputActive) {
            PushUndo(frames);
            drawing = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) drawing = false;

        if (drawing && !inputActive) {
            Color original = drawColor;
            if (rubberActive) drawColor = WHITE;
            ApplyBrush(mx, my, frames);
            drawColor = original;
        }

        if (IsKeyPressed(KEY_ONE)) { inputActive = true; inputMode = RGB_MODE; }
        if (IsKeyPressed(KEY_TWO)) { inputActive = true; inputMode = HSV_MODE; }
        if (IsKeyPressed(KEY_THREE)) { inputActive = true; inputMode = HEX_MODE; }

        UpdateTexture(canvas, frames[currentFrame].data);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(canvas, 0, 0, WHITE);

        for (int i = 0; i < (int)colorHistory.size(); i++) {
            DrawRectangle(10 + i * 30, HEIGHT - 40, 28, 28, colorHistory[i]);
        }

        DrawColorWheel(WIDTH - 100, HEIGHT - 120, 50);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int dx = mx - (WIDTH - 100), dy = my - (HEIGHT - 120);
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= 50) {
                float angle = atan2f(dy, dx);
                if (angle < 0) angle += 2 * PI;
                drawColor = HSVtoRGB(angle / (2 * PI), 1, 1);
                AddColor(drawColor);
            }
        }

        std::vector<Color> palette = GeneratePalette(drawColor);
        for (int i = 0; i < (int)palette.size(); i++) {
            int x = WIDTH - 160 + i * 30, y = HEIGHT - 60;
            DrawRectangle(x, y, 28, 28, palette[i]);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                mx >= x && mx <= x + 28 && my >= y && my <= y + 28) {
                drawColor = palette[i];
                AddColor(drawColor);
            }
        }

        if (inputActive) {
            DrawRectangle(40, HEIGHT / 2 - 30, 440, 50, LIGHTGRAY);
            DrawText(inputBuffer, 50, HEIGHT / 2 - 15, 30, BLACK);
            DrawText("Tap keys below to type | ENTER to confirm", 50, HEIGHT / 2 + 30, 15, GRAY);
            DrawKeyboard();
        }

        EndDrawing();
    }

    UnloadTexture(canvas);
    for (int i = 0; i < NUM_FRAMES; i++) UnloadImage(frames[i]);
    while (!undoStack.empty()) { free(undoStack.top()); undoStack.pop(); }
    while (!redoStack.empty()) { free(redoStack.top()); redoStack.pop(); }

    CloseWindow();
    return 0;
}
