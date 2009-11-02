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
// Dlg_Perms.cpp: Change files permissions form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Form.h"
#include "Globals.h"
#include "Dlg_FTP.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMProcPermshwnd;
HWND FRMPermsOk;
HWND FRMPermsCancel;
HWND FRMPermsFrameOwner;
HWND FRMPermsFrameGroup;
HWND FRMPermsFrameOther;
HWND FRMPermsChkOwnerR;
HWND FRMPermsChkOwnerW;
HWND FRMPermsChkOwnerE;
HWND FRMPermsChkGroupR;
HWND FRMPermsChkGroupW;
HWND FRMPermsChkGroupE;
HWND FRMPermsChkOtherR;
HWND FRMPermsChkOtherW;
HWND FRMPermsChkOtherE;
long PermChkUndef;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMProcPerms(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT EnterValuePerms;
    
	switch(uMsg)
	{
        case WM_INITDIALOG:
            WAControlSetText(hwndDlg, "Change directories/files permissions");
            FRMProcPermshwnd = hwndDlg;
            FRMPermsFrameOwner = WACreateFrame(2, -1, 98, 72, hwndDlg, "Owner", 0, 0, 0);
            FRMPermsFrameGroup = WACreateFrame(2 + 101, -1, 98, 72, hwndDlg, "Group", 0, 0, 0);
            FRMPermsFrameOther = WACreateFrame(2 + (101 * 2), -1, 98, 72, hwndDlg, "Other", 0, 0, 0);
            if(PermOwnerR == 0)
            {
                FRMPermsChkOwnerR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOwner, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
            }
            else if(PermOwnerR == PermNbrFiles)
            {
                FRMPermsChkOwnerR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOwner, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
                CheckBoxSetState(FRMPermsChkOwnerR, 1);
            }
            else
            {
                FRMPermsChkOwnerR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOwner, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 1);
                CheckBoxSetState(FRMPermsChkOwnerR, BST_INDETERMINATE);
            }
            if(PermOwnerW == 0)
            {
                FRMPermsChkOwnerW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOwner, "Write", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermOwnerW == PermNbrFiles)
            {
                FRMPermsChkOwnerW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOwner, "Write", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkOwnerW, 1);
            }
            else
            {
                FRMPermsChkOwnerW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOwner, "Write", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkOwnerW, BST_INDETERMINATE);
            }
            if(PermOwnerX == 0)
            {
                FRMPermsChkOwnerE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOwner, "Execute", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermOwnerX == PermNbrFiles)
            {
                FRMPermsChkOwnerE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOwner, "Execute", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkOwnerE, 1);
            }
            else
            {
                FRMPermsChkOwnerE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOwner, "Execute", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkOwnerE, BST_INDETERMINATE);
            }
            if(PermGroupR == 0)
            {
                FRMPermsChkGroupR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameGroup, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
            }
            else if(PermGroupR == PermNbrFiles)
            {
                FRMPermsChkGroupR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameGroup, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
                CheckBoxSetState(FRMPermsChkGroupR, 1);
            }
            else
            {
                FRMPermsChkGroupR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameGroup, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 1);
                CheckBoxSetState(FRMPermsChkGroupR, BST_INDETERMINATE);
            }
            if(PermGroupW == 0)
            {
                FRMPermsChkGroupW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameGroup, "Write", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermGroupW == PermNbrFiles)
            {
                FRMPermsChkGroupW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameGroup, "Write", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkGroupW, 1);
            }
            else
            {
                FRMPermsChkGroupW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameGroup, "Write", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkGroupW, BST_INDETERMINATE);
            }
            if(PermGroupX == 0)
            {
                FRMPermsChkGroupE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameGroup, "Execute", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermGroupX == PermNbrFiles)
            {
                FRMPermsChkGroupE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameGroup, "Execute", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkGroupE, 1);
            }
            else
            {
                FRMPermsChkGroupE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameGroup, "Execute", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkGroupE, BST_INDETERMINATE);
            }
            if(PermOtherR == 0)
            {
                FRMPermsChkOtherR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOther, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
            }
            else if(PermOtherR == PermNbrFiles)
            {
                FRMPermsChkOtherR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOther, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 0);
                CheckBoxSetState(FRMPermsChkOtherR, 1);
            }
            else
            {
                FRMPermsChkOtherR = WACreateCheckBox(14, 16, 80, 15, FRMPermsFrameOther, "Read", 1, 0, WS_TABSTOP | WS_GROUP, 1);
                CheckBoxSetState(FRMPermsChkOtherR, BST_INDETERMINATE);
            }
            if(PermOtherW == 0)
            {
                FRMPermsChkOtherW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOther, "Write", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermOtherW == PermNbrFiles)
            {
                FRMPermsChkOtherW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOther, "Write", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkOtherW, 1);
            }
            else
            {
                FRMPermsChkOtherW = WACreateCheckBox(14, 16 + 17, 80, 15, FRMPermsFrameOther, "Write", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkOtherW, BST_INDETERMINATE);
            }
            if(PermOtherX == 0)
            {
                FRMPermsChkOtherE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOther, "Execute", 1, 0, WS_TABSTOP, 0);
            }
            else if(PermOtherX == PermNbrFiles)
            {
                FRMPermsChkOtherE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOther, "Execute", 1, 0, WS_TABSTOP, 0);
                CheckBoxSetState(FRMPermsChkOtherE, 1);
            }
            else
            {
                FRMPermsChkOtherE = WACreateCheckBox(14, 16 + (17 * 2), 80, 15, FRMPermsFrameOther, "Execute", 1, 0, WS_TABSTOP, 1);
                CheckBoxSetState(FRMPermsChkOtherE, BST_INDETERMINATE);
            }
            FRMPermsOk = WACreateButton(116 + 29, 84, 77, 23, hwndDlg, "Ok", 3, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMPermsCancel = WACreateButton(195 + 29, 84, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FreezeTimer = 1;
            return(1);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePerms);
            WAGDIDrawHorzSep(hwndDlg, 0, 77, 304);
            EndPaint(hwndDlg, &EnterValuePerms);
			break;
		case WM_COMMAND:
            if((HWND) lParam == FRMPermsOk)
            {
                switch(CheckBoxGetState(FRMPermsChkOwnerR))
                {
                    case BST_UNCHECKED:
                        PermOwnerR = 0;
						break;
                    case BST_CHECKED:
                        PermOwnerR = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkOwnerW))
                {
                    case BST_UNCHECKED:
                        PermOwnerW = 0;
						break;
                    case BST_CHECKED:
                        PermOwnerW = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkOwnerE))
                {
                    case BST_UNCHECKED:
                        PermOwnerX = 0;
						break;
                    case BST_CHECKED:
                        PermOwnerX = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkGroupR))
                {
                    case BST_UNCHECKED:
                        PermGroupR = 0;
						break;
                    case BST_CHECKED:
                        PermGroupR = PermNbrFiles;
						break;
				}
                switch(CheckBoxGetState(FRMPermsChkGroupW))
                {
                    case BST_UNCHECKED:
                        PermGroupW = 0;
						break;
                    case BST_CHECKED:
                        PermGroupW = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkGroupE))
                {
                    case BST_UNCHECKED:
                        PermGroupX = 0;
						break;
                    case BST_CHECKED:
                        PermGroupX = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkOtherR))
                {
                    case BST_UNCHECKED:
                        PermOtherR = 0;
						break;
                    case BST_CHECKED:
                        PermOtherR = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkOtherW))
                {
                    case BST_UNCHECKED:
                        PermOtherW = 0;
						break;
                    case BST_CHECKED:
                        PermOtherW = PermNbrFiles;
						break;
                }
                switch(CheckBoxGetState(FRMPermsChkOtherE))
                {
                    case BST_UNCHECKED:
                        PermOtherX = 0;
						break;
                    case BST_CHECKED:
                        PermOtherX = PermNbrFiles;
						break;
                }
                WAControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPermsCancel)
            {
                WAControlClose(hwndDlg);
                return(0);
			}
			break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}
