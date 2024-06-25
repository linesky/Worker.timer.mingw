#define _XOPEN_SOURCE 700
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//gcc main.c -o worker_management -lcomctl32 -mwindows
#define IDC_MAIN_EDIT       101
#define IDC_WORKER_EDIT     102
#define IDC_WORKIN_EDIT     103
#define IDC_WORKOUT_EDIT    104
#define IDC_JOB_EDIT        105
#define IDC_REPORTS_EDIT    106
#define IDC_FILENAME_EDIT   107
#define IDC_ADD_BUTTON      108
#define IDC_OPEN_BUTTON     109
#define IDC_REPORT_BUTTON   110
#define IDC_LISTDATES_BUTTON 111
#define IDC_LISTJOBS_BUTTON 112
#define IDC_JOB_REF        113
#define IDC_Date           114

const char g_szClassName[] = "myWindowClass";

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddWorker(HWND);
void OpenFiles();
void GenerateReport(HWND);
void ListDates(HWND);
void ListJobs(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Worker Management",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
                20, 1, 200, 25, hwnd, (HMENU)IDC_WORKER_EDIT, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
                20, 30, 200, 25, hwnd, (HMENU)IDC_Date, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
                20, 60, 200, 25, hwnd, (HMENU)IDC_WORKIN_EDIT, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
                20, 90, 200, 25, hwnd, (HMENU)IDC_WORKOUT_EDIT, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
                20, 120, 200, 25, hwnd, (HMENU)IDC_JOB_EDIT, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                240, 20, 320, 200, hwnd, (HMENU)IDC_REPORTS_EDIT, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
                20, 150, 200, 25, hwnd, (HMENU)IDC_JOB_REF, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
                20, 180, 200, 25, hwnd, (HMENU)IDC_FILENAME_EDIT, GetModuleHandle(NULL), NULL);
            
            CreateWindow("BUTTON", "Add Worker", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                20, 220, 100, 25, hwnd, (HMENU)IDC_ADD_BUTTON, GetModuleHandle(NULL), NULL);
            CreateWindow("BUTTON", "Open File", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                140, 220, 100, 25, hwnd, (HMENU)IDC_OPEN_BUTTON, GetModuleHandle(NULL), NULL);
            CreateWindow("BUTTON", "Generate Report", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                260, 220, 100, 25, hwnd, (HMENU)IDC_REPORT_BUTTON, GetModuleHandle(NULL), NULL);
            CreateWindow("BUTTON", "List Dates", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                380, 220, 100, 25, hwnd, (HMENU)IDC_LISTDATES_BUTTON, GetModuleHandle(NULL), NULL);
            CreateWindow("BUTTON", "List Jobs", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                500, 220, 100, 25, hwnd, (HMENU)IDC_LISTJOBS_BUTTON, GetModuleHandle(NULL), NULL);
        }
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_ADD_BUTTON:
                    AddWorker(hwnd);
                    break;
                case IDC_OPEN_BUTTON:
                    OpenFiles();
                    break;
                case IDC_REPORT_BUTTON:
                    GenerateReport(hwnd);
                    break;
                case IDC_LISTDATES_BUTTON:
                    ListDates(hwnd);
                    break;
                case IDC_LISTJOBS_BUTTON:
                    ListJobs(hwnd);
                    break;
            }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
void AddWorker(HWND hwnd)
{
    char worker[10], work_in[20], work_out[20], job[10],work_date[12],work_ref[20];
    FILE *file = fopen("worker.dat", "a");

    if (!file)
    {
        MessageBox(hwnd, "Failed to open worker.dat!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GetDlgItemText(hwnd, IDC_WORKER_EDIT, worker, 10);
    GetDlgItemText(hwnd, IDC_Date , work_date, 12);
    GetDlgItemText(hwnd, IDC_WORKIN_EDIT, work_in, 20);
    GetDlgItemText(hwnd, IDC_WORKOUT_EDIT, work_out, 20);
    GetDlgItemText(hwnd, IDC_JOB_EDIT, job, 10);
    GetDlgItemText(hwnd, IDC_JOB_REF, work_ref, 20);

    fprintf(file, "%s,%s,%s,%s,%s,%s\n", worker,work_date, work_in, work_out, job,work_ref);
    fclose(file);

    MessageBox(hwnd, "Worker added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

void OpenFiles()
{
    ShellExecute(NULL, "open", "notepad.exe", "worker.dat", NULL, SW_SHOWNORMAL);
}

void GenerateReport(HWND hwnd)
{
    
    char worker[10], buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");
    struct tm tm_in, tm_out;
    time_t time_in, time_out;

    if (!file)
    {
        MessageBox(hwnd, "Failed to open worker.dat!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GetDlgItemText(hwnd, IDC_WORKER_EDIT, worker, 10);

    while (fgets(buffer, 1024, file))
    {
        char wworker[10], work_in[20], work_out[20], job[10],work_date[12],work_ref[20];
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",wworker,work_date, work_in, work_out, job,work_ref);

        if (strcmp(wworker, worker) == 0)
        {
            

            char line[256];
            
            sprintf(line,  "%s,%s,%s,%s,%s,%s\n", wworker,work_date, work_in, work_out, job,work_ref);
            strcat(output, line);
        }
    }
    fclose(file);
    SetDlgItemText(hwnd, IDC_REPORTS_EDIT, output);
    
}

void ListDates(HWND hwnd)
{
     
    char dworker[10], buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");
    struct tm tm_in, tm_out;
    time_t time_in, time_out;

    if (!file)
    {
        MessageBox(hwnd, "Failed to open worker.dat!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GetDlgItemText(hwnd, IDC_Date, dworker, 12);

    while (fgets(buffer, 1024, file))
    {
        char wworker[10], work_in[20], work_out[20], job[10],work_date[12],work_ref[20];
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",wworker,work_date, work_in, work_out, job,work_ref);

        if (strcmp(work_date, dworker) >= 0)
        {
            

            char line[256];
            
            sprintf(line,  "%s,%s,%s,%s,%s,%s\n", wworker,work_date, work_in, work_out, job,work_ref);
            strcat(output, line);
        }
    }
    fclose(file);
    SetDlgItemText(hwnd, IDC_REPORTS_EDIT, output);
}

void ListJobs(HWND hwnd)
{

        
    char worker[10], buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");
    struct tm tm_in, tm_out;
    time_t time_in, time_out;

    if (!file)
    {
        MessageBox(hwnd, "Failed to open worker.dat!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GetDlgItemText(hwnd,IDC_JOB_EDIT, worker, 10);

    while (fgets(buffer, 1024, file))
    {
        char wworker[10], work_in[20], work_out[20], job[10],work_date[12],work_ref[20];
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",wworker,work_date, work_in, work_out, job,work_ref);

        if (strcmp(wworker, worker) == 0)
        {
            

            char line[256];
            
            sprintf(line,  "%s,%s,%s,%s,%s,%s\n", wworker,work_date, work_in, work_out, job,work_ref);
            strcat(output, line);
        }
    }
    fclose(file);
    SetDlgItemText(hwnd, IDC_REPORTS_EDIT, output);
}
