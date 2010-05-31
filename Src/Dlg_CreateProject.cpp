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
// Dlg_CreateProject.cpp: Project types selection form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ProjectsBuilder.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_CreateProject.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMCreateProjectSysTab;
HWND FRMCreateProjectListView;
HWND FRMCreateProjectTextBox;
HWND FRMCreateProjectCombo;
HWND FRMCreateProjectLabel;
HWND FRMCreateProjectCmdOk;
HWND FRMCreateProjectCmdCancel;
HWND FRMCreateProjectStartup;
HWND FRMCreateProjectRecent;
long *PrjDats;
CStr FRMCreateProjectRetVal;
CStr FRMCreateProjectSelRecent;
long FRMCreateProjectRecentTabNbr;
CList <char *> FRMCreateProjectTabs;
CList <int> FRMCreateProjectTabsNbr;

// -----------------------------------------------------------------------
// Initialize Form 
int CALLBACK FRMCreateProjectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    int i = 0;
    CStr ShowAtStartup;
    CStr GatherFiles;
    long SelectedItem = 0;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ProjectCreate = 0;
            ProjectGather = 0;
            WAControlSetText(hwndDlg, "Select a project");
            FRMCreateProjectSysTab = WACreateSysTab(2, 1, 485, 280, hwndDlg, 0, 0, GlobalImageList1, WS_TABSTOP | TCS_BUTTONS | TCS_FLATBUTTONS | TCS_HOTTRACK);
            WASysTabSetSeparators(FRMCreateProjectSysTab, 0);
            FRMCreateProjectTabs.Erase();
            FRMCreateProjectTabsNbr.Erase();
            for(i = 0; i <= 999; i++)
            {
                FRMCreateProjectRetVal = WAIniReadKey("Tabs", "Tab" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(FRMCreateProjectRetVal.Len() == 0) break;
				FRMCreateProjectTabs.Add(FRMCreateProjectRetVal.Get_String());
				FRMCreateProjectTabsNbr.Add(0L);
                WASysTabAddItem(FRMCreateProjectSysTab, FRMCreateProjectRetVal, i, ICON_NEWDIR);
                FRMCreateProjectFillProjectsTabs(i);
            }
            // Recent files
            FRMCreateProjectRecentTabNbr = i;
            WASysTabAddItem(FRMCreateProjectSysTab, "Recent", i, ICON_FILES);
            FRMCreateProjectCmdOk = WACreateButton(331, 326 - 26, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMCreateProjectCmdCancel = WACreateButton(410, 326 - 26, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMCreateProjectListView = WACreateListView(4, 26, 483, 237, hwndDlg, 3, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMCreateProjectListView, "Wizard", 50, 2);
            WAListViewAddCol(FRMCreateProjectListView, "Author", 210, 1);
            WAListViewAddCol(FRMCreateProjectListView, "Project type", 200, 0);
            FRMCreateProjectFilCListView(0);
            FRMCreateProjectTextBox = WACreateTextBox(77, 266, 302, 20, hwndDlg, "", 4, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            FRMCreateProjectCombo = WACreateComboBox(382, 266, 106, 150, hwndDlg, "", 5, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
			WAComboBoxAddItem(FRMCreateProjectCombo, "Gather files", -1);
			WAComboBoxAddItem(FRMCreateProjectCombo, "Don't gather files", -1);
            if(WAIniReadBoolKey("Layout", "GatherFiles", MainIniFile) == 1)
            {
				WAComboBoxSetIndex(FRMCreateProjectCombo, 1);
            }
            else
            {
				WAComboBoxSetIndex(FRMCreateProjectCombo, 0);
			}
			FRMCreateProjectLabel = WACreateLabel(4, 268, 70, 16, hwndDlg, "Project name :", 6, 0, 0, 0);
            FRMCreateProjectStartup = WACreateCheckBox(7, 304, 170, 15, hwndDlg, "Show at startup", 7, 0, WS_TABSTOP | WS_GROUP, 0);
            ShowAtStartup = WAIniReadKey("Layout", "ShowProjects", MainIniFile);
            if(ShowAtStartup.Len() != 0) if(strcmpi(ShowAtStartup.Get_String(), "1") == 0) CheckBoxSetState(FRMCreateProjectStartup, 1);
            FRMCreateProjectRecent = WACreateListView(4, 26, 483, 261, hwndDlg, 8, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMCreateProjectRecent, "Folder", 253, 1);
            WAListViewAddCol(FRMCreateProjectRecent, "File", 208, 0);
            if(FRMCreateProjectRecentTabNbr == 0)
            {
				// No project type found: display recent projects list
				FRMCreateProjectChangeCurrentTab(hwndDlg);
            }
            else
            {
				WAControlVisible(FRMCreateProjectRecent, 0);
				WAListViewSetItemSel(FRMCreateProjectListView, 0);
				WAControlSendMessage(hwndDlg, EN_CHANGE << 16, (long) FRMCreateProjectTextBox);
				SetFocus(FRMCreateProjectTextBox);
            }
			FreezeTimer = 1;
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            WAGDIDrawHorzSep(hwndDlg, 0, WAControlClientHeight(hwndDlg) - 33, 490);
            EndPaint(hwndDlg, &CreatePs);
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMCreateProjectCmdOk)
            {
				ProjectGather = WAComboBoxGetIndex(FRMCreateProjectCombo);
                if(WASysTabGetCurrentItem(FRMCreateProjectSysTab) == FRMCreateProjectRecentTabNbr)
                {
                    FRMCreateProjectSelRecent = RetrieveSelRecentPrj();
                    if(FRMCreateProjectSelRecent.Len() != 0)
                    {
						ProjectCreate = 2;
						WAControlClose(hwndDlg);
					}
                }
                else
                {
					if(RetrieveProjectDatas() == 1) WAControlClose(hwndDlg);
                }
                return(0);
            }
            else if((HWND) lParam == FRMCreateProjectCmdCancel)
            {
                ProjectCreate = 0;
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMCreateProjectTextBox)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case EN_CHANGE:
                        if(WATextBoxGetLen(FRMCreateProjectTextBox) > 0)
                        {
                            WAControlEnable(FRMCreateProjectCmdOk, 1);
                        }
                        else
                        {
                            WAControlEnable(FRMCreateProjectCmdOk, 0);
                        }
						break;
				}
            }
			break;
		case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case NM_DBLCLK:
                    if(WAControlGetNotifiedhWnd(lParam) == FRMCreateProjectListView)
                    {
                        if(WAListViewGetItemUnderCursor(FRMCreateProjectListView) != -1)
                        {
                            if(RetrieveProjectDatas() == 1) WAControlClose(hwndDlg);
                        }
                    }
                    else if(WAControlGetNotifiedhWnd(lParam) == FRMCreateProjectRecent)
                    {
                        SelectedItem = WAListViewGetItemUnderCursor(FRMCreateProjectRecent);
                        if(SelectedItem != -1)
                        {
                            FRMCreateProjectSelRecent = RetrieveSelRecentPrj();
                            ProjectCreate = 2;
                            WAControlClose(hwndDlg);
                        }
                    }
                    return(0);
                case TCN_SELCHANGE:
                    FRMCreateProjectChangeCurrentTab(hwndDlg);
                    return(0);
            }
			break;
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMCreateProjectListView, GetSysColor(COLOR_WINDOW));
            WAListViewSetBackColor(FRMCreateProjectRecent, GetSysColor(COLOR_WINDOW));
			break;
		case WM_CLOSE:
            WAIniWriteKey("Layout", "ShowProjects", CheckBoxGetState(FRMCreateProjectStartup), MainIniFile);
            WAIniWriteKey("Layout", "GatherFiles", WAComboBoxGetIndex(FRMCreateProjectCombo), MainIniFile);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
	}
    return(0);
}

// -----------------------------------------------------------------------
// SysTab hook proc
LRESULT CALLBACK FRMCreateProjectSysTabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMCreateProjectTextBox)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case EN_CHANGE:
                        if(WATextBoxGetLen(FRMCreateProjectTextBox) > 0)
                        {
                            WAControlEnable(FRMCreateProjectCmdOk, 1);
                        }
                        else
                        {
                            WAControlEnable(FRMCreateProjectCmdOk, 0);
                        }
						break;
				}
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Change the current tab
void FRMCreateProjectChangeCurrentTab(HWND hwndDlg)
{
    if(WASysTabGetCurrentItem(FRMCreateProjectSysTab) == FRMCreateProjectRecentTabNbr)
    {
        WAControlFreeze(FRMCreateProjectListView, 1);
		WAControlFreeze(FRMCreateProjectRecent, 1);
		WAControlVisible(FRMCreateProjectTextBox, 0);
		WAControlVisible(FRMCreateProjectCombo, 0);
        WAControlVisible(FRMCreateProjectLabel, 0);
        if(FillProjectsRecents() != 0) WAControlEnable(FRMCreateProjectCmdOk, 1);
        else WAControlEnable(FRMCreateProjectCmdOk, 0);
		if(WAControlIsVisible(FRMCreateProjectRecent) == 0) WAControlVisible(FRMCreateProjectRecent, 1);
        if(WAControlIsVisible(FRMCreateProjectListView) == 1) WAControlVisible(FRMCreateProjectListView, 0);
        WAControlFreeze(FRMCreateProjectListView, 0);
		WAControlFreeze(FRMCreateProjectRecent, 0);
    }
    else
    {
		WAControlFreeze(FRMCreateProjectListView, 1);
		WAControlFreeze(FRMCreateProjectRecent, 1);
        FRMCreateProjectFilCListView(WASysTabGetCurrentItem(FRMCreateProjectSysTab));
        WAControlVisible(FRMCreateProjectTextBox, 1);
        WAControlVisible(FRMCreateProjectCombo, 1);
        WAControlVisible(FRMCreateProjectLabel, 1);
        WAControlSendMessage(hwndDlg, EN_CHANGE << 16, (long) FRMCreateProjectTextBox);
        SetFocus(FRMCreateProjectTextBox);
        WAControlBringToTop(FRMCreateProjectListView);
		if(WAControlIsVisible(FRMCreateProjectListView) == 0) WAControlVisible(FRMCreateProjectListView, 1);
        if(WAControlIsVisible(FRMCreateProjectRecent) == 1) WAControlVisible(FRMCreateProjectRecent, 0);
        WAControlFreeze(FRMCreateProjectListView, 0);
		WAControlFreeze(FRMCreateProjectRecent, 0);
    }
}

// -----------------------------------------------------------------------
// Load the list with availables templates
void FRMCreateProjectFilCListView(long CurrentTab)
{
    CStr SavePrjDats;
    CStr Credits;
    CStr Wizard;
    CStr PrjVersion;
    int i = 0;
    int k = 0;

    WAListViewClear(FRMCreateProjectListView);
    for(i = 0; i <= 999; i++)
    {
        FRMCreateProjectRetVal = WAIniReadKey("Projects", "Prj" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        if(FRMCreateProjectRetVal.Len() == 0) break;
        PrjDats = StringSplit(FRMCreateProjectRetVal, "|");
        // Check if this is an old MAsmEd project ini file
        Credits = "";
        // This is a homemade project ?
        SavePrjDats = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 4);
        if(strcmp(SavePrjDats.Left(1).Get_String(), "*") == 0)
        {
            Credits = SavePrjDats.Mid(2).Get_String() + (CStr) " (From work by)";
        }
        else
        {
            Credits = SavePrjDats;
        }
        // Language tab redirector
        SavePrjDats = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 5);
        if(CurrentTab == GetTabNumber(SavePrjDats))
        {
            WAListViewAddItem(FRMCreateProjectListView, StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 1), i, StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 2).Get_Long());
            WAListViewSetSubItem(FRMCreateProjectListView, Credits, k, 1);
			Wizard = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 7);
            if(Wizard.Len() == 0) WAListViewSetSubItem(FRMCreateProjectListView, "No", k, 2);
            else WAListViewSetSubItem(FRMCreateProjectListView, "Yes", k, 2);
			k++;
        }
        StringReleaseSplit(PrjDats);
    }
}

// -----------------------------------------------------------------------
// Load the project types
void FRMCreateProjectFillProjectsTabs(long CurrentTab)
{
    CStr SavePrjDats;
    int i = 0;
    int k = 0;

    for(i = 0; i <= 999; i++)
    {
        FRMCreateProjectRetVal = WAIniReadKey("Projects", "Prj" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        if(FRMCreateProjectRetVal.Len() == 0) break;
        PrjDats = StringSplit(FRMCreateProjectRetVal, "|");
        // Check if this is an old MAsmEd project ini file
        // Language tab redirector
        if(StringGetSplitUBound(PrjDats) >= 5)
        {
            SavePrjDats = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 5);
            if(CurrentTab == GetTabNumber(SavePrjDats))
            {
                FRMCreateProjectTabsNbr.Set(CurrentTab, k);
                k++;
            }
        }
        else
        {
            FRMCreateProjectTabsNbr.Set(CurrentTab, k);
            k++;
        }
        StringReleaseSplit(PrjDats);
    }
}

// -----------------------------------------------------------------------
// Retrieve the tab number associated with the name
long GetTabNumber(CStr TabNameToSearch)
{
    int i = 0;

    for(i = 0; i < FRMCreateProjectTabs.Amount(); i++)
    {
        if(strcmpi(TabNameToSearch.Get_String(), FRMCreateProjectTabs.Get(i)->Content) == 0) break;
    }
    return(i);
}

// -----------------------------------------------------------------------
// Store the selected project infos
long RetrieveProjectDatas(void)
{
    long ReturnValue = 0;
	long EntryNumber = 0;
    long EntryAdd = 0;
    int i = 0;

    if(WAControlGetText(FRMCreateProjectTextBox).Len() == 0)
    {
        WAMiscMsgBox(hMDIform.hWnd, "Project not named.", MB_ERROR, Requesters);
        SetFocus(FRMCreateProjectTextBox);
        return(ReturnValue);
    }
    EntryAdd = 0;
    if(WASysTabGetCurrentItem(FRMCreateProjectSysTab) != 0)
    {
        for(i = 1; i <= WASysTabGetCurrentItem(FRMCreateProjectSysTab); i++)
        {
            EntryAdd = EntryAdd + FRMCreateProjectTabsNbr.Get(i - 1)->Content + 1;
        }
    }
    EntryNumber = WAListViewGetSelItem(FRMCreateProjectListView, -1);
	if(EntryNumber == -1)
	{
		WAMiscMsgBox(hMDIform.hWnd, "No project type selected.", MB_ERROR, Requesters);
		return(0);
	}
	EntryNumber += EntryAdd;
    FRMCreateProjectRetVal = WAIniReadKey("Projects", "Prj" + (CStr) StringNumberComplement(EntryNumber, 3).Get_String(), ProjectsIniFile);
    if(FRMCreateProjectRetVal.Len() == 0)
    {
        WAMiscMsgBox(hMDIform.hWnd, "Mangled projects header.", MB_ERROR, Requesters);
        return(ReturnValue);
    }
    PrjDats = StringSplit(FRMCreateProjectRetVal, "|");
    ProjectTitle = WAControlGetText(FRMCreateProjectTextBox);
    ProjectType = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 0);
    ProjectTypeName = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 1);
    ProjectTypeExtInternalImg = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 2);
    ProjectTypeExt = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 3);
	ProjectPostRename = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 6);
	ProjectWizard = StringGetSplitElement(FRMCreateProjectRetVal, PrjDats, 7);

    ProjectTypeImg = WAIniReadKey(ProjectType, "APPICON", ProjectsIniFile);
    ProjectCreate = 1;
    ReturnValue = 1;
    StringReleaseSplit(PrjDats);
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the recents projects listview
long FillProjectsRecents(void)
{
    int i = 0;
    long ReturnValue = 0;
	CStr ProjectRetVal;
    CStr ProjectRetType;
    CStr RealProjectName;
    long RealIcon = 0;
    CStr BufString;

    WAListViewClear(FRMCreateProjectRecent);
    for(i = 0; i <= MaxRecentPrj - 1; i++)
    {
        ProjectRetVal = WAIniReadKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ProjectRetVal.Len() == 0) break;
        RealProjectName = WAFileReplaceExtension(WAFileGetFileName(ProjectRetVal), "");
        if(strcmp(RealProjectName.Right(1).Get_String(), ".") == 0) RealProjectName = RealProjectName.Mid(1, RealProjectName.Len() - 1);
        ProjectRetType = WAIniReadKey("Project", "TypeExt", ProjectRetVal);
        BufString = ProjectRetType.Upper_Case();
		if(BufString == "WINEXE")
		{
            RealIcon = ICON_MAINICON;
		}
		else if(BufString == "DOSEXE")
		{
            RealIcon = ICON_CONSOLE;
		}
		else if(BufString == "WINDLL")
		{
            RealIcon = ICON_COMPONENT;
		}
		else if(BufString == "WINLIB")
		{
            RealIcon = ICON_STATIC;
		}
		else if(BufString == "WINSYS")
		{
            RealIcon = ICON_SNAP;
		}
		else
		{
            RealIcon = ICON_FILEASSOC;
        }
        WAListViewAddItem(FRMCreateProjectRecent, RealProjectName, i, RealIcon);
        RealProjectName = WAFileGetDirectory(ProjectRetVal);
        if(strcmp(RealProjectName.Right(1).Get_String(), "\\") == 0) RealProjectName = RealProjectName.Mid(1, RealProjectName.Len() - 1);
        WAListViewSetSubItem(FRMCreateProjectRecent, RealProjectName, i, 1);
        ReturnValue++;
    }
    if(WAListViewItemCount(FRMCreateProjectRecent) != 0) WAListViewSetItemSel(FRMCreateProjectRecent, 0);
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Build the selected recent project
CStr RetrieveSelRecentPrj(void)
{
    CStr ReturnValue;
    CStr PrjName;
	long SelectedItem;

    SelectedItem = WAListViewGetSelItem(FRMCreateProjectRecent, -1);
	if(SelectedItem == -1)
	{
		WAMiscMsgBox(hMDIform.hWnd, "No project type selected.", MB_ERROR, Requesters);
		return(ReturnValue);
	}
    PrjName = WAListViewGetItemText(FRMCreateProjectRecent, SelectedItem, 1) + (CStr) "\\";
    PrjName = PrjName + (CStr) WAListViewGetItemText(FRMCreateProjectRecent, SelectedItem, 0).Get_String() + (CStr) ".med";
    ReturnValue = PrjName;
	return(ReturnValue);
}
