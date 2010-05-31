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
// Dlg_Favorites.cpp: Favorite files form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "AddIns.h"
#include "Menus.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Favorites.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMFavListView;
HWND FRMFavCmdSave;
HWND FRMFavCmdCancel;
HWND FRMFavCmdOpen;
HWND FRMFavToolbar;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFavoritesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT Ps;
    int i = 0;
    int j = 0;
    CStr NameToAdd;
    long CurrentSelected = 0;
	long *FavFilesArray = 0;
	CStr SelFavFiles;
	LV_KEYDOWN *lParamKey = 0;
    
	switch(uMsg)
	{
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMFavListView, GetSysColor(COLOR_WINDOW));
			break;
        case WM_INITDIALOG:
            WAControlSetText(hwndDlg, "Favorite files");
            FRMFavCmdSave = WACreateButton(327, 292, 77, 23, hwndDlg, "Save", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMFavCmdCancel = WACreateButton(406, 292, 77, 23, hwndDlg, "Close", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMFavCmdOpen = WACreateButton(243, 292, 77, 23, hwndDlg, "Open", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMFavToolbar = WACreateToolBar(2, 1, 449, 23, hwndDlg, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMFavToolbar, "", FAVADDFILES, ICON_ADDFILES, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFavToolbar, "", FAVADDALLDIRECTORY, ICON_ADDDIRECTORY, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFavToolbar, "", FAVADDACTIVEFILE, ICON_ADDACTIVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFavToolbar, "", FAVADDALLACTIVEFILES, ICON_ADDALLACTIVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMFavToolbar, 0);
            WAToolBarAddButton(FRMFavToolbar, "", FAVREMSELECTION, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMFavListView = WACreateListView(2, 26, 482, 253, hwndDlg, 5, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMFavListView, "Path", 280, 0);
            WAListViewAddCol(FRMFavListView, "Name", 180, 1);
			LoadFavList();
            FreezeTimer = 1;
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == FRMFavCmdOpen)
            {
				OpenFavFiles();
                return(0);
            }
            else if((HWND) lParam == FRMFavCmdSave)
            {
                SaveFavList();
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFavCmdCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFavToolbar)
            {
                switch(wParam)
                {
                    case FAVADDFILES:
						SelFavFiles = WAComDlgGetOpenFileName(hwndDlg, "All files (*.*)|*.*","", 1, CurrentDir);
						WAComDlgParseMultiFilesSelection(SelFavFiles, &EnumFavFiles, MULTIFILESENUM_FORWARD, 0);
                        return(0);
                    case FAVADDALLDIRECTORY:
                        NameToAdd = WAComDlgBrowseForFolder(hwndDlg, "Select the directory to add...");
                        if(NameToAdd.Len() != 0)
                        {
                            if(WAListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
                            {
								if(CheckFavFile(NameToAdd) == 0)
								{
	                                j = WAListViewItemCount(FRMFavListView);
									WAListViewAddItem(FRMFavListView, NameToAdd, j, ICON_NEW);
									WAListViewSetSubItem(FRMFavListView, NameToAdd, j, 1);
								}
							}
                        }
                        return(0);
                    case FAVADDACTIVEFILE:
                        if(NbForms != 0)
                        {
                            ChildStruct = LoadStructure(CurrentForm);
                            if(ChildStruct->FileLoaded == 1)
                            {
                                NameToAdd = CMGetRealFile(ChildStruct->RFile);
                                if(WAListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
                                {
                                    if(CheckFavFile(NameToAdd) == 0)
                                    {
										j = WAListViewItemCount(FRMFavListView);
										WAListViewAddItem(FRMFavListView, WAFileGetFileName(NameToAdd), j, ICON_NEW);
										WAListViewSetSubItem(FRMFavListView, NameToAdd, j, 1);
									}
								}
                            }
                        }
                        return(0);
                    case FAVADDALLACTIVEFILES:
                        WAMMEnumSourceWindows((long) &AddAllActiveFilesProc);
                        return(0);
                    case FAVREMSELECTION:
						RemFavFiles();
                        return(0);
                }
            }
			break;
		case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMFavListView)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
						case LVN_KEYDOWN:
							lParamKey = (LV_KEYDOWN *) lParam;
							if((lParamKey->wVKey & 0xffff) == 46) RemFavFiles();
							break;
						case NM_DBLCLK:
							OpenFavFiles();
							break;
				}
            }
            else
            {
				switch(WAControlGetNotifiedMsg(lParam))
				{
					case TTN_NEEDTEXT:
						switch(WAControlGetNotifiedID(lParam))
						{
							case FAVADDFILES:
								WAToolBarDisplayToolTip("Add file(s) into list", lParam);
								return(0);
							case FAVADDACTIVEFILE:
								WAToolBarDisplayToolTip("Add active file into list", lParam);
								return(0);
							case FAVADDALLACTIVEFILES:
								WAToolBarDisplayToolTip("Add all opened files into list", lParam);
								return(0);
							case FAVADDALLDIRECTORY:
								WAToolBarDisplayToolTip("Add a directory into list", lParam);
								return(0);
							case FAVREMSELECTION:
								WAToolBarDisplayToolTip("Remove selected file(s) from the list", lParam);
								return(0);
						}
						break;
				}
			}
			break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            WAGDIDrawHorzSep(hwndDlg, 0, WAControlClientHeight(hwndDlg) - 33, 486);
            EndPaint(hwndDlg, &Ps);
			break;
        case WM_CLOSE:
			CreateFavoritesMenu();
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
	}
    return(0);
}

// -----------------------------------------------------------------------
// Callback for WAMMEnumChilds
long CALLBACK AddAllActiveFilesProc(HWND hWnd)
{
    CStr NameToAdd;
    int i = 0;

    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->FileLoaded == 1)
    {
        NameToAdd = CMGetRealFile(ChildStruct->RFile);
        if(WAListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
        {
			if(CheckFavFile(NameToAdd) == 0)
			{
				i = WAListViewItemCount(FRMFavListView);
				WAListViewAddItem(FRMFavListView, WAFileGetFileName(NameToAdd), i, ICON_NEW);
				WAListViewSetSubItem(FRMFavListView, NameToAdd, i, 1);
			}
		}
    }
    return(1);
}

// -----------------------------------------------------------------------
// save the list of favorite files into main ini file
void SaveFavList(void)
{
    int i = 0;

    // Remove the section
    WAIniDeleteKey("Favorites", "", MainIniFile);
    // And reconstruct it
    for(i = 0; i <= WAListViewItemCount(FRMFavListView) - 1; i++)
    {
        WAIniWriteKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), WAListViewGetItemText(FRMFavListView, i, 1), MainIniFile);
    }
}

// -----------------------------------------------------------------------
// Laod the list of favorite files from main ini file
void LoadFavList(void)
{
    int i = 0;
    int j = 0;
	CStr FileToAdd;

	WAListViewClear(FRMFavListView);
    for(i = 0; i <= 999; i++)
    {
        FileToAdd = WAIniReadKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
		if(FileToAdd.Len() == 0) break;
		j = WAListViewItemCount(FRMFavListView);
        WAListViewAddItem(FRMFavListView, WAFileGetFileName(FileToAdd), j, ICON_NEW);
		WAListViewSetSubItem(FRMFavListView, FileToAdd, j, 1);
	}
}

// -----------------------------------------------------------------------
// Check if an entry is already in favorite files list
long CheckFavFile(CStr FavFileToCheck)
{
	int i = 0;

	for(i = 0; i <= WAListViewItemCount(FRMFavListView) - 1; i++)
	{
		if(strcmpi(WAListViewGetItemText(FRMFavListView, i, 1).Get_String(), FavFileToCheck.Get_String()) == 0)
		{
			return(1);
		}
	}
	return(0);
}

// -----------------------------------------------------------------------
// Open the selected favorites files
void OpenFavFiles(void)
{
	long CurrentSelected = 0;

    CurrentSelected = WAListViewGetSelItem(FRMFavListView, -1);
    while(CurrentSelected != -1)
    {
		OpenUnknownFile(WAListViewGetItemText(FRMFavListView, CurrentSelected, 1), TRUE);
        CurrentSelected = WAListViewGetSelItem(FRMFavListView, CurrentSelected);
    }
}

// -----------------------------------------------------------------------
// Remove the selected favorites files
void RemFavFiles(void)
{
	long CurrentSelected;

	CurrentSelected = WAListViewGetSelItem(FRMFavListView, -1);
	while(CurrentSelected != -1)
	{
		WAListViewDeleteItem(FRMFavListView, CurrentSelected);
		CurrentSelected = WAListViewGetSelItem(FRMFavListView, -1);
	}
}

// -----------------------------------------------------------------------
// Callback to add the selected favorites files
long CALLBACK EnumFavFiles(char *FileToAdd, long UserValue)
{
	CStr BufString;
	int j;

	BufString = FileToAdd;
	if(WAListViewFindSubItem(FRMFavListView, BufString, 1, CASE_UNSENSITIVE) == -1)
	{
		if(CheckFavFile(BufString) == 0)
		{
			j = WAListViewItemCount(FRMFavListView);
            WAListViewAddItem(FRMFavListView, WAFileGetFileName(BufString), j, ICON_NEW);
			WAListViewSetSubItem(FRMFavListView, BufString, j, 1);
		}
	}
	// Continue enumeration
	return(1);
}
