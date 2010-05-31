// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2010 Franck Charlet.
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
// Dlg_Constants.cpp: Project extra constants creation
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ProjectsBuilder.h"
#include "Globals.h"
#include "Dlg_Strings.h"

// -----------------------------------------------------------------------
// Constants
#define CONSTANT_CREATE 1
#define CONSTANT_REPLACE 2
#define CONSTANT_DELETE 3

// -----------------------------------------------------------------------
// Variables
HWND FRMConstantshwnd;
HWND hFRMConstantsOk;
HWND hFRMConstantsCancel;
HWND FRMConstantsToolbar;
HWND FRMConstantsListview;
HWND FRMConstantsConstName;
HWND FRMConstantsConstValue;
long LVConstantsInEdit;

// -----------------------------------------------------------------------
// Functions
int CALLBACK FRMConstantsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int SaveConstantsFile(void);
LRESULT CALLBACK LVConstantsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CheckConstantsSymbol(char *Constant_Name);

// -----------------------------------------------------------------------
// Create an empty string resource
void CreateConstants(HWND hParent)
{
    WACreateModalDialog(-1, -1, 410, 289, hParent, &FRMConstantsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMConstantsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    PAINTSTRUCT EnterValuePs;
    long CurrentSelected = 0;
    CStr CurrentStringFromStrings;
    CStr CurrentStringFromStrings2;
    CStr Constant_Name;
    CStr Constant_Value;

	switch(uMsg)
	{
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMConstantsListview, GetSysColor(COLOR_WINDOW));
			break;
		case WM_INITDIALOG:
            WAControlSetText(hwndDlg, "Project constants");
            FRMConstantshwnd = hwndDlg;
            hFRMConstantsOk = WACreateButton(251, 263, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hFRMConstantsCancel = WACreateButton(330, 263, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMConstantsToolbar = WACreateToolBar(1, 1, 410, 23, hwndDlg, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
			WAToolBarAddButton(FRMConstantsToolbar, "", CONSTANT_CREATE, ICON_ADDCONST, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			WAToolBarAddButton(FRMConstantsToolbar, "", CONSTANT_REPLACE, ICON_REPLACECONST, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			WAToolBarAddSeparator(FRMConstantsToolbar, 0);
			WAToolBarAddButton(FRMConstantsToolbar, "", CONSTANT_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			FRMConstantsListview = WACreateListView(2, 25, 406, 206, hwndDlg, 4, GlobalImageList3, &LVConstantsHook, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL | WS_TABSTOP, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMConstantsListview, "Name", 303, 1);
            WAListViewAddCol(FRMConstantsListview, "Value", 80, 0);
            WAListViewSetHeaderPosition(FRMConstantsListview, 0, 1);
            WAListViewSetHeaderPosition(FRMConstantsListview, 1, 0);
			FRMConstantsConstName = WACreateTextBox(2, 232, 300, 20, hwndDlg, "", 5, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
			FRMConstantsConstValue = WACreateTextBox(304, 232, 104, 20, hwndDlg, "", 6, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            // Fill the list
            for(i = 0; i < 999; i++)
            {
                CurrentStringFromStrings = WAIniReadKey("Constants", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
                if(CurrentStringFromStrings.Len() == 0) break;
                CurrentStringFromStrings2 = WAIniReadKey("Constants", "Value" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
				WAListViewAddItem(FRMConstantsListview, CurrentStringFromStrings2, i, -1);
                WAListViewSetSubItem(FRMConstantsListview, CurrentStringFromStrings, i, 1);
            }
			WAControlSetText(FRMConstantsConstName, WAListViewGetItemText(FRMConstantsListview, 0, 0));
			WAControlSetText(FRMConstantsConstValue, WAListViewGetItemText(FRMConstantsListview, 0, 1));
            if(WAListViewItemCount(FRMConstantsListview) > 0) WAListViewSetItemSel(FRMConstantsListview, 0);
			SetFocus(FRMConstantsListview);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            WAGDIDrawHorzSep(hwndDlg, 0, 256, 410);
            EndPaint(hwndDlg, &EnterValuePs);
			break;
        case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMConstantsListview)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
					case NM_CLICK:
					case LVN_ITEMCHANGED:
						CurrentSelected = WAListViewGetSelItem(FRMConstantsListview, -1);
						if(CurrentSelected != -1)
						{
							WAControlSetText(FRMConstantsConstName, WAListViewGetItemText(FRMConstantsListview, CurrentSelected, 1));
							WAControlSetText(FRMConstantsConstValue, WAListViewGetItemText(FRMConstantsListview, CurrentSelected, 0));
                        }
						return(0);
                }
            }
			switch(WAControlGetNotifiedMsg(lParam))
			{
                case TTN_NEEDTEXT:
                    switch(WAControlGetNotifiedID(lParam))
                    {
                        case CONSTANT_CREATE:
                            WAToolBarDisplayToolTip("Create new constant", lParam);
                            return(0);
                        case CONSTANT_REPLACE:
                            WAToolBarDisplayToolTip("Replace constant", lParam);
                            return(0);
                        case CONSTANT_DELETE:
                            WAToolBarDisplayToolTip("Delete selected constant", lParam);
                            return(0);
                    }
			}
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMConstantsToolbar)
            {
                switch(wParam)
                {
                    case CONSTANT_CREATE:
						Constant_Name = WAControlGetText(FRMConstantsConstName);
						if(Constant_Name.Len())
						{
							if(CheckConstantsSymbol(Constant_Name.Get_String()) >= 0)
							{
								WAMiscMsgBox(hwndDlg, "Constant already exists.", MB_ERROR, Requesters);
								SetFocus(FRMConstantsConstName);
								break;
							}
							if(StringIsLabel(Constant_Name))
							{
								Constant_Value = WAControlGetText(FRMConstantsConstValue);
								i = 0;
								if(Constant_Value[i] == '-') i++;
								// Only accept dec and hex values
								if(StringIsDecimal(Constant_Value.Get_String(i), 10))
								{
									WAListViewAddItem(FRMConstantsListview, Constant_Value.Get_String(), WAListViewItemCount(FRMConstantsListview), -1);
								}
								else if(StringIsHexaDecimal(Constant_Value))
								{
									WAListViewAddItem(FRMConstantsListview, Constant_Value.Get_String(), WAListViewItemCount(FRMConstantsListview), -1);
								}
								else
								{
									WAMiscMsgBox(hwndDlg, "Invalid constant value.", MB_ERROR, Requesters);
									SetFocus(FRMConstantsConstValue);
									break;
								}
								WAListViewSetSubItem(FRMConstantsListview, Constant_Name, WAListViewItemCount(FRMConstantsListview) - 1, 1);
								WAListViewSetItemSel(FRMConstantsListview, WAListViewItemCount(FRMConstantsListview) - 1);
								SetFocus(FRMConstantsListview);
								break;
							}
						}
						WAMiscMsgBox(hwndDlg, "Invalid constant name.", MB_ERROR, Requesters);
						SetFocus(FRMConstantsConstName);
						break;
					case CONSTANT_REPLACE:
						CurrentSelected = WAListViewGetSelItem(FRMConstantsListview, -1);
						if(CurrentSelected != -1)
						{
							Constant_Name = WAControlGetText(FRMConstantsConstName);
							if(Constant_Name.Len())
							{
								if(CheckConstantsSymbol(Constant_Name.Get_String()) != CurrentSelected)
								{
									WAMiscMsgBox(hwndDlg, "Constant already exists.", MB_ERROR, Requesters);
									SetFocus(FRMConstantsConstName);
									break;
								}
								if(StringIsLabel(Constant_Name))
								{
									Constant_Value = WAControlGetText(FRMConstantsConstValue);
									i = 0;
									if(Constant_Value[i] == '-') i++;
									// Only accept dec and hex values
									if(StringIsDecimal(Constant_Value.Get_String(i), 10))
									{
										WAListViewSetSubItem(FRMConstantsListview, Constant_Value.Get_String(), CurrentSelected, 0);
									}
									else if(StringIsHexaDecimal(Constant_Value))
									{
										WAListViewSetSubItem(FRMConstantsListview, Constant_Value.Get_String(), CurrentSelected, 0);
									}
									else
									{
										WAMiscMsgBox(hwndDlg, "Invalid constant value.", MB_ERROR, Requesters);
										SetFocus(FRMConstantsConstValue);
										break;
									}
									WAListViewSetSubItem(FRMConstantsListview, Constant_Name, CurrentSelected, 1);
									WAListViewSetItemSel(FRMConstantsListview, WAListViewItemCount(FRMConstantsListview) - 1);
									SetFocus(FRMConstantsListview);
									break;
								}
            				}
							WAMiscMsgBox(hwndDlg, "Invalid constant name.", MB_ERROR, Requesters);
							SetFocus(FRMConstantsConstName);
							break;
						}
						break;
					case CONSTANT_DELETE:
						break;
				}
				return(0);
			}
			if((HWND) lParam == hFRMConstantsOk)
			{
                if(SaveConstantsFile()) WAControlClose(hwndDlg);
				return(0);
			}
			else if((HWND) lParam == hFRMConstantsCancel)
			{
                WAControlClose(hwndDlg);
                return(0);
			}
			break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Save a constants file for current project directly in project file
int SaveConstantsFile(void)
{
    int i = 0;
	int k = 0;
    HANDLE StrFileHandle = 0;

    for(i = 0; i < WAListViewItemCount(FRMConstantsListview); i++)
    {
		if(WAListViewGetItemText(FRMConstantsListview, i, 0).Len() != 0 && WAListViewGetItemText(FRMConstantsListview, i, 1).Len() != 0)
		{
			if(WAListViewGetItemText(FRMConstantsListview, i, 0).Len() == 0 || WAListViewGetItemText(FRMConstantsListview, i, 1).Len() == 0)
			{
				WAMiscMsgBox(FRMConstantshwnd, "Entry is incomplete.", MB_ERROR, Requesters);
				// Lead user to the faulty entry
				WAListViewSetItemSel(FRMConstantsListview, i);
				SetFocus(FRMConstantsListview);
				return(0);
			}
			WAIniWriteKey("Constants", "Name" + (CStr) StringNumberComplement(k, 3).Get_String(), WAListViewGetItemText(FRMConstantsListview, i, 1), ProjectFName);
			WAIniWriteKey("Constants", "Value" + (CStr) StringNumberComplement(k, 3).Get_String(), WAListViewGetItemText(FRMConstantsListview, i, 0), ProjectFName);
			k++;
		}
	}
	return(1);
}

// -----------------------------------------------------------------------
// Listview hook
LRESULT CALLBACK LVConstantsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(LVStringsInEdit == 0)
    {
        switch(uMsg)
        {
            case WM_CHAR:
                if(WAListViewPasteAutoEdit(FRMConstantsListview, wParam, 0) == 1)
                {
                    LVConstantsInEdit = 1;
                    return(0);
                }
        }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Check if a constant already exists in list
// Returns -1 if not found or position if found
int CheckConstantsSymbol(char *Constant_Name)
{
    for(int i = 0; i < WAListViewItemCount(FRMConstantsListview); i++)
    {
		if(lstrcmpi(WAListViewGetItemText(FRMConstantsListview, i, 1).Get_String(), Constant_Name) == 0)
		{
			return(i);
		}
	}
	return(-1);
}
