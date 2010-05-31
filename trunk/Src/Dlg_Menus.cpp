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
// Dlg_Menus.cpp: Menus resource creation
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Splitters.h"
#include "ProjectsBuilder.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_Menus.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMMenushwnd;
CStr MenusTitle;
HWND FRMenusOk;
HWND FRMMenusCancel;
HWND FRMMenusToolbar;
HWND FRMMenusTreeView;
HWND FRMMenusFrame;
HWND FRMMenusCaption;
HWND FRMMenusCommand;
HWND FRMMenusChkChecked;
HWND FRMMenusChkGrayed;
HWND FRMMenusChkRadio;
HWND FRMMenusCbBreak;
HWND FRMMenusChkBold;

long MenusBase;
long EditMenus;
CStr MenusFileToEdit;
CStr MenusBaseToEdit;

CList <HTREEITEM> MenusArray;
CList <int> MenusTypeArray;
CList <int> MenusCmdArray;
CList <int> MenusPropsArray;

long InCloseProcess;

// -----------------------------------------------------------------------
// Create an empty menus resource
void CreateMenus(void)
{
    long EntryMnuCount = 0;

	EditMenus = 0;
    EntryMnuCount = WATreeViewGetChildItemsCount(hTreeView, hTreeViewMenus);
    // Check if entry already exists
    while(WATreeViewSearchChildPartialText(hTreeView, hTreeViewMenus, "MENUS_" + (CStr) EntryMnuCount) != -1)
    {
        EntryMnuCount++;
    }
    MenusBase = EntryMnuCount;
    MenusTitle = "Create new menus resource";
    WACreateModalDialog(-1, -1, 500, 315, hMDIform.hWnd, &FRMMenusProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    // Free up some memory
    MenusArray.Erase();
    MenusTypeArray.Erase();
    MenusCmdArray.Erase();
    MenusPropsArray.Erase();
}

// -----------------------------------------------------------------------
// Edit strings resource
void EditMenusTable(CStr MenusEditBase, CStr MenusFile)
{
    EditMenus = 1;
    MenusBaseToEdit = MenusEditBase;
    MenusFileToEdit = MenusFile;
    MenusTitle = "Edit menus resource";
    WACreateModalDialog(-1, -1, 500, 315, hMDIform.hWnd, &FRMMenusProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    // Free up some memory
    MenusArray.Erase();
    MenusTypeArray.Erase();
    MenusCmdArray.Erase();
    MenusPropsArray.Erase();
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMMenusProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HTREEITEM TemphMenu = 0;
    HTREEITEM CurrentTreeItem = 0;
    PAINTSTRUCT EnterValuePs;
    long TVIndex = 0;
	long *lParamKey = 0;
    long TreeViewKey = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            WATreeViewSetBackColor(FRMMenusTreeView, GetSysColor(COLOR_WINDOW));
			break;
		case WM_INITDIALOG:
            InCloseProcess = 0;
            MenusArray.Erase();
            MenusTypeArray.Erase();
            MenusCmdArray.Erase();
            MenusPropsArray.Erase();
            FRMMenushwnd = hwndDlg;
            WAControlSetText(hwndDlg, MenusTitle);
            FRMenusOk = WACreateButton(341, 289, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMMenusCancel = WACreateButton(420, 289, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMMenusToolbar = WACreateToolBar(1, 1, 460, 23, hwndDlg, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSADDBAR, ICON_MENUED, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSADDPOPUP, ICON_POPUP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSADDITEM, ICON_ITEM, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSADDSEPARATOR, ICON_SEPARATOR, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMMenusToolbar, 0);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSDELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMMenusToolbar, 0);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSUP, ICON_UP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSDOWN, ICON_DOWN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMMenusToolbar, 0);
            WAToolBarAddButton(FRMMenusToolbar, "", MENUSVIEW, ICON_STACK, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMMenusTreeView = WACreateTreeView(2, 33, 234, 243, hwndDlg, 4, GlobalImageList1, &MenuTreeViewHook, TVS_DISABLEDRAGDROP, WS_EX_STATICEDGE);
            WATreeViewSetIndent(FRMMenusTreeView, 1);
            FRMMenusFrame = WACreateFrame(239, 27, 260, 250, hwndDlg, "Menu properties", 0, &MenusFrameHook, 0);
            WACreateLabel(7, 20, 50, 17, FRMMenusFrame, "Caption :", 0, 0, 0, 0);
            FRMMenusCaption = WACreateTextBox(63, 17, 190 - 1, 20, FRMMenusFrame, "", 5, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            WACreateLabel(7, 45, 100, 17, FRMMenusFrame, "Command :", 0, 0, 0, 0);
            FRMMenusCommand = WACreateTextBox(63, 42, 190 - 1, 20, FRMMenusFrame, "", 6, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, WS_EX_STATICEDGE);
            WATextBoxSetMaxLen(FRMMenusCommand, 5);
            FRMMenusChkChecked = WACreateCheckBox(63, 64, 90, 18, FRMMenusFrame, "Checked", 7, 0, WS_TABSTOP, 0);
            FRMMenusChkGrayed = WACreateCheckBox(161, 64, 90, 18, FRMMenusFrame, "Grayed", 8, 0, WS_TABSTOP, 0);
            FRMMenusChkRadio = WACreateCheckBox(63, 81, 90, 18, FRMMenusFrame, "Radio check", 9, 0, WS_TABSTOP, 0);
            FRMMenusChkBold = WACreateCheckBox(161, 81, 90, 18, FRMMenusFrame, "Default", 10, 0, WS_TABSTOP, 0);
            WACreateLabel(7, 104, 100, 17, FRMMenusFrame, "Break :", 0, 0, 0, 0);
            FRMMenusCbBreak = WACreateComboBox(63, 100, 191, 150, FRMMenusFrame, "", 12, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            WAComboBoxAddItem(FRMMenusCbBreak, "[None]", -1);
            WAComboBoxAddItem(FRMMenusCbBreak, "MFT_MENUBARBREAK", -1);
            WAComboBoxAddItem(FRMMenusCbBreak, "MFT_MENUBREAK", -1);
            WAComboBoxAddItem(FRMMenusCbBreak, "MFT_RIGHTJUSTIFY", -1);
            WAComboBoxSetIndex(FRMMenusCbBreak, 0);
            WAControlEnable(FRMMenusCaption, 0);
            WAControlEnable(FRMMenusCommand, 0);
            WAControlEnable(FRMMenusChkChecked, 0);
            WAControlEnable(FRMMenusChkGrayed, 0);
            WAControlEnable(FRMMenusChkRadio, 0);
            WAControlEnable(FRMMenusChkBold, 0);
            WAControlEnable(FRMMenusCbBreak, 0);
            if(EditMenus == 1) FillTReeViewMenus();
            FreezeTimer = 1;
            SetFocus(FRMMenusTreeView);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            WAGDIDrawHorzSep(hwndDlg, 0, 25, 500);
            WAGDIDrawHorzSep(hwndDlg, 0, 282, 500);
            EndPaint(hwndDlg, &EnterValuePs);
			break;
		case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(WAControlGetNotifiedID(lParam))
                    {
                        case MENUSADDBAR:
                            WAToolBarDisplayToolTip("Create new menu bar", lParam);
                            return(0);
                        case MENUSADDPOPUP:
                            WAToolBarDisplayToolTip("Add menu popup", lParam);
                            return(0);
                        case MENUSADDITEM:
                            WAToolBarDisplayToolTip("Add menu item", lParam);
                            return(0);
                        case MENUSADDSEPARATOR:
                            WAToolBarDisplayToolTip("Add menu separator", lParam);
                            return(0);
                        case MENUSDELETE:
                            WAToolBarDisplayToolTip("Delete menu entry", lParam);
                            return(0);
                        case MENUSUP:
                            WAToolBarDisplayToolTip("Move entry up", lParam);
                            return(0);
                        case MENUSDOWN:
                            WAToolBarDisplayToolTip("Move entry down", lParam);
                            return(0);
                        case MENUSVIEW:
                            WAToolBarDisplayToolTip("Test menu", lParam);
                            return(0);
                    }
					break;
                case TVN_KEYDOWN:
                    lParamKey = (long *) lParam;
					TreeViewKey = lParamKey[3] & 0xffff;
                    switch(TreeViewKey)
                    {
                        case 46:
							CurrentTreeItem = WATreeViewGetSelectedItem(FRMMenusTreeView);
							if(CurrentTreeItem) WATreeViewRemoveItem(FRMMenusTreeView, CurrentTreeItem);
							return(0);
					}
					break;
                case TVN_DELETEITEM:
                    // Clear entry from array
                    // (Message handled for recursivity considerations)
                    if(InCloseProcess == 0)
                    {
                        TVIndex = GetEntryIndex(WATreeViewGetDeletedItem(lParam));
                        MenusArray.Set(TVIndex, 0);
                    }
                    return(0);
                case TVN_SELCHANGED:
                    TemphMenu = WATreeViewGetSelectedItem(FRMMenusTreeView);
                    TVIndex = GetEntryIndex(TemphMenu);
                    switch(MenusTypeArray.Get(TVIndex)->Content)
                    {
                        case MENUTYPE_SEPS:
                            WAControlEnable(FRMMenusCaption, 0);
                            WAControlEnable(FRMMenusCommand, 0);
                            WAControlEnable(FRMMenusChkChecked, 0);
                            WAControlEnable(FRMMenusChkGrayed, 0);
                            WAControlEnable(FRMMenusChkRadio, 0);
                            WAControlEnable(FRMMenusChkBold, 0);
                            WAControlEnable(FRMMenusCbBreak, 0);
                            WAControlSetText(FRMMenusCaption, "<Separator>");
                            WAControlSetText(FRMMenusCommand, "");
                            PutMenuProps(TVIndex);
							break;
                        case MENUTYPE_BAR:
                            WAControlEnable(FRMMenusCaption, 1);
                            WAControlEnable(FRMMenusCommand, 0);
                            WAControlEnable(FRMMenusChkChecked, 0);
                            WAControlEnable(FRMMenusChkGrayed, 1);
                            WAControlEnable(FRMMenusChkRadio, 0);
                            WAControlEnable(FRMMenusChkBold, 0);
                            WAControlEnable(FRMMenusCbBreak, 1);
                            if(strcmpi(WATreeViewGetItemText(FRMMenusTreeView, TemphMenu).Get_String(), "<Empty entry>") == 0)
                            {
                                WAControlSetText(FRMMenusCaption, "");
                            }
                            else
                            {
                                WAControlSetText(FRMMenusCaption, WATreeViewGetItemText(FRMMenusTreeView, TemphMenu));
                            }
                            WAControlSetText(FRMMenusCommand, "");
                            PutMenuProps(TVIndex);
							break;
                        case MENUTYPE_POPUP:
                            WAControlEnable(FRMMenusCaption, 1);
                            WAControlEnable(FRMMenusCommand, 0);
                            WAControlEnable(FRMMenusChkChecked, 0);
                            WAControlEnable(FRMMenusChkGrayed, 0);
                            WAControlEnable(FRMMenusChkRadio, 0);
                            WAControlEnable(FRMMenusChkBold, 0);
                            WAControlEnable(FRMMenusCbBreak, 0);
                            if(strcmpi(WATreeViewGetItemText(FRMMenusTreeView, TemphMenu).Get_String(), "<Empty entry>") == 0)
                            {
                                WAControlSetText(FRMMenusCaption, "");
                            }
                            else
                            {
                                WAControlSetText(FRMMenusCaption, WATreeViewGetItemText(FRMMenusTreeView, TemphMenu));
                            }
                            WAControlSetText(FRMMenusCommand, "");
                            PutMenuProps(TVIndex);
							break;
						case MENUTYPE_ITEM:
                            WAControlEnable(FRMMenusCaption, 1);
                            WAControlEnable(FRMMenusCommand, 1);
                            WAControlEnable(FRMMenusChkChecked, 1);
                            WAControlEnable(FRMMenusChkGrayed, 1);
                            WAControlEnable(FRMMenusChkRadio, 1);
                            WAControlEnable(FRMMenusChkBold, 1);
                            WAControlEnable(FRMMenusCbBreak, 0);
                            if(strcmpi(WATreeViewGetItemText(FRMMenusTreeView, TemphMenu).Get_String(), "<Empty entry>") == 0)
                            {
                                WAControlSetText(FRMMenusCaption, "");
                            }
                            else
                            {
                                WAControlSetText(FRMMenusCaption, WATreeViewGetItemText(FRMMenusTreeView, TemphMenu));
                            }
                            WAControlSetText(FRMMenusCommand, MenusCmdArray.Get(TVIndex)->Content);
                            PutMenuProps(TVIndex);
                    }
                    return(0);
            }
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMMenusToolbar)
            {
                switch(wParam)
                {
                    case MENUSADDBAR:
                        TemphMenu = WATreeViewAddItem(FRMMenusTreeView, "Menu bar", TVI_ROOT, 0, ICON_MENUED, ICON_MENUED, TVIS_BOLD, 1);
                        MenusArray.Add(TemphMenu);
                        MenusTypeArray.Add(MENUTYPE_BAR);
                        MenusCmdArray.Add(1L);
                        MenusPropsArray.Add(0L);
                        // Select it by default if it is the first entry
                        if(WATreeViewGetItemCount(FRMMenusTreeView) == 1)
                        {
                            WATreeViewSetSelectedItem(FRMMenusTreeView, TemphMenu);
                        }
                        SetFocus(FRMMenusTreeView);
						break;
					case MENUSADDPOPUP:
                        CurrentTreeItem = WATreeViewGetSelectedItem(FRMMenusTreeView);
                        if(CurrentTreeItem == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "No entry selected.", MB_ERROR, Requesters);
                            SetFocus(FRMMenusTreeView);
                            return(0);
                        }
                        TVIndex = GetEntryIndex(CurrentTreeItem);
                        if((MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_POPUP) &&
                           (MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_BAR))
                        {
                            CurrentTreeItem = WATreeViewGetItemParent(FRMMenusTreeView, CurrentTreeItem);
                        }
                        TemphMenu = WATreeViewAddItem(FRMMenusTreeView, "Menu popup", CurrentTreeItem, 0, ICON_POPUP, ICON_POPUP, 0, 1);
                        MenusArray.Add(TemphMenu);
                        MenusTypeArray.Add(MENUTYPE_POPUP);
                        MenusCmdArray.Add(1L);
                        MenusPropsArray.Add(0L);
                        SetFocus(FRMMenusTreeView);
						break;
                    case MENUSADDITEM:
                        CurrentTreeItem = WATreeViewGetSelectedItem(FRMMenusTreeView);
                        if(CurrentTreeItem == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "No entry selected.", MB_ERROR, Requesters);
                            SetFocus(FRMMenusTreeView);
                            return(0);
                        }
                        TVIndex = GetEntryIndex(CurrentTreeItem);
                        if((MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_POPUP) &&
                           (MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_BAR))
                        {
                            CurrentTreeItem = WATreeViewGetItemParent(FRMMenusTreeView, CurrentTreeItem);
                        }
                        TemphMenu = WATreeViewAddItem(FRMMenusTreeView, "Menu item", CurrentTreeItem, 0, ICON_ITEM, ICON_ITEM, 0, 1);
                        MenusArray.Add(TemphMenu);
                        MenusTypeArray.Add(MENUTYPE_ITEM);
                        MenusCmdArray.Add(1L);
                        MenusPropsArray.Add(0L);
                        SetFocus(FRMMenusTreeView);
						break;
                    case MENUSADDSEPARATOR:
                        CurrentTreeItem = WATreeViewGetSelectedItem(FRMMenusTreeView);
                        if(CurrentTreeItem == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "No entry selected.", MB_ERROR, Requesters);
                            SetFocus(FRMMenusTreeView);
                            return(0);
                        }
                        TVIndex = GetEntryIndex(CurrentTreeItem);
                        if((MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_POPUP) &&
                           (MenusTypeArray.Get(TVIndex)->Content != MENUTYPE_BAR))
                        {
                            CurrentTreeItem = WATreeViewGetItemParent(FRMMenusTreeView, CurrentTreeItem);
                        }
                        TemphMenu = WATreeViewAddItem(FRMMenusTreeView, "<Separator>", CurrentTreeItem, 0, ICON_SEPARATOR, ICON_SEPARATOR, 0, 1);
                        MenusArray.Add(TemphMenu);
                        MenusTypeArray.Add(MENUTYPE_SEPS);
                        MenusCmdArray.Add(1L);
                        MenusPropsArray.Add(0L);
                        SetFocus(FRMMenusTreeView);
						break;
                    case MENUSDELETE:
                        RemoveMenuEntry(WATreeViewGetSelectedItem(FRMMenusTreeView));
						break;
					case MENUSUP:
                        MoveMenuEntryUp(WATreeViewGetSelectedItem(FRMMenusTreeView));
						break;
					case MENUSDOWN:
                        MoveMenuEntryDown(WATreeViewGetSelectedItem(FRMMenusTreeView));
						break;
                    case MENUSVIEW:
                        TestMenus();
						break;
                }
                return(0);
            }
            else if((HWND) lParam == FRMenusOk)
            {
                if(EditMenus == 0)
                {
                    CreateNewMenusFile();
                    ProjectModified = TRUE;
                }
                else
                {
                    SaveMenusFile(MenusFileToEdit);
                }
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMMenusCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
			}
			break;
        case WM_CLOSE:
            // Avoid delete items notifications to speed the things up
            // (It is slow enough like that)
            InCloseProcess = 1;
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Treeview control hook
LRESULT CALLBACK MenuTreeViewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HTREEITEM RightSelection = 0;
    
	switch(uMsg)
	{
        case WM_RBUTTONDBLCLK:
            return(0);
        case WM_LBUTTONDOWN:
            RightSelection = WATreeViewGetItemFromPos(FRMMenusTreeView, lParam & 0xFFFF, ((lParam & 0xFFFF0000) >> 16));
            if(RightSelection != 0) WATreeViewSetSelectedItem(FRMMenusTreeView, RightSelection);
			break;
		case WM_RBUTTONDOWN:
            RightSelection = WATreeViewGetItemFromPos(FRMMenusTreeView, lParam & 0xFFFF, ((lParam & 0xFFFF0000) >> 16));
            if(RightSelection != 0)
            {
                WATreeViewSetSelectedItem(FRMMenusTreeView, RightSelection);
                SetFocus(FRMMenusTreeView);
            }
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Retrieve index of a treeview entry from hWnds array
long GetEntryIndex(HTREEITEM hWnd)
{
    int i = 0;
    long ReturnValue = -1;
    
    for(i = 0; i < MenusArray.Amount(); i++)
    {
        if(MenusArray.Get(i)->Content != 0)
        {
            if(MenusArray.Get(i)->Content == hWnd)
            {
                ReturnValue = i;
                break;
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Treeview control hook
LRESULT CALLBACK MenusFrameHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HTREEITEM hItemToUpdate = 0;
    long TVIndex = 0;
    long CurrentProp = 0;
    long CbIndexToStore = 0;
    CStr CommandStr;
    CStr MenuCapt;

    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMMenusCaption)
            {
                if(WAControlGetNotifiedCommand(wParam) == EN_CHANGE)
                {
                    hItemToUpdate = WATreeViewGetSelectedItem(FRMMenusTreeView);
                    MenuCapt = WAControlGetText(FRMMenusCaption);
                    if(MenuCapt.Len() == 0) MenuCapt = "<Empty entry>";
                    WATreeViewSetItemText(FRMMenusTreeView, hItemToUpdate, MenuCapt);
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusCommand)
            {
                if(WAControlGetNotifiedCommand(wParam) == EN_CHANGE)
                {
                    TVIndex = GetCurrentPropIndex();
                    CommandStr = WAControlGetText(FRMMenusCommand);
                    if(CommandStr.Len() == 0)
                    {
                        MenusCmdArray.Set(TVIndex, 0);
                    }
                    else
                    {
                        MenusCmdArray.Set(TVIndex, CommandStr.Get_Long());
                    }
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusCbBreak)
            {
                if(WAControlGetNotifiedCommand(wParam) == CBN_SELCHANGE)
                {
                    TVIndex = GetCurrentPropIndex();
                    CurrentProp = MenusPropsArray.Get(TVIndex)->Content;
                    CbIndexToStore = WAComboBoxGetIndex(FRMMenusCbBreak);
                    CbIndexToStore = (CbIndexToStore * 0x10000) & 0xFF0000;
                    CurrentProp = CurrentProp & 0xFF00FFFF;
                    CurrentProp = CurrentProp | CbIndexToStore;
                    MenusPropsArray.Set(TVIndex, CurrentProp);
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusChkChecked)
            {
                TVIndex = GetCurrentPropIndex();
                CurrentProp = MenusPropsArray.Get(TVIndex)->Content;
                CurrentProp = CurrentProp & 0xFFFFFFFE;
                CurrentProp = CurrentProp | CheckBoxGetState(FRMMenusChkChecked);
                MenusPropsArray.Set(TVIndex, CurrentProp);
                return(0);
            }
            else if((HWND) lParam == FRMMenusChkGrayed)
            {
                    TVIndex = GetCurrentPropIndex();
                    CurrentProp = MenusPropsArray.Get(TVIndex)->Content;
                    CurrentProp = CurrentProp & 0xFFFFFFFD;
                    CurrentProp = CurrentProp | (CheckBoxGetState(FRMMenusChkGrayed) * 2);
                    MenusPropsArray.Set(TVIndex, CurrentProp);
                    return(0);
            }
            else if((HWND) lParam == FRMMenusChkRadio)
            {
                TVIndex = GetCurrentPropIndex();
                CurrentProp = MenusPropsArray.Get(TVIndex)->Content;
                CurrentProp = CurrentProp & 0xFFFFFFFB;
                CurrentProp = CurrentProp | (CheckBoxGetState(FRMMenusChkRadio) * 4);
                MenusPropsArray.Set(TVIndex, CurrentProp);
                return(0);
            }
            else if((HWND) lParam == FRMMenusChkBold)
            {
                TVIndex = GetCurrentPropIndex();
                CurrentProp = MenusPropsArray.Get(TVIndex)->Content;
                CurrentProp = CurrentProp & 0xFFFFFFF7;
                CurrentProp = CurrentProp | (CheckBoxGetState(FRMMenusChkBold) * 8);
                MenusPropsArray.Set(TVIndex, CurrentProp);
                return(0);
            }
			break;
	}
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Set controls according to menu properties
void PutMenuProps(long PropIndex)
{
    long MenuProp = 0;
    long MenuPropBreak = 0;
    
	MenuProp = MenusPropsArray.Get(PropIndex)->Content;
    if((MenuProp & MENUPROP_CHECKED) != 0)
    {
        CheckBoxSetState(FRMMenusChkChecked, 1);
    }
    else
    {
        CheckBoxSetState(FRMMenusChkChecked, 0);
    }
    if((MenuProp & MENUPROP_GRAYED) != 0)
    {
        CheckBoxSetState(FRMMenusChkGrayed, 1);
    }
    else
    {
        CheckBoxSetState(FRMMenusChkGrayed, 0);
    }
    if((MenuProp & MENUPROP_RADIO) != 0)
    {
        CheckBoxSetState(FRMMenusChkRadio, 1);
    }
    else
    {
        CheckBoxSetState(FRMMenusChkRadio, 0);
    }
    if((MenuProp & MENUPROP_BOLD) != 0)
    {
        CheckBoxSetState(FRMMenusChkBold, 1);
    }
    else
    {
        CheckBoxSetState(FRMMenusChkBold, 0);
    }
    MenuPropBreak = (MenuProp & MENUPROP_BREAKMASK) / 0x10000;
    WAComboBoxSetIndex(FRMMenusCbBreak, MenuPropBreak);
}

// -----------------------------------------------------------------------
// Return properties datas of current selected menu
long GetCurrentPropIndex(void)
{
    HTREEITEM hItemToUpdate = 0;
    
	hItemToUpdate = WATreeViewGetSelectedItem(FRMMenusTreeView);
    return(GetEntryIndex(hItemToUpdate));
}

// -----------------------------------------------------------------------
// Create a new menus ini file for current project
void CreateNewMenusFile(void)
{
    int i = 0;
    HANDLE MnuFileHandle = 0;
    CStr MnuFileName;
    CStr MnuName;
    CStr EntryToAdd;
    long EntryIDX = 0;
    HTREEITEM hBar = 0;
    HTREEITEM hChild = 0;
    HTREEITEM SavedChild = 0;
    long ItemNbrToSave = 0;

    MnuName = "MENUS_" + (CStr) MenusBase;
    MnuFileName = WAFileGetDirectory(ProjectFName) + (CStr) MnuName + (CStr) ".mnu";
    MnuFileHandle = WAFileCreateEmpty(MnuFileName, NO_SECURITY_DESCRIPTOR);
    WAFileWriteLine(MnuFileHandle, "; Chromatic resource menus file");
    WAFileWriteLine(MnuFileHandle, "; Do NOT edit this file manually");
    WAFileWriteLine(MnuFileHandle, "; unless you know what you're doing !!!");
    WAFileWriteLine(MnuFileHandle, "; (C) 2001-2010 Franck Charlet.");
    WAFileWriteLine(MnuFileHandle, "");
    WAFileWriteLine(MnuFileHandle, "[MNUDAT]");
    WAFileClose(MnuFileHandle);
    EntryToAdd = MnuName + (CStr) " (" + (CStr) MnuFileName + (CStr) ")";
    // Save menu bar by bar
    hBar = WATreeViewGetRoot(FRMMenusTreeView);
    ItemNbrToSave = WATreeViewGetItemCount(FRMMenusTreeView);
    for(i = 0; i <= (long) ItemNbrToSave - 1; i++)
    {
        EntryIDX = GetEntryIndex(hBar);
        WAIniWriteKey("MNUDAT", "MNU" + (CStr) StringNumberComplement(i, 3).Get_String(), WATreeViewGetItemText(FRMMenusTreeView, MenusArray.Get(EntryIDX)->Content), MnuFileName);
        WAIniWriteKey("MNUDAT", "MNUCMD" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusCmdArray.Get(EntryIDX)->Content, MnuFileName);
        WAIniWriteKey("MNUDAT", "MNUTYPE" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusTypeArray.Get(EntryIDX)->Content, MnuFileName);
        WAIniWriteKey("MNUDAT", "MNUPROPS" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusPropsArray.Get(EntryIDX)->Content, MnuFileName);
        WAIniWriteKey("MNUDAT", "MNULEVEL" + (CStr) StringNumberComplement(i, 3).Get_String(), WATreeViewGetItemLevel(FRMMenusTreeView, MenusArray.Get(EntryIDX)->Content), MnuFileName);
        hChild = WATreeViewGetFirstItemChild(FRMMenusTreeView, hBar);
        if(hChild == 0)
        {
            // No childs ?
            hChild = WATreeViewGetNextItem(FRMMenusTreeView, hBar);
            if(hChild == 0)
            {
SeekParent:     hChild = WATreeViewGetItemParent(FRMMenusTreeView, hBar);
                SavedChild = hChild;
                hChild = WATreeViewGetNextItem(FRMMenusTreeView, hChild);
                if(hChild == 0)
                {
                    hBar = SavedChild;
                    if(SavedChild == 0) break;
                    goto SeekParent;
                }
            }
        }
        hBar = hChild;
    }
    AddMenuInArray(RESPROPS_DEFAULT, 0);
    WATreeViewAddItem(hTreeView, EntryToAdd, hTreeViewMenus, 0, ICON_RES, ICON_RES, 0, 1);
}

// -----------------------------------------------------------------------
// Save a menus ini file for current project
long SaveMenusFile(CStr MenusFileToSave)
{
    int i = 0;
    HANDLE MnuFileHandle = 0;
    long ItemNbrToSave = 0;
    HTREEITEM hChild = 0;
    long EntryIDX = 0;
    HTREEITEM hBar = 0;
    HTREEITEM SavedChild = 0;

    MnuFileHandle = WAFileCreateEmpty(MenusFileToSave, NO_SECURITY_DESCRIPTOR);
    if(MnuFileHandle != INVALID_HANDLE_VALUE)
    {
        WAFileWriteLine(MnuFileHandle, "; Chromatic resource menus file");
        WAFileWriteLine(MnuFileHandle, "; Do NOT edit this file manually");
        WAFileWriteLine(MnuFileHandle, "; unless you know what you're doing !!!");
        WAFileWriteLine(MnuFileHandle, "; (C) 2001-2010 Franck Charlet.");
        WAFileWriteLine(MnuFileHandle, "");
        WAFileWriteLine(MnuFileHandle, "[MNUDAT]");
        WAFileClose(MnuFileHandle);
        // Save menu bar by bar
        hBar = WATreeViewGetRoot(FRMMenusTreeView);
        ItemNbrToSave = WATreeViewGetItemCount(FRMMenusTreeView);
        for(i = 0; i <= (long) ItemNbrToSave - 1; i++)
        {
            EntryIDX = GetEntryIndex(hBar);
            WAIniWriteKey("MNUDAT", "MNU" + (CStr) StringNumberComplement(i, 3).Get_String(), WATreeViewGetItemText(FRMMenusTreeView, MenusArray.Get(EntryIDX)->Content), MenusFileToSave);
            WAIniWriteKey("MNUDAT", "MNUCMD" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusCmdArray.Get(EntryIDX)->Content, MenusFileToSave);
            WAIniWriteKey("MNUDAT", "MNUTYPE" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusTypeArray.Get(EntryIDX)->Content, MenusFileToSave);
            WAIniWriteKey("MNUDAT", "MNUPROPS" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusPropsArray.Get(EntryIDX)->Content, MenusFileToSave);
            WAIniWriteKey("MNUDAT", "MNULEVEL" + (CStr) StringNumberComplement(i, 3).Get_String(), WATreeViewGetItemLevel(FRMMenusTreeView, MenusArray.Get(EntryIDX)->Content), MenusFileToSave);
            hChild = WATreeViewGetFirstItemChild(FRMMenusTreeView, hBar);
            if(hChild == 0)
            {
                // No childs ?
                hChild = WATreeViewGetNextItem(FRMMenusTreeView, hBar);
                if(hChild == 0)
                {
SeekParent:         hChild = WATreeViewGetItemParent(FRMMenusTreeView, hBar);
                    SavedChild = hChild;
                    hChild = WATreeViewGetNextItem(FRMMenusTreeView, hChild);
                    if(hChild == 0)
                    {
                        hBar = SavedChild;
                        if(SavedChild == 0) break;
                        goto SeekParent;
                    }
                }
            }
            hBar = hChild;
        }
    }
    else
    {
        WAMiscMsgBox(FRMMenushwnd, "Can't save menus file.", MB_ERROR, Requesters);
    }
    return(ItemNbrToSave);
}

// -----------------------------------------------------------------------
// Fill the treeview with ini file datas
void FillTReeViewMenus(void)
{
    HTREEITEM LasthMenu = 0;
    CStr MenuEntryName;
    CStr MenuEntryCmd;
    CStr MenuEntryType;
    long MenuEntryTypeLng = 0;
    int i = 0;
    int j = 0;
    CStr MenuEntryProps;
    CStr MenuEntryLevel;
    long MenuEntryLevelLng = 0;
    long MenuEntryOldLevel = 0;

    for(i = 0; i <= 999; i++)
    {
        MenuEntryName = WAIniReadKey("MNUDAT", "MNU" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileToEdit);
        if(MenuEntryName.Len() == 0) break;
        MenuEntryCmd = WAIniReadKey("MNUDAT", "MNUCMD" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileToEdit);
        if(MenuEntryCmd.Len() == 0) break;
        MenuEntryType = WAIniReadKey("MNUDAT", "MNUTYPE" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileToEdit);
        if(MenuEntryType.Len() == 0) break;
        MenuEntryProps = WAIniReadKey("MNUDAT", "MNUPROPS" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileToEdit);
        if(MenuEntryProps.Len() == 0) break;
        MenuEntryLevel = WAIniReadKey("MNUDAT", "MNULEVEL" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileToEdit);
        if(MenuEntryLevel.Len() == 0) break;
        MenuEntryTypeLng = MenuEntryType.Get_Long();
        MenuEntryLevelLng = MenuEntryLevel.Get_Long();
        switch(MenuEntryTypeLng)
        {
            case MENUTYPE_BAR:
                LasthMenu = WATreeViewAddItem(FRMMenusTreeView, MenuEntryName, TVI_ROOT, 0, ICON_MENUED, ICON_MENUED, TVIS_BOLD, 1);
                MenusArray.Add(LasthMenu);
                MenusTypeArray.Add(MenuEntryType.Get_Long());
                MenusCmdArray.Add(MenuEntryCmd.Get_Long());
                MenusPropsArray.Add(MenuEntryProps.Get_Long());
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_POPUP:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                        {
                            LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                        }
                    }
                }
                else
                {
                    // Stay at same level
                    LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                }
                LasthMenu = WATreeViewAddItem(FRMMenusTreeView, MenuEntryName, LasthMenu, 0, ICON_POPUP, ICON_POPUP, 0, 1);
                MenusArray.Add(LasthMenu);
                MenusTypeArray.Add(MENUTYPE_POPUP);
                MenusCmdArray.Add(0L);
                MenusPropsArray.Add(MenuEntryProps.Get_Long());
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_ITEM:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                        {
                            LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                        }
                    }
                }
                else
                {
                    // Stay at same level
                    LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                }
                LasthMenu = WATreeViewAddItem(FRMMenusTreeView, MenuEntryName, LasthMenu, 0, ICON_ITEM, ICON_ITEM, 0, 1);
                MenusArray.Add(LasthMenu);
                MenusTypeArray.Add(MENUTYPE_ITEM);
                MenusCmdArray.Add(MenuEntryCmd.Get_Long());
                MenusPropsArray.Add(MenuEntryProps.Get_Long());
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_SEPS:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                        {
                            LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                        }
                    }
                }
                else
                {
                    // Stay at same level
                    LasthMenu = WATreeViewGetItemParent(FRMMenusTreeView, LasthMenu);
                }
                LasthMenu = WATreeViewAddItem(FRMMenusTreeView, MenuEntryName, LasthMenu, 0, ICON_SEPARATOR, ICON_SEPARATOR, 0, 1);
                MenusArray.Add(LasthMenu);
                MenusTypeArray.Add(MENUTYPE_SEPS);
                MenusCmdArray.Add(0L);
                MenusPropsArray.Add(0L);
                MenuEntryOldLevel = MenuEntryLevelLng;
        }
    }
}

// -----------------------------------------------------------------------
// Remove a menu entry in both the treeview and arrays
// (Provided that Windows never creates double hwnds for treeview entries)
void RemoveMenuEntry(HTREEITEM hItem)
{
    WATreeViewRemoveItem(FRMMenusTreeView, hItem);
}

// -----------------------------------------------------------------------
// Move an entry up into treeview
void MoveMenuEntryUp(HTREEITEM hItem)
{
    HTREEITEM NewhItem = 0;
    HTREEITEM ItemToSwap = 0;
    long IdxCurrent = 0;
    long IdxToSwap = 0;
    TV_ITEM TreeViewItem;
    TV_INSERTSTRUCT TreeViewInsertItem;
    CStr TVItemTxt;
    HTREEITEM hChild = 0;

    ItemToSwap = WATreeViewGetPreviousItem(FRMMenusTreeView, hItem);
    // First item of a group ?
    if(ItemToSwap == 0) return;
    IdxCurrent = GetEntryIndex(hItem);
    IdxToSwap = GetEntryIndex(ItemToSwap);
    // Collapse item
    WATreeViewSetItemExpandedState(FRMMenusTreeView, ItemToSwap, 0);
    // Retrieve item's infos
    TVItemTxt = TVItemTxt.String(1024, 1);
    TreeViewItem.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_CHILDREN | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
    TreeViewItem.stateMask = 0xffff; //TVIS_ALL
    TreeViewItem.hItem = ItemToSwap;
    TreeViewItem.cchTextMax = 1024;
    TreeViewItem.pszText = TVItemTxt.Get_String();
    SendMessage(FRMMenusTreeView, TVM_GETITEM, 0, (LPARAM) &TreeViewItem);
    // Insert item
    TreeViewInsertItem.hParent = WATreeViewGetItemParent(FRMMenusTreeView, ItemToSwap);
    TreeViewInsertItem.hInsertAfter = hItem;
    TreeViewInsertItem.item.mask = TreeViewItem.mask;
    TreeViewInsertItem.item.iImage = TreeViewItem.iImage;
    TreeViewInsertItem.item.iSelectedImage = TreeViewItem.iSelectedImage;
    TreeViewInsertItem.item.pszText = TreeViewItem.pszText;
    TreeViewInsertItem.item.cchTextMax = strlen(TreeViewItem.pszText);
    TreeViewInsertItem.item.cChildren = TreeViewItem.cChildren;
    TreeViewInsertItem.item.state = TreeViewItem.state;
    TreeViewInsertItem.item.stateMask = TreeViewItem.stateMask;
    NewhItem = (HTREEITEM) SendMessage(FRMMenusTreeView, TVM_INSERTITEM, 0, (LPARAM) &TreeViewInsertItem);
	if(WATreeViewGetChildItemsCount(FRMMenusTreeView, WATreeViewGetItemParent(FRMMenusTreeView, NewhItem)) == 1) WAControlRefreshLocal(FRMMenusTreeView);
    // Move childs if there's any (bcoz Windows is a real bitch)
    hChild = WATreeViewGetFirstItemChild(FRMMenusTreeView, ItemToSwap);
    if(hChild != 0) MoveEntryChilds(hChild, NewhItem);
    //' Delete old item + childrens (at least this shit can do that for us)
    SendMessage(FRMMenusTreeView, TVM_DELETEITEM, 0, (long) ItemToSwap);
    // Register the new hwnd
    MenusArray.Set(IdxToSwap, NewhItem);
    if(WATreeViewGetChildItemsCount(FRMMenusTreeView, WATreeViewGetItemParent(FRMMenusTreeView, ItemToSwap)) == 1) WAControlRefreshLocal(FRMMenusTreeView);
}

// -----------------------------------------------------------------------
// Move an entry down into treeview
void MoveMenuEntryDown(HTREEITEM hItem)
{
    HTREEITEM NewhItem = 0;
    HTREEITEM ItemToSwap = 0;
    long IdxCurrent = 0;
    long IdxToSwap = 0;
    TV_ITEM TreeViewItem;
    TV_INSERTSTRUCT TreeViewInsertItem;
    CStr TVItemTxt;
    HTREEITEM hChild = 0;
    HTREEITEM hItemTopInsert = 0;

    ItemToSwap = WATreeViewGetNextItem(FRMMenusTreeView, hItem);
    // First item of a group ?
    if(ItemToSwap == 0) return;
    IdxCurrent = GetEntryIndex(hItem);
    IdxToSwap = GetEntryIndex(ItemToSwap);
    // Collapse item
    WATreeViewSetItemExpandedState(FRMMenusTreeView, ItemToSwap, 0);
    // Retrieve item's infos
    TVItemTxt = TVItemTxt.String(1024, 1);
    TreeViewItem.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_CHILDREN | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
    TreeViewItem.stateMask = 0xffff; //TVIS_ALL
    TreeViewItem.hItem = ItemToSwap;
    TreeViewItem.cchTextMax = 1024;
    TreeViewItem.pszText = TVItemTxt.Get_String();
    SendMessage(FRMMenusTreeView, TVM_GETITEM, 0, (LPARAM) &TreeViewItem);
    // Insert item
    TreeViewInsertItem.hParent = WATreeViewGetItemParent(FRMMenusTreeView, ItemToSwap);
    hItemTopInsert = WATreeViewGetPreviousItem(FRMMenusTreeView, hItem);
    if(hItemTopInsert == 0) hItemTopInsert = TVI_FIRST;
    TreeViewInsertItem.hInsertAfter = hItemTopInsert;
    TreeViewInsertItem.item.mask = TreeViewItem.mask;
    TreeViewInsertItem.item.iImage = TreeViewItem.iImage;
    TreeViewInsertItem.item.iSelectedImage = TreeViewItem.iSelectedImage;
    TreeViewInsertItem.item.pszText = TreeViewItem.pszText;
    TreeViewInsertItem.item.cchTextMax = strlen(TreeViewItem.pszText);
    TreeViewInsertItem.item.cChildren = TreeViewItem.cChildren;
    TreeViewInsertItem.item.state = TreeViewItem.state;
    TreeViewInsertItem.item.stateMask = TreeViewItem.stateMask;
    NewhItem = (HTREEITEM) SendMessage(FRMMenusTreeView, TVM_INSERTITEM, 0, (LPARAM) &TreeViewInsertItem);
    if(WATreeViewGetChildItemsCount(FRMMenusTreeView, WATreeViewGetItemParent(FRMMenusTreeView, NewhItem)) == 1) WAControlRefreshLocal(FRMMenusTreeView);
    // Move childs if there's any (bcoz Windows is a real bitch)
    hChild = WATreeViewGetFirstItemChild(FRMMenusTreeView, ItemToSwap);
    if(hChild != 0) MoveEntryChilds(hChild, NewhItem);
    // Delete old item + childrens (at least this shit can do that for us)
    SendMessage(FRMMenusTreeView, TVM_DELETEITEM, 0, (long) ItemToSwap);
    // Register the new hwnd
    MenusArray.Set(IdxToSwap, NewhItem);
    if(WATreeViewGetChildItemsCount(FRMMenusTreeView, WATreeViewGetItemParent(FRMMenusTreeView, ItemToSwap)) == 1) WAControlRefreshLocal(FRMMenusTreeView);
}

// -----------------------------------------------------------------------
// Move a child entry + it's childrens (we need recursivity here)
void MoveEntryChilds(HTREEITEM hTVChild, HTREEITEM MainParent)
{
    CStr TVItemTxt;
    TV_ITEM TreeViewItem;
    TV_INSERTSTRUCT TreeViewInsertItem;
    HTREEITEM hNewChild = 0;
    long IdxChild = 0;
    HTREEITEM hNextChild = 0;
    HTREEITEM SubChild = 0;

MoveNextChild:
    hNextChild = WATreeViewGetNextItem(FRMMenusTreeView, hTVChild);
    IdxChild = GetEntryIndex(hTVChild);
    TVItemTxt = TVItemTxt.String(1024, 1);
    TreeViewItem.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_CHILDREN | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE;
    TreeViewItem.stateMask = 0xffff; //TVIS_ALL
    TreeViewItem.hItem = hTVChild;
    TreeViewItem.cchTextMax = 1024;
    TreeViewItem.pszText = TVItemTxt.Get_String();
    SendMessage(FRMMenusTreeView, TVM_GETITEM, 0, (LPARAM) &TreeViewItem);
    TreeViewInsertItem.hParent = MainParent;
    TreeViewInsertItem.hInsertAfter = TVI_LAST;
    TreeViewInsertItem.item.mask = TreeViewItem.mask | TVIF_HANDLE;
    TreeViewInsertItem.item.iImage = TreeViewItem.iImage;
    TreeViewInsertItem.item.iSelectedImage = TreeViewItem.iSelectedImage;
    TreeViewInsertItem.item.pszText = TreeViewItem.pszText;
    TreeViewInsertItem.item.cchTextMax = strlen(TreeViewItem.pszText);
    TreeViewInsertItem.item.cChildren = TreeViewItem.cChildren;
    TreeViewInsertItem.item.state = TreeViewItem.state;
    TreeViewInsertItem.item.stateMask = TreeViewItem.stateMask;
    TreeViewInsertItem.item.hItem = hTVChild;
    hNewChild = (HTREEITEM) SendMessage(FRMMenusTreeView, TVM_INSERTITEM, 0, (LPARAM) &TreeViewInsertItem);
    // Check if there's children in old treeview entry
    SubChild = WATreeViewGetFirstItemChild(FRMMenusTreeView, hTVChild);
    // Yea then add them to new entry
    if(SubChild != 0) MoveEntryChilds(SubChild, hNewChild);
    // Delete old item + childrens (at least this shit can do that for us)
    SendMessage(FRMMenusTreeView, TVM_DELETEITEM, 0, (long) hTVChild);
    MenusArray.Set(IdxChild, hNewChild);
    // End of children list ?
    if(hNextChild != 0)
    {
        // No proceed with next one
        hTVChild = hNextChild;
        goto MoveNextChild;
    }
}

// -----------------------------------------------------------------------
// Create a dialog for menus testing
void TestMenus(void)
{
	CStr BufString;
    
	if(SaveMenusFile(WAFileGetDirectory(ProjectFName) + (CStr) "mnutest.mut") == 0)
	{
        BufString = WAFileGetDirectory(ProjectFName) + (CStr) "mnutest.mut";
		DeleteFile(BufString.Get_String());
        WAMiscMsgBox(FRMMenushwnd, "Can't display empty menus resource.", MB_ERROR, Requesters);
        return;
    }
    WACreateModalDialog(-1, -1, 550, 110, FRMMenushwnd, &FRMTestMenusProc, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, 1);
    BufString = WAFileGetDirectory(ProjectFName) + (CStr) "mnutest.mut";
	DeleteFile(BufString.Get_String());
}

// -----------------------------------------------------------------------
// Test Form
int CALLBACK FRMTestMenusProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            WAControlSetText(hwndDlg, "Menus resource test");
            ConstructTestMenus(hwndDlg);
            return(1);
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Construct test menus structure
void ConstructTestMenus(HWND hwnd)
{
    MENUITEMINFO InfosToChange;
    HMENU LasthMenu = 0;
    HMENU LasthBar = 0;
    long LasthBarProps = 0;
    HMENU LasthMenuSub = 0;
    CStr LastMenuStr;
    long LastBarPop = 0;
    CStr MenuEntryName;
    CStr MenuEntryCmd;
    CStr MenuEntryType;
    long MenuEntryTypeLng = 0;
    int i = 0;
    int j = 0;
    CStr MenuEntryProps;
    long MenuEntryPropsLng = 0;
    CStr MenuEntryLevel;
    long MenuEntryLevelLng = 0;
    long MenuEntryOldLevel = 0;
    HMENU hMainMenu = 0;
    CStr TestFile;
    CList <HMENU> PopupStack;
    CList <int> LevelStack;
    long BarPos = 0;
    long ItemNumber = 0;
    long ItemState = 0;
    long ItemType = 0;
    long ItemTypeMask = 0;

    TestFile = WAFileGetDirectory(ProjectFName) + (CStr) "mnutest.mut";
    hMainMenu = CreateMenu();
    SetMenu(hwnd, hMainMenu);         // Create and attach primary bar
    ItemNumber = 1;
    BarPos = 0;
    PopupStack.Erase();
    LevelStack.Erase();
    for(i = 0; i<= 999; i++)
    {
        MenuEntryName = WAIniReadKey("MNUDAT", "MNU" + (CStr) StringNumberComplement(i, 3).Get_String(), TestFile);
        if(MenuEntryName.Len() == 0) break;
        MenuEntryCmd = WAIniReadKey("MNUDAT", "MNUCMD" + (CStr) StringNumberComplement(i, 3).Get_String(), TestFile);
        if(MenuEntryCmd.Len() == 0) break;
        MenuEntryType = WAIniReadKey("MNUDAT", "MNUTYPE" + (CStr) StringNumberComplement(i, 3).Get_String(), TestFile);
        if(MenuEntryType.Len() == 0) break;
        MenuEntryProps = WAIniReadKey("MNUDAT", "MNUPROPS" + (CStr) StringNumberComplement(i, 3).Get_String(), TestFile);
        if(MenuEntryProps.Len() == 0) break;
        MenuEntryLevel = WAIniReadKey("MNUDAT", "MNULEVEL" + (CStr) StringNumberComplement(i, 3).Get_String(), TestFile);
        if(MenuEntryLevel.Len() == 0) break;
        MenuEntryTypeLng = MenuEntryType.Get_Long();
        MenuEntryLevelLng = MenuEntryLevel.Get_Long();
        MenuEntryPropsLng = MenuEntryProps.Get_Long();
        switch(MenuEntryTypeLng)
        {
            case MENUTYPE_BAR:
                // Attach bar
                if(LasthBar != 0)
                {
                    if(strcmpi(LastMenuStr.Get_String(), "<Empty entry>") == 0)
                    {
                        ItemType = 0;
                        AppendMenu(hMainMenu, MF_POPUP, (UINT) LasthBar, NULL);
                    }
                    else
                    {
                        ItemType = MFT_STRING;
                        AppendMenu(hMainMenu, MF_POPUP, (UINT) LasthBar, LastMenuStr.Get_String());
                    }
                    RtlZeroMemory(&InfosToChange, sizeof(InfosToChange));
                    ItemState = 0;
                    if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_RIGHTJUSTIFY) ItemType = ItemType | MFT_RIGHTJUSTIFY;
                    if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BARBREAK) ItemType = ItemType | MFT_MENUBARBREAK;
                    if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BREAK) ItemType = ItemType | MFT_MENUBREAK;
                    if((LasthBarProps & MENUPROP_GRAYED) != 0) ItemState = ItemState | MFS_GRAYED;
                    ItemTypeMask = MIIM_FTYPE;
                    InfosToChange.cbSize = sizeof(InfosToChange);
                    InfosToChange.fMask = ItemTypeMask | MIIM_STATE;
                    InfosToChange.fType = ItemType;
                    if(strcmpi(LastMenuStr.Get_String(), "<Empty entry>") == 0)
                    {
                        InfosToChange.cch = 0;
                        InfosToChange.dwTypeData = "";
                    }
                    else
                    {
                        InfosToChange.cch = sizeof(LastMenuStr);
                        InfosToChange.dwTypeData = LastMenuStr.Get_String();
                    }
                    InfosToChange.fState = ItemState;
                    SetMenuItemInfo(hMainMenu, (UINT) LasthBar, 0, &InfosToChange);
                }
                // Create bar popup
                LasthMenu = CreatePopupMenu();
                LasthBar = LasthMenu;
                LasthBarProps = MenuEntryPropsLng;
                LastMenuStr = MenuEntryName;
                PopupStack.Erase();
                LevelStack.Erase();
                LastBarPop = (long) LasthBar;
                BarPos = ItemNumber;
                ItemNumber++;
                PopupStack.Add(LasthBar);
                LevelStack.Add(MenuEntryLevelLng);
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_POPUP:
                for(j = (LevelStack.Amount() - 1); j >= 0; j--)
                {
                    if(LevelStack.Get(j)->Content == (MenuEntryLevelLng - 1)) break;
                }
                LasthMenu = PopupStack.Get(j)->Content;
                LasthMenuSub = CreatePopupMenu();
                if(strcmpi(MenuEntryName.Get_String(), "<Empty entry>") == 0)
                {
                    AppendMenu(LasthMenu, MF_POPUP, (UINT) LasthMenuSub, NULL);
                }
                else
                {
                    AppendMenu(LasthMenu, MF_POPUP, (UINT) LasthMenuSub, MenuEntryName.Get_String());
                }
                LasthMenu = LasthMenuSub;
                LastBarPop = (long) LasthMenu;
                ItemNumber++;
                PopupStack.Add(LasthMenu);
                LevelStack.Add(MenuEntryLevelLng);
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_ITEM:
                for(j = (LevelStack.Amount() - 1); j >= 0; j--)
                {
                    if(LevelStack.Get(j)->Content == (MenuEntryLevelLng - 1)) break;
                }
                LasthMenu = PopupStack.Get(j)->Content;
                if(strcmpi(MenuEntryName.Get_String(), "<Empty entry>") == 0)
                {
                    ItemType = 0;
                    AppendMenu(LasthMenu, MF_STRING, ItemNumber, NULL);
                }
                else
                {
                    ItemType = MFT_STRING;
                    AppendMenu(LasthMenu, MF_STRING, ItemNumber, NormalizeMenuString(&MenuEntryName).Get_String());
                }
                ItemState = 0;
                if((MenuEntryPropsLng & MENUPROP_CHECKED) != 0) ItemState = ItemState | MFS_CHECKED;
                if((MenuEntryPropsLng & MENUPROP_GRAYED) != 0) ItemState = ItemState | MFS_GRAYED;
                if((MenuEntryPropsLng & MENUPROP_BOLD) != 0) ItemState = ItemState | MFS_DEFAULT;
                if((MenuEntryPropsLng & MENUPROP_RADIO) != 0) ItemType = ItemType | MFT_RADIOCHECK;
                WAMenuSetItemState(LasthMenu, ItemState, ItemNumber);
                RtlZeroMemory(&InfosToChange, sizeof(InfosToChange));
                ItemTypeMask = MIIM_FTYPE;
                InfosToChange.cbSize = sizeof(InfosToChange);
                InfosToChange.fMask = ItemTypeMask;
                InfosToChange.fType = ItemType;
                if(strcmpi(MenuEntryName.Get_String(), "<Empty entry>") == 0)
                {
                    InfosToChange.cch = 0;
                    InfosToChange.dwTypeData = "";
                }
                else
                {
                    InfosToChange.cch = sizeof(MenuEntryName);
                    InfosToChange.dwTypeData = NormalizeMenuString(MenuEntryName).Get_String();
                }
                SetMenuItemInfo(LasthMenu, ItemNumber, 0, &InfosToChange);
                ItemNumber++;
                PopupStack.Add(LastBarPop);
                LevelStack.Add(MenuEntryLevelLng);
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
            case MENUTYPE_SEPS:
                for(j = (LevelStack.Amount() - 1); j >= 0; j--)
                {
                    if(LevelStack.Get(j)->Content == (MenuEntryLevelLng - 1)) break;
                }
                LasthMenu = PopupStack.Get(j)->Content;
                AppendMenu(LasthMenu, MF_SEPARATOR, -1, "-");
                ItemNumber++;
                PopupStack.Add(LastBarPop);
                LevelStack.Add(MenuEntryLevelLng);
                MenuEntryOldLevel = MenuEntryLevelLng;
				break;
        }
    }
    // Attach last bar popup to primary bar
    if(LasthBar != 0)
    {
        if(strcmpi(LastMenuStr.Get_String(), "<Empty entry>") == 0)
        {
            AppendMenu(hMainMenu, MF_POPUP, (UINT) LasthBar, NULL);
            ItemType = 0;
        }
        else
        {
            AppendMenu(hMainMenu, MF_POPUP, (UINT) LasthBar, LastMenuStr.Get_String());
            ItemType = MFT_STRING;
        }
        ItemState = 0;
        if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_RIGHTJUSTIFY) ItemType = ItemType | MFT_RIGHTJUSTIFY;
        if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BARBREAK) ItemType = ItemType | MFT_MENUBARBREAK;
        if(((LasthBarProps & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BREAK) ItemType = ItemType | MFT_MENUBREAK;
        if((LasthBarProps & MENUPROP_GRAYED) != 0) ItemState = ItemState | MFS_GRAYED;
        ItemTypeMask = MIIM_FTYPE;
        RtlZeroMemory(&InfosToChange, sizeof(InfosToChange));
        InfosToChange.cbSize = sizeof(InfosToChange);
        InfosToChange.fMask = ItemTypeMask | MIIM_STATE;
        InfosToChange.fType = ItemType;
        if(strcmpi(LastMenuStr.Get_String(), "<Empty entry>") == 0)
        {
            InfosToChange.cch = 0;
            InfosToChange.dwTypeData = "";
        }
        else
        {
            InfosToChange.cch = LastMenuStr.Len();
            InfosToChange.dwTypeData = LastMenuStr.Get_String();
        }
        InfosToChange.fState = ItemState;
        SetMenuItemInfo(hMainMenu, (UINT) LasthBar, 0, &InfosToChange);
    }
    DrawMenuBar(hwnd);
}

// -----------------------------------------------------------------------
// Replace menus resources shortcuts 
CStr NormalizeMenuString(CStr StrToCalc)
{
	CStr ReturnValue;

    ReturnValue = StringReplace(StrToCalc, "\\t", "\t", -1, -1, Text_Compare);
	return(ReturnValue);
}
