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
// Dlg_SearchInFiles.cpp: Search results form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "Imagelists.h"
#include "ConstRes.h"
#include "Main.h"
#include "Languages.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Results.h"
#include "Dlg_SearchInFiles.h"

// -----------------------------------------------------------------------
// Constants
#define EXT_IDBASE 3000

// -----------------------------------------------------------------------
// Variables
HWND FRMFileSearchhWnd;
HWND FRMFileSearchhFrame;
HWND FRMFileSearchComboKeyWord;
HWND FRMFileSearchComboExt;
HWND FRMFileSearchComboDir;
HWND FRMFileSearchTBKey;
HWND FRMFileSearchTBExt;
HWND FRMFileSearchTBSel;
HWND FRMFileSearchChkVariables;
HWND FRMFileSearchChkCaseSens;
HWND FRMFileSearchChkIncludes;
HWND FRMFileSearchChkSubDirs;
HWND FRMFileSearchChkRaw;
HWND FRMFileSearchOk;
HWND FRMFileSearchCancel;
HWND FRMFileSearchFileLabel;
long SearchRawResult;
CStr FRMFileSearchSearchDir;
CList <char *> Directories;
CStr FRMFileSearchCommChar;
CStr FRMFileSearchSearchWord;
long FRMFileSearchInCalc;
long FRMFileSearchStopCalc;
WIN32_FIND_DATA FindIncludeDat;
CList <char *> Includes;
CList <int> IncludesBol;
CList <char *> SearchExtensions;
long *WildCardArray;
HMENU hExtensionsMenu;
long ExtensionsNumber;

long FRMFileSearchChkVariablesValue;
long FRMFileSearchChkCaseSensValue;
long FRMFileSearchChkIncludesValue;
long FRMFileSearchChkSubDirsValue;
long FRMFileSearchChkRawValue;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFileSearchProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
	CStr ExtToAdd;
    POINT PopMenuCoords;
	long *ExtArray;
	PAINTSTRUCT FileSearchPs;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMFileSearchhWnd = hwndDlg;
            OkSearch = 0;
            WAControlSetText(hwndDlg, "Search in files");
			FRMFileSearchhFrame = WACreateFrame(2, -4, 262, 196, hwndDlg, "", 0, 0, 0);
            WACreateLabel(5, 11, 180, 14, FRMFileSearchhFrame, "Keyword :", 0, 0, 0, 0);
			WACreateLabel(5, 49, 180, 14, FRMFileSearchhFrame, "Extensions :", 0, 0, 0, 0);
            WACreateLabel(5, 87, 180, 14, FRMFileSearchhFrame, "Directory :", 0, 0, 0, 0);
            FRMFileSearchFileLabel = WACreateLabel(2, 194, 345, 16, hwndDlg, "", 103, 0, 0, 0);
            FRMFileSearchOk = WACreateButton(268, 2, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMFileSearchCancel = WACreateButton(268, 26, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
			FRMFileSearchComboKeyWord = WACreateComboBox(7, 22, 225, 100, hwndDlg, "", 3, 0, WS_TABSTOP | CBS_DROPDOWN | WS_GROUP);
            FRMFileSearchTBKey = WACreateToolBar(234, 21, 25, 23, hwndDlg, GlobalImageList1, 5, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMFileSearchTBKey, "", FRMFileSearchBUTTONCLEARKEY, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			FRMFileSearchComboExt = WACreateComboBox(7, 60, 201, 100, hwndDlg, LastWild, 4, 0, WS_TABSTOP | CBS_DROPDOWN);
            FRMFileSearchTBExt = WACreateToolBar(210, 59, 50, 23, hwndDlg, GlobalImageList1, 5, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMFileSearchTBExt, "", FRMFileSearchBUTTONEXT, ICON_MENUDROPDOWN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFileSearchTBExt, "", FRMFileSearchBUTTONCLEAREXT, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			FRMFileSearchComboDir = WACreateComboBox(7, 98, 201, 100, hwndDlg, WAIniReadKey("Layout", "SearchDir", MainIniFile), 6, 0, WS_TABSTOP | CBS_DROPDOWN);
            FRMFileSearchTBSel = WACreateToolBar(210, 97, 50, 23, hwndDlg, GlobalImageList1, 7, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMFileSearchTBSel, "", FRMFileSearchBUTTONOPEN, ICON_DIRSEARCH, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFileSearchTBSel, "", FRMFileSearchBUTTONCLEAROPEN, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
			FRMFileSearchChkVariables = WACreateCheckBox(14, 131, 117, 19, hwndDlg, "Search for variables", 8, 0, WS_TABSTOP, 0);
            FRMFileSearchChkCaseSens = WACreateCheckBox(14, 150, 117, 19, hwndDlg, "Case sensitive", 9, 0, WS_TABSTOP, 0);
            FRMFileSearchChkRaw = WACreateCheckBox(14, 169, 117, 19, hwndDlg, "Raw results", 10, 0, WS_TABSTOP, 0);
            FRMFileSearchChkIncludes = WACreateCheckBox(138, 131, 117, 19, hwndDlg, "Search in includes", 11, 0, WS_TABSTOP, 0);
            FRMFileSearchChkSubDirs = WACreateCheckBox(138, 150, 117, 19, hwndDlg, "Sub directories", 12, 0, WS_TABSTOP, 0);
            if(NbForms != 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                WAControlSetText(FRMFileSearchComboKeyWord, GetCurrentWord(ChildStruct->hChildCodeMax));
            }
            CheckBoxSetState(FRMFileSearchChkVariables, FRMFileSearchChkVariablesValue);
            CheckBoxSetState(FRMFileSearchChkCaseSens, FRMFileSearchChkCaseSensValue);
            CheckBoxSetState(FRMFileSearchChkIncludes, FRMFileSearchChkIncludesValue);
            CheckBoxSetState(FRMFileSearchChkSubDirs, FRMFileSearchChkSubDirsValue);
            CheckBoxSetState(FRMFileSearchChkRaw, FRMFileSearchChkRawValue);
            if(FRMFileSearchChkRawValue == 1) WAControlEnable(FRMFileSearchChkVariables, 0);
            else WAControlEnable(FRMFileSearchChkVariables, 1);
            WAControlBringToTop(FRMFileSearchTBSel);
            WAControlBringToTop(FRMFileSearchComboDir);
            WAControlBringToTop(FRMFileSearchTBExt);
            WAControlBringToTop(FRMFileSearchComboExt);
            WAControlBringToTop(FRMFileSearchTBKey);
            WAControlBringToTop(FRMFileSearchComboKeyWord);
            WAControlEnable(FRMFileSearchOk, 0);
            WAControlSendMessage(hwndDlg, CBN_EDITCHANGE << 16, (long) FRMFileSearchComboKeyWord);
            // Create the extensions menus
            hExtensionsMenu = CreatePopupMenu();
			ExtensionsNumber = 0;
			SearchExtensions.Erase();
			for(i = 0;i <= 999; i++)
			{
				ExtToAdd = WAIniReadKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
				if(ExtToAdd.Len() == 0) break;
				ExtArray = StringSplit(ExtToAdd, "|");
				if(StringGetSplitUBound(ExtArray) == 1)
				{
					// Extract extensions and store them
					SearchExtensions.Add(StringGetSplitElement(ExtToAdd, ExtArray, 1).Get_String());
					AppendMenu(hExtensionsMenu, MF_STRING, EXT_IDBASE + i, ExtToAdd.Get_String());
					ExtensionsNumber++;
				}
				StringReleaseSplit(ExtArray);
			}
            WAComboBoxFillFromIniFile(FRMFileSearchComboKeyWord, "SearchKeys", MainIniFile);
            WAComboBoxFillFromIniFile(FRMFileSearchComboExt, "SearchExts", MainIniFile);
            WAComboBoxFillFromIniFile(FRMFileSearchComboDir, "SearchDirs", MainIniFile);
			FreezeTimer = 1;
            return(1);
        case WM_COMMAND:
            if((HWND) lParam == FRMFileSearchOk)
            {
                RunfileSearch(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchCancel)
            {
                OkSearch = 0;
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchTBKey)
            {
                switch(wParam)
                {
					case FRMFileSearchBUTTONCLEARKEY:
                        WAComboBoxReset(FRMFileSearchComboKeyWord);
                        WAIniDeleteKey("SearchKeys", "", MainIniFile);
                        WAControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) FRMFileSearchComboKeyWord);
						break;
				}
				return(0);
            }
            else if((HWND) lParam == FRMFileSearchTBExt)
            {
                switch(wParam)
                {
					case FRMFileSearchBUTTONEXT:
						GetCursorPos(&PopMenuCoords);
						WAToolBarDisplayPopupMenu(FRMFileSearchTBExt, FRMFileSearchBUTTONEXT, hExtensionsMenu, hwndDlg);
						break;
					case FRMFileSearchBUTTONCLEAREXT:
                        WAComboBoxReset(FRMFileSearchComboExt);
                        WAIniDeleteKey("SearchExts", "", MainIniFile);
                        WAControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) FRMFileSearchComboExt);
						break;
				}
				return(0);
            }
            else if((HWND) lParam == FRMFileSearchTBSel)
            {
                switch(wParam)
                {
					case FRMFileSearchBUTTONOPEN:
						SelectDirToSearch();
						break;
					case FRMFileSearchBUTTONCLEAROPEN:
                        WAComboBoxReset(FRMFileSearchComboDir);
                        WAIniDeleteKey("SearchDirs", "", MainIniFile);
						WAIniDeleteKey("Layout", "SearchDir", MainIniFile);
                        WAControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) FRMFileSearchComboDir);
						break;
				}
				return(0);
            }
            else if((HWND) lParam == FRMFileSearchChkVariables)
            {
                FRMFileSearchChkVariablesValue = CheckBoxGetState(FRMFileSearchChkVariables);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchChkCaseSens)
            {
                FRMFileSearchChkCaseSensValue = CheckBoxGetState(FRMFileSearchChkCaseSens);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchChkIncludes)
            {
                FRMFileSearchChkIncludesValue = CheckBoxGetState(FRMFileSearchChkIncludes);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchChkSubDirs)
            {
                FRMFileSearchChkSubDirsValue = CheckBoxGetState(FRMFileSearchChkSubDirs);
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchChkRaw)
            {
                FRMFileSearchChkRawValue = CheckBoxGetState(FRMFileSearchChkRaw);
                if(FRMFileSearchChkRawValue == 1)
                {
                    WAControlEnable(FRMFileSearchChkVariables, 0);
                }
                else
                {
                    WAControlEnable(FRMFileSearchChkVariables, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMFileSearchComboKeyWord)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case CBN_SELENDOK:
                    case CBN_EDITCHANGE:
                        if(WAControlGetTextLen(FRMFileSearchComboKeyWord) > 0)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboExt) > 0)
                            {
								if(WAControlGetTextLen(FRMFileSearchComboDir) > 0)
								{
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
                        }
                        WAControlEnable(FRMFileSearchOk, 0);
						break;
                    case CBN_SELCHANGE:
                        if(WAComboBoxGetIndex(FRMFileSearchComboKeyWord) != CB_ERR)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboExt) > 0)
                            {
                                if(WAControlGetTextLen(FRMFileSearchComboDir) > 0)
                                {
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
                        }
                        WAControlEnable(FRMFileSearchOk, 0);
						break;
				}
            }
            else if((HWND) lParam == FRMFileSearchComboExt)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case CBN_SELENDOK:
                    case CBN_EDITCHANGE:
                        if(WAControlGetTextLen(FRMFileSearchComboExt) > 0)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboKeyWord) > 0)
                            {
								if(WAControlGetTextLen(FRMFileSearchComboDir) > 0)
								{
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
						}
						WAControlEnable(FRMFileSearchOk, 0);
						break;
                    case CBN_SELCHANGE:
                        if(WAComboBoxGetIndex(FRMFileSearchComboExt) != CB_ERR)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboKeyWord) > 0)
                            {
								if(WAControlGetTextLen(FRMFileSearchComboDir) > 0)
								{
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
						}
                        WAControlEnable(FRMFileSearchOk, 0);
						break;
                }
            }
            else if((HWND) lParam == FRMFileSearchComboDir)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case CBN_SELENDOK:
                    case CBN_EDITCHANGE:
                        if(WAControlGetTextLen(FRMFileSearchComboDir) > 0)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboExt) > 0)
                            {
								if(WAControlGetTextLen(FRMFileSearchComboKeyWord) > 0)
								{
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
						}
						WAControlEnable(FRMFileSearchOk, 0);
						break;
                    case CBN_SELCHANGE:
                        if(WAComboBoxGetIndex(FRMFileSearchComboDir) != CB_ERR)
                        {
                            if(WAControlGetTextLen(FRMFileSearchComboExt) > 0)
                            {
								if(WAControlGetTextLen(FRMFileSearchComboKeyWord) > 0)
								{
									WAControlEnable(FRMFileSearchOk, 1);
									break;
								}
							}
						}
                        WAControlEnable(FRMFileSearchOk, 0);
						break;
                }
            }
            else
            {
				if(wParam >= EXT_IDBASE)
				{
					if((long) (wParam - EXT_IDBASE) < ExtensionsNumber)
					{
						WAControlSetText(FRMFileSearchComboExt, SearchExtensions.Get(wParam - EXT_IDBASE)->Content);
					}
				}
			}
			break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &FileSearchPs);
            WAGDIDrawHorzSep(hwndDlg, 3, 126, 260);
            EndPaint(hwndDlg, &FileSearchPs);
			break;
        case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(WAControlGetNotifiedID(lParam))
                    {
                        case FRMFileSearchBUTTONEXT:
                            WAToolBarDisplayToolTip("Select extensions from global list", lParam);
                            return(0);
                        case FRMFileSearchBUTTONOPEN:
                            WAToolBarDisplayToolTip("Select search directory", lParam);
                            return(0);
                        case FRMFileSearchBUTTONCLEARKEY:
                            WAToolBarDisplayToolTip("Clear list", lParam);
                            return(0);
                        case FRMFileSearchBUTTONCLEAREXT:
                            WAToolBarDisplayToolTip("Clear list", lParam);
                            return(0);
                        case FRMFileSearchBUTTONCLEAROPEN:
                            WAToolBarDisplayToolTip("Clear list", lParam);
                            return(0);
                    }
            }
			break;
        case WM_CLOSE:
            if(hExtensionsMenu != 0) DestroyMenu(hExtensionsMenu);
			FreezeTimer = 0;
			EndDialog(hwndDlg, 0);
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Check a dir for subdirs
void CheckSubDirs(CStr DirToCheck)
{
    CStr RetDir;
    CStr BufString;
    
	RetDir = WAFileDir(&((CStr) DirToCheck + (CStr) "*.*"), FILE_ATTRIBUTE_DIRECTORY);
    while(RetDir.Len() != 0)
    {
		// Pass over parent directories
        if(RetDir == ".") goto NoValidDir;
        else if(RetDir == "..") goto NoValidDir;
		else
		{
            // Check if it is a directory
            BufString = DirToCheck + (CStr) RetDir;
			if((GetFileAttributes(BufString.Get_String()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
                // Store found directory
                BufString = DirToCheck + (CStr) RetDir + (CStr) "\\";
				Directories.Add(BufString.Get_String());
            }
        }
NoValidDir:
        RetDir = WAFileDir();
    }
}

// -----------------------------------------------------------------------
// Search in files procedure
void RunfileSearch(HWND hWnd)
{
    CStr IncludeFileName;
    HANDLE hFindIncFile = 0;
    CStr FirstFile;
    CStr SPat;
    CStr RetFile;
    long TextToSearch = 0;
    long TextToSearchLen = 0;
    long SaveCurSearhcPos = 0;
    CStr CurrentFileSave;
    CStr RetrievedFile;
    long InCurFile = 0;
    long InCurFileLine = 0;
    CStr PrimDir;
    CStr RetfileToAdd;
    long MaxCheckIncludes = 0;
    long InCurText = 0;
    long InCurTextTab = 0;
    long InCurTextLine = 0;
    long CurrentTextLines = 0;
    int x = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int m = 0;
    int n = 0;
    CStr StrJoinInclude;
    long StrPosit = 0;
    long StrPositQuote = 0;
    long ReCheckLn = 0;
    long MaxCheck = 0;
    long Nested = 0;
    long FInclude = 0;
    CStr CurChar;
    CStr CurLine;
    long *IncTab = 0;
    CStr IncludeLine;
    long PosInc = 0;
    CStr CurLine2;
    CStr CurLineInc;
    long PosComment = 0;
    CStr NormalLine;
    long CsSens = 0;
    long PosWord = 0;
    long NbFilesPr = 0;
    long NbLinesPr = 0;
    CStr Plur;
    CStr Plur2;
    CStr IncName;
    CStr IncNameFoot;
    long IncNameDouble = 0;
    CStr BufString;
	CStr TempWildCards;
	CStr TempSingleWildCard;
	char *Double_Entry1;
	char *Double_Entry2;

	LastSearchFiles = 0;
	LastSearchLines = 0;
	LastSearch = "";
	LastWild = "";

    if(FRMFileSearchInCalc == 1)
    {
        FRMFileSearchStopCalc = 1;
        return;
    }

    // Check for valid informations
    if(WAControlGetText(FRMFileSearchComboKeyWord).Len() == 0)
    {
        WAMiscMsgBox(FRMFileSearchhWnd, "Enter a keyword.", MB_ERROR, Requesters);
        SetFocus(FRMFileSearchComboKeyWord);
        return;
    }
    if(WAControlGetText(FRMFileSearchComboExt).Len() == 0)
    {
        WAMiscMsgBox(FRMFileSearchhWnd, "Enter a search pattern.", MB_ERROR, Requesters);
        SetFocus(FRMFileSearchComboExt);
        return;
    }
    // Ask for a directory if entry is empty or invalid
	if((WAControlGetText(FRMFileSearchComboDir).Len() == 0) || (WAFileIsDirectory(WAControlGetText(FRMFileSearchComboDir)) == 0))
	{
        SelectDirToSearch();
        if((WAControlGetText(FRMFileSearchComboDir).Len() == 0) || (WAFileIsDirectory(WAControlGetText(FRMFileSearchComboDir)) == 0)) return;
    }

    // Store datas for results display
    LastSearch = WAControlGetText(FRMFileSearchComboKeyWord);
    LastWild = WAControlGetText(FRMFileSearchComboExt);
	
	WAComboBoxSaveInIniFile(FRMFileSearchComboKeyWord, LastSearch, "SearchKeys", MainIniFile);
	WAComboBoxSaveInIniFile(FRMFileSearchComboExt, LastWild, "SearchExts", MainIniFile);
	WAComboBoxSaveInIniFile(FRMFileSearchComboDir, WAControlGetText(FRMFileSearchComboDir), "SearchDirs", MainIniFile);
	WAIniWriteKey("Layout", "SearchDir", WAControlGetText(FRMFileSearchComboDir), MainIniFile);

    // Store keyword for results display
    switch(FRMFileSearchChkVariablesValue)
    {
        // Procedures
        case BST_UNCHECKED:
            ProcType = "procedure";
			break;
		// Variables
        case BST_CHECKED:
            ProcType = "variable";
			break;
    }

    FRMFileSearchInCalc = 1;
    FRMFileSearchStopCalc = 0;

    WACursorSetWait();
    WAControlSetText(hWnd, "Search in files (seeking " + (CStr) ProcType + (CStr) "...)");

    WAControlSetText(FRMFileSearchOk, "Stop");
    WAControlEnable(FRMFileSearchCancel, 0);

    if(hFRMResults != 0)
    {
		WAListViewClear(FrmResultsListView);
		WAMiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
		WAControlFreeze(FrmResultsListView,1);
	}

    // Take search word
    FRMFileSearchSearchWord = WAControlGetText(FRMFileSearchComboKeyWord).Trim();

	switch(FRMFileSearchChkCaseSensValue)
	{
        case BST_UNCHECKED:
            CsSens = Text_Compare;
			break;
		case BST_CHECKED:
            CsSens = Binary_Compare;
			break;
    }
    
    if(strcmp(WAControlGetText(FRMFileSearchComboDir).Right(1).Get_String(), "\\") != 0)
    {
        WAControlSetText(FRMFileSearchComboDir, WAControlGetText(FRMFileSearchComboDir) + (CStr) "\\");
    }

    // Reset global linezs
    ProcLines = 0;
    
    Includes.Erase();
    IncludesBol.Erase();
    Directories.Erase();

    // Get wildcards
	TempWildCards = WAControlGetText(FRMFileSearchComboExt) + (CStr) ";";
	WildCardArray = StringSplit(TempWildCards, ";");

    // Include Sub directories
    if(FRMFileSearchChkSubDirsValue == BST_CHECKED)
    {
        WAControlSetText(FRMFileSearchFileLabel, "Retrieving directory structure...");
        PrimDir = WAControlGetText(FRMFileSearchComboDir);
        CheckSubDirs(PrimDir);
        // Check if directories were found in primary target
        if(Directories.Amount() != 0)
        {
            i = 0;
            // Fill the dir array
            while(i < Directories.Amount())
            {
                PrimDir = Directories.Get(i)->Content;
                CheckSubDirs(PrimDir);
                i++;
            }
            // Now use the directory array to fill the found file array
            i = 0;
            j = 0;
            while(i < Directories.Amount())
            {
                // Search files matching wildcards in subdirs
				for(j = 0; j <= StringGetSplitUBound(WildCardArray); j++)
				{
					TempSingleWildCard = StringGetSplitElement(TempWildCards, WildCardArray, j);
					if(TempSingleWildCard.Len() != 0)
					{
						RetfileToAdd = WAFileDir(&((CStr) Directories.Get(i)->Content + (CStr) TempSingleWildCard), FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_COMPRESSED);
						while(RetfileToAdd.Len() != 0)
						{
							BufString = Directories.Get(i)->Content + (CStr) RetfileToAdd;
							Includes.Add(BufString.Get_String());
							IncludesBol.Add(0L);
							RetfileToAdd = WAFileDir();
						}
					}
				}
				i++;
            }
        }
    }

	// Perform all wildcards
	for(i = 0; i <= StringGetSplitUBound(WildCardArray); i++)
	{
		TempSingleWildCard = StringGetSplitElement(TempWildCards, WildCardArray, i);
		if(TempSingleWildCard.Len() != 0)
		{
			SPat = WAControlGetText(FRMFileSearchComboDir) + (CStr) TempSingleWildCard;
			// Return to primary dir and begin the checkup
			RetFile = WAFileDir(&SPat, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_COMPRESSED);
			if(RetFile.Len() != 0)
			{
				// Take first file (others are considered as includes)
				FirstFile = WAControlGetText(FRMFileSearchComboDir) + (CStr) RetFile;
				// Proceed with second file
				RetFile = WAFileDir();
				// Begin search for asm file in given directory (primary)
				while(RetFile.Len() != 0)
				{
					BufString = WAControlGetText(FRMFileSearchComboDir) + (CStr) RetFile;
					Includes.Add(BufString.Get_String());
					IncludesBol.Add(0L);
					RetFile = WAFileDir();
				}
			}
		}
	}

    // Perform include search
    if(FRMFileSearchChkIncludesValue == BST_CHECKED)
    {
        WAControlSetText(FRMFileSearchFileLabel, "Preprocessing files...");
        j = 0;
        MaxCheckIncludes = Includes.Amount();
        while(j < MaxCheckIncludes)
        {
            if(strlen(Includes.Get(j)->Content) != 0)
            {
                FRMFileSearchCommChar = GetCMLangCommentByFile(Includes.Get(j)->Content);
                if(FRMFileSearchCommChar.Len() == 0) goto NoMoreIncludes;
                WAControlSetText(FRMFileSearchFileLabel, Includes.Get(j)->Content);
                TextToSearch = (long) MLoadFile(Includes.Get(j)->Content, &TextToSearchLen);
                LoCase((char *) TextToSearch);
                // Remove single lines comments if not in raw mode
				if(FRMFileSearchChkRawValue == 0)
				{
					RemoveSingleLineComment((char *) TextToSearch, TextToSearchLen, FRMFileSearchCommChar.Get_String());
                }
				// Get include name type
                IncName = GetCMLangIncludeNameByFile(Includes.Get(j)->Content);
                IncNameFoot = GetCMLangIncludeNameFootByFile(Includes.Get(j)->Content);
                IncNameDouble = GetCMLangDoubleslashByFile(Includes.Get(j)->Content).Get_Long();
                if(IncName.Len() == 0) IncName = "include";
                // Search for "include" word (case insensitive)
                InCurText = SearchInText(TextToSearch, TextToSearchLen, IncName + (CStr) " ", Text_Compare, 1);
                InCurTextTab = SearchInText(TextToSearch, TextToSearchLen, IncName + (CStr) "\t", Text_Compare, 1);
                if(InCurTextTab != 0)
                {
                    if(InCurTextTab < InCurText) InCurText = InCurTextTab;
                    if(InCurText == 0) InCurText = InCurTextTab;
                }
                if(InCurText != 0)
                {
					// Remove comments blocks if not in raw mode
					if(FRMFileSearchChkRawValue == 0)
					{
						RemoveCommentBlocksFromFileByFile(TextToSearch, TextToSearchLen, Includes.Get(j)->Content);
					}
                    InCurTextLine = InCurText - 1,
                    CurrentTextLines = CountFileLines((char *) TextToSearch, TextToSearchLen);
FoundMoreNestedIncludes:
                    for(i = InCurTextLine; i <= CurrentTextLines; i++)
                    {
                        NormalLine = SearchGetTextLine(TextToSearch, TextToSearchLen, i);
                        CurLine = StringReplace(NormalLine, "\t", " ", 1, -1, Binary_Compare);
                        CurLineInc = StringReplace(CurLine.Trim(), " ", CurLine.Chr(2), 1, -1, Binary_Compare);
                        // Record includes references for later use
                        PosInc = CurLineInc.In_Str(1, IncName + (CStr) CurLineInc.Chr(2).Get_String(), Text_Compare);
                        IncTab = 0;
                        if(PosInc != 0)
                        {
                            PosComment = CurLineInc.In_Str(1, FRMFileSearchCommChar, Text_Compare);
                            if(PosComment != 0) if(PosComment < PosInc) goto FalseInclude;
                            IncludeLine = StringReplace(CurLineInc, FRMFileSearchCommChar, CurLineInc.Chr(2), 1, -1, Binary_Compare);
                            IncludeLine = IncludeLine.Mid(PosInc);
                            IncTab = StringSplit(IncludeLine, IncludeLine.Chr(2));
                            if(StringGetSplitUBound(IncTab) < 1) goto FalseInclude;
                            // Loop to check already stored includes
                            FInclude = 0;
                            for(n = 1; n <= StringGetSplitUBound(IncTab); n++)
                            {
                                StrJoinInclude = StringGetSplitElement(IncludeLine, IncTab, n).Trim();
                                if(StrJoinInclude.Len() != 0) break;
                            }
                            //StrJoinInclude = StringReplace(Upper_Case(StrJoinInclude), Chr(0), "", 1, -1, vbBinary_Compare)
                            if(IncNameFoot.Len() != 0) StrJoinInclude = StrJoinInclude.Mid(1, StrJoinInclude.Len() - IncNameFoot.Len());
                            // Remove double slashes
                            if(IncNameDouble != 1) StrJoinInclude = StringReplace(StrJoinInclude, "\\\\", "\\", 1, -1, Binary_Compare);
                            for(k = 0; k < Includes.Amount(); k++)
                            {
                                if(strlen(Includes.Get(k)->Content) != 0)
                                {
                                    if(strcmpi(Includes.Get(k)->Content, StrJoinInclude.Get_String()) == 0)
                                    {
                                        FInclude = 1;
                                        break;
                                    }
                                }
                            }
                            if(FInclude == 1) goto FalseInclude;
                            //IncludeFileName = StringReplace(
							IncludeFileName = StringReplace(StringGetSplitElement(IncludeLine, IncTab, n), "\"", "", 1, -1, Binary_Compare);
							//Chr(0), "", 1, -1, vbBinary_Compare);
                            if(IncNameFoot.Len() != 0) IncludeFileName = IncludeFileName.Mid(1, IncludeFileName.Len() - IncNameFoot.Len());
                            // Remove double slashes
                            if(IncNameDouble != 1) IncludeFileName = StringReplace(IncludeFileName, "\\\\", "\\", 1, -1, Binary_Compare);
                            hFindIncFile = FindFirstFile(IncludeFileName.Get_String(), &FindIncludeDat);
                            if(hFindIncFile == INVALID_HANDLE_VALUE)
                            {
                                IncludeFileName = WAFileGetDirectory(Includes.Get(j)->Content) + (CStr) IncludeFileName;
                                hFindIncFile = FindFirstFile(IncludeFileName.Get_String(), &FindIncludeDat);
                                if(hFindIncFile == INVALID_HANDLE_VALUE) goto FalseInclude;
                                FInclude = 0;
                                for(k = 0; k < Includes.Amount(); k++)
                                {
                                    if(strlen(Includes.Get(k)->Content) != 0)
                                    {
                                        if(strcmpi(Includes.Get(k)->Content, IncludeFileName.Get_String()) == 0)
                                        {
                                            FInclude = 1;
                                            break;
                                        }
                                    }
                                }
                                if(FInclude == 1)
                                {
                                    FindClose(hFindIncFile);
                                    goto FalseInclude;
                                }
                            }
                            FindClose(hFindIncFile);
                            IncludeFileName = StringReplace(IncludeFileName, "\t", "", 1, -1, Binary_Compare);
                            IncludeFileName = IncludeFileName.Trim();
                            Includes.Add(IncludeFileName.Get_String());
                            IncludesBol.Add(0L);
FalseInclude:               StringReleaseSplit(IncTab);
                        }
                        if(FRMFileSearchStopCalc == 1)
                        {
                            if(TextToSearch != 0) FreeMem(TextToSearch);
                            TextToSearch = 0;
                            goto StopAll;
                        }
                        // Search new include (always case unsensitive)
                        SaveCurSearhcPos = CurrentRetriPosition;
                        InCurText = SearchInText(TextToSearch, TextToSearchLen, IncName + (CStr) " ", Text_Compare, CurrentRetriPosition + 1);
                        if(InCurText == 0)
                        {
                            CurrentRetriPosition = SaveCurSearhcPos;
                            InCurText = SearchInText(TextToSearch, TextToSearchLen, IncName + (CStr) "\t", Text_Compare, CurrentRetriPosition + 1);
                        }
                        if(InCurText == 0)
                        {
                            if(TextToSearch != 0) FreeMem(TextToSearch);
                            TextToSearch = 0;
                            goto NoMoreIncludes;
                        }
                        InCurTextLine = InCurText - 1;
                        goto FoundMoreNestedIncludes;
                    }
                }
NoMoreIncludes:;
            }
            j++;
        }
        if(TextToSearch != 0) FreeMem(TextToSearch);
        TextToSearch = 0;
    }

    // Remove double entries
    for(i = 0; i < Includes.Amount(); i++)
    {
		Double_Entry1 = Includes.Get(i)->Content;
        if(strlen(Double_Entry1) != 0)
        {
            for(j = 0; j < Includes.Amount(); j++)
            {
                if(i != j)
                {
					Double_Entry2 = Includes.Get(j)->Content;
                    if(strlen(Double_Entry2) != 0) {
                        if(strcmpi(Double_Entry1, Double_Entry2) == 0)
                        {
							// TODO: checkup
						    Includes.Set(i, "");
                            IncludesBol.Set(i, 0L);
                        }
                    }
                }
            }
        }
    }

    if(hFRMResults == 0) hFRMResults = WACreateDialog(-1, -1, 600, 264, hMDIform.hWnd, 0, LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_FIND + ICON_BASE)), "Search results", &FrmResultsInitProc, &FrmResultsWinHook, 0, WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX, SW_HIDE);
    
	ReCheckLn = 0;
    if(FirstFile.Len() != 0)
    {
        // ------
        SearchOpenFile(FirstFile);
        NbLinesPr = NbLinesPr + CurrentFileLines;
		NbFilesPr++;
        WAControlSetText(FRMFileSearchFileLabel, FirstFile);
        InCurFile = SearchInFile(FirstFile, FRMFileSearchSearchWord, CsSens, 1, FRMFileSearchChkRawValue);
        if(InCurFile != 0)
        {
            WAControlSetText(FRMFileSearchFileLabel, FirstFile);
            InCurFileLine = InCurFile - 1;
FoundMoreEntriesFirst:
            for(j = InCurFileLine; j <= CurrentFileLines - 1; j++)
            {
                if(ReCheckLn == 1)
                {
                    NormalLine = NormalLine.Left(NormalLine.Len() - 1) + (CStr) SearchGetLines(j).Get_String();
                }
                else
                {
                    NormalLine = SearchGetLines(j);
                }
                CurrentFileSave = FirstFile;
                ReCheckLn = 0;
                if(CsSens == Binary_Compare) if(NormalLine.In_Str(1, FRMFileSearchSearchWord, Binary_Compare) == 0) goto SuiteCheck;
                goto CheckSyntax;
SuiteCheck:     if(FRMFileSearchStopCalc == 1)
                {
                    // Free pending file
                    SearchCloseFile();
                    goto StopAll;
                }
                InCurFile = ContinueSearchInFile(FirstFile, FRMFileSearchSearchWord, CsSens, CurrentRetriPosition + 1);
                if(InCurFile == 0) goto NoMoreEntriesFirst;
                // Yes take the line
                InCurFileLine = InCurFile - 1;
                goto FoundMoreEntriesFirst;
            }
        }
NoMoreEntriesFirst:
        SearchCloseFile();
    }

    // Add includes in search
    Nested = 1;
    MaxCheck = Includes.Amount();
    m = 0;
    while(m < MaxCheck)
    {
        // Empty (in case)
        if(strlen(Includes.Get(m)->Content) != 0)
        {
            // Check if the file exists
            if(WAFileExist(Includes.Get(m)->Content) != 0)
            {
                // Already checked file
                if(IncludesBol.Get(m)->Content == 0)
                {
                    FRMFileSearchCommChar = GetCMLangCommentByFile(Includes.Get(m)->Content);
                    if(FRMFileSearchCommChar.Len() == 0) goto CantOpen;
                    // Open the file for search
                    SearchOpenFile(Includes.Get(m)->Content);
					NbLinesPr = NbLinesPr + CurrentFileLines;
                    NbFilesPr++;
                    // Search into a form
                    ReCheckLn = 0;
                    CurrentRetriPosition = 1;
                    WAControlSetText(FRMFileSearchFileLabel, Includes.Get(m)->Content);
                    // Check if the keyword is in the file
                    // Return line
                    InCurFile = SearchInFile(Includes.Get(m)->Content, FRMFileSearchSearchWord, CsSens, CurrentRetriPosition, FRMFileSearchChkRawValue);
                    if(InCurFile != 0)
                    {
                        InCurFileLine = InCurFile - 1;
FoundMoreEntries:       for(j = InCurFileLine; j <= CurrentFileLines; j++)
                        {
                            // Take the line
                            if(ReCheckLn == 1)
                            {
                                NormalLine = NormalLine.Left(NormalLine.Len() - 1) + (CStr) SearchGetLines(j).Get_String();
                            }
                            else
                            {
                                NormalLine = SearchGetLines(j);
                            }
                            CurrentFileSave = Includes.Get(m)->Content;
                            ReCheckLn = 0;
                            if(CsSens == Binary_Compare) if(NormalLine.In_Str(1, FRMFileSearchSearchWord, Binary_Compare) == 0) goto SuiteCheckInc;
                            goto CheckSyntax;
SuiteCheckInc:              if(FRMFileSearchStopCalc == 1)
                            {
                                // unload current file
                                // Free pending file
                                SearchCloseFile();
                                goto StopAll;
                            }
                            // Check rest of file from position+1
                            InCurFile = ContinueSearchInFile(Includes.Get(m)->Content, FRMFileSearchSearchWord, CsSens, CurrentRetriPosition + 1);
                            if(InCurFile == 0) goto NoMoreEntries;
                            // Yes take the line
                            InCurFileLine = InCurFile - 1;
                            goto FoundMoreEntries;
                        }
                    }
NoMoreEntries:      SearchCloseFile();
CantOpen:           IncludesBol.Set(m, 1L);
                }
            }
        }
        // Refresh maximum value
        MaxCheck = Includes.Amount();
        m++;
    }
StopAll:
    WACursorSetNormal();
    SearchCloseFile();
	if(FrmResultsListView != 0) WAControlFreeze(FrmResultsListView,0);
    if(NbFilesPr == 1) Plur = "";
    else Plur = "s";
    if(NbLinesPr == 1) Plur2 = "";
    else Plur2 = "s";
	LastSearchFiles = NbFilesPr;
	LastSearchLines = NbLinesPr;
	BufString = NbFilesPr + (CStr) " file" + (CStr) Plur + (CStr) " and " + (CStr) NbLinesPr + (CStr) " line" + (CStr) Plur2 + (CStr) ")";
    if(ProcLines == 1)
    {
        WAControlSetText(hFRMResults, "Search results for " + (CStr) ProcType + (CStr) ": '" + (CStr) LastSearch + (CStr) "' (" + (CStr) WAListViewItemCount(FrmResultsListView) + (CStr) " occurrence in " + (CStr) BufString);
    }
    else
    {
        WAControlSetText(hFRMResults, "Search results for " + (CStr) ProcType + (CStr) ": '" + (CStr) LastSearch + (CStr) "' (" + (CStr) WAListViewItemCount(FrmResultsListView) + (CStr) " occurrences in " + (CStr) BufString);
    }
    WAControlSetText(FRMFileSearchOk, "Ok");
    WAControlEnable(FRMFileSearchCancel, 1);
    WAControlSetText(hWnd, "Search in files");
    FRMFileSearchInCalc = 0;
    FRMFileSearchStopCalc = 0;
    OkSearch = 1;
    WAControlClose(hWnd);
	// Clear the datas
	StringReleaseSplit(WildCardArray);
    Includes.Erase();
    IncludesBol.Erase();
    Directories.Erase();
    return;

// -----------------------------------------------------------------------
// Syntax check
CheckSyntax:
    // Get the line and replace tabs by spaces
    CurLine = StringReplace(NormalLine, "\t", " ", 1, -1, Binary_Compare);
    // Remove spaces
    CurLineInc = StringReplace(CurLine.Trim(), " ", CurLine.Chr(2), 1, -1, Binary_Compare);
    CurLine = StringReplace(CurLine, " ", "", 1, -1, Binary_Compare);
    // Don't search empty lines
    if(CurLine.Len() != 0)
    {
        // Check for line terminator
        // Don't check if Comment antislash
        if(strcmp(CurLineInc.Right(1).Get_String(), "\\") == 0)
        {
            // Search for a comment before if yes return false
            PosComment = CurLineInc.In_Str(1, FRMFileSearchCommChar, Text_Compare);
            if(PosComment != 0) if(PosComment < CurLineInc.Len()) goto OkBrakeLine;
            ReCheckLn = 1;
            goto FastReload;
        }
OkBrakeLine:
        switch(FRMFileSearchChkVariablesValue)
        {
            // Procedures search
            case BST_UNCHECKED:
                // Search the word
ForceToRaw:     PosWord = CurLine.In_Str(1, FRMFileSearchSearchWord, CsSens);
                if(PosWord != 0)
                {
                    if(FRMFileSearchChkRawValue == 1)
                    {
                        AddFile(CurrentFileSave, j, "RAW", NormalLine);
                        goto OkLineProc;
                    }
                    for(x = 0; x < TabCheckFunc.Amount(); x++)
                    {
                        if(strlen(TabCheckFunc.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckFunc.Get(x)->Content, FRMFileSearchSearchWord,
                                      TabCheckFuncPos.Get(x)->Content) == 1)
                        {
                            AddFile(CurrentFileSave, j, TabCheckFuncComment.Get(x)->Content, NormalLine);
                            goto OkLineProc;
                        }
                    }
                    // Not a variable
                    goto OkLineProc2;
OkLineProc:;    }
            // Variables search
				break;
            case BST_CHECKED:
                if(FRMFileSearchChkRawValue == 1) goto ForceToRaw;
                PosWord = CurLine.In_Str(1, FRMFileSearchSearchWord, CsSens);
                if(PosWord != 0)
                {
                    for(x = 0; x < TabCheckVars.Amount(); x++)
                    {
                        if(strlen(TabCheckVars.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckVars.Get(x)->Content, FRMFileSearchSearchWord,
                                      TabCheckVarsPos.Get(x)->Content) == 1)
                        {
                            AddFile(CurrentFileSave, j, TabCheckVarsComment.Get(x)->Content, NormalLine);
                            goto OkLineVar;
                        }
                    }
                    // Not a variable
                    goto OkLineProc2;
OkLineVar:
                    // Now THE TRICK to looks like being intelligent:
                    // check if the variable is used somewhere as a function if yes we can safely
                    // remove it from the list

                    // Search strings
                    // Search the word in the line
                    StrPosit = CurLine.In_Str(1, FRMFileSearchSearchWord, CsSens);
                    if(StrPosit != 0)
                    {
                        StrPositQuote = CurLine.In_Str(1, "\"");
                        if(StrPositQuote != 0)
                        {
                            // Found a quote
                            if(StrPositQuote < StrPosit)
                            {
                                StrPositQuote = CurLine.In_Str(StrPositQuote + 1, "\"");
                                if(StrPositQuote == 0) goto UnmatchedString;
                                // Included in quotes ?
                                if(StrPositQuote > StrPosit)
                                {
UnmatchedString:                    RemFile(CurrentFileSave);
                                    goto OkLineProc2;
                                }
                            }
                        }
                    }

                    // Search strings
                    // Search the word in the line
                    StrPosit = CurLine.In_Str(1, FRMFileSearchSearchWord, CsSens);
                    if(StrPosit != 0)
                    {
                        StrPositQuote = CurLine.In_Str(1, "'");
                        if(StrPositQuote != 0)
                        {
                            // Found a quote
                            if(StrPositQuote < StrPosit)
                            {
                                StrPositQuote = CurLine.In_Str(StrPositQuote + 1, "'");
                                if(StrPositQuote == 0) goto UnmatchedString2;
                                // Included in quotes ?
                                if(StrPositQuote > StrPosit)
                                {
UnmatchedString2:                   RemFile(CurrentFileSave);
                                    goto OkLineProc2;
                                }
                            }
                        }
                    }

                    // Search for structure
                    if(ProcSearch(CurLine, "\"", FRMFileSearchSearchWord, POS_BEFORE) == 1)
                    {
                        RemFile(CurrentFileSave);
                        goto OkLineProc2;
                    }
                    if(ProcSearch(CurLine, "\"", FRMFileSearchSearchWord, POS_AFTER) == 1)
                    {
                        RemFile(CurrentFileSave);
                        goto OkLineProc2;
                    }
                    for(x = 0; x < TabCheckFunc.Amount(); x++)
                    {
                        if(strlen(TabCheckFunc.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckFunc.Get(x)->Content, FRMFileSearchSearchWord,
                                      TabCheckFuncPos.Get(x)->Content) == 1)
                        {
                            RemFile(CurrentFileSave);
                            goto OkLineProc2;
                        }
                    }
OkLineProc2:;   }
        }
    }
FastReload:
    if(Nested == 0)
    {
        // Check first file
        goto SuiteCheck;
    }
    else
    {
        // Check all includes
        goto SuiteCheckInc;
    }
    return;
}

// -----------------------------------------------------------------------
// Dir selection
void SelectDirToSearch(void)
{
    FRMFileSearchSearchDir = WAComDlgBrowseForFolder(FRMFileSearchhWnd, "Select search directory...");
    if(FRMFileSearchSearchDir.Len() != 0)
    {
        WAControlSetText(FRMFileSearchComboDir, FRMFileSearchSearchDir);
        WAIniWriteKey("Layout", "SearchDir", FRMFileSearchSearchDir, MainIniFile);
	}
}
