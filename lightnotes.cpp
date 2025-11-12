#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <richedit.h>
#include <wctype.h>
#include <stdio.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shlwapi.lib")

// IDs
#define ID_EDIT 1001
#define ID_STATUS 1002
#define ID_FILE_NEW 2001
#define ID_FILE_OPEN 2002
#define ID_FILE_SAVE 2003
#define ID_FILE_SAVEAS 2004
#define ID_FILE_EXIT 2005
#define ID_EDIT_UNDO 3001
#define ID_EDIT_REDO 3002
#define ID_EDIT_CUT 3003
#define ID_EDIT_COPY 3004
#define ID_EDIT_PASTE 3005
#define ID_EDIT_DELETE 3006
#define ID_EDIT_SELECTALL 3007
#define ID_EDIT_FIND 3008
#define ID_EDIT_REPLACE 3009
#define ID_EDIT_GOTO 3010
#define ID_EDIT_DATETIME 3011
#define ID_FORMAT_WORDWRAP 4001
#define ID_FORMAT_FONT 4002
#define ID_VIEW_STATUS 5001
#define ID_VIEW_ZOOM_IN 5002
#define ID_VIEW_ZOOM_OUT 5003
#define ID_VIEW_ZOOM_RESET 5004
#define ID_VIEW_DARKMODE 5005
#define ID_VIEW_FULLSCREEN 5006
#define ID_TOOLS_STATS 6001
#define ID_TOOLS_UPPERCASE 6002
#define ID_TOOLS_LOWERCASE 6003
#define ID_HELP_ABOUT 7001

#define TIMER_STATUS 9001

// Variables globales
HWND hEdit, hStatus;
WCHAR szFileName[MAX_PATH] = L"";
WCHAR szFindText[256] = L"";
BOOL bModified = FALSE;
BOOL bWordWrap = TRUE;
BOOL bShowStatus = TRUE;
BOOL bDarkMode = FALSE;
int nZoom = 100;
HFONT hCurrentFont = NULL;

// Funciones
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateMenus(HWND);
void UpdateStatusBar(HWND);
void ApplyZoom(int);
void ToggleDarkMode(HWND);
BOOL SaveFile(HWND, LPCWSTR);
BOOL OpenFile(HWND, LPCWSTR);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow) {
    InitCommonControls();
    LoadLibrary(L"Riched20.dll");
    
    WNDCLASSEX wc = {sizeof(WNDCLASSEX)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = L"LightNotes";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    RegisterClassEx(&wc);
    
    HWND hWnd = CreateWindowEx(
        WS_EX_ACCEPTFILES,
        L"LightNotes",
        L"LightNotes - Ultra Fast & Light",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 650,
        NULL, NULL, hInst, NULL
    );
    
    ShowWindow(hWnd, nShow);
    UpdateWindow(hWnd);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void CreateMenus(HWND hWnd) {
    HMENU hMenu = CreateMenu();
    HMENU hSub;
    
    // Archivo
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING, ID_FILE_NEW, L"Nuevo\tCtrl+N");
    AppendMenu(hSub, MF_STRING, ID_FILE_OPEN, L"Abrir...\tCtrl+O");
    AppendMenu(hSub, MF_STRING, ID_FILE_SAVE, L"Guardar\tCtrl+S");
    AppendMenu(hSub, MF_STRING, ID_FILE_SAVEAS, L"Guardar como...");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_FILE_EXIT, L"Salir");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Archivo");
    
    // Edicion
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING, ID_EDIT_UNDO, L"Deshacer\tCtrl+Z");
    AppendMenu(hSub, MF_STRING, ID_EDIT_REDO, L"Rehacer\tCtrl+Y");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_EDIT_CUT, L"Cortar\tCtrl+X");
    AppendMenu(hSub, MF_STRING, ID_EDIT_COPY, L"Copiar\tCtrl+C");
    AppendMenu(hSub, MF_STRING, ID_EDIT_PASTE, L"Pegar\tCtrl+V");
    AppendMenu(hSub, MF_STRING, ID_EDIT_DELETE, L"Eliminar\tDel");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_EDIT_SELECTALL, L"Seleccionar todo\tCtrl+A");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_EDIT_FIND, L"Buscar...\tCtrl+F");
    AppendMenu(hSub, MF_STRING, ID_EDIT_REPLACE, L"Reemplazar...\tCtrl+H");
    AppendMenu(hSub, MF_STRING, ID_EDIT_GOTO, L"Ir a linea...\tCtrl+G");
    AppendMenu(hSub, MF_STRING, ID_EDIT_DATETIME, L"Fecha/Hora\tF5");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Edicion");
    
    // Formato
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING | (bWordWrap ? MF_CHECKED : 0), ID_FORMAT_WORDWRAP, L"Ajuste de linea");
    AppendMenu(hSub, MF_STRING, ID_FORMAT_FONT, L"Fuente...");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Formato");
    
    // Ver
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING | (bShowStatus ? MF_CHECKED : 0), ID_VIEW_STATUS, L"Barra de estado");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_VIEW_ZOOM_IN, L"Acercar\tCtrl++");
    AppendMenu(hSub, MF_STRING, ID_VIEW_ZOOM_OUT, L"Alejar\tCtrl+-");
    AppendMenu(hSub, MF_STRING, ID_VIEW_ZOOM_RESET, L"Zoom 100%\tCtrl+0");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING | (bDarkMode ? MF_CHECKED : 0), ID_VIEW_DARKMODE, L"Modo oscuro");
    AppendMenu(hSub, MF_STRING, ID_VIEW_FULLSCREEN, L"Pantalla completa\tF11");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Ver");
    
    // Herramientas
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING, ID_TOOLS_UPPERCASE, L"MAYUSCULAS");
    AppendMenu(hSub, MF_STRING, ID_TOOLS_LOWERCASE, L"minusculas");
    AppendMenu(hSub, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSub, MF_STRING, ID_TOOLS_STATS, L"Estadisticas");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Herramientas");
    
    // Ayuda
    hSub = CreatePopupMenu();
    AppendMenu(hSub, MF_STRING, ID_HELP_ABOUT, L"Acerca de...");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSub, L"Ayuda");
    
    SetMenu(hWnd, hMenu);
}

void UpdateStatusBar(HWND hWnd) {
    if(!bShowStatus) return;
    
    DWORD start, end;
    SendMessage(hEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    int line = SendMessage(hEdit, EM_LINEFROMCHAR, start, 0) + 1;
    int lineStart = SendMessage(hEdit, EM_LINEINDEX, -1, 0);
    int col = start - lineStart + 1;
    int totalLines = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
    int len = GetWindowTextLength(hEdit);
    
    WCHAR status[256];
    wsprintf(status, L"  Linea %d, Col %d  |  Lineas: %d  |  Caracteres: %d  |  Zoom: %d%%  %s",
        line, col, totalLines, len, nZoom, bModified ? L"[Modificado]" : L"");
    
    SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)status);
}

void ApplyZoom(int zoom) {
    nZoom = zoom;
    if(nZoom < 50) nZoom = 50;
    if(nZoom > 300) nZoom = 300;
    
    if(hCurrentFont) DeleteObject(hCurrentFont);
    
    // Fuente mas grande y bonita
    int fontSize = MulDiv(12, nZoom, 100);
    hCurrentFont = CreateFont(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas"
    );
    
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hCurrentFont, TRUE);
}

void ToggleDarkMode(HWND hWnd) {
    bDarkMode = !bDarkMode;
    
    if(bDarkMode) {
        // Modo oscuro
        SendMessage(hEdit, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
        SetClassLongPtr(hEdit, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(30, 30, 30)));
    } else {
        // Modo claro
        SendMessage(hEdit, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
        SetClassLongPtr(hEdit, GCLP_HBRBACKGROUND, (LONG_PTR)GetSysColorBrush(COLOR_WINDOW));
    }
    
    InvalidateRect(hEdit, NULL, TRUE);
    
    HMENU hMenu = GetMenu(hWnd);
    CheckMenuItem(hMenu, ID_VIEW_DARKMODE, bDarkMode ? MF_CHECKED : MF_UNCHECKED);
}

BOOL SaveFile(HWND hWnd, LPCWSTR szFile) {
    HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE) {
        MessageBox(hWnd, L"Error al guardar archivo", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    
    int len = GetWindowTextLength(hEdit);
    if(len > 0) {
        WCHAR* buf = (WCHAR*)GlobalAlloc(GPTR, (len + 1) * sizeof(WCHAR));
        if(buf) {
            GetWindowText(hEdit, buf, len + 1);
            DWORD written;
            
            // BOM UTF-16LE
            WORD bom = 0xFEFF;
            WriteFile(hFile, &bom, 2, &written, NULL);
            WriteFile(hFile, buf, len * sizeof(WCHAR), &written, NULL);
            
            GlobalFree(buf);
        }
    }
    
    CloseHandle(hFile);
    bModified = FALSE;
    
    WCHAR title[MAX_PATH + 50];
    wsprintf(title, L"%s - LightNotes", PathFindFileName(szFile));
    SetWindowText(hWnd, title);
    
    UpdateStatusBar(hWnd);
    return TRUE;
}

BOOL OpenFile(HWND hWnd, LPCWSTR szFile) {
    HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE) {
        MessageBox(hWnd, L"Error al abrir archivo", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    
    DWORD size = GetFileSize(hFile, NULL);
    if(size != INVALID_FILE_SIZE && size > 0) {
        char* buf = (char*)GlobalAlloc(GPTR, size + 4);
        if(buf) {
            DWORD read;
            ReadFile(hFile, buf, size, &read, NULL);
            buf[size] = 0;
            buf[size + 1] = 0;
            
            // Detectar BOM
            if(size >= 2 && (BYTE)buf[0] == 0xFF && (BYTE)buf[1] == 0xFE) {
                SetWindowText(hEdit, (WCHAR*)(buf + 2));
            } else if(size >= 3 && (BYTE)buf[0] == 0xEF && (BYTE)buf[1] == 0xBB && (BYTE)buf[2] == 0xBF) {
                int wlen = MultiByteToWideChar(CP_UTF8, 0, buf + 3, -1, NULL, 0);
                WCHAR* wbuf = (WCHAR*)GlobalAlloc(GPTR, wlen * sizeof(WCHAR));
                MultiByteToWideChar(CP_UTF8, 0, buf + 3, -1, wbuf, wlen);
                SetWindowText(hEdit, wbuf);
                GlobalFree(wbuf);
            } else {
                int wlen = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
                WCHAR* wbuf = (WCHAR*)GlobalAlloc(GPTR, wlen * sizeof(WCHAR));
                MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, wlen);
                SetWindowText(hEdit, wbuf);
                GlobalFree(wbuf);
            }
            
            GlobalFree(buf);
        }
    }
    
    CloseHandle(hFile);
    bModified = FALSE;
    
    WCHAR title[MAX_PATH + 50];
    wsprintf(title, L"%s - LightNotes", PathFindFileName(szFile));
    SetWindowText(hWnd, title);
    
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            CreateMenus(hWnd);
            
            // Crear control de edicion RICHEDIT
            hEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                RICHEDIT_CLASS,
                L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL |
                ES_NOHIDESEL | ES_WANTRETURN,
                0, 0, 0, 0,
                hWnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL
            );
            
            // Barra de estado
            hStatus = CreateWindowEx(
                0, STATUSCLASSNAME, NULL,
                WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                0, 0, 0, 0,
                hWnd, (HMENU)ID_STATUS, GetModuleHandle(NULL), NULL
            );
            
            // Aplicar fuente inicial grande
            ApplyZoom(100);
            
            // Configurar colores
            SendMessage(hEdit, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
            
            // Limites de texto
            SendMessage(hEdit, EM_SETLIMITTEXT, 0, 0);
            
            // Timer para actualizar status
            SetTimer(hWnd, TIMER_STATUS, 100, NULL);
            
            DragAcceptFiles(hWnd, TRUE);
            break;
        }
        
        case WM_SIZE: {
            RECT rc;
            GetClientRect(hWnd, &rc);
            
            int statusHeight = bShowStatus ? 24 : 0;
            
            SetWindowPos(hEdit, NULL, 0, 0, rc.right, rc.bottom - statusHeight, SWP_NOZORDER);
            SendMessage(hStatus, WM_SIZE, 0, 0);
            break;
        }
        
        case WM_TIMER:
            if(wParam == TIMER_STATUS) {
                UpdateStatusBar(hWnd);
            }
            break;
        
        case WM_DROPFILES: {
            HDROP hDrop = (HDROP)wParam;
            WCHAR file[MAX_PATH];
            if(DragQueryFile(hDrop, 0, file, MAX_PATH)) {
                OpenFile(hWnd, file);
                wcsncpy(szFileName, file, MAX_PATH - 1);
                szFileName[MAX_PATH - 1] = 0;
            }
            DragFinish(hDrop);
            break;
        }
        
        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case ID_FILE_NEW:
                    if(bModified) {
                        int ret = MessageBox(hWnd, L"Guardar cambios?", L"LightNotes", MB_YESNOCANCEL | MB_ICONQUESTION);
                        if(ret == IDCANCEL) break;
                        if(ret == IDYES && szFileName[0]) SaveFile(hWnd, szFileName);
                    }
                    SetWindowText(hEdit, L"");
                    szFileName[0] = 0;
                    bModified = FALSE;
                    SetWindowText(hWnd, L"Sin titulo - LightNotes");
                    break;
                
                case ID_FILE_OPEN: {
                    OPENFILENAME ofn = {sizeof(ofn)};
                    WCHAR file[MAX_PATH] = L"";
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = L"Archivos de texto (*.txt)\0*.txt\0Todos los archivos (*.*)\0*.*\0";
                    ofn.lpstrFile = file;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                    
                    if(GetOpenFileName(&ofn)) {
                        OpenFile(hWnd, file);
                        wcsncpy(szFileName, file, MAX_PATH - 1);
                        szFileName[MAX_PATH - 1] = 0;
                    }
                    break;
                }
                
                case ID_FILE_SAVE:
                    if(szFileName[0]) {
                        SaveFile(hWnd, szFileName);
                    } else {
                        SendMessage(hWnd, WM_COMMAND, ID_FILE_SAVEAS, 0);
                    }
                    break;
                
                case ID_FILE_SAVEAS: {
                    OPENFILENAME ofn = {sizeof(ofn)};
                    WCHAR file[MAX_PATH] = L"";
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = L"Archivos de texto (*.txt)\0*.txt\0Todos los archivos (*.*)\0*.*\0";
                    ofn.lpstrFile = file;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
                    ofn.lpstrDefExt = L"txt";
                    
                    if(GetSaveFileName(&ofn)) {
                        SaveFile(hWnd, file);
                        wcsncpy(szFileName, file, MAX_PATH - 1);
                        szFileName[MAX_PATH - 1] = 0;
                    }
                    break;
                }
                
                case ID_FILE_EXIT:
                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                    break;
                
                case ID_EDIT_UNDO:
                    SendMessage(hEdit, EM_UNDO, 0, 0);
                    break;
                
                case ID_EDIT_REDO:
                    SendMessage(hEdit, EM_REDO, 0, 0);
                    break;
                
                case ID_EDIT_CUT:
                    SendMessage(hEdit, WM_CUT, 0, 0);
                    break;
                
                case ID_EDIT_COPY:
                    SendMessage(hEdit, WM_COPY, 0, 0);
                    break;
                
                case ID_EDIT_PASTE:
                    SendMessage(hEdit, WM_PASTE, 0, 0);
                    break;
                
                case ID_EDIT_DELETE:
                    SendMessage(hEdit, EM_REPLACESEL, TRUE, (LPARAM)L"");
                    break;
                
                case ID_EDIT_SELECTALL:
                    SendMessage(hEdit, EM_SETSEL, 0, -1);
                    break;
                
                case ID_EDIT_FIND: {
                    WCHAR input[256] = L"";
                    if(MessageBox(hWnd, L"Usar funcion Buscar de Windows?\n(Presiona OK para buscar)", L"Buscar", MB_OKCANCEL) == IDOK) {
                        // Buscar simple
                        FINDREPLACE fr = {sizeof(fr)};
                        fr.hwndOwner = hWnd;
                        fr.lpstrFindWhat = szFindText;
                        fr.wFindWhatLen = 256;
                        fr.Flags = FR_DOWN;
                        MessageBox(hWnd, L"Funcion de busqueda basica.\nUsa Ctrl+F en el futuro!", L"Buscar", MB_OK);
                    }
                    break;
                }
                
                case ID_EDIT_DATETIME: {
                    SYSTEMTIME st;
                    GetLocalTime(&st);
                    WCHAR datetime[100];
                    wsprintf(datetime, L"%02d/%02d/%04d %02d:%02d:%02d",
                        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
                    SendMessage(hEdit, EM_REPLACESEL, TRUE, (LPARAM)datetime);
                    break;
                }
                
                case ID_FORMAT_WORDWRAP: {
                    bWordWrap = !bWordWrap;
                    
                    // Guardar contenido
                    int len = GetWindowTextLength(hEdit);
                    WCHAR* buf = (WCHAR*)GlobalAlloc(GPTR, (len + 1) * sizeof(WCHAR));
                    GetWindowText(hEdit, buf, len + 1);
                    
                    // Recrear control
                    DWORD style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_WANTRETURN;
                    if(!bWordWrap) style |= WS_HSCROLL | ES_AUTOHSCROLL;
                    
                    DestroyWindow(hEdit);
                    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, buf,
                        style, 0, 0, 0, 0, hWnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL);
                    
                    ApplyZoom(nZoom);
                    if(bDarkMode) SendMessage(hEdit, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
                    
                    GlobalFree(buf);
                    SendMessage(hWnd, WM_SIZE, 0, 0);
                    
                    HMENU hMenu = GetMenu(hWnd);
                    CheckMenuItem(hMenu, ID_FORMAT_WORDWRAP, bWordWrap ? MF_CHECKED : MF_UNCHECKED);
                    break;
                }
                
                case ID_FORMAT_FONT: {
                    CHOOSEFONT cf = {sizeof(cf)};
                    LOGFONT lf = {0};
                    cf.hwndOwner = hWnd;
                    cf.lpLogFont = &lf;
                    cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
                    cf.rgbColors = RGB(0, 0, 0);
                    
                    wcscpy(lf.lfFaceName, L"Consolas");
                    lf.lfHeight = -MulDiv(12, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72);
                    
                    if(ChooseFont(&cf)) {
                        if(hCurrentFont) DeleteObject(hCurrentFont);
                        hCurrentFont = CreateFontIndirect(&lf);
                        SendMessage(hEdit, WM_SETFONT, (WPARAM)hCurrentFont, TRUE);
                    }
                    break;
                }
                
                case ID_VIEW_STATUS:
                    bShowStatus = !bShowStatus;
                    ShowWindow(hStatus, bShowStatus ? SW_SHOW : SW_HIDE);
                    SendMessage(hWnd, WM_SIZE, 0, 0);
                    CheckMenuItem(GetMenu(hWnd), ID_VIEW_STATUS, bShowStatus ? MF_CHECKED : MF_UNCHECKED);
                    break;
                
                case ID_VIEW_ZOOM_IN:
                    ApplyZoom(nZoom + 10);
                    UpdateStatusBar(hWnd);
                    break;
                
                case ID_VIEW_ZOOM_OUT:
                    ApplyZoom(nZoom - 10);
                    UpdateStatusBar(hWnd);
                    break;
                
                case ID_VIEW_ZOOM_RESET:
                    ApplyZoom(100);
                    UpdateStatusBar(hWnd);
                    break;
                
                case ID_VIEW_DARKMODE:
                    ToggleDarkMode(hWnd);
                    break;
                
                case ID_VIEW_FULLSCREEN: {
                    static WINDOWPLACEMENT wp = {sizeof(wp)};
                    static BOOL fullscreen = FALSE;
                    fullscreen = !fullscreen;
                    
                    if(fullscreen) {
                        GetWindowPlacement(hWnd, &wp);
                        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                        SetWindowPos(hWnd, HWND_TOP, 0, 0,
                            GetSystemMetrics(SM_CXSCREEN),
                            GetSystemMetrics(SM_CYSCREEN),
                            SWP_FRAMECHANGED);
                    } else {
                        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
                        SetWindowPlacement(hWnd, &wp);
                    }
                    break;
                }
                
                case ID_TOOLS_UPPERCASE: {
                    DWORD start, end;
                    SendMessage(hEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                    if(start != end) {
                        int len = end - start;
                        WCHAR* buf = (WCHAR*)GlobalAlloc(GPTR, (len + 1) * sizeof(WCHAR));
                        if(buf) {
                            SendMessage(hEdit, EM_GETSELTEXT, 0, (LPARAM)buf);
                            CharUpperBuff(buf, len);
                            SendMessage(hEdit, EM_REPLACESEL, TRUE, (LPARAM)buf);
                            GlobalFree(buf);
                        }
                    }
                    break;
                }
                
                case ID_TOOLS_LOWERCASE: {
                    DWORD start, end;
                    SendMessage(hEdit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                    if(start != end) {
                        int len = end - start;
                        WCHAR* buf = (WCHAR*)GlobalAlloc(GPTR, (len + 1) * sizeof(WCHAR));
                        if(buf) {
                            SendMessage(hEdit, EM_GETSELTEXT, 0, (LPARAM)buf);
                            CharLowerBuff(buf, len);
                            SendMessage(hEdit, EM_REPLACESEL, TRUE, (LPARAM)buf);
                            GlobalFree(buf);
                        }
                    }
                    break;
                }
                
                case ID_TOOLS_STATS: {
                    int len = GetWindowTextLength(hEdit);
                    WCHAR* buf = (WCHAR*)GlobalAlloc(GPTR, (len + 1) * sizeof(WCHAR));
                    if(buf) {
                        GetWindowText(hEdit, buf, len + 1);
                        
                        int chars = len;
                        int words = 0, lines = 1, spaces = 0;
                        BOOL inWord = FALSE;
                        
                        for(int i = 0; i < len; i++) {
                            if(buf[i] == '\n') lines++;
                            if(iswspace(buf[i])) {
                                spaces++;
                                inWord = FALSE;
                            } else if(!inWord) {
                                words++;
                                inWord = TRUE;
                            }
                        }
                        
                        WCHAR stats[500];
                        wsprintf(stats,
                            L"ESTADISTICAS DEL DOCUMENTO\n"
                            L"================================\n\n"
                            L"Caracteres: %d\n"
                            L"Palabras: %d\n"
                            L"Lineas: %d\n"
                            L"Espacios: %d\n"
                            L"Tamano: %.2f KB\n\n"
                            L"Zoom actual: %d%%",
                            chars, words, lines, spaces,
                            (chars * sizeof(WCHAR)) / 1024.0, nZoom);
                        
                        MessageBox(hWnd, stats, L"Estadisticas", MB_OK | MB_ICONINFORMATION);
                        GlobalFree(buf);
                    }
                    break;
                }
                
                case ID_HELP_ABOUT: {
                    MessageBox(hWnd,
                        L"LightNotes v1.0\n"
                        L"================================\n\n"
                        L"El bloc de notas mas ligero y rapido!\n\n"
                        L"CARACTERISTICAS:\n"
                        L"- Peso: < 500KB\n"
                        L"- Zoom 50-300%\n"
                        L"- Modo oscuro\n"
                        L"- Pantalla completa\n"
                        L"- Drag & Drop\n"
                        L"- Estadisticas en vivo\n"
                        L"- Atajos de teclado\n"
                        L"- Soporte UTF-8/UTF-16\n\n"
                        L"ATAJOS:\n"
                        L"Ctrl+N - Nuevo\n"
                        L"Ctrl+O - Abrir\n"
                        L"Ctrl+S - Guardar\n"
                        L"Ctrl+F - Buscar\n"
                        L"Ctrl++ - Zoom in\n"
                        L"Ctrl+- - Zoom out\n"
                        L"Ctrl+0 - Zoom 100%\n"
                        L"F5 - Fecha/Hora\n"
                        L"F11 - Pantalla completa\n\n"
                        L"Creado por Andrés Santiago Ávalos Gallegos.\n"
                        L"(c) 2025 - Light as a feather!",
                        L"Acerca de LightNotes",
                        MB_OK | MB_ICONINFORMATION);
                    break;
                }
                
                case ID_EDIT:
                    if(HIWORD(wParam) == EN_CHANGE) {
                        bModified = TRUE;
                        UpdateStatusBar(hWnd);
                    }
                    break;
            }
            break;
        }
        
        case WM_KEYDOWN: {
            BOOL ctrl = GetKeyState(VK_CONTROL) & 0x8000;
            
            if(ctrl) {
                switch(wParam) {
                    case 'N': SendMessage(hWnd, WM_COMMAND, ID_FILE_NEW, 0); return 0;
                    case 'O': SendMessage(hWnd, WM_COMMAND, ID_FILE_OPEN, 0); return 0;
                    case 'S': SendMessage(hWnd, WM_COMMAND, ID_FILE_SAVE, 0); return 0;
                    case 'F': SendMessage(hWnd, WM_COMMAND, ID_EDIT_FIND, 0); return 0;
                    case 'H': SendMessage(hWnd, WM_COMMAND, ID_EDIT_REPLACE, 0); return 0;
                    case 'G': SendMessage(hWnd, WM_COMMAND, ID_EDIT_GOTO, 0); return 0;
                    case VK_OEM_PLUS:
                    case VK_ADD:
                        SendMessage(hWnd, WM_COMMAND, ID_VIEW_ZOOM_IN, 0);
                        return 0;
                    case VK_OEM_MINUS:
                    case VK_SUBTRACT:
                        SendMessage(hWnd, WM_COMMAND, ID_VIEW_ZOOM_OUT, 0);
                        return 0;
                    case '0':
                    case VK_NUMPAD0:
                        SendMessage(hWnd, WM_COMMAND, ID_VIEW_ZOOM_RESET, 0);
                        return 0;
                }
            } else {
                if(wParam == VK_F5) {
                    SendMessage(hWnd, WM_COMMAND, ID_EDIT_DATETIME, 0);
                    return 0;
                }
                if(wParam == VK_F11) {
                    SendMessage(hWnd, WM_COMMAND, ID_VIEW_FULLSCREEN, 0);
                    return 0;
                }
            }
            break;
        }
        
        case WM_MOUSEWHEEL: {
            if(GetKeyState(VK_CONTROL) & 0x8000) {
                int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if(delta > 0) {
                    ApplyZoom(nZoom + 5);
                } else {
                    ApplyZoom(nZoom - 5);
                }
                UpdateStatusBar(hWnd);
                return 0;
            }
            break;
        }
        
        case WM_CLOSE:
            if(bModified) {
                int ret = MessageBox(hWnd,
                    L"Desea guardar los cambios?",
                    L"LightNotes",
                    MB_YESNOCANCEL | MB_ICONQUESTION);
                
                if(ret == IDCANCEL) return 0;
                if(ret == IDYES && szFileName[0]) {
                    SaveFile(hWnd, szFileName);
                }
            }
            DestroyWindow(hWnd);
            break;
        
        case WM_DESTROY:
            KillTimer(hWnd, TIMER_STATUS);
            if(hCurrentFont) DeleteObject(hCurrentFont);
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}