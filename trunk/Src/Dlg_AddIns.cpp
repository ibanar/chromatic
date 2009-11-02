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
// Dlg_AddIns.cpp: AddIns manager form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "AddIns.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "Dlg_AddIns.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMAddInshWnd;
HWND FRMAddInsCmdOk;
HWND FRMAddInsCmdCancel;
HWND FRMAddInsListView;
HMENU hAddInsMenu;
CList <char *> LocalAddInsFiles;
long CurrentPopupItem;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMAddInsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    long CurrentLvItem = 0;
    long AddInIcon = 0;
    CStr RunningInIniFile;
    POINT PopMenuCoords;
    CStr OpFilters;
    CStr LdFile;
    FARPROC AddInAuthUpDate = 0;
    FARPROC AddInDescUpDate = 0;
    long OldRunningState = 0;
    CStr BufString;

	switch(uMsg)
	{
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMAddInsListView, GetSysColor(COLOR_WINDOW));
			break;
		case WM_INITDIALOG:
            FRMAddInshWnd = hwndDlg;
            WAControlSetText(hwndDlg, "AddIns manager");
            FRMAddInsCmdOk = WACreateButton(327, 292, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMAddInsCmdCancel = WACreateButton(406, 292, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMAddInsListView = WACreateListView(2, 1, 482, 278, hwndDlg, 0, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP | LVS_EX_CHECKBOXES, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMAddInsListView, "Author", 198, 0);
            WAListViewAddCol(FRMAddInsListView, "Description", 265, 1);
            WAListViewSetItemSel(FRMAddInsListView, 0);
            FillAddinsList();
            hAddInsMenu = CreatePopupMenu();
            AppendMenu(hAddInsMenu, MF_STRING, ADDIN_IDBASE + ADDIN_ENABLE, "Load/Unload AddIn");
            WAMenuSetDefaultItem(hAddInsMenu, 0);
            AppendMenu(hAddInsMenu, MF_SEPARATOR, (UINT) -1, "-");
            AppendMenu(hAddInsMenu, MF_STRING, ADDIN_IDBASE + ADDIN_MENU, "Display in AddIns menu");
            AppendMenu(hAddInsMenu, MF_STRING, ADDIN_IDBASE + ADDIN_REMMENU, "Remove from AddIns menu");
            AppendMenu(hAddInsMenu, MF_SEPARATOR, (UINT) -1, "-");
            AppendMenu(hAddInsMenu, MF_STRING, ADDIN_IDBASE + ADDIN_UPDATE, "Update AddIn");
            FreezeTimer = 1;
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            WAGDIDrawHorzSep(hwndDlg, 0, 285, 486);
            EndPaint(hwndDlg, &CreatePs);
			break;
		case WM_COMMAND:
            if((HWND) lParam == FRMAddInsCmdOk)
            {
                SaveAddInList();
                ReFillAddInsArrays();
                FillAddInsMenu();
                RefreshRunningStates();
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMAddInsCmdCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
            }
            switch(wParam)
            {
                case (ADDIN_ENABLE + ADDIN_IDBASE):
                    AddInIcon = WAListViewGetSubItemImage(FRMAddInsListView, CurrentPopupItem, 0);
                    if(AddInIcon == ICON_ADDINR) AddInIcon = ICON_ADDINS;
                    else AddInIcon = ICON_ADDINR;
                    WAListViewSetSubItemImage(FRMAddInsListView, AddInIcon, CurrentPopupItem, 0);
                    return(0);
                case (ADDIN_MENU + ADDIN_IDBASE):
                    WAListViewSetItemCheckbox(FRMAddInsListView, CurrentPopupItem, 1);
                    return(0);
                case (ADDIN_REMMENU + ADDIN_IDBASE):
                    WAListViewSetItemCheckbox(FRMAddInsListView, CurrentPopupItem, 0);
                    return(0);
                case (ADDIN_UPDATE + ADDIN_IDBASE):
                    OpFilters = AppTitle + (CStr) " AddIn (*.dll)|*.dll";
                    LdFile = WAComDlgGetOpenFileName(hwndDlg, OpFilters, LastAddInDir, 0, CurrentDir);
                    if(LdFile.Len() != 0)
                    {
                        // Check if the AddIn is located in AddIns directory
                        if(strcmpi(WAFileGetDirectory(LdFile).Get_String(), Dirs[DIR_ADDINS]) == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "New AddIn can't be located in AddIns directory.", MB_ERROR, Requesters);
                            return(0);
                        }
                        // Check if file exist
                        if(WAFileExist(Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) WAFileGetFileName(LdFile)) == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "No corresponding AddIn found.", MB_ERROR, Requesters);
                            return(0);
                        }
                        // Check if names are identicals
                        if(strcmpi(LocalAddInsFiles.Get(CurrentPopupItem)->Content,
                                   WAFileGetFileName(LdFile).Get_String()) != 0)
                        {
                            WAMiscMsgBox(hwndDlg, "AddIn name doesn't match.", MB_ERROR, Requesters);
                            return(0);
                        }
                        if(IsAddIn(LdFile) == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "Not a Chromatic AddIn.", MB_ERROR, Requesters);
                            return(0);
                        }
                        // Step 1: Shutdown it
                        OldRunningState = ShutDownAddIn(CurrentPopupItem);
                        // Step 2: Delete it
						BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) LocalAddInsFiles.Get(CurrentPopupItem)->Content;
                        DeleteFile(BufString.Get_String());
                        // Step 3: Copy it
                        CopyFile(LdFile.Get_String(), BufString.Get_String(), 0);
                        // Step 4: Restart it
                        RunningAddIns.Set(CurrentPopupItem, OldRunningState);
                        if(RestartAddIn(CurrentPopupItem) == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "Can't update this AddIn.", MB_ERROR, Requesters);
                            return(0);
                        }
                        SetForegroundWindow(FRMAddInshWnd);
                        SetActiveWindow(FRMAddInshWnd);
                        // Step 5: Update listview with possible new infos
                        AddInDescUpDate = GetProcAddress(AddInsDLL.Get(CurrentPopupItem)->Content, "AddInDescription");
                        AddInAuthUpDate = GetProcAddress(AddInsDLL.Get(CurrentPopupItem)->Content, "AddInAuthor");
                        WAListViewSetSelItemText(FRMAddInsListView, GetDLLDescription(LocalAddInsFiles.Get(CurrentPopupItem)->Content, AddInDescUpDate), 0);
                        WAListViewSetSelItemText(FRMAddInsListView, GetDLLAuthor(AddInAuthUpDate), 1);
                        LastAddInDir = WAFileGetDirectory(LdFile);
						WAMiscMsgBox(hwndDlg, "AddIn '" + GetDLLDescription(LocalAddInsFiles.Get(CurrentPopupItem)->Content, AddInDescUpDate) + (CStr) "' updated.", MB_INFORMATION, Requesters);
                    }
                    return(0);
            }
			break;
        case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMAddInsListView)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        CurrentLvItem = WAListViewGetItemUnderCursor(FRMAddInsListView);
                        if(CurrentLvItem != -1)
                        {
                            AddInIcon = WAListViewGetSubItemImage(FRMAddInsListView, CurrentLvItem, 0);
                            if(AddInIcon == ICON_ADDINR) AddInIcon = ICON_ADDINS;
                            else AddInIcon = ICON_ADDINR;
                            WAListViewSetItemSel(FRMAddInsListView, CurrentLvItem);
                            WAListViewSetSubItemImage(FRMAddInsListView, AddInIcon, CurrentLvItem, 0);
                        }
						return(0);
                }
            }
			break;
		case WM_CONTEXTMENU:
            CurrentLvItem = WAListViewGetItemUnderCursor(FRMAddInsListView);
            if(CurrentLvItem != -1)
            {
                WAListViewSetItemSel(FRMAddInsListView, CurrentLvItem);
                CurrentPopupItem = WAListViewGetItemUnderCursor(FRMAddInsListView);
                SetFocus(FRMAddInsListView);
				GetCursorPos(&PopMenuCoords);
                TrackPopupMenu(hAddInsMenu, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopMenuCoords.x, PopMenuCoords.y, 0, hwndDlg, NULL);
            }
			return(0);
        case WM_CLOSE:
            if(hAddInsMenu != 0) DestroyMenu(hAddInsMenu);
            FreezeTimer = 0;
            FRMAddInsListView = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// The the listview with found AddIns
void FillAddinsList(void)
{
    int i = 0;
    CStr AddInName;
    HMODULE AddInLib = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    CStr Description;
    CStr Auth;
    long PosInIniFile = 0;
    CStr LoadedInIniFile;
    long AddInIcon = 0;
    CStr RunningInIniFile;
    CStr BufString;

    AddInName = WAFileDir(&((CStr) Dirs[DIR_ADDINS] + (CStr) "\\*.dll"));
    LocalAddInsFiles.Erase();
    while(AddInName.Len() != 0)
    {
        BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInName;
		AddInLib = LoadLibrary(BufString.Get_String());
        if(AddInLib != 0)
        {
            AddInDesc = GetProcAddress(AddInLib, "AddInDescription");
            AddInAuth = GetProcAddress(AddInLib, "AddInAuthor");
            AddInLoad = GetProcAddress(AddInLib, "AddInLoad");
            AddInUnload = GetProcAddress(AddInLib, "AddInUnLoad");
            AddInMenu = GetProcAddress(AddInLib, "AddInMenu");
            // Must have all procedures
            if(AddInDesc != 0 && AddInAuth != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
            {
                Description = GetDLLDescription(AddInName, AddInDesc);
                Auth = GetDLLAuthor(AddInAuth);
                AddInIcon = ICON_ADDINR;
                PosInIniFile = CheckAddInIniList(AddInName);
                if(PosInIniFile != -1)
                {
                    // Check if it is running if found in ini file
                    RunningInIniFile = WAIniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(PosInIniFile, 3).Get_String(), MainIniFile);
                    if(strcmpi(RunningInIniFile.Get_String(), "1") == 0) AddInIcon = ICON_ADDINS;
                }
                WAListViewAddItem(FRMAddInsListView, Description, i, AddInIcon);
                WAListViewSetSubItem(FRMAddInsListView, Auth, i, 1);
                // Check in the ini file if it's been already loaded
                PosInIniFile = CheckAddInIniList(AddInName);
                if(PosInIniFile != -1)
                {
                    LoadedInIniFile = WAIniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(PosInIniFile, 3).Get_String(), MainIniFile);
                    if(LoadedInIniFile.Len() != 0)
                    {
                        if(strcmpi(LoadedInIniFile.Get_String(), "1") == 0)
                        {
                            WAListViewSetItemCheckbox(FRMAddInsListView, i, 1);
                        }
                        else
                        {
                            WAListViewSetItemCheckbox(FRMAddInsListView, i, 0);
                        }
                    }
                    else
                    {
                        WAListViewSetItemCheckbox(FRMAddInsListView, i, 0);
                    }
                }
                LocalAddInsFiles.Add(AddInName.Get_String());
                i++;
            }
            FreeLibrary(AddInLib);
            // Dlg_Filters.cpp for more infos
            #ifndef _DEBUG
				CloseHandle(AddInLib);
			#endif
		}
        AddInName = WAFileDir();
    }
}

// -----------------------------------------------------------------------
// Save the list of selected addins
void SaveAddInList(void)
{
    int i = 0;
    long NewAddInPos = 0;
    long AddInIcon = 0;

    // Delete every entries
    WAIniDeleteKey("AddIns", "", MainIniFile);
    // First refresh ini file with found addins
    for(i = 0; i <= (long) WAListViewItemCount(FRMAddInsListView) - 1; i++)
    {
        // Obtain an empty slot
        NewAddInPos = GetAddInSlot();
        WAIniWriteKey("AddIns", "File" + (CStr) StringNumberComplement(NewAddInPos, 3).Get_String(), LocalAddInsFiles.Get(i)->Content, MainIniFile);
        WAIniWriteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(NewAddInPos, 3).Get_String(), WAListViewGetItemCheckbox(FRMAddInsListView, i), MainIniFile);
        // User wants to run it now ?
        AddInIcon = WAListViewGetSubItemImage(FRMAddInsListView, i, 0);
        if(AddInIcon == ICON_ADDINS) WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NewAddInPos, 3).Get_String(), "1", MainIniFile);
        else WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NewAddInPos, 3).Get_String(), "0", MainIniFile);
    }
    // Save old datas
    OldRunningAddIns.Erase();
	OldRunningAddIns.MAdd(RunningAddIns.Amount(), 0L);
    OldAddInsFiles.Erase();
	OldAddInsFiles.MAdd(AddInsFiles.Amount(), "");
    OldAddInsDLL.Erase();
    OldAddInsDLL.MAdd(AddInsDLL.Amount(), 0L);
    for(i = 0; i < RunningAddIns.Amount(); i++)
    {
        // Save names
        OldAddInsFiles.Set(i, AddInsFiles.Get(i)->Content);
        // Save handles
        OldAddInsDLL.Set(i, AddInsDLL.Get(i)->Content);
        // Save running states
        OldRunningAddIns.Set(i, RunningAddIns.Get(i)->Content);
    }
}

// -----------------------------------------------------------------------
// Refresh the running states of the addins after manager modifications
void RefreshRunningStates(void)
{
    long OldAddInIndex = 0;
    long AddInIndex = 0;
    CStr AddInFileName;
    CStr AddInRunning;
    int i = 0;

    // Run them or not
    for(i = 0; i <= 999; i++)
    {
        AddInFileName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = WAIniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        OldAddInIndex = SearchOldAddInName(AddInFileName);
        if(OldAddInIndex != -1)
        {
            // Want to it to run ?
            if(strcmpi(AddInRunning.Get_String(), "1") == 0)
            {
                // Let's run it (if it wasn't running before)
                if(OldRunningAddIns.Get(OldAddInIndex)->Content != 1)
                {
                    AddInIndex = SearchAddInName(AddInFileName);
                    if(AddInsLoad.Get(AddInIndex)->Content != 0)
                    {
                        switch(JumpToAddrArg(AddInsLoad.Get(AddInIndex)->Content, (long) &AddInLib))
                        {
                            case ADDIN_FINISHED:
                                RunningAddIns.Set(AddInIndex, 0);
                                // Unload it now
                                if(AddInsUnLoad.Get(AddInIndex)->Content != 0)
                                {
                                    JumpToAddr(AddInsUnLoad.Get(AddInIndex)->Content);
                                    WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", MainIniFile);
                                }
								break;
                            case ADDIN_PERSISTANT:
                                RunningAddIns.Set(AddInIndex, 1);
                                // Mark it as persistantly running
                                WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "1", MainIniFile);
								break;
						}
                    }
                }
            }
            else
            {
                // Let's stop it (if it wasn't stopped before)
                if(OldRunningAddIns.Get(OldAddInIndex)->Content != 0)
                {
                    AddInIndex = SearchAddInName(AddInFileName);
					if(AddInsUnLoad.Get(AddInIndex)->Content != 0)
					{
                        JumpToAddr(AddInsUnLoad.Get(AddInIndex)->Content);
                        WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", MainIniFile);
                    }
                }
            }
        }
        else
        {
            // Not found to check if user wants it to be running now
            if(strcmpi(AddInRunning.Get_String(), "1") == 0)
            {
                AddInIndex = SearchAddInName(AddInFileName);
                if(AddInsLoad.Get(AddInIndex)->Content != 0)
                {
                    switch(JumpToAddrArg(AddInsLoad.Get(AddInIndex)->Content, (long) &AddInLib))
                    {
                        case ADDIN_FINISHED:
                            RunningAddIns.Set(AddInIndex, 0);
                            // Unload it now
                            if(AddInsUnLoad.Get(AddInIndex)->Content != 0)
                            {
                                JumpToAddr(AddInsUnLoad.Get(AddInIndex)->Content);
                                WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", MainIniFile);
                            }
							break;
                        case ADDIN_PERSISTANT:
                            RunningAddIns.Set(AddInIndex, 1);
                            // Mark it as persistantly running
                            WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "1", MainIniFile);
							break;
					}
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Search an addin in the list
long CheckAddInIniList(CStr FileName)
{
    int i = 0;
    long ReturnValue = -1;
    CStr AddInName;

    for(i = 0; i <= 999; i++)
    {
        AddInName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInName.Len() == 0) break;
        if(strcmpi(AddInName.Get_String(), FileName.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the last slot available for an addin in the ini file
long GetAddInSlot(void)
{
    int i = 0;
    long ReturnValue = 0;
    CStr AddInName;

    for(i = 0; i <= 999; i++)
    {
        AddInName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInName.Len() == 0) break;
        ReturnValue++;
    }
    return(ReturnValue);
}
