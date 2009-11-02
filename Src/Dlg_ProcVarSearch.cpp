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
// Dlg_ProcVarSearch.cpp: Procedures/Variables search form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Languages.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_ProcVarSearch.h"
#include "Dlg_Results.h"

// -----------------------------------------------------------------------
// Private constants
#define FRMProcVarSearchBUTTONKEY 1

// -----------------------------------------------------------------------
// Variables
HWND FRMProcVarSearchComboKeyWord;
HWND FRMProcVarSearchOk;
HWND FRMProcVarSearchCancel;
HWND FRMProcVarSearchFrame;
HWND FRMProcVarSearchChkCaseSens;
HWND FRMProcVarSearchChkIncludes;
HWND FRMProcVarSearchChkRaw;
HWND FRMProcVarSearchRadioProc;
HWND FRMProcVarSearchRadioVar;
HWND FRMProcVarSearchFileLabel;
HWND FRMProcVarSearchhwnd;
HWND FRMProcVarSearchTBKey;
long FRMProcSearchInCalc;
long FRMProcSearchStopCalc;
CStr FRMProcSearchCommChar;
CStr FRMProcSearchSearchWord;

long FRMProcVarSearchRadioVPValue;
long FRMProcVarSearchChkIncludesValue;
long FRMProcVarSearchChkCaseSensValue;
long FRMProcVarSearchChkRawValue;
WIN32_FIND_DATA ProcFindIncludeDat;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMProcVarSearchProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
	PAINTSTRUCT ProcVarSearchPs;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMProcVarSearchhwnd = hwndDlg;
            OkSearch = 0;
            WAControlSetText(hwndDlg, "Proc/Var/Const search");
            FRMProcVarSearchFrame = WACreateFrame(2, -4, 262, 122, hwndDlg, "", 0, &FRMProcVarSearchFrameProc, 0);
            WACreateLabel(5, 11, 180, 14, FRMProcVarSearchFrame, "Keyword :", 0, 0, 0, 0);
            FRMProcVarSearchOk = WACreateButton(268, 2, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMProcVarSearchCancel = WACreateButton(268, 26, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMProcVarSearchComboKeyWord = WACreateComboBox(7, 22, 225, 100, hwndDlg, "", 3, 0, WS_TABSTOP | CBS_DROPDOWN | WS_GROUP);
            FRMProcVarSearchTBKey = WACreateToolBar(234, 21, 25, 23, hwndDlg, GlobalImageList1, 5, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMProcVarSearchTBKey, "", FRMProcVarSearchBUTTONKEY, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMProcVarSearchChkIncludes = WACreateCheckBox(14, 55, 130, 19, hwndDlg, "Search in includes", 4, 0, WS_TABSTOP, 0);
            FRMProcVarSearchChkCaseSens = WACreateCheckBox(14, 73, 130, 19, hwndDlg, "Case sensitive", 5, 0, WS_TABSTOP, 0);
            FRMProcVarSearchChkRaw = WACreateCheckBox(14, 91, 130, 19, hwndDlg, "Raw results", 6, 0, WS_TABSTOP, 0);
            FRMProcVarSearchRadioProc = WACreateRadioButton(156, 55, 100, 19, hwndDlg, "Procedures", 7, 0, WS_TABSTOP | WS_GROUP);
            FRMProcVarSearchRadioVar = WACreateRadioButton(156, 73, 100, 19, hwndDlg, "Var/Const", 8, 0, WS_TABSTOP);
            FRMProcVarSearchFileLabel = WACreateLabel(2, 122, 345, 16, hwndDlg, "", 109, 0, 0, 0);
            WAComboBoxFillFromIniFile(FRMProcVarSearchComboKeyWord, "SearchKeys", MainIniFile);
            switch(FRMProcVarSearchRadioVPValue)
            {
                case 0:
                    RadioButtonSetState(FRMProcVarSearchRadioProc, BST_CHECKED);
					break;
				case 1:
                    RadioButtonSetState(FRMProcVarSearchRadioVar, BST_CHECKED);
					break;
            }
            if(NbForms != 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                WAControlSetText(FRMProcVarSearchComboKeyWord, GetCurrentWord(ChildStruct->hChildCodeMax));
            }
            CheckBoxSetState(FRMProcVarSearchChkIncludes, FRMProcVarSearchChkIncludesValue);
            CheckBoxSetState(FRMProcVarSearchChkCaseSens, FRMProcVarSearchChkCaseSensValue);
            CheckBoxSetState(FRMProcVarSearchChkRaw, FRMProcVarSearchChkRawValue);
            if(FRMProcVarSearchChkRawValue == 1)
            {
                WAControlEnable(FRMProcVarSearchRadioProc, 0);
                WAControlEnable(FRMProcVarSearchRadioVar, 0);
            }
            else
            {
                WAControlEnable(FRMProcVarSearchRadioProc, 1);
                WAControlEnable(FRMProcVarSearchRadioVar, 1);
            }
            FreezeTimer = 1;
            WAControlEnable(FRMProcVarSearchOk, 0);
            WAControlBringToTop(FRMProcVarSearchTBKey);
			WAControlBringToTop(FRMProcVarSearchComboKeyWord);
			WAControlSendMessage(hwndDlg, CBN_EDITCHANGE << 16, (LPARAM) FRMProcVarSearchComboKeyWord);
            SetFocus(FRMProcVarSearchComboKeyWord);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &ProcVarSearchPs);
            WAGDIDrawHorzSep(hwndDlg, 3, 50, 260);
            EndPaint(hwndDlg, &ProcVarSearchPs);
			break;
        case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
					switch(WAControlGetNotifiedID(lParam))
					{
						case FRMProcVarSearchBUTTONKEY:
                            WAToolBarDisplayToolTip("Clear list", lParam);
							return(0);
					}
					break;
			}
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMProcVarSearchOk)
            {
                RunProcSearch(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchCancel)
            {
                OkSearch = 0;
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchChkIncludes)
            {
                FRMProcVarSearchChkIncludesValue = CheckBoxGetState(FRMProcVarSearchChkIncludes);
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchChkCaseSens)
            {
                FRMProcVarSearchChkCaseSensValue = CheckBoxGetState(FRMProcVarSearchChkCaseSens);
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchChkRaw)
            {
                FRMProcVarSearchChkRawValue = CheckBoxGetState(FRMProcVarSearchChkRaw);
                if(FRMProcVarSearchChkRawValue == 1)
                {
                    WAControlEnable(FRMProcVarSearchRadioProc, 0);
                    WAControlEnable(FRMProcVarSearchRadioVar, 0);
                }
                else
                {
                    WAControlEnable(FRMProcVarSearchRadioProc, 1);
                    WAControlEnable(FRMProcVarSearchRadioVar, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchComboKeyWord)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case CBN_SELENDOK:
                    case CBN_EDITCHANGE:
                        if(WAControlGetTextLen(FRMProcVarSearchComboKeyWord) > 0)
                        {
                            WAControlEnable(FRMProcVarSearchOk, 1);
                        }
                        else
                        {
                            WAControlEnable(FRMProcVarSearchOk, 0);
                        }
						break;
                    case CBN_SELCHANGE:
                        if(WAComboBoxGetIndex(FRMProcVarSearchComboKeyWord) != CB_ERR)
                        {
                            WAControlEnable(FRMProcVarSearchOk, 1);
                        }
                        else
                        {
                            WAControlEnable(FRMProcVarSearchOk, 0);
                        }
						break;
                }
            }
            else if((HWND) lParam == FRMProcVarSearchTBKey)
            {
                switch(wParam)
                {
					case FRMProcVarSearchBUTTONKEY:
                        WAComboBoxReset(FRMProcVarSearchComboKeyWord);
                        WAIniDeleteKey("SearchKeys", "", MainIniFile);
                        WAControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) FRMProcVarSearchComboKeyWord);
						break;
				}
				return(0);
			}
			break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
	}
    return(0);
}

// -----------------------------------------------------------------------
// Search for a procedure or a variable in opened files or project files
void RunProcSearch(HWND hwnd)
{
    int i = 0;
    int j = 0;
    int n = 0;
    int k = 0;
    int m = 0;
    int x = 0;
    long Nested = 0;
    long PermanentFileLen = 0;
    CStr IncludeFileName;
    HANDLE hFindIncFile = 0;
    long StrPosit = 0;
    long StrPositQuote = 0;
    long MaxCheckIncludes = 0;
    long TextToSearch = 0;
    long TextToSearchLen = 0;
    long InCurText = 0;
    long InCurTextTab = 0;
    long SaveCurSearhcPos = 0;
    long InCurTextLine = 0;
    long CsSens = 0;
    CStr NormalLine;
    CStr CurLine;
    CStr CurLineInc;
    CStr IncludeLine;
    long PosInc = 0;
    long PosWord = 0;
    CStr CurrentFileSave;
    long PosComment = 0;
    long *IncTab = 0;
    long FInclude = 0;
    CStr StrJoinInclude;
    long CurrentTextLines = 0;
    long NbFilesPr = 0;
    long NbLinesPr = 0;
    CStr Plur;
    CStr Plur2;
    long ReCheckLn = 0;
    long MaxCheck = 0;
    long InCurFile = 0;
    long InCurFileLine = 0;
    CStr CurLine2;
    CStr IncName;
    CStr IncNameFoot;
    long IncNameDouble = 0;
    CStr BufString;

	LastSearchFiles = 0;
	LastSearchLines = 0;
	LastSearch = "";
	LastWild = "";

    if(FRMProcSearchInCalc == 1)
    {
        FRMProcSearchStopCalc = 1;
        return;
    }
    switch(FRMProcVarSearchChkCaseSensValue)
    {
        case BST_UNCHECKED:
            CsSens = Text_Compare;
			break;
		case BST_CHECKED:
            CsSens = Binary_Compare;
			break;
    }
    
    // Check for valid informations
    if(WAControlGetText(FRMProcVarSearchComboKeyWord).Len() == 0)
    {
        WAMiscMsgBox(FRMProcVarSearchhwnd, "Enter a keyword.", MB_ERROR, Requesters);
        SetFocus(FRMProcVarSearchComboKeyWord);
        return;
    }
    
    // Store keyword for results display
    LastSearch = WAControlGetText(FRMProcVarSearchComboKeyWord);

	WAComboBoxSaveInIniFile(FRMProcVarSearchComboKeyWord, LastSearch, "SearchKeys", MainIniFile);

    switch(FRMProcVarSearchRadioVPValue)
    {
        // Procedures
        case 0:
            ProcType = "procedure";
			break;
		// Variables
        case 1:
            ProcType = "variable";
			break;
    }
    
    // Reset global linezs
    ProcLines = 0;
    FRMProcSearchStopCalc = 0;

    // Fill default comment char if found
    if(NbForms != 0) FRMProcSearchCommChar = GetCMLangComment(CurrentForm);
    
    // Take search word
    FRMProcSearchSearchWord = WAControlGetText(FRMProcVarSearchComboKeyWord).Trim();
    WACursorSetWait();
    WAControlSetText(hwnd, "Proc/Var/Const search (seeking " + (CStr) ProcType + (CStr) "...)");
    WAControlSetText(FRMProcVarSearchOk, "Stop");
    WAControlEnable(FRMProcVarSearchCancel, 0);
    if(hFRMResults != 0) WAListViewClear(FrmResultsListView);
    GetAllFiles();
    
    // Perform include search
    if(FRMProcVarSearchChkIncludesValue == BST_CHECKED)
    {
        WAControlSetText(FRMProcVarSearchFileLabel, "Preprocessing files...");
        j = 0;
        MaxCheckIncludes = FilesList.Amount();
        while(j < MaxCheckIncludes)
        {
            if(strlen(FilesList.Get(j)->Content) != 0)
            {
                FRMProcSearchCommChar = GetCMLangCommentByFile(FilesList.Get(j)->Content);
                // Can't determine language
                if(FRMProcSearchCommChar.Len() == 0) goto NoMoreIncludes;
                // File not loaded ?
                if(FilesListhWnd.Get(j)->Content == 0)
                {
                    TextToSearch = (long) MLoadFile(FilesList.Get(j)->Content, &TextToSearchLen);
                    IncName = GetCMLangIncludeNameByFile(FilesList.Get(j)->Content);
                    IncNameFoot = GetCMLangIncludeNameFootByFile(FilesList.Get(j)->Content);
                    IncNameDouble = GetCMLangDoubleslashByFile(FilesList.Get(j)->Content).Get_Long();
                }
                else
                {
                    PermanentFileLen = CM_GetTextLengthAll(FilesListhWnd.Get(j)->Content, 1);
                    // Don't proceed empty permanent files
                    if(PermanentFileLen == 0) goto NoMoreIncludes;
                    TextToSearchLen = PermanentFileLen;
                    TextToSearch = AllocMem(TextToSearchLen);
                    CM_GetTextAllLng(FilesListhWnd.Get(j)->Content, TextToSearch);
                    IncName = GetCMLangIncludeNameByFile(FilesList.Get(j)->Content);
                    IncNameFoot = GetCMLangIncludeNameFootByFile(FilesList.Get(j)->Content);
                    IncNameDouble = GetCMLangDoubleslashByFile(FilesList.Get(j)->Content).Get_Long();
                }
                LoCase((char *) TextToSearch);

                // Remove single lines comments if not in raw mode
                if(FRMProcVarSearchChkRawValue == 0)
                {
					RemoveSingleLineComment((char *) TextToSearch, TextToSearchLen, FRMProcSearchCommChar.Get_String());
                }
				// Get include name type
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
					// Remove single lines comments if not in raw mode
					if(FRMProcVarSearchChkRawValue == 0)
					{
						RemoveCommentBlocksFromFileByFile(TextToSearch, TextToSearchLen, FilesList.Get(j)->Content);
                    }
					InCurTextLine = InCurText - 1;
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
                            PosComment = CurLineInc.In_Str(1, FRMProcSearchCommChar, Text_Compare);
                            if(PosComment != 0) if(PosComment < PosInc) goto FalseInclude;
                            IncludeLine = StringReplace(CurLineInc, FRMProcSearchCommChar, CurLineInc.Chr(2), 1, -1, Binary_Compare);
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
                            //StrJoinInclude = StringReplace(StrJoinInclude.Upper_Case(), Chr(0), "", 1, -1, vbBinary_Compare)
                            if(IncNameFoot.Len() != 0) StrJoinInclude = StrJoinInclude.Mid(1, StrJoinInclude.Len() - IncNameFoot.Len());
                            // Remove double slashes
                            if(IncNameDouble != 1) StrJoinInclude = StringReplace(StrJoinInclude, "\\\\", "\\", 1, -1, Binary_Compare);
                            for(k = 0; k < FilesList.Amount(); k++)
                            {
                                if(strlen(FilesList.Get(k)->Content) != 0)
                                {
                                    if(strcmpi(FilesList.Get(k)->Content, StrJoinInclude.Get_String()) == 0)
                                    {
                                        FInclude = 1;
                                        break;
                                    }
                                }
                            }
                            if(FInclude == 1) goto FalseInclude;
                            //IncludeFileName = StringReplace(
							IncludeFileName = StringReplace(StringGetSplitElement(IncludeLine, IncTab, n), "\"", "", 1, -1, Binary_Compare);
							//, Chr(0), "", 1, -1, vbBinary_Compare)
                            if(IncNameFoot.Len() != 0) IncludeFileName = IncludeFileName.Mid(1, IncludeFileName.Len() - IncNameFoot.Len());
                            // Remove double slashes
                            if(IncNameDouble != 1) IncludeFileName = StringReplace(IncludeFileName, "\\\\", "\\", 1, -1, Binary_Compare);
                            hFindIncFile = FindFirstFile(IncludeFileName.Get_String(), &ProcFindIncludeDat);
                            if(hFindIncFile == INVALID_HANDLE_VALUE)
                            {
                                IncludeFileName = WAFileGetDirectory(FilesList.Get(j)->Content) + (CStr) IncludeFileName;
                                hFindIncFile = FindFirstFile(IncludeFileName.Get_String(), &ProcFindIncludeDat);
                                if(hFindIncFile == INVALID_HANDLE_VALUE) goto FalseInclude;
                                FInclude = 0;
                                for(k = 0; k < FilesList.Amount(); k++)
                                {
                                    if(strlen(FilesList.Get(k)->Content) != 0)
                                    {
                                        if(strcmpi(FilesList.Get(k)->Content, IncludeFileName.Get_String()) == 0)
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
                            FilesList.Add(IncludeFileName.Get_String());
                            FilesListhWnd.Add(0L);
FalseInclude:               StringReleaseSplit(IncTab);
                        }
                        if(FRMProcSearchStopCalc == 1)
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
    if(hFRMResults == 0) hFRMResults = WACreateDialog(-1, -1, 600, 264, hMDIform.hWnd, 0, LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_FIND + ICON_BASE)), "Search results", &FrmResultsInitProc, &FrmResultsWinHook, 0, WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX, SW_HIDE);
    ReCheckLn = 0;
    MaxCheck = FilesList.Amount();
    m = 0;
    while(m < MaxCheck)
    {
        // Empty (in case)
        if(strlen(FilesList.Get(m)->Content) != 0)
        {
            // Already checked ?
            if(FilesListhWnd.Get(m)->Content != (HWND) -1)
            {
                FRMProcSearchCommChar = GetCMLangCommentByFile(FilesList.Get(m)->Content);
                // No file loaded in form ?
                if(FRMProcSearchCommChar.Len() == 0) FRMProcSearchCommChar = GetCMLangComment(GetParent(FilesListhWnd.Get(m)->Content));
                // Can't determine language
                if(FRMProcSearchCommChar.Len() == 0) goto CantDetermineEntry;
                if(FilesListhWnd.Get(m)->Content != 0)
                {
                    // Load it from CodeMax
                    SearchOpenFileCM(FilesListhWnd.Get(m)->Content);
                    NbLinesPr = NbLinesPr + CurrentFileLines;
					NbFilesPr++;
                    ReCheckLn = 0;
                    CurrentRetriPosition = 1;
                    WAControlSetText(FRMProcVarSearchFileLabel, FilesList.Get(m)->Content);
                    InCurFile = SearchInFileCM(FilesListhWnd.Get(m)->Content, FRMProcSearchSearchWord, CsSens, CurrentRetriPosition, FRMProcSearchCommChar, FRMProcVarSearchChkRawValue);
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
                            CurrentFileSave = FilesList.Get(m)->Content;
                            ReCheckLn = 0;
                            if(CsSens == Binary_Compare)
                            {
                                if(NormalLine.In_Str(1, FRMProcSearchSearchWord, Binary_Compare) == 0) goto SuiteCheckInc;
                            }
                            Nested = 0;
                            goto CheckSyntax;
SuiteCheckInc:              if(FRMProcSearchStopCalc == 1)
                            {
                                // unload current file
                                // Free pending file
                                SearchCloseFile();
                                goto StopAll;
                            }
                            // Check rest of file from position+1
                            InCurFile = ContinueSearchInFileCM(FilesListhWnd.Get(m)->Content, FRMProcSearchSearchWord, CsSens, CurrentRetriPosition + 1, FRMProcSearchCommChar);
                            if(InCurFile == 0) goto NoMoreEntries;
                            // Yes take the line
                            InCurFileLine = InCurFile - 1;
                            goto FoundMoreEntries;
                        }
                    }
                    goto NoMoreEntries;
                }
                else
                {
                    // Load file from disk
                    if(WAFileExist(FilesList.Get(m)->Content) == 0) goto FToSearchNotFound;
                    SearchOpenFile(FilesList.Get(m)->Content);
                    NbLinesPr = NbLinesPr + CurrentFileLines;
                    NbFilesPr++;
                    ReCheckLn = 0;
                    CurrentRetriPosition = 1;
                    WAControlSetText(FRMProcVarSearchFileLabel, FilesList.Get(m)->Content);
                    InCurFile = SearchInFile(FilesList.Get(m)->Content, FRMProcSearchSearchWord, CsSens, CurrentRetriPosition, FRMProcVarSearchChkRawValue);
                    if(InCurFile != 0)
                    {
                        InCurFileLine = InCurFile - 1;
FoundMoreEntries2:      for(j = InCurFileLine; j <= CurrentFileLines; j++)
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
                            CurrentFileSave = FilesList.Get(m)->Content;
                            ReCheckLn = 0;
                            if(CsSens == Binary_Compare) if(NormalLine.In_Str(1, FRMProcSearchSearchWord, Binary_Compare) == 0) goto SuiteCheckInc2;
                            Nested = 1;
                            goto CheckSyntax;
SuiteCheckInc2:             if(FRMProcSearchStopCalc == 1)
                            {
                                // Unload current file
                                // Free pending file
                                SearchCloseFile();
                                goto StopAll;
                            }
                            // Check rest of file from position+1
                            InCurFile = ContinueSearchInFile(FilesList.Get(m)->Content, FRMProcSearchSearchWord, CsSens, CurrentRetriPosition + 1);
                            if(InCurFile == 0) goto NoMoreEntries;
                            // Yes take the line
                            InCurFileLine = InCurFile - 1;
                            goto FoundMoreEntries2;
                        }
                    }
NoMoreEntries:      SearchCloseFile();
                }
CantDetermineEntry:
                FilesListhWnd.Set(m, (HWND) -1L);
            }
FToSearchNotFound:;
        }
        // Refresh maximum value
        MaxCheck = FilesList.Amount();
        m++;
    }
StopAll:
    WACursorSetNormal();
    SearchCloseFile();
    if(NbFilesPr == 1) Plur = "";
    else Plur = "s";
    if(NbLinesPr == 1) Plur = "";
    else Plur = "s";
	LastSearchFiles = NbFilesPr;
	LastSearchLines = NbLinesPr;
	BufString = NbFilesPr + (CStr) " file" + (CStr) Plur + (CStr) " and " + (CStr) NbLinesPr + (CStr) " line" + (CStr) Plur2 + (CStr) ")";
    if(ProcLines == 1) WAControlSetText(hFRMResults, "Search results for " + (CStr) ProcType + (CStr) ": '" + (CStr) LastSearch + (CStr) "' (" + (CStr) WAListViewItemCount(FrmResultsListView) + (CStr) " possible occurrence in " + (CStr) BufString);
    else WAControlSetText(hFRMResults, "Search results for " + (CStr) ProcType + (CStr) ": '" + (CStr) LastSearch + (CStr) "' (" + (CStr) WAListViewItemCount(FrmResultsListView) + (CStr) " possible occurrences in " + (CStr) BufString);
    WAControlSetText(FRMProcVarSearchOk, "Ok");
    WAControlEnable(FRMProcVarSearchCancel, 1);
    WAControlSetText(FRMProcVarSearchFileLabel, "");
    WAControlSetText(hwnd, "Proc/Var/Const search");
    FRMProcSearchInCalc = 0;
    FRMProcSearchStopCalc = 0;
    OkSearch = 1;
    WAControlClose(hwnd);
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
        // Don't check if Comment anti-slash
        if(strcmp(CurLineInc.Right(1).Get_String(), "\\") == 0)
        {
            // Search for a comment before if yes return false
            PosComment = CurLineInc.In_Str(1, FRMProcSearchCommChar, Text_Compare);
            if(PosComment != 0) if(PosComment < CurLineInc.Len()) goto OkBrakeLine;
            ReCheckLn = 1;
            goto FastReload;
        }
OkBrakeLine:
        switch(FRMProcVarSearchRadioVPValue)
        {
            // Procedures search
            case BST_UNCHECKED:
                // Search the word
ForceToRaw:     PosWord = CurLine.In_Str(1, FRMProcSearchSearchWord, CsSens);
                if(PosWord != 0)
                {
                    if(FRMProcVarSearchChkRawValue == 1)
                    {
                        AddFile(CurrentFileSave, j, "RAW", NormalLine);
                        goto OkLineProc;
                    }
                    for(x = 0; x < TabCheckFunc.Amount(); x++)
                    {
                        if(strlen(TabCheckFunc.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckFunc.Get(x)->Content, FRMProcSearchSearchWord,
                                      TabCheckFuncPos.Get(x)->Content) == 1)
                        {
                            AddFile(CurrentFileSave, j, TabCheckFuncComment.Get(x)->Content, NormalLine);
                            goto OkLineProc;
                        }
                    }
                    // Not a variable
                    goto OkLineProc2;
OkLineProc:;    }
				break;
            // Variables search
			case BST_CHECKED:
                if(FRMProcVarSearchChkRawValue == 1) goto ForceToRaw;
                PosWord = CurLine.In_Str(1, FRMProcSearchSearchWord, CsSens);
                if(PosWord != 0)
                {
                    for(x = 0; x < TabCheckVars.Amount(); x++)
                    {
                        if(strlen(TabCheckVars.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckVars.Get(x)->Content,
                                      FRMProcSearchSearchWord, TabCheckVarsPos.Get(x)->Content) == 1)
                        {
                            AddFile(CurrentFileSave, j, TabCheckVarsComment.Get(x)->Content, NormalLine);
                            goto OkLineVar;
                        }
                    }
                    // Not a variable
                    goto OkLineProc2;
OkLineVar:
                    // Now THE TRICK to look like being intelligent:
                    // check if the variable is used somewhere as a function if yes we can safely
                    // remove it from the list

                    // Search strings
                    // Search the word in the line
                    StrPosit = CurLine.In_Str(1, FRMProcSearchSearchWord, CsSens);
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
                    StrPosit = CurLine.In_Str(1, FRMProcSearchSearchWord, CsSens);
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
                    if(ProcSearch(CurLine, "\"", FRMProcSearchSearchWord, POS_BEFORE) == 1)
                    {
                        RemFile(CurrentFileSave);
                        goto OkLineProc2;
                    }
                    if(ProcSearch(CurLine, "\"", FRMProcSearchSearchWord, POS_AFTER) == 1)
                    {
                        RemFile(CurrentFileSave);
                        goto OkLineProc2;
                    }
                    for(x = 0; x < TabCheckFunc.Amount(); x++)
                    {
                        if(strlen(TabCheckFunc.Get(x)->Content) == 0) break;
                        if(ProcSearch(CurLine, TabCheckFunc.Get(x)->Content,
                                      FRMProcSearchSearchWord, TabCheckFuncPos.Get(x)->Content) == 1)
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
        goto SuiteCheckInc;
    }
    else
    {
        // Check all includes
        goto SuiteCheckInc2;
    }
}

// -----------------------------------------------------------------------
// Frame hook proc
LRESULT CALLBACK FRMProcVarSearchFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMProcVarSearchRadioProc)
            {
                FRMProcVarSearchRadioVPValue = 0;
                return(0);
            }
            else if((HWND) lParam == FRMProcVarSearchRadioVar)
            {
                FRMProcVarSearchRadioVPValue = 1;
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}
