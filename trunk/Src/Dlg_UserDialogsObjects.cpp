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
// Dlg_UserDialogsObjects.cpp: User dialogs objects
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Splitters.h"
#include "MiscFunctions.h"
#include "ProjectsBuilder.h"
#include "Dlg_UserDialogs.h"

// -----------------------------------------------------------------------
// Variables
CList <int *> ControlsList;
CList <LPCTRLSELECTION> SelectedControls;

// -----------------------------------------------------------------------
// Functions
LRESULT CALLBACK ObjectProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ObjectProcChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ObjectProcChildChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// Create a new control on dialog
// These controls needs special handling:
// Picture
// Animate
// Rebar
// Header (Check the resizing)

int *CreateNewControl(HWND hWnd)
{
    HWND hTool = 0;
	int *hLink = 0;
    long NbrProps = 0;
	HWND hNewObject = NULL;
	HWND hNewObjectChild = NULL;
	HWND hNewObjectChildChild = NULL;
	int Flags = 0;

    switch(DiagControlNumber)
    {
        case CONTROL_PICTURE:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreatePictureBox(0, 0, ObjectWidth, ObjectHeight, hNewObject, ImageListGetIcon(GlobalImageList4, ICON_RES_PICTURE), IMAGE_BITMAP, 0, &ObjectProcChild, WS_TABSTOP | SS_CENTERIMAGE);
            NbrProps = 10;
			break;
	    case CONTROL_LABEL:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateLabel(0, 0, ObjectWidth, ObjectHeight, hNewObject, "Static text", 0, &ObjectProc, WS_TABSTOP, 0);
            NbrProps = 10;
			break;
        case CONTROL_TEXTBOX:
            hNewObject = WACreateTextBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "EditBox", 0, &ObjectProc, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);
            NbrProps = 10;
			break;
        case CONTROL_FRAME:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateLabel(0, 0, ObjectWidth, ObjectHeight, hNewObject, "", 0, &ObjectProcChildChild, WS_TABSTOP, 0);
            hNewObjectChildChild = WACreateFrame(0, 0, ObjectWidth, ObjectHeight, hNewObjectChild, "GroupBox", 0, &ObjectProcChild, WS_TABSTOP);
            NbrProps = 10;
			break;
        case CONTROL_BUTTON:
            hNewObject = WACreateButton(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "Button", 0, 0, 0, &ObjectProc, WS_TABSTOP, 0);
            NbrProps = 10;
			break;
        case CONTROL_CHECKBOX:
            hNewObject = WACreateCheckBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "CheckBox", 0, &ObjectProc, WS_TABSTOP, 0);
            NbrProps = 10;
			break;
        case CONTROL_RADIOBUTTON:
            hNewObject = WACreateRadioButton(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "Radio Button", 0, &ObjectProc, WS_TABSTOP);
            NbrProps = 10;
			break;
        case CONTROL_COMBOBOX:
            hNewObject = WACreateComboBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "ComboBox", 0, &ObjectProc, CBS_DROPDOWNLIST | WS_TABSTOP);
            WAComboBoxAddItem(hNewObject, "ComboBox", -1);
            WAComboBoxSetIndex(hNewObject, 0);
            // Correct height
            ObjectHeight = WAControlHeight(hNewObject);
            NbrProps = 10;
			break;
        case CONTROL_LISTBOX:
            hNewObject = WACreateListBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, 0, WS_TABSTOP | WS_VSCROLL, WS_EX_CLIENTEDGE);
            WAListBoxAddItem(hNewObject, "ListBox", -1);
            NbrProps = 10;
			break;
        case CONTROL_HSCROLLBAR:
            hNewObject = WACreateScrollBar(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, WS_TABSTOP);
            NbrProps = 10;
			break;
        case CONTROL_TREEVIEW:
            hNewObject = WACreateTreeView(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, 0, &ObjectProc, WS_TABSTOP | TVS_DISABLEDRAGDROP, WS_EX_CLIENTEDGE);
            WATreeViewSetIndent(hNewObject, 1);
            WATreeViewAddItem(hNewObject, "Tree Control", TVI_ROOT, 0, 0, 0, 0, 0);
            NbrProps = 10;
			break;
        case CONTROL_LISTVIEW:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateListView(0, 0, ObjectWidth, ObjectHeight, hNewObject, 0, 0, &ObjectProcChild, 0, LVS_REPORT | WS_TABSTOP | LVS_SINGLESEL, WS_EX_CLIENTEDGE);
            WAListViewAddCol(hNewObjectChild, "List Control", ObjectWidth - 5, 0);
			WAControlHookWin(WAListViewGetSysHeader(hNewObjectChild), &ObjectProcChildChild);
			NbrProps = 10;
			break;
        case CONTROL_SYSTAB:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateSysTab(0, 0, ObjectWidth, ObjectHeight, hNewObject, 0, &ObjectProcChild, 0, WS_TABSTOP);
            WASysTabAddItem(hNewObjectChild, "Tab Control", 0, 0);
            NbrProps = 10;
			break;
        case CONTROL_PROGRESSBAR:
            hNewObject = WACreateProgressBar(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, 0, 100, WS_TABSTOP);
            ProgressBarSetRelPos(hNewObject, 100);
            NbrProps = 10;
			break;
        case CONTROL_TRACKBAR:
            hNewObject = WACreateTrackBar(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, 0, 10, WS_TABSTOP);
            WATrackBarSetTicks(hNewObject, 1);
            NbrProps = 10;
			break;
        case CONTROL_UPDOWN:
            hNewObject = WACreateUpDown(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, WS_TABSTOP);
            ObjectWidth = WAControlWidth(hNewObject);
            ObjectHeight = WAControlHeight(hNewObject);
            NbrProps = 10;
			break;
        case CONTROL_XCOMBOBOX:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateExComboBox(0, 0, ObjectWidth, ObjectHeight, hNewObject, "Extended ComboBox", 0, &ObjectProcChild, 0, CBS_DROPDOWN | WS_TABSTOP | WS_DISABLED);
            ObjectHeight = WAControlHeight(hNewObjectChild);
            MoveWindow(hNewObject, ObjectX, ObjectY, ObjectWidth, ObjectHeight, 1);
			NbrProps = 10;
			break;
        case CONTROL_TEXTIP:
            hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateIPBox(0, 0, ObjectWidth, ObjectHeight, hNewObject, 0, 0, &ObjectProc, WS_TABSTOP | WS_DISABLED, 1, IPSerif);
            MoveWindow(hNewObjectChild, 0, 0, ObjectWidth, ObjectHeight, 1);
            NbrProps = 10;
			break;
        case CONTROL_HOTKEY:
            hNewObject = WACreateFictive(ObjectX, ObjectY, 27, 27, hWnd, 0, &ObjectProc, ImageListGetIcon(GlobalImageList4, ICON_RES_HOTKEY));
            ObjectWidth = 27;
            ObjectHeight = 27;
            NbrProps = 10;
			break;
        case CONTROL_CALENDAR:
            hNewObject = WACreateMonthCalendar(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, WS_TABSTOP | MCS_DAYSTATE);
            NbrProps = 10;
			break;
        case CONTROL_DATEPICKER:
            hNewObject = WACreateDateTimePicker(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, 0, &ObjectProc, WS_TABSTOP);
            NbrProps = 10;
			break;
        case CONTROL_ANIMATION:
            hNewObject = WACreatePictureBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, ImageListGetIcon(GlobalImageList4, ICON_RES_ANIMATION), IMAGE_ICON, 0, &ObjectProc, WS_TABSTOP | SS_CENTERIMAGE);
            NbrProps = 10;
			break;
        case CONTROL_TOOLBAR:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateToolBar(0, 0, ObjectWidth, ObjectHeight, hNewObject, 0, 0, -1, &ObjectProcChild, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBS_FIXEDLENGTH | WS_TABSTOP, WS_BORDER);
            WAToolBarAddButton(hNewObjectChild, "", 0, 0, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            NbrProps = 10;
			break;
        case CONTROL_STATUSBAR:
            hNewObject = WACreateStatusBar("StatusBar", STATUSBAR_PARTSUNKEN, hWnd, 0, &ObjectProc, WS_BORDER);
            ObjectX = WAControlLeft(hNewObject);
            ObjectY = WAControlTop(hNewObject);
            ObjectWidth = WAControlWidth(hNewObject);
            ObjectHeight = WAControlHeight(hNewObject);
            NbrProps = 10;
			break;
        case CONTROL_RICHTEXT:
            hNewObject = WACreateRichTextBox(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "Rich EditBox", 0, &ObjectProc, WS_TABSTOP | ES_AUTOHSCROLL, 1);
            NbrProps = 10;
			break;
        case CONTROL_PAGER:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreatePager(0, 0, ObjectWidth, ObjectHeight, hNewObject, 0, 0, PGS_HORZ);
            NbrProps = 10;
			break;
        case CONTROL_HEADER:
            hNewObject = WACreateHeader(ObjectX, ObjectY, ObjectWidth, ObjectHeight, hWnd, "Header", 0, &ObjectProc, WS_TABSTOP | HDS_BUTTONS);
            WAHeaderAddItem(hNewObject, "Header", ObjectWidth / 2, 0, 0);
            NbrProps = 10;
			break;
        case CONTROL_REBAR:
			hNewObject = WACreateContainer(ObjectX, ObjectY, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProc);
            hNewObjectChild = WACreateRebar(0, 0, ObjectWidth, ObjectHeight, hNewObject, &ObjectProcChild, 0, WS_TABSTOP | RBS_BANDBORDERS);
			//hNewObjectChildChild = WACreateContainer(0, 0, ObjectWidth, ObjectHeight, 0, hWnd, "", &ObjectProcChildChild);
            WARebarAddBand(hNewObjectChild, 0, "Rebar", ObjectHeight, RBBS_GRIPPERALWAYS, ObjectWidth);
            WARebarBandVisible(hNewObjectChild, 0, 1);
		//	WARebarResize(hNewObjectChild);
			Flags = OBJECT_FLAG_REBAR;
	        NbrProps = 10;
			break;
        case CONTROL_TOOLTIP:
            hNewObject = WACreateFictive(ObjectX, ObjectY, 27, 27, hWnd, 0, &ObjectProc, ImageListGetIcon(GlobalImageList4, ICON_RES_TOOLTIP));
            ObjectWidth = 27;
            ObjectHeight = 27;
            NbrProps = 10;
			break;
    }
    // Add it to the linked list
    hLink = AddObjectEntry(DiagControlNumber, NbrProps, 0, 0, 0, 0, 0, 0, 0, 0, hNewObject, hNewObjectChild, hNewObjectChildChild, Flags);
	// Select it
	SelectControl(hNewObject, ObjectX, ObjectY, ObjectWidth, ObjectHeight);
	return(hLink);
}

// -----------------------------------------------------------------------
// Select control
void CaptureCtrl(HWND hWnd, int Shift)
{
	HWND Obj_Parent;
	RECT Move_Rect;
	LPRECT Stored_Move_Rect;
	POINT MovePos;
	int MoveX;
	int MoveY;
	RECT Parent_Rect;
	
	// Start moving
	SetCapture(hWnd);
	Obj_Parent = GetParent(hWnd);
	GetWindowRect(hWnd, &Move_Rect);
	ScreenRectToClient(Obj_Parent, &Move_Rect);

	UnSelectControlsList();
	SelectControl(hWnd, Move_Rect.left, Move_Rect.top, Move_Rect.right - Move_Rect.left, Move_Rect.bottom - Move_Rect.top);
	// Update the rect structure associated with this control
	Stored_Move_Rect = GetRectSelectionFromControl(hWnd);
	*GetCaptureStatusFromControl(hWnd) = TRUE;

	WAControlRepaint(hWnd);
	HDC Move_HDC = GetDC(Obj_Parent);
	DrawFocusRect(Move_HDC, &Move_Rect);
	ReleaseDC(Obj_Parent, Move_HDC);
	Stored_Move_Rect->left = Move_Rect.left;
	Stored_Move_Rect->top = Move_Rect.top;
	// Get the gap between cursor and control positions
	GetCursorPos(&MovePos);
	MoveX = MovePos.x;
	MoveY = MovePos.y;
	Parent_Rect.left = MoveX;
	Parent_Rect.top = MoveY;
	Parent_Rect.right = MoveX;
	Parent_Rect.bottom = MoveY;
	ScreenRectToClient(Obj_Parent, &Parent_Rect);
	Stored_Move_Rect->right = (Parent_Rect.left - Move_Rect.left);
	Stored_Move_Rect->bottom = (Parent_Rect.top - Move_Rect.top);
	BringWindowToTop(hWnd);
}

// -----------------------------------------------------------------------
// Move selected control
void MoveCtrl(HWND hWnd)
{
	POINT MovePos;
	HWND Obj_Parent;
	RECT Move_Rect;
	int SizeX;
	int SizeY;
	int MoveX;
	int MoveY;
	RECT Parent_Rect;
	HDC Size_HDC;
	RECT Old_Move_Rect;
	LPRECT Stored_Move_Rect;
	
	int *Capt = GetCaptureStatusFromControl(hWnd);
	if(Capt)
	{
		if(*Capt)
		{
			GetCursorPos(&MovePos);
			Obj_Parent = GetParent(hWnd);
			GetWindowRect(hWnd, &Move_Rect);
			ScreenRectToClient(Obj_Parent, &Move_Rect);
			Stored_Move_Rect = GetRectSelectionFromControl(hWnd);

			SizeX = Move_Rect.right - Move_Rect.left;
			SizeY = Move_Rect.bottom - Move_Rect.top;
			MoveX = MovePos.x;
			MoveY = MovePos.y;
			Parent_Rect.left = MoveX;
			Parent_Rect.top = MoveY;
			Parent_Rect.right = MoveX;
			Parent_Rect.bottom = MoveY;
			ScreenRectToClient(Obj_Parent, &Parent_Rect);
			MoveX = Parent_Rect.left;
			MoveY = Parent_Rect.top;
			MoveX -= Stored_Move_Rect->right;
			MoveY -= Stored_Move_Rect->bottom;

			// Bound coordinates
			GetWindowRect(Obj_Parent, &Parent_Rect);
			ScreenRectToClient(Obj_Parent, &Parent_Rect);

			if(MoveX > Parent_Rect.right) MoveX = Parent_Rect.right;
			if(MoveY > Parent_Rect.bottom) MoveY = Parent_Rect.bottom;
			if(GridAlign)
			{
				if(GridXLarg > 1) MoveX = ((MoveX / GridXLarg) * GridXLarg) + 1;
				if(GridYLong > 1) MoveY = ((MoveY / GridYLong) * GridYLong) + 1;
				// Correct the position
				MoveX--;
				MoveY--;
			}

			Size_HDC = GetDC(Obj_Parent);
			Old_Move_Rect.left = Stored_Move_Rect->left;
			Old_Move_Rect.top = Stored_Move_Rect->top;
			Old_Move_Rect.right = Stored_Move_Rect->left + SizeX;
			Old_Move_Rect.bottom = Stored_Move_Rect->top + SizeY;
			DrawFocusRect(Size_HDC, &Old_Move_Rect);
			Move_Rect.left = MoveX;
			Move_Rect.top = MoveY;
			Move_Rect.right = MoveX + SizeX;
			Move_Rect.bottom = MoveY + SizeY;
			DrawFocusRect(Size_HDC, &Move_Rect);
			ReleaseDC(Obj_Parent, Size_HDC);
			Stored_Move_Rect->left = MoveX;
			Stored_Move_Rect->top = MoveY;
		}
	}
}

// -----------------------------------------------------------------------
// End of move sequence
void ReleaseCtrl(HWND hWnd, int Cancel)
{
	POINT MovePos;
	HWND Obj_Parent;
	RECT Move_Rect;
	int SizeX;
	int SizeY;
	int MoveX;
	int MoveY;
	RECT Parent_Rect;
	HDC Size_HDC;
	RECT Old_Move_Rect;
	LPRECT Stored_Move_Rect;
	LPGRIPBOXDAT GripDat;

	int *Capt = GetCaptureStatusFromControl(hWnd);
	if(Capt)
	{
		if(*Capt)
		{
			*Capt = FALSE;
			// Done moving

			GetCursorPos(&MovePos);
			Obj_Parent = GetParent(hWnd);
			GetWindowRect(hWnd, &Move_Rect);
			ScreenRectToClient(Obj_Parent, &Move_Rect);
			Stored_Move_Rect = GetRectSelectionFromControl(hWnd);

			SizeX = Move_Rect.right - Move_Rect.left;
			SizeY = Move_Rect.bottom - Move_Rect.top;
			MoveX = MovePos.x;
			MoveY = MovePos.y;
			Parent_Rect.left = MoveX;
			Parent_Rect.top = MoveY;
			Parent_Rect.right = MoveX;
			Parent_Rect.bottom = MoveY;
			ScreenRectToClient(Obj_Parent, &Parent_Rect);
			MoveX = Parent_Rect.left;
			MoveY = Parent_Rect.top;
			MoveX -= Stored_Move_Rect->right;
			MoveY -= Stored_Move_Rect->bottom;
			GetWindowRect(Obj_Parent, &Parent_Rect);
			ScreenRectToClient(Obj_Parent, &Parent_Rect);
			if(MoveX > Parent_Rect.right) MoveX = Parent_Rect.right;
			if(MoveY > Parent_Rect.bottom) MoveY = Parent_Rect.bottom;
			if(GridAlign)
			{
				if(GridXLarg > 1) MoveX = ((MoveX / GridXLarg) * GridXLarg) + 1;
				if(GridYLong > 1) MoveY = ((MoveY / GridYLong) * GridYLong) + 1;
				// Correct the position
				MoveX--;
				MoveY--;
			}

			Size_HDC = GetDC(Obj_Parent);
			Old_Move_Rect.left = Stored_Move_Rect->left;
			Old_Move_Rect.top = Stored_Move_Rect->top;
			Old_Move_Rect.right = Stored_Move_Rect->left + SizeX;
			Old_Move_Rect.bottom = Stored_Move_Rect->top + SizeY;
			DrawFocusRect(Size_HDC, &Old_Move_Rect);
			ReleaseDC(Obj_Parent, Size_HDC);

			if(!Cancel)
			{
				Move_Rect.left = MoveX;
				Move_Rect.top = MoveY;
				Move_Rect.right = MoveX + SizeX;
				Move_Rect.bottom = MoveY + SizeY;
				ResizeGripBoxFromControl(hWnd, &Move_Rect);

				// Update the rect structure associated with this control
				GripDat = GetGripBoxFromControl(hWnd);
				BringWindowToTop(hWnd);
				BringWindowToTop(GripDat->BottomBox);
				BringWindowToTop(GripDat->BottomLeftBox);
				BringWindowToTop(GripDat->BottomRightBox);
				BringWindowToTop(GripDat->LeftBox);
				BringWindowToTop(GripDat->RightBox);
				BringWindowToTop(GripDat->TopBox);
				BringWindowToTop(GripDat->TopLeftBox);
				BringWindowToTop(GripDat->TopRightBox);
				WAControlRepaint(GripDat->BottomBox);
				WAControlRepaint(GripDat->BottomLeftBox);
				WAControlRepaint(GripDat->BottomRightBox);
				WAControlRepaint(GripDat->LeftBox);
				WAControlRepaint(GripDat->RightBox);
				WAControlRepaint(GripDat->TopBox);
				WAControlRepaint(GripDat->TopLeftBox);
				WAControlRepaint(GripDat->TopRightBox);
			}
			ReleaseCapture();
		}
	}
}

// -----------------------------------------------------------------------
// Common objects hook for childs of childs
LRESULT CALLBACK ObjectProcChildChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_SETCURSOR:
            WACursorSetNormal();
            return(TRUE);
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_NCMOUSEMOVE:
        case WM_CUT:
        case WM_COPY:
        case WM_PASTE:
        case WM_CLEAR:
        case WM_UNDO:
        case WM_MENUCHAR:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CONTEXTMENU:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_NCLBUTTONUP:
        case WM_NCRBUTTONUP:
        case WM_NCMBUTTONUP:
        case WM_COMMAND:
            return(0);
		case WM_MOUSEACTIVATE:
			return(MA_ACTIVATE);
        case WM_LBUTTONDOWN:
			CaptureCtrl(GetParent(GetParent(hWnd)), (wParam & MK_SHIFT));
			return(0);
        case WM_MOUSEMOVE:
			MoveCtrl(GetParent(GetParent(hWnd)));
            return(0);
        case WM_LBUTTONUP:
			ReleaseCtrl(GetParent(GetParent(hWnd)), FALSE);
            return(0);
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
			ReleaseCtrl(GetParent(GetParent(hWnd)), TRUE);
            return(0);
//        case WM_ERASEBKGND:
//			return(TRUE);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Common objects hook for childs
LRESULT CALLBACK ObjectProcChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
        case WM_SETCURSOR:
            WACursorSetNormal();
            return(TRUE);
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_NCMOUSEMOVE:
//        case BM_SETSTYLE:
        case WM_CUT:
        case WM_COPY:
        case WM_PASTE:
        case WM_CLEAR:
        case WM_UNDO:
        case WM_MENUCHAR:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CONTEXTMENU:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_NCLBUTTONUP:
        case WM_NCRBUTTONUP:
        case WM_NCMBUTTONUP:
        case WM_COMMAND:
            return(0);
		case WM_MOUSEACTIVATE:
			return(MA_ACTIVATE);
        case WM_LBUTTONDOWN:
			CaptureCtrl(GetParent(hWnd), (wParam & MK_SHIFT));
			return(0);
        case WM_MOUSEMOVE:
			MoveCtrl(GetParent(hWnd));
            return(0);
        case WM_LBUTTONUP:
			ReleaseCtrl(GetParent(hWnd), FALSE);
            return(0);
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
			ReleaseCtrl(GetParent(hWnd), TRUE);
            return(0);
  //      case WM_ERASEBKGND:
	//		return(TRUE);
	}
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Common objects hook
LRESULT CALLBACK ObjectProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIZE MySize;
    HWND hPager = 0;

	switch(uMsg)
	{
        case WM_SETCURSOR:
            WACursorSetNormal();
            return(TRUE);
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_NCMOUSEMOVE:
        case BM_SETSTYLE:
        case WM_CUT:
        case WM_COPY:
        case WM_PASTE:
        case WM_CLEAR:
        case WM_UNDO:
        case WM_MENUCHAR:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CONTEXTMENU:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDBLCLK:
        case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_NCLBUTTONUP:
        case WM_NCRBUTTONUP:
        case WM_NCMBUTTONUP:
        case WM_COMMAND:
			return(0);
		case WM_MOUSEACTIVATE:
			return(MA_ACTIVATE);
        case WM_LBUTTONDOWN:
			CaptureCtrl(hWnd, (wParam & MK_SHIFT));
			return(0);
        case WM_MOUSEMOVE:
			MoveCtrl(hWnd);
            return(0);
        case WM_LBUTTONUP:
			ReleaseCtrl(hWnd, FALSE);
            return(0);
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
			ReleaseCtrl(hWnd, TRUE);
            return(0);
        case WM_NOTIFY:
            switch(WAControlGetNotifiedMsg(lParam))
            {
                // Resize Pagers
                case PGN_CALCSIZE:
                    hPager = WAControlGetNotifiedhWnd(lParam);
                    MySize.cx = WAControlWidth(hPager);
                    MySize.cy = WAControlHeight(hPager);
                    WAPagerDisplaySetSize((LPNMPGCALCSIZE) lParam, &MySize);
                    return(0);
				case LVN_COLUMNCLICK:
					return(0);
			}
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Retieve the list of created menus in project resources
CStr GetMenusIDs(void)
{
    int i = 0;
    CStr ReturnValue;
    CStr BufString;
	HTREEITEM TreeViewChildEntry;

    // Construct menus list
    BufString = "(None)";

    // Save menus
    TreeViewChildEntry = WATreeViewGetFirstItemChild(hTreeView, hTreeViewMenus);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        BufString = BufString + (CStr) "|" + (CStr) GetFileNameFromTreeView(TreeViewChildEntry).Get_String();
        TreeViewChildEntry = WATreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    ReturnValue = BufString;
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add an object into the list
int *AddObjectEntry(long ObjType, long NbrProperties, long LeftR, long LeftRPos, long TopR, long TopRPos,
                    long WidthR, long WidthRPos, long HeightR, long HeightRPos, HWND hWnd, HWND hWndChild,
                    HWND hWndChildChild, int Flags)
{
    int *MemHandle = 0;
    long MemSize = 0;
    
	MemSize = (NbrProperties * (PROPERTY_LEN * 4)) + (OBJECT_LEN * 4);
    // Alloc room for object
	MemHandle = (int *) calloc(MemSize, 1);

    MemHandle[OBJECT_TYPE] = ObjType;
    // Callbacks
    MemHandle[OBJECT_DIMROUTINELEFT] = LeftR;
    MemHandle[OBJECT_DIMROUTINELEFTPOS] = LeftRPos;
    MemHandle[OBJECT_DIMROUTINETOP] = TopR;
    MemHandle[OBJECT_DIMROUTINETOPPOS] = TopRPos;
    MemHandle[OBJECT_DIMROUTINEWIDTH] = WidthR;
    MemHandle[OBJECT_DIMROUTINEWIDTHPOS] = WidthRPos;
    MemHandle[OBJECT_DIMROUTINEHEIGHT] = HeightR;
    MemHandle[OBJECT_DIMROUTINEHEIGHTPOS] = HeightRPos;
    // Extra infos
    MemHandle[OBJECT_HWND] = (long) hWnd;
    MemHandle[OBJECT_HWNDCHILD] = (long) hWndChild;
    MemHandle[OBJECT_HWNDCHILDCHILD] = (long) hWndChildChild;
    MemHandle[OBJECT_FLAGS] = Flags;
	MemHandle[OBJECT_PROPNBR] = NbrProperties;

	ControlsList.Add(MemHandle);
    return(MemHandle);
}

// -----------------------------------------------------------------------
// Store a property header
void StorePropertyHeader(long PropNumber, int *hMem, CStr PropName, long PropType, long PropFlags, long PropExtend,
                         long PropRoutine, long PropLocked, CStr PropStringStore)
{
    long PosDatas = 0;

    PosDatas = OBJECT_LEN + (PropNumber * PROPERTY_LEN);
	// Store name of the property as a new pointer
    hMem[PosDatas + PROPERTY_NAME] = (long) strdup(PropName.Get_String());
    hMem[PosDatas + PROPERTY_TYPE] = PropType;
    // Store datas of the property
	hMem[PosDatas + PROPERTY_STORAGE] = (long) strdup(PropStringStore.Get_String());
    hMem[PosDatas + PROPERTY_FLAGS] = PropFlags;
    hMem[PosDatas + PROPERTY_EXTEND] = PropExtend;
    hMem[PosDatas + PROPERTY_ROUTINE] = PropRoutine;
    hMem[PosDatas + PROPERTY_LOCKED] = PropLocked;
}

// -----------------------------------------------------------------------
// Retrieve a property informations
long GetObjectDatas(int *hMem, long Index)
{
    return(hMem[Index]);
}

// -----------------------------------------------------------------------
// Retrieve a property member
long GetPropertyDatas(int *hMem, long PropNumber, long PropIndex)
{
    long PosDatas = 0;

    PosDatas = OBJECT_LEN + (PropNumber * PROPERTY_LEN);
    return(hMem[PosDatas + PropIndex]);
}

// -----------------------------------------------------------------------
// Set a property member
void SetPropertyDatas(int *hMem, long PropNumber, long PropIndex, long Datas)
{
    long PosDatas = 0;

    PosDatas = OBJECT_LEN + (PropNumber * PROPERTY_LEN);

	switch(PropIndex)
	{
		case PROPERTY_NAME:
		case PROPERTY_STORAGE:
			// These two ones are strings
			free((void *) hMem[PosDatas + PropIndex]);
			hMem[PosDatas + PropIndex] = (long) strdup((char *) Datas);
			break;
		default:
			hMem[PosDatas + PropIndex] = Datas;
			break;
	}
}

// -----------------------------------------------------------------------
// Retrieve the index of a control from it's handle
long GetControlIndex(HWND hWnd)
{
    int *CurMem = 0;

    for(int i = 0; i < ControlsList.Amount(); i++)
    {
		CurMem = ControlsList.Get(i)->Content;
		if((HWND) CurMem[OBJECT_HWND] == hWnd) return(i);
	}
    return(-1);
}

// -----------------------------------------------------------------------
// Retrieve the datas of a control from it's handle
long GetControlDatas(HWND hWnd, int Datas_Index)
{
    int *CurMem = 0;

    for(int i = 0; i < ControlsList.Amount(); i++)
    {
		CurMem = (int *) ControlsList.Get(i)->Content;
		if((HWND) CurMem[OBJECT_HWND] == hWnd) return(CurMem[Datas_Index]);
	}
    return(0);
}

// -----------------------------------------------------------------------
// Delete a control
void EraseControl(long ControlIndex)
{
    long i = 0;
    int *CurMem = 0;

	CurMem = ControlsList.Get(ControlIndex)->Content;
	// Free the strings allocated for the property
	for(i = 0; i < CurMem[OBJECT_PROPNBR]; i++)
	{
		free((int *) CurMem[OBJECT_LEN + (i * PROPERTY_LEN) + PROPERTY_NAME]);
		free((int *) CurMem[OBJECT_LEN + (i * PROPERTY_LEN) + PROPERTY_STORAGE]);
	}
	// Erase it from the list
	ControlsList.Del(ControlIndex);
}

// -----------------------------------------------------------------------
// Delete all controls
void EraseControlsList(void)
{
    while(ControlsList.Amount() != 0)
    {
		EraseControl(0);
    }
}

// -----------------------------------------------------------------------
// Hide the gripbox of a control
void HideGripSelection(HWND hWnd)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

	for(i = 0; i < SelectedControls.Amount(); i++)
	{
		SelControl = SelectedControls.Get(i)->Content;
		if(hWnd == SelControl->hControl)
		{
			// Hide it
			GripBoxVisible(SelControl->hGrip, 0);
			break;
		}
	}
}

// -----------------------------------------------------------------------
// Show the gripbox of a control
void ShowGripSelection(HWND hWnd)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

	for(i = 0; i < SelectedControls.Amount(); i++)
	{
		SelControl = SelectedControls.Get(i)->Content;
		if(hWnd == SelControl->hControl)
		{
			// Show it
			GripBoxVisible(SelControl->hGrip, 1);
			break;
		}
	}
}

// -----------------------------------------------------------------------
// UnSelect a control
void UnSelectControl(HWND hWnd)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

	for(i = 0; i < SelectedControls.Amount(); i++)
	{
		SelControl = SelectedControls.Get(i)->Content;
		if(hWnd == SelControl->hControl)
		{
			// Remove the corresponding gripbox
			GripBoxClose(SelControl->hGrip);
			SelectedControls.Del(i);
			break;
		}
	}
}

// -----------------------------------------------------------------------
// UnSelect all controls
void UnSelectControlsList(void)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

    for(i = 0; i < SelectedControls.Amount(); i++)
    {
		SelControl = (LPCTRLSELECTION) SelectedControls.Get(i)->Content;
		GripBoxClose(SelControl->hGrip);
    }
	SelectedControls.Erase();
}

// -----------------------------------------------------------------------
// Create a control selection
void SelectControl(HWND hWnd, long Left, long Top, long Width, long Height)
{
    LPGRIPBOXDAT hGrip;
	LPCTRLSELECTION NewSelection;

	// Create the corresponding gripbox
	hGrip = WACreateGripBox(Left - 8, Top - 8, Width + 16, Height + 16, GetParent(hWnd), 0, 0x6E5920, 0);
	// Create a selection for the newly created control
	NewSelection = (LPCTRLSELECTION) calloc(sizeof(CTRLSELECTION), 1);
	NewSelection->hGrip = hGrip;
	NewSelection->hControl = hWnd;
	NewSelection->Ctrl_Rect.left = Left;
	NewSelection->Ctrl_Rect.top = Top;
	NewSelection->Ctrl_Rect.right = Width + Left;
	NewSelection->Ctrl_Rect.bottom = Height + Top;
	NewSelection->Captured = FALSE;
	SelectedControls.Add(NewSelection);
}

// -----------------------------------------------------------------------
// Resize a control
void ResizeControlFromGripBox(LPGRIPBOXDAT GripDats, LPRECT NewSizeRect)
{
	HWND Resize_hWnd;
	HWND Resize_hWndChild;
	HWND Resize_hWndChildChild;

	Resize_hWnd = GetControlFromGripBox(GripDats);
	if(Resize_hWnd)
	{
		GripBoxResize(GripDats, NewSizeRect);
		MoveWindow(Resize_hWnd, NewSizeRect->left, NewSizeRect->top, (NewSizeRect->right - NewSizeRect->left),
		           (NewSizeRect->bottom - NewSizeRect->top), 1);

		Resize_hWndChild = (HWND) GetControlDatas(Resize_hWnd, OBJECT_HWNDCHILDCHILD);
		if(Resize_hWndChild)
		{
		    MoveWindow(Resize_hWndChild, 0, 0, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
        }
		Resize_hWndChildChild = (HWND) GetControlDatas(Resize_hWnd, OBJECT_HWNDCHILD);
		if(Resize_hWndChildChild)
		{
		    MoveWindow(Resize_hWndChildChild, 0, 0,
		              (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
        }
		//if(GetControlDatas(Resize_hWnd, OBJECT_FLAGS) & OBJECT_FLAG_REBAR) WARebarResize(Resize_hWndChild);
	}
}

// -----------------------------------------------------------------------
// Resize a gripbox
void ResizeGripBoxFromControl(HWND hWnd, LPRECT NewSizeRect)
{
	LPGRIPBOXDAT Resize_hWnd;
	HWND Resize_hWndChild;
	HWND Resize_hWndChildRebar;

	Resize_hWnd = GetGripBoxFromControl(hWnd);
	if(Resize_hWnd)
	{
		Resize_hWndChild = (HWND) GetControlDatas(hWnd, OBJECT_HWNDCHILDCHILD);
		if(Resize_hWndChild) MoveWindow(Resize_hWndChild, 0, 0, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
		if(GetControlDatas(hWnd, OBJECT_FLAGS) & OBJECT_FLAG_REBAR)
		{
			Resize_hWndChildRebar = (HWND) GetControlDatas(hWnd, OBJECT_HWNDCHILD);
			MoveWindow(Resize_hWndChildRebar, 0, 0, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
			MoveWindow(hWnd, NewSizeRect->left, NewSizeRect->top, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
		}
		else
		{
			Resize_hWndChildRebar = (HWND) GetControlDatas(hWnd, OBJECT_HWNDCHILD);
			if(Resize_hWndChildRebar) MoveWindow(Resize_hWndChildRebar, 0, 0, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
			MoveWindow(hWnd, NewSizeRect->left, NewSizeRect->top, (NewSizeRect->right - NewSizeRect->left), (NewSizeRect->bottom - NewSizeRect->top), 1);
		}
		GripBoxResize(Resize_hWnd, NewSizeRect);
	}
}

// -----------------------------------------------------------------------
// Retrieve the control associated with a gripbox
HWND GetControlFromGripBox(LPGRIPBOXDAT hGrip)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

    for(i = 0; i < SelectedControls.Amount(); i++)
    {
		SelControl = SelectedControls.Get(i)->Content;
		if(hGrip == SelControl->hGrip) return(SelControl->hControl);
    }
	return(NULL);
}

// -----------------------------------------------------------------------
// Retrieve the gripbox associated with a control
LPGRIPBOXDAT GetGripBoxFromControl(HWND hControl)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

    for(i = 0; i < SelectedControls.Amount(); i++)
    {
		SelControl = SelectedControls.Get(i)->Content;
		if(hControl == SelControl->hControl) return(SelControl->hGrip);
    }
	return(NULL);
}

// -----------------------------------------------------------------------
// Retrieve the rect associated with a control
LPRECT GetRectSelectionFromControl(HWND hControl)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

    for(i = 0; i < SelectedControls.Amount(); i++)
    {
		SelControl = SelectedControls.Get(i)->Content;
		if(hControl == SelControl->hControl) return(&SelControl->Ctrl_Rect);
    }
	return(NULL);
}

// -----------------------------------------------------------------------
// Retrieve the capture status of a control
int *GetCaptureStatusFromControl(HWND hControl)
{
    long i = 0;
	LPCTRLSELECTION SelControl;

    for(i = 0; i < SelectedControls.Amount(); i++)
    {
		SelControl = (LPCTRLSELECTION) SelectedControls.Get(i)->Content;
		if(hControl == SelControl->hControl) return(&SelControl->Captured);
    }
	return(NULL);
}
