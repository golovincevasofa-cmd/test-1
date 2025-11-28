#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#pragma comment(linker, "/subsystem:windows /entry:wWinMainCRTStartup")

// Глобальные переменные 1
int steps = 0;
int gold = 0;
const int N = 10;  // Размеры карты 1 
const int M = 10;
int** map = NULL;

// Размеры ячеек 2
const int WIDTH = 30;
const int HEIGHT = 20;

// Прототипы функций
void InitializeMap();
void drawMap(HDC hdc);
void Left();
void Right();
void Up();
void Down();
void Save();
void Load();

// НОВЫЕ ФУНКЦИИ ДЛЯ ЗАДАНИЙ
void InsertWallLeft();      // Задача 8.2.4 - K - стена слева
void InsertGoldRight();     // Задача 8.2.4 - R - золото справа
void ToggleWallUp();        // Задача 8.2.5 - U - переключить стену сверху
void ToggleGoldDown();      // Задача 8.2.6 - D - переключить золото снизу
void DestroyWallsRight();   // Задача 8.2.7 - Z - уничтожить стены справа

// Функция для отрисовки панели управления
void DrawControlPanel(HDC hdc);

// Инициализация карты
void InitializeMap() {
    if (map) {
        for (int i = 0; i < N; i++)
            free(map[i]);
        free(map);
    }

    map = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        map[i] = (int*)malloc(M * sizeof(int));
        for (int j = 0; j < M; j++) {
            // Простая начальная карта: границы - стены, внутри - пустота, один игрок
            if (i == 0 || i == N - 1 || j == 0 || j == M - 1)
                map[i][j] = 2; // стены
            else
                map[i][j] = 0; // пусто
        }
    }
    // Размещаем игрока
    map[1][1] = 1;
    // Размещаем немного золота
    map[2][2] = 3;
    map[3][5] = 3;
    map[5][7] = 3;
}

void drawMap(HDC hdc) {
    // кисть для пустого поля
    HBRUSH hBrushEmptyCell = CreateSolidBrush(RGB(200, 200, 200)); // серый
    // кисть для поля с золотом
    HBRUSH hBrushGold = CreateSolidBrush(RGB(255, 255, 0)); // желтый
    // кисть для стены
    HBRUSH hBrushWall = CreateSolidBrush(RGB(0, 0, 0)); // черный
    // кисть для игрока
    HBRUSH hBrushMan = CreateSolidBrush(RGB(0, 0, 255)); // синий

    // Коды ячеек:
    // 0 - свободна
    // 1 - игрок
    // 2 - препятствие
    // 3 - золото
    HBRUSH brush[4] = { hBrushEmptyCell, hBrushMan, hBrushWall, hBrushGold };

    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            int x1 = j * WIDTH;
            int x2 = (j + 1) * WIDTH;
            int y1 = i * HEIGHT;
            int y2 = (i + 1) * HEIGHT;

            RECT r = { x1, y1, x2, y2 };
            FillRect(hdc, &r, brush[map[i][j]]);
        }
    }

    // Все кисти удаляем!!!
    for (i = 0; i < 4; i++)
        DeleteObject(brush[i]);
}



void Left() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 1; j < M; j++) {
            if (map[i][j] == 1) { // если в map[i][j] игрок
                if (map[i][j - 1] == 0) { // если слева от игрока - пустая клетка
                    map[i][j - 1] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i][j - 1] == 3) {// если слева от игрока - золото
                    map[i][j - 1] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}

void Right() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = M - 2; j >= 0; j--) {
            if (map[i][j] == 1) { // если в map[i][j] игрок
                if (map[i][j + 1] == 0) {
                    map[i][j + 1] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i][j + 1] == 3) {
                    map[i][j + 1] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}

void Up() {
    int i, j;
    for (i = 1; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (map[i][j] == 1) { // если в map[i][j] игрок
                if (map[i - 1][j] == 0) {
                    map[i - 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i - 1][j] == 3) {
                    map[i - 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}

void Down() {
    int i, j;
    for (i = N - 2; i >= 0; i--) {
        for (j = 0; j < M; j++) {
            if (map[i][j] == 1) { // если в map[i][j] игрок
                if (map[i + 1][j] == 0) {
                    map[i + 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i + 1][j] == 3) {
                    map[i + 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}


// Задача 8.2.4 - При нажатии 'K' вставить стену слева от игрока
void InsertWallLeft() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 1; j < M; j++) {
            if (map[i][j] == 1) { // нашли игрока
                if (map[i][j - 1] != 2 && map[i][j - 1] != 1) { // если слева не стена и не игрок
                    map[i][j - 1] = 2; // ставим стену
                    return;
                }
            }
        }
    }
}

// Задача 8.2.4 - При нажатии 'R' вставить золото справа от игрока
void InsertGoldRight() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M - 1; j++) {
            if (map[i][j] == 1) { // нашли игрока
                if (map[i][j + 1] != 3 && map[i][j + 1] != 1) { // если справа не золото и не игрок
                    map[i][j + 1] = 3; // ставим золото
                    return;
                }
            }
        }
    }
}

// Задача 8.2.5 - При нажатии 'U' переключить стену сверху (создать/удалить)
void ToggleWallUp() {
    int i, j;
    for (i = 1; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (map[i][j] == 1) { // нашли игрока
                if (map[i - 1][j] == 2) { // если сверху стена - убираем её
                    map[i - 1][j] = 0; // делаем пустой
                }
                else if (map[i - 1][j] != 1) { // если сверху не игрок - ставим стену
                    map[i - 1][j] = 2; // ставим стену
                }
                return;
            }
        }
    }
}

// Задача 8.2.6 - При нажатии 'D' переключить золото снизу (создать/удалить)
void ToggleGoldDown() {
    int i, j;
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < M; j++) {
            if (map[i][j] == 1) { // нашли игрока
                if (map[i + 1][j] == 3) { // если снизу золото - убираем его
                    map[i + 1][j] = 0; // делаем пустой
                    if (gold > 0) gold--; // уменьшаем счетчик золота
                }
                else if (map[i + 1][j] != 1) { // если снизу не игрок - ставим золото
                    map[i + 1][j] = 3; // ставим золото
                }
                return;
            }
        }
    }
}

// Задача 8.2.7 - При нажатии 'Z' уничтожить стены справа до границы
void DestroyWallsRight() {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            if (map[i][j] == 1) { // нашли игрока
                // Уничтожаем стены справа от игрока до правой границы
                for (int k = j + 1; k < M; k++) {
                    if (map[i][k] == 2) { // если это стена
                        map[i][k] = 0; // уничтожаем стену
                    }
                }
                return;
            }
        }
    }
}

void Save() {
    FILE* fout;
    errno_t err = fopen_s(&fout, "game.txt", "w");
    if (err != 0 || fout == NULL) {
        MessageBox(NULL, _T("Выходной файл не создался"), _T("Ошибка"), MB_OK);
        return;
    }

    fprintf(fout, "%d %d\n", N, M);

    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            fprintf(fout, "%d ", map[i][j]);
        }
        fprintf(fout, "\n");
    }

    fprintf(fout, "%d %d", steps, gold);
    fclose(fout);
}

void Load() {
    FILE* fin;
    errno_t err = fopen_s(&fin, "game.txt", "r");
    if (err != 0 || fin == NULL) {
        MessageBox(NULL, _T("Входной файл не создался"), _T("Ошибка"), MB_OK);
        return;
    }

    int n, m;
    fscanf_s(fin, "%d", &n);
    fscanf_s(fin, "%d", &m);

    // Освобождаем старую карту
    if (map) {
        for (int i = 0; i < N; i++)
            free(map[i]);
        free(map);
    }

    // Создаем новую карту
    map = (int**)malloc(N * sizeof(int*));
    int i, j;
    for (i = 0; i < N; i++) {
        map[i] = (int*)malloc(M * sizeof(int));
        for (j = 0; j < M; j++) {
            fscanf_s(fin, "%d", &map[i][j]);
        }
    }

    fscanf_s(fin, "%d", &steps);
    fscanf_s(fin, "%d", &gold);
    fclose(fin);
}

// Функция для отрисовки панели управления
void DrawControlPanel(HDC hdc) {
    int startY = N * HEIGHT + 10;
    int lineHeight = 20;

    // Сохраняем старый цвет текста
    COLORREF oldTextColor = SetTextColor(hdc, RGB(0, 0, 0));

    // Заголовок
    TextOut(hdc, 10, startY, _T("Панель управления"), 26);

    // Движение
    TextOut(hdc, 10, startY + lineHeight, _T("Движение:"), 10);
    TextOut(hdc, 10, startY + lineHeight * 2, _T("Стрелочки - Перемещение "), 10);

    // Основные функции
    TextOut(hdc, 10, startY + lineHeight * 3, _T("Основные функции:"), 17);
    TextOut(hdc, 20, startY + lineHeight * 4, _T("S - Сохранить игру"), 19);
    TextOut(hdc, 20, startY + lineHeight * 5, _T("L - Загрузить игру"), 19);

    // Специальные функции
    TextOut(hdc, 10, startY + lineHeight * 6, _T("Специальные функции:"), 22);
    TextOut(hdc, 20, startY + lineHeight * 7, _T("K - Поставить стену слева"), 26);
    TextOut(hdc, 20, startY + lineHeight * 8, _T("R - Поставить золото справа"), 28);
    TextOut(hdc, 20, startY + lineHeight * 9, _T("U - Переключить стену сверху"), 30);
    TextOut(hdc, 20, startY + lineHeight * 10, _T("D - Переключить золото снизу"), 31);
    TextOut(hdc, 20, startY + lineHeight * 11, _T("Z - Уничтожить стены справа"), 30);


    // Восстанавливаем старый цвет
    SetTextColor(hdc, oldTextColor);
}

const TCHAR CLASS_NAME[] = _T("Sample Window Class");

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Главная функция Windows приложения
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Инициализация карты
    InitializeMap();

    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Создание окна (увеличиваем высоту для панели управления)
    int windowWidth = M * WIDTH + 16;
    int windowHeight = N * HEIGHT + 400; // Увеличили высоту для панели управления

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        _T("Игра - Сбор золота с панелью управления"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    // Цикл сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Освобождение памяти карты
    if (map) {
        for (int i = 0; i < N; i++)
            free(map[i]);
        free(map);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Отрисовываем карту
        drawMap(hdc);

        // Отображаем статистику
        TCHAR stats[100];
        _stprintf_s(stats, 100, _T("Шаги: %d | Золото: %d"), steps, gold);
        TextOut(hdc, 10, N * HEIGHT + 5, stats, _tcslen(stats));

        // Отрисовываем панель управления
        DrawControlPanel(hdc);

        EndPaint(hWnd, &ps);
    }
                 break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_DOWN:
            Down();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            Left();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            Up();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            Right();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x53: // S - Save
            Save();
            break;
        case 0x4C: // L - Load
            Load();
            InvalidateRect(hWnd, NULL, TRUE);
            break;

           
        case 0x4B: // K - стена слева (вместо L)
            InsertWallLeft();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x52: // R - золото справа
            InsertGoldRight();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x55: // U - переключить стену сверху
            ToggleWallUp();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x44: // D - переключить золото снизу
            ToggleGoldDown();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x5A: // Z - уничтожить стены справа
            DestroyWallsRight();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}