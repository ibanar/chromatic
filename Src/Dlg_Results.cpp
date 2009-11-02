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
// Dlg_Results.cpp: Search results form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Results.h"

// -----------------------------------------------------------------------
// Variables
HWND hFRMResults;
HWND FrmResultsCmdClose;
HWND FrmResultsCmdSave;
HWND FrmResultsCmdView;
HWND FrmResultsCmdOpen;
HWND FrmResultsListView;
long FrmResultsWidth;
long FrmResultsHeight;
CStr FRMResultStr;

// -----------------------------------------------------------------------
// Initialize results window
void CALLBACK FrmResultsInitProc(HWND hWnd)
{
    FrmResultsListView = WACreateListView(0, 0, 77, 23, hWnd, 0, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP, WS_EX_STATICEDGE);
    WAListViewAddCol(FrmResultsListView, "Related code", 10, 3);
    WAListViewAddCol(FrmResultsListView, "Type", 10, 2);
    WAListViewAddCol(FrmResultsListView, "Line number ", 50, 1);
    WAListViewAddCol(FrmResultsListView, "File", 10, 0);
    FrmResultsCmdClose = WACreateButton(150, 1, 77, 23, hWnd, "Close", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FrmResultsCmdSave = WACreateButton(150, 26, 77, 23, hWnd, "To file", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FrmResultsCmdView = WACreateButton(150, 50, 77, 23, hWnd, "View", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FrmResultsCmdOpen = WACreateButton(150, 74, 77, 23, hWnd, "To doc", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
}

// -----------------------------------------------------------------------
// Default window hook
LRESULT CALLBACK FrmResultsWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ColsGlobalWidth = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FrmResultsListView, GetSysColor(COLOR_WINDOW));
			break;
		case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FrmResultsListView)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        GoToLine(WAListViewGetSelItemText(FrmResultsListView, 0), WAListViewGetSelItemText(FrmResultsListView, 1).Get_Long() - 1, 1);
                        return(0);
                    case LVN_COLUMNCLICK:
                        WAListViewSort(FrmResultsListView, WAListViewGetNotifiedColumnIndex(lParam), &FRMResultsSort);
						WAListViewReOrder(FrmResultsListView);
                        return(0);
                }
            }
			break;
        case WM_SIZE:
            FrmResultsWidth = WAControlClientWidth(hWnd);
            FrmResultsHeight = WAControlClientHeight(hWnd);
            MoveWindow(FrmResultsCmdClose, FrmResultsWidth - WAControlWidth(FrmResultsCmdClose), 1, 77, 23, 1);
            MoveWindow(FrmResultsCmdSave, FrmResultsWidth - WAControlWidth(FrmResultsCmdSave), 26, 77, 23, 1);
            MoveWindow(FrmResultsCmdView, FrmResultsWidth - WAControlWidth(FrmResultsCmdView), 50, 77, 23, 1);
            MoveWindow(FrmResultsCmdOpen, FrmResultsWidth - WAControlWidth(FrmResultsCmdOpen), 74, 77, 23, 1);
            MoveWindow(FrmResultsListView, 1, 1, FrmResultsWidth - WAControlWidth(FrmResultsCmdClose) - 3, FrmResultsHeight - 2, 1);
            WAListViewSetColWidth(FrmResultsListView, 0, LVSCW_AUTOSIZE);
            WAListViewSetColWidth(FrmResultsListView, 2, LVSCW_AUTOSIZE);
            WAListViewSetColWidth(FrmResultsListView, 1, LVSCW_AUTOSIZE_USEHEADER);
            WAListViewSetColWidth(FrmResultsListView, 3, LVSCW_AUTOSIZE);
            ColsGlobalWidth = WAListViewGetColWidth(FrmResultsListView, 0);
            ColsGlobalWidth = ColsGlobalWidth + WAListViewGetColWidth(FrmResultsListView, 1);
            ColsGlobalWidth = ColsGlobalWidth + WAListViewGetColWidth(FrmResultsListView, 2);
            ColsGlobalWidth = ColsGlobalWidth + WAListViewGetColWidth(FrmResultsListView, 3);
            if(WAControlWidth(FrmResultsListView) > ColsGlobalWidth)
            {
                WAListViewSetColWidth(FrmResultsListView,
                                      3,
                                      WAListViewGetColWidth(FrmResultsListView, 3) + (WAControlWidth(FrmResultsListView) - ColsGlobalWidth));
            }
            return(DefWindowProc(hWnd, uMsg, wParam, lParam));
        case WM_COMMAND:
            if((HWND) lParam == FrmResultsCmdClose)
            {
                WAControlClose(hFRMResults);
                return(0);
            }
            else if((HWND) lParam == FrmResultsCmdView)
            {
                GoToLine(WAListViewGetSelItemText(FrmResultsListView, 0), WAListViewGetSelItemText(FrmResultsListView, 1).Get_Long() - 1, 1);
                return(0);
            }
            else if((HWND) lParam == FrmResultsCmdSave)
            {
                SaveResultFile();
                return(0);
            }
            else if((HWND) lParam == FrmResultsCmdOpen)
            {
                NewResultFile();
                return(0);
            }
			break;
        case WM_DESTROY:
            hFRMResults = 0;
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Save results as a file
void SaveResultFile(void)
{
    CStr FName;
    CStr Filters;
    int i = 0;
    long GreatestType = 0;
    long GreatestFile = 0;
    long GreatestLn = 0;
    long GreatestRelCode = 0;
    CStr ToWRiteVar[3 + 1];
    CStr ToAdd;
    CStr HeadLab;
    CStr TexttoAdd;
    CStr Slots[3 + 1];

	Filters = "All files (*.*)|*.*";
    WAControlEnable(hFRMResults, 0);
    FName = WAComDlgGetSaveFileName(hMDIform.hWnd, Filters, "", CurrentDir);
    if(FName.Len() != 0)
    {
        // List overview to check for greatest elements
        GreatestType = WAListViewGetHeaderLabel(FrmResultsListView, 0).Len();
        GreatestFile = WAListViewGetHeaderLabel(FrmResultsListView, 1).Len();
        GreatestLn = WAListViewGetHeaderLabel(FrmResultsListView, 2).Len();
        GreatestRelCode = WAListViewGetHeaderLabel(FrmResultsListView, 3).Len();
        for(i = 0; i <= WAListViewItemCount(FrmResultsListView) - 1; i++)
        {
            if(GreatestType < WAListViewGetItemText(FrmResultsListView, i, 0).Len()) GreatestType = WAListViewGetItemText(FrmResultsListView, i, 0).Len();
            if(GreatestFile < WAListViewGetItemText(FrmResultsListView, i, 1).Len()) GreatestFile = WAListViewGetItemText(FrmResultsListView, i, 1).Len();
            if(GreatestLn < WAListViewGetItemText(FrmResultsListView, i, 2).Len()) GreatestLn = WAListViewGetItemText(FrmResultsListView, i, 2).Len();
            if(GreatestRelCode < WAListViewGetItemText(FrmResultsListView, i, 3).Len()) GreatestRelCode = WAListViewGetItemText(FrmResultsListView, i, 3).Len();
        }
        ToAdd = "";
        HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 0);
        if((GreatestType - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestType - HeadLab.Len());
        ToWRiteVar[0] = HeadLab + ToAdd;
        ToAdd = "";
        HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 1);
        if((GreatestFile - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestFile - HeadLab.Len());
        ToWRiteVar[1] = HeadLab + ToAdd;
        ToAdd = "";
        HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 2);
        if((GreatestLn - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestLn - HeadLab.Len());
        ToWRiteVar[2] = HeadLab + ToAdd;
        ToAdd = "";
        HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 3);
        if((GreatestRelCode - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestRelCode - HeadLab.Len());
        ToWRiteVar[3] = HeadLab + ToAdd;
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 0)] = ToWRiteVar[0];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 1)] = ToWRiteVar[1];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 2)] = ToWRiteVar[2];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 3)] = ToWRiteVar[3];
        TexttoAdd = Slots[0] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[1] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[2] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[3];
        FRMResultStr = WAControlGetText(hFRMResults) + (CStr) "\r\n\r\n";
        FRMResultStr = FRMResultStr + (CStr) TexttoAdd + (CStr) "\r\n";
        FRMResultStr = FRMResultStr + (CStr) FRMResultStr.String(GreatestType + 1 + GreatestFile + 1 + GreatestLn + 1 + GreatestRelCode + 1, '-').Get_String() + (CStr) "\r\n";
        for(i = 0; i <= WAListViewItemCount(FrmResultsListView) - 1; i++)
        {
            ToAdd = "";
            if((GreatestType - WAListViewGetItemText(FrmResultsListView, i, 0).Len()) > 0) ToAdd = ToAdd.Space(GreatestType - WAListViewGetItemText(FrmResultsListView, i, 0).Len());
            ToWRiteVar[0] = WAListViewGetItemText(FrmResultsListView, i, 0) + (CStr) ToAdd;
            ToAdd = "";
            if((GreatestFile - WAListViewGetItemText(FrmResultsListView, i, 1).Len()) > 0) ToAdd = ToAdd.Space(GreatestFile - WAListViewGetItemText(FrmResultsListView, i, 1).Len());
            ToWRiteVar[1] = WAListViewGetItemText(FrmResultsListView, i, 1) + (CStr) ToAdd;
            ToAdd = "";
            if((GreatestLn - WAListViewGetItemText(FrmResultsListView, i, 2).Len()) > 0) ToAdd = ToAdd.Space(GreatestLn - WAListViewGetItemText(FrmResultsListView, i, 2).Len());
            ToWRiteVar[2] = WAListViewGetItemText(FrmResultsListView, i, 2) + (CStr) ToAdd;
            ToAdd = "";
            if((GreatestRelCode - WAListViewGetItemText(FrmResultsListView, i, 3).Len()) > 0) ToAdd = ToAdd.Space(GreatestRelCode - WAListViewGetItemText(FrmResultsListView, i, 3).Len());
            ToWRiteVar[3] = WAListViewGetItemText(FrmResultsListView, i, 3) + (CStr) ToAdd;
            Slots[WAListViewGetHeaderPosition(FrmResultsListView, 0)] = ToWRiteVar[0];
            Slots[WAListViewGetHeaderPosition(FrmResultsListView, 1)] = ToWRiteVar[1];
            Slots[WAListViewGetHeaderPosition(FrmResultsListView, 2)] = ToWRiteVar[2];
            Slots[WAListViewGetHeaderPosition(FrmResultsListView, 3)] = ToWRiteVar[3];
            TexttoAdd = Slots[0] + (CStr) " ";
            TexttoAdd = TexttoAdd + Slots[1] + (CStr) " ";
            TexttoAdd = TexttoAdd + Slots[2] + (CStr) " ";
            TexttoAdd = TexttoAdd + Slots[3];
            FRMResultStr = FRMResultStr + (CStr) TexttoAdd;
        }
        if(MSaveFile(FName.Get_String(), (long) FRMResultStr.Get_String(), FRMResultStr.Len()) == 0)
        {
            WAMiscMsgBox(hMDIform.hWnd, "Error in saving file: '" + (CStr) FName + (CStr) "'.", MB_ERROR, Requesters);
        }
        else
        {
            WAMiscMsgBox(hMDIform.hWnd, "Results saved as '" + (CStr) FName + (CStr) "'.", MB_INFORMATION, Requesters);
        }
    }
    WAControlEnable(hFRMResults, 1);
}

// -----------------------------------------------------------------------
// Create a new window with results
void NewResultFile(void)
{
    CStr FName;
    CStr Filters;
    int i = 0;
    long GreatestType = 0;
    long GreatestFile = 0;
    long GreatestLn = 0;
    long GreatestRelCode = 0;
    CStr ToWRiteVar[3 + 1];
    CStr ToAdd;
    CStr HeadLab;
    HWND NewChildHandle = 0;
    CStr TexttoAdd;
    CStr Slots[3 + 1];

    StoreLanguageToOpen("");
    NewChildHandle = CreateNewFile("Search results.txt");
    if(NewChildHandle == 0) goto ErrorOpenResult;
    ForceChildFile(NewChildHandle);
    
    // List overview to check for greatest elements
    GreatestType = WAListViewGetHeaderLabel(FrmResultsListView, 0).Len();
    GreatestFile = WAListViewGetHeaderLabel(FrmResultsListView, 1).Len();
    GreatestLn = WAListViewGetHeaderLabel(FrmResultsListView, 2).Len();
    GreatestRelCode = WAListViewGetHeaderLabel(FrmResultsListView, 3).Len();
    for(i = 0; i <= WAListViewItemCount(FrmResultsListView) - 1; i++)
    {
        if(GreatestType < WAListViewGetItemText(FrmResultsListView, i, 0).Len()) GreatestType = WAListViewGetItemText(FrmResultsListView, i, 0).Len();
        if(GreatestFile < WAListViewGetItemText(FrmResultsListView, i, 1).Len()) GreatestFile = WAListViewGetItemText(FrmResultsListView, i, 1).Len();
        if(GreatestLn < WAListViewGetItemText(FrmResultsListView, i, 2).Len()) GreatestLn = WAListViewGetItemText(FrmResultsListView, i, 2).Len();
        if(GreatestRelCode < WAListViewGetItemText(FrmResultsListView, i, 3).Len()) GreatestRelCode = WAListViewGetItemText(FrmResultsListView, i, 3).Len();
    }
    
    ToAdd = "";
    HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 0);
    if((GreatestType - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestType - HeadLab.Len());
    ToWRiteVar[0] = HeadLab + (CStr) ToAdd;
    ToAdd = "";
    HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 1);
    if((GreatestFile - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestFile - HeadLab.Len());
    ToWRiteVar[1] = HeadLab + (CStr) ToAdd;
    ToAdd = "";
    HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 2);
    if((GreatestLn - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestLn - HeadLab.Len());
    ToWRiteVar[2] = HeadLab + (CStr) ToAdd;
    ToAdd = "";
    HeadLab = WAListViewGetHeaderLabel(FrmResultsListView, 3);
    if((GreatestRelCode - HeadLab.Len()) > 0) ToAdd = ToAdd.Space(GreatestRelCode - HeadLab.Len());
    ToWRiteVar[3] = HeadLab + (CStr) ToAdd;
    Slots[WAListViewGetHeaderPosition(FrmResultsListView, 0)] = ToWRiteVar[0];
    Slots[WAListViewGetHeaderPosition(FrmResultsListView, 1)] = ToWRiteVar[1];
    Slots[WAListViewGetHeaderPosition(FrmResultsListView, 2)] = ToWRiteVar[2];
    Slots[WAListViewGetHeaderPosition(FrmResultsListView, 3)] = ToWRiteVar[3];
    TexttoAdd = WAControlGetText(hFRMResults) + (CStr) "\r\n\r\n";
    TexttoAdd = TexttoAdd + (CStr) Slots[0] + (CStr) " ";
    TexttoAdd = TexttoAdd + Slots[1] + (CStr) " ";
    TexttoAdd = TexttoAdd + Slots[2] + (CStr) " ";
    TexttoAdd = TexttoAdd + Slots[3] + (CStr) "\r\n";
    TexttoAdd = TexttoAdd + TexttoAdd.String(GreatestType + 1 + GreatestFile + 1 + GreatestLn + 1 + GreatestRelCode + 1, '-').Get_String() + (CStr) "\r\n";
	for(i = 0; i <= WAListViewItemCount(FrmResultsListView) - 1; i++)
	{
        ToAdd = "";
        if((GreatestType - WAListViewGetItemText(FrmResultsListView, i, 0).Len()) > 0) ToAdd = ToAdd.Space(GreatestType - WAListViewGetItemText(FrmResultsListView, i, 0).Len());
        ToWRiteVar[0] = WAListViewGetItemText(FrmResultsListView, i, 0) + (CStr) ToAdd;
        ToAdd = "";
        if((GreatestFile - WAListViewGetItemText(FrmResultsListView, i, 1).Len()) > 0) ToAdd = ToAdd.Space(GreatestFile - WAListViewGetItemText(FrmResultsListView, i, 1).Len());
        ToWRiteVar[1] = WAListViewGetItemText(FrmResultsListView, i, 1) + (CStr) ToAdd;
        ToAdd = "";
        if((GreatestLn - WAListViewGetItemText(FrmResultsListView, i, 2).Len()) > 0) ToAdd = ToAdd.Space(GreatestLn - WAListViewGetItemText(FrmResultsListView, i, 2).Len());
        ToWRiteVar[2] = WAListViewGetItemText(FrmResultsListView, i, 2) + (CStr) ToAdd;
        ToAdd = "";
        if((GreatestRelCode - WAListViewGetItemText(FrmResultsListView, i, 3).Len()) > 0) ToAdd = ToAdd.Space(GreatestRelCode - WAListViewGetItemText(FrmResultsListView, i, 3).Len());
        ToWRiteVar[3] = WAListViewGetItemText(FrmResultsListView, i, 3) + (CStr) ToAdd;
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 0)] = ToWRiteVar[0];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 1)] = ToWRiteVar[1];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 2)] = ToWRiteVar[2];
        Slots[WAListViewGetHeaderPosition(FrmResultsListView, 3)] = ToWRiteVar[3];
        TexttoAdd = TexttoAdd + (CStr) Slots[0] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[1] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[2] + (CStr) " ";
        TexttoAdd = TexttoAdd + Slots[3] + (CStr) "\r\n";
    }
    SendTextToChild(NewChildHandle, TexttoAdd);
    return;
ErrorOpenResult:
    WAControlEnable(hFRMResults, 0);
    WAMiscMsgBox(hMDIform.hWnd, "Can't create new window.", MB_ERROR, Requesters);
    WAControlEnable(hFRMResults, 1);
}

// -----------------------------------------------------------------------
// Item sorting function
int CALLBACK FRMResultsSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	int ReturnValue = 0;
	long Value1Lng = 0;
	long Value2Lng = 0;
    
	switch(lParamSort)
	{
        case 0:
            ReturnValue = strcmpi(WAListViewGetItemText(FrmResultsListView, lParam1, lParamSort).Get_String(), WAListViewGetItemText(FrmResultsListView, lParam2, lParamSort).Get_String());
			break;
		case 1:
			Value1Lng = WAListViewGetItemText(FrmResultsListView, lParam1, lParamSort).Get_Long();
            Value2Lng = WAListViewGetItemText(FrmResultsListView, lParam2, lParamSort).Get_Long();
            if(Value1Lng > Value2Lng)
            {
				ReturnValue = 1;
            }
            else if(Value1Lng < Value2Lng)
            {
				ReturnValue = -1;
            }
            else
            {
				ReturnValue = 0;
            }
			break;
        case 2:
            ReturnValue = strcmpi(WAListViewGetItemText(FrmResultsListView, lParam1, lParamSort).Get_String(), WAListViewGetItemText(FrmResultsListView, lParam2, lParamSort).Get_String());
			break;
        case 3:
            ReturnValue = strcmpi(WAListViewGetItemText(FrmResultsListView, lParam1, lParamSort).Get_String(), WAListViewGetItemText(FrmResultsListView, lParam2, lParamSort).Get_String());
			break;
    }
    WAListViewReOrder(FrmResultsListView);
	return(ReturnValue);
}
