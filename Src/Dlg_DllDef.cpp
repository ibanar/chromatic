// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2009 Franck Charlet.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// -----------------------------------------------------------------------
// Dlg_DllDef.cpp: Dll exports form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "Globals.h"
#include "Dlg_DllDef.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMDllDefCmdOk;
HWND FRMDllDefCmdCancel;
HWND FRMDllDefCmdSelAll;
HWND FRMDllDefCmdUnSelAll;
HWND FRMDllDefListview;
HWND FRMDllDefComboBox;
long *DllDefArray;
CStr DllDefFile;
CStr RealProcList;

// -----------------------------------------------------------------------
// Display the form
void DisplayDefForm(CStr DefFile)
{
    DllDefFile = DefFile;
	RealProcList = GetModulesProc();
    DllDefArray = StringSplit(RealProcList, RealProcList.Chr(2));
    WACreateModalDialog(-1, -1, 342, 295, hMDIform.hWnd, &FRMDllDef, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMDllDef(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMDllDefListview, GetSysColor(COLOR_WINDOW));
			break;
		case WM_INITDIALOG:
            WAControlSetText(hwndDlg, "Select procedures to export");
            FRMDllDefCmdOk = WACreateButton(263, 1 + 16, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMDllDefCmdCancel = WACreateButton(263, 25 + 16, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMDllDefCmdSelAll = WACreateButton(263, 51 + 16, 77, 23, hwndDlg, "Select all", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMDllDefCmdUnSelAll = WACreateButton(263, 75 + 16, 77, 23, hwndDlg, "Unselect all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            WACreateLabel(1, 2, 250, 15, hwndDlg, "Entry point :", 0, 0, 0, 0);
            FRMDllDefComboBox = WACreateComboBox(2, 17, 258, 150, hwndDlg, "", 5, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            WACreateLabel(1, 40, 250, 15, hwndDlg, "Exports :", 0, 0, 0, 0);
            FRMDllDefListview = WACreateListView(2, 55, 258, 238, hwndDlg, 6, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMDllDefListview, "", 300, 0);
            WAComboBoxAddItem(FRMDllDefComboBox, "[None]", -1);
            FreezeTimer = 1;
            WAComboBoxSetIndex(FRMDllDefComboBox, DllDefFillFilesList());
            WAListViewSetItemSel(FRMDllDefListview, 0);
            SetFocus(FRMDllDefComboBox);
            return(0);
        case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMDllDefListview)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        WAListViewCheckBoxItemDoubleClick(FRMDllDefListview);
                        return(0);
                }
            }
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMDllDefCmdOk)
            {
                SaveDllDef();
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMDllDefCmdCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMDllDefCmdSelAll)
            {
                for(i = 0; i <= WAListViewItemCount(FRMDllDefListview) - 1; i++)
                {
                    WAListViewSetItemCheckbox(FRMDllDefListview, i, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMDllDefCmdUnSelAll)
            {
                for(i = 0; i <= WAListViewItemCount(FRMDllDefListview) - 1; i++)
                {
                    WAListViewSetItemCheckbox(FRMDllDefListview, i, 0);
                }
                return(0);
            }
			break;
        case WM_CLOSE:
            StringReleaseSplit(DllDefArray);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the listview with filenames
long DllDefFillFilesList(void)
{
    int i = 0;
    CStr DefValue;
    CStr DefEntryValue;
    long ReturnValue = 0;

    DefEntryValue = WAIniReadKey("ENTRYPOINT", "Proc", DllDefFile);
    for(i = 0; i <= StringGetSplitUBound(DllDefArray) - 1; i++)
    {
        if(strlen(StringGetSplitElement(RealProcList, DllDefArray, i).Get_String()) != 0)
        {
            WAListViewAddItem(FRMDllDefListview, StringGetSplitElement(RealProcList, DllDefArray, i), i, 0);
            if(strcmpi(StringGetSplitElement(RealProcList, DllDefArray, i).Get_String(), DefEntryValue.Get_String()) == 0) ReturnValue = i + 1;
            WAComboBoxAddItem(FRMDllDefComboBox, StringGetSplitElement(RealProcList, DllDefArray, i), -1);
            DefValue = WAIniReadKey("EXPORTS", StringGetSplitElement(RealProcList, DllDefArray, i), DllDefFile);
            if(DefValue.Len() != 0) WAListViewSetItemCheckbox(FRMDllDefListview, i, 1);
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save selected exports in exports file
void SaveDllDef(void)
{
    int i = 0;
    int j = 0;

    WAIniWriteSection("ENTRYPOINT", "", DllDefFile);
    if(WAComboBoxGetIndex(FRMDllDefComboBox) == 0)
    {
        WAIniWriteKey("ENTRYPOINT", "Proc", "", DllDefFile);
    }
    else
    {
        WAIniWriteKey("ENTRYPOINT", "Proc", WAControlGetText(FRMDllDefComboBox), DllDefFile);
    }
    WAIniWriteSection("EXPORTS", "", DllDefFile);
    for(i = 0; i <= WAListViewItemCount(FRMDllDefListview) - 1; i++)
    {
        if(WAListViewGetItemCheckbox(FRMDllDefListview, i) == 1)
        {
            WAIniWriteKey("EXPORTS", WAListViewGetItemText(FRMDllDefListview, i, 0), "1", DllDefFile);
            WAIniWriteKey("EXPORTS", "Fnc" + (CStr) StringNumberComplement(j, 3).Get_String(), WAListViewGetItemText(FRMDllDefListview, i, 0), DllDefFile);
            j++;
        }
    }
}
