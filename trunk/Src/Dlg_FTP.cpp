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
// Dlg_FTP.cpp: FTP manager form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WALib_WinSock.h"
#include "ConstMisc.h"
#include "MiscFunctions.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "Crypt.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_FTP.h"
#include "Dlg_EnterValue.h"
#include "Dlg_Perms.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMFTPhWnd;
CStr FRMFTPTitle;
HWND FRMFTPCbAccounts;
HWND FRMFTPListview;
HWND FRMFTPCmdClose;
HWND FRMFTPToolbar;
HWND FRMFTPStatLabel;

CStr FTPIP;
long FTPAnonymous;
long FTPGotPass;
CStr FTPUserName;
CStr FTPUserPass;
CStr FTPPath;
long FTPPort;
long FTPTimeOut;
long FTPTransType;
long FTPPassive;
    
SOCKET FRMFTPhSocketDir;
SOCKET FRMFTPhSocketDown;
SOCKET FRMFTPhSocketUp;
SOCKET FRMFTPhSocketDirIncoming;
SOCKET FRMFTPhSocketDownloading;
SOCKET FRMFTPhSocketUploading;
SOCKET FTMFTPhSocketMain;
sockaddr_in FRMFTPSocketInfos;
sockaddr_in FTMFTPSocketInfosAccept;
long FRMFTPDirEntries;
long FTMFTPConnectionState;
WSADATA FRMFTPMyWinsock;
CStr FTPCurrentIP;
long FTPCurrentPort;
long FTPNoAnonAllowed;
CStr FTPCurrentPath;

CStr NewDatToSend;
CStr FTPDirectories;
long *FTPDirectoriesArray;
long *FilesArray;
CStr OldRename;
CStr NewRename;
long CurrentSelected;
long CurrentOperation;
HANDLE CurrenthFile;
long OkUser;
long OkConnected;
long TriggerDir;
CStr UpLdFile;

CList <char *> PermFileName;
CList <char *> PermFilePerms;

long PermOwnerR;
long PermOwnerW;
long PermOwnerX;
long PermGroupR;
long PermGroupW;
long PermGroupX;
long PermOtherR;
long PermOtherW;
long PermOtherX;
long PermNbrFiles;
CStr CurrentDownloaded;
CStr CurrentUploaded;
CStr CurrentSize;
long CurrentSizeLng;
long CurrentResume;
long CurrentUp;
CStr CurrentUpReal;
long CurrentUphFile;
long CurrentUpLen;
long CurrentUpActual;
long CurrentUpFileNbr;
long CurrentUpBreak;
long WinSErrFlag;
HMENU hFTPMenu;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFTPProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long CurrentLvItem = 0;
    long CurrentPopupItem = 0;
    POINT PopMenuCoords;
    PAINTSTRUCT Ps;
	CStr BufString;

    switch(uMsg)
    {
        // Main socket
        case WM_SYSCOLORCHANGE:
            WAListViewSetBackColor(FRMFTPListview, GetSysColor(COLOR_WINDOW));
			break;
		case WM_MAINSOCKET:
            MainSocketEvents(lParam, hwndDlg);
			break;
        // Directories socket
        case WM_DIRSOCKET:
            DirSocketEvents(lParam, hwndDlg);
			break;
        case WM_DOWNLOADSOCKET:
            DownloadSocketEvents(lParam, hwndDlg);
			break;
        case WM_UPLOADSOCKET:
            UploadSocketEvents(lParam, hwndDlg);
			break;
        case WM_INITDIALOG:
            FRMFTPhWnd = hwndDlg;
            WAControlSetText(hwndDlg, "FTP manager");
            FRMFTPCbAccounts = WACreateComboBox(2, 2, 152, 150, hwndDlg, "", 0, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            FillFTPCombo(FRMFTPCbAccounts);
            FRMFTPToolbar = WACreateToolBar(155, 1, 449, 23, hwndDlg, GlobalImageList1, 1, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPCONNECT, ICON_EARTH, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMFTPToolbar, 0);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPREFRESH, ICON_REFRESH, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDIRUP, ICON_DIRUP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDIRVIEW, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddSeparator(FRMFTPToolbar, 0);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPNEWDIR, ICON_NEWDIR, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDIRRENAME, ICON_RENAMEDIR, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDEL, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDIRPERMS, ICON_DIRPERMS, TBSTYLE_BUTTON, 0, 1);
            WAToolBarAddSeparator(FRMFTPToolbar, 0);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPDOWNLOAD, ICON_DOWNLOAD, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            WAToolBarAddButton(FRMFTPToolbar, "", FTPUPLOAD, ICON_UPLOAD, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMFTPListview = WACreateListView(2, 26, 556, 297, hwndDlg, 3, 0, &FRMFTPListviewHook, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | WS_TABSTOP, WS_EX_STATICEDGE);
            WAListViewAddCol(FRMFTPListview, "Attributes", 76, 3);
            WAListViewAddCol(FRMFTPListview, "Date", 123, 2);
            WAListViewAddCol(FRMFTPListview, "Size", 105, 1);
            WAListViewAddCol(FRMFTPListview, "Name", 230, 0);
            FRMFTPCmdClose = WACreateButton(480, 336, 77, 23, hwndDlg, "Close", 2, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP, Buttons_StaticEdge);
            FRMFTPStatLabel = WACreateLabel(2, 340, 470, 16, hwndDlg, "", 4, 0, 0, 0);
            if(WAComboBoxCount(FRMFTPCbAccounts) == 1)
            {
                WAComboBoxSetIndex(FRMFTPCbAccounts, 0);
                FillAccount(0);
            }
            CurrentOperation = FTPOP_NONE;
            WinsockInit(&FRMFTPMyWinsock, 0x101);
            WriteFTPStatus("Not connected.");
            hFTPMenu = CreatePopupMenu();
            // Create the contextual menu
            AppendMenu(hFTPMenu, MF_STRING, FTP_IDBASE + FTP_DOWNLOAD, "Download selected entries");
            WAMenuSetDefaultItem(hFTPMenu, 0);
            AppendMenu(hFTPMenu, MF_SEPARATOR, -1, "-");
            AppendMenu(hFTPMenu, MF_STRING, FTP_IDBASE + FTP_DELETE, "Delete selected entries");
            FreezeTimer = 1;
            CurrentSelected = -1;
            return(1);
        case WM_COMMAND:
            if((HWND) lParam == FRMFTPToolbar)
            {
                switch(wParam)
                {
                    case FTPCONNECT:
                        if(WAControlGetText(FRMFTPCbAccounts).Len() == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Select an account.", MB_ERROR, Requesters);
                            SetFocus(FRMFTPCbAccounts);
                            return(0);
                        }
                        if(FTMFTPConnectionState == 1)
                        {
                            BufString = "FTP manager is currently connected to: \r" + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".\rBreak connection ?";
                            switch(WAMiscMsgBox(FRMFTPhWnd, BufString, MB_QUESTION, Requesters))
                            {
                                case IDYES:
                                    BreakConnection(hwndDlg);
									break;
								case IDNO:
                                    return(0);
                            }
                        }
                        OkUser = 0;
                        OkConnected = 0;
                        TriggerDir = 0;
                        ConnectToFTP(hwndDlg);
                        return(0);
                    case FTPREFRESH:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        CurrentOperation = FTPOP_NONE;
                        FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, hwndDlg, WM_DIRSOCKET);
                        return(0);
                    case FTPDIRUP:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "CDUP");
                        return(0);
                    case FTPDIRVIEW:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        if(strcmpi(WAListViewGetSelItemText(FRMFTPListview, 1).Get_String(), "(DIR)") == 0)
                        {
                            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "CWD " + (CStr) WAListViewGetSelItemText(FRMFTPListview, 0).Get_String());
                        }
                        else
                        {
                            WAMiscMsgBox(hwndDlg, "No directory selected.", MB_ERROR, Requesters);
                        }
                        return(0);
                    case FTPNEWDIR:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        NewDatToSend = MiscInputBox(hwndDlg, "Enter new remote directory name", ICON_ASKDIR, "", 0, INPUTBOX_SIMPLETEXT, "");
                        if(NewDatToSend.Len() != 0) SocketSendASCIIDatasCR(FTMFTPhSocketMain, "MKD " + (CStr) NewDatToSend);
                        return(0);
                    case FTPDIRRENAME:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        if(WAListViewGetSelItemText(FRMFTPListview, 0).Len() == 0)
                        {
                            WAMiscMsgBox(hwndDlg, "No directory/file selected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        OldRename = WAListViewGetSelItemText(FRMFTPListview, 0);
                        if(strcmpi(WAListViewGetSelItemText(FRMFTPListview, 1).Get_String(), "(DIR)") == 0)
                        {
                            NewRename = MiscInputBox(hwndDlg, "Enter new directory name", ICON_RENAMEDIR, OldRename, 0, INPUTBOX_SIMPLETEXT, "");
                        }
                        else
                        {
                            NewRename = MiscInputBox(hwndDlg, "Enter new file name", ICON_RENAMEDIR, OldRename, 0, INPUTBOX_SIMPLETEXT, "");
                        }
                        if(NewRename.Len() != 0) SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RNFR " + (CStr) OldRename);
                        return(0);
                    case FTPDEL:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        CurrentSelected = WAListViewGetSelItem(FRMFTPListview, -1);
                        if(CurrentSelected == -1)
                        {
                            WAMiscMsgBox(hwndDlg, "No directories/files selected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        switch(WAMiscMsgBox(hwndDlg, "Really want to delete all selected entries ?", MB_QUESTION, Requesters))
                        {
                            case IDYES:
                                CurrentOperation = FTPOP_DELETE;
                                WriteFTPStatus("Deleting entries...");
                                if(strcmpi(WAListViewGetItemText(FRMFTPListview, CurrentSelected, 1).Get_String(), "(DIR)") == 0)
                                {
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RMD " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0));
                                }
                                else
                                {
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "DELE " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0));
                                }
								break;
						}
                        return(0);
                    case FTPDIRPERMS:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        CurrentSelected = WAListViewGetSelItem(FRMFTPListview, -1);
                        if(CurrentSelected == -1)
                        {
                            WAMiscMsgBox(hwndDlg, "No directories/files selected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        FillPermissions();
                        CurrentOperation = FTPOP_PERMS;
                        WACreateModalDialog(-1, -1, 304, 110, FRMFTPhWnd, &FRMProcPerms, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
                        return(0);
                    case FTPDOWNLOAD:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        CurrentSelected = WAListViewGetSelItem(FRMFTPListview, -1);
                        if(CurrentSelected == -1)
                        {
                            WAMiscMsgBox(hwndDlg, "No files selected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        CurrentOperation = FTPOP_DOWNLOAD;
                        switch(FTPTransType)
                        {
                            case 1:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
								break;
                            case 2:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
								break;
						}
                        return(0);
                    case FTPUPLOAD:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        if(SelectFilesToUpload(FRMFTPhWnd) == 1)
                        {
                            CurrentUp = 1;
                            CurrentUpBreak = 0;
                            CurrentOperation = FTPOP_UPLOAD;
                            switch(FTPTransType)
                            {
                                case 1:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
									break;
                                case 2:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
									break;
							}
                        }
						return(0);
                }
			}
			else if((HWND) lParam == FRMFTPCbAccounts)
			{
                    if((wParam & 0x7FFF0000) == 0x10000) FillAccount(WAComboBoxGetIndex(FRMFTPCbAccounts));
                    return(0);
			}
			else if((HWND) lParam == FRMFTPCmdClose)
			{
                    WAControlClose(hwndDlg);
                    return(0);
            }
            // Wrap these two messages (coming from menu)
            switch(wParam)
            {
                case (FTP_IDBASE + FTP_DOWNLOAD):
                    SendMessage(hwndDlg, WM_COMMAND, FTPDOWNLOAD, (long) FRMFTPToolbar);
                    return(0);
                case (FTP_IDBASE + FTP_DELETE):
                    SendMessage(hwndDlg, WM_COMMAND, FTPDEL, (long) FRMFTPToolbar);
                    return(0);
            }
			break;
        case WM_NOTIFY:
            if(WAControlGetNotifiedhWnd(lParam) == FRMFTPListview)
            {
                switch(WAControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        if(FTMFTPConnectionState == 0)
                        {
                            WAMiscMsgBox(FRMFTPhWnd, "Not connected.", MB_ERROR, Requesters);
                            return(0);
                        }
                        if(strcmpi(WAListViewGetSelItemText(FRMFTPListview, 1).Get_String(), "(DIR)") == 0)
                        {
                            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "CWD " + (CStr) WAListViewGetSelItemText(FRMFTPListview, 0));
                        }
                        else
                        {
                            CurrentSelected = WAListViewGetSelItem(FRMFTPListview, -1);
                            if(CurrentSelected == -1)
                            {
                                WAMiscMsgBox(hwndDlg, "No directories/files selected.", MB_ERROR, Requesters);
                                return(0);
                            }
                            CurrentOperation = FTPOP_DOWNLOAD;
                            switch(FTPTransType)
                            {
                                case 1:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
									break;
								case 2:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
                            }
                        }
						break;
                    case LVN_COLUMNCLICK:
                        WAListViewSort(FRMFTPListview, WAListViewGetNotifiedColumnIndex(lParam), &FRMFTPListviewSort);
                        return(0);
				}
            }
            switch(WAControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(WAControlGetNotifiedID(lParam))
                    {
                        case FTPCONNECT:
                            WAToolBarDisplayToolTip("Connect to FTP server", lParam);
                            return(0);
                        case FTPREFRESH:
                            WAToolBarDisplayToolTip("Refresh content", lParam);
                            return(0);
                        case FTPDIRUP:
                            WAToolBarDisplayToolTip("Back to parent directory", lParam);
                            return(0);
                        case FTPDIRVIEW:
                            WAToolBarDisplayToolTip("View selected directory", lParam);
                            return(0);
                        case FTPNEWDIR:
                            WAToolBarDisplayToolTip("Create new directory", lParam);
                            return(0);
                        case FTPDIRRENAME:
                            WAToolBarDisplayToolTip("Rename file/directory", lParam);
                            return(0);
                        case FTPDEL:
                            WAToolBarDisplayToolTip("Delete files/directories", lParam);
                            return(0);
                        case FTPDIRPERMS:
                            WAToolBarDisplayToolTip("Change files/directories atttributes", lParam);
                            return(0);
                        case FTPDOWNLOAD:
                            WAToolBarDisplayToolTip("Download selected files", lParam);
                            return(0);
                        case FTPUPLOAD:
                            WAToolBarDisplayToolTip("Upload files", lParam);
                            return(0);
                    }
					break;
			}
			break;
		case WM_CONTEXTMENU:
            CurrentLvItem = WAListViewGetItemUnderCursor(FRMFTPListview);
            if(CurrentLvItem != -1)
            {
                WAListViewSetItemSel(FRMFTPListview, CurrentLvItem);
                CurrentPopupItem = WAListViewGetItemUnderCursor(FRMFTPListview);
                SetFocus(FRMFTPListview);
				GetCursorPos(&PopMenuCoords);
                TrackPopupMenu(hFTPMenu, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopMenuCoords.x, PopMenuCoords.y, 0, hwndDlg, NULL);
            }
			return(0);
		case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            WAGDIDrawHorzSep(hwndDlg, 0, WAControlClientHeight(hwndDlg) - 33, WAControlClientWidth(hwndDlg));
            EndPaint(hwndDlg, &Ps);
			break;
		case WM_CLOSE:
            // Just in case
            if(FTMFTPConnectionState == 1)
            {
                BufString = "FTP manager is currently connected to: \r" + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".\rBreak connection ?";
                switch(WAMiscMsgBox(FRMFTPhWnd, BufString, MB_QUESTION, Requesters))
                {
                    case IDYES:
                        BreakConnection(hwndDlg);
						break;
					case IDNO:
                        return(1);
                }
            }
            if(CurrenthFile != INVALID_HANDLE_VALUE)
            {
                if(CurrenthFile != 0) WAFileClose(CurrenthFile);
            }
            FTMFTPConnectionState = 0;
            CurrenthFile = 0;
            WinsockUnInit();
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
			break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Get account informations
void FillAccount(long AccNumber)
{
    CStr AccName;
    CStr FFPort;
    CStr FFTimeOut;
    CStr FFAnon;
    CStr RetrieveUserP;

    if(AccNumber == -1) return;
    AccName = WAComboBoxGetItem(FRMFTPCbAccounts, AccNumber);
    FTPIP = WAIniReadKey(AccName, "FTPIP", FtpAccountsIniFile);
    FFAnon = WAIniReadKey(AccName, "FTPAnon", FtpAccountsIniFile);
    FTPAnonymous = 0;
    if(FFAnon.Len() != 0) FTPAnonymous = WAIniReadKey(AccName, "FTPAnon", FtpAccountsIniFile).Get_Long();
    FTPUserName = WAIniReadKey(AccName, "FTPUserName", FtpAccountsIniFile);
    FTPUserPass = "";
    FTPGotPass = 0;
    RetrieveUserP = WAIniReadKey(AccName, "FTPSavePass", FtpAccountsIniFile);
    if(RetrieveUserP.Len() != 0) FTPGotPass = RetrieveUserP.Get_Long();
    if(strcmpi(RetrieveUserP.Get_String(), "2") == 0)
    {
        // Load and decrypt it
        FTPUserPass = DecryptPassword(WAIniReadKey(AccName, "FTPUserPass", FtpAccountsIniFile));
    }
    else
    {
        // Load password as plain text
        FTPUserPass = WAIniReadKey(AccName, "FTPUserPass", FtpAccountsIniFile);
    }
    FTPPath = WAIniReadKey(AccName, "FTPPath", FtpAccountsIniFile);
    FFPort = WAIniReadKey(AccName, "FTPPort", FtpAccountsIniFile);
    FFTimeOut = WAIniReadKey(AccName, "FTPTimeout", FtpAccountsIniFile);
    FTPPort = 21;
    if(FFPort.Len() != 0) FTPPort = FFPort.Get_Long();
    FTPTimeOut = 500;
    if(FFTimeOut.Len() != 0) FTPTimeOut = FFTimeOut.Get_Long();
    FTPTransType = WAIniReadKey(AccName, "FTPTransfersType", FtpAccountsIniFile).Get_Long();
    FTPPassive = WAIniReadKey(AccName, "FTPPassive", FtpAccountsIniFile).Get_Long();
}

// -----------------------------------------------------------------------
// Write a text in status part
void WriteFTPStatus(CStr Txt)
{
    WAControlSetText(FRMFTPStatLabel, Txt);
}

// -----------------------------------------------------------------------
// FTP messages handler
void DispatchFTPMessages(CStr Message)
{
    int k = 0;
    long *MessageArray = 0;
    CStr SavedMessage;
	CStr BufString;

    MessageArray = StringSplit(Message, "\r\n");
    for(k = 0; k <= StringGetSplitUBound(MessageArray); k++)
    {
        SavedMessage = StringGetSplitElement(Message, MessageArray, k);
        BufString = SavedMessage.Left(3);
		if(BufString == "200")
		{
            switch(CurrentOperation)
            {
                case FTPOP_DOWNLOAD:
                    if(SavedMessage.In_Str(1, "PORT", Text_Compare) != 0)
                    {
SkipDirs:				CurrentSize = WAListViewGetItemText(FRMFTPListview, CurrentSelected, 1);
                        if(strcmpi(CurrentSize.Get_String(), "(DIR)") == 0)
                        {
SkipFile:					CurrentSelected = WAListViewGetSelItem(FRMFTPListview, CurrentSelected);
                            if(CurrentSelected == -1)
                            {
                                CurrentOperation = FTPOP_NONE;
                                goto ReleaseMessage;
                            }
                            goto SkipDirs;
                        }
                        CurrentDownloaded = WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0);
                        CurrentSize = CurrentSize.Trim();
                        CurrentSizeLng = CurrentSize.Get_Long();
                        // Check if file exists to ask a resume operation
                        if(WAFileExist(Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded) != 0)
                        {
                            // File exist and size is lower: ask for resume
                            CurrentResume = WAFileGetSize(Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded);
                            if(CurrentResume < CurrentSizeLng)
                            {
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "REST " + (CStr) CurrentResume);
                                goto ReleaseMessage;
                            }
                            else
                            {
                                if(WAMiscMsgBox(FRMFTPhWnd, "File '" + (CStr) Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded + (CStr) "' already exists.\rOverwrite ?", MB_QUESTION, Requesters) == IDNO) goto SkipFile;
                            }
                        }
                        CurrentResume = 0;
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RETR " + (CStr) CurrentDownloaded);
                    }
                    else
                    {
                        FRMFTPhSocketDown = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DOWNLOADSOCKET);
                    }
					break;
                case FTPOP_UPLOAD:
                    if(CurrentUpBreak == 0)
                    {
                        if(SavedMessage.In_Str(1, "PORT", Text_Compare) != 0)
                        {
                            if(CurrentUp > StringGetSplitUBound(FilesArray))
                            {
                                // No more file to upload: free array and refresh directory
                                StringReleaseSplit(FilesArray);
                                CurrentOperation = FTPOP_NONE;
                                FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                                WACursorSetNormal();
                            }
                            else
                            {
                                if(StringGetSplitElement(UpLdFile, FilesArray, CurrentUp).Len() != 0)
                                {
                                    // Initiate uploading
                                    CurrentUpReal = StringGetSplitElement(UpLdFile, FilesArray, CurrentUp);
                                    CurrentUploaded = WAFileGetFileName(StringGetSplitElement(UpLdFile, FilesArray, CurrentUp));
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "STOR " + (CStr) CurrentUploaded);
                                }
                            }
                        }
                        else
                        {
                            FRMFTPhSocketUp = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_UPLOADSOCKET);
                        }
                    }
                    else
                    {
                        WACursorSetNormal();
                    }
					break;
				default:
                    // First dir
                    if(TriggerDir == 1) FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                    TriggerDir = 0;
                    if(SavedMessage.In_Str(1, "PORT", Text_Compare) != 0) SocketSendASCIIDatasCR(FTMFTPhSocketMain, "LIST");
            }
		}
		else if(BufString == "350")
		{
            // Resume accepted: let's go
            switch(CurrentOperation)
            {
                case FTPOP_DOWNLOAD:
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RETR " + (CStr) CurrentDownloaded);
					break;
			}
		}
		else if(BufString == "226")
		{
            switch(CurrentOperation)
            {
                case FTPOP_DOWNLOAD:
                    // Transfer complete
                    CurrentSelected = WAListViewGetSelItem(FRMFTPListview, CurrentSelected);
                    if(CurrentSelected == -1)
                    {
                        // No more selected entries
                        WACursorSetNormal();
                        CurrentOperation = FTPOP_NONE;
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
                    }
                    else
                    {
                        // Download next file/directory
                        if(strcmpi(WAListViewGetItemText(FRMFTPListview, CurrentSelected, 1).Get_String(), "(DIR)") != 0)
                        {
                            FRMFTPhSocketDown = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DOWNLOADSOCKET);
                        }
                    }
					break;
                case FTPOP_UPLOAD:
                    if(CurrentUpBreak == 0)
                    {
                        // Increment file number
                        CurrentUp++;
                        // And proceed with next file
                        switch(FTPTransType)
                        {
                            case 1:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
								break;
							case 2:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
								break;
                        }
                    }
            }
		}
		else if(BufString == "220")
		{
            if(OkUser == 0)
            {
                // Ask for user name
                if(FTPAnonymous != 0)
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "USER anonymous");
                }
                else
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "USER " + (CStr) FTPUserName);
                }
            }
            OkUser = 1;
		}
		else if(BufString == "230")
		{
            if(OkConnected == 0)
            {
                // Connected
                if(FTPCurrentPath.Len() != 0) SocketSendASCIIDatasCR(FTMFTPhSocketMain, "CWD " + (CStr) FTPCurrentPath);
                if(FTPPassive != 0) SocketSendASCIIDatasCR(FTMFTPhSocketMain, "PASV");
                if(FTPTransType != 0)
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
                    TriggerDir = 1;
                }
                else
                {
                    TriggerDir = 0;
                    FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                }
            }
            OkConnected = 1;
		}
		else if(BufString == "250")
		{
            if(SavedMessage.In_Str(1, "DELE ") != 0)
            {
                // Set next file to delete
                CurrentSelected = WAListViewGetSelItem(FRMFTPListview, CurrentSelected);
                if(CurrentSelected == -1)
                {
                    // No more selected entries: refresh dir
                    FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                    CurrentOperation = FTPOP_NONE;
                }
                else
                {
                    // Delete next file/directory
                    if(strcmpi(WAListViewGetItemText(FRMFTPListview, CurrentSelected, 1).Get_String(), "(DIR)") == 0)
                    {
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RMD " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0).Get_String());
                    }
                    else
                    {
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "DELE " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0).Get_String());
                    }
                }
            }
            else
            {
                FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
            }
		}
		else if(BufString == "331")
		{
            // Ask for password
            if(FTPNoAnonAllowed == 0)
            {
                if(FTPAnonymous == 0)
                {
                    // No password AND password not saved ?
                    if((FTPUserPass == "") & (FTPGotPass == 0))
                    {
                        NewDatToSend = MiscInputBox(FRMFTPhWnd, "Enter new password", ICON_FTPUSER, "", 0, INPUTBOX_PASSWORD | INPUTBOX_ALLOW_EMPTY, "");
                        //if(NewDatToSend.Len() == 0) {
                            //BreakConnection(FRMFTPhWnd);
                        //} else {
                            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "PASS " + (CStr) NewDatToSend);
                        //}
                    }
                    else
                    {
                        SocketSendASCIIDatasCR(FTMFTPhSocketMain, "PASS " + (CStr) FTPUserPass);
                    }
                }
                else
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "PASS anonymous@" + (CStr) FRMFTPSocketInfos.sin_addr.S_un.S_un_b.s_b1 + (CStr) "." + (CStr) FRMFTPSocketInfos.sin_addr.S_un.S_un_b.s_b2 + (CStr) "." + (CStr) FRMFTPSocketInfos.sin_addr.S_un.S_un_b.s_b3 + (CStr) "." + (CStr) FRMFTPSocketInfos.sin_addr.S_un.S_un_b.s_b4);
                }
            }
            else
            {
                NewDatToSend = MiscInputBox(FRMFTPhWnd, "Enter new password", ICON_FTPUSER, "", 0, INPUTBOX_PASSWORD | INPUTBOX_ALLOW_EMPTY, "");
                if(NewDatToSend.Len() == 0)
                {
                    BreakConnection(FRMFTPhWnd);
                }
                else
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "PASS " + (CStr) NewDatToSend);
                }
            }
		}
		else if(BufString == "350")
		{
            // Effective rename
            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RNTO " + (CStr) NewRename);
		}
		else if(BufString == "421")
		{
            WAMiscMsgBox(FRMFTPhWnd, SavedMessage.Mid(5), MB_ERROR, Requesters);
		}
		else if(BufString == "426")
		{
            WAMiscMsgBox(FRMFTPhWnd, SavedMessage.Mid(5), MB_ERROR, Requesters);
		}
		else if(BufString == "257")
		{
            FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
		}
		else if(BufString == "530")
		{
            if(SavedMessage.In_Str(1, "anonymous", Text_Compare) != 0)
            {
                NewDatToSend = MiscInputBox(FRMFTPhWnd, "Enter new user name", ICON_FTPUSER, "", 0, INPUTBOX_SIMPLETEXT, "");
                if(NewDatToSend.Len() == 0)
                {
                    BreakConnection(FRMFTPhWnd);
                }
                else
                {
                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "USER " + (CStr) NewDatToSend);
                    FTPNoAnonAllowed = 1;
                }
            }
            else
            {
                if(SavedMessage.In_Str(1, "logged", Text_Compare) != 0)
                {
                    if(SavedMessage.In_Str(1, "too many", Text_Compare) == 0)
                    {
                        NewDatToSend = MiscInputBox(FRMFTPhWnd, "Enter new password", ICON_FTPUSER, "", 0, INPUTBOX_PASSWORD, "");
                        if(NewDatToSend.Len() == 0)
                        {
                            BreakConnection(FRMFTPhWnd);
                        }
                        else
                        {
                            FTPUserPass = NewDatToSend;
                            // Ask for user name
                            if(FTPAnonymous != 0)
                            {
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "USER anonymous");
                            }
                            else
                            {
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "USER " + (CStr) FTPUserName);
                            }
                        }
                    }
                }
                else
                {
                    WAMiscMsgBox(FRMFTPhWnd, SavedMessage, MB_ERROR, Requesters);
                }
            }
		}
		else if(BufString == "550")
		{
            if(CurrentSelected != -1)
            {
                BufString = "Server returned message: \r" + (CStr) CMGetRealFile(SavedMessage) + (CStr) "\rWant to continue operation ?";
                switch(WAMiscMsgBox(FRMFTPhWnd, BufString, MB_ERRORQUESTION, Requesters))
                {
                    case IDYES:
                        CurrentSelected = WAListViewGetSelItem(FRMFTPListview, CurrentSelected);
                        if(CurrentSelected == -1)
                        {
                            // No more selected entries
                            FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                            CurrentOperation = FTPOP_NONE;
                        }
                        else
                        {
                            // Delete next file/directory
                            if(strcmpi(WAListViewGetItemText(FRMFTPListview, CurrentSelected, 1).Get_String(), "(DIR)") == 0)
                            {
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "RMD " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0));
                            }
                            else
                            {
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, "DELE " + (CStr) WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0));
                            }
                        }
						break;
                    case IDNO:
                        FRMFTPhSocketDir = WAFTPInitiatePort(FTMFTPhSocketMain, &FRMFTPSocketInfos, FRMFTPhWnd, WM_DIRSOCKET);
                        CurrentOperation = FTPOP_NONE;
						break;
				}
            }
            else
            {
                WAMiscMsgBox(FRMFTPhWnd, SavedMessage, MB_ERROR, Requesters);
            }
        }
    }
ReleaseMessage:
    StringReleaseSplit(MessageArray);
}

// -----------------------------------------------------------------------
// Connect to FTP server
void ConnectToFTP(HWND hWnd)
{
    long WSError = 0;
	CStr BufString;

    FTMFTPhSocketMain = SocketCreate();
    if(FTMFTPhSocketMain == -1)
    {
        WriteFTPStatus("Not connected.");
        WAMiscMsgBox(FRMFTPhWnd, "Can't create socket.", MB_ERROR, Requesters);
        return;
    }
    FTPCurrentIP = FTPIP;
    FTPCurrentPort = FTPPort;
    FTPCurrentPath = FTPPath;
    WriteFTPStatus("Connecting to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) "...");
    WinSErrFlag = 0;
    WSError = SocketConnect(FTMFTPhSocketMain, &FRMFTPSocketInfos, WM_MAINSOCKET, hWnd, FTPCurrentIP, FTPCurrentPort, FTPTimeOut);
    if(WSError != 0)
    {
        WinSErrFlag = 1;
        SocketDestroy(FTMFTPhSocketMain, hWnd);
        WriteFTPStatus("Not connected.");
        BufString = "Can't connect to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".\rWinsock returned error: " + (CStr) WSError;
        WAMiscMsgBox(FRMFTPhWnd, BufString, MB_ERROR, Requesters);
        return;
    }
    FTPNoAnonAllowed = 0;
    FTMFTPConnectionState = 0;
    CurrentSelected = -1;
}

// -----------------------------------------------------------------------
// Handle the main socket events
void MainSocketEvents(long Message, HWND hWnd)
{
    CStr Retrieved;

    switch(Message)
    {
        case FD_READ:
            Retrieved = SocketReceiveASCIIDatas(FTMFTPhSocketMain);
            // Check if there's other datas to retrieve
            DispatchFTPMessages(Retrieved);
			break;
        case FD_WRITE:
			break;
        case FD_CONNECT:
            if(WinSErrFlag == 0)
            {
                FTMFTPConnectionState = 1;
                WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                WAControlEnable(FRMFTPCbAccounts, 0);
            }
			break;
        case FD_CLOSE:
            WriteFTPStatus("Not connected.");
            SocketDestroy(FTMFTPhSocketMain, hWnd);
            WAControlEnable(FRMFTPCbAccounts, 1);
            FTMFTPConnectionState = 0;
			break;
		case FD_ACCEPT:
			break;
    }
}

// -----------------------------------------------------------------------
// Handle upload socket events
void UploadSocketEvents(long Message, HWND hWnd)
{
    CStr RetrievedDirEntry;
    long CurrentSend = 0;

    switch(Message)
    {
        case FD_WRITE:
            if(CurrentUpActual < CurrentUpLen)
            {
				CurrentSend = send(FRMFTPhSocketUploading, (char *) CurrentUphFile + CurrentUpActual, CurrentUpLen - CurrentUpActual, 0);
                if(CurrentSend == -1)
                {
                    if(WAMiscMsgBox(FRMFTPhWnd, (CStr) "Winsock error: " + (CStr) WSAGetLastError() + (CStr) "\rContinue operation ?", MB_ERRORQUESTION, Requesters) == IDYES)
                    {
                        FreeMem(CurrentUphFile);
                        CurrentUphFile = 0;
                        SocketDestroy(FRMFTPhSocketUploading, hWnd);
                        SocketDestroy(FRMFTPhSocketUp, hWnd);
                        WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                        SetFocus(FRMFTPListview);
                        CurrentUp++;
                        // And proceed with next file
                        switch(FTPTransType)
                        {
                            case 1:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, (CStr) "TYPE A");
								break;
							case 2:
                                SocketSendASCIIDatasCR(FTMFTPhSocketMain, (CStr) "TYPE I");
								break;
						}
                    }
                    else
                    {
                        CurrentUpBreak = 1;
                        FreeMem(CurrentUphFile);
                        CurrentUphFile = 0;
                        SocketDestroy(FRMFTPhSocketUploading, hWnd);
                        SocketDestroy(FRMFTPhSocketUp, hWnd);
                        WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                        SetFocus(FRMFTPListview);
                    }
                }
                else
                {
                    if(CurrentSend > 0)
                    {
                        CurrentUpActual = CurrentSend;
                        WriteFTPStatus((CStr) "Uploading file '" + (CStr) CurrentUploaded + (CStr) "' (" + (CStr) CurrentUpActual + (CStr) "/" + (CStr) CurrentUpLen + (CStr) " bytes)...");
                    }
                }
            }
            else
            {
                FreeMem(CurrentUphFile);
                SocketDestroy(FRMFTPhSocketUploading, hWnd);
                SocketDestroy(FRMFTPhSocketUp, hWnd);
                WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                SetFocus(FRMFTPListview);
            }
			break;
        case FD_ACCEPT:
            FRMFTPhSocketUploading = SocketAcceptConnection(FRMFTPhSocketUp, &FTMFTPSocketInfosAccept);
            CurrentUpActual = 0;
            CurrentUphFile = (long) MLoadFile(CurrentUpReal.Get_String(), &CurrentUpLen);
            if(CurrentUphFile == 0)
            {
                if(WAMiscMsgBox(FRMFTPhWnd, (CStr) "Error while loading file '" + (CStr) CurrentUpReal + (CStr) "'\rContinue operation ?", MB_ERRORQUESTION, Requesters) == IDYES)
                {
                    FreeMem(CurrentUphFile);
                    CurrentUphFile = 0;
                    SocketDestroy(FRMFTPhSocketUploading, hWnd);
                    SocketDestroy(FRMFTPhSocketUp, hWnd);
                    WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                    SetFocus(FRMFTPListview);
                    CurrentUp++;
                    // And proceed with next file
                    switch(FTPTransType)
                    {
                        case 1:
                            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
							break;
						case 2:
                            SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
							break;
					}
                }
                else
                {
                    CurrentUpBreak = 1;
                    SocketDestroy(FRMFTPhSocketUploading, hWnd);
                    SocketDestroy(FRMFTPhSocketUp, hWnd);
                    WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                    SetFocus(FRMFTPListview);
                }
            }
            else
            {
                if(CurrentUphFile != 0)
                {
                    CurrentUpActual = send(FRMFTPhSocketUploading, (char *) CurrentUphFile, CurrentUpLen, 0);
                    if(CurrentUpActual == -1)
                    {
                        if(WAMiscMsgBox(FRMFTPhWnd, (CStr) "Winsock error: " + (CStr) WSAGetLastError() + (CStr) "\rContinue operation ?", MB_ERRORQUESTION, Requesters) == IDYES)
                        {
                            CurrentUp++;
                            // And proceed with next file
                            switch(FTPTransType)
                            {
                                case 1:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE A");
									break;
								case 2:
                                    SocketSendASCIIDatasCR(FTMFTPhSocketMain, "TYPE I");
									break;
							}
                        }
                        else
                        {
                            CurrentUpBreak = 1;
                            FreeMem(CurrentUphFile);
                            CurrentUphFile = 0;
                            SocketDestroy(FRMFTPhSocketUploading, hWnd);
                            SocketDestroy(FRMFTPhSocketUp, hWnd);
                            WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
                            SetFocus(FRMFTPListview);
                        }
                    }
                    else
                    {
                        WriteFTPStatus((CStr) "Uploading file '" + (CStr) CurrentUploaded + (CStr) "' (0/" + (CStr) CurrentUpLen + (CStr) " bytes)...");
                        WACursorSetWait();
                    }
                }
            }
			break;
        case FD_CLOSE:
            FreeMem(CurrentUphFile);
            CurrentUphFile = 0;
            SocketDestroy(FRMFTPhSocketUploading, hWnd);
            SocketDestroy(FRMFTPhSocketUp, hWnd);
            WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
            SetFocus(FRMFTPListview);
			break;
    }
}

// -----------------------------------------------------------------------
// Handle downloads socket events
void DownloadSocketEvents(long Message, HWND hWnd)
{
    long *RetrievedDirEntry = 0;
    long RetrievedDirEntryLen = 0;

    switch(Message)
    {
        case FD_READ:
            RetrievedDirEntryLen = SocketGetRevcLen(FRMFTPhSocketDownloading);
            RetrievedDirEntry = (long *) AllocMem(RetrievedDirEntryLen);
            RetrievedDirEntryLen = SocketReceiveUnknownDatas(FRMFTPhSocketDownloading, RetrievedDirEntry, RetrievedDirEntryLen);
            if(RetrievedDirEntryLen != 0)
            {
                if(CurrenthFile != INVALID_HANDLE_VALUE)
                {
                    WAFileWriteBuffer(CurrenthFile, RetrievedDirEntry, RetrievedDirEntryLen);
                }
                CurrentResume = CurrentResume + RetrievedDirEntryLen;
            }
            WriteFTPStatus((CStr) "Retrieving file '" + (CStr) CurrentDownloaded + (CStr) "' (" + (CStr) CurrentResume + (CStr) "/" + (CStr) CurrentSizeLng + (CStr) " bytes)...");
            FreeMem((long) RetrievedDirEntry);
			break;
        case FD_ACCEPT:
            FRMFTPhSocketDownloading = SocketAcceptConnection(FRMFTPhSocketDown, &FTMFTPSocketInfosAccept);
            if(CurrentResume != 0)
            {
                WriteFTPStatus((CStr) "Resuming file '" + (CStr) CurrentDownloaded + (CStr) "' at " + (CStr) CurrentResume + (CStr) " bytes...");
                if(CurrentResume != 0)
                {
                    CurrenthFile = WAFileOpenW(Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded, CurrentResume);
                }
                else
                {
                    CurrenthFile = WAFileCreateEmpty((CStr) Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded, NO_SECURITY_DESCRIPTOR);
                }
            }
            else
            {
                WriteFTPStatus((CStr) "Retrieving file '" + (CStr) CurrentDownloaded + (CStr) "' (0/" + (CStr) CurrentSizeLng + (CStr) " bytes)...");
                CurrenthFile = WAFileCreateEmpty(Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) CurrentDownloaded, NO_SECURITY_DESCRIPTOR);
            }
            WACursorSetWait();
			break;
		case FD_CLOSE:
            if(CurrenthFile != INVALID_HANDLE_VALUE)
            {
                if(CurrenthFile != 0)
                {
                    WAFileClose(CurrenthFile);
                }
            }
            CurrenthFile = 0;
            SocketDestroy(FRMFTPhSocketDownloading, hWnd);
            SocketDestroy(FRMFTPhSocketDown, hWnd);
            WriteFTPStatus("Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
            SetFocus(FRMFTPListview);
    }
}

// -----------------------------------------------------------------------
// Handle directories socket events
void DirSocketEvents(long Message, HWND hWnd)
{
    CStr RetrievedDirEntry;

    switch(Message)
    {
        case FD_READ:
            RetrievedDirEntry = SocketReceiveASCIIDatas(FRMFTPhSocketDirIncoming);
            FTPDirectories = FTPDirectories + (CStr) RetrievedDirEntry;
			break;
		case FD_ACCEPT:
            FRMFTPhSocketDirIncoming = SocketAcceptConnection(FRMFTPhSocketDir, &FTMFTPSocketInfosAccept);
            WriteFTPStatus("Retrieving directory...");
            WACursorSetWait();
            FTPDirectories = "";
            FRMFTPDirEntries = 0;
			break;
        case FD_CLOSE:
            FilCListView();
            WACursorSetNormal();
            SocketDestroy(FRMFTPhSocketDirIncoming, hWnd);
            SocketDestroy(FRMFTPhSocketDir, hWnd);
            WriteFTPStatus((CStr) "Connected to " + (CStr) FTPCurrentIP + (CStr) " on port " + (CStr) FTPCurrentPort + (CStr) ".");
            SetFocus(FRMFTPListview);
			break;
    }
}

// -----------------------------------------------------------------------
// Break the current connection
void BreakConnection(HWND hWnd)
{
    if(FTMFTPhSocketMain != -1)
    {
        SocketSendASCIIDatasCR(FTMFTPhSocketMain, (CStr) "QUIT");
        SocketDestroy(FTMFTPhSocketMain, hWnd);
    }
    WAControlEnable(FRMFTPCbAccounts, 1);
    FTMFTPConnectionState = 0;
    FTPNoAnonAllowed = 0;
	WriteFTPStatus("Not connected.");
}

// -----------------------------------------------------------------------
// Fill the received directories entries
void FilCListView(void)
{
    int i = 0;
    CStr CurrentDirEntry;
    CStr PointEntry;
    long FoundPoint = 0;
    long FoundDoublePoint = 0;
    CStr PointDate;
    CStr PointAttr;
    CStr DoublePointDate;
    CStr DoublePointAttr;
    CStr EntrDt;

    FTPDirectoriesArray = StringSplit(FTPDirectories, "\r\n");
    WAListViewClear(FRMFTPListview);
    for(i = 0; i <= (long) StringGetSplitUBound(FTPDirectoriesArray) - 1; i++)
    {
        CurrentDirEntry = StringGetSplitElement(FTPDirectories, FTPDirectoriesArray, i).Trim();
        if(CurrentDirEntry.Len() != 0)
        {
            if(WAFTPIsEntryDir(CurrentDirEntry) == 0)
            {
                EntrDt = WAFTPGetEntryDate(CurrentDirEntry);
                if(EntrDt.Len() != 0)
                {
                    WAListViewAddItem(FRMFTPListview, WAFTPGetEntryFileName(CurrentDirEntry), FRMFTPDirEntries, 0);
                    WAListViewSetSubItem(FRMFTPListview, WAFTPGetEntryFileSize(CurrentDirEntry), FRMFTPDirEntries, 1);
                    WAListViewSetSubItem(FRMFTPListview, EntrDt, FRMFTPDirEntries, 2);
                    WAListViewSetSubItem(FRMFTPListview, WAFTPGetEntryFileAttributes(CurrentDirEntry), FRMFTPDirEntries, 3);
                }
            }
            else
            {
                PointEntry = WAFTPGetEntryFileName(CurrentDirEntry);
                if(PointEntry == ".")
                {
                    FoundPoint = 1;
                    PointDate = WAFTPGetEntryDate(CurrentDirEntry);
                    PointAttr = WAFTPGetEntryFileAttributes(CurrentDirEntry);
                }
                else if(PointEntry == "..")
                {
                    FoundDoublePoint = 1;
                    DoublePointDate = WAFTPGetEntryDate(CurrentDirEntry);
                    DoublePointAttr = WAFTPGetEntryFileAttributes(CurrentDirEntry);
                }
                else
                {
                    EntrDt = WAFTPGetEntryDate(CurrentDirEntry);
                    if(EntrDt.Len() != 0)
                    {
                        WAListViewAddItem(FRMFTPListview, PointEntry, 0, 0);
                        WAListViewSetSubItem(FRMFTPListview, "(DIR)", 0, 1);
                        WAListViewSetSubItem(FRMFTPListview, EntrDt, 0, 2);
                        WAListViewSetSubItem(FRMFTPListview, WAFTPGetEntryFileAttributes(CurrentDirEntry), 0, 3);
                        FRMFTPDirEntries++;
                    }
                }
            }
        }
    }
    StringReleaseSplit(FTPDirectoriesArray);
    if(FoundDoublePoint == 1)
    {
        WAListViewAddItem(FRMFTPListview, "..", 0, 0);
        WAListViewSetSubItem(FRMFTPListview, "(DIR)", 0, 1);
        WAListViewSetSubItem(FRMFTPListview, DoublePointDate, 0, 2);
        WAListViewSetSubItem(FRMFTPListview, DoublePointAttr, 0, 3);
    }
    if(FoundPoint == 1)
    {
        WAListViewAddItem(FRMFTPListview, ".", 0, 0);
        WAListViewSetSubItem(FRMFTPListview, "(DIR)", 0, 1);
        WAListViewSetSubItem(FRMFTPListview, PointDate, 0, 2);
        WAListViewSetSubItem(FRMFTPListview, PointAttr, 0, 3);
    }
}

// -----------------------------------------------------------------------
// User select files to upload
long SelectFilesToUpload(HWND hWnd)
{
    CStr OpFilters;
    CStr FirstUp;
    CStr BufString;

    OpFilters = "All files (*.*)|*.*";
    UpLdFile = WAComDlgGetOpenFileName(hWnd, OpFilters, "", 1, CurrentDir);
    FilesArray = 0;
    if(UpLdFile.Len() == 0) return(0);
    FilesArray = StringSplit(UpLdFile, BufString.Chr(2));
    // Several files selected ?
    if(StringGetSplitUBound(FilesArray) > 0)
    {
        FirstUp = StringGetSplitElement(UpLdFile, FilesArray, 0);
        // Check anti-slash of first entry (directory)
        if(strcmp(FirstUp.Right(1).Get_String(), "\\") == 0)
        {
            UpLdFile = StringReplace(UpLdFile, BufString.Chr(2), (CStr) BufString.Chr(2) + (CStr) FirstUp, 1, -1, Binary_Compare);
        }
        else
        {
            UpLdFile = StringReplace(UpLdFile, BufString.Chr(2), (CStr) BufString.Chr(2) + (CStr) FirstUp + (CStr) "\\", 1, -1, Binary_Compare);
        }
        StringReleaseSplit(FilesArray);
        FilesArray = StringSplit(UpLdFile, BufString.Chr(2));
        goto UploadSelected;
    }
    // Single file selected ?
    if(StringGetSplitUBound(FilesArray) == 0)
    {
        FirstUp = StringGetSplitElement(UpLdFile, FilesArray, 0);
        StringReleaseSplit(FilesArray);
        // Must be first element not 0
        UpLdFile = BufString.Chr(2) + (CStr) FirstUp;
        FilesArray = StringSplit(UpLdFile, BufString.Chr(2));
    }
    else
    {
        // No files
        StringReleaseSplit(FilesArray);
        FilesArray = 0;
    }
UploadSelected:
    return(1);
}

// -----------------------------------------------------------------------
// Fill the selected files permissions slots
void FillPermissions(void)
{
    CStr FDPerm;

    CurrentSelected = WAListViewGetSelItem(FRMFTPListview, -1);
    PermFileName.Erase();
    PermFilePerms.Erase();
    PermNbrFiles = 0;
    PermOwnerR = 0;
    PermOwnerW = 0;
    PermOwnerX = 0;
    PermGroupR = 0;
    PermGroupW = 0;
    PermGroupX = 0;
    PermOtherR = 0;
    PermOtherW = 0;
    PermOtherX = 0;
    while(CurrentSelected != -1)
    {
        PermFileName.Add(WAListViewGetItemText(FRMFTPListview, CurrentSelected, 0).Get_String());
        PermFilePerms.Add("");
        PermNbrFiles++;
        FDPerm = WAListViewGetItemText(FRMFTPListview, CurrentSelected, 3);
        if(strcmpi(FDPerm.Mid(1, 1).Get_String(), "r") == 0) PermOwnerR++;
        if(strcmpi(FDPerm.Mid(2, 1).Get_String(), "w") == 0) PermOwnerW++;
        if(strcmpi(FDPerm.Mid(3, 1).Get_String(), "x") == 0) PermOwnerX++;
        if(strcmpi(FDPerm.Mid(4, 1).Get_String(), "r") == 0) PermGroupR++;
        if(strcmpi(FDPerm.Mid(5, 1).Get_String(), "w") == 0) PermGroupW++;
        if(strcmpi(FDPerm.Mid(6, 1).Get_String(), "x") == 0) PermGroupX++;
        if(strcmpi(FDPerm.Mid(7, 1).Get_String(), "r") == 0) PermOtherR++;
        if(strcmpi(FDPerm.Mid(8, 1).Get_String(), "w") == 0) PermOtherW++;
        if(strcmpi(FDPerm.Mid(9, 1).Get_String(), "x") == 0) PermOtherX++;
        CurrentSelected = WAListViewGetSelItem(FRMFTPListview, CurrentSelected);
    }
}

// -----------------------------------------------------------------------
// Listview hook
LRESULT CALLBACK FRMFTPListviewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYUP:
            // Del key
            if(wParam == 46)
            {
                WAControlSendMessage(FRMFTPhWnd, FTPDEL, (long) FRMFTPToolbar);
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Item sorting function
int CALLBACK FRMFTPListviewSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int ReturnValue = 0;
    CStr Data1;
    CStr Data2;
	long Datas1Lng = 0;
	long Datas2Lng = 0;
	FILETIME Date1;
	FILETIME Date2;

    Data1 = WAListViewGetItemText(FRMFTPListview, lParam1, 0);
    Data2 = WAListViewGetItemText(FRMFTPListview, lParam2, 0);
    if((strcmpi(Data1.Get_String(), ".") == 0) ||
       (strcmpi(Data2.Get_String(), ".") == 0))
    {
        ReturnValue = 0;
        goto ExitSort;
    }
    if((strcmpi(Data1.Get_String(), "..") == 0) ||
       (strcmpi(Data2.Get_String(), "..") == 0))
    {
        ReturnValue = 0;
        goto ExitSort;
    }
    Data1 = WAListViewGetItemText(FRMFTPListview, lParam1, lParamSort);
    Data2 = WAListViewGetItemText(FRMFTPListview, lParam2, lParamSort);
    switch(lParamSort)
    {
        case 0:
            // Names
            ReturnValue = strcmpi(Data1.Get_String(), Data2.Get_String());
			break;
		case 1:
            // Sizes
            // Dir entries are always lower
            if(strcmpi(Data1.Get_String(), "(DIR)") == 0)
            {
                if(strcmpi(Data2.Get_String(), "(DIR)") == 0)
                {
                    ReturnValue = 0;
                    goto ExitSort;
                }
                ReturnValue = -1;
                goto ExitSort;
            }
            if(strcmpi(Data2.Get_String(), "(DIR)") == 0)
            {
                ReturnValue = 1;
                goto ExitSort;
            }
			Datas1Lng = Data1.Get_Long();
			Datas2Lng = Data2.Get_Long();
	        if(Datas1Lng > Datas2Lng)
	        {
                ReturnValue = 1;
	        }
	        else if(Datas1Lng < Datas2Lng)
	        {
                ReturnValue = -1;
	        }
	        else
	        {
                ReturnValue = 0;
            }
			break;
        case 2:
			// (Return values are ignored here)
			WADateStringToDate(Data1, &Date1);
			WADateStringToDate(Data2, &Date2);
			ReturnValue = -CompareFileTime(&Date1, &Date2);
			break;
		case 3:
            // Permissions
            ReturnValue = strcmpi(Data1.Get_String(), Data2.Get_String());
			break;
	}
ExitSort:
    WAListViewReOrder(FRMFTPListview);
	return(ReturnValue);
}
