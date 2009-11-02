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
// Dlg_PrjProps.cpp: Project properties form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ProjectsBuilder.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_Constants.h"
#include "Dlg_PrjProps.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMPrjPropshWnd;
HWND FRMPrjPropsFr1;
HWND FRMPrjPropsFr2;
HWND FRMPrjPropsFr3;
HWND FRMPrjPropsTxtTitle;
HWND FRMPrjPropsFileType;
HWND FRMPrjPropsSetIcon;
HWND FRMPrjPropsIcon;
HWND FRMPrjPropsConstants;
HANDLE FRMPrjPropsIconhandle;
HWND FRMPrjPropsTxtMajor;
HWND FRMPrjPropsTxtMinor;
HWND FRMPrjPropsTxtRevision;
HWND FRMPrjPropsTxtRelease;
HWND FRMPrjPropsOS;
HWND FRMPrjPropsTypeProp;
HWND FRMPrjPropsTypeTxt;

HWND FRMPrjPropsIncludeVersion;
HWND FRMPrjPropsIncludeXPManifest;

HWND FRMPrjPropsReg;
HWND FRMPrjPropsOk;
HWND FRMPrjPropsCancel;

CStr LastPrjIconDir;
long LastListBoxIndex;

CStr TmpPrj_Comments;
CStr TmpPrj_CompanyName;
CStr TmpPrj_FileDescription;
CStr TmpPrj_LegalCopyRight;
CStr TmpPrj_LegalTrademarks;
CStr TmpPrj_ProductName;

CStr PrjDir;
CStr NewIconFile;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMPrjPropsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    CStr BufString;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMPrjPropshWnd = hwndDlg;
            WAControlSetText(hwndDlg, "Project properties");
            FRMPrjPropsFr1 = WACreateFrame(2, -1, 202, 108, hwndDlg, "Application", 0, 0, 0);
            FRMPrjPropsFr2 = WACreateFrame(206, -1, 194, 108, hwndDlg, "Version", 0, 0, 0);
            FRMPrjPropsFr3 = WACreateFrame(2, 108, 398, 94, hwndDlg, "Version informations", 14, &FRMPrjPropsFrame3, 0);
            WACreateLabel(5, 21 + 20, 35, 15, FRMPrjPropsFr1, "Title :", 0, 0, 0, 0);
            WACreateLabel(5, 41 + 25, 52, 15, FRMPrjPropsFr1, "File type :", 0, 0, 0, 0);
            WACreateLabel(131, 41 + 25, 34, 15, FRMPrjPropsFr1, "Icon :", 0, 0, 0, 0);
            FRMPrjPropsOk = WACreateButton(243, 214, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsCancel = WACreateButton(322, 214, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsReg = WACreateButton(159, 214, 77, 23, hwndDlg, "Register", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsConstants = WACreateButton(3, 214, 77, 23, hwndDlg, "Constants", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsIncludeVersion = WACreateCheckBox(7, 18, 140, 15, FRMPrjPropsFr1, "Include version resource", 5, 0, WS_TABSTOP | WS_GROUP, 0);
            FRMPrjPropsTxtTitle = WACreateTextBox(36, 18 + 20, 159, 20, FRMPrjPropsFr1, "", 6, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            FRMPrjPropsFileType = WACreateComboBox(5, 56 + 25, 123, 150, FRMPrjPropsFr1, "", 7, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            WAComboBoxAddItem(FRMPrjPropsFileType, "UNKNOWN", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "APP", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "DLL", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "DRV", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "FONT", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "VXD", -1);
            WAComboBoxAddItem(FRMPrjPropsFileType, "STATIC_LIB", -1);
            WAComboBoxSetIndex(FRMPrjPropsFileType, 0);
            FRMPrjPropsSetIcon = WACreateButton(133, 56 + 25, 25, 21, hwndDlg, "...", 8, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            // Current icon
			if(ProjectTypeScheduledImg.Len()) BufString = ProjectTypeScheduledImg;
			else BufString = ProjectDir + (CStr) "\\" + (CStr) ProjectTypeImg;
			FRMPrjPropsIconhandle = LoadImage(0, BufString.Get_String(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            FRMPrjPropsIcon = WACreatePictureBox(163, 43 + 25, 35, 34, FRMPrjPropsFr1, FRMPrjPropsIconhandle, IMAGE_ICON, 9, 0, SS_CENTERIMAGE);
            WACreateLabel(7, 15, 52, 13, FRMPrjPropsFr2, "Major :", 0, 0, 0, 0);
            WACreateLabel(99, 15, 52, 13, FRMPrjPropsFr2, "Minor :", 0, 0, 0, 0);
            WACreateLabel(7, 35, 52, 13, FRMPrjPropsFr2, "Revision :", 0, 0, 0, 0);
            WACreateLabel(99, 35, 52, 13, FRMPrjPropsFr2, "Release :", 0, 0, 0, 0);
            FRMPrjPropsTxtMajor = WACreateTextBox(59, 12, 33, 20, FRMPrjPropsFr2, "", 10, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtMinor = WACreateTextBox(151, 12, 33, 20, FRMPrjPropsFr2, "", 11, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtRevision = WACreateTextBox(59, 32, 33, 20, FRMPrjPropsFr2, "", 12, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtRelease = WACreateTextBox(151, 32, 33, 20, FRMPrjPropsFr2, "", 13, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            WACreateLabel(7, 61, 31, 15, FRMPrjPropsFr2, "OS :", 0, 0, 0, 0);
            FRMPrjPropsOS = WACreateComboBox(34, 57, 150, 150, FRMPrjPropsFr2, "", 14, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            WAComboBoxAddItem(FRMPrjPropsOS, "UNKNOWN", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "DOS", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "NT", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "WINDOWS16", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "WINDOWS32", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "DOS_WINDOWS16", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "DOS_WINDOWS32", -1);
            WAComboBoxAddItem(FRMPrjPropsOS, "NT_WINDOWS32", -1);
            WAComboBoxSetIndex(FRMPrjPropsOS, 0);
            FRMPrjPropsIncludeXPManifest = WACreateCheckBox(16, 85, 170, 15, FRMPrjPropsFr2, "Include Windows XP manifest", 15, 0, WS_TABSTOP | WS_GROUP, 0);
            WACreateLabel(5, 16, 52, 13, FRMPrjPropsFr3, "Type :", 0, 0, 0, 0);
            FRMPrjPropsTypeProp = WACreateListBox(5, 30, 140, 59, FRMPrjPropsFr3, 16, 0, 0, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
            WAListBoxAddItem(FRMPrjPropsTypeProp, "Comments", -1);
            WAListBoxAddItem(FRMPrjPropsTypeProp, "Company name", -1);
            WAListBoxAddItem(FRMPrjPropsTypeProp, "File description", -1);
            WAListBoxAddItem(FRMPrjPropsTypeProp, "Legal copyright", -1);
            WAListBoxAddItem( FRMPrjPropsTypeProp, "Legal trademarks", -1);
            WAListBoxAddItem(FRMPrjPropsTypeProp, "Product name", -1);
            WAListBoxSetIndex(FRMPrjPropsTypeProp, 0);
            FRMPrjPropsTypeTxt = WACreateTextBox(147, 13 - 2, 245, 79, FRMPrjPropsFr3, "", 17, &FRMPrjPropsTypeTxtHook, WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_WANTRETURN, WS_EX_STATICEDGE);
            LoadUserInfos();
            WAControlSetText(FRMPrjPropsTxtMajor, Prj_Major);
            WAControlSetText(FRMPrjPropsTxtMinor, Prj_Minor);
            WAControlSetText(FRMPrjPropsTxtRevision, Prj_Revision);
            WAControlSetText(FRMPrjPropsTxtRelease, Prj_Release);
            CheckBoxSetState(FRMPrjPropsIncludeVersion, Prj_IncludeVersion);
            CheckBoxSetState(FRMPrjPropsIncludeXPManifest, Prj_IncludeManifest);
            WAComboBoxSetIndex(FRMPrjPropsOS, Prj_OS);
            WAComboBoxSetIndex(FRMPrjPropsFileType, Prj_FileType);
            WAControlSetText(FRMPrjPropsTxtTitle, ProjectTitle);
            TmpPrj_Comments = Prj_Comments;
            if(Prj_CompanyName.Len()) TmpPrj_CompanyName = Prj_CompanyName;
            TmpPrj_FileDescription = Prj_FileDescription;
            if(Prj_LegalCopyRight.Len()) TmpPrj_LegalCopyRight = Prj_LegalCopyRight;
            if(Prj_LegalTrademarks.Len()) TmpPrj_LegalTrademarks = Prj_LegalTrademarks;
            TmpPrj_ProductName = Prj_ProductName;
            WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_Comments);
            FreezeTimer = 1;
            return(1);
        case WM_COMMAND:
            if((HWND) lParam == FRMPrjPropsOk)
            {
				ProjectTypeScheduledImg = NewIconFile;
                SaveProjectProperties();
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsConstants)
            {
                CreateConstants(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsReg)
            {
                RegisterUserInfos();
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsSetIcon)
            {
                SetProjectIcon();
                return(0);
            }
			break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            WAGDIDrawHorzSep(hwndDlg, 0, 207, 402);
            EndPaint(hwndDlg, &CreatePs);
			break;
		case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Load user infos from main ini file
void LoadUserInfos(void)
{
    TmpPrj_CompanyName = StringReplace(WAIniReadKey("UserInfos", "CompanyName", MainIniFile), TmpPrj_CompanyName.Chr(2), TmpPrj_CompanyName.Chr(13) + TmpPrj_CompanyName.Chr(10), 1, -1, Binary_Compare);
	TmpPrj_LegalCopyRight = StringReplace(WAIniReadKey("UserInfos", "LegalCopyRight", MainIniFile), TmpPrj_LegalCopyRight.Chr(2), TmpPrj_LegalCopyRight.Chr(13) + TmpPrj_LegalCopyRight.Chr(10), 1, -1, Binary_Compare);
	TmpPrj_LegalTrademarks = StringReplace(WAIniReadKey("UserInfos", "LegalTrademarks", MainIniFile), TmpPrj_LegalTrademarks.Chr(2), TmpPrj_LegalTrademarks.Chr(13) + TmpPrj_LegalTrademarks.Chr(10), 1, -1, Binary_Compare);
}

// -----------------------------------------------------------------------
// Register user infos in main ini file
void RegisterUserInfos(void)
{
    WAIniWriteKey("UserInfos", "CompanyName", StringReplace(TmpPrj_CompanyName, TmpPrj_CompanyName.Chr(13) + TmpPrj_CompanyName.Chr(10), TmpPrj_CompanyName.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    WAIniWriteKey("UserInfos", "LegalCopyRight", StringReplace(TmpPrj_LegalCopyRight, TmpPrj_LegalCopyRight.Chr(13) + TmpPrj_LegalCopyRight.Chr(10), TmpPrj_LegalCopyRight.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    WAIniWriteKey("UserInfos", "LegalTrademarks", StringReplace(TmpPrj_LegalTrademarks, TmpPrj_LegalTrademarks.Chr(13) + TmpPrj_LegalTrademarks.Chr(10), TmpPrj_LegalTrademarks.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    WAMiscMsgBox(hMDIform.hWnd, "User infos registered.", MB_INFORMATION, Requesters);
}

// -----------------------------------------------------------------------
// Version frame
LRESULT CALLBACK FRMPrjPropsFrame3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPrjPropsTypeProp)
            {
                switch(WAListBoxGetSelItemIndex(FRMPrjPropsTypeProp))
                {
                    case 0:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_Comments);
                        LastListBoxIndex = 0;
						break;
                    case 1:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_CompanyName);
                        LastListBoxIndex = 1;
						break;
                    case 2:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_FileDescription);
                        LastListBoxIndex = 2;
						break;
                    case 3:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_LegalCopyRight);
                        LastListBoxIndex = 3;
						break;
                    case 4:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_LegalTrademarks);
                        LastListBoxIndex = 4;
						break;
                    case 5:
                        WAControlSetText(FRMPrjPropsTypeTxt, TmpPrj_ProductName);
                        LastListBoxIndex = 5;
						break;
                }
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsTypeTxt)
            {
                switch(WAControlGetNotifiedCommand(wParam))
                {
                    case EN_CHANGE:
                        switch(WAListBoxGetSelItemIndex(FRMPrjPropsTypeProp))
                        {
                            case 0:
                                TmpPrj_Comments = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
				            case 1:
                                TmpPrj_CompanyName = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
                            case 2:
                                TmpPrj_FileDescription = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
                            case 3:
                                TmpPrj_LegalCopyRight = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
                            case 4:
                                TmpPrj_LegalTrademarks = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
                            case 5:
                                TmpPrj_ProductName = WAControlGetText(FRMPrjPropsTypeTxt);
								break;
                        }
						break;
                }
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// A hack to avoid some repainting problems with the parent frame
LRESULT CALLBACK FRMPrjPropsTypeTxtHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            if(wParam == 27)
            {
                WAControlSendMessage(FRMPrjPropshWnd, WM_CLOSE, (long) FRMPrjPropsCancel);
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Change the icon associated with the project
void SetProjectIcon(void)
{
    CStr LdFile;
	CStr OpFilters;
    CStr IconDir;

    OpFilters = "Icon files (*.ico)|*.ico";
    LdFile = WAComDlgGetOpenFileName(FRMPrjPropshWnd, OpFilters, LastPrjIconDir, 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    IconDir = WAFileGetDirectory(LdFile);
    if(strcmp(IconDir.Right(1).Get_String(), "\\") == 0) IconDir = IconDir.Left(IconDir.Len() - 1);
    LastPrjIconDir = IconDir;
    NewIconFile = LdFile;
	PictureBoxChangeIcon(FRMPrjPropsIcon, (HICON) LoadImage(0, LdFile.Get_String(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
}

// -----------------------------------------------------------------------
// Dispatch the properties
void SaveProjectProperties(void)
{
    CStr BufString;

	Prj_Major = 0;
    Prj_Minor = 0;
    Prj_Revision = 0;
    Prj_Release = 0;
    if(WAControlGetText(FRMPrjPropsTxtMajor).Len() != 0) Prj_Major = WAControlGetText(FRMPrjPropsTxtMajor).Get_Long();
    if(WAControlGetText(FRMPrjPropsTxtMinor).Len() != 0) Prj_Minor = WAControlGetText(FRMPrjPropsTxtMinor).Get_Long();
    if(WAControlGetText(FRMPrjPropsTxtRevision).Len() != 0) Prj_Revision = WAControlGetText(FRMPrjPropsTxtRevision).Get_Long();
    if(WAControlGetText(FRMPrjPropsTxtRelease).Len() != 0) Prj_Release = WAControlGetText(FRMPrjPropsTxtRelease).Get_Long();
    Prj_IncludeVersion = CheckBoxGetState(FRMPrjPropsIncludeVersion);
    Prj_IncludeManifest = CheckBoxGetState(FRMPrjPropsIncludeXPManifest);
    if(WAComboBoxGetIndex(FRMPrjPropsOS) == -1) WAComboBoxSetIndex(FRMPrjPropsOS, 0);
    Prj_OS = WAComboBoxGetIndex(FRMPrjPropsOS);
    if(WAComboBoxGetIndex(FRMPrjPropsFileType) == -1) WAComboBoxSetIndex(FRMPrjPropsFileType, 0);
    Prj_FileType = WAComboBoxGetIndex(FRMPrjPropsFileType);
    ProjectTitle = WAControlGetText(FRMPrjPropsTxtTitle);
    Prj_Comments = TmpPrj_Comments;
    Prj_CompanyName = TmpPrj_CompanyName;
    Prj_FileDescription = TmpPrj_FileDescription;
    Prj_LegalCopyRight = TmpPrj_LegalCopyRight;
    Prj_LegalTrademarks = TmpPrj_LegalTrademarks;
    Prj_ProductName = TmpPrj_ProductName;
    ProjectModified = TRUE;
}
