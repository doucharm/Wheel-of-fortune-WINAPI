#include "framework.h"
#include "Project8.h"
#include <codecvt>
#include<iostream>
#include<fstream>
#include <windows.h>
#include<vector>
#include <wingdi.h>

#include "Header.h"
#include "Wheel.h"
#include "Player.h"
#define MAX_LOADSTRING 100
using namespace std;

HINSTANCE hInst;                              
WCHAR szTitle[MAX_LOADSTRING];                
WCHAR szWindowClass[MAX_LOADSTRING]; 



HWND handleforwindow2,handleforwindow3;
HWND box_for_phrase, button_for_solve;

vector<Player>Player_list;
string question, answer;
String_Reveal Display_String;
int round_of_playing;
string guessed_letters = "";
Wheel wheel;
bool Entering_Char = false;
bool Entering_Phrase = false;
bool End_game = false;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow3(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam);


void Draw_BackGround(LPCWSTR background_file, HWND hWnd)
{
    HBITMAP bitmap_to_draw = (HBITMAP)LoadImage(NULL, background_file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    PAINTSTRUCT     ps;
    HDC             hdc;
    BITMAP          bitmap;
    HDC             hdcMem;
    HGDIOBJ         oldBitmap;
    hdc = BeginPaint(hWnd, &ps);
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, bitmap_to_draw);
    GetObject(bitmap_to_draw, sizeof(bitmap), &bitmap);
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    EndPaint(hWnd, &ps);
}
string Get_Guessing_String(String_Reveal current_guess)
{
    string pom;
    for (int i = 0; i < current_guess.line.length(); i++)
    {
        if (current_guess.is_revealed.at(i) == true)
        {
            pom += current_guess.line[i];
        }
        else pom += '_';
        pom += " ";
    }
    return pom;
}
void Draw_Question_and_Answer(HDC hdc, HWND hWnd)
{

    
    long lfHeight = -MulDiv(26, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 700, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
    SelectObject(hdc, hFont);
    RECT rc;
    GetClientRect(hWnd, &rc);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(question);
    LPCWSTR result = wide.c_str();
    DrawText(hdc, result, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    lfHeight = -MulDiv(36, GetDeviceCaps(hdc, LOGPIXELSY), 72);

    HFONT hFont_for_GUESSING = CreateFont(lfHeight, 0, 0, 0, 800, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
    rc.top = 80;
    rc.left = 100;
    rc.right = 1200;
    rc.bottom = 2500;
    SelectObject(hdc, hFont_for_GUESSING);
    string display_line;
    display_line = Get_Guessing_String(Display_String);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter1;
    std::wstring wide1 = converter1.from_bytes(display_line);
    LPCWSTR result1 = wide1.c_str();
    DrawText(hdc, result1, -1, &rc, DT_CENTER | DT_VCENTER | DT_EXPANDTABS);
  
}
void Host_Speech(string line, HDC hdc)
{

    long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
    SelectObject(hdc, hFont);

    if (line.find('\n') == -1)
    {
        int pom = line.length();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter3;
        std::wstring wide3 = converter3.from_bytes(line);
        TextOut(hdc, 1000, 260, wide3.c_str(), pom);
    }
    else
    {
        int k = line.find('\n');
        string line1 = line.substr(0, k);
        string line2 = line.substr(k);
        int pom1 = line1.length();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter3;
        std::wstring wide3 = converter3.from_bytes(line1);
        TextOut(hdc, 1000, 260, wide3.c_str(), pom1);
        int pom2 = line2.length();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter4;
        std::wstring wide4 = converter3.from_bytes(line2);
        TextOut(hdc, 1000, 290, wide4.c_str(), pom2);
    }


}
void Display_Money(HDC hdc)
{
    long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Ariel");
    SelectObject(hdc, hFont);
    TCHAR buf[256];
    _itot_s(Player_list.at(0).Get_Cash(), buf, 10);
    if (Player_list.at(0).Get_Cash() == 0)
    {
        TextOut(hdc, 80, 510, L"0000", 4);
    }  else TextOut(hdc, 80, 510, buf, 4);
    _itot_s(Player_list.at(1).Get_Cash(), buf, 10);
    if (Player_list.at(1).Get_Cash() == 0)
    {
        TextOut(hdc, 500, 510, L"0000", 4);
    }  else TextOut(hdc, 500, 510, buf, 4);
    
    _itot_s(Player_list.at(2).Get_Cash(), buf, 10);
    if (Player_list.at(2).Get_Cash() == 0)
    {
        TextOut(hdc, 1000, 510, L"0000", 4);
    } else TextOut(hdc, 1000, 510, buf, 4);
}
char Bot1_Guess(HDC hdc)
{
    int pom3 = rand() % 20 + 65;
    int n = Get_Procent(Display_String);
    long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Ariel");
    SelectObject(hdc, hFont);
  
    if (n > pom3)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;
        std::wstring wide2 = converter2.from_bytes(answer);
        int pom = wide2.length();
        TextOut(hdc, 100, 425, L"The hidden phrase is", 21);
        TextOut(hdc, 100, 450, wide2.c_str(), pom);
        End_game = true;
        Player_list.at(0).Phrase_Guessed();
        Sleep(1000);
        return 0;
    }
    else
    {
        char c;
        do
        {
            int m = rand() % 26 + 65;
            c = (char)m;
        } while (guessed_letters.find(c) != -1);
        string voice = "I will guess the letter ";
        voice += c;
        voice += ' ';
        long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Ariel");
        SelectObject(hdc, hFont);
        int pom = voice.length();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;
        std::wstring wide2 = converter2.from_bytes(voice);
        TextOut(hdc, 100, 450, wide2.c_str(), pom);
        return c;
    }
}
char Bot2_Guess(HDC hdc)
{
    int pom3 = rand() % 20 + 65;
    int n = Get_Procent(Display_String);
    long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Ariel");
    SelectObject(hdc, hFont);

    if (n > pom3)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;
        std::wstring wide2 = converter2.from_bytes(answer);
        int pom = wide2.length();
        TextOut(hdc, 1000, 425, L"Is the answer phrase", 21);
        TextOut(hdc, 1000, 450, wide2.c_str(), pom);
        Player_list.at(2).Phrase_Guessed();
        End_game = true;
        Sleep(1000);
        return 0;
    }
    else
    {
        char c;
        do
        {
            int m = rand() % 26 + 65;
            c = (char)m;
        } while (guessed_letters.find(c) != -1);
        string voice = "Are there any letter ";
        voice += c;
        voice += ' ';
        long lfHeight = -MulDiv(14, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Ariel");
        SelectObject(hdc, hFont);
        int pom = voice.length();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;
        std::wstring wide2 = converter2.from_bytes(voice);
        TextOut(hdc, 1000, 450, wide2.c_str(), pom);
        return c;
    }
}
void Show_Turn(HDC hdc)
{
    long lfHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
    SelectObject(hdc, hFont);
    int k = round_of_playing % 3;
    switch (k)
    {
    case 0:
    {
        TextOut(hdc, 500, 20, L"Bot 1's  turn", 14);
        break;
    }
    case 1:
    {
        TextOut(hdc, 500, 20, L"Player's turn", 14);
        break;
    }
    case 2:
    {
        TextOut(hdc, 500, 20, L"Bot 2's  turn", 14);
        break;
    }
    }

}
bool Is_Winner()
{
    for (int i = 0; i < Player_list.size(); i++)
    {
        if (Player_list.at(1).Get_Cash() < Player_list.at(i).Get_Cash())
        {
            return false;
        }
    }
    return true;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
   
    Player bot1;
    Player bot2;
    Player player;
    Player_list.insert(Player_list.end(), { bot1, player, bot2 });
    
    WNDCLASSEX windowclassforwindow2;
    ZeroMemory(&windowclassforwindow2, sizeof(WNDCLASSEX));
    windowclassforwindow2.cbClsExtra = NULL;
    windowclassforwindow2.cbSize = sizeof(WNDCLASSEX);
    windowclassforwindow2.cbWndExtra = NULL;
    windowclassforwindow2.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowclassforwindow2.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowclassforwindow2.hIcon = NULL;
    windowclassforwindow2.hIconSm = NULL;
    windowclassforwindow2.hInstance = hInst;
    windowclassforwindow2.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
    windowclassforwindow2.lpszClassName = L"window class2";
    windowclassforwindow2.lpszMenuName = NULL;
    windowclassforwindow2.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&windowclassforwindow2))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Window class creation failed for window 2",
            L"Window Class Failed",
            MB_ICONERROR);
    }
    handleforwindow2 = CreateWindowEx(NULL,
        windowclassforwindow2.lpszClassName,
        L"Wheel of forutune",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1280,
        620,
        NULL,
        NULL,
        hInst,
        NULL);


    if (!handleforwindow2)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT8));

    MSG msg;
    
    WNDCLASSEX windowclassforwindow3;
    ZeroMemory(&windowclassforwindow3, sizeof(WNDCLASSEX));
    windowclassforwindow3.cbClsExtra = NULL;
    windowclassforwindow3.cbSize = sizeof(WNDCLASSEX);
    windowclassforwindow3.cbWndExtra = NULL;
    windowclassforwindow3.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowclassforwindow3.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowclassforwindow3.hIcon = NULL;
    windowclassforwindow3.hIconSm = NULL;
    windowclassforwindow3.hInstance = hInst;
    windowclassforwindow3.lpfnWndProc = (WNDPROC)windowprocessforwindow3;
    windowclassforwindow3.lpszClassName = L"window class3";
    windowclassforwindow3.lpszMenuName = NULL;
    windowclassforwindow3.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&windowclassforwindow3))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Window class creation failed for window 2",
            L"Window Class Failed",
            MB_ICONERROR);
    }
    handleforwindow3 = CreateWindowEx(NULL,
        windowclassforwindow3.lpszClassName,
        L"End screen",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1280,
        620,
        NULL,
        NULL,
        hInst,
        NULL);


    if (!handleforwindow3)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }
    
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    while (GetMessage(&msg, handleforwindow2, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    while (GetMessage(&msg, handleforwindow3, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT8);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindow(TEXT("button"), TEXT("START"),
            WS_VISIBLE | WS_CHILD,
            110, 100, 150, 50,
            hWnd, (HMENU)1, NULL, NULL);

        CreateWindow(TEXT("button"), TEXT("HOW TO PLAY"),
            WS_VISIBLE | WS_CHILD,
            110, 200, 150, 50,
            hWnd, (HMENU)2, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("HIGHSCORES"),
            WS_VISIBLE | WS_CHILD,
            110, 300, 150, 50,
            hWnd, (HMENU)3, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("EXIT"),
            WS_VISIBLE | WS_CHILD,
            110, 400, 150, 50,
            hWnd, (HMENU)4, NULL, NULL);
            break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            
            break;
        case 4:
        {
            DestroyWindow(hWnd);
            break;
        }
        case 2:
        {
            MessageBox(hWnd, L"Wait for your turn to spin the wheel\n Bankrupt havles your total money\n After clicking 'Guess a letter ' \n you can enter a single character from the board\n Incorrect phrase guess takeaway 500$\n Correct phrase guess give you 1200$.\n You can use the 2nd button to guess the hidden phrase\n which should be entered in all CAPS *Wait 4s before entering your guess phrase*", L"How to play", MB_ICONEXCLAMATION);
            break;
        }
        case 3:
        {
            MessageBox(hWnd, L"This feature is avaiable in the premium version",L"Pay me!!", MB_ICONEXCLAMATION);
            break;

        }
        case 1:
        {
            SetFocus(hWnd);
            ShowWindow(handleforwindow2, 1);
           
            break;
        }

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        Draw_BackGround(L"opening.bmp", hWnd);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
LRESULT CALLBACK windowprocessforwindow2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT     ps;
        HDC             hdc;
        HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"wheel_of_fortune.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        int num = round_of_playing % 3;
        BITMAP          bitmap;
        HDC             hdcMem;
        HGDIOBJ         oldBitmap;
        hdc = BeginPaint(hWnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, map_of_the_screen);
        GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
        Show_Turn(hdc);
        Display_Money(hdc);
        if (!End_game)
        {
            string host_line = "The theme is \n ";
            host_line += question;
            Host_Speech(host_line, hdc);
            Draw_Question_and_Answer(hdc, hWnd);
            wheel.Display(hdc);
            int wheel_handler = wheel.value;
            if (wheel_handler == 0)
            {
                Player_list.at(num).Lose_Money();
                round_of_playing++;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
            else if (wheel_handler == -1)
            {
                round_of_playing++;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
            else
            {

                switch (num)
                {
                case 0:
                {
                    char guessed = Bot1_Guess(hdc);
                    Host_Speech("Show me all the \n letters " + guessed, hdc);
                    Check_Guess_and_Display(&Display_String, guessed);
                    guessed_letters += guessed;
                    Player_list.at(0).Guess_Correct_Letter(Count_Occurences(answer, guessed), wheel.value);
                    if (Count_Occurences(answer, guessed) == 0)
                    {
                        round_of_playing++;
                    }
                    Sleep(1000);
                    InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    break;
                }
                case 2:
                {
                    char guessed = Bot2_Guess(hdc);
                    Host_Speech("Are there any \n    letters " + guessed, hdc);
                    Check_Guess_and_Display(&Display_String, guessed);
                    guessed_letters += guessed;
                    Sleep(1000);
                    Player_list.at(2).Guess_Correct_Letter(Count_Occurences(answer, guessed), wheel.value);
                    if (Count_Occurences(answer, guessed) == 0)
                    {
                        round_of_playing++;
                    }
                    Sleep(2000);
                    InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    break;
                }
                default:
                {
                    Host_Speech("What will your\nguess gonna be ?", hdc);
                    CreateWindow(TEXT("button"), TEXT("GUESS A LETTER"),
                        WS_VISIBLE | WS_CHILD,
                        450, 400, 120, 50,
                        hWnd, (HMENU)101, NULL, NULL);

                    CreateWindow(TEXT("button"), TEXT("GUESS PHRASE"),
                        WS_VISIBLE | WS_CHILD,
                        700, 400, 120, 50,
                        hWnd, (HMENU)102, NULL, NULL);

                    break;
                }
                }
            }
        }
        else 
        {
            Host_Speech("That's the correct answer\n 1000 point for you", hdc);
            Display_String.All_Correct();
            Draw_Question_and_Answer(hdc,hWnd);
            Sleep(3000);
            ShowWindow(handleforwindow3, 1);
        }
        break;

    }
    case WM_COMMAND:
    {

        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case 101:
        {
            Entering_Char = true;
            SetFocus(hWnd);   
            break;
        }
        case 102:
        {
            SetFocus(hWnd);
            box_for_phrase = CreateWindow(TEXT("Edit"), TEXT("Enter your guess"), WS_CHILD |WS_VISIBLE| WS_BORDER, 600, 480, 250, 30, hWnd, NULL, NULL, NULL);
            button_for_solve = CreateWindow(TEXT("button"), TEXT("Solve"), WS_CHILD|WS_VISIBLE, 600, 520, 50, 50, hWnd, (HMENU)103, NULL, NULL);
            break;
        }        
        case 103:
        {
            TCHAR val[40] = {0};
            int k = GetWindowTextLength(box_for_phrase);
            GetWindowText(box_for_phrase, val, k + 1);
            wstring test = val;
            string test2(test.begin(), test.end());
            if (test2 != answer)
            {
                Player_list.at(1).Phrase_Failed();
                round_of_playing++;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
            else
            {
                End_game = true;
                Player_list.at(1).Phrase_Guessed();
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);

        }
        ;
    }
    break;

    case WM_KEYDOWN:
    {

        if (Entering_Char)
        {
            SetFocus(hWnd);
            char letter_guessed = wParam;
            Check_Guess_and_Display(&Display_String, letter_guessed);
            Entering_Char = false;
            int k = Count_Occurences(answer, letter_guessed);
            if (guessed_letters.find(letter_guessed) != -1)
            {
                k = 0;
            }
            guessed_letters += letter_guessed;
            Player_list.at(1).Guess_Correct_Letter(k, wheel.value);
            if (k > 0)
            { 
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
            else
            {
                round_of_playing++;
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
        }


    }
    break;
    case WM_CREATE:
    {
        wheel = Wheel();
        Get_Question_and_Answer(&question, &answer);
        Display_String = Get_Guess_String(answer);
        round_of_playing = 0;
        break;

    }
   
    case WM_DESTROY:
    {
        return 0;
    }
    break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK windowprocessforwindow3(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
      
    case WM_CREATE:
    {
        
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT     ps;
        HDC             hdc;
        BITMAP          bitmap;
        HDC             hdcMem;
        HGDIOBJ         oldBitmap;
        hdc = BeginPaint(hWnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        long lfHeight = -MulDiv(36, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        HFONT hFont = CreateFont(lfHeight, 0, 0, 0, 700, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
        SelectObject(hdc, hFont);
        LPCWSTR d = L"YOUR SCORE:";
        TCHAR buf[256];
        _itot_s(Player_list.at(1).Get_Cash(), buf, 10);
        int k = rand() % 3;
        if (Is_Winner())
        {
            switch (k)
            {
            case 0:
            {
                HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"end5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                SelectObject(hdcMem, map_of_the_screen);
                GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, 0x00FFFFFF);
                TextOut(hdc, 100, 150, d,13);
                TextOut(hdc, 450, 150, buf, 4);
                break;
            }
            default:
            {
               
                HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"end4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                SelectObject(hdcMem, map_of_the_screen);
                GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, 0x00FFFFFF);
                TextOut(hdc,800, 100, d, 13);
                TextOut(hdc, 1150, 100, buf, 4);
            }
            }

        }
        else 
        {

            switch (k)
            {
            case 0:
            {
                HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"end3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                SelectObject(hdcMem, map_of_the_screen);        
                GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SetBkMode(hdc, TRANSPARENT);
                TextOut(hdc, 100, 50, d, 13);
                TextOut(hdc, 450, 50, buf, 4);
                break;
            }
            case 1:
            {
                HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"end2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                SelectObject(hdcMem, map_of_the_screen);
                GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, 0x00FFFFFF);
                TextOut(hdc, 200, 350, d, 13);
                TextOut(hdc, 550, 350, buf, 4);
                break;
            }
            default:
            {
               
                HBITMAP map_of_the_screen = (HBITMAP)LoadImage(NULL, L"end1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                SelectObject(hdcMem, map_of_the_screen);
                GetObject(map_of_the_screen, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, 0x00FFFFFF);
                TextOut(hdc, 300, 400, d, 13);
                TextOut(hdc, 650, 400, buf, 4);
            }
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

