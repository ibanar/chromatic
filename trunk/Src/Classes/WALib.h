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
// WALib.h: declaration of used variables / structures for WALib.cpp
// -----------------------------------------------------------------------

#ifndef _WALIB_H_
#define _WALIB_H_

// -----------------------------------------------------------------------
// Includes
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501

#define HAVE_SSE_REGS
#include <windows.h>
#define DECLSPEC_DEPRECATED
#include <CommCtrl.h>
#include <CommDlg.h>
#include <richedit.h>
#include <shlobj.h>
#include <ShellAPI.h>
#include <Winsock.h>
#include <Psapi.h>           // Coming from the platform SDK
#include <Dbghelp.h>         // Coming from the platform SDK
#include <time.h>
#include <string.h>
#include <uxtheme.h>
#include <htmlhelp.h>

// Classes
#include "CList.h"
#include "CStr.h"

// External DLLs
#include "ChromaticSupDLL.h"

#include "../Miscfunctions.h"

// -----------------------------------------------------------------------
// Constants
#define MB_INFORMATION MB_ICONINFORMATION + MB_OK
#define MB_QUESTION MB_ICONQUESTION + MB_YESNO
#define MB_ERRORQUESTION MB_ICONERROR + MB_YESNO
#define MB_ERROR MB_ICONERROR + MB_OK
#define MB_QUESTIONCANCEL MB_ICONQUESTION + MB_YESNOCANCEL

#define SPLITTER_HORZ 1
#define SPLITTER_VERT 2
#define SPLITTER_RESIZING 1
#define SPLITTER_NORESIZING 0

#define MSG_SPLITTER_RESIZED WM_USER + 100
#define MSG_COLORBOX_CLICKED WM_USER + 101
#define MSG_SPLASH_PAINT WM_USER + 102

#define STATUSBAR_PARTSUNKEN 2
#define STATUSBAR_PARTNORMAL 4
#define STATUSBAR_PARTRAISED 8
#define STATUSBAR_PARTOWNER 16

#define WAINIT_OK 0
#define WAINIT_NOEXCONTROLS 1
#define WAINIT_RICHEDCONTROL10 2
#define WAINIT_NORICHEDCONTROL 4

#define DUMPBOX_LINELEFT WM_USER + 201
#define DUMPBOX_LINERIGHT WM_USER + 202
#define DUMPBOX_PAGELEFT WM_USER + 203
#define DUMPBOX_PAGERIGHT WM_USER + 204
#define DUMPBOX_LINEUP WM_USER + 205
#define DUMPBOX_LINEDOWN WM_USER + 206
#define DUMPBOX_PAGEUP WM_USER + 207
#define DUMPBOX_PAGEDOWN WM_USER + 208
#define DUMPBOX_POSITIONLEFTRIGHT WM_USER + 209
#define DUMPBOX_POSITIONUPDOWN WM_USER + 210
#define DUMPBOX_TRACKLEFTRIGHT WM_USER + 211
#define DUMPBOX_TRACKUPDOWN WM_USER + 212
#define DUMPBOX_TOP WM_USER + 213
#define DUMPBOX_BOTTOM WM_USER + 214
#define DUMPBOX_LEFT WM_USER + 215
#define DUMPBOX_RIGHT WM_USER + 216

#define MODALDLG_STOCK_EMPTY 0
#define MODALDLG_STOCK_STDCLOSE 1
#define MODALDLG_STOCK_STDOKCANCEL 2
#define MODALDLG_STOCK_WIZARD 3

#define WIZARD_STOCK_STDX 560
#define WIZARD_STOCK_STDY 357
#define WIZARD_STOCK_PICTUREX 200

#define BUTTON_STOCK_CLOSE_ID 1
#define BUTTON_STOCK_OK_ID 1
#define BUTTON_STOCK_FINISH_ID 1
#define BUTTON_STOCK_CANCEL_ID 2
#define BUTTON_STOCK_PREVIOUS_ID 3
#define BUTTON_STOCK_NEXT_ID 4

#define CHILD_MSG_CREATED WM_USER + 217
#define CHILD_MSG_DESTROYED WM_USER + 218
#define CHILD_MSG_SETTITLE WM_USER + 219

#define GRIPBOX_MSG_ASKSIZE WM_USER + 220
#define GRIPBOX_MSG_ASKSTEPS WM_USER + 221
#define GRIPBOX_MSG_RESIZED WM_USER + 222
#define GRIPBOX_TOPLEFT 0
#define GRIPBOX_TOP 1
#define GRIPBOX_TOPRIGHT 2
#define GRIPBOX_BOTTOMLEFT 3
#define GRIPBOX_BOTTOM 4
#define GRIPBOX_BOTTOMRIGHT 5
#define GRIPBOX_LEFT 6
#define GRIPBOX_RIGHT 7

#define GRIPBOX_STATE_RESET 0
#define GRIPBOX_STATE_RESIZING 1

#define CASE_SENSITIVE 0
#define CASE_UNSENSITIVE 1

#define MULTIFILESENUM_FORWARD 0
#define MULTIFILESENUM_BACKWARD 1

#define DATE_GET_SECONDS 1
#define DATE_NO_SECONDS 0

// -----------------------------------------------------------------------
// Structures
typedef struct _WAMDIDAT
{
    HWND hWnd;
    HWND hClient;
    HWND hDock;
} WAMDIDAT, *LPWAMDIDAT;

typedef struct _SPLITTERDAT
{
    HWND hClient;
    HWND hParent;
    long Orientation;
    long Sizing;
    long MinLeftTop;
    long MaxRightBottom;
    long SplitBarSize;
    long StartupPos;
    long OldXPos;
    long OldYPos;
    long OldWndProc;
    long StartDragX;
    long StartDragY;
    long DisplacementX;
    long DisplacementY;
} SPLITTERDAT, *LPSPLITTERDAT;

typedef struct _GRIPBOXDAT
{
    HWND TopLeftBox;
    HWND TopBox;
    HWND TopRightBox;
    HWND BottomLeftBox;
    HWND BottomBox;
    HWND BottomRightBox;
    HWND LeftBox;
    HWND RightBox;
} GRIPBOXDAT, *LPGRIPBOXDAT;

typedef struct _GRIPBOXSQRBOXDAT
{
	long Type;
	long State;
	long StepX;
	long StepY;
	RECT ResizeRect;
	RECT OldRect;
    COLORREF Color;
	LPGRIPBOXDAT Container;
} GRIPBOXSQRBOXDAT, *LPGRIPBOXSQRBOXDAT;

typedef struct _BACKDC
{
    HWND hWnd;
    COLORREF Color;
    HDC hDC;
    HBITMAP Bitmap;
    HGDIOBJ OldObject;
} BACKDC, *LPBACKDC;

typedef struct _DLGTEMPLATE2
{
	DLGTEMPLATE Diag;
	long Padding[6 + 1];
} DLGTEMPLATE2, *LPDLGTEMPLATE2;

typedef struct _NM_TREEVIEWEXPAND
{
    NMHDR hdr;
    long action;
    long itemOld;
    long itemNew;
    long itemNew2;
    long itemNew3;
    long itemNew4;
    long itemNew5;
    long itemNew6;
    long itemNew7;
    long itemNew8;
    long itemNew9;
} NM_TREEVIEWEXPAND, *LPNM_TREEVIEWEXPAND;

typedef struct _WAFILTERFILE
{
	long FileLength;
	HGLOBAL FileMem;
} WAFILTERFILE, *LPWAFILTERFILE;

// -----------------------------------------------------------------------
// Variables
extern HFONT WASerifFont;
extern HFONT WASerifFont10;
extern HFONT WACourrierNewFont9;
extern HFONT WAMarlettFont9;
extern HFONT WAArialFont8;
extern HBRUSH WABgBrush;
extern HWND LastDialog;
extern long WABgColor;
extern long WATxtColor;
extern long WAToolTipFont;
extern CHOOSECOLOR MyColor;
extern COLORREF MyCustomColors[17];
extern LOGFONT MyLogFont;
extern CHOOSEFONT MyFont;
extern OPENFILENAME MyOpenFName;
extern OPENFILENAME MySaveFName;
extern HGLOBAL hMemFont;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK WAWidgetInit(HINSTANCE hInst, HICON AppIcon, long vSplitCursor, long hSplitCursor);
void CALLBACK WAWidgetUnInit(HINSTANCE hInst);
HWND CALLBACK WACreateSplashDialog(long DLeft, long DTop, long DWidth, long DHeight, CStr DTitle, HWND hParent, HMENU hMenu, HICON hIcon,
					               void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType);
long CALLBACK WACreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, CStr DTitle, DLGPROC WindowProc, long Centered, long ExtraDatas);
long CALLBACK WACreateModalDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, DLGPROC WindowProc, long ExtraStyle, long Centered);
HWND CALLBACK WACreateDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, HMENU hMenu, HICON hIcon, CStr DTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType);
LRESULT CALLBACK DialogSetIcon(HWND hWnd, HICON hIcon);
long CALLBACK DialogGetXUnit(void);
long CALLBACK DialogGetYUnit(void);
long CALLBACK DialogXPixelToUnit(long XCoord, long XUnit);
long CALLBACK DialogYPixelToUnit(long YCoord, long YUnit);
long CALLBACK DialogXUnitToPixel(long XCoord, long XUnit);
long CALLBACK DialogYUnitToPixel(long YCoord, long YUnit);
HWND CALLBACK WACreateContainer(long SPLeft, long SPTop, long SPWidth, long SPHeight, long CtrlID, HWND hParent, CStr Name, WNDPROC WindowProc);
HWND CALLBACK WACreateSplitter(long SPLeft, long SPTop, long SPDim, long CtrlID, LPSPLITTERDAT SplitterStruct, WNDPROC WindowProc);
HWND CALLBACK WACreateSplitBar(long SPLeft, long SPTop, long SPWidth, long SPHeight, HWND hParent, long CtrlID, LPSPLITTERDAT SplitStruct);
HWND CALLBACK WACreateMDIDialog(long MDILeft, long MDITop, long MDIWidth, long MDIHeight, HWND hParent, HMENU hMenu, HICON hIcon, CStr MDITitle, 
					            void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ShowType);
HWND CALLBACK WACreateMDIChildDialog(HWND hParent, CStr CTitle, HICON hIcon, 
					                 void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ZeroPos, CStr CustomClass);
HWND CALLBACK WACreateToolBar(long TBLeft, long TBTop, long TBWidth, long TBHeight, HWND hParent, HIMAGELIST hImageList, long CtrlID, long StdBitmapsType, WNDPROC WindowProc, long ExtraStyle, long ExtendedStyle);
long CALLBACK WAToolBarAddButton(HWND hToolbar, CStr ButtonText, long TBButtonID, long ImgIndex, long ButtonStyle, long ButtonState, long NoImage);
long CALLBACK WAToolBarGetButton(HWND hToolbar, long TBButtonID, LPTBBUTTON ButtonStruct);
long CALLBACK WAToolBarGetButtonXSize(HWND hToolbar);
long CALLBACK WAToolBarGetButtonYSize(HWND hToolbar);
long CALLBACK WAToolBarGetButtonsCount(HWND hToolbar);
long CALLBACK WAToolBarGetRowsCount(HWND hToolbar);
HIMAGELIST CALLBACK WAToolBarGetImagelist(HWND hToolbar);
long CALLBACK WAToolBarAddSeparator(HWND hToolbar, long TBButtonID);
long CALLBACK WAToolBarGetNotifiedToolTip(long lParam);
long CALLBACK WAToolBarGetNotifiedDropDownItem(long lParam);
void CALLBACK WAToolBarDisplayToolTip(CStr TextToShow, long lParam);
long CALLBACK WAToolBarResize(HWND hToolbar);
long CALLBACK WAToolBarSetButtonPressed(HWND hToolbar, long TBButtonID, long Pressed);
long CALLBACK WAToolBarSetButtonChecked(HWND hToolbar, long TBButtonID, long Checked);
long CALLBACK WAToolBarSetButtonEnabled(HWND hToolbar, long TBButtonID, long EnableState);
long CALLBACK WAToolBarIsButtonPressed(HWND hToolbar, long TBButtonID);
long CALLBACK WAToolBarIsButtonChecked(HWND hToolbar, long TBButtonID);
long CALLBACK WAToolBarDisplayPopupMenu(HWND hToolbar, long TBButtonID, HMENU hMenu, HWND hParent);
long CALLBACK WAToolBarGetYSize(HWND hToolbar);
long CALLBACK WAToolBarGetXSize(HWND hToolbar);
long CALLBACK WAToolBarGetButtonXPosition(HWND hToolbar, long TBButtonID);
long CALLBACK WAToolBarGetXYSize(HWND hToolbar, LPSIZE TBSize);
long CALLBACK WAToolBarGetRealPos(HWND hToolbar, LPPOINT TBPosition);
long CALLBACK WAToolBarGetButtonXYPos(HWND hToolbar, long TBButtonID, LPPOINT TBPosition);
long CALLBACK WAToolBarGetButtonIndexXSize(HWND hToolbar, long TBButtonID);
long CALLBACK WAToolBarGetXPadding(HWND hToolbar);
long CALLBACK WAToolBarGetYPadding(HWND hToolbar);
long CALLBACK WAToolBarGetNotifiedHotItem(long lParam);
long CALLBACK WAToolBarRemoveButton(HWND hToolbar,long ButtonID);
long CALLBACK WAToolBarSetButtonBitmap(HWND hToolbar, long TBButtonID, long BtnIndex);
HIMAGELIST CALLBACK WACreateImageList(long IconWidth, long IconHeight, long MaxIcon);
void CALLBACK ImageListDestroy(HIMAGELIST hImageList);
long CALLBACK ImageListAddIcon(HIMAGELIST hImageList, long IconNumber);
long CALLBACK ImageListGetIconsCount(HIMAGELIST hImageList);
HICON CALLBACK ImageListGetIcon(HIMAGELIST hImageList, long IconIdx);
HWND CALLBACK WACreateSysTab(long FLeft, long FTop, long FWidth, long FHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle);
long CALLBACK WASysTabAddItem(HWND htab, CStr TabText, long TabIndex, long ImgIndex);
long CALLBACK WASysTabGetCurrentItem(HWND htab);
HIMAGELIST CALLBACK WASysTabGetImagelist(HWND htab);
long CALLBACK WASysTabSetCurrentItem(HWND htab, long TabItem);
long CALLBACK WASysTabSetFocusItem(HWND htab, long TabItem);
long CALLBACK WASysTabHighLightItem(HWND htab, long TabItem, long HighState);
long CALLBACK WASysTabItemsCount(HWND htab);
long CALLBACK WASysTabGetRowsCount(HWND htab);
long CALLBACK WASysTabSetSeparators(HWND htab, long Enable);
long CALLBACK WASysTabSetItemsSize(HWND htab,long Width,long Height);
long CALLBACK WASysTabRemoveItem(HWND htab,long ItemIndex);
long CALLBACK WASysTabSetMinWidth(HWND htab,long Width);
long CALLBACK WASysTabSetPadding(HWND htab,long Width,long Height);
HWND CALLBACK WACreateTrackBar(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long TBMin, long TBMax, long ExtraStyle);
long CALLBACK WATrackBarSetRange(HWND hTrackBar, long TBMin, long TBMax, long Redraw);
long CALLBACK WATrackBarSetPos(HWND hTrackBar, long TBNewPos);
long CALLBACK WATrackBarGetPos(HWND hTrackBar);
long CALLBACK WATrackBarSetTicks(HWND hTrackBar, long TicksFreq);
HWND CALLBACK WATrackBarGetToolTips(HWND hTrackBar);
HWND CALLBACK WACreateToolTip(HWND hParent, long CtrlID, long ExtraStyle);
long CALLBACK WAToolTipAddTool(HWND hToolTip, HWND hWndOwner, long ID, long ExtraStyle);
HWND CALLBACK WACreateUpDown(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK WACreateProgressBar(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long PBMin, long PBMax, long ExtraStyle);
long CALLBACK ProgressBarSetRange(HWND hProgressBar, long PBMin, long PBMax);
long CALLBACK ProgressBarSetRelPos(HWND hProgressBar, long PBNewPos);
HWND CALLBACK WACreateStatusBar(CStr SBText, long RaiseType, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK StatusBarSetParts(HWND hStatusBar, long SBNbParts, long *PartsArray);
long CALLBACK StatusBarSetText(HWND hStatusBar, long SBPart, long RaiseType, CStr SBText, long OwnerDatas);
long CALLBACK StatusBarGetYSize(HWND hStatusBar);
HWND CALLBACK WACreateColorBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, COLORREF Color, long ExtraStyle);
COLORREF CALLBACK ColorBoxGetColor(HWND hWnd);
long CALLBACK ColorBoxSetColor(HWND hWnd, COLORREF Color);
HWND CALLBACK WACreateDumpBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, HFONT hFont, long MaxX, long MaxY, long PageValueX, long PageValueY, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
void CALLBACK WADumpBoxResize(HWND hWnd, long left, long top, long Width, long Height);
long CALLBACK WADumpBoxScrollDown(HWND hWnd, long Lines, long Factor, long Integral);
long CALLBACK WADumpBoxScrollUp(HWND hWnd, long Lines, long Factor, long Integral);
long CALLBACK WADumpBoxGetVisibleLines(HWND hWnd, long Factor, long Intregral);
long CALLBACK WADumpBoxGetVisibleColumns(HWND hWnd, long Factor, long Integral);
LPGRIPBOXDAT CALLBACK WACreateGripBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, COLORREF Color, long ExtraStyle);
COLORREF CALLBACK GripBoxGetColor(LPGRIPBOXDAT GripDats);
void CALLBACK GripBoxSetColor(LPGRIPBOXDAT GripDats, COLORREF Color);
void CALLBACK GripBoxClose(LPGRIPBOXDAT GripDats);
void CALLBACK GripBoxVisible(LPGRIPBOXDAT GripDats, long CState);
void CALLBACK GripBoxResize(LPGRIPBOXDAT GripDats, LPRECT NewSizeRect);
HWND CALLBACK WACreateLabel(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, CStr LText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, HFONT hFont);
HGDIOBJ CALLBACK LabelGetImage(HWND hLabel, long ImageType);
HWND CALLBACK WACreatePictureBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, HGDIOBJ hPicture, long PictureType, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK PictureBoxChangeIcon(HWND hPictureBox, HICON IconHandle);
long CALLBACK PictureBoxChangeImage(HWND hPictureBox, HANDLE ImageHandle);
HWND CALLBACK WACreateFrame(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK FrameRefreshToolbar(HWND hToolbar);
HWND CALLBACK WACreateButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, long ImgType, HICON hImage, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
long CALLBACK ButtonSetIcon(HWND hbutton, long ImType, HICON hImage);
HANDLE CALLBACK ButtonGetImage(HWND hbutton, long ImageType);
long CALLBACK ButtonGetState(HWND hbutton);
HWND CALLBACK WACreateIPBox(long IPBLeft, long IPBTop, long IPBWidth, long IPBHeight, HWND hParent, long IPAddress, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder, HFONT hFont);
long CALLBACK IPBoxClearIP(HWND hIPBox);
long CALLBACK IPBoxGetIP(HWND hIPBox);
long CALLBACK IPBoxSetIP(HWND hIPBox, long IPAddress);
long CALLBACK IPBoxSetFocus(HWND hIPBox, long FieldNumber);
long CALLBACK IPBoxSetRange(HWND hIPBox, long FieldNumber, long LowValue, long HighValue);
long CALLBACK IPBoxIsBlank(HWND hIPBox);
HWND CALLBACK WACreateRichTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder);
HWND CALLBACK WACreateTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
long CALLBACK WATextBoxGetLen(HWND hTextBox);
long CALLBACK WATextBoxSetMaxLen(HWND hTextBox, long TBMaxLen);
long CALLBACK WATextBoxGetMaxLen(HWND hTextBox);
long CALLBACK WATextBoxGetTextHandle(HWND hTextBox);
void CALLBACK WATextBoxAddText(HWND hTextBox, CStr TbText);
long CALLBACK WATextBoxSelText(HWND hTextBox, long NbrToSel);
long CALLBACK WATextBoxRemoveSel(HWND hTextBox);
long CALLBACK WATextBoxSetCaretPos(HWND hTextBox, long PosToSel);
long CALLBACK WATextBoxLinesCount(HWND hTextBox);
long CALLBACK WATextBoxGetMarginWidth(HWND hTextBox);
long CALLBACK WATextBoxGetFirstVisibleLine(HWND hTextBox);
long CALLBACK WATextBoxIsModified(HWND hTextBox);
long CALLBACK WATextBoxGetCurrentOptions(HWND hTextBox);
CStr CALLBACK WATextBoxGetPasswordChar(HWND hTextBox);
long CALLBACK WATextBoxSetPasswordChar(HWND hTextBox, CStr PassChar);
long CALLBACK WATextBoxGetSelection(HWND hTextBox);
long CALLBACK WATextBoxGetScrollPos(HWND hTextBox);
long CALLBACK WATextBoxGetBreakProcAddress(HWND hTextBox);
long CALLBACK WATextBoxCanPaste(HWND hTextBox);
long CALLBACK WATextBoxCanUndo(HWND hTextBox);
HWND CALLBACK WACreateHexBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, long MaxDigits, long ExtraStyle, long ExtraExStyle);
HWND CALLBACK WACreateCheckBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long UndefState);
long CALLBACK CheckBoxGetState(HWND hCB);
long CALLBACK CheckBoxSetState(HWND hCB, long CheckedState);
HWND CALLBACK WACreateRebar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, WNDPROC WindowProc, long CtrlID, long ExtraStyle);
long CALLBACK WARebarAddBand(HWND hRebar, HWND hChild, CStr RBText, long RBYSize, long ExtraStyle, long MinSize);
long CALLBACK WARebarRemoveBand(HWND hRebar, long BandNumber);
long CALLBACK WARebarGetXSize(HWND hRebar);
long CALLBACK WARebarGetYSize(HWND hRebar);
long CALLBACK WARebarResize(HWND hRebar);
long CALLBACK WARebarSetBackColor(HWND hRebar, COLORREF RBColor);
long CALLBACK WARebarSetTextColor(HWND hRebar, COLORREF RBColor);
long CALLBACK WARebarBandVisible(HWND hRebar, long RBBand, long RBVisible);
long CALLBACK WARebarGetBandsCount(HWND hRebar);
long CALLBACK WARebarGetHeight(HWND hRebar);
COLORREF CALLBACK WARebarGetBackGroundColor(HWND hRebar);
long CALLBACK WARebarGetRowsCount(HWND hRebar);
long CALLBACK WARebarGetToolTips(HWND hRebar);
COLORREF CALLBACK WARebarGetTextColor(HWND hRebar);
long CALLBACK WARebarGetPalette(HWND hRebar);
HIMAGELIST CALLBACK WARebarGetImageList(HWND hRebar);
HWND CALLBACK WACreatePager(long PLeft, long PTop, long PWidth, long PHeight, HWND hParent, HWND hChild, long CtrlID, long ExtraStyle);
void CALLBACK WAPagerDisplaySetSize(LPNMPGCALCSIZE lParam, LPSIZE PGNewSize);
long CALLBACK WAPagerMaximizeX(HWND hPager, long ChildYSize);
long CALLBACK WAPagerMaximizeY(HWND hPager, long ChildXSize);
HWND CALLBACK WAPagerGetNotifiedChild(long lParam);
long CALLBACK WAPagerSetBackColor(HWND hPager, COLORREF Color);
COLORREF CALLBACK WAPagerGetBackColor(HWND hPager);
long CALLBACK WAPagerGetBorderSize(HWND hPager);
long CALLBACK WAPagerGetButtonSize(HWND hPager);
long CALLBACK WAPagerGetScrollPos(HWND hPager);
long CALLBACK WAPagerGetOrientation(HWND hPager);
long CALLBACK WAPagerSetOrientation(HWND hPager, long Orientation);
long CALLBACK WAPagerChangeOrientation(HWND hPager, LPNMPGCALCSIZE CalcSizeNotify, long NewOrientation, LPSIZE NewSize);
HWND CALLBACK WACreateClient(HWND hParent, HMENU hMenuWindowList, HICON hIcon);
long CALLBACK WAClientResize(HWND hClient, long AddX, long AddY, long SubX, long SubY);
HWND CALLBACK WAClientGetActiveChild(HWND hClient);
HWND CALLBACK WAClientSetNextChild(HWND hClient);
HWND CALLBACK WAClientSetPreviousChild(HWND hClient);
long CALLBACK WAClientTileHorizontal(HWND hClient);
long CALLBACK WAClientTileVertical(HWND hClient);
long CALLBACK WAClientTileCascade(HWND hClient);
long CALLBACK WAClientTileArrangeIconic(HWND hClient);
HWND CALLBACK WACreateRadioButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK RadioButtonGetState(HWND hRB);
long CALLBACK RadioButtonSetState(HWND hRB, long CheckedState);
HWND CALLBACK WACreateListView(long LVLeft, long LVTop, long LVWidth, long LVHeight, HWND hParent, long CtrlID, HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle, long WStyle, long ExtraExStyle);
long CALLBACK WAListViewAddCol(HWND hListview, CStr LVColText, long LVWidth, long ColPosition);
long CALLBACK WAListViewSetColWidth(HWND hListview, long LVColNumber, long LVWidth);
long CALLBACK WAListViewGetColWidth(HWND hListview, long LVColNumber);
long CALLBACK WAListViewSetScroll(HWND hLV, long DeltaX, long DeltaY);
long CALLBACK WAListViewClear(HWND hLV);
long CALLBACK WAListViewDetachImageList(HWND hLV);
long CALLBACK WAListViewAddItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVImage);
long CALLBACK WAListViewDeleteItem(HWND hListview, long LVItemPos);
long CALLBACK WAListViewSetSubItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVSubItemPos);
long CALLBACK WAListViewSetSubItemImage(HWND hListview, long LVItemImage, long LVItemPos, long LVSubItemPos);
long CALLBACK WAListViewGetSubItemImage(HWND hListview, long LVItemPos, long LVSubItemPos);
long CALLBACK WAListViewEditValidated(long lParam);
CStr CALLBACK WAListViewGetEditResult(long lParam);
long CALLBACK WAListViewItemCount(HWND hListview);
long CALLBACK WAListViewSelItemCount(HWND hListview);
long CALLBACK WAListViewMultiSelFirstItem(HWND hListview);
long CALLBACK WAListViewGetItemSelState(HWND hListview, long LVItemIndex);
HWND CALLBACK WAListViewSetEditModeOn(HWND hListview, long LVItemIndex, long ExtraEditStyle);
long CALLBACK WAListViewSetItemSel(HWND hListview, long LVItemIndex);
long CALLBACK WAListViewSetItemHighlight(HWND hListview, long LVItemIndex, long Enable);
HWND CALLBACK WAListViewGetEditControl(HWND hListview);
long CALLBACK WAListViewSetItemFocus(HWND hListview, long LVItemIndex);
long CALLBACK WAListViewGetItemUnderCursor(HWND hListview);
long CALLBACK WAListViewGetFirstFocusItem(HWND hListview);
long CALLBACK WAListViewGetSelItem(HWND hListview, long LVIndex);
CStr CALLBACK WAListViewGetSelItemText(HWND hListview, long LVSubItemNumber);
long CALLBACK WAListViewSetSelItemText(HWND hListview, CStr LVItemText, long LVSubItemNumber);
long CALLBACK WAListViewCheckBoxItemDoubleClick(HWND hListview);
CStr CALLBACK WAListViewGetItemText(HWND hListview, long LVItemIndex, long LVSubItemNumber);
HICON CALLBACK WAListViewGetItemIcon(HWND hListview, long LVItemIndex, long LVSubItemNumber);
long CALLBACK WAListViewGetNotifiedColumnIndex(long lParam);
long CALLBACK WAListViewSort(HWND hListview, long LVSubItemNumber, PFNLVCOMPARE SortProc);
long CALLBACK WAListViewReOrder(HWND hListview);
CStr CALLBACK WAListViewGetHeaderLabel(HWND hListview, long LVColNumber);
long CALLBACK WAListViewGetHeaderPosition(HWND hListview, long LVColNumber);
long CALLBACK WAListViewSetHeaderPosition(HWND hListview, long LVColNumber, long NewPosition);
long CALLBACK WAListViewSetBackColor(HWND hListview, COLORREF LVColor);
COLORREF CALLBACK WAListViewGetBackColor(HWND hListview);
COLORREF CALLBACK WAListViewGetTextBackColor(HWND hListview);
COLORREF CALLBACK WAListViewGetTextColor(HWND hListview);
long CALLBACK WAListViewGetToolTips(HWND hListview);
long CALLBACK WAListViewGetTopIndex(HWND hListview);
long CALLBACK WAListViewGetCountPerPage(HWND hListview);
long CALLBACK WAListViewGetExStyle(HWND hListview);
HWND CALLBACK WAListViewGetSysHeader(HWND hListview);
long CALLBACK WAListViewGetHotCursor(HWND hListview);
HIMAGELIST CALLBACK WAListViewGetImageList(HWND hListview, long ListType);
long CALLBACK WAListViewSetItemCheckbox(HWND hListview, long LVItemIndex, long LVCheckState);
long CALLBACK WAListViewGetItemCheckbox(HWND hListview, long LVItemIndex);
long CALLBACK WAListViewIsCheckboxNotify(HWND hListview, long LVItemIndex);
long CALLBACK WAListViewFindItem(HWND hListview, CStr LVItemText);
long CALLBACK WAListViewFindSubItem(HWND hListview, CStr TextToFind, long SubItemNbr, long SearchCase);
long CALLBACK WAListViewBeginDrag(long lParam, HWND hParent, LPPOINT DragRect);
long CALLBACK WAListViewMoveDrag(LPPOINT DragRect);
void CALLBACK WAListViewEndDrag(HWND hParent);
long CALLBACK WAListViewPasteAutoEdit(HWND hListview, long wParam, long ExtraEditStyle);
HWND CALLBACK WACreateTreeView(long TVLeft, long TVTop, long TVWidth, long TVHeight, HWND hParent, long CtrlID, HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
HTREEITEM CALLBACK WATreeViewAddItem(HWND hTreeView, CStr TVItemText, HTREEITEM TVParent, HTREEITEM TVChildPos, long TVImage, long TVSelImage, long ExtraStyle, long RefreshParentAfterAdd);
long CALLBACK WATreeViewSetToolTips(HWND hTreeView, long hToolTips);
long CALLBACK WATreeViewGetToolTips(HWND hTreeView);
long CALLBACK WATreeViewSetIndent(HWND hTreeView, long TVIndent);
long CALLBACK WATreeViewRemoveItem(HWND hTreeView, HTREEITEM hItem);
long CALLBACK WATreeViewSetBackColor(HWND hTreeView, COLORREF TVColor);
long CALLBACK WATreeViewGetExpandingState(long lParam);
HTREEITEM CALLBACK WATreeViewGetExpandingItem(long lParam);
HTREEITEM CALLBACK WATreeViewGetDeletedItem(long lParam);
long CALLBACK WATreeViewSetItemIcon(HWND hTreeView, HTREEITEM hItem, long IconNumber);
long CALLBACK WATreeViewSetItemExpandedState(HWND hTreeView, HTREEITEM hItem, long ExpandState);
long CALLBACK WATreeViewGetItemExpandedState(HWND hTreeView, HTREEITEM hItem);
long CALLBACK WATreeViewSetSingleExpandState(HWND hTreeView, long ExpandState);
HTREEITEM CALLBACK WATreeViewGetSelectedItem(HWND hTreeView);
HTREEITEM CALLBACK WATreeViewGetItemFromPos(HWND hTreeView, long XPos, long YPos);
long CALLBACK WATreeViewSetSelectedItem(HWND hTreeView, HTREEITEM hItem);
CStr CALLBACK WATreeViewGetItemText(HWND hTreeView, HTREEITEM hItem);
long CALLBACK WATreeViewSetItemText(HWND hTreeView, HTREEITEM hItem, CStr ItemText);
HTREEITEM CALLBACK WATreeViewGetItemParent(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK WATreeViewGetRoot(HWND hTreeView);
long CALLBACK WATreeViewGetItemCount(HWND hTreeView);
long CALLBACK WATreeViewGetVisibleCount(HWND hTreeView);
HTREEITEM CALLBACK WATreeViewGetFirstItemChild(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK WATreeViewGetNextItem(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK WATreeViewGetPreviousItem(HWND hTreeView, HTREEITEM hItem);
long CALLBACK WATreeViewSearchItemText(HWND hTreeView, HTREEITEM hNode, CStr ItemText);
long CALLBACK WATreeViewSearchChildPartialText(HWND hTreeView, HTREEITEM hNode, CStr ItemText);
long CALLBACK WATreeViewGetChildItemsCount(HWND hTreeView, HTREEITEM hNode);
long CALLBACK WATreeViewGetSelChildItemPos(HWND hTreeView, HTREEITEM hNode, HTREEITEM hParent);
long CALLBACK WATreeViewGetItemLevel(HWND hTreeView, HTREEITEM hNode);
COLORREF CALLBACK WATreeViewGetBackColor(HWND hTreeView);
long CALLBACK WATreeViewGetItemTextRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect);
long CALLBACK WATreeViewGetItemRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect);
HWND CALLBACK WATreeViewGetEditControl(HWND hTreeView);
HIMAGELIST CALLBACK WATreeViewGetImageList(HWND hTreeView, long ListType);
long CALLBACK WATreeViewGetIndent(HWND hTreeView);
COLORREF CALLBACK WATreeViewGetInsertMarkColor(HWND hTreeView);
COLORREF CALLBACK WATreeViewGetLineColor(HWND hTreeView);
COLORREF CALLBACK WATreeViewGetTextColor(HWND hTreeView);
long CALLBACK WATreeViewGetItemHeight(HWND hTreeView);
long CALLBACK WATreeViewGetScrollTime(HWND hTreeView);
HWND CALLBACK WACreateFictive(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, HICON hImage);
HWND CALLBACK WACreateToolTip(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK WACreateAnimation(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK WACreateHotkey(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long InvalidCombination, long ValidCombination, long VirtualKey, long Modifiers, long ExtraStyle);
long CALLBACK WAHotkeySetRules(HWND hHotkey, long InvalidCombination, long ValidCombination);
long CALLBACK WAHotkeySetKey(HWND hHotkey, long VirtualKey, long Modifiers);
HWND CALLBACK WACreateDateTimePicker(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK WACreateMonthCalendar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK WACreateHeader(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK WAHeaderAddItem(HWND hHeader, CStr HeaderText, long Headerwidth, long HeaderPosition, long ExtraStyle);
HWND CALLBACK WACreateExComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle);
long CALLBACK WAExCBSetImageList(HWND hExCB, HIMAGELIST hImageList);
HWND CALLBACK WACreateComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK WAComboBoxAddItem(HWND hCB, CStr CBItemText, long CBItemIndex);
long CALLBACK WAComboBoxDeleteItem(HWND hCB, long CBItemIndex);
long CALLBACK WAComboBoxReset(HWND hCB);
long CALLBACK WAComboBoxSetMaxLen(HWND hCB, long TBMaxLen);
long CALLBACK WAComboBoxCount(HWND hCB);
long CALLBACK WAComboBoxGetIndex(HWND hCB);
long CALLBACK WAComboBoxSetIndex(HWND hCB, long CBIndex);
CStr CALLBACK WAComboBoxGetItem(HWND hCB, long CBIndex);
CStr CALLBACK WAComboBoxGetCurrentItem(HWND hCB);
long CALLBACK WAComboBoxItemExist(HWND hCB, CStr CBItem);
long CALLBACK WAComboBoxGetItemData(HWND hLB, long CBIndex);
void WAComboBoxSaveInIniFile(HWND hCombo, CStr EntryToAdd, CStr IniKey, CStr IniFile);
void WAComboBoxFillFromIniFile(HWND hCombo, CStr IniKey, CStr IniFile);
HWND CALLBACK WACreateListBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long DragOn, long ExtraStyle, long ExtraExStyle);
long CALLBACK WAListBoxAddItem(HWND hLB, CStr LBItemText, long LBItemIndex);
long CALLBACK WAListBoxReset(HWND hLB);
long CALLBACK WAListBoxSetHorzScrollWidth(HWND hLB, long LBWidth);
long CALLBACK WAListBoxSetColumnsWidth(HWND hLB, long LBWidth);
long CALLBACK WAListBoxCount(HWND hLB);
long CALLBACK WAListBoxGetTopIndex(HWND hLB);
long CALLBACK WAListBoxSetTopIndex(HWND hLB, long LBIndex);
long CALLBACK WAListBoxSetItemData(HWND hLB, long LBIndex, long LBDat);
long CALLBACK WAListBoxGetItemData(HWND hLB, long LBIndex);
long CALLBACK WAListBoxIsItemSelected(HWND hLB, long LBIndex);
long CALLBACK WAListBoxGetSelItemIndex(HWND hLB);
long CALLBACK WAListBoxSetIndex(HWND hLB, long LBIndex);
CStr CALLBACK WAListBoxGetCurrentItem(HWND hLB);
CStr CALLBACK WAListBoxGetItem(HWND hLB, long LBIndex);
long CALLBACK WAListBoxDeleteItem(HWND hLB, long LBIndex);
long CALLBACK WAListBoxReplaceItem(HWND hLB, long LBIndex, CStr LBNewItemText);
long CALLBACK WAListBoxReplaceSelItem(HWND hLB, CStr LBNewItemText);
void CALLBACK WAListBoxProcessDrag(HWND hLB, long Message, long lParam, long DragCursor);
long CALLBACK WAListBoxSelItemDown(HWND hLB);
long CALLBACK WAListBoxSelItemUp(HWND hLB);
long CALLBACK WAListBoxSelItemRemove(HWND hLB);
long CALLBACK WAListBoxItemExist(HWND hListBox, CStr CBItem);
long CALLBACK WAListBoxItemUnderCursor(HWND hListBox);
HWND CALLBACK WACreateScrollBar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK WAScrollBarGetMinRange(HWND hWnd, long ScrollBarType);
long CALLBACK WAScrollBarGetMaxRange(HWND hWnd, long ScrollBarType);
long CALLBACK WAScrollBarGetPageRange(HWND hWnd, long ScrollBarType);
long CALLBACK WAScrollBarSetPageRange(HWND hWnd, long ScrollBarType, long PageValue);
long CALLBACK WAScrollBarSetMinMaxRange(HWND hWnd, long ScrollBarType, long nMin, long nMax);
void CALLBACK WAScrollBarShowHide(HWND hWnd, long ObjWidth, long ObjHeight, long Useclient);
HWND CALLBACK WACreateDockingBox(CStr Title, WNDPROC WindowProc, HWND hRoot, char *IniFile);
long CALLBACK DockingBoxWasVisible(CStr DockName, CStr IniFile);
void CALLBACK DockingBoxRepaint(HWND hWnd);
long CALLBACK DockingBoxGetState(HWND hDock);
void CALLBACK DockingBoxRemove(HWND hDock);
void CALLBACK DockingBoxShow(HWND hDock, int MaxXSize, int MaxYSize, int DefaultXSize, int DefaultYSize, int DefaultState);
CStr CALLBACK WAMenuGetString(HMENU hMenu, long ItemID);
long CALLBACK WAMenuEnable(HMENU hMenu, long ItemPosition, long Enable);
long CALLBACK WAMenuSetDefaultItem(HMENU hMenu, long ItemPosition);
long CALLBACK WAMenuSetItemState(HMENU hMenu, long ItemState, long ItemID);
long CALLBACK WAMenuSetItemType(HMENU hMenu, long ItemType, long ItemID);
long CALLBACK WAMenuSetItemText(HMENU hMenu, CStr ItemText, long ItemID);
long CALLBACK ScreenWidth(void);
long CALLBACK ScreenHeight(void);
void CALLBACK ScreenRectToClient(HWND hWnd, LPRECT RectToFill);
HCURSOR CALLBACK WACursorSetWait(void);
HCURSOR CALLBACK WACursorSetNo(void);
HCURSOR CALLBACK WACursorSetCross(void);
HCURSOR CALLBACK WACursorSetNormal(void);
long CALLBACK WACursorDisable(void);
long CALLBACK WACursorEnable(long OldCursorState);
CStr CALLBACK WAIniReadKey(CStr Section, CStr Key, CStr File);
long CALLBACK WAIniReadBoolKey(CStr Section, CStr Key, CStr File);
long CALLBACK WAIniWriteSection(CStr Section, CStr Key, CStr File);
long CALLBACK WAIniDeleteKey(CStr Section, CStr Key, CStr File);
long CALLBACK WAIniWriteKey(CStr Section, CStr Key, CStr Value, CStr File);
long CALLBACK WAMiscClipBoardCopyText(CStr Txt);
CStr CALLBACK WAMiscClipBoardPasteText(void);
long CALLBACK WAMiscClipBoardIsEmpty(void);
void CALLBACK WAMiscShellAddFileToRecents(CStr FileName);
long CALLBACK MathGenerateRandomNumber(long MaxNumber);
long CALLBACK WAMiscWaitEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators);
long CALLBACK WAMiscMsgBox(HWND hParent, CStr MBText, long MBType, CStr MBTitle);
long CALLBACK WAMiscSendBroadCastMsg(long BDMessage);
void CALLBACK WAMiscDoEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators);
HRESULT CALLBACK WAMiscObtainGUID(LPGUID GUIDToObtain);
HGLOBAL CALLBACK WAMiscAllocMem(long Amount);
long CALLBACK WAMiscFreeMem(void *MemHandle);
void CALLBACK WAMiscCopyMemLoop(void *MemSource, void *MemDest, long Length, long MemLeftOffset, long RepeatCount);
void CALLBACK WAMiscCopyMemWithOffsets(void *MemSource, void *MemDest, long Length, long SourceOffset, long DestOffset);
long CALLBACK WAMiscGetOSClass(void);
CStr CALLBACK WADateGetNow(long ReportsSeconds);
CStr CALLBACK WADateGetYear(void);
CStr CALLBACK WADateGetMonth(void);
CStr CALLBACK WADateGetDay(void);
CStr CALLBACK WADateGetHour(void);
CStr CALLBACK WADateGetMinutes(void);
CStr CALLBACK WADateGetSeconds(void);
CStr CALLBACK WADateGetFormat(CStr DateFormat);
CStr CALLBACK WADateGetTimeFormat(CStr DateFormat);
long CALLBACK WADateStringToDate(CStr StrDate, FILETIME *DestDate);
long CALLBACK StringFilterNonPrintableChars(long ASCIIChar, long CorrectChar);
long CALLBACK StringIsLabel(CStr AsmText);
long CALLBACK StringIsHexaDecimal(CStr HexaText);
long CALLBACK StringIsDecimal(CStr DecText, long MaxLen);
long CALLBACK StringIsLabelChar(CStr CLetter);
long CALLBACK StringIsDigitChar(CStr Letter);
long CALLBACK StringIsHexaChar(CStr Letter);
long CALLBACK StringIsAlphaChar(CStr Letter);
long CALLBACK StringIsBlankChar(CStr Letter);
long CALLBACK StringIsOpenBracketChar(CStr Letter);
long CALLBACK StringIsCloseBracketChar(CStr Letter);
long CALLBACK StringIsUnderLineChar(CStr Letter);
long CALLBACK StringIsDotChar(CStr Letter);
long CALLBACK StringIsQuoteChar(CStr Letter);
long CALLBACK StringIsCommaChar(CStr Letter);
long CALLBACK StringIsArobasChar(CStr Letter);
long CALLBACK StringIsEOL(CStr Letter);
CStr CALLBACK StringNumberComplement(long Nbr, long Complement);
CStr CALLBACK StringHexNumberComplement(long Nbr, long Complement);
CStr CALLBACK StringBinNumberComplement(long Nbr, long Complement);
void CALLBACK StringFree(long StringToFree);
long CALLBACK StringLen(long StringToCalc);
CStr CALLBACK StringCat(CStr String1, CStr String2);
CStr CALLBACK StringDecToString(long DecimalNbr);
long CALLBACK StringStringToDec(CStr DecString);
long CALLBACK StringStringToHex(CStr HexString);
long CALLBACK StringCount(CStr MaIn_String, CStr StringToCount, long CountStart, long CompareType);
CStr CALLBACK StringReplace(CStr MaIn_String, CStr StringToFind, CStr StringToReplace, long ReplaceStart, long RepCount, long CompareType);
CStr CALLBACK StringHex32(long Number);
CStr CALLBACK StringHex16(long Number);
CStr CALLBACK StringHex8(long Number);
CStr CALLBACK StringCopy(CStr DestString, CStr SourceString, long SourceStringLen, long DestStringLen);
CStr CALLBACK StringCopyAppendZero(CStr DestString, CStr SourceString);
CStr CALLBACK StringCopyFixed(CStr DestString, CStr SourceString);
CStr CALLBACK StringClear(CStr DestString, long DestStringLen);
CStr CALLBACK StringAppend(CStr DestString, CStr SourceString);
long * CALLBACK StringSplit(CStr StrToSplit, CStr StrSplitter);
long CALLBACK StringGetSplitUBound(long * SplitMem);
CStr CALLBACK StringGetSplitElement(CStr StrFromSplit, long *SplitMem, long Element);
void CALLBACK StringReleaseSplit(long *SplitMem);
CStr CALLBACK StringJoin(CStr StrFromSplit, long *SplitMem, CStr JoIn_String, long FirstElement, long ElementLimit);
CStr CALLBACK StringConvertToCStr(char *StringToConvert);
char * CALLBACK StringCStrConvertToString(CStr StringToConvert);
long CALLBACK StringCountLines(CStr StringToCount, long Length);
long CALLBACK StringGetLinesOffsets(CStr StringToCount);
void CALLBACK StringFillHexBytes(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFillHexWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFillHexDWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFileNameToLabel(char *FileName);
long CALLBACK WAFileGetWriteTime(CStr FileName, LPFILETIME FileWriteTime);
long CALLBACK WAFileGetCreationTime(CStr FileName, LPFILETIME FileCreationTime);
long CALLBACK WAFileGetAccessedTime(CStr FileName, LPFILETIME FileAccessedTime);
CStr CALLBACK WAFileGetExtension(CStr FileName);
CStr CALLBACK WAFileRemoveExtension(CStr FileName);
CStr CALLBACK WAFileReplaceExtension(CStr FileName, CStr NewExtension);
CStr CALLBACK WAFileGetDirectory(CStr FileName);
CStr CALLBACK WAFileGetFileName(CStr FileName);
HANDLE CALLBACK WAFileOpenR(CStr FileName);
HANDLE CALLBACK WAFileOpenW(CStr FileName, long FilePos);
HANDLE CALLBACK WAFileOpenWAppend(CStr FileName);
long CALLBACK WAFileClose(HANDLE hFile);
HANDLE CALLBACK WAFileCreateEmpty(CStr FileName, long Security);
long CALLBACK WAFileWriteBuffer(HANDLE FileHandle, LPCVOID BufferToWrite, long LenToWrite);
long CALLBACK WAFileWriteBufferVB(HANDLE FileHandle, CStr BufferToWrite, long LenToWrite);
long CALLBACK WAFileWriteLine(HANDLE FileHandle, CStr LineToWrite);
long CALLBACK WAFileIsDirectory(CStr FileToSearch);
long CALLBACK WAFileGetSize(CStr FileName);
long CALLBACK WAFileExist(CStr FileToSearch);
long CALLBACK WAFileIsReadOnly(CStr FileToCheck);
long CALLBACK WAFileIsUnix(CStr FileToCheck);
HGLOBAL CALLBACK WAFileLoadIntoMem(CStr FileName, long *BytesRead);
long CALLBACK WAFileSaveFromMem(CStr FileName, long MemToSave, long LenToSave);
CStr CALLBACK WAFileDir(CStr *PathName = 0,int Attributes = FILE_ATTRIBUTE_NORMAL);
long CALLBACK WARegistryUpdateKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, CStr SubKeyValue, long KeyType);
long CALLBACK WARegistryDeleteKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, long KeyType);
CStr CALLBACK WARegistryGetKeyValue(HKEY KeyRoot, CStr KeyName, CStr SubKeyRef);
void CALLBACK WAGDIFillWindow(HWND hWnd, COLORREF Color);
void CALLBACK WAGDIFillWindowFromBrush(HWND hWnd, HBRUSH EraseBrush);
void CALLBACK WAGDIFillWindowFromBrushAndhDC(HWND hWnd, HDC hDC, HBRUSH EraseBrush);
HBRUSH CALLBACK WAGDICreateColorBrush(COLORREF Color);
HFONT CALLBACK WAGDIObtainFont(CStr FontNameToObtain, long FontSizeToObtain, HWND hWnd, long Bold, long Italic);
long CALLBACK WAGDIGetFontWidth(HWND hWnd, HFONT hFont);
long CALLBACK WAGDIGetFontHeight(HWND hWnd, HFONT hFont);
void CALLBACK WAGDIDrawHorzSep(HWND hWnd, long X, long Y, long Width);
void CALLBACK WAGDIDrawVertSep(HWND hWnd, long X, long Y, long Height);
void CALLBACK WAGDIDrawLine(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor);
void CALLBACK WAGDIDrawLineXOR(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor);
long CALLBACK WAGDIDrawPixel(HWND hWnd, long X, long Y, long PixelColor);
void CALLBACK WAGDIWriteText(HDC hDC, long X, long Y, CStr Text, long TextColor, HFONT hTextFont, long TextTransparent, long BackGroundColor);
void CALLBACK WAGDIWriteClippedText(HDC hDC, long X, long Y, long Width, long Height, CStr Text, long TextColor, HFONT hTextFont, long TextTransparent, long BackGroundColor);
long CALLBACK WAGDIGetTextWidth(HWND hWnd, HFONT hFont, CStr Txt);
long CALLBACK WAGDIGetTextHeight(HWND hWnd, HFONT hFont, CStr Txt);
HDC CALLBACK WAGDICreateBackDC(LPBACKDC BackStruct);
void CALLBACK WAGDIDestroyBackDC(LPBACKDC BackStruct);
long CALLBACK WAGDIBlitBackDC(LPRECT Dimensions, LPBACKDC BackStruct);
long CALLBACK WAGDIColorCalcLuminosity(COLORREF Color, long LuminosityFactor);
HFONT CALLBACK WAGDIGetSerif(void);
HFONT CALLBACK WAGDIGetSerif10(void);
HFONT CALLBACK WAGDIGetCourierNew9(void);
long CALLBACK WAControlNextHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long CALLBACK WAControlSendMessage(HWND hWnd, long Command, long lParam);
long CALLBACK WAControlHeight(HWND hWnd);
long CALLBACK WAControlClientTop(HWND hWnd);
long CALLBACK WAControlClientLeft(HWND hWnd);
long CALLBACK WAControlClientWidth(HWND hWnd);
long CALLBACK WAControlClientHeight(HWND hWnd);
long CALLBACK WAControlBringToTop(HWND hWnd);
long CALLBACK WAControlBringToBottom(HWND hWnd);
long CALLBACK WAControlSetTopMost(HWND hWnd);
long CALLBACK WAControlRemTopMost(HWND hWnd);
void CALLBACK WAControlRefresh(HWND hWnd);
void CALLBACK WAControlRefreshLocal(HWND hWnd);
void CALLBACK WAControlRepaint(HWND hWnd);
long CALLBACK WAControlResize(HWND hControl, long CtlLeft, long CtlTop, long CtlWidth, long CtlHeight);
long CALLBACK WAControlFreeze(HWND hControl, long FreezeState);
void CALLBACK WAControlBound(HWND hParentControl, long &CtlLeft, long &CtlTop, long &CtlWidth, long &CtlHeight);
long CALLBACK WAControlEnable(HWND hWnd, long CState);
long CALLBACK WAControlVisible(HWND hWnd, long CState);
long CALLBACK WAControlIsVisible(HWND hWnd);
long CALLBACK WAControlIsEnabled(HWND hWnd);
CStr CALLBACK WAControlGetText(HWND hWnd);
long CALLBACK WAControlGetTextLen(HWND hWnd);
long CALLBACK WAControlSetText(HWND hWnd, CStr TextToSet);
long CALLBACK WAControlSetTextByAddr(HWND hWnd, long TextToSet);
long CALLBACK WAControlGetNotifiedMsg(long lParam);
HWND CALLBACK WAControlGetNotifiedhWnd(long lParam);
long CALLBACK WAControlGetNotifiedID(long lParam);
long CALLBACK WAControlTop(HWND hWnd);
long CALLBACK WAControlLeft(HWND hWnd);
long CALLBACK WAControlWidth(HWND hWnd);
HWND CALLBACK WAControlGetHwndFromPoint(void);
long CALLBACK WAControlGetNotifiedCommand(long wParam);
long CALLBACK WAControlGetXMousePos(long lParam);
long CALLBACK WAControlGetYMousePos(long lParam);
long CALLBACK WAControlSetSerif8(HWND hWnd);
HFONT CALLBACK WAControlGetFont(HWND hWnd);
HICON CALLBACK WAControlGetIcon(HWND hWnd, long IconType);
long CALLBACK WAControlGetHotKey(HWND hWnd);
long CALLBACK WAControlSetFont(HWND hWnd, HFONT hFont);
long CALLBACK WAControlSetFontNoRedraw(HWND hWnd, HFONT hFont);
long CALLBACK WAControlIsWindowChild(HWND hWnd);
CStr CALLBACK WAControlGetClassName(HWND hWnd);
long CALLBACK WAControlSetClassCursor(HWND hWnd, HCURSOR hCursor);
long CALLBACK WAControlHookWin(HWND hWnd, WNDPROC WindowProc);
long CALLBACK WAControlUnHookWin(HWND hWnd);
long CALLBACK WAControlGetWindowState(HWND hWnd);
long CALLBACK WAControlClose(HWND hWnd);
LRESULT CALLBACK WAControlCallwindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long WAComDlgChooseColor(HWND hWnd, long DefaultCol);
CStr WAComDlgGetOpenFileName(HWND  hWnd, CStr OFilters, CStr InitDir, long MultiSelect, CStr DefaultDir);
CStr WAComDlgGetSaveFileName(HWND hWnd, CStr OFilters, CStr InitDir, CStr DefaultDir);
long WAComDlgChooseFont(HWND hWnd, CStr FntName, long FntSize, long FixedOnly);
CStr WAComDlgBrowseForFolder(HWND hWnd, CStr Title);
CStr WAComDlgParseMultiFilesSelection(CStr SelectedFiles, long (CALLBACK *EnumProc)(char *, long), long SearchDirection, long UserValue);
long AllocMem(long MemSize);
void FreeMem(long MemHandle);

#endif
