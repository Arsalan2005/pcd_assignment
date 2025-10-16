#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "compiler.h"
#include "codegen.h"

// Control IDs
#define ID_INPUT_EDIT 1001
#define ID_OUTPUT_EDIT 1002
#define ID_COMPILE_BTN 1003
#define ID_CLEAR_BTN 1004
#define ID_GRAMMAR_COMBO 1005
#define ID_TAB_CONTROL 1006

// Tab indices
#define TAB_TOKENS 0
#define TAB_LL_PARSE 1
#define TAB_LR_PARSE 2
#define TAB_ASSEMBLY 3

// Global variables
HWND hInputEdit, hOutputEdit, hCompileBtn, hClearBtn, hGrammarCombo, hTabControl;
HWND hTabOutputs[4];
Grammar current_grammar;
char output_buffer[50000];

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void CompileInput();
void ClearAll();
void AppendOutput(const char* text);
void SwitchTab(int tabIndex);

// WinMain entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "CompilerGUI";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return 0;
    }
    
    HWND hwnd = CreateWindowEx(
        0,
        "CompilerGUI",
        "Compiler Design - LL & LR Parser with Assembly Code Generator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800,
        NULL, NULL, hInstance, NULL
    );
    
    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            InitCommonControls();
            CreateControls(hwnd);
            load_sample_grammar(&current_grammar, 3); // Load default grammar
            break;
            
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_COMPILE_BTN) {
                CompileInput();
            }
            else if (LOWORD(wParam) == ID_CLEAR_BTN) {
                ClearAll();
            }
            else if (LOWORD(wParam) == ID_GRAMMAR_COMBO && HIWORD(wParam) == CBN_SELCHANGE) {
                int idx = SendMessage(hGrammarCombo, CB_GETCURSEL, 0, 0);
                load_sample_grammar(&current_grammar, idx + 1);
                SetWindowText(hOutputEdit, "Grammar loaded successfully!\r\n\r\nReady to compile.");
            }
            break;
            
        case WM_NOTIFY: {
            LPNMHDR pnmhdr = (LPNMHDR)lParam;
            if (pnmhdr->idFrom == ID_TAB_CONTROL && pnmhdr->code == TCN_SELCHANGE) {
                int tabIndex = TabCtrl_GetCurSel(hTabControl);
                SwitchTab(tabIndex);
            }
            break;
        }
            
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            // Resize controls
            MoveWindow(hGrammarCombo, 10, 10, 300, 25, TRUE);
            MoveWindow(hCompileBtn, 320, 10, 100, 30, TRUE);
            MoveWindow(hClearBtn, 430, 10, 100, 30, TRUE);
            MoveWindow(hInputEdit, 10, 50, width - 20, 150, TRUE);
            MoveWindow(hTabControl, 10, 210, width - 20, 40, TRUE);
            MoveWindow(hOutputEdit, 10, 260, width - 20, height - 270, TRUE);
            
            // Resize tab outputs
            for (int i = 0; i < 4; i++) {
                MoveWindow(hTabOutputs[i], 10, 260, width - 20, height - 270, TRUE);
            }
            break;
        }
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void CreateControls(HWND hwnd) {
    // Grammar selection combo box
    hGrammarCombo = CreateWindow("COMBOBOX", "",
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
        10, 10, 300, 200,
        hwnd, (HMENU)ID_GRAMMAR_COMBO, NULL, NULL);
    
    SendMessage(hGrammarCombo, CB_ADDSTRING, 0, (LPARAM)"Grammar 1: Arithmetic (Left Recursive)");
    SendMessage(hGrammarCombo, CB_ADDSTRING, 0, (LPARAM)"Grammar 2: Simple LL(1)");
    SendMessage(hGrammarCombo, CB_ADDSTRING, 0, (LPARAM)"Grammar 3: Expression (LL Compatible)");
    SendMessage(hGrammarCombo, CB_SETCURSEL, 2, 0);
    
    // Compile button
    hCompileBtn = CreateWindow("BUTTON", "COMPILE",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        320, 10, 100, 30,
        hwnd, (HMENU)ID_COMPILE_BTN, NULL, NULL);
    
    // Clear button
    hClearBtn = CreateWindow("BUTTON", "CLEAR",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        430, 10, 100, 30,
        hwnd, (HMENU)ID_CLEAR_BTN, NULL, NULL);
    
    // Input text box
    hInputEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        10, 50, 1160, 150,
        hwnd, (HMENU)ID_INPUT_EDIT, NULL, NULL);
    
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
    SendMessage(hInputEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    SetWindowText(hInputEdit, "id + id * id");
    
    // Tab control
    hTabControl = CreateWindow(WC_TABCONTROL, "",
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        10, 210, 1160, 40,
        hwnd, (HMENU)ID_TAB_CONTROL, NULL, NULL);
    
    TCITEM tie;
    tie.mask = TCIF_TEXT;
    
    tie.pszText = "Tokens";
    TabCtrl_InsertItem(hTabControl, 0, &tie);
    
    tie.pszText = "LL Parser";
    TabCtrl_InsertItem(hTabControl, 1, &tie);
    
    tie.pszText = "LR Parser";
    TabCtrl_InsertItem(hTabControl, 2, &tie);
    
    tie.pszText = "Assembly Code";
    TabCtrl_InsertItem(hTabControl, 3, &tie);
    
    // Create output windows for each tab
    for (int i = 0; i < 4; i++) {
        hTabOutputs[i] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL | WS_HSCROLL,
            10, 260, 1160, 490,
            hwnd, (HMENU)(ID_OUTPUT_EDIT + i), NULL, NULL);
        SendMessage(hTabOutputs[i], WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    
    hOutputEdit = hTabOutputs[0];
    ShowWindow(hOutputEdit, SW_SHOW);
}

void AppendOutput(const char* text) {
    int len = GetWindowTextLength(hOutputEdit);
    SendMessage(hOutputEdit, EM_SETSEL, len, len);
    SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)text);
}

void SwitchTab(int tabIndex) {
    for (int i = 0; i < 4; i++) {
        ShowWindow(hTabOutputs[i], SW_HIDE);
    }
    hOutputEdit = hTabOutputs[tabIndex];
    ShowWindow(hOutputEdit, SW_SHOW);
}

void CompileInput() {
    char input[1000];
    GetWindowText(hInputEdit, input, sizeof(input));
    
    if (strlen(input) == 0) {
        MessageBox(NULL, "Please enter input string!", "Error", MB_ICONERROR);
        return;
    }
    
    // Clear all outputs
    for (int i = 0; i < 4; i++) {
        SetWindowText(hTabOutputs[i], "");
    }
    
    // TAB 0: TOKENS
    hOutputEdit = hTabOutputs[TAB_TOKENS];
    output_buffer[0] = '\0';
    
    strcat(output_buffer, "╔══════════════════════════════════════════════════════════════╗\r\n");
    strcat(output_buffer, "║                    LEXICAL ANALYSIS                          ║\r\n");
    strcat(output_buffer, "╚══════════════════════════════════════════════════════════════╝\r\n\r\n");
    
    init_lexer(input);
    Token token;
    
    strcat(output_buffer, "TOKEN TYPE          LEXEME              LINE    COLUMN\r\n");
    strcat(output_buffer, "─────────────────────────────────────────────────────────────\r\n");
    
    do {
        token = get_next_token();
        char line[200];
        sprintf(line, "%-20s%-20s%-8d%-8d\r\n",
                token_type_to_string(token.type),
                token.lexeme,
                token.line,
                token.column);
        strcat(output_buffer, line);
    } while (token.type != TOKEN_EOF);
    
    SetWindowText(hTabOutputs[TAB_TOKENS], output_buffer);
    
    // TAB 1: LL PARSER
    hOutputEdit = hTabOutputs[TAB_LL_PARSE];
    output_buffer[0] = '\0';
    
    strcat(output_buffer, "╔══════════════════════════════════════════════════════════════╗\r\n");
    strcat(output_buffer, "║                    LL PARSER (TOP-DOWN)                      ║\r\n");
    strcat(output_buffer, "╚══════════════════════════════════════════════════════════════╝\r\n\r\n");
    
    // Redirect stdout to capture parsing output
    freopen("temp_ll.txt", "w", stdout);
    
    FirstFollowSet first_sets[MAX_SYMBOLS];
    FirstFollowSet follow_sets[MAX_SYMBOLS];
    
    init_ll_parser(&current_grammar);
    compute_first_sets(&current_grammar, first_sets);
    compute_follow_sets(&current_grammar, first_sets, follow_sets);
    build_ll_table(&current_grammar, first_sets, follow_sets);
    
    bool ll_result = parse_ll(input);
    
    fclose(stdout);
    freopen("CON", "w", stdout);
    
    // Read captured output
    FILE* temp_file = fopen("temp_ll.txt", "r");
    char line[500];
    while (fgets(line, sizeof(line), temp_file)) {
        strcat(output_buffer, line);
        strcat(output_buffer, "\r");
    }
    fclose(temp_file);
    remove("temp_ll.txt");
    
    strcat(output_buffer, "\r\n\r\n");
    strcat(output_buffer, "═══════════════════════════════════════════════════════════════\r\n");
    if (ll_result) {
        strcat(output_buffer, "                    ✓ INPUT ACCEPTED                          \r\n");
    } else {
        strcat(output_buffer, "                    ✗ INPUT REJECTED                           \r\n");
    }
    strcat(output_buffer, "═══════════════════════════════════════════════════════════════\r\n");
    
    SetWindowText(hTabOutputs[TAB_LL_PARSE], output_buffer);
    
    // TAB 2: LR PARSER
    hOutputEdit = hTabOutputs[TAB_LR_PARSE];
    output_buffer[0] = '\0';
    
    strcat(output_buffer, "╔══════════════════════════════════════════════════════════════╗\r\n");
    strcat(output_buffer, "║                  LR PARSER (BOTTOM-UP)                       ║\r\n");
    strcat(output_buffer, "╚══════════════════════════════════════════════════════════════╝\r\n\r\n");
    
    freopen("temp_lr.txt", "w", stdout);
    
    init_lr_parser(&current_grammar);
    bool lr_result = parse_lr(input);
    
    fclose(stdout);
    freopen("CON", "w", stdout);
    
    temp_file = fopen("temp_lr.txt", "r");
    while (fgets(line, sizeof(line), temp_file)) {
        strcat(output_buffer, line);
        strcat(output_buffer, "\r");
    }
    fclose(temp_file);
    remove("temp_lr.txt");
    
    strcat(output_buffer, "\r\n\r\n");
    strcat(output_buffer, "═══════════════════════════════════════════════════════════════\r\n");
    if (lr_result) {
        strcat(output_buffer, "                    ✓ INPUT ACCEPTED                          \r\n");
    } else {
        strcat(output_buffer, "                    ✗ INPUT REJECTED                           \r\n");
    }
    strcat(output_buffer, "═══════════════════════════════════════════════════════════════\r\n");
    
    SetWindowText(hTabOutputs[TAB_LR_PARSE], output_buffer);
    
    // TAB 3: ASSEMBLY CODE
    hOutputEdit = hTabOutputs[TAB_ASSEMBLY];
    output_buffer[0] = '\0';
    
    strcat(output_buffer, "╔══════════════════════════════════════════════════════════════╗\r\n");
    strcat(output_buffer, "║                   ASSEMBLY CODE GENERATION                    ║\r\n");
    strcat(output_buffer, "╚══════════════════════════════════════════════════════════════╝\r\n\r\n");
    
    generate_code_from_tokens();
    
    freopen("temp_tac.txt", "w", stdout);
    print_tac();
    fclose(stdout);
    freopen("CON", "w", stdout);
    
    temp_file = fopen("temp_tac.txt", "r");
    while (fgets(line, sizeof(line), temp_file)) {
        strcat(output_buffer, line);
        strcat(output_buffer, "\r");
    }
    fclose(temp_file);
    remove("temp_tac.txt");
    
    strcat(output_buffer, "\r\n\r\n");
    
    char* asm_code = generate_assembly();
    strcat(output_buffer, asm_code);
    
    SetWindowText(hTabOutputs[TAB_ASSEMBLY], output_buffer);
    
    // Switch to tokens tab
    TabCtrl_SetCurSel(hTabControl, 0);
    SwitchTab(0);
    
    MessageBox(NULL, "Compilation Complete!\r\nCheck each tab for results.", "Success", MB_ICONINFORMATION);
}

void ClearAll() {
    SetWindowText(hInputEdit, "");
    for (int i = 0; i < 4; i++) {
        SetWindowText(hTabOutputs[i], "");
    }
}
