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
// Dlg_Properties.cpp: Properties form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Form.h"
#include "MsgCodeMax.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "Statusbar.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ConstCodeMax.h"
#include "Splitters.h"
#include "APIDb.h"
#include "Crypt.h"
#include "MDI_Childs.h"
#include "Dlg_Properties.h"
#include "Dlg_EnterValue.h"
#include "Dlg_Registry.h"

// -----------------------------------------------------------------------
// Constants
#define TOOLTIP_ID 1000

#define EXT_REPLACE 1
#define EXT_INSERT 2
#define EXT_DELETE 3

#define BACKUP_SELDIR 4
#define BACKUP_DELETE 5

#define PROFILE_LOAD 6
#define PROFILE_SAVE 7

#define FOLDER_CHOOSE 8

#define FTPACC_CREATE 9
#define FTPACC_SAVE 10
#define FTPACC_DELETE 11

#define DEBUGGER_VALID 12

// -----------------------------------------------------------------------
// Variables
HWND FRMPropertiesSysTab;
HWND FRMPropertiesCmdOk;
HWND FRMPropertiesCmdCancel;
HWND FRMPropertieshwnd;

// Controls (Page 1)
HWND FRMPropertiesPageEditor;
HWND FRMPropertiesPageFileExtensions;
HWND FRMPropertiesPageOther;
HWND FRMPropertiesPageFont;
HWND FRMPropertiesPageAutoSave;
HWND FRMPropertiesPageEditor2;
HWND FRMPropertiesPageLineNumbering;
HWND FRMPropertiesPageBackup;
HWND FRMPropertiesChkLeftMargin;
HWND FRMPropertiesChkWhiteSpaces;
HWND FRMPropertiesChkHighlightLine;
HWND FRMPropertiesChkCursorwrap;
HWND FRMPropertiesChkSyntaxHighlighting;
HWND FRMPropertiesChkLanguageScopeIndent;
HWND FRMPropertiesChkTextDragAndDrop;
HWND FRMPropertiesChkSmoothScrolling;
HWND FRMPropertiesChkLineNumberTooltip;
HWND FRMPropertiesChkCaretAlwaysVisible;
HWND FRMPropertiesChkTabsToSpaces;
HWND FRMPropertiesChkAutoCorrectKeywords;
HWND FRMPropertiesChkHorizontalSplitter;
HWND FRMPropertiesChkDetectChange;
HWND FRMPropertiesChkRememberState;
HWND FRMPropertiesChkUNIX;
HWND FRMPropertiesChkAutoZoom;
HWND FRMPropertiesChkOutputDates;
HWND FRMPropertiesCbLines;
HWND FRMPropertiesTxtLines;
HWND FRMPropertiesTxtFileExt;
HWND FRMPropertiesLbFileExt;
HWND FRMPropertiesTbFileExt;
HWND FRMPropertiesLbFont;
HWND FRMPropertiesCmdFontChoose;
HWND FRMPropertiesTabulations;
HWND FRMPropertiesRecentFiles;
HWND FRMPropertiesRecentProjects;
HWND FRMPropertiesCbSave;
HWND FRMPropertiesChkVerbose;
HWND FRMPropertiesChkSaveAll;
HWND FRMPropertiesChkMultiplesInst;
HWND FRMPropertiesChkRemember;
HWND FRMPropertiesChkBakFiles;
HWND FRMPropertiesChkDecorateBak;
HWND FRMPropertiesChkAcceptFiles;
HWND FRMPropertiesChkAutoMax;
HWND FRMPropertiesChkAutoClean;
HWND FRMPropertiesChkMinimize;
HWND FRMPropertiesChkOnTop;
HWND FRMPropertiesChkSaveBk;
HWND FRMPropertiesTxtBackupDir;
HWND FRMPropertiesTbBackup;

// Controls (Page 2)
HWND FRMPropertiesPageColors;
HWND FRMPropertiesPageAPIHelp;
HWND FRMPropertiesPageSDKHelp;
HWND FRMPropertiesPageCodeMax;
HWND FRMPropertiesPageDDKHelp;
HWND FRMPropertiesPageDirectXSDKHelp;
HWND FRMPropertiesPageExtrasLayouts;
HWND FRMPropertiesCodeMax;
HWND FRMPropertiesCbColors;
HWND FRMPropertiesTbColors;
HWND FRMPropertiesColBackGround;
HWND FRMPropertiesColLineNumber;
HWND FRMPropertiesColNumber;
HWND FRMPropertiesColStrings;
HWND FRMPropertiesColComments;
HWND FRMPropertiesColKeywords;
HWND FRMPropertiesColDirectives;
HWND FRMPropertiesColAPIHighlighting;
HWND FRMPropertiesColExtraHighLight1;
HWND FRMPropertiesColExtraHighLight2;
HWND FRMPropertiesColExtraHighLight3;
HWND FRMPropertiesColLuminosity;
HWND FRMPropertiesColToolTip;
HWND FRMPropertiesColNormalText;
HWND FRMPropertiesColMargin;
HWND FRMPropertiesColHighlightText;
HWND FRMPropertiesColRegisters;
HWND FRMPropertiesColExtraFunctions;
HWND FRMPropertiesColScopeKeywords;
HWND FRMPropertiesColSeparators;
HWND FRMPropertiesChkItalic;
HWND FRMPropertiesTxtWindowsAPI;
HWND FRMPropertiesCmdWindowsAPI;
HWND FRMPropertiesTxtMSDN;
HWND FRMPropertiesCmdMSDN;
HWND FRMPropertiesTxtDDK;
HWND FRMPropertiesCmdDDK;
HWND FRMPropertiesTxtDirectXSDK;
HWND FRMPropertiesCmdDirectXSDK;
HWND FRMPropertiesCbDefLang;
HWND FRMPropertiesCmdRegs;

// Controls (Page 3)
HWND FRMPropertiesPageFolders;
HWND FRMPropertiesPageProjects;
HWND FRMPropertiesPageProtoGen;
HWND FRMStepListView;
HWND FRMPropertiesTxtROOTDIR;
HWND FRMPropertiesTxtBINDIR;
HWND FRMPropertiesTxtINCLUDEDIR;
HWND FRMPropertiesTxtLIBDIR;
HWND FRMPropertiesTxtHELPDIR;
HWND FRMPropertiesTbROOTDIR;
HWND FRMPropertiesTbBINDIR;
HWND FRMPropertiesTbINCLUDEDIR;
HWND FRMPropertiesTbLIBDIR;
HWND FRMPropertiesTbHELPDIR;
HWND FRMPropertiesTxtPROJECTSDIR;
HWND FRMPropertiesTbPROJECTSDIR;
HWND FRMPropertiesTxtArgs;
HWND FRMPropertiesCbDebugger;
HWND FRMPropertiesCmdDebugger;
HWND FRMPropertiesTbValidDebugger;
HWND FRMPropertiesCbSkin;
HWND FRMPropertiesTxtGenProg;
HWND FRMPropertiesCmdGenProg;
HWND FRMPropertiesTxtGenSource;
HWND FRMPropertiesTxtGenDest;

// Controls (Page 4)
HWND FRMPropertiesPageFTPManage;
HWND FRMPropertiesPageAccountInfos;
HWND FRMPropertiesPageTransfersTypes;
HWND FRMPropertiesCbAccounts;
HWND FRMPropertiesTbAccounts;
HWND FRMPropertiesTxtIP;
HWND FRMPropertiesChkAnonymous;
HWND FRMPropertiesTxtUserName;
HWND FRMPropertiesTxtPassword;
HWND FRMPropertiesCbPasswordType;
HWND FRMPropertiesTxtInitialPath;
HWND FRMPropertiesTxtFTPPort;
HWND FRMPropertiesTxtFTPTimeOut;
HWND FRMPropertiesRBASCII;
HWND FRMPropertiesRBBinary;
HWND FRMPropertiesChkPassive;

// Controls (Page 5)
HWND FRMPropertiesPageCodingHelp;
HWND FRMPropertiesPageExtrasLayouts2;
HWND FRMPropertiesChkWinAPITooltipInfos;
HWND FRMPropertiesChkUseFncDB;
HWND FRMPropertiesChkUseConstDB;
HWND FRMPropertiesTxtAPILines;
HWND FRMPropertiesChkShowSplash;
HWND FRMPropertiesChkTVSingle;
HWND FRMPropertiesChkTVTool;
HWND FRMPropertiesChkRecentDoc;
HWND FRMPropertiesChkAutoOut;
HWND FRMPropertiesChkUseFileDir;
HWND FRMPropertiesChkCreateDrop;
HWND FRMPropertiesChkSaveProjectState;

// Other variables
CStr FRMPropertiesRetVal;
HFONT LblFont;

// Profiles variables
COLORREF Prof_BackColor;
COLORREF Prof_NormText;
COLORREF Prof_HighText;
COLORREF Prof_MarginColor;
COLORREF Prof_LinesColor;
COLORREF Prof_NumbersColorValue;
COLORREF Prof_StringsColorValue;
COLORREF Prof_CommentsColorValue;
// Scope keywords
COLORREF Prof_KeyWordsColorValue;
COLORREF Prof_APIHighColor;
// FRMPropertiesColKeywords
COLORREF Prof_ColorAValue;
// FRMPropertiesColSeparators
COLORREF Prof_ColorBValue;
// FRMPropertiesColDirectives
COLORREF Prof_ColorCValue;
// FRMPropertiesColRegisters
COLORREF Prof_ColorDValue;
// FRMPropertiesColExtraFunctions
COLORREF Prof_ColorEValue;
// FRMPropertiesColExtraHighLight1
COLORREF Prof_ColorFValue;
// FRMPropertiesColExtraHighLight2
COLORREF Prof_ColorGValue;
// FRMPropertiesColExtraHighLight3
COLORREF Prof_ColorHValue;

// Used for luminosity
COLORREF Orig_Prof_BackColor;
COLORREF Orig_Prof_NormText;
COLORREF Orig_Prof_HighText;
COLORREF Orig_Prof_MarginColor;
COLORREF Orig_Prof_LinesColor;
COLORREF Orig_Prof_NumbersColorValue;
COLORREF Orig_Prof_StringsColorValue;
COLORREF Orig_Prof_CommentsColorValue;
COLORREF Orig_Prof_KeyWordsColorValue;
COLORREF Orig_Prof_APIHighColor;
COLORREF Orig_Prof_ColorAValue;
COLORREF Orig_Prof_ColorBValue;
COLORREF Orig_Prof_ColorCValue;
COLORREF Orig_Prof_ColorDValue;
COLORREF Orig_Prof_ColorEValue;
COLORREF Orig_Prof_ColorFValue;
COLORREF Orig_Prof_ColorGValue;
COLORREF Orig_Prof_ColorHValue;

long Prof_ItalicComment;
long Prof_Luminosity;

long FRMPropertiesFirstTab;

CM_RANGE LineRange;
CStr NewAccountName;
long FRMPropertiesInFtpCreation;

CStr PrefsGenProg;
CStr PrefsGenCmd;
CStr PrefsGenDest;

CStr PrfCurrentSkin;
CList <char *> PrgSkinArray;
CList <HWND> PrgSkinLabels;
CList <HWND> PrgSkinControls;
CList <char *> PrgSkinControlsDest;
CList <HWND> SkinListViews;

HWND Assigned_ToolTip;

// -----------------------------------------------------------------------
// Private functions
LRESULT CALLBACK FRMPropertiesFrameGeneral(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameColors(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameExtensions(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesCMHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameCodeMax(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameManagment(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RadioHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameBuilding(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropStepListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddNewDebugger(HWND hCombo, CStr Debugger);

// -----------------------------------------------------------------------
// Display properties according to FRMPropertiesFirstTab variable
void DisplayProperties(void)
{
    WACreateModalDialog(-1, -1, 527, 370, hMDIform.hWnd, FRMPropertiesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    if(RefreshProperties == 1) RefreshChilds();
    if(NbForms != 0) RefreshStatusBar(CurrentForm);
    else ClearStatusBarParts();
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMPropertiesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    int i = 0;
	CStr BufString;
    long LbOldIdx = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMStepListView, GetSysColor(COLOR_WINDOW));
            for(i = 0; i < SkinListViews.Amount(); i++)
            {
                if(SkinListViews.Get(i)->Content != 0) WAListViewSetBackColor(SkinListViews.Get(i)->Content, GetSysColor(COLOR_WINDOW));
            }
			break;
        case WM_INITDIALOG:
            FRMPropertiesInFtpCreation = 0;
            RefreshProperties = 0;
            FRMPropertieshwnd = hwndDlg;
            WAControlSetText(hwndDlg, AppTitle + (CStr) " properties");
			FRMPropertiesSysTab = WACreateSysTab(2, 1, 520, 334, hwndDlg, 0, 0, GlobalImageList1, WS_TABSTOP | TCS_BUTTONS | TCS_FLATBUTTONS | TCS_HOTTRACK);
            WASysTabSetSeparators(FRMPropertiesSysTab, 0);
            WASysTabAddItem(FRMPropertiesSysTab, "General", 0, ICON_PROPERTIES);
            WASysTabAddItem(FRMPropertiesSysTab, "Editor layouts", 1, ICON_COLORS);
            WASysTabAddItem(FRMPropertiesSysTab, "Compiler/Assembler", 2, ICON_ASM);
            WASysTabAddItem(FRMPropertiesSysTab, "FTP accounts", 3, ICON_EARTH);
            WASysTabAddItem(FRMPropertiesSysTab, "Extras options", 4, ICON_STAR);
            CreatePropPage1(hwndDlg);
            CreatePropPage2(hwndDlg);
            CreatePropPage3(hwndDlg);
            CreatePropPage4(hwndDlg);
            CreatePropPage5(hwndDlg);
            BuildFTP(FRMPropertiesCbAccounts);
            FRMPropertiesCmdOk = WACreateButton(368, 344, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMPropertiesCmdCancel = WACreateButton(447, 344, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FillProfil();
            FillProperties();
            Fillskins();
            FillCurrentSkin();
            FillDefLanguage();
            FRMPropertiesInitCodeMax(FRMPropertiesCodeMax);
            WASysTabSetCurrentItem(FRMPropertiesSysTab, FRMPropertiesFirstTab);
            FRMPropertiesChangeCurrentTab();
            FreezeTimer = 1;
            return(1);
        case WM_SIZE:
            WAListBoxSetHorzScrollWidth(FRMPropertiesLbFileExt, WAControlWidth(FRMPropertiesLbFileExt) * 2);
			break;
		case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TCN_SELCHANGE:
                    FRMPropertiesChangeCurrentTab();
                    return(0);
                case TTN_NEEDTEXT:
					switch(WAControlGetNotifiedID(lParam))
					{
						case BACKUP_SELDIR:
							WAToolBarDisplayToolTip("Select new backup dir", lParam);
							return(0);
						case BACKUP_DELETE:
							WAToolBarDisplayToolTip("Empty selected dir", lParam);
							return(0);
                        case EXT_REPLACE:
                            WAToolBarDisplayToolTip("Replace entry", lParam);
                            return(0);
                        case EXT_INSERT:
                            WAToolBarDisplayToolTip("Insert new entry", lParam);
                            return(0);
                        case EXT_DELETE:
                            WAToolBarDisplayToolTip("Delete entry", lParam);
                            return(0);
                        case PROFILE_LOAD:
                            WAToolBarDisplayToolTip("Load selected colors profile", lParam);
                            return(0);
                        case PROFILE_SAVE:
                            WAToolBarDisplayToolTip("Save colors profile", lParam);
                            return(0);
                        case FOLDER_CHOOSE:
							WAToolBarDisplayToolTip("Select a directory", lParam);
							return(0);
                        case FTPACC_CREATE:
                            WAToolBarDisplayToolTip("Create a new account", lParam);
                            return(0);
                        case FTPACC_SAVE:
                            WAToolBarDisplayToolTip("Update current account", lParam);
                            return(0);
                        case FTPACC_DELETE:
                            WAToolBarDisplayToolTip("Delete current account", lParam);
                            return(0);
                        case DEBUGGER_VALID:
							WAToolBarDisplayToolTip("Add entry into list", lParam);
							return(0);
                    }
			}
			break;
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCmdOk)
            {
                if(FRMPropertiesInFtpCreation == 1)
                {
                    if(WAMiscMsgBox(hwndDlg, "Current FTP account not saved.\rSave it now ?", MB_QUESTION, Requesters) == IDYES) SaveFTPProps();
                }
                SavePrefs();
                RefreshProperties = 1;
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdMSDN)
            {
                GetHelpFile(FRMPropertiesTxtMSDN);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDDK)
            {
                GetHelpFile(FRMPropertiesTxtDDK);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdWindowsAPI)
            {
                GetHelpFile(FRMPropertiesTxtWindowsAPI);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDirectXSDK)
            {
                GetHelpFile(FRMPropertiesTxtDirectXSDK);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdRegs)
            {
                WACreateModalDialog(-1, -1, 334, 215, FRMPropertieshwnd, FRMRegsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdGenProg)
            {
				GetGenProtoProg();
				return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDebugger)
            {
                GetDebugger(FRMPropertiesCbDebugger);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbBackup)
            {
                switch(wParam)
                {
                    case BACKUP_SELDIR:
                        FRMPropertiesSelectBakDir();
                        return(0);
                    case BACKUP_DELETE:
                        WAControlSetText(FRMPropertiesTxtBackupDir, "");
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbFileExt)
            {
				switch(wParam)
				{
                    case EXT_REPLACE:
                        if(WAListBoxCount(FRMPropertiesLbFileExt) != 0)
                        {
                            LbOldIdx = WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                            if(WAControlGetText(FRMPropertiesTxtFileExt).Len() != 0)
                            {
                                if(WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                                {
                                    WAListBoxReplaceItem(FRMPropertiesLbFileExt, WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt), WAControlGetText(FRMPropertiesTxtFileExt));
                                    WAListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                    SetFocus(FRMPropertiesLbFileExt);
                                }
                            }
                        }
                        return(0);
                    case EXT_INSERT:
                        LbOldIdx = WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                        if(WAControlGetText(FRMPropertiesTxtFileExt).Len() != 0)
                        {
                            if(WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                            {
AddFileExt:						WAListBoxAddItem(FRMPropertiesLbFileExt, WAControlGetText(FRMPropertiesTxtFileExt), WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt));
                                WAListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                SetFocus(FRMPropertiesLbFileExt);
                            }
                            else
                            {
                                LbOldIdx = WAListBoxCount(FRMPropertiesLbFileExt);
                                WAListBoxSetIndex(FRMPropertiesLbFileExt, WAListBoxCount(FRMPropertiesLbFileExt));
                                goto AddFileExt;
                            }
                        }
                        return(0);
                    case EXT_DELETE:
                        LbOldIdx = WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                        if(WAListBoxCount(FRMPropertiesLbFileExt) != 0)
                        {
                            if(WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                            {
                                WAListBoxDeleteItem(FRMPropertiesLbFileExt, WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt));
                                if(WAListBoxCount(FRMPropertiesLbFileExt) != 0)
                                {
                                    if(LbOldIdx > (WAListBoxCount(FRMPropertiesLbFileExt) - 1)) LbOldIdx = WAListBoxCount(FRMPropertiesLbFileExt) - 1;
                                    if(LbOldIdx != -1) WAListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                    WAControlSetText(FRMPropertiesTxtFileExt, WAListBoxGetItem(FRMPropertiesLbFileExt, WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
                                    SetFocus(FRMPropertiesLbFileExt);
                                }
                                else
                                {
                                    WAControlSetText(FRMPropertiesTxtFileExt, "");
                                }
                            }
                            if(WAListBoxCount(FRMPropertiesLbFileExt) == 0) SetFocus(FRMPropertiesTxtFileExt);
                        }
                        else
                        {
                            SetFocus(FRMPropertiesTxtFileExt);
                        }
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbColors)
            {
                switch(wParam)
                {
                    case PROFILE_LOAD:
                        if(WAControlGetText(FRMPropertiesCbColors).Len() == 0)
                        {
                            WAMiscMsgBox(FRMPropertieshwnd, "Select a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMPropertiesCbColors);
                        }
                        else
                        {
                            LoadColorsProfile(WAControlGetText(FRMPropertiesCbColors));
                        }
                        return(0);
                    case PROFILE_SAVE:
                        if(SaveColorsProfile(WAControlGetText(FRMPropertiesCbColors)) == 0)
                        {
                            WAMiscMsgBox(FRMPropertieshwnd, "Enter a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMPropertiesCbColors);
                        }
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbROOTDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtROOTDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbBINDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtBINDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbINCLUDEDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtINCLUDEDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbLIBDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtLIBDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbHELPDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtHELPDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbPROJECTSDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtPROJECTSDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbValidDebugger)
            {
                // Add teh entry into the list
                BufString = WAControlGetText(FRMPropertiesCbDebugger);
                WAComboBoxSaveInIniFile(FRMPropertiesCbDebugger, BufString, "ExtDebuggers", MainIniFile);
                AddNewDebugger(FRMPropertiesCbDebugger, BufString);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbAccounts)
            {
                switch(wParam)
                {
                    // New FTP account
                    case FTPACC_CREATE:
                        FRMPropertiesInFtpCreation = 0;
                        // Ask user for a new account name
                        NewAccountName = MiscInputBox(FRMPropertieshwnd, "Enter new FTP account name", ICON_ASKDIR, "", 0, INPUTBOX_SIMPLETEXT, "");
                        if(NewAccountName.Len() != 0)
                        {
                            if(WAComboBoxItemExist(FRMPropertiesCbAccounts, NewAccountName) != -1)
                            {
                                WAMiscMsgBox(FRMPropertieshwnd, "This account already exists.", MB_ERROR, Requesters);
                                return(0);
                            }
                            WAComboBoxSetIndex(FRMPropertiesCbAccounts, WAComboBoxAddItem(FRMPropertiesCbAccounts, NewAccountName, -1));
                            ResetFTPProps();
                            FRMPropertiesInFtpCreation = 1;
                        }
                        SetFocus(FRMPropertiesTxtIP);
                        return(0);
                    // Save FTP account
                    case FTPACC_SAVE:
                        SaveFTPProps();
                        FRMPropertiesInFtpCreation = 0;
                        return(0);
                    // Delete FTP account
                    case FTPACC_DELETE:
                        if(WAComboBoxCount(FRMPropertiesCbAccounts) != 0)
                        {
                            if(WAMiscMsgBox(FRMPropertieshwnd, "Do you really want to delete this account ?", MB_QUESTION, Requesters) == IDYES) {
                                DeleteAccountFromIni(WAControlGetText(FRMPropertiesCbAccounts));
                                WAComboBoxDeleteItem(FRMPropertiesCbAccounts, WAComboBoxGetIndex(FRMPropertiesCbAccounts));
                                if(WAComboBoxCount(FRMPropertiesCbAccounts) != 0)
                                {
                                    WAComboBoxSetIndex(FRMPropertiesCbAccounts, 0);
                                    FillFTPProps();
                                }
                                else
                                {
                                    ResetFTPProps();
                                    WAComboBoxSetIndex(FRMPropertiesCbAccounts, -1);
                                }
                            }
                        }
                        FRMPropertiesInFtpCreation = 0;
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesCmdFontChoose)
            {
                FRMPropertiesSelectFont();
                return(0);
            }
			break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            WAGDIDrawHorzSep(hwndDlg, 0, 337, 527);
            EndPaint(hwndDlg, &CreatePs);
			break;
		case WM_CLOSE:
            if(LblFont != 0) DeleteObject(LblFont);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            FocusOnActiveChild();
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Create first properties page
void CreatePropPage1(HWND hParent)
{
    FRMPropertiesPageEditor = WACreateFrame(4, 26, 155, 306, hParent, "Editor", 0, 0, 0);
    FRMPropertiesPageFileExtensions = WACreateFrame(161, 26, 232, 183, hParent, "Files extensions", 0, &FRMPropertiesFrameExtensions, 0);
    FRMPropertiesPageFont = WACreateFrame(161, 209, 232, 41, hParent, "Font", 0, 0, 0);
    FRMPropertiesPageLineNumbering = WACreateFrame(161, 250, 232, 41, hParent, "Lines numbering", 0, 0, 0);
    FRMPropertiesPageOther = WACreateFrame(395, 26, 128, 72, hParent, "Other", 0, 0, 0);
    FRMPropertiesPageAutoSave = WACreateFrame(395, 98, 128, 56, hParent, "AutoSave", 0, 0, 0);
    FRMPropertiesPageEditor2 = WACreateFrame(395, 154, 128, 178, hParent, "Editor", 0, &FRMPropertiesFrameGeneral, 0);
    FRMPropertiesPageBackup = WACreateFrame(161, 291, 232, 41, hParent, "Default backup directory", 0, 0, 0);
    FRMPropertiesChkLeftMargin = WACreateCheckBox(11, 21 - 7, 140, 15, FRMPropertiesPageEditor, "Left margin", 11, 0, WS_TABSTOP | WS_GROUP, 0);
    FRMPropertiesChkWhiteSpaces = WACreateCheckBox(11, 37 - 7, 140, 15, FRMPropertiesPageEditor, "White spaces", 12, 0, WS_TABSTOP, 0);
    FRMPropertiesChkHighlightLine = WACreateCheckBox(11, 53 - 7, 140, 15, FRMPropertiesPageEditor, "Highlight selected line", 13, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCursorwrap = WACreateCheckBox(11, 69 - 7, 140, 15, FRMPropertiesPageEditor, "Cursor wrap", 14, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSyntaxHighlighting = WACreateCheckBox(11, 85 - 7, 140, 15, FRMPropertiesPageEditor, "Syntax highlighting", 15, 0, WS_TABSTOP, 0);
    FRMPropertiesChkLanguageScopeIndent = WACreateCheckBox(11, 101 - 7, 140, 15, FRMPropertiesPageEditor, "Language scope indent", 16, 0, WS_TABSTOP, 0);
    FRMPropertiesChkTextDragAndDrop = WACreateCheckBox(11, 117 - 7, 140, 15, FRMPropertiesPageEditor, "Text drag and drop", 17, 0, WS_TABSTOP, 0);
    CheckBoxSetState(FRMPropertiesChkTextDragAndDrop, 1);
    FRMPropertiesChkSmoothScrolling = WACreateCheckBox(11, 133 - 7, 140, 15, FRMPropertiesPageEditor, "Smooth scrolling", 18, 0, WS_TABSTOP, 0);
    FRMPropertiesChkLineNumberTooltip = WACreateCheckBox(11, 149 - 7, 140, 15, FRMPropertiesPageEditor, "Line number tooltip", 19, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCaretAlwaysVisible = WACreateCheckBox(11, 165 - 7, 140, 15, FRMPropertiesPageEditor, "Caret always visible", 20, 0, WS_TABSTOP, 0);
    FRMPropertiesChkTabsToSpaces = WACreateCheckBox(11, 181 - 7, 140, 15, FRMPropertiesPageEditor, "Tabs to spaces", 21, 0, WS_TABSTOP, 0);
    FRMPropertiesChkHorizontalSplitter = WACreateCheckBox(11, 197 - 7, 140, 15, FRMPropertiesPageEditor, "Horizontal splitter", 23, 0, WS_TABSTOP, 0);
    FRMPropertiesChkDetectChange = WACreateCheckBox(11, 213- 7, 140, 15, FRMPropertiesPageEditor, "File change detection", 24, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRememberState = WACreateCheckBox(11, 229 - 7, 140, 15, FRMPropertiesPageEditor, "Remember last state", 25, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUNIX = WACreateCheckBox(11, 245 - 7, 140, 15, FRMPropertiesPageEditor, "Auto convert Unix files", 26, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoZoom = WACreateCheckBox(11, 261 - 7, 140, 15, FRMPropertiesPageEditor, "Auto focus opened files", 27, 0, WS_TABSTOP, 0);
    FRMPropertiesChkOutputDates = WACreateCheckBox(11, 277 - 7, 140, 15, FRMPropertiesPageEditor, "Dates prefix in output", 28, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveAll = WACreateCheckBox(11, 293 - 7, 140, 15, FRMPropertiesPageEditor, "Save files on user cmd", 29, 0, WS_TABSTOP, 0);
    FRMPropertiesCbLines = WACreateComboBox(39, 15, 84, 150, FRMPropertiesPageLineNumbering, "", 30, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    WAComboBoxAddItem(FRMPropertiesCbLines, "(None)", -1);
    WAComboBoxAddItem(FRMPropertiesCbLines, "Binary", -1);
    WAComboBoxAddItem(FRMPropertiesCbLines, "Decimal", -1);
    WAComboBoxAddItem(FRMPropertiesCbLines, "Hexadecimal", -1);
    WAComboBoxAddItem(FRMPropertiesCbLines, "Octal", -1);
    WAComboBoxSetIndex(FRMPropertiesCbLines, 0);
    WACreateLabel(5, 18, 33, 15, FRMPropertiesPageLineNumbering, "Style :", 0, 0, 0, 0);
    WACreateLabel(126, 18, 44, 15, FRMPropertiesPageLineNumbering, "Start at :", 0, 0, 0, 0);
    FRMPropertiesTxtLines = WACreateTextBox(172, 15, 54, 20, FRMPropertiesPageLineNumbering, "", 31, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesTxtFileExt = WACreateTextBox(5, 15, 222, 20, FRMPropertiesPageFileExtensions, "", 32, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesLbFileExt = WACreateListBox(4, 36, 197, 143, FRMPropertiesPageFileExtensions, 33, 0, 0, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbFileExt = WACreateToolBar(203 + 161, 37 + 26, 25, 72, hParent, GlobalImageList1, 34, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    WAToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_REPLACE, ICON_REPLACE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_INSERT, ICON_INSERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    FRMPropertiesLbFont = WACreateLabel(7, 17, 195, 15, FRMPropertiesPageFont, "", 0, 0, 0, 0);
	FRMPropertiesCmdFontChoose = WACreateButton(202 + 161, 14 + 209, 25, 21, hParent, "...", 35, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
	WACreateLabel(27, 12, 63, 15, FRMPropertiesPageOther, "Tabulations :", 0, 0, 0, 0);
    FRMPropertiesTabulations = WACreateTextBox(92, 10, 27, 20, FRMPropertiesPageOther, "", 36, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    WACreateLabel(26, 31, 63, 15, FRMPropertiesPageOther, "Recent files :", 0, 0, 0, 0);
    FRMPropertiesRecentFiles = WACreateTextBox(92, 29, 27, 20, FRMPropertiesPageOther, "", 37, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    WACreateLabel(7, 50, 84, 15, FRMPropertiesPageOther, "Recent projects :", 0, 0, 0, 0);
    FRMPropertiesRecentProjects = WACreateTextBox(92, 48, 27, 20, FRMPropertiesPageOther, "", 38, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesCbSave = WACreateComboBox(7, 14, 114, 150, FRMPropertiesPageAutoSave, "", 39, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    WAComboBoxAddItem(FRMPropertiesCbSave, "(None)", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "1 minute", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "2 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "5 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "10 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "15 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "30 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "45 minutes", -1);
    WAComboBoxAddItem(FRMPropertiesCbSave, "1 hour", -1);
    WAComboBoxSetIndex(FRMPropertiesCbSave, 0);
    FRMPropertiesChkVerbose = WACreateCheckBox(7, 37, 110, 15, FRMPropertiesPageAutoSave, "Display files list", 40, 0, WS_TABSTOP, 0);
	FRMPropertiesChkMultiplesInst = WACreateCheckBox(5, 9 + 5, 120, 15, FRMPropertiesPageEditor2, "Multiple instances", 41, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRemember = WACreateCheckBox(5, 25 + 5, 120, 15, FRMPropertiesPageEditor2, "Remember last pos", 42, 0, WS_TABSTOP, 0);
    FRMPropertiesChkBakFiles = WACreateCheckBox(5, 41 + 5, 120, 15, FRMPropertiesPageEditor2, "Create .BAK files", 43, 0, WS_TABSTOP, 0);
    FRMPropertiesChkDecorateBak = WACreateCheckBox(5, 57 + 5, 120, 15, FRMPropertiesPageEditor2, "Decorate .BAK files", 44, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAcceptFiles = WACreateCheckBox(5, 73 + 5, 120, 15, FRMPropertiesPageEditor2, "Accept files", 45, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoMax = WACreateCheckBox(5, 89 + 5, 118, 15, FRMPropertiesPageEditor2, "Auto maximize childs", 46, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoClean = WACreateCheckBox(5, 105 + 5, 118, 15, FRMPropertiesPageEditor2, "Auto clear output", 47, 0, WS_TABSTOP, 0);
    FRMPropertiesChkMinimize = WACreateCheckBox(5, 121 + 5, 118, 15, FRMPropertiesPageEditor2, "Minimize to systray", 48, 0, WS_TABSTOP, 0);
    FRMPropertiesChkOnTop = WACreateCheckBox(5, 137 + 5, 118, 15, FRMPropertiesPageEditor2, "Always on top", 49, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveBk = WACreateCheckBox(5, 153 + 5, 118, 15, FRMPropertiesPageEditor2, "Save bookmarks", 50, 0, WS_TABSTOP, 0);
	FRMPropertiesTxtBackupDir = WACreateTextBox(6, 14, 169, 20, FRMPropertiesPageBackup, "", 50, 0, ES_READONLY | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesTbBackup = WACreateToolBar(177 + 161, 13 + 291, 50, 24, hParent, GlobalImageList1, 51, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    WAToolBarAddButton(FRMPropertiesTbBackup, "", BACKUP_SELDIR, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbBackup, "", BACKUP_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
}

// -----------------------------------------------------------------------
// Create second properties page
void CreatePropPage2(HWND hParent)
{
    FRMPropertiesPageColors = WACreateFrame(4, 26, 257, 210, hParent, "Editor colors", 0, &FRMPropertiesFrameColors, 0);
    FRMPropertiesPageAPIHelp = WACreateFrame(263, 26, 260, 43, hParent, "Windows API help", 0, 0, 0);
    FRMPropertiesPageSDKHelp = WACreateFrame(263, 69, 260, 43, hParent, "MSDN/Platform SDK help", 0, 0, 0);
    FRMPropertiesPageDDKHelp = WACreateFrame(263, 112, 260, 43, hParent, "Windows DDK help", 0, 0, 0);
    FRMPropertiesPageDirectXSDKHelp = WACreateFrame(263, 155, 260, 43, hParent, "DirectX SDK help", 0, 0, 0);
    FRMPropertiesPageExtrasLayouts = WACreateFrame(263, 198, 260, 38, hParent, "Extra layouts", 0, 0, 0);
    FRMPropertiesPageCodeMax = WACreateFrame(6 - 2, 235, 518, 97, hParent, "", 0, &FRMPropertiesFrameCodeMax, 0);
    FRMPropertiesCodeMax = CreateCodeMax(0, 4, 518, 93, FRMPropertiesPageCodeMax, ApphInstance, 0);
    WAControlHookWin(FRMPropertiesCodeMax, &FRMPropertiesCMHook);
    FRMPropertiesCbColors = WACreateComboBox(7, 14, 116, 150, FRMPropertiesPageColors, "", 0, 0, CBS_DROPDOWN | WS_TABSTOP);
    FRMPropertiesTbColors = WACreateToolBar(125 + 4 , 13 + 26, 50, 24, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    WAToolBarAddButton(FRMPropertiesTbColors, "", PROFILE_LOAD, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbColors, "", PROFILE_SAVE, ICON_SAVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WACreateLabel(7, 39, 97, 15, FRMPropertiesPageColors, "Background :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 1), 97, 15, FRMPropertiesPageColors, "Lines numbers :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 2), 97, 15, FRMPropertiesPageColors, "Numbers :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 3), 97, 15, FRMPropertiesPageColors, "Strings :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 4), 97, 15, FRMPropertiesPageColors, "Comments :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 5), 97, 15, FRMPropertiesPageColors, "Keywords :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 6), 97, 15, FRMPropertiesPageColors, "Directives :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 7), 97, 15, FRMPropertiesPageColors, "API highlighting :", 0, 0, 0, 0);
    WACreateLabel(7, 39 + (16 * 8), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    WACreateLabel(131, 39, 97, 15, FRMPropertiesPageColors, "Normal text :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 1), 97, 15, FRMPropertiesPageColors, "Margin :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 2), 97, 15, FRMPropertiesPageColors, "Highlight text :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 3), 97, 15, FRMPropertiesPageColors, "Registers :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 4), 97, 15, FRMPropertiesPageColors, "Extra functions :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 5), 97, 15, FRMPropertiesPageColors, "Scope keywords :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 6), 97, 15, FRMPropertiesPageColors, "Separators :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 7), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    WACreateLabel(131, 39 + (16 * 8), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    FRMPropertiesColBackGround = WACreateColorBox(96, 38, 26, 15, FRMPropertiesPageColors, 2, 0, 0);
    FRMPropertiesColLineNumber = WACreateColorBox(96, 38 + (16 * 1), 26, 15, FRMPropertiesPageColors, 3, 0, 0);
    FRMPropertiesColNumber = WACreateColorBox(96, 38 + (16 * 2), 26, 15, FRMPropertiesPageColors, 4, 0, 0);
    FRMPropertiesColStrings = WACreateColorBox(96, 38 + (16 * 3), 26, 15, FRMPropertiesPageColors, 5, 0, 0);
    FRMPropertiesColComments = WACreateColorBox(96, 38 + (16 * 4), 26, 15, FRMPropertiesPageColors, 6, 0, 0);
    FRMPropertiesColKeywords = WACreateColorBox(96, 38 + (16 * 5), 26, 15, FRMPropertiesPageColors, 7, 0, 0);
    FRMPropertiesColDirectives = WACreateColorBox(96, 38 + (16 * 6), 26, 15, FRMPropertiesPageColors, 8, 0, 0);
    FRMPropertiesColAPIHighlighting = WACreateColorBox(96, 38 + (16 * 7), 26, 15, FRMPropertiesPageColors, 9, 0, 0);
    FRMPropertiesColExtraHighLight1 = WACreateColorBox(96, 38 + (16 * 8), 26, 15, FRMPropertiesPageColors, 10, 0, 0);
    FRMPropertiesColNormalText = WACreateColorBox(221, 38, 26, 15, FRMPropertiesPageColors, 11, 0, 0);
    FRMPropertiesColMargin = WACreateColorBox(221, 38 + (16 * 1), 26, 15, FRMPropertiesPageColors, 12, 0, 0);
    FRMPropertiesColHighlightText = WACreateColorBox(221, 38 + (16 * 2), 26, 15, FRMPropertiesPageColors, 13, 0, 0);
    FRMPropertiesColRegisters = WACreateColorBox(221, 38 + (16 * 3), 26, 15, FRMPropertiesPageColors, 14, 0, 0);
    FRMPropertiesColExtraFunctions = WACreateColorBox(221, 38 + (16 * 4), 26, 15, FRMPropertiesPageColors, 15, 0, 0);
    FRMPropertiesColScopeKeywords = WACreateColorBox(221, 38 + (16 * 5), 26, 15, FRMPropertiesPageColors, 16, 0, 0);
    FRMPropertiesColSeparators = WACreateColorBox(221, 38 + (16 * 6), 26, 15, FRMPropertiesPageColors, 17, 0, 0);
    FRMPropertiesColExtraHighLight2 = WACreateColorBox(221, 38 + (16 * 7), 26, 15, FRMPropertiesPageColors, 18, 0, 0);
    FRMPropertiesColExtraHighLight3 = WACreateColorBox(221, 38 + (16 * 8), 26, 15, FRMPropertiesPageColors, 19, 0, 0);
    FRMPropertiesColLuminosity = WACreateTrackBar(2, 187, 152, 20, FRMPropertiesPageColors, 20, 0, 0, 200, TBS_NOTICKS | TBS_TOOLTIPS | TBS_BOTH);
    WATrackBarSetPos(FRMPropertiesColLuminosity, 100);
	Assigned_ToolTip = WATrackBarGetToolTips(FRMPropertiesColLuminosity);
	FRMPropertiesChkItalic = WACreateCheckBox(155, 188, 97, 15, FRMPropertiesPageColors, "Italic comments", 20, 0, WS_TABSTOP, 0);
    FRMPropertiesTxtWindowsAPI = WACreateTextBox(5, 16, 223, 20, FRMPropertiesPageAPIHelp, "", 21, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesCmdWindowsAPI = WACreateButton(230 + 263, 16 + 26, 25, 21, hParent, "...", 22, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FRMPropertiesTxtMSDN = WACreateTextBox(5, 16, 223, 20, FRMPropertiesPageSDKHelp, "", 23, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdMSDN = WACreateButton(230 + 263, 16 + 69, 25, 21, hParent, "...", 24, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FRMPropertiesTxtDDK = WACreateTextBox(5, 16, 223, 20, FRMPropertiesPageDDKHelp, "", 25, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdDDK = WACreateButton(230 + 263, 16 + 112, 25, 21, hParent, "...", 26, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FRMPropertiesTxtDirectXSDK = WACreateTextBox(5, 16, 223, 20, FRMPropertiesPageDirectXSDKHelp, "", 27, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdDirectXSDK = WACreateButton(230 + 263, 16 + 155, 25, 21, hParent, "...", 28, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    WACreateLabel(5, 15, 97, 15, FRMPropertiesPageExtrasLayouts, "Default language :", 0, 0, 0, 0);
    FRMPropertiesCbDefLang = WACreateComboBox(100, 12, 87, 150, FRMPropertiesPageExtrasLayouts, "", 29, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    FRMPropertiesCmdRegs = WACreateButton(189 + 263, 12 + 198, 66, 21, hParent, "Extensions", 30, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
}

// -----------------------------------------------------------------------
// Create third properties page
void CreatePropPage3(HWND hParent)
{
    FRMPropertiesPageFolders = WACreateFrame(4, 26, 275, 212, hParent, "Select folders", 0, 0, 0);
    FRMPropertiesPageProjects = WACreateFrame(281, 26, 242, 306, hParent, "Project building", 0, &FRMPropertiesFrameBuilding, 0);
    FRMPropertiesPageProtoGen = WACreateFrame(4, 238, 275, 94, hParent, "Prototypes generator", 0, 0, 0);
    WACreateLabel(6, 13, 236, 15, FRMPropertiesPageFolders, "Main directory [ROOTDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtROOTDIR = WACreateTextBox(6, 26, 236, 20, FRMPropertiesPageFolders, "", 1, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	
    FRMPropertiesTbROOTDIR = WACreateToolBar(244 + 4, 25 + 26, 25, 23, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbROOTDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	
    WACreateLabel(6, 13 + 32, 236, 15, FRMPropertiesPageFolders, "Bin directory [BINDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtBINDIR = WACreateTextBox(6, 26 + 32, 236, 20, FRMPropertiesPageFolders, "", 3, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesTbBINDIR = WACreateToolBar(244 + 4, 25 + 26 + 32, 25, 23, hParent, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbBINDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	WACreateLabel(6, 13 + (32 * 2), 236, 15, FRMPropertiesPageFolders, "Include directory [INCLUDEDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtINCLUDEDIR = WACreateTextBox(6, 26 + (32 * 2), 236, 20, FRMPropertiesPageFolders, "", 5, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesTbINCLUDEDIR = WACreateToolBar(244 + 4, 25 + 26 + (32 * 2), 25, 23, hParent, GlobalImageList1, 6, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbINCLUDEDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	WACreateLabel(6, 13 + (32 * 3), 236, 15, FRMPropertiesPageFolders, "Lib/Units directory [LIBDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtLIBDIR = WACreateTextBox(6, 26 + (32 * 3), 236, 20, FRMPropertiesPageFolders, "", 7, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesTbLIBDIR = WACreateToolBar(244 + 4, 25 + 26 + (32 * 3), 25, 23, hParent, GlobalImageList1, 8, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbLIBDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	WACreateLabel(6, 13 + (32 * 4), 236, 15, FRMPropertiesPageFolders, "Help directory [HELPDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtHELPDIR = WACreateTextBox(6, 26 + (32 * 4), 236, 20, FRMPropertiesPageFolders, "", 9, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesTbHELPDIR = WACreateToolBar(244 + 4, 25 + 26 + (32 * 4), 25, 23, hParent, GlobalImageList1, 10, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbHELPDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	WACreateLabel(6, 13 + (32 * 5), 236, 15, FRMPropertiesPageFolders, "Projects directory [PROJECTSDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtPROJECTSDIR = WACreateTextBox(6, 26 + (32 * 5), 236, 20, FRMPropertiesPageFolders, "", 11, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbPROJECTSDIR = WACreateToolBar(244 + 4, 25 + 26 + (32 * 5), 25, 23, hParent, GlobalImageList1, 12, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbPROJECTSDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
	WACreateLabel(6, 20, 106, 15, FRMPropertiesPageProtoGen, "Program :", 0, 0, 0, 0);
    FRMPropertiesTxtGenProg = WACreateTextBox(58, 17, 183, 20, FRMPropertiesPageProtoGen, "", 13, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
	FRMPropertiesCmdGenProg = WACreateButton(243 + 4, 17 + 238, 25, 21, hParent, "...", 14, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    WACreateLabel(6, 20 + 25, 106, 15, FRMPropertiesPageProtoGen, "Source :", 0, 0, 0, 0);
    FRMPropertiesTxtGenSource = WACreateTextBox(58, 17 + 25, 210, 20, FRMPropertiesPageProtoGen, "", 15, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    WACreateLabel(6, 20 + 25 + 25, 106, 15, FRMPropertiesPageProtoGen, "Dest. :", 0, 0, 0, 0);
    FRMPropertiesTxtGenDest = WACreateTextBox(58, 17 + 25 + 25, 210, 20, FRMPropertiesPageProtoGen, "", 16, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    WACreateLabel(7, 13, 145, 15, FRMPropertiesPageProjects, "Debugger :", 0, 0, 0, 0);
    FRMPropertiesCbDebugger = WACreateComboBox(7, 28, 177, 150, FRMPropertiesPageProjects, "", 17, 0, CBS_DROPDOWN | WS_TABSTOP);
    WAComboBoxFillFromIniFile(FRMPropertiesCbDebugger, "ExtDebuggers", MainIniFile);
    FRMPropertiesCmdDebugger = WACreateButton(177 + 289, 28 + 26, 25, 21, hParent, "...", 18, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FRMPropertiesTbValidDebugger = WACreateToolBar(181 + 289 + 22, 25 + 28, 25, 23, hParent, GlobalImageList1, 19, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbValidDebugger, "", DEBUGGER_VALID, ICON_INSERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WACreateLabel(7, 52, 131, 15, FRMPropertiesPageProjects, "Run arguments :", 0, 0, 0, 0);
    FRMPropertiesTxtArgs = WACreateTextBox(7, 65, 130, 20, FRMPropertiesPageProjects, "", 20, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    WACreateLabel(7, 88, 131, 15, FRMPropertiesPageProjects, "Building skin :", 0, 0, 0, 0);
    FRMPropertiesCbSkin = WACreateComboBox(7, 103, 130, 150, FRMPropertiesPageProjects, "", 21, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    FRMStepListView = WACreateListView(140, 52, 95, 72, FRMPropertiesPageProjects, 22, 0, &PropStepListViewProc, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
    WAListViewAddCol(FRMStepListView, "", 76, 0);
    WAListViewAddItem(FRMStepListView, "Build Res.", 0, -1);
    WAListViewAddItem(FRMStepListView, "Compile", 1, -1);
    WAListViewAddItem(FRMStepListView, "Link", 2, -1);
    WAListViewAddItem(FRMStepListView, "Cleanup", 3, -1);
}

// -----------------------------------------------------------------------
// Create fourth properties page
void CreatePropPage4(HWND hParent)
{
    FRMPropertiesPageFTPManage = WACreateFrame(4, 26, 519, 49, hParent, "Managment", 0, &FRMPropertiesFrameManagment, 0);
    FRMPropertiesPageAccountInfos = WACreateFrame(4, 75, 303, 170 + 17 + 8 + 20, hParent, "Account informations", 0, &FRMPropertiesFrameMain, 0);
    FRMPropertiesPageTransfersTypes = WACreateFrame(318, 75, 205, 69, hParent, "Transfers type", 0, 0, 0);
    WACreateLabel(9, 20, 78, 15, FRMPropertiesPageFTPManage, "Account name :", 0, 0, 0, 0);
    FRMPropertiesCbAccounts = WACreateComboBox(90, 17, 152, 150, FRMPropertiesPageFTPManage, "", 0, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
	FRMPropertiesTbAccounts = WACreateToolBar(244 + 4, 16 + 26, 84, 23, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    WAToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_CREATE, ICON_ASKDIR, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_SAVE, ICON_SAVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WAToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    WACreateLabel(9, 20, 116, 15, FRMPropertiesPageAccountInfos, "Domain or IP address :", 0, 0, 0, 0);
    WACreateLabel(9, 45 + 17, 116, 15, FRMPropertiesPageAccountInfos, "User name :", 0, 0, 0, 0);
    WACreateLabel(9, 71 + 17, 116, 15, FRMPropertiesPageAccountInfos, "Password :", 0, 0, 0, 0);
    WACreateLabel(9, 95 + 17, 116, 15, FRMPropertiesPageAccountInfos, "Save password :", 0, 0, 0, 0);
    WACreateLabel(9, 112 + 17 + 8, 116, 15, FRMPropertiesPageAccountInfos, "Initial path :", 0, 0, 0, 0);
    WACreateLabel(9, 137 + 17 + 8, 56, 15, FRMPropertiesPageAccountInfos, "FTP port :", 0, 0, 0, 0);
    FRMPropertiesTxtIP = WACreateTextBox(124, 17, 170, 20, FRMPropertiesPageAccountInfos, "", 2, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesChkAnonymous = WACreateCheckBox(124, 41, 129, 15, FRMPropertiesPageAccountInfos, "Anonymous login", 3, 0, WS_TABSTOP, 0);
    FRMPropertiesTxtUserName = WACreateTextBox(124, 42 + 17, 170, 20, FRMPropertiesPageAccountInfos, "", 4, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesTxtPassword = WACreateTextBox(124, 67 + 17, 170, 20, FRMPropertiesPageAccountInfos, "", 5, 0, ES_AUTOHSCROLL | WS_TABSTOP | ES_PASSWORD, WS_EX_STATICEDGE);
    FRMPropertiesCbPasswordType = WACreateComboBox(205, 92 + 17, 89, 150, FRMPropertiesPageAccountInfos, "", 0, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    WAComboBoxAddItem(FRMPropertiesCbPasswordType, "Don't save", -1);
    WAComboBoxAddItem(FRMPropertiesCbPasswordType, "Plain text", -1);
    WAComboBoxAddItem(FRMPropertiesCbPasswordType, "Encrypted", -1);
    FRMPropertiesTxtInitialPath = WACreateTextBox(124, 109 + 17 + 8, 170, 20, FRMPropertiesPageAccountInfos, "", 7, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesTxtFTPPort = WACreateTextBox(249, 134 + 17 + 8, 45, 20, FRMPropertiesPageAccountInfos, "", 8, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    WACreateLabel(9, 162 + 17 + 8, 116, 15, FRMPropertiesPageAccountInfos, "Connection timeout :", 0, 0, 0, 0);
    FRMPropertiesTxtFTPTimeOut = WACreateTextBox(249, 159 + 17 + 8, 45, 20, FRMPropertiesPageAccountInfos, "", 9, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesRBASCII = WACreateRadioButton(21, 21, 75, 17, FRMPropertiesPageTransfersTypes, "ASCII type", 10, 0, WS_TABSTOP | WS_GROUP);
    FRMPropertiesRBBinary = WACreateRadioButton(105, 21, 81, 17, FRMPropertiesPageTransfersTypes, "Binary type", 11, 0, WS_TABSTOP);
    FRMPropertiesChkPassive = WACreateCheckBox(21, 42, 103, 15, FRMPropertiesPageTransfersTypes, "Passive transfers", 12, 0, WS_TABSTOP, 0);
    RadioButtonSetState(FRMPropertiesRBBinary, 1);
}

// -----------------------------------------------------------------------
// Create fifth properties page
void CreatePropPage5(HWND hParent)
{
    FRMPropertiesPageCodingHelp = WACreateFrame(4, 26, 185, 136, hParent, "Coding help", 0, 0, 0);
    FRMPropertiesPageExtrasLayouts2 = WACreateFrame(4, 162, 185, 170, hParent, "Extras layouts", 0, 0, 0);
	FRMPropertiesChkAutoCorrectKeywords = WACreateCheckBox(11, 23, 160, 15, FRMPropertiesPageCodingHelp, "Auto correct keywords", 22, 0, WS_TABSTOP, 0);
    FRMPropertiesChkWinAPITooltipInfos = WACreateCheckBox(11, 43, 160, 15, FRMPropertiesPageCodingHelp, "Display functions tooltips", 2, 0, WS_TABSTOP | WS_GROUP, 0);
    WACreateLabel(37, 66, 110, 15, FRMPropertiesPageCodingHelp, "lines in keywords list", 0, 0, 0, 0);
    FRMPropertiesTxtAPILines = WACreateTextBox(11, 63, 20, 20, FRMPropertiesPageCodingHelp, "", 3, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesChkUseFncDB = WACreateCheckBox(11, 88, 160, 15, FRMPropertiesPageCodingHelp, "Use functions database", 4, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUseConstDB = WACreateCheckBox(11, 108, 160, 15, FRMPropertiesPageCodingHelp, "Use constants database", 5, 0, WS_TABSTOP, 0);

	FRMPropertiesChkShowSplash = WACreateCheckBox(11, 19, 160, 15, FRMPropertiesPageExtrasLayouts2, "Show splash screen", 6, 0, WS_TABSTOP | WS_GROUP, 0);
    FRMPropertiesChkTVSingle = WACreateCheckBox(11, 19 + (18 * 1), 160, 15, FRMPropertiesPageExtrasLayouts2, "Single click expand treeview", 7, 0, WS_TABSTOP | WS_GROUP, 0);
	FRMPropertiesChkTVTool = WACreateCheckBox(11, 19 + (18 * 2), 160, 15, FRMPropertiesPageExtrasLayouts2, "Tooltips in treeview", 8, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRecentDoc = WACreateCheckBox(11, 19 + (18 * 3), 160, 15, FRMPropertiesPageExtrasLayouts2, "Store in Windows docs", 9, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoOut = WACreateCheckBox(11, 19 + (18 * 4), 160, 15, FRMPropertiesPageExtrasLayouts2, "Silent output", 10, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUseFileDir = WACreateCheckBox(11, 19 + (18 * 5), 160, 15, FRMPropertiesPageExtrasLayouts2, "Use files directories", 11, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCreateDrop = WACreateCheckBox(11, 19 + (18 * 6), 160, 15, FRMPropertiesPageExtrasLayouts2, "New window for dropped files", 12, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveProjectState = WACreateCheckBox(11, 19 + (18 * 7), 160, 15, FRMPropertiesPageExtrasLayouts2, "Save project state", 13, 0, WS_TABSTOP, 0);
}

// -----------------------------------------------------------------------
// Fill the profiles combo
void FillProfil(void)
{
    int i = 0;
    
	WAComboBoxReset(FRMPropertiesCbColors);
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ColorsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        WAComboBoxAddItem(FRMPropertiesCbColors, FRMPropertiesRetVal, -1);
    }
}

// -----------------------------------------------------------------------
// Fill all the properties
void FillProperties(void)
{
    int i = 0;

    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoSaveDelay", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAComboBoxSetIndex(FRMPropertiesCbSave, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "VerboseSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkVerbose, FRMPropertiesRetVal.Get_Long());
    WAControlSetText(FRMPropertiesTxtROOTDIR, WAIniReadKey("Dirs", DirsRepl[DIR_ROOT], MainIniFile));
    WAControlSetText(FRMPropertiesTxtBINDIR, WAIniReadKey("Dirs", DirsRepl[DIR_BIN], MainIniFile));
    WAControlSetText(FRMPropertiesTxtINCLUDEDIR, WAIniReadKey("Dirs", DirsRepl[DIR_INCLUDE], MainIniFile));
    WAControlSetText(FRMPropertiesTxtLIBDIR, WAIniReadKey("Dirs", DirsRepl[DIR_LIB], MainIniFile));
    WAControlSetText(FRMPropertiesTxtHELPDIR, WAIniReadKey("Dirs", DirsRepl[DIR_HELP], MainIniFile));
    WAControlSetText(FRMPropertiesTxtPROJECTSDIR, WAIniReadKey("Dirs", DirsRepl[DIR_PROJECTS], MainIniFile));
    FRMPropertiesRetVal = WAIniReadKey("Layout", "LinesStyle", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAComboBoxSetIndex(FRMPropertiesCbLines, FRMPropertiesRetVal.Get_Long());
    WAControlSetText(FRMPropertiesTxtLines, "0");
    FRMPropertiesRetVal = WAIniReadKey("Layout", "LinesStart", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAControlSetText(FRMPropertiesTxtLines, FRMPropertiesRetVal);
    WAControlSetText(FRMPropertiesTabulations, "8");
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Tabs", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAControlSetText(FRMPropertiesTabulations, FRMPropertiesRetVal);

    // Added fallback for non existant colors (should put default ones)
    FRMPropertiesRetVal = WAIniReadKey("Layout", "BackColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColBackGround, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColBackGround, CurBackColor);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "NormText", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColNormalText, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColNormalText, CurNormTextColor);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "HighText", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColHighlightText, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColHighlightText, CurHighTextColor);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "MarginColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColMargin, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColMargin, CurMarginColor);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "LinesColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColLineNumber, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColLineNumber, CurLineColor);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "APIHighColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColAPIHighlighting, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColAPIHighlighting, APIColor);
    }
    ColorBoxSetColor(FRMPropertiesColNumber, LangNumbersColor);
    ColorBoxSetColor(FRMPropertiesColStrings, LangStringsColor);
    ColorBoxSetColor(FRMPropertiesColComments, LangCommentsColor);
    ColorBoxSetColor(FRMPropertiesColScopeKeywords, LangKeyWordsColor);
    ColorBoxSetColor(FRMPropertiesColKeywords, LangColorAColor);
    ColorBoxSetColor(FRMPropertiesColDirectives, LangColorCColor);
    ColorBoxSetColor(FRMPropertiesColRegisters, LangColorDColor);
    ColorBoxSetColor(FRMPropertiesColExtraFunctions, LangColorEColor);
    ColorBoxSetColor(FRMPropertiesColSeparators, LangColorBColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight1, LangColorFColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight2, LangColorGColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight3, LangColorHColor);
    Prof_BackColor = ColorBoxGetColor(FRMPropertiesColBackGround);
    Orig_Prof_BackColor = Prof_BackColor;
	Prof_NormText = ColorBoxGetColor(FRMPropertiesColNormalText);
    Orig_Prof_NormText = Prof_NormText;
    Prof_HighText = ColorBoxGetColor(FRMPropertiesColHighlightText);
    Orig_Prof_HighText = Prof_HighText;
    Prof_MarginColor = ColorBoxGetColor(FRMPropertiesColMargin);
    Orig_Prof_MarginColor = Prof_MarginColor;
    Prof_LinesColor = ColorBoxGetColor(FRMPropertiesColLineNumber);
    Orig_Prof_LinesColor = Prof_LinesColor;
    Prof_APIHighColor = ColorBoxGetColor(FRMPropertiesColAPIHighlighting);
    Orig_Prof_APIHighColor = Prof_APIHighColor;
    Prof_NumbersColorValue = ColorBoxGetColor(FRMPropertiesColNumber);
    Orig_Prof_NumbersColorValue = Prof_NumbersColorValue;
    Prof_StringsColorValue = ColorBoxGetColor(FRMPropertiesColStrings);
    Orig_Prof_StringsColorValue = Prof_StringsColorValue;
    Prof_CommentsColorValue = ColorBoxGetColor(FRMPropertiesColComments);
    Orig_Prof_CommentsColorValue = Prof_CommentsColorValue;
    Prof_KeyWordsColorValue = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    Orig_Prof_KeyWordsColorValue = Prof_KeyWordsColorValue;
    Prof_ColorAValue = ColorBoxGetColor(FRMPropertiesColKeywords);
    Orig_Prof_ColorAValue = Prof_ColorAValue;
    Prof_ColorBValue = ColorBoxGetColor(FRMPropertiesColSeparators);
    Orig_Prof_ColorBValue = Prof_ColorBValue;
    Prof_ColorCValue = ColorBoxGetColor(FRMPropertiesColDirectives);
    Orig_Prof_ColorCValue = Prof_ColorCValue;
    Prof_ColorDValue = ColorBoxGetColor(FRMPropertiesColRegisters);
    Orig_Prof_ColorDValue = Prof_ColorDValue;
    Prof_ColorEValue = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    Orig_Prof_ColorEValue = Prof_ColorEValue;
    Prof_ColorFValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    Orig_Prof_ColorFValue = Prof_ColorFValue;
    Prof_ColorGValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    Orig_Prof_ColorGValue = Prof_ColorGValue;
    Prof_ColorHValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    Orig_Prof_ColorHValue = Prof_ColorHValue;
    FRMPropertiesRetVal = WAIniReadKey("Layout", "CommentItalic", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkItalic, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Margin", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLeftMargin, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "WhiteSpace", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkWhiteSpaces, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "HighLight", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkHighlightLine, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "WordWrap", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCursorwrap, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Syntax", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSyntaxHighlighting, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "IndentScope", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLanguageScopeIndent, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "EnDragDrop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTextDragAndDrop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "APIToolTip", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkWinAPITooltipInfos, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "SmoothScrolling", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSmoothScrolling, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "LineToolTip", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLineNumberTooltip, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "FollowScrolling", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCaretAlwaysVisible, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "ExpandTabs", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTabsToSpaces, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoCorrect", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoCorrectKeywords, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "HorizSplit", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkHorizontalSplitter, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "DetectChange", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkDetectChange, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "RememberState", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRememberState, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoConvUNIX", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUNIX, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoZoomFiles", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoZoom, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "OutputDates", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkOutputDates, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveAll, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Multiple", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkMultiplesInst, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Remember", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRemember, FRMPropertiesRetVal.Get_Long());
	FRMPropertiesRetVal = WAIniReadKey("Layout", "CreateBak", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkBakFiles, FRMPropertiesRetVal.Get_Long());
	FRMPropertiesRetVal = WAIniReadKey("Layout", "DecorateBak", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkDecorateBak, FRMPropertiesRetVal.Get_Long());
	FRMPropertiesRetVal = WAIniReadKey("Layout", "AcceptFiles", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAcceptFiles, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoMax", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoMax, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AutoClear", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoClean, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "UseSysTray", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkMinimize, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "AlwaysOnTop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkOnTop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "SaveBookmarks", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveBk, FRMPropertiesRetVal.Get_Long());
    WAControlSetText(FRMPropertiesTxtBackupDir, WAIniReadKey("Layout", "BackupDir", MainIniFile));
    SetBackupEn();
    FRMPropertiesRetVal = WAIniReadKey("Layout", "Recent", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        WAControlSetText(FRMPropertiesRecentFiles, FRMPropertiesRetVal);
    }
    else
    {
        WAControlSetText(FRMPropertiesRecentFiles, MaxRecent);
    }
    FRMPropertiesRetVal = WAIniReadKey("Layout", "RecentPrj", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        WAControlSetText(FRMPropertiesRecentProjects, FRMPropertiesRetVal);
    }
    else
    {
        WAControlSetText(FRMPropertiesRecentProjects, MaxRecentPrj);
    }
    SetFontView();
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        WAListBoxAddItem(FRMPropertiesLbFileExt, RegisteredExts.Get(i)->Content, -1);
    }
    if(WAListBoxCount(FRMPropertiesLbFileExt) != 0)
    {
        WAListBoxSetIndex(FRMPropertiesLbFileExt, 0);
        WAControlSetText(FRMPropertiesTxtFileExt, WAListBoxGetItem(FRMPropertiesLbFileExt, WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
    }
    WAControlSetText(FRMPropertiesTxtArgs, WAIniReadKey("Layout", "PrjArg", MainIniFile));
    WAControlSetText(FRMPropertiesCbDebugger, WAIniReadKey("Layout", "PrjDebugger", MainIniFile));
    WAControlSetText(FRMPropertiesTxtWindowsAPI, WAIniReadKey("Layout", "WinHelp", MainIniFile));
    WAControlSetText(FRMPropertiesTxtMSDN, WAIniReadKey("Layout", "MSDNHelp", MainIniFile));
    WAControlSetText(FRMPropertiesTxtDDK, WAIniReadKey("Layout", "DDKHelp", MainIniFile));
    WAControlSetText(FRMPropertiesTxtDirectXSDK, WAIniReadKey("Layout", "DirectXSDKHelp", MainIniFile));
    FRMPropertiesRetVal = WAIniReadKey("Layout", "TVToolTips", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTVTool, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "WinRecent", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRecentDoc, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "SilentAuto", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoOut, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "UseFileDir", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseFileDir, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "NewWindowOnDrop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCreateDrop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "SaveProjectState", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveProjectState, FRMPropertiesRetVal.Get_Long());
    // Load building steps
    FRMPropertiesRetVal = WAIniReadKey("Layout", "StepRes", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAListViewSetItemCheckbox(FRMStepListView, 0, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "StepAsm", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAListViewSetItemCheckbox(FRMStepListView, 1, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "StepLink", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAListViewSetItemCheckbox(FRMStepListView, 2, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "StepClean", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) WAListViewSetItemCheckbox(FRMStepListView, 3, FRMPropertiesRetVal.Get_Long());
    // Load prototypes generator datas
    PrefsGenProg = WAIniReadKey("Layout", "GenProto", MainIniFile);
    PrefsGenCmd = WAIniReadKey("Layout", "GenCmd", MainIniFile);
    PrefsGenDest = WAIniReadKey("Layout", "GenDest", MainIniFile);
    WAControlSetText(FRMPropertiesTxtGenProg, PrefsGenProg);
    WAControlSetText(FRMPropertiesTxtGenSource, PrefsGenCmd);
    WAControlSetText(FRMPropertiesTxtGenDest, PrefsGenDest);
    FRMPropertiesRetVal = WAIniReadKey("Layout", "UseDbFnc", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseFncDB, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "UseDbConst", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseConstDB, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "DbLinesNumbers", MainIniFile);
    // Put a default value (6 lines)
	if(FRMPropertiesRetVal.Len() == 0) FRMPropertiesRetVal = "6";
    WAControlSetText(FRMPropertiesTxtAPILines, FRMPropertiesRetVal);
    FRMPropertiesRetVal = WAIniReadKey("Layout", "ShowSplash", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkShowSplash, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = WAIniReadKey("Layout", "TVSingle", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTVSingle, FRMPropertiesRetVal.Get_Long());
}

// -----------------------------------------------------------------------
// Save the preferences
void SavePrefs(void)
{
    int i = 0;
    int k = 0;
    CStr TmpSkinName;
    long SkinValue = 0;
    long RadioValue = 0;
    CStr TextBoxValue;
    CStr TmpBakDir;
    CStr TmpHelpWinAPI;
    CStr TmpHelpMSDN;
    CStr TmpHelpDDK;
    CStr TmpHelpDirectXSDK;
    CStr DefLangToStore;
    CStr BufString;

    RegisteredExts.Erase();
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(FRMPropertiesRetVal.Len()  == 0) break;
        WAIniDeleteKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
    }
    for(i = 0; i <= (long) WAListBoxCount(FRMPropertiesLbFileExt) - 1; i++)
    {
        RegisteredExts.Add(WAListBoxGetItem(FRMPropertiesLbFileExt, i).Get_String());
        WAIniWriteKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), RegisteredExts.Get(i)->Content, MainIniFile);
    }
    // Correct extensions size
    //ReDim Preserve RegisteredExts(i)

    // Editor
    WAIniWriteKey("Layout", "Margin", CheckBoxGetState(FRMPropertiesChkLeftMargin), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLeftMargin) == 0) LeftM = 0;
    else LeftM = 1;
    WAIniWriteKey("Layout", "WhiteSpace", CheckBoxGetState(FRMPropertiesChkWhiteSpaces), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkWhiteSpaces) == 0) White = 0;
    else White = 1;
    WAIniWriteKey("Layout", "Highlight", CheckBoxGetState(FRMPropertiesChkHighlightLine), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) == 0) ShowH = 0;
    else ShowH = 1;
    WAIniWriteKey("Layout", "WordWrap", CheckBoxGetState(FRMPropertiesChkCursorwrap), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkCursorwrap) == 0) Wrap = 0;
    else Wrap = 1;
    WAIniWriteKey("Layout", "Syntax", CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting) == 0) SyntaxH = 0;
    else SyntaxH = 1;
    WAIniWriteKey("Layout", "IndentScope", CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent) == 0) IndentScope = 0;
    else IndentScope = 1;
    WAIniWriteKey("Layout", "EnDragDrop", CheckBoxGetState(FRMPropertiesChkTextDragAndDrop), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkTextDragAndDrop) == 0) EnDragDrop = 0;
    else EnDragDrop = 1;
    WAIniWriteKey("Layout", "APIToolTip", CheckBoxGetState(FRMPropertiesChkWinAPITooltipInfos), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkWinAPITooltipInfos) == 0) APITTip = 0;
    else APITTip = 1;
    WAIniWriteKey("Layout", "SmoothScrolling", CheckBoxGetState(FRMPropertiesChkSmoothScrolling), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSmoothScrolling) == 0) Smooth = 0;
    else Smooth = 1;
    WAIniWriteKey("Layout", "LineToolTip", CheckBoxGetState(FRMPropertiesChkLineNumberTooltip), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLineNumberTooltip) == 0) LineTTip = 0;
    else LineTTip = 1;
    WAIniWriteKey("Layout", "FollowScrolling", CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible) == 0) CursorFollow = 0;
    else CursorFollow = 1;
    WAIniWriteKey("Layout", "ExpandTabs", CheckBoxGetState(FRMPropertiesChkTabsToSpaces), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkTabsToSpaces) == 0) AutoExpand = 0;
    else AutoExpand = 1;
    WAIniWriteKey("Layout", "AutoCorrect", CheckBoxGetState(FRMPropertiesChkAutoCorrectKeywords), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoCorrectKeywords) == 0) AutoCorrectKey = 0;
    else AutoCorrectKey = 1;
    WAIniWriteKey("Layout", "HorizSplit", CheckBoxGetState(FRMPropertiesChkHorizontalSplitter), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkHorizontalSplitter) == 0) HorizSplit = 0;
    else HorizSplit = 1;
    WAIniWriteKey("Layout", "DetectChange", CheckBoxGetState(FRMPropertiesChkDetectChange), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkDetectChange) == 0) DetectChange = 0;
    else DetectChange = 1;
    WAIniWriteKey("Layout", "RememberState", CheckBoxGetState(FRMPropertiesChkRememberState), MainIniFile);
    WAIniWriteKey("Layout", "AutoConvUNIX", CheckBoxGetState(FRMPropertiesChkUNIX), MainIniFile);
    WAIniWriteKey("Layout", "AutoZoomFiles", CheckBoxGetState(FRMPropertiesChkAutoZoom), MainIniFile);
    WAIniWriteKey("Layout", "OutputDates", CheckBoxGetState(FRMPropertiesChkOutputDates), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkOutputDates) == 0) OutputDates = 0;
    else OutputDates = 1;
    
    // Lines numbering
    WAIniWriteKey("Layout", "LinesStyle", WAComboBoxGetIndex(FRMPropertiesCbLines), MainIniFile);
    LnStyle = WAComboBoxGetIndex(FRMPropertiesCbLines);
    if(WAControlGetText(FRMPropertiesTxtLines).Len() == 0) WAControlSetText(FRMPropertiesTxtLines, "0");
    WAIniWriteKey("Layout", "LinesStart", WAControlGetText(FRMPropertiesTxtLines), MainIniFile);
    LnStart = WAControlGetText(FRMPropertiesTxtLines).Get_Long();

    // Font
    WAIniWriteKey("Layout", "FontName", CurFontName, MainIniFile);
    WAIniWriteKey("Layout", "FontSize", CurFontSize, MainIniFile);
    if(CurFontHandle != 0) DeleteObject(CurFontHandle);
    CurFontHandle = WAGDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
    if(NbForms != 0) ChFontAllChilds(hMDIform.hClient);

    // Other
    if(WAControlGetText(FRMPropertiesTabulations).Len() == 0) WAControlSetText(FRMPropertiesTabulations, "8");
    WAIniWriteKey("Layout", "Tabs", WAControlGetText(FRMPropertiesTabulations), MainIniFile);
    Tabulations = WAControlGetText(FRMPropertiesTabulations).Get_Long();
    if(WAControlGetText(FRMPropertiesRecentFiles).Len() == 0) WAControlSetText(FRMPropertiesRecentFiles, "12");
    WAIniWriteKey("Layout", "Recent", WAControlGetText(FRMPropertiesRecentFiles), MainIniFile);
    MaxRecent = WAControlGetText(FRMPropertiesRecentFiles).Get_Long();
    if(WAControlGetText(FRMPropertiesRecentProjects).Len() == 0) WAControlSetText(FRMPropertiesRecentProjects, "5");
    WAIniWriteKey("Layout", "RecentPrj", WAControlGetText(FRMPropertiesRecentProjects), MainIniFile);
    MaxRecentPrj = WAControlGetText(FRMPropertiesRecentProjects).Get_Long();

    // AutoSave delay
    WAIniWriteKey("Layout", "AutoSaveDelay", WAComboBoxGetIndex(FRMPropertiesCbSave), MainIniFile);
    AutoSaveDelay = WAComboBoxGetIndex(FRMPropertiesCbSave);
    WAIniWriteKey("Layout", "VerboseSave", CheckBoxGetState(FRMPropertiesChkVerbose), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkVerbose) == 0) VerboseSave = 0;
    else VerboseSave = 1;
    FRMPropertiesRetVal = WAIniReadKey("Layout", "VerboseSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkVerbose, FRMPropertiesRetVal.Get_Long());
    
    // General
    WAIniWriteKey("Layout", "AutoSave", CheckBoxGetState(FRMPropertiesChkSaveAll), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSaveAll) == 0) AutoSave = 0;
    else AutoSave = 1;
    WAIniWriteKey("Layout", "Multiple", CheckBoxGetState(FRMPropertiesChkMultiplesInst), MainIniFile);
    WAIniWriteKey("Layout", "Remember", CheckBoxGetState(FRMPropertiesChkRemember), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkRemember) == 0) Remember = 0;
    else Remember = 1;
    WAIniWriteKey("Layout", "CreateBak", CheckBoxGetState(FRMPropertiesChkBakFiles), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkBakFiles) == 0) CreateBak = 0;
    else CreateBak = 1;
    WAIniWriteKey("Layout", "DecorateBak", CheckBoxGetState(FRMPropertiesChkDecorateBak), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkDecorateBak) == 0) DecorateBak = 0;
    else DecorateBak = 1;
    WAIniWriteKey("Layout", "AcceptFiles", CheckBoxGetState(FRMPropertiesChkAcceptFiles), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAcceptFiles) == 0) AcceptFiles = 0;
    else AcceptFiles = 1;
    
	// Set client state
    DragAcceptFiles(hMDIform.hClient, AcceptFiles);
    WAIniWriteKey("Layout", "AutoMax", CheckBoxGetState(FRMPropertiesChkAutoMax), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoMax) == 0) AutoMaximizeChilds = 0;
    else AutoMaximizeChilds = 1;
    WAIniWriteKey("Layout", "AutoClear", CheckBoxGetState(FRMPropertiesChkAutoClean), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoClean) == 0) AutoClearOut = 0;
    else AutoClearOut = 1;
    
    WAIniWriteKey("Layout", "UseSysTray", CheckBoxGetState(FRMPropertiesChkMinimize), MainIniFile);
    WAIniWriteKey("Layout", "AlwaysOnTop", CheckBoxGetState(FRMPropertiesChkOnTop), MainIniFile);
    WAIniWriteKey("Layout", "SaveBookmarks", CheckBoxGetState(FRMPropertiesChkSaveBk), MainIniFile);
    WAIniWriteKey("Layout", "ShowSplash", CheckBoxGetState(FRMPropertiesChkShowSplash), MainIniFile);
    WAIniWriteKey("Layout", "TVSingle", CheckBoxGetState(FRMPropertiesChkTVSingle), MainIniFile);
    
	// Refresh the treeview's expand type
    SetTreeviewExpand();
    WAIniWriteKey("Layout", "UseDbFnc", CheckBoxGetState(FRMPropertiesChkUseFncDB), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkUseFncDB) == 0) UseDbFnc = 0;
    else UseDbFnc = 1;
    WAIniWriteKey("Layout", "UseDbConst", CheckBoxGetState(FRMPropertiesChkUseConstDB), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkUseConstDB) == 0) UseDbConst = 0;
    else UseDbConst = 1;
    
    // Number of lines in API fncs/constants popup list
    if(WAControlGetText(FRMPropertiesTxtAPILines).Len() == 0) WAControlSetText(FRMPropertiesTxtAPILines, "1");
    WAIniWriteKey("Layout", "DbLinesNumbers", WAControlGetText(FRMPropertiesTxtAPILines), MainIniFile);
    DbLinesNumbers = WAControlGetText(FRMPropertiesTxtAPILines).Get_Long();
    
    WAIniWriteKey("Layout", "TVToolTips", CheckBoxGetState(FRMPropertiesChkTVTool), MainIniFile);
    // Refresh the treeview's tooltips
    SetTreeviewTT();

    WAIniWriteKey("Layout", "WinRecent", CheckBoxGetState(FRMPropertiesChkRecentDoc), MainIniFile);
    WAIniWriteKey("Layout", "SilentAuto", CheckBoxGetState(FRMPropertiesChkAutoOut), MainIniFile);
    WAIniWriteKey("Layout", "UseFileDir", CheckBoxGetState(FRMPropertiesChkUseFileDir), MainIniFile);
    WAIniWriteKey("Layout", "NewWindowOnDrop", CheckBoxGetState(FRMPropertiesChkCreateDrop), MainIniFile);
    WAIniWriteKey("Layout", "SaveProjectState", CheckBoxGetState(FRMPropertiesChkSaveProjectState), MainIniFile);

    // Default backup directory
    TmpBakDir = WAControlGetText(FRMPropertiesTxtBackupDir);
    TmpBakDir = ChangeAbsolutePaths(TmpBakDir);
    WAIniWriteKey("Layout", "BackupDir", TmpBakDir, MainIniFile);
    WAControlSetText(FRMPropertiesTxtBackupDir, TmpBakDir);

    // Help files
    TmpHelpWinAPI = WAControlGetText(FRMPropertiesTxtWindowsAPI);
    TmpHelpMSDN = WAControlGetText(FRMPropertiesTxtMSDN);
    TmpHelpDDK = WAControlGetText(FRMPropertiesTxtDDK);
    TmpHelpDirectXSDK = WAControlGetText(FRMPropertiesTxtDirectXSDK);
    TmpHelpWinAPI = ChangeAbsolutePaths(TmpHelpWinAPI);
    TmpHelpMSDN = ChangeAbsolutePaths(TmpHelpMSDN);
    TmpHelpDDK = ChangeAbsolutePaths(TmpHelpDDK);
    TmpHelpDirectXSDK = ChangeAbsolutePaths(TmpHelpDirectXSDK);
    WAIniWriteKey("Layout", "WinHelp", TmpHelpWinAPI, MainIniFile);
    WAIniWriteKey("Layout", "MSDNHelp", TmpHelpMSDN, MainIniFile);
    WAIniWriteKey("Layout", "DDKHelp", TmpHelpDDK, MainIniFile);
    WAIniWriteKey("Layout", "DirectXSDKHelp", TmpHelpDirectXSDK, MainIniFile);
    WAControlSetText(FRMPropertiesTxtWindowsAPI, TmpHelpWinAPI);
    WAControlSetText(FRMPropertiesTxtMSDN, TmpHelpMSDN);

    // Refresh masm directories
    free(Dirs[DIR_ROOT]);
	Dirs[DIR_ROOT] = strdup(WAControlGetText(FRMPropertiesTxtROOTDIR).Get_String());
    WAIniWriteKey("Dirs", DirsRepl[DIR_ROOT], Dirs[DIR_ROOT], MainIniFile);
    free(Dirs[DIR_BIN]);
	Dirs[DIR_BIN] = strdup(WAControlGetText(FRMPropertiesTxtBINDIR).Get_String());
    WAIniWriteKey("Dirs", DirsRepl[DIR_BIN], Dirs[DIR_BIN], MainIniFile);
    free(Dirs[DIR_INCLUDE]);
    Dirs[DIR_INCLUDE] = strdup(WAControlGetText(FRMPropertiesTxtINCLUDEDIR).Get_String());
    WAIniWriteKey("Dirs",DirsRepl[DIR_INCLUDE], Dirs[DIR_INCLUDE], MainIniFile);
    free(Dirs[DIR_LIB]);
    Dirs[DIR_LIB] = strdup(WAControlGetText(FRMPropertiesTxtLIBDIR).Get_String());
    WAIniWriteKey("Dirs", DirsRepl[DIR_LIB], Dirs[DIR_LIB], MainIniFile);
    free(Dirs[DIR_HELP]);
    Dirs[DIR_HELP] = strdup(WAControlGetText(FRMPropertiesTxtHELPDIR).Get_String());
    WAIniWriteKey("Dirs", DirsRepl[DIR_HELP], Dirs[DIR_HELP], MainIniFile);
    free(Dirs[DIR_PROJECTS]);
    Dirs[DIR_PROJECTS] = strdup(WAControlGetText(FRMPropertiesTxtPROJECTSDIR).Get_String());
    // Set it by default just in case
    if(strlen(Dirs[DIR_PROJECTS]) == 0)
    {
		free(Dirs[DIR_PROJECTS]);
		BufString = AppPath + (CStr) "\\Projects";
    	Dirs[DIR_PROJECTS] = strdup(BufString.Get_String());
    }
	WAIniWriteKey("Dirs", DirsRepl[DIR_PROJECTS], Dirs[DIR_PROJECTS], MainIniFile);
    
    // Save colors
    Prof_BackColor = ColorBoxGetColor(FRMPropertiesColBackGround);
    Prof_NormText = ColorBoxGetColor(FRMPropertiesColNormalText);
    Prof_HighText = ColorBoxGetColor(FRMPropertiesColHighlightText);
    Prof_MarginColor = ColorBoxGetColor(FRMPropertiesColMargin);
    Prof_LinesColor = ColorBoxGetColor(FRMPropertiesColLineNumber);
    Prof_NumbersColorValue = ColorBoxGetColor(FRMPropertiesColNumber);
    Prof_StringsColorValue = ColorBoxGetColor(FRMPropertiesColStrings);
    Prof_CommentsColorValue = ColorBoxGetColor(FRMPropertiesColComments);
    Prof_KeyWordsColorValue = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    Prof_APIHighColor = ColorBoxGetColor(FRMPropertiesColAPIHighlighting);
    Prof_ColorAValue = ColorBoxGetColor(FRMPropertiesColKeywords);
    Prof_ColorBValue = ColorBoxGetColor(FRMPropertiesColSeparators);
    Prof_ColorCValue = ColorBoxGetColor(FRMPropertiesColDirectives);
    Prof_ColorDValue = ColorBoxGetColor(FRMPropertiesColRegisters);
    Prof_ColorEValue = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    Prof_ColorFValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    Prof_ColorGValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    Prof_ColorHValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    
    // Change global colors variables
    CurBackColor = Prof_BackColor;
    CurMarginColor = Prof_MarginColor;
    CurNormTextColor = Prof_NormText;
    CurHighTextColor = Prof_HighText;
    CurLineColor = Prof_LinesColor;
    APIColor = Prof_APIHighColor;
    LangNumbersColor = Prof_NumbersColorValue;
    LangStringsColor = Prof_StringsColorValue;
    LangCommentsColor = Prof_CommentsColorValue;
    LangColorAColor = Prof_ColorAValue;
    LangColorBColor = Prof_ColorBValue;
    LangColorCColor = Prof_ColorCValue;
    LangColorDColor = Prof_ColorDValue;
    LangColorEColor = Prof_ColorEValue;
    LangColorFColor = Prof_ColorFValue;
    LangColorGColor = Prof_ColorGValue;
    LangColorHColor = Prof_ColorHValue;
    LangKeyWordsColor = Prof_KeyWordsColorValue;
 
    WAIniWriteKey("Layout", "BackColor", "0x" + (CStr) BufString.Hex_To_String(Prof_BackColor).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "NormText", "0x" + (CStr) BufString.Hex_To_String(Prof_NormText).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "HighText", "0x" + (CStr) BufString.Hex_To_String(Prof_HighText).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "MarginColor", "0x" + (CStr) BufString.Hex_To_String(Prof_MarginColor).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "LinesColor", "0x" + (CStr) BufString.Hex_To_String(Prof_LinesColor).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "NumbersColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_NumbersColorValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "APIHighColor", "0x" + (CStr) BufString.Hex_To_String(Prof_APIHighColor).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "StringsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_StringsColorValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "CommentsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_CommentsColorValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "KeyWordsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_KeyWordsColorValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorAValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorAValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorBValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorBValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorCValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorCValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorDValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorDValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorEValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorEValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorFValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorFValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorGValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorGValue).Get_String(), MainIniFile);
    WAIniWriteKey("Layout", "ColorHValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorHValue).Get_String(), MainIniFile);
	WAIniWriteKey("Layout", "CommentItalic", CheckBoxGetState(FRMPropertiesChkItalic), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkItalic) == 0) ItalicFont = 0;
    else ItalicFont = 1;
   
    // Save prototypes generator datas
    WAIniWriteKey("Layout", "GenProto", WAControlGetText(FRMPropertiesTxtGenProg), MainIniFile);
    WAIniWriteKey("Layout", "GenCmd", WAControlGetText(FRMPropertiesTxtGenSource), MainIniFile);
    WAIniWriteKey("Layout", "GenDest", WAControlGetText(FRMPropertiesTxtGenDest), MainIniFile);

    // Project building
    WAIniWriteKey("Layout", "PrjArg", WAControlGetText(FRMPropertiesTxtArgs), MainIniFile);
    WAIniWriteKey("Layout", "PrjDebugger", WAControlGetText(FRMPropertiesCbDebugger), MainIniFile);
    // Save the list of debuggers now
    //WAComboBoxSaveInIniFile(FRMPropertiesCbDebugger, WAControlGetText(FRMPropertiesCbDebugger), "ExtDebuggers", MainIniFile);
    //AddNewDebugger(FRMPropertiesCbDebugger, WAControlGetText(FRMPropertiesCbDebugger));

    // Save building steps
    WAIniWriteKey("Layout", "StepRes", WAListViewGetItemCheckbox(FRMStepListView, 0), MainIniFile);
    WAIniWriteKey("Layout", "StepAsm", WAListViewGetItemCheckbox(FRMStepListView, 1), MainIniFile);
    WAIniWriteKey("Layout", "StepLink", WAListViewGetItemCheckbox(FRMStepListView, 2), MainIniFile);
    WAIniWriteKey("Layout", "StepClean", WAListViewGetItemCheckbox(FRMStepListView, 3), MainIniFile);
    
    // Get skin filename
    if(WAComboBoxCount(FRMPropertiesCbSkin) != 0)
    {
		TmpSkinName = PrgSkinArray.Get(WAComboBoxGetIndex(FRMPropertiesCbSkin))->Content;
    }
	// Save skin
    for(i = 0; i < PrgSkinControlsDest.Amount(); i++)
    {
        BufString = PrgSkinControlsDest.Get(i)->Content;
		BufString = BufString.Left(5).Upper_Case();
		if(BufString == "COMBO")
		{
            SkinValue = WAComboBoxGetIndex(PrgSkinControls.Get(i)->Content);
            WAIniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, SkinValue, TmpSkinName);
		}
		else if(BufString == "CHECK")
		{
            SkinValue = CheckBoxGetState(PrgSkinControls.Get(i)->Content);
            WAIniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, SkinValue, TmpSkinName);
		}
		else if(BufString == "TEXTB")
		{
            TextBoxValue = WAControlGetText(PrgSkinControls.Get(i)->Content);
            WAIniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, TextBoxValue, TmpSkinName);
		}
		else if(BufString == "LISTC")
		{
            for(k = 0; k <= (long) WAListViewItemCount(PrgSkinControls.Get(i)->Content); k++)
            {
                BufString = PrgSkinControlsDest.Get(i)->Content;
				BufString = BufString + (CStr) StringNumberComplement(k, 3).Get_String();
				WAIniWriteKey("CURRENT", BufString, WAListViewGetItemCheckbox(PrgSkinControls.Get(i)->Content, k), TmpSkinName);
            }
		}
		else if(BufString == "RADIO")
		{
            SkinValue = RadioButtonGetState(PrgSkinControls.Get(i)->Content);
            if(SkinValue != 0)
            {
                BufString = PrgSkinControlsDest.Get(i)->Content;
				BufString = BufString.Mid(9);
				RadioValue = BufString.Get_Long();
                BufString = PrgSkinControlsDest.Get(i)->Content;
				BufString = BufString.Left(8);
				WAIniWriteKey("CURRENT", BufString, RadioValue, TmpSkinName);
            }
        }
    }
    TmpSkinName = ChangeAbsolutePaths(TmpSkinName);
    WAIniWriteKey("BuildSkins", "CurrentSkin", TmpSkinName, SkinsIniFile);

    // Set main window on top or not
    if(CheckBoxGetState(FRMPropertiesChkOnTop) == 1) WAControlSetTopMost(hMDIform.hWnd);
    else WAControlRemTopMost(hMDIform.hWnd);
    
    // Save default selected language
    DefLangToStore = WAControlGetText(FRMPropertiesCbDefLang);
    if(DefLangToStore.Len() == 0) DefLangToStore = "";
    WAIniWriteKey("RefLanguages", "DefLang", DefLangToStore, LanguagesIniFile);

	// Re-init the autosave timer
	RefreshTimer(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Enable/Disable backup directory selection
void SetBackupEn(void)
{
    switch(CheckBoxGetState(FRMPropertiesChkBakFiles))
    {
        case 0:
            WAToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_SELDIR, 0);
            WAToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_DELETE, 0);
			WAControlEnable(FRMPropertiesChkDecorateBak, 0);
			break;
		case 1:
            WAToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_SELDIR, 1);
            WAToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_DELETE, 1);
			WAControlEnable(FRMPropertiesChkDecorateBak, 1);
			break;
    }
}

// -----------------------------------------------------------------------
// View the font Name/Size
void SetFontView(void)
{
    WAControlSetText(FRMPropertiesLbFont, CurFontName + (CStr) " (" + (CStr) CurFontSize + (CStr) ")");
    if(LblFont != 0) DeleteObject(LblFont);
    LblFont = WAGDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
}

// -----------------------------------------------------------------------
// General Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameGeneral(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesChkBakFiles)
            {
                SetBackupEn();
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Colors Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameColors(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int Luminosity;

	switch(uMsg)
	{
        case WM_COMMAND:
			if((HWND) lParam == FRMPropertiesChkItalic)
			{
				FRMPropertiesSetItalicComments(FRMPropertiesCodeMax);
				return(0);
            }
			break;
        case MSG_COLORBOX_CLICKED:
            if((HWND) wParam == FRMPropertiesColBackGround)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColLineNumber)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColNumber)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColStrings)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColComments)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColKeywords)
            {
				FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColDirectives)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColAPIHighlighting)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColNormalText)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColMargin)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColHighlightText)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColRegisters)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraFunctions)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColScopeKeywords)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColSeparators)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesChkItalic)
            {
				FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight1)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight2)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight3)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
			break;
        case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
					if(Assigned_ToolTip != NULL)
					{
						if(WAControlGetNotifiedhWnd(lParam) == Assigned_ToolTip)
						{
							Luminosity = WATrackBarGetPos(FRMPropertiesColLuminosity);
							WAToolBarDisplayToolTip("Luminosity: " + (CStr) Luminosity + (CStr) "%", lParam);
							return(0);
						}
					}
				}
			break;
        case WM_HSCROLL:
            switch(wParam & 0xFFFF)
            {
                case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
                case TB_TOP:
                case TB_BOTTOM:
                case TB_LINEUP:
                case TB_LINEDOWN:
                case TB_PAGEUP:
                case TB_PAGEDOWN:           
					Prof_Luminosity = WATrackBarGetPos(FRMPropertiesColLuminosity);
					ColorBoxSetColor(FRMPropertiesColBackGround, WAGDIColorCalcLuminosity(Orig_Prof_BackColor, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColNormalText, WAGDIColorCalcLuminosity(Orig_Prof_NormText, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColHighlightText, WAGDIColorCalcLuminosity(Orig_Prof_HighText, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColMargin, WAGDIColorCalcLuminosity(Orig_Prof_MarginColor, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColLineNumber, WAGDIColorCalcLuminosity(Orig_Prof_LinesColor, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColNumber, WAGDIColorCalcLuminosity(Orig_Prof_NumbersColorValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColStrings, WAGDIColorCalcLuminosity(Orig_Prof_StringsColorValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColComments, WAGDIColorCalcLuminosity(Orig_Prof_CommentsColorValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColScopeKeywords, WAGDIColorCalcLuminosity(Orig_Prof_KeyWordsColorValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColAPIHighlighting, WAGDIColorCalcLuminosity(Orig_Prof_APIHighColor, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColKeywords, WAGDIColorCalcLuminosity(Orig_Prof_ColorAValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColSeparators, WAGDIColorCalcLuminosity(Orig_Prof_ColorBValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColDirectives, WAGDIColorCalcLuminosity(Orig_Prof_ColorCValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColRegisters, WAGDIColorCalcLuminosity(Orig_Prof_ColorDValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColExtraFunctions, WAGDIColorCalcLuminosity(Orig_Prof_ColorEValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColExtraHighLight1, WAGDIColorCalcLuminosity(Orig_Prof_ColorFValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColExtraHighLight2, WAGDIColorCalcLuminosity(Orig_Prof_ColorGValue, Prof_Luminosity));
					ColorBoxSetColor(FRMPropertiesColExtraHighLight3, WAGDIColorCalcLuminosity(Orig_Prof_ColorHValue, Prof_Luminosity));
					BuildPreview(FRMPropertiesCodeMax);
					GdiFlush();
    				return(0);
			}
			break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Pick up a new color
void FRMPropertiesSetColor(HWND hwnd)
{
    if(WAComDlgChooseColor(FRMPropertieshwnd, ColorBoxGetColor(hwnd)) != 0)
    {
        ColorBoxSetColor(hwnd, MyColor.rgbResult);
        BuildPreview(FRMPropertiesCodeMax);
    }
}

// -----------------------------------------------------------------------
// Colors Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameExtensions(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch(uMsg)
   {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesLbFileExt)
            {
				WAControlSetText(FRMPropertiesTxtFileExt, WAListBoxGetItem(FRMPropertiesLbFileExt, WAListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
				return(0);
            } 
			break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Init minima properties for codemax preview
void FRMPropertiesInitCodeMax(HWND hwnd)
{
	CStr BufString;
    
	CM_SetReadOnly(hwnd, 1);
    CM_ShowScrollBar(hwnd, 0, 1);
    CM_ShowScrollBar(hwnd, 1, 1);
    CM_EnableSplitter(hwnd, 0, 1);
    CM_EnableGlobalProps(hwnd, 1);
    CM_EnableColumnSel(hwnd, 0);
    CM_EnableHideSel(hwnd, 0);
    CM_SetLanguage(hwnd, "Assembler");
    BufString = "WndProc proc hWin:DWORD,uMsg:DWORD,wParam:DWORD,lParam:DWORD\r\n";
	BufString = BufString + "comment \\\r\n";
	BufString = BufString + "  Save 50 bytes from WndProc with Snap\r\n";
	BufString = BufString + "  Then exit.\r\n";
	BufString = BufString + "\\\r\n";
	BufString = BufString + "\t.if uMsg == WM_DESTROY\r\n";
	BufString = BufString + "\t\tszText LogFile,\"d:\\test.dbg\"\r\n";
	BufString = BufString + "\t\tinvoke SnapSaveFile,ADDR LogFile,ADDR WndProc,50\r\n";
	BufString = BufString + "\t\tszText TheMsg,\"Assembler, Pure & Simple\"\r\n";
	BufString = BufString + "\t\tinvoke MessageBox,hWin,ADDR TheMsg,ADDR szDisplayName,MB_OK\r\n";
	BufString = BufString + "\t\txor eax,eax\r\n";
	BufString = BufString + "\t\tinvoke PostQuitMessage,eax\r\n";
	BufString = BufString + "\t\treturn 0\r\n";
	BufString = BufString + "\t.endif\r\n";
	BufString = BufString + "\tret\r\n";
	BufString = BufString + "WndProc endp";
	CM_SetText(hwnd,BufString.Get_String());
}

// -----------------------------------------------------------------------
// Build the codemax preview control
void BuildPreview(HWND hwnd)
{
    WAControlSetFont(hwnd, LblFont);
    CM_EnableSelBounds(hwnd, CheckBoxGetState(FRMPropertiesChkCursorwrap));
    CM_EnableLeftMargin(hwnd, CheckBoxGetState(FRMPropertiesChkLeftMargin));
    CM_EnableWhitespaceDisplay(hwnd, CheckBoxGetState(FRMPropertiesChkWhiteSpaces));
    SetLines(hwnd);
    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) != 0) CM_SetHighlightedLine(hwnd, FRMPropertiesGetCurrentLineNumber(hwnd));
    else CM_SetHighlightedLine(hwnd, -1);
    if(WAControlGetText(FRMPropertiesTabulations).Len() == 0) CM_SetTabSize(hwnd, 8);
    else CM_SetTabSize(hwnd, WAControlGetText(FRMPropertiesTabulations).Get_Long());
	switch(CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent))
	{
        case 0:
            CM_SetAutoIndentMode(hwnd, CM_INDENT_OFF);
			break;
		case 1:
            CM_SetAutoIndentMode(hwnd, CM_INDENT_SCOPE);
			break;
    }
    CM_EnableColorSyntax(hwnd, CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting));
    CM_EnableLineToolTips(hwnd, CheckBoxGetState(FRMPropertiesChkLineNumberTooltip));
    CM_EnableSmoothScrolling(hwnd, CheckBoxGetState(FRMPropertiesChkSmoothScrolling));
    FRMPropertiesSetCodeMaxColors(hwnd);
    FRMPropertiesSetItalicComments(hwnd);
    CM_EnableSplitter(hwnd, 1, CheckBoxGetState(FRMPropertiesChkHorizontalSplitter));
}

// -----------------------------------------------------------------------
// Select context help files
void GetHelpFile(HWND hwnd)
{
    CStr LdFile;
    CStr OpFilters;

    OpFilters = "Help files (*.hlp;*.chm;*.col)|*.hlp;*.chm;*.col";
    LdFile = WAComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    WAControlSetText(hwnd, LdFile);
}

// -----------------------------------------------------------------------
// Select prototype generator
void GetGenProtoProg(void)
{
    CStr LdFile;
    CStr OpFilters;
 
    OpFilters = "Exe files (*.exe;*.com)|*.exe;*.com";
    LdFile = WAComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    WAControlSetText(FRMPropertiesTxtGenProg, LdFile);
}

// -----------------------------------------------------------------------
// Select a font
void FRMPropertiesSelectFont(void)
{
    CStr FName;
    int i = 0;

    if(WAComDlgChooseFont(FRMPropertieshwnd, CurFontName, CurFontSize, 1) != 0)
    {
        for(i = 0; i <= LF_FACESIZE - 1; i++)
        {
            if(MyLogFont.lfFaceName[i] == 0) break;
            FName = FName + FName.Chr(MyLogFont.lfFaceName[i]);
        }
        if(FName.Len() == 0) return;
        CurFontName = FName;
        CurFontSize = MyFont.iPointSize / 10;
        SetFontView();
    }
}

// -----------------------------------------------------------------------
// Choose Masm32 directories
void FRMPropertiesChooseROOTDIR(HWND hwnd)
{
    CStr NewROOTDIR;
    CStr NewMAsmCaption;
    CStr BufString;

    if(hwnd == FRMPropertiesTxtROOTDIR) NewMAsmCaption = "Select main directory...";
    else if(hwnd == FRMPropertiesTxtBINDIR) NewMAsmCaption = "Select binaries directory...";
    else if(hwnd == FRMPropertiesTxtINCLUDEDIR) NewMAsmCaption = "Select includes directory...";
    else if(hwnd == FRMPropertiesTxtLIBDIR) NewMAsmCaption = "Select libraries/units directory...";
    else if(hwnd == FRMPropertiesTxtHELPDIR) NewMAsmCaption = "Select help directory...";
    else if(hwnd == FRMPropertiesTxtPROJECTSDIR) NewMAsmCaption = "Select Chromatic projects directory...";
    NewROOTDIR = WAComDlgBrowseForFolder(FRMPropertieshwnd, NewMAsmCaption);
    if(NewROOTDIR.Len() != 0)
    {
        WAControlSetText(hwnd, NewROOTDIR);
        if(hwnd == FRMPropertiesTxtROOTDIR)
        {
            BufString = "Populate directories from '" + (CStr) NewROOTDIR + (CStr) "' ?";
            if(WAMiscMsgBox(FRMPropertieshwnd, BufString, MB_QUESTION, Requesters) == IDYES)
            {
                if(strcmp(NewROOTDIR.Right(1).Get_String(), "\\") == 0) NewROOTDIR = NewROOTDIR.Left(NewROOTDIR.Len() - 1);
                if(WAFileExist(NewROOTDIR + (CStr) "\\bin\\win32"))
                {
					WAControlSetText(FRMPropertiesTxtBINDIR, NewROOTDIR + (CStr) "\\bin\\win32");
                }
                else
                {
					if(WAFileExist(NewROOTDIR + (CStr) "\\bin"))
					{
						WAControlSetText(FRMPropertiesTxtBINDIR, NewROOTDIR + (CStr) "\\bin");
					}
					else
					{
						WAControlSetText(FRMPropertiesTxtBINDIR, "");
					}
				}
                if(WAFileExist(NewROOTDIR + (CStr) "\\include"))
                {
					WAControlSetText(FRMPropertiesTxtINCLUDEDIR, NewROOTDIR + (CStr) "\\include");
                }
                else
                {
					WAControlSetText(FRMPropertiesTxtINCLUDEDIR, "");
				}
				if(WAFileExist(NewROOTDIR + (CStr) "\\lib"))
				{
					WAControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\lib");
				}
				else
				{
					if(WAFileExist(NewROOTDIR + (CStr) "\\units\\win32\\rtl"))
					{
						WAControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units\\win32\\rtl");
					}
					else
					{
						if(WAFileExist(NewROOTDIR + (CStr) "\\units\\win32"))
						{
							WAControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units\\win32");
						}
						else
						{
							if(WAFileExist(NewROOTDIR + (CStr) "\\units"))
							{
								WAControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units");
							}
							else
							{
								WAControlSetText(FRMPropertiesTxtLIBDIR, "");
							}
						}
					}
				}
				if(WAFileExist(NewROOTDIR + (CStr) "\\help"))
				{
					WAControlSetText(FRMPropertiesTxtHELPDIR, NewROOTDIR + (CStr) "\\help");
				}
				else
				{
					if(WAFileExist(NewROOTDIR + (CStr) "\\doc"))
					{
						WAControlSetText(FRMPropertiesTxtHELPDIR, NewROOTDIR + (CStr) "\\doc");
					}
					else
					{
						WAControlSetText(FRMPropertiesTxtHELPDIR, "");
					}
				}
			}
        }
    }
}

// -----------------------------------------------------------------------
// Load profiles datas
void LoadColorsProfile(CStr ProfileName)
{
    CStr RetColProf;
    CStr RetValProf;
    CStr BufString;

    // Load profiles datas
    RetColProf = WAIniReadKey(ProfileName, "BackColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_BackColor = RetColProf.Get_Hex();
    Orig_Prof_BackColor = Prof_BackColor;
	RetColProf = WAIniReadKey(ProfileName, "NormText", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_NormText = RetColProf.Get_Hex();
    Orig_Prof_NormText = Prof_NormText;
    RetColProf = WAIniReadKey(ProfileName, "HighText", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_HighText = RetColProf.Get_Hex();
    Orig_Prof_HighText = Prof_HighText;
    RetColProf = WAIniReadKey(ProfileName, "MarginColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_MarginColor = RetColProf.Get_Hex();
    Orig_Prof_MarginColor = Prof_MarginColor;
    RetColProf = WAIniReadKey(ProfileName, "LinesColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_LinesColor = RetColProf.Get_Hex();
    Orig_Prof_LinesColor = Prof_LinesColor;
    RetColProf = WAIniReadKey(ProfileName, "NumbersColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_NumbersColorValue = RetColProf.Get_Hex();
    Orig_Prof_NumbersColorValue = Prof_NumbersColorValue;
    RetColProf = WAIniReadKey(ProfileName, "StringsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_StringsColorValue = RetColProf.Get_Hex();
    Orig_Prof_StringsColorValue = Prof_StringsColorValue;
    RetColProf = WAIniReadKey(ProfileName, "CommentsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_CommentsColorValue = RetColProf.Get_Hex();
    Orig_Prof_CommentsColorValue = Prof_CommentsColorValue;
    RetColProf = WAIniReadKey(ProfileName, "KeyWordsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_KeyWordsColorValue = RetColProf.Get_Hex();
    Orig_Prof_KeyWordsColorValue = Prof_KeyWordsColorValue;
    RetColProf = WAIniReadKey(ProfileName, "APIHighColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_APIHighColor = RetColProf.Get_Hex();
    Orig_Prof_APIHighColor = Prof_APIHighColor;
    RetColProf = WAIniReadKey(ProfileName, "ColorAValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorAValue = RetColProf.Get_Hex();
    Orig_Prof_ColorAValue = Prof_ColorAValue;
    RetColProf = WAIniReadKey(ProfileName, "ColorBValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorBValue = RetColProf.Get_Hex();
    Orig_Prof_ColorBValue = Prof_ColorBValue;
    RetColProf = WAIniReadKey(ProfileName, "ColorCValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorCValue = RetColProf.Get_Hex();
    Orig_Prof_ColorCValue = Prof_ColorCValue;
    RetColProf = WAIniReadKey(ProfileName, "ColorDValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorDValue = RetColProf.Get_Hex();
    Orig_Prof_ColorDValue = Prof_ColorDValue;
    RetColProf = WAIniReadKey(ProfileName, "ColorEValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorEValue = RetColProf.Get_Hex();
    Orig_Prof_ColorEValue = Prof_ColorEValue;

    RetValProf = WAIniReadKey(ProfileName, "ItalicComment", ColorsIniFile);
    if(RetValProf.Len() == 0) goto ProfileCorrupted;
    Prof_ItalicComment = RetValProf.Get_Long();

    // Handle new profile keys
    RetColProf = WAIniReadKey(ProfileName, "ColorFValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorFValue = RetColProf.Get_Hex();
    RetColProf = WAIniReadKey(ProfileName, "ColorGValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorGValue = RetColProf.Get_Hex();
    RetColProf = WAIniReadKey(ProfileName, "ColorHValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorHValue = RetColProf.Get_Hex();
    
    ColorBoxSetColor(FRMPropertiesColBackGround, Prof_BackColor);
    ColorBoxSetColor(FRMPropertiesColNormalText, Prof_NormText);
    ColorBoxSetColor(FRMPropertiesColHighlightText, Prof_HighText);
    ColorBoxSetColor(FRMPropertiesColMargin, Prof_MarginColor);
    ColorBoxSetColor(FRMPropertiesColLineNumber, Prof_LinesColor);
    ColorBoxSetColor(FRMPropertiesColNumber, Prof_NumbersColorValue);
    ColorBoxSetColor(FRMPropertiesColStrings, Prof_StringsColorValue);
    ColorBoxSetColor(FRMPropertiesColComments, Prof_CommentsColorValue);
    ColorBoxSetColor(FRMPropertiesColScopeKeywords, Prof_KeyWordsColorValue);
    ColorBoxSetColor(FRMPropertiesColAPIHighlighting, Prof_APIHighColor);
    ColorBoxSetColor(FRMPropertiesColKeywords, Prof_ColorAValue);
    ColorBoxSetColor(FRMPropertiesColDirectives, Prof_ColorCValue);
    ColorBoxSetColor(FRMPropertiesColRegisters, Prof_ColorDValue);
    ColorBoxSetColor(FRMPropertiesColExtraFunctions, Prof_ColorEValue);
    ColorBoxSetColor(FRMPropertiesColSeparators, Prof_ColorBValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight1, Prof_ColorFValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight2, Prof_ColorGValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight3, Prof_ColorHValue);
    
	// Reset slider
    WATrackBarSetPos(FRMPropertiesColLuminosity, 100);
	CheckBoxSetState(FRMPropertiesChkItalic, Prof_ItalicComment);
    BuildPreview(FRMPropertiesCodeMax);
    return;
ProfileCorrupted:
    WAMiscMsgBox(FRMPropertieshwnd, "Color profile corrupted or missing.", MB_ERROR, Requesters);
    SetFocus(FRMPropertiesCbColors);
}

// -----------------------------------------------------------------------
// Save profiles datas
long SaveColorsProfile(CStr ProfileName)
{
    CStr ProfNameToRegister;
    int i = 0;
    long NoReg = 0;
    CStr UProfName;
	CStr BufString;

    ProfNameToRegister = StringReplace(ProfileName.Trim(), "\t", "", 1, -1, Binary_Compare);
    if(ProfNameToRegister.Len() == 0) return(0);
    UProfName = ProfNameToRegister;
    NoReg = 0;
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ColorsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(strcmpi(FRMPropertiesRetVal.Get_String(), UProfName.Get_String()) == 0)
        {
            NoReg = 1;
            if(WAMiscMsgBox(FRMPropertieshwnd, "Profile already exists. Overwrite it ?", MB_QUESTION, Requesters) == IDNO)
            {
                NoReg = 2;
                break;
            }
            else
            {
                break;
            }
        }
    }
    switch(NoReg)
    {
        // New profil
        case 0:
            // Register name
            WAIniWriteKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ProfNameToRegister, ColorsIniFile);
			break;
		case 2:
            return(1);
    }
    WAIniWriteKey(ProfNameToRegister, "BackColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColBackGround)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "NormText", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColNormalText)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "HighText", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColHighlightText)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "MarginColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColMargin)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "LinesColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColLineNumber)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "NumbersColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColNumber)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "StringsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColStrings)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "CommentsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColComments)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "KeyWordsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColScopeKeywords)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "APIHighColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColAPIHighlighting)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorAValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColKeywords)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorBValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColSeparators)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorCValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColDirectives)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorDValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColRegisters)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorEValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraFunctions)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorFValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight1)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorGValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight2)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ColorHValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight3)).Get_String(), ColorsIniFile);
    WAIniWriteKey(ProfNameToRegister, "ItalicComment", CheckBoxGetState(FRMPropertiesChkItalic), ColorsIniFile);
    FillProfil();
    WAControlSetText(FRMPropertiesCbColors, ProfNameToRegister);
    BufString = "Profile '" + (CStr) ProfNameToRegister + (CStr) "' saved.";
    WAMiscMsgBox(FRMPropertieshwnd, BufString, MB_INFORMATION, Requesters);
	return(1);
}

// -----------------------------------------------------------------------
// Set line displaying
void SetLines(HWND hWnd)
{
    CStr LnSt;

    LnSt = WAControlGetText(FRMPropertiesTxtLines);
    if(LnSt.Len() == 0)
    {
        LnSt = "0";
        WAControlSetText(FRMPropertiesTxtLines, LnSt);
    }
	SetCodeMaxLineStyle(hWnd, WAComboBoxGetIndex(FRMPropertiesCbLines));
}

// -----------------------------------------------------------------------
// Retrieve current line number
long FRMPropertiesGetCurrentLineNumber(HWND hwnd)
{
    CM_GetSel(hwnd, &LineRange, 1);
    return(LineRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Set comment font style
void FRMPropertiesSetItalicComments(HWND hwnd)
{
    CM_FONTSTYLES PrevFontStyle;
    
	memset(&PrevFontStyle, 0, sizeof(PrevFontStyle));
	switch(CheckBoxGetState(FRMPropertiesChkItalic))
	{
        case 0:
            PrevFontStyle.byComment = CM_FONT_NORMAL;
			break;
		case 1:
            PrevFontStyle.byComment = CM_FONT_ITALIC;
			break;
    }
    PrevFontStyle.byKeyword = CM_FONT_NORMAL;
    PrevFontStyle.byLineNumber = CM_FONT_NORMAL;
    PrevFontStyle.byNumber = CM_FONT_NORMAL;
    PrevFontStyle.byOperator = CM_FONT_NORMAL;
    PrevFontStyle.byScopeKeyword = CM_FONT_NORMAL;
    PrevFontStyle.byString = CM_FONT_NORMAL;
    PrevFontStyle.byExtra1 = CM_FONT_NORMAL;
    PrevFontStyle.byExtra2 = CM_FONT_NORMAL;
    PrevFontStyle.byExtra3 = CM_FONT_NORMAL;
    PrevFontStyle.byTagAttributeName = CM_FONT_NORMAL;
    PrevFontStyle.byTagElementName = CM_FONT_NORMAL;
    PrevFontStyle.byTagEntity = CM_FONT_NORMAL;
    PrevFontStyle.byTagText = CM_FONT_NORMAL;
    PrevFontStyle.byText = CM_FONT_NORMAL;
    CM_SetFontStyles(hwnd, &PrevFontStyle);
}

// -----------------------------------------------------------------------
// Set comment font style
void FRMPropertiesSetCodeMaxColors(HWND hwnd)
{
    CM_COLORS PrevColsToSet;

	memset(&PrevColsToSet, 0, sizeof(PrevColsToSet));
    PrevColsToSet.crText = ColorBoxGetColor(FRMPropertiesColNormalText);
    PrevColsToSet.crTextBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crBookmark = 0xDD7766;
    PrevColsToSet.crBookmarkBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crBreakpoint = 0x0011DD;
    PrevColsToSet.crBreakpointBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crComment = ColorBoxGetColor(FRMPropertiesColComments);
    PrevColsToSet.crCommentBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crKeyword = ColorBoxGetColor(FRMPropertiesColKeywords);
    PrevColsToSet.crKeywordBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crNumber = ColorBoxGetColor(FRMPropertiesColNumber);
    PrevColsToSet.crNumberBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crOperator = ColorBoxGetColor(FRMPropertiesColSeparators);
    PrevColsToSet.crOperatorBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crScopeKeyword = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    PrevColsToSet.crScopeKeywordBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crString = ColorBoxGetColor(FRMPropertiesColStrings);
    PrevColsToSet.crStringBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagAttributeName = ColorBoxGetColor(FRMPropertiesColDirectives);
    PrevColsToSet.crTagAttributeNameBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagElementName = ColorBoxGetColor(FRMPropertiesColRegisters);
    PrevColsToSet.crTagElementNameBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra1 = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    PrevColsToSet.crExtra1Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra2 = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    PrevColsToSet.crExtra2Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra3 = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    PrevColsToSet.crExtra3Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crLineNumber = ColorBoxGetColor(FRMPropertiesColLineNumber);
    PrevColsToSet.crLineNumberBk = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crTagEntity = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    PrevColsToSet.crTagEntityBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagTextBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crHighlightedLine = ColorBoxGetColor(FRMPropertiesColHighlightText);
    PrevColsToSet.crLeftMargin = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crVDividerLines = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crHDividerLines = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crWindow = ColorBoxGetColor(FRMPropertiesColBackGround);
    // Set colors
    CM_SetColors(hwnd, &PrevColsToSet);
}

// -----------------------------------------------------------------------
// Properties CodeMax hook proc
LRESULT CALLBACK FRMPropertiesCMHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CONTEXTMENU:
            return(0);
        case WM_DROPFILES:
            return(0);
        case WM_MENUSELECT:
            return(0);
        case WM_EXITMENULOOP:
            return(0);
        case WM_COMMAND:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Select the default backup directory
void FRMPropertiesSelectBakDir(void)
{
    CStr NewBakDir;
    
	NewBakDir = WAComDlgBrowseForFolder(FRMPropertieshwnd, "Select default .BAK directory...");
    if(NewBakDir.Len() != 0)
    {
        NewBakDir = ChangeAbsolutePaths(NewBakDir);
        WAControlSetText(FRMPropertiesTxtBackupDir, NewBakDir);
    }
}

// -----------------------------------------------------------------------
// CodeMax frame hook proc
LRESULT CALLBACK FRMPropertiesFrameCodeMax(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case CMN_SELCHANGE:
                    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) == 1)
                    {
                        LoadCurrentSel(FRMPropertiesCodeMax);
                        if(CodeMaxCurRange.posStart.nLine != (long) CM_GetHighlightedLine(FRMPropertiesCodeMax)) CM_SetHighlightedLine(FRMPropertiesCodeMax, CodeMaxCurRange.posStart.nLine);
                    }
					break;
                case CMN_VSCROLL:
                    FRMProPertiesConfineCaret();
					break;
                case CMN_SHOWPROPS:
                    return(1);
                case CMN_REGISTEREDCMD:
                    return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Managment frame hook proc
LRESULT CALLBACK FRMPropertiesFrameManagment(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCbAccounts)
            {
                if(WAControlGetNotifiedCommand(wParam) == CBN_SELCHANGE)
                {
                    FRMPropertiesInFtpCreation = 0;
                    FillFTPProps();
                    SetFocus(FRMPropertiesTxtIP);
                }
                return(0);
			} 
			break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Main frame hook proc
LRESULT CALLBACK FRMPropertiesFrameMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesChkAnonymous)
            {
                SetAnonymousState(0);
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Make caret always visible
void FRMProPertiesConfineCaret(void)
{
    long ViCount = 0;
    long ViTop = 0;

    if(CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible) == 1)
    {
        LoadCurrentSel(FRMPropertiesCodeMax);
        CodeMaxCurRange.posStart.nLine = CodeMaxCurRange.posEnd.nLine;
        CodeMaxCurRange.posStart.nCol = CodeMaxCurRange.posEnd.nCol;
        ViCount = CM_GetVisibleLineCount(FRMPropertiesCodeMax, CM_GetCurrentView(FRMPropertiesCodeMax), 1);
        ViTop = CM_GetTopIndex(FRMPropertiesCodeMax, CM_GetCurrentView(FRMPropertiesCodeMax));
        if(CodeMaxCurRange.posStart.nLine < (long) ViTop)
        {
            CodeMaxCurRange.posStart.nLine = ViTop;
            CodeMaxCurRange.posEnd.nLine = ViTop;
            CM_SetSel(FRMPropertiesCodeMax, &CodeMaxCurRange, 1);
            return;
        }
        if(CodeMaxCurRange.posStart.nLine >= (long) (ViTop + ViCount))
        {
            CodeMaxCurRange.posStart.nLine = ViTop + ViCount - 1;
            CodeMaxCurRange.posEnd.nLine = ViTop + ViCount - 1;
            CM_SetSel(FRMPropertiesCodeMax, &CodeMaxCurRange, 1);
            return;
        }
    }
}
    
// -----------------------------------------------------------------------
// Refresh the codemax childs
void RefreshChilds(void)
{
    if(NbForms != 0) EnumChildWindows(hMDIform.hClient, &EnumRefreshChilds, 0);
}

int CALLBACK EnumRefreshChilds(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(WAControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CMGetRealFile(ChildStruct->RFile).Len() != 0)
                {
					// hChildCodeMax can be null in case of a custom child
					if(ChildStruct->hChildCodeMax != NULL)
					{
						SetCodeMaxColors(ChildStruct->hChildCodeMax);
						if(ChildStruct->CodeMaxPropertiesHook != NULL)
						{
							// Run hook
							ChildStruct->CodeMaxPropertiesHook(ChildStruct->hChildCodeMax);
						}
					}
				}
            }
        }
        return(1);
    }
}

// -----------------------------------------------------------------------
// Build ftp accounts list
void BuildFTP(HWND hComboBox)
{
    FillFTPCombo(hComboBox);
    if(WAComboBoxCount(FRMPropertiesCbAccounts) != 0)
    {
        WAComboBoxSetIndex(FRMPropertiesCbAccounts, 0);
        FillFTPProps();
    }
}

// -----------------------------------------------------------------------
// Fill all ftp account fields
void FillFTPProps(void)
{
    CStr AccountName;
    CStr DecryptedPass;

    AccountName = WAControlGetText(FRMPropertiesCbAccounts);
    if(AccountName.Len() == 0) return;
    WAControlSetText(FRMPropertiesTxtIP, WAIniReadKey(AccountName, "FTPIP", FtpAccountsIniFile));
    FRMPropertiesRetVal = WAIniReadKey(AccountName, "FTPAnon", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        CheckBoxSetState(FRMPropertiesChkAnonymous, FRMPropertiesRetVal.Get_Long());
    }
    else
    {
        CheckBoxSetState(FRMPropertiesChkAnonymous, 0);
    }
    SetAnonymousState(1);
    WAControlSetText(FRMPropertiesTxtUserName, WAIniReadKey(AccountName, "FTPUserName", FtpAccountsIniFile));
    FRMPropertiesRetVal = WAIniReadKey(AccountName, "FTPSavePass", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        WAComboBoxSetIndex(FRMPropertiesCbPasswordType, FRMPropertiesRetVal.Get_Long());
        switch(FRMPropertiesRetVal.Get_Long())
        {
            case 0:
                WAControlSetText(FRMPropertiesTxtPassword, "");
				break;
			case 1:
                WAControlSetText(FRMPropertiesTxtPassword, WAIniReadKey(AccountName, "FTPUserPass", FtpAccountsIniFile));
				break;
            case 2:
                // Load password and decrypt it
                DecryptedPass = WAIniReadKey(AccountName, "FTPUserPass", FtpAccountsIniFile);
                WAControlSetText(FRMPropertiesTxtPassword, DecryptPassword(DecryptedPass));
				break;
        }
    }
    else
    {
        WAComboBoxSetIndex(FRMPropertiesCbPasswordType, 0);
        WAControlSetText(FRMPropertiesTxtPassword, "");
    }
    WAControlSetText(FRMPropertiesTxtInitialPath, WAIniReadKey(AccountName, "FTPPath", FtpAccountsIniFile));
    WAControlSetText(FRMPropertiesTxtFTPPort, WAIniReadKey(AccountName, "FTPPort", FtpAccountsIniFile));
    
    WAControlSetText(FRMPropertiesTxtFTPTimeOut, WAIniReadKey(AccountName, "FTPTimeOut", FtpAccountsIniFile));
    FRMPropertiesRetVal = WAIniReadKey(AccountName, "FTPTransfersType", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        switch(FRMPropertiesRetVal.Get_Long())
        {
            case 1:
                RadioButtonSetState(FRMPropertiesRBASCII, 1);
                RadioButtonSetState(FRMPropertiesRBBinary, 0);
				break;
			case 2:
                RadioButtonSetState(FRMPropertiesRBBinary, 1);
                RadioButtonSetState(FRMPropertiesRBASCII, 0);
				break;
        }
    }
    else
    {
        RadioButtonSetState(FRMPropertiesRBASCII, 0);
        RadioButtonSetState(FRMPropertiesRBBinary, 1);
    }
    FRMPropertiesRetVal = WAIniReadKey(AccountName, "FTPPassive", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        CheckBoxSetState(FRMPropertiesChkPassive, FRMPropertiesRetVal.Get_Long());
    }
    else
    {
        CheckBoxSetState(FRMPropertiesChkPassive, 0);
    }
}

// -----------------------------------------------------------------------
// Reset all ftp account fields
void ResetFTPProps(void)
{
    WAControlSetText(FRMPropertiesTxtIP, "");
    CheckBoxSetState(FRMPropertiesChkAnonymous, 0);
    SetAnonymousState(0);
    WAControlSetText(FRMPropertiesTxtUserName, "");
    WAComboBoxSetIndex(FRMPropertiesCbPasswordType, 0);
    WAControlSetText(FRMPropertiesTxtPassword, "");
    WAControlSetText(FRMPropertiesTxtInitialPath, "");
    WAControlSetText(FRMPropertiesTxtFTPPort, "");
    WAControlSetText(FRMPropertiesTxtFTPTimeOut, "");
    RadioButtonSetState(FRMPropertiesRBASCII, 0);
    RadioButtonSetState(FRMPropertiesRBBinary, 1);
    CheckBoxSetState(FRMPropertiesChkPassive, 0);
}

// -----------------------------------------------------------------------
// Enable/Disable anonymous state
void SetAnonymousState(long DontFocus)
{
    switch(CheckBoxGetState(FRMPropertiesChkAnonymous))
    {
        case 0:
            WAControlEnable(FRMPropertiesTxtUserName, 1);
            WAControlEnable(FRMPropertiesTxtPassword, 1);
            WAControlEnable(FRMPropertiesCbPasswordType, 1);
            if(DontFocus == 0) SetFocus(FRMPropertiesTxtUserName);
			break;
        case 1:
            WAControlEnable(FRMPropertiesTxtUserName, 0);
            WAControlEnable(FRMPropertiesTxtPassword, 0);
            WAControlEnable(FRMPropertiesCbPasswordType, 0);
			break;
	}
}

// -----------------------------------------------------------------------
// Save all ftp account fields
void SaveFTPProps(void)
{
    int i = 0;
    CStr AccountName;
    CStr BufString;
    
	AccountName = WAControlGetText(FRMPropertiesCbAccounts);
    if(AccountName.Len() == 0) return;
    // Search account key
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(strcmpi(AccountName.Get_String(), FRMPropertiesRetVal.Get_String()) == 0) break;
    }
    // Add account name
    WAIniWriteKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), AccountName, FtpAccountsIniFile);
    // Save account datas
    WAIniWriteKey(AccountName, "FTPIP", WAControlGetText(FRMPropertiesTxtIP), FtpAccountsIniFile);
    WAIniWriteKey(AccountName, "FTPAnon", CheckBoxGetState(FRMPropertiesChkAnonymous), FtpAccountsIniFile);
    WAIniWriteKey(AccountName, "FTPUserName", WAControlGetText(FRMPropertiesTxtUserName), FtpAccountsIniFile);
    switch(WAComboBoxGetIndex(FRMPropertiesCbPasswordType))
    {
        case 0:
            WAIniWriteKey(AccountName, "FTPUserPass", "", FtpAccountsIniFile);
			break;
		case 1:
            WAIniWriteKey(AccountName, "FTPUserPass", WAControlGetText(FRMPropertiesTxtPassword), FtpAccountsIniFile);
			break;
        case 2:
            WAIniWriteKey(AccountName, "FTPUserPass", CryptPassword(WAControlGetText(FRMPropertiesTxtPassword)), FtpAccountsIniFile);
			break;
    }
    WAIniWriteKey(AccountName, "FTPSavePass ", WAComboBoxGetIndex(FRMPropertiesCbPasswordType), FtpAccountsIniFile);
    WAIniWriteKey(AccountName, "FTPPath", WAControlGetText(FRMPropertiesTxtInitialPath), FtpAccountsIniFile);
    WAIniWriteKey(AccountName, "FTPPort", WAControlGetText(FRMPropertiesTxtFTPPort), FtpAccountsIniFile);
    WAIniWriteKey(AccountName, "FTPTimeOut", WAControlGetText(FRMPropertiesTxtFTPTimeOut), FtpAccountsIniFile);
    switch(RadioButtonGetState(FRMPropertiesRBASCII))
    {
        case 1:
            WAIniWriteKey(AccountName, "FTPTransfersType", "1", FtpAccountsIniFile);
			break;
	}
    switch(RadioButtonGetState(FRMPropertiesRBBinary))
    {
        case 1:
            WAIniWriteKey(AccountName, "FTPTransfersType", "2", FtpAccountsIniFile);
			break;
    }
    WAIniWriteKey(AccountName, "FTPPassive", CheckBoxGetState(FRMPropertiesChkPassive), FtpAccountsIniFile);
    BufString = "FTP account '" + (CStr) AccountName + (CStr) "' updated.",
    WAMiscMsgBox(FRMPropertieshwnd, BufString, MB_INFORMATION, Requesters);
}

// -----------------------------------------------------------------------
// Set the current tab to view
void FRMPropertiesChangeCurrentTab(void)
{
    switch(WASysTabGetCurrentItem(FRMPropertiesSysTab))
    {
        case 0:
            WAControlVisible(FRMPropertiesPageColors, 0);
            WAControlVisible(FRMPropertiesTbColors, 0);
            WAControlVisible(FRMPropertiesPageAPIHelp, 0);
            WAControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            WAControlVisible(FRMPropertiesPageSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdMSDN, 0);
            WAControlVisible(FRMPropertiesPageCodeMax, 0);
            WAControlVisible(FRMPropertiesPageDDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDDK, 0);
            WAControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            WAControlVisible(FRMPropertiesCmdRegs, 0);
            WAControlVisible(FRMPropertiesPageFolders, 0);
            WAControlVisible(FRMPropertiesTbROOTDIR, 0);
            WAControlVisible(FRMPropertiesTbBINDIR, 0);
            WAControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            WAControlVisible(FRMPropertiesTbLIBDIR, 0);
            WAControlVisible(FRMPropertiesTbHELPDIR, 0);
            WAControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            WAControlVisible(FRMPropertiesPageProjects, 0);
            WAControlVisible(FRMPropertiesCmdDebugger, 0);
            WAControlVisible(FRMPropertiesTbValidDebugger, 0);
            WAControlVisible(FRMPropertiesPageProtoGen, 0);
            WAControlVisible(FRMPropertiesCmdGenProg, 0);
            WAControlVisible(FRMPropertiesPageFTPManage, 0);
            WAControlVisible(FRMPropertiesTbAccounts, 0);
            WAControlVisible(FRMPropertiesPageAccountInfos, 0);
            WAControlVisible(FRMPropertiesPageTransfersTypes, 0);
            WAControlVisible(FRMPropertiesPageCodingHelp, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            WAControlVisible(FRMPropertiesPageEditor, 1);
            WAControlVisible(FRMPropertiesPageFileExtensions, 1);
            WAControlVisible(FRMPropertiesTbFileExt, 1);
            WAControlVisible(FRMPropertiesPageOther, 1);
            WAControlVisible(FRMPropertiesPageFont, 1);
            WAControlVisible(FRMPropertiesCmdFontChoose, 1);
            WAControlVisible(FRMPropertiesPageAutoSave, 1);
            WAControlVisible(FRMPropertiesPageEditor2, 1);
            WAControlVisible(FRMPropertiesPageLineNumbering, 1);
            WAControlVisible(FRMPropertiesPageBackup, 1);
            WAControlVisible(FRMPropertiesTbBackup, 1);
            WAControlBringToTop(FRMPropertiesTbBackup);
            WAControlBringToTop(FRMPropertiesTbFileExt);
            WAControlBringToTop(FRMPropertiesPageEditor);
			break;
		case 1:
            WAControlVisible(FRMPropertiesPageEditor, 0);
            WAControlVisible(FRMPropertiesPageFileExtensions, 0);
            WAControlVisible(FRMPropertiesTbFileExt, 0);
            WAControlVisible(FRMPropertiesPageOther, 0);
            WAControlVisible(FRMPropertiesPageFont, 0);
            WAControlVisible(FRMPropertiesCmdFontChoose, 0);
            WAControlVisible(FRMPropertiesPageAutoSave, 0);
            WAControlVisible(FRMPropertiesPageEditor2, 0);
            WAControlVisible(FRMPropertiesPageLineNumbering, 0);
            WAControlVisible(FRMPropertiesPageBackup, 0);
            WAControlVisible(FRMPropertiesTbBackup, 0);
            WAControlVisible(FRMPropertiesPageFolders, 0);
            WAControlVisible(FRMPropertiesTbROOTDIR, 0);
            WAControlVisible(FRMPropertiesTbBINDIR, 0);
            WAControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            WAControlVisible(FRMPropertiesTbLIBDIR, 0);
            WAControlVisible(FRMPropertiesTbHELPDIR, 0);
            WAControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            WAControlVisible(FRMPropertiesPageProjects, 0);
            WAControlVisible(FRMPropertiesCmdDebugger, 0);
            WAControlVisible(FRMPropertiesTbValidDebugger, 0);
            WAControlVisible(FRMPropertiesPageProtoGen, 0);
            WAControlVisible(FRMPropertiesCmdGenProg, 0);
            WAControlVisible(FRMPropertiesPageFTPManage, 0);
            WAControlVisible(FRMPropertiesTbAccounts, 0);
            WAControlVisible(FRMPropertiesPageAccountInfos, 0);
            WAControlVisible(FRMPropertiesPageTransfersTypes, 0);
            WAControlVisible(FRMPropertiesPageCodingHelp, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            BuildPreview(FRMPropertiesCodeMax);
            WAControlVisible(FRMPropertiesPageColors, 1);
            WAControlVisible(FRMPropertiesTbColors, 1);
            WAControlVisible(FRMPropertiesPageAPIHelp, 1);
            WAControlVisible(FRMPropertiesCmdWindowsAPI, 1);
            WAControlVisible(FRMPropertiesPageSDKHelp, 1);
            WAControlVisible(FRMPropertiesCmdMSDN, 1);
            WAControlVisible(FRMPropertiesPageCodeMax, 1);
            WAControlVisible(FRMPropertiesPageDDKHelp, 1);
            WAControlVisible(FRMPropertiesCmdDDK, 1);
            WAControlVisible(FRMPropertiesPageDirectXSDKHelp, 1);
            WAControlVisible(FRMPropertiesCmdDirectXSDK, 1);
            WAControlVisible(FRMPropertiesPageExtrasLayouts, 1);
            WAControlVisible(FRMPropertiesCmdRegs, 1);
            WAControlBringToTop(FRMPropertiesTbColors);
			break;
		case 2:
            WAControlVisible(FRMPropertiesPageEditor, 0);
            WAControlVisible(FRMPropertiesPageFileExtensions, 0);
            WAControlVisible(FRMPropertiesTbFileExt, 0);
            WAControlVisible(FRMPropertiesPageOther, 0);
            WAControlVisible(FRMPropertiesPageFont, 0);
            WAControlVisible(FRMPropertiesCmdFontChoose, 0);
            WAControlVisible(FRMPropertiesPageAutoSave, 0);
            WAControlVisible(FRMPropertiesPageEditor2, 0);
            WAControlVisible(FRMPropertiesPageLineNumbering, 0);
            WAControlVisible(FRMPropertiesPageBackup, 0);
            WAControlVisible(FRMPropertiesTbBackup, 0);
            WAControlVisible(FRMPropertiesPageColors, 0);
            WAControlVisible(FRMPropertiesTbColors, 0);
            WAControlVisible(FRMPropertiesPageAPIHelp, 0);
            WAControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            WAControlVisible(FRMPropertiesPageSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdMSDN, 0);
            WAControlVisible(FRMPropertiesPageCodeMax, 0);
            WAControlVisible(FRMPropertiesPageDDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDDK, 0);
            WAControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            WAControlVisible(FRMPropertiesCmdRegs, 0);
            WAControlVisible(FRMPropertiesPageFTPManage, 0);
            WAControlVisible(FRMPropertiesTbAccounts, 0);
            WAControlVisible(FRMPropertiesPageAccountInfos, 0);
            WAControlVisible(FRMPropertiesPageTransfersTypes, 0);
            WAControlVisible(FRMPropertiesPageCodingHelp, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            WAControlVisible(FRMPropertiesPageFolders, 1);
            WAControlVisible(FRMPropertiesTbROOTDIR, 1);
            WAControlVisible(FRMPropertiesTbBINDIR, 1);
            WAControlVisible(FRMPropertiesTbINCLUDEDIR, 1);
            WAControlVisible(FRMPropertiesTbLIBDIR, 1);
            WAControlVisible(FRMPropertiesTbHELPDIR, 1);
            WAControlVisible(FRMPropertiesTbPROJECTSDIR, 1);
            WAControlVisible(FRMPropertiesPageProjects, 1);
            WAControlVisible(FRMPropertiesCmdDebugger, 1);
            WAControlVisible(FRMPropertiesTbValidDebugger, 1);
            WAControlVisible(FRMPropertiesPageProtoGen, 1);
            WAControlVisible(FRMPropertiesCmdGenProg, 1);
            WAControlBringToTop(FRMPropertiesTbROOTDIR);
            WAControlBringToTop(FRMPropertiesTbBINDIR);
            WAControlBringToTop(FRMPropertiesTbINCLUDEDIR);
            WAControlBringToTop(FRMPropertiesTbLIBDIR);
            WAControlBringToTop(FRMPropertiesTbHELPDIR);
            WAControlBringToTop(FRMPropertiesTbPROJECTSDIR);
            WAControlBringToTop(FRMPropertiesTbValidDebugger);
			break;
		case 3:
            WAControlVisible(FRMPropertiesPageEditor, 0);
            WAControlVisible(FRMPropertiesPageFileExtensions, 0);
            WAControlVisible(FRMPropertiesTbFileExt, 0);
            WAControlVisible(FRMPropertiesPageOther, 0);
            WAControlVisible(FRMPropertiesPageFont, 0);
            WAControlVisible(FRMPropertiesCmdFontChoose, 0);
            WAControlVisible(FRMPropertiesPageAutoSave, 0);
            WAControlVisible(FRMPropertiesPageEditor2, 0);
            WAControlVisible(FRMPropertiesPageLineNumbering, 0);
            WAControlVisible(FRMPropertiesPageBackup, 0);
            WAControlVisible(FRMPropertiesTbBackup, 0);
            WAControlVisible(FRMPropertiesPageColors, 0);
            WAControlVisible(FRMPropertiesTbColors, 0);
            WAControlVisible(FRMPropertiesPageAPIHelp, 0);
            WAControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            WAControlVisible(FRMPropertiesPageSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdMSDN, 0);
            WAControlVisible(FRMPropertiesPageCodeMax, 0);
            WAControlVisible(FRMPropertiesPageDDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDDK, 0);
            WAControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            WAControlVisible(FRMPropertiesCmdRegs, 0);
            WAControlVisible(FRMPropertiesPageFolders, 0);
            WAControlVisible(FRMPropertiesTbROOTDIR, 0);
            WAControlVisible(FRMPropertiesTbBINDIR, 0);
            WAControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            WAControlVisible(FRMPropertiesTbLIBDIR, 0);
            WAControlVisible(FRMPropertiesTbHELPDIR, 0);
            WAControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            WAControlVisible(FRMPropertiesPageProjects, 0);
            WAControlVisible(FRMPropertiesCmdDebugger, 0);
            WAControlVisible(FRMPropertiesTbValidDebugger, 0);
            WAControlVisible(FRMPropertiesPageProtoGen, 0);
            WAControlVisible(FRMPropertiesCmdGenProg, 0);
            WAControlVisible(FRMPropertiesPageCodingHelp, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            WAControlVisible(FRMPropertiesPageFTPManage, 1);
            WAControlVisible(FRMPropertiesTbAccounts, 1);
            WAControlVisible(FRMPropertiesPageAccountInfos, 1);
            WAControlVisible(FRMPropertiesPageTransfersTypes, 1);
            WAControlBringToTop(FRMPropertiesTbAccounts);
			break;
		case 4:
            WAControlVisible(FRMPropertiesPageEditor, 0);
            WAControlVisible(FRMPropertiesPageFileExtensions, 0);
            WAControlVisible(FRMPropertiesTbFileExt, 0);
            WAControlVisible(FRMPropertiesPageOther, 0);
            WAControlVisible(FRMPropertiesPageFont, 0);
            WAControlVisible(FRMPropertiesCmdFontChoose, 0);
            WAControlVisible(FRMPropertiesPageAutoSave, 0);
            WAControlVisible(FRMPropertiesPageEditor2, 0);
            WAControlVisible(FRMPropertiesPageLineNumbering, 0);
            WAControlVisible(FRMPropertiesPageBackup, 0);
            WAControlVisible(FRMPropertiesTbBackup, 0);
            WAControlVisible(FRMPropertiesPageColors, 0);
            WAControlVisible(FRMPropertiesTbColors, 0);
            WAControlVisible(FRMPropertiesPageAPIHelp, 0);
            WAControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            WAControlVisible(FRMPropertiesPageSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdMSDN, 0);
            WAControlVisible(FRMPropertiesPageCodeMax, 0);
            WAControlVisible(FRMPropertiesPageDDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDDK, 0);
            WAControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            WAControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            WAControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            WAControlVisible(FRMPropertiesCmdRegs, 0);
            WAControlVisible(FRMPropertiesPageFolders, 0);
            WAControlVisible(FRMPropertiesTbROOTDIR, 0);
            WAControlVisible(FRMPropertiesTbBINDIR, 0);
            WAControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            WAControlVisible(FRMPropertiesTbLIBDIR, 0);
            WAControlVisible(FRMPropertiesTbHELPDIR, 0);
            WAControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            WAControlVisible(FRMPropertiesPageProjects, 0);
            WAControlVisible(FRMPropertiesCmdDebugger, 0);
            WAControlVisible(FRMPropertiesTbValidDebugger, 0);
            WAControlVisible(FRMPropertiesPageProtoGen, 0);
            WAControlVisible(FRMPropertiesCmdGenProg, 0);
            WAControlVisible(FRMPropertiesPageFTPManage, 0);
            WAControlVisible(FRMPropertiesTbAccounts, 0);
            WAControlVisible(FRMPropertiesPageAccountInfos, 0);
            WAControlVisible(FRMPropertiesPageTransfersTypes, 0);
            WAControlVisible(FRMPropertiesPageCodingHelp, 1);
            WAControlVisible(FRMPropertiesPageExtrasLayouts2, 1);
    }
}

// -----------------------------------------------------------------------
// Delete an account in ftpaccounts ini file
void DeleteAccountFromIni(CStr AccountToDelete)
{
    int i = 0;

    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(strcmpi(AccountToDelete.Get_String(), FRMPropertiesRetVal.Get_String()) == 0)
        {
            WAIniDeleteKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPIP", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPAnon", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPUserName", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPUserPass", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPSavePass ", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPPath", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPPort", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPUseProxy", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPTransfersType", FtpAccountsIniFile);
            WAIniDeleteKey(AccountToDelete, "FTPPassive", FtpAccountsIniFile);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Read skins definitions
void Fillskins(void)
{
    int i = 0;
    CStr SkinToAdd;
    long SelectedIdx = 0;
    
	PrgSkinArray.Erase();
    PrfCurrentSkin = WAIniReadKey("BuildSkins", "CurrentSkin", SkinsIniFile);
    PrfCurrentSkin = ChangeRelativePaths(PrfCurrentSkin);
    for(i = 0; i <= 999; i++)
    {
        SkinToAdd = WAIniReadKey("BuildSkins", "Skin" + (CStr) StringNumberComplement(i, 3).Get_String(), SkinsIniFile);
        if(SkinToAdd.Len() == 0) break;
        SkinToAdd = ChangeRelativePaths(SkinToAdd);
        if(strcmpi(SkinToAdd.Get_String(), PrfCurrentSkin.Get_String()) == 0) SelectedIdx = i;
        PrgSkinArray.Add(SkinToAdd.Get_String());
        SkinToAdd = WAIniReadKey("SKINFOS", "Title", SkinToAdd);
        WAComboBoxAddItem(FRMPropertiesCbSkin, SkinToAdd, -1);
    }
    WAComboBoxSetIndex(FRMPropertiesCbSkin, SelectedIdx);
}

// -----------------------------------------------------------------------
// Read skins datas
void FillCurrentSkin(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    long ctId = 0;
    long CurrentRadio = 0;
    long FirstRadio = 0;
    long *ElementArray = 0;
    HWND ElementLabel = 0;
    CStr Skinelement;
    HWND ElementhWnd = 0;
    HWND ElementFrame = 0;
    CStr ElementName;
    CStr ElementCoords;
    CStr ElementSelected;
    CStr BufString;

    PrgSkinControls.Erase();
    PrgSkinControlsDest.Erase();
    PrgSkinLabels.Erase();
    SkinListViews.Erase();
    ctId = 22;               // Minimum ID
    for(i = 0; i <= 999; i++)
    {
        Skinelement = WAIniReadKey("TEMPHEADER", "Opt" + (CStr) StringNumberComplement(i, 3).Get_String(), PrfCurrentSkin);
        if(Skinelement.Len() == 0) break;
		BufString = Skinelement.Left(5).Upper_Case();
        if(BufString == "COMBO")
        {
            ElementName = WAIniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = WAIniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = WACreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = WAIniReadKey(Skinelement, "ComboDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = WACreateComboBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, "", ctId, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    for(j = 0; j <= 999; j++)
                    {
                        ElementName = WAIniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String(), PrfCurrentSkin);
                        if(ElementName.Len() == 0) break;
                        WAComboBoxAddItem(ElementhWnd, ElementName, -1);
                    }
                    ElementSelected = WAIniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                    if(ElementSelected.Len() == 0) ElementSelected = "0";
                    WAComboBoxSetIndex(ElementhWnd, ElementSelected.Get_Long());
                }
            }
        }
        else if(BufString == "RADIO")
        {
            ElementName = WAIniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = WAIniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementFrame = WACreateFrame(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementFrame);
                ctId++;
                FirstRadio = WS_GROUP;
                for(j = 0; j <= 999; j++)
                {
                    ElementName = WAIniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String(), PrfCurrentSkin);
                    if(ElementName.Len() == 0) break;
                    ElementCoords = WAIniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String() + (CStr) "Dat", PrfCurrentSkin);
                    if(ElementCoords.Len() != 0)
                    {
                        ElementArray = StringSplit(ElementCoords, "|");
                        ElementhWnd = WACreateRadioButton(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), ElementFrame, ElementName, ctId, 0, WS_TABSTOP | FirstRadio);
                        StringReleaseSplit(ElementArray);
                        BufString = Skinelement + (CStr) j;
						PrgSkinControlsDest.Add(BufString.Get_String());
                        PrgSkinControls.Add(ElementhWnd);
                        ctId++;
                        ElementSelected = WAIniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                        if(ElementSelected.Len() == 0) ElementSelected = "0";
                        CurrentRadio = ElementSelected.Get_Long();
                        if((long) CurrentRadio == j) RadioButtonSetState(ElementhWnd, 1);
                        FirstRadio = 0;
                    }
                }
            }
        }
        else if(BufString == "CHECK")
        {
            ElementName = WAIniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = WAIniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementhWnd = WACreateCheckBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, WS_TABSTOP, 0);
                StringReleaseSplit(ElementArray);
                ctId++;
                PrgSkinControlsDest.Add(Skinelement.Get_String());
                PrgSkinControls.Add(ElementhWnd);
                ElementSelected = WAIniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                if(ElementSelected.Len() == 0) ElementSelected = "0";
                CheckBoxSetState(ElementhWnd, ElementSelected.Get_Long());
            }
        }
        else if(BufString == "TEXTB")
        {
            ElementName = WAIniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = WAIniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = WACreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = WAIniReadKey(Skinelement, "TextDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = WACreateTextBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, "", ctId, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    WAControlSetText(ElementhWnd, WAIniReadKey("CURRENT", Skinelement, PrfCurrentSkin));
                }
            }
        }
        else if(BufString == "LISTC")
        {
            ElementName = WAIniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = WAIniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = WACreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = WAIniReadKey(Skinelement, "ListDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = WACreateListView(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ctId, 0, PropStepListViewProc, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
                    WAListViewAddCol(ElementhWnd, "", StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long() - 23, 0);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    SkinListViews.Add(ElementhWnd);
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    for(k = 0; k <= 999; k++)
                    {
                        ElementName = WAIniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(k, 3).Get_String(), PrfCurrentSkin);
                        if(ElementName.Len() == 0) break;
                        WAListViewAddItem(ElementhWnd, ElementName, k, -1);
                        ElementSelected = WAIniReadKey("CURRENT", Skinelement + (CStr) StringNumberComplement(k, 3).Get_String(), PrfCurrentSkin);
                        if(ElementSelected.Len() == 0) ElementSelected = "0";
                        WAListViewSetItemCheckbox(ElementhWnd, k, ElementSelected.Get_Long());
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Project building Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameBuilding(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CStr TempCurrentSkin;
    int i = 0;

    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCbSkin)
            {
                if(WAControlGetNotifiedCommand(wParam) == CBN_SELCHANGE)
                {
                    TempCurrentSkin = PrgSkinArray.Get(WAComboBoxGetIndex(FRMPropertiesCbSkin))->Content;
                    if(strcmpi(TempCurrentSkin.Get_String(), PrfCurrentSkin.Get_String()) != 0)
                    {
                        ClearSkincontrols();
                        WAControlRefresh(FRMPropertiesPageProjects);
                        PrfCurrentSkin = TempCurrentSkin;
                        FillCurrentSkin();
                    }
                    return(0);
                }
            }
			break;
        case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMStepListView)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        WAListViewCheckBoxItemDoubleClick(FRMStepListView);
                        return(0);
                }
            }
            else
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        for(i = 0; i < SkinListViews.Amount(); i++)
                        {
                            if(SkinListViews.Get(i)->Content != 0)
                            {
                                if(SkinListViews.Get(i)->Content == WAControlGetNotifiedhWnd(lParam))
                                {
                                    WAListViewCheckBoxItemDoubleClick(SkinListViews.Get(i)->Content);
                                    return(0);
                                }
                            }
                        }
                }
            }
			break;
		case WM_ERASEBKGND:
            WAGDIDrawHorzSep(hWnd, 2, 129, 238);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Clear skin screen
void ClearSkincontrols(void)
{
    int i = 0;

    for(i = 0; i < PrgSkinLabels.Amount(); i++)
    {
        DestroyWindow(PrgSkinLabels.Get(i)->Content);
    }
    for(i = 0; i < PrgSkinControls.Amount(); i++)
    {
        DestroyWindow(PrgSkinControls.Get(i)->Content);
    }
}

// -----------------------------------------------------------------------
// Fill the default language combo
void FillDefLanguage(void)
{
    long AsmPos = 0;
    CStr OldDefault;
    int i = 0;

    OldDefault = WAIniReadKey("RefLanguages", "DefLang", LanguagesIniFile);
	WAComboBoxAddItem(FRMPropertiesCbDefLang, "(None)", -1);
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = WAIniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        FRMPropertiesRetVal = ChangeRelativePaths(FRMPropertiesRetVal);
        FRMPropertiesRetVal = WAIniReadKey("LangSpec", "LangName", FRMPropertiesRetVal);
        if(FRMPropertiesRetVal.Len() != 0)
        {
            if(OldDefault.Len() != 0)
            {
                // Retrieve index of selected language
                if(strcmpi(FRMPropertiesRetVal.Get_String(), OldDefault.Get_String()) == 0) AsmPos = i + 1;
            }
            WAComboBoxAddItem(FRMPropertiesCbDefLang, FRMPropertiesRetVal, -1);
        }
    }
    WAComboBoxSetIndex(FRMPropertiesCbDefLang, AsmPos);
}

// -----------------------------------------------------------------------
// Select debugger
void GetDebugger(HWND hWnd)
{
    CStr LdFile;
    CStr OpFilters;

    OpFilters = "Exe files (*.exe;*.com)|*.exe;*.com";
    LdFile = WAComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    // Save the old entry into the combo
    //WAComboBoxSaveInIniFile(hWnd, LdFile, "ExtDebuggers", MainIniFile);
    //AddNewDebugger(hWnd, LdFile);
    WAControlSetText(hWnd, LdFile);
}

// -----------------------------------------------------------------------
// Hack here coz listview control sucks badly
LRESULT CALLBACK PropStepListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            WAControlBringToTop(hWnd);
            WAListViewSetItemFocus(hWnd, WAListViewGetFirstFocusItem(hWnd));
            return(1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Add the new external debugger entry
void AddNewDebugger(HWND hCombo, CStr Debugger)
{
    int i;
    int Found = FALSE;

    for(i = 0; i < WAComboBoxCount(hCombo); i++)
    {
        if(strcmpi(WAComboBoxGetItem(hCombo, i).Get_String(), Debugger.Get_String()) == 0)
        {
            Found = TRUE;
            break;
        }
    }
    if(!Found) WAComboBoxAddItem(hCombo, Debugger, -1);
}
