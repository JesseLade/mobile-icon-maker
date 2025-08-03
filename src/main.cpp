// main.cpp - Raylib Icon Maker by Demi 💖 PRO MAX (with GUI Text Input + Color Wheel + Palette)
// Saves/loads by user-typed path (SAVE_MODE/LOAD_MODE) for iOS/MacOS/Codemagic compatibility.
//
#include "raylib.h"
#include <vector>
#include <stack>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <string>

#if defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
  #define PLATFORM "iOS"
#elif defined(__APPLE__) && (!defined(TARGET_OS_IPHONE) || !TARGET_OS_IPHONE)
  #define PLATFORM "MACOS"
#elif defined(__ANDROID__)
  #define PLATFORM "ANDROID"
#elif defined(_WIN32)
  #define PLATFORM "WINDOWS"
#else
  #define PLATFORM "LINUX"
#endif

const int WIDTH = 512, HEIGHT = 512;
int brushSize = 8;
bool brushIsCircle = true;
int currentFrame = 0;
const int NUM_FRAMES = 4;

Color drawColor = BLACK;
std::vector<Color> colorHistory;
std::stack<unsigned char*> undoStack, redoStack;
bool showAI = false;
bool showInfo = false;

enum InputMode { NONE, RGB_MODE, HSV_MODE, HEX_MODE, LOAD_MODE, SAVE_MODE };
InputMode inputMode = NONE;
char inputBuffer[128] = "";
int inputLength = 0;
bool inputActive = false;
<<<<<<< HEAD
bool rubberActive = false;

char keyLayout[] = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM#";
int selectedKey = -1;
=======
//NEW RUBBER
bool rubberActive = false;
std::string lastLoadError, lastSaveMsg;
>>>>>>> Initial commit: Raylib Icon Maker

// Color conversion helpers
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
<<<<<<< HEAD
            drawColor = (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
=======
            drawColor = {(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
>>>>>>> Initial commit: Raylib Icon Maker
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
<<<<<<< HEAD
                drawColor = (Color){ r, g, b, 255 };
=======
                unsigned char a = 255;
                if (hex.length() == 8) a = std::stoi(hex.substr(6, 2), nullptr, 16);
                drawColor = {r, g, b, a};
>>>>>>> Initial commit: Raylib Icon Maker
                AddColor(drawColor);
            } catch (...) {}
        }
    } else if (inputMode == LOAD_MODE) {
        std::string loadPath = inputBuffer;
        Image loadedImage = LoadImage(loadPath.c_str());
        if (loadedImage.data && loadedImage.width == WIDTH && loadedImage.height == HEIGHT) {
            PushUndo(frames);
            UnloadImage(frames[currentFrame]);
            frames[currentFrame] = loadedImage;
            lastLoadError = "Loaded from " + loadPath;
        } else {
            if (loadedImage.data) UnloadImage(loadedImage);
            lastLoadError = "Load failed (wrong size or unreadable): " + loadPath;
        }
    } else if (inputMode == SAVE_MODE) {
        std::string savePath = inputBuffer;
        if (ExportImage(frames[currentFrame], savePath.c_str())) {
            lastSaveMsg = "Saved as " + savePath;
        } else {
            lastSaveMsg = "Failed to save: " + savePath;
        }
    }
    inputActive = false;
    inputBuffer[0] = '\0';
    inputLength = 0;
    inputMode = NONE;
}

// Palette generation
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

// Color wheel
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

<<<<<<< HEAD
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
=======
int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Visual Icon Maker");
>>>>>>> Initial commit: Raylib Icon Maker
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
            Color originalColor = drawColor;
            if (rubberActive) drawColor = WHITE;  // Erase
            ApplyBrush(mx, my, frames);
            drawColor = originalColor;
        }

        // --- Input triggers ---
        if (!inputActive) {
            if (IsKeyPressed(KEY_R)) { inputActive = true; inputMode = RGB_MODE; }
            else if (IsKeyPressed(KEY_C)) { inputActive = true; inputMode = HSV_MODE; }
            else if (IsKeyPressed(KEY_H)) { inputActive = true; inputMode = HEX_MODE; }
            else if (IsKeyPressed(KEY_L)) { inputActive = true; inputMode = LOAD_MODE; inputBuffer[0] = '\0'; inputLength = 0; }
            else if (IsKeyPressed(KEY_S)) { inputActive = true; inputMode = SAVE_MODE; inputBuffer[0] = '\0'; inputLength = 0; }
        }

        // --- Text GUI input field for active input mode ---
        if (inputActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (inputLength < 127 && (isprint(key) || key == '#')) {
                    inputBuffer[inputLength++] = (char)key;
                    inputBuffer[inputLength] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputLength > 0) inputBuffer[--inputLength] = '\0';
            if (IsKeyPressed(KEY_ENTER)) ProcessInput(frames);
        }

        // --- Main keys and logic, only when not in input box ---
        if (!inputActive) {
            if (IsKeyPressed(KEY_X)) { PushUndo(frames); frames[currentFrame] = GenImageColor(WIDTH, HEIGHT, WHITE); }
            if (IsKeyPressed(KEY_Z) && !undoStack.empty()) {
                unsigned char* last = undoStack.top(); undoStack.pop();
                unsigned char* redoSnap = (unsigned char*)malloc(WIDTH * HEIGHT * 4);
                memcpy(redoSnap, frames[currentFrame].data, WIDTH * HEIGHT * 4);
                redoStack.push(redoSnap);
                memcpy(frames[currentFrame].data, last, WIDTH * HEIGHT * 4);
                free(last);
            }
            if (IsKeyPressed(KEY_Y) && !redoStack.empty()) {
                unsigned char* redoSnap = redoStack.top(); redoStack.pop();
                PushUndo(frames);
                memcpy(frames[currentFrame].data, redoSnap, WIDTH * HEIGHT * 4);
                free(redoSnap);
            }
            if (IsKeyPressed(KEY_B)) brushIsCircle = !brushIsCircle;
            if (IsKeyPressed(KEY_LEFT_BRACKET)) brushSize = std::max(1, brushSize - 1);
            if (IsKeyPressed(KEY_RIGHT_BRACKET)) brushSize = std::min(100, brushSize + 1);
            if (IsKeyPressed(KEY_N)) currentFrame = (currentFrame + 1) % NUM_FRAMES;
            if (IsKeyPressed(KEY_P)) currentFrame = (currentFrame - 1 + NUM_FRAMES) % NUM_FRAMES;
            if (IsKeyPressed(KEY_A)) showAI = !showAI;
            if (IsKeyPressed(KEY_I)) showInfo = !showInfo;
            if (IsKeyPressed(KEY_TAB)) {
                rubberActive = !rubberActive;
                SetMouseCursor(rubberActive ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
            }
        }

        UpdateTexture(canvas, frames[currentFrame].data);

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(canvas, 0, 0, WHITE);

        for (int i = 0; i < (int)colorHistory.size(); i++) {
            DrawRectangle(10 + i * 30, HEIGHT - 40, 28, 28, colorHistory[i]);
        }
        if (showAI) DrawRectangle(WIDTH - 160, 10, 150, 150, LIGHTGRAY);

        // Draw preview brush at mouse
        for (int dy = -brushSize; dy <= brushSize; dy++)
            for (int dx = -brushSize; dx <= brushSize; dx++)
                if (!brushIsCircle || dx * dx + dy * dy <= brushSize * brushSize)
                    DrawPixel(mx + dx, my + dy, drawColor);

        // 🆕 Color Wheel
        int wheelX = WIDTH - 100, wheelY = HEIGHT - 120;
        DrawColorWheel(wheelX, wheelY, 50);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !inputActive) {
            int dx = mx - wheelX, dy = my - wheelY;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= 50) {
                float angle = atan2f(dy, dx);
                if (angle < 0) angle += 2 * PI;
                float hue = angle / (2 * PI);
                drawColor = HSVtoRGB(hue, 1, 1);
                AddColor(drawColor);
            }
        }
        // 🆕 Palette based on drawColor
        std::vector<Color> palette = GeneratePalette(drawColor);
        for (int i = 0; i < (int)palette.size(); i++) {
            int x = WIDTH - 160 + i * 30, y = HEIGHT - 60;
            DrawRectangle(x, y, 28, 28, palette[i]);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !inputActive &&
                mx >= x && mx <= x + 28 && my >= y && my <= y + 28) {
                drawColor = palette[i];
                AddColor(drawColor);
            }
        }

        std::string credits = std::string("main.cpp - Raylib Icon Maker by Demi 💖 PRO MAX (") + PLATFORM + ")";
        DrawText(credits.c_str(), 10, 10, 10, GRAY);

        // --- Input GUI box ---
        if (inputActive) {
            DrawRectangle(50, HEIGHT / 2 - 30, 400, 60, LIGHTGRAY);
            DrawRectangleLines(50, HEIGHT / 2 - 30, 400, 60, DARKGRAY);
            std::string prompt;
            if (inputMode == RGB_MODE) prompt = "Enter R G B (0-255): ";
            else if (inputMode == HSV_MODE) prompt = "Enter H S V (0-1): ";
            else if (inputMode == HEX_MODE) prompt = "Enter HEX (#RRGGBB): ";
            else if (inputMode == LOAD_MODE) prompt = "Enter path to LOAD from:";
            else if (inputMode == SAVE_MODE) prompt = "Enter path to SAVE as:";
            else prompt = "Input:";
            DrawText(prompt.c_str(), 60, HEIGHT/2 - 22, 18, GRAY);
            DrawText(inputBuffer, 60, HEIGHT / 2 - 2, 24, BLACK);
            DrawText("Press ENTER | BACKSPACE to edit", 60, HEIGHT/2 + 20, 12, GRAY);
        }

        if (!lastLoadError.empty()) { DrawText(lastLoadError.c_str(), 10, HEIGHT-20, 12, MAROON); }
        if (!lastSaveMsg.empty()) { DrawText(lastSaveMsg.c_str(), WIDTH/2, HEIGHT-20, 12, DARKGREEN); }

        // --- Info popup ---
        if (showInfo) {
            DrawRectangle(60, 60, WIDTH - 120, HEIGHT - 120, Fade(LIGHTGRAY, 0.95f));
            DrawRectangleLines(60, 60, WIDTH - 120, HEIGHT - 120, GRAY);
            int x = 80, y = 80;
            DrawText("🎨 IconMaker Controls:", x, y, 24, BLACK); y += 40;
            DrawText("Left Click       - Paint | Tab - Toggle Eraser", x, y, 18, DARKGRAY); y += 25;
            DrawText("R   - RGB mode     C - HSV mode     H - HEX mode", x, y, 18, DARKGRAY); y += 25;
            DrawText("L   - Load by path | S - Save by path", x, y, 18, DARKGRAY); y += 25;
            DrawText("B   - Brush shape toggle         [/] - Size", x, y, 18, DARKGRAY); y += 25;
            DrawText("N / P - Next/Prev frame | X - Clear frame", x, y, 18, DARKGRAY); y += 25;
            DrawText("Z / Y - Undo/Redo  | I - Toggle this help", x, y, 18, DARKGRAY); y += 25;
            DrawText("Use SAVE/LOAD to specify filename and path!", x, y+10, 18, MAROON);
            DrawText("Press [I] to close this window.", x, y+50, 16, GRAY);
        }

        EndDrawing();
    }

    UnloadTexture(canvas);
    for (int i = 0; i < NUM_FRAMES; i++) UnloadImage(frames[i]);
    while (!undoStack.empty()) { free(undoStack.top()); undoStack.pop(); }
    while (!redoStack.empty()) { free(redoStack.top()); redoStack.pop(); }

    CloseWindow();
}

// main.cpp - Raylib Icon Maker by Demi 💖 PRO MAX (GUI Text Input ✓, Color Wheel ✓, Palette ✓, RGB/HSV/HEX, Eraser, Undo/Redo, Save/Load by path)
