#include <menu.h>
#include <morse.h>

using namespace std;

std::vector<MenuItem> items;
int selected = 0;
HANDLE hIn, hOut;
DWORD prevMode;

Menu::Menu(const std::vector<MenuItem>& options) : items(options)
{
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hIn, &prevMode);
    
    DWORD newMode = prevMode;
    newMode &= ~ENABLE_QUICK_EDIT_MODE;
    newMode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hIn, newMode);

   // hideCursor();
}

Menu::~Menu()
{
    SetConsoleMode(hIn, prevMode);
    showCursor();
}

void Menu::hideCursor()
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hOut, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &info);
}

void Menu::showCursor()
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hOut, &info);
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &info);
}

void Menu::gotoXY(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, pos);
}

void Menu::eraselines(int count)
{
    if (count > 0)
    {
        std::cout << "\x1b[2K"; // Delete current line
        for (int i = 1; i < count; i++)
        {
            std::cout << "\x1b[1A" // Move cursor up one line
                << "\x1b[2K"; // Delete the entire line
        }
        std::cout << "\r"; // Move cursor to the beginning of the line
    }
}

// Clear a few lines below the menu (where actions print) so previous longer output
// doesn't leave trailing characters when a shorter message is printed.
void Menu::clearOutputArea(int lines = 3)
{
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    if (!GetConsoleScreenBufferInfo(hOut, &sbi)) return;

    DWORD width = (DWORD)sbi.dwSize.X;
    int startRow = 2 + static_cast<int>(items.size()); // header (2 lines) + menu lines
    DWORD written = 0;

    for (int i = 0; i < lines; ++i)
    {
        COORD pos = { 0, static_cast<SHORT>(startRow + i) };
        FillConsoleOutputCharacterA(hOut, ' ', width, pos, &written);
        FillConsoleOutputAttribute(hOut, sbi.wAttributes, width, pos, &written);
    }

    // Place cursor at the first cleared line so action output appears there
    COORD outPos = { 0, static_cast<SHORT>(startRow) };
    SetConsoleCursorPosition(hOut, outPos);
}

void Menu::draw()
{
    gotoXY(0, 0);
    std::cout << "MORSE (Press F1 for help.)\n\n";
    for (int i = 0; i < items.size(); i++)
    {
        if (i == selected)
        {
            SetConsoleTextAttribute(hOut, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "(*)  " << items[i].label << "\n";
            SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else
        {
            std::cout << "( )  " << items[i].label << "\n";
        }
    }
}

// Wait for a console key-down event and return (consumes function keys properly)
void Menu::waitForKeyDown()
{
    INPUT_RECORD rec;
    DWORD read;
    while (ReadConsoleInput(hIn, &rec, 1, &read))
    {
        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown)
            break;
        // ignore everything else (key up, mouse events, etc.)
    }
}

void Menu::run()
{
    draw();
    INPUT_RECORD record;
    DWORD read;
    bool running = true;

    while (running)
    {
        ReadConsoleInput(hIn, &record, 1, &read);

        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
        {
            switch (record.Event.KeyEvent.wVirtualKeyCode)
            {
            case VK_UP:
                if (selected > 0)
                {
                    selected--;
                    draw();
                    clearOutputArea();
                    items[selected].action();
                }
                break;
            case VK_DOWN:
                if (selected < items.size() - 1)
                {
                    selected++;
                    draw();
                    clearOutputArea();
                    items[selected].action();
                }
                break;
            case VK_RETURN:
                clearOutputArea();
                items[selected].action();
                running = false;
                break;
            case VK_ESCAPE:
                std::cout << "\nExiting...\n";
                running = false;
                break;
            case VK_F1:
                system("cls");
                std::cout << "HELP SCREEN\n";
                Morse m; std::cout << m.GetHelpTxt();
                std::cout << "Press any key to return...\n";

                // Use ReadConsoleInput to wait for a key-down and consume console events properly.
                waitForKeyDown();

                // Re-apply the console input mode to ensure mouse input is enabled
                DWORD mode;
                if (GetConsoleMode(hIn, &mode))
                {
                    mode &= ~ENABLE_QUICK_EDIT_MODE;
                    mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
                    SetConsoleMode(hIn, mode);
                }

                // Clear any stray events (optional) and restore screen
                FlushConsoleInputBuffer(hIn);
                eraselines(2);
                // clear console
                system("cls");
                draw(); // redraw menu

                clearOutputArea();
                items[selected].action();
                break;
            }
            draw();
        }
        else if (record.EventType == MOUSE_EVENT)
        {
            auto& me = record.Event.MouseEvent;

            if (me.dwEventFlags == 0 && (me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
            {
                int row = me.dwMousePosition.Y - 2;
                if (row >= 0 && row < items.size())
                {
                    selected = row;
                    draw();
                    clearOutputArea();
                    items[selected].action();
                    //running = false;
                }
            }
        }
    }
}