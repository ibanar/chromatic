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
// Dlg_ImageView.h: declaration of used variables / structures for Dlg_ImageView.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_IMAGEVIEW_H_
#define _DLG_IMAGEVIEW_H_

// -----------------------------------------------------------------------
// Constants
#define VIEW_ICON 0
#define VIEW_CURSOR 1
#define VIEW_BITMAP 2

// -----------------------------------------------------------------------
// Structures
typedef struct _IMGVIEW
{
    WNDPROC Proc;
    HANDLE hBitmap;
    long ImType;
    long OldX;
    long OldY;
    HIMAGELIST CurrentImage;
} IMGVIEW, *LPIMGVIEW;

// -----------------------------------------------------------------------
// Variables
extern long DisplayType;
extern CStr FileToDisplay;
extern IMGVIEW ViewStruct;
extern HANDLE ImgHandle;
extern long NewViewMem;
extern HIMAGELIST hImgList;
extern long Patched;

// -----------------------------------------------------------------------
// Functions declarations
void DisplayImageView(long ImageType, CStr ImageFileName);
void CALLBACK FrmImageViewInitProc(HWND hwnd);
LRESULT CALLBACK FrmImageViewWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadViewStruct(HWND hwnd);
void StoreViewStruct(HWND hwnd);
void ScrollPicHorz(HWND hwnd, long Amount, long Absolute);
void ScrollPicVert(HWND hwnd, long Amount, long Absolute);
void ResizeImgView(HWND hwnd);

#endif