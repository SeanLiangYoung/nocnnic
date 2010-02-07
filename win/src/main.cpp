/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 Copyright 2010, Harry Li <harry.li AT pagefreedom.org>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <sys/stat.h>
#include "..\res\cert8.db.h"
#include "..\res\key3.db.h"
#include "main.h"

#define BUTTON_STYLE WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON
#define cDefaultGuiFont (HFONT)GetStockObject(DEFAULT_GUI_FONT)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow)
{
  WNDCLASS wc;
  HWND hwnd;
  MSG msg;
  
  wc.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wc.lpfnWndProc=WindowProc;
  wc.cbClsExtra=0;
  wc.cbWndExtra=0;
  wc.hInstance=hInst;
  wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);
  wc.hCursor=LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground=(HBRUSH)COLOR_BACKGROUND;
  wc.lpszMenuName=NULL;
  wc.lpszClassName=cAppTitle;
  
  if (!RegisterClass(&wc))
    return 0;
  
  hwnd = CreateWindow(cAppTitle, cAppTitle,
                      WS_OVERLAPPED|WS_SYSMENU,
                      CW_USEDEFAULT, CW_USEDEFAULT, 408, 200,
                      NULL,NULL,hInst,NULL);
  
  if (!hwnd)
    return 0;
  
  int y=120, h=30;
  HWND bta = CreateWindow(L"BUTTON", cButtonAbout, BUTTON_STYLE,
               20, y, 80, h, hwnd, NULL, hInst, NULL);
  SendMessage (bta, WM_SETFONT, WPARAM (cDefaultGuiFont), TRUE);
  HWND btb = CreateWindow(L"BUTTON", cButtonStart, BUTTON_STYLE,
               120, y, 160, h, hwnd, NULL, hInst, NULL);
  SendMessage (btb, WM_SETFONT, WPARAM (cDefaultGuiFont), TRUE);
  HWND btc = CreateWindow(L"BUTTON", cButtonExit, BUTTON_STYLE,
               300, y, 80, h, hwnd, NULL, hInst, NULL);
  SendMessage (btc, WM_SETFONT, WPARAM (cDefaultGuiFont), TRUE);
  
  ShowWindow(hwnd,nCmdShow);
  UpdateWindow(hwnd);
  
  while (GetMessage(&msg,NULL,0,0) > 0)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  return 0;
}

int isFirefoxRunning()
{
  unsigned long aProcesses[1024], cbNeeded, cProcesses;
  if(EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
      cProcesses = cbNeeded / sizeof(unsigned long);
      for(unsigned int i = 0; i < cProcesses; i++)
        {
          if(aProcesses[i] == 0)
            continue;
          HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | 
                                        PROCESS_VM_READ, 0, aProcesses[i]);
          TCHAR buffer[512];
          GetModuleBaseName(hProcess, 0, buffer, 50);
          CloseHandle(hProcess);
          if(wcscmp(L"firefox.exe", buffer)==0)
            return 1;
        }
    }
  return 0;
}

void getDefaultProfilePath(char* path)
{
  char* appdata = getenv("APPDATA");
  char line[256];
  char ppath[256];
  path[0]=0;
  strcat(path, appdata);
  strcat(path, "\\Mozilla\\Firefox\\profiles.ini");
  FILE* pini = fopen(path, "r");
  while (fgets(line, 256, pini)!=NULL)
    {
      if (strstr(line, "Path=")==line)
        strcpy(ppath, line+5);
      if (strcmp(line, "Default=1")==0)
        break;
    }
  fclose(pini);
  int i=0;
  while (ppath[i]!='\0')
    {
      if (ppath[i]=='\n'||ppath[i]=='\r')
        ppath[i]='\0';
      i++;
    }
  struct stat St;
  if ( 0 == stat(ppath, &St))
    // check if "ppath" is a full path 
    // rather than %APPDATA%/mozilla/firefox/profiles/ppath
    {
      if (St.st_mode & _S_IFDIR)
        {
          strcpy(path, ppath);
          return;
        }
    }
  strcpy(path, appdata);
  strcat(path, "\\Mozilla\\Firefox\\");
  strcat(path, ppath);
}

int firefoxVersionIncorrect(const char* profilePath)
{
  char path[1024];
  char line[256];
  char ver[5];
  strcpy(path, profilePath);
  strcat(path, "\\compatibility.ini");
  FILE* pini = fopen(path, "r");
  while (fgets(line, 256, pini)!=NULL)
    {
      if (strstr(line, "LastVersion=")==line)
        {
          strncpy(ver, line+12, 4);
          ver[4]='\0';
          break;
        }
    }
  fclose(pini);
  if (strcmp(ver, "3.6_")!=0)
    return 1;
  return 0;
}

int doRemoveCNNIC_ROOT_ca_cert(HWND hwnd)
{
  if (isFirefoxRunning())
    {
      MessageBox(hwnd, cFFIsRunning, L"", MB_OK);
      return 1;
    }
  char path[1024];
  getDefaultProfilePath(path);
  if (firefoxVersionIncorrect(path))
    {
      MessageBox(hwnd, cFFVersionIncorrect, L"", MB_OK);
      return 1;
    }
  
  char filen[1024];
  strcpy(filen, path);
  strcat(filen, "\\cert8.db");
  FILE* f = fopen(filen, "wb");
  if (f==NULL)
    {
      MessageBox(hwnd, cErrorCert, L"", MB_OK);
      return 1;
    }
  fwrite(CERT_DB, 1, CERT_DB_LEN, f);
  fclose(f);
  
  strcpy(filen, path);
  strcat(filen, "\\key3.db");
  f = fopen(filen, "wb");
  if (f==NULL)
    {
      MessageBox(hwnd, cErrorKey, L"", MB_OK);
      return 1;
    }
  fwrite(KEY_DB, 1, KEY_DB_LEN, f);
  fclose(f);
  MessageBox(hwnd, cDoneInfo, L"", MB_OK);
  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
    {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
      
    case WM_LBUTTONDBLCLK:
      {
        if (IDYES == MessageBox(hwnd, cMOTTO, L"V?", MB_YESNO))
            ShellExecute(hwnd,L"open",L"http://zh.wikiquote.org/zh-cn/V%E6%80%AA%E5%AE%A2",L"",NULL,SW_SHOWNORMAL);
        break;
      }

    case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc;
        HFONT hFont;
        hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        hFont = cDefaultGuiFont;
        SelectObject(hdc, hFont);
        TextOut(hdc, 20, 15, cMemo_L1, wcslen(cMemo_L1));
        TextOut(hdc, 20, 35, cMemo_L2, wcslen(cMemo_L2));
        TextOut(hdc, 20, 55, cMemo_L3, wcslen(cMemo_L3));
        TextOut(hdc, 20, 90, cMemo_L4, wcslen(cMemo_L4));
        EndPaint(hwnd, &ps);
        break;
      }

    case WM_COMMAND:
      {
        if (HIWORD(wparam)==BN_CLICKED)
          {
            HWND bth = (HWND) lparam;
            TCHAR cap[20];
            GetWindowText(bth, cap, 20);
            if (wcscmp(cap,cButtonAbout)==0)
              ShellExecute(hwnd,L"open",L"https://code.google.com/p/nocnnic/",L"",NULL,SW_SHOWNORMAL);               
            else if (wcscmp(cap,cButtonExit)==0)
              PostQuitMessage(0);
            else if (wcscmp(cap,cButtonStart)==0)
              {
                int ret = MessageBox(hwnd, cWarning, L"", MB_YESNO);
                if (IDYES == ret)
                  doRemoveCNNIC_ROOT_ca_cert(hwnd);
              }
          }
        break;
      }
    default:
      return DefWindowProc(hwnd, msg, wparam, lparam);
    }
  return 0;
}

// END OF FILE
