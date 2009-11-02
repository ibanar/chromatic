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
// Scripts.cpp: Scripts interpreter.
// -----------------------------------------------------------------------
// TODO: Convert everything and try to make addins
//       (will need some modif of the addins manager)
//       and wizards to run.

// -----------------------------------------------------------------------
// Compiler directives
#pragma warning(disable:4530) // Can use exceptions handler without warning

// -----------------------------------------------------------------------
// Includes
#include "Scripts.h"

#include "Objects_Handler.h"
#include "Constants.h"

#include "Classes/ImageList.h"
#include "Classes/BackDC.h"
#include "Classes/GripBoxDat.h"
#include "Classes/SplitterDat.h"
#include "Classes/MiniChildDat.h"
#include "Classes/AddInContextDat.h"
#include "Classes/DockingBox.h"
#include "Classes/Screen.h"
#include "Classes/Rect.h"
#include "Classes/FileTime.h"
#include "Classes/Now.h"
#include "Classes/CommonDialog.h"
#include "Classes/Button.h"

// -----------------------------------------------------------------------
// Variables
extern luaL_reg Chromatic_Methods[];

void *Classes_Stack[1024];
int Stack_Pos;

// -----------------------------------------------------------------------
// Private functions
void InsertPILLWALib(long CodeContext);
long CALLBACK Script_WACreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, char *DTitle, DLGPROC WinProc, long Centered);
HWND CALLBACK Script_WACreateDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType);
HWND CALLBACK Script_WACreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass);
HWND CALLBACK Script_WACreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType);
HWND CALLBACK Script_WACreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType);
void Script_FreeResources(lua_State *L);
void CALLBACK WAMMEnumKillWindows(void);
int CALLBACK EnumKill(HWND hWnd, long lParam);
HGLOBAL CALLBACK PILL_WAMiscAllocMem(long Amount);
long CALLBACK PILL_WAMiscFreeMem(void *MemHandle);

// -----------------------------------------------------------------------
// Scripts debugging function
/*void ScriptDebug(lua_State *C, lua_Debug *ar);
{

}*/

// -----------------------------------------------------------------------
// Init the running context for scripts
int AllocScriptEngine(void)
{

    return(TRUE);
}

// -----------------------------------------------------------------------
// Release the resources of the scripts engine
void ReleaseScriptEngine(void)
{

}

// -----------------------------------------------------------------------
// Run a script as string
long InsertScriptCode(char *ScriptName, char *SnippetVal)
{
    int i;
    lua_State *L;
    CList <HWND> Script_Windows;
    CList <HMODULE> Script_Libraries;

    Script_Windows.Erase();
    Script_Libraries.Erase();

	L = lua_open();
    if(L)
    {
        L->Windows = (void *) &Script_Windows;
        L->Libraries = (void *) &Script_Libraries;
        L->In_Dialog = 0;
        L->Obj_Stack = (void **) malloc(OBJ_STACK_SIZE);
        L->Script_Name = ScriptName;
        memset(L->Obj_Stack, 0, OBJ_STACK_SIZE);
        L->Obj_Stack_Pos = 0;
        luaL_openlibs(L);
        luaL_openlib(L, "ChromaticLib", Chromatic_Methods, 0);

        // Declare the constants
        i = 0;
        while(Script_Constants[i].Name)
        {
	        lua_pushstring(L, Script_Constants[i].Name);
	        lua_pushinteger(L, Script_Constants[i].Value);
	        lua_rawset(L, -3);
            i++;
        }

        ImageList::Install(L);
        BackDC::Install(L);
        GripBoxDat::Install(L);
        SplitterDat::Install(L);
        MiniChildDat::Install(L);
        AddInContextDat::Install(L);
        DockingBox::Install(L);
        Screen::Install(L);
        Rect::Install(L);
        FileTime::Install(L);
        Now::Install(L);
        CommonDialog::Install(L);
        Button::Install(L);
  
  	    L->In_Dialog = 0;

        // Run the script now
        int Error = luaL_dostring(L, SnippetVal);

        Script_FreeResources(L);

        // Print any error if necessary
        if(Error) PrintScriptError(L);

	    lua_close(L);
    }        
	return(1);
}

// -----------------------------------------------------------------------
// Run a script file
long RunScriptFile(CStr FileName, CStr Arguments)
{
//	long ReturnCode;
//	PILL_Error CodeError;
    /*int i = 0;
    int Args_Str_Len = Arguments.Len();
    int Nbr_Args = 1;
    int Cur_Arg = 0;
  //  char *Start_Arg;
    CStr Arg_Nbr_Str;*/

//	ScriptContext = InitLUAContext();
  //  if(ScriptContext) {
    /*	InsertPILLWALib(CodeContext);
    
        char *Args = (char *) malloc(Args_Str_Len + 1);
        if(Args) {
            memset(Args, 0, Args_Str_Len + 1);
            memcpy(Args, Arguments.Get_String(), Args_Str_Len);
            Start_Arg = Args;
            if(Arguments.In_Str(1, Arguments.Chr(2))) {
                while(Args[i]) {
                    if(Args[i] == 2) {
                        Nbr_Args++;
                    }
                    i++;
                }
                i = 0;
	            PILL_Add_Variable(CodeContext, "ARGV", PILL_VAR_STRING, (long) "", Nbr_Args },
                while(Args[i]) {
                    if(Args[i] == 2) {
                        Args[i] = 0;
	                    PILL_Set_Variable_Value(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Cur_Arg);
                        Start_Arg = &Args[i] + 1;
                        Cur_Arg++;
                    }
                    i++;
                }
	            PILL_Set_Variable_Value(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Cur_Arg);
            } else {
	            PILL_Add_Variable(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Nbr_Args },
            }
        } else {
            Arg_Nbr_Str = "ARGV" + (CStr) (Nbr_Args - 1);
	        PILL_Add_Constant(CodeContext, Arg_Nbr_Str.Get_String(), PILL_VAR_STRING, (long) Arguments.Get_String() },
            WriteToStatus("Couldn't allocate room for additional arguments");
        }
	    PILL_Add_Constant(CodeContext, "ARGC", Nbr_Args },

	    ReturnCode = PILL_Run_Script(CodeContext, FileName.Get_String());
	    if(ReturnCode > 0 && ReturnCode < PILL_ERROR_USER_DEFINED) {
		    if(ReturnCode != PILL_ERROR_GHOST) {
			    PILL_Ask_Error_Details(CodeContext, &CodeError);
			    WriteToStatus(FileName + (CStr) ":" + (CStr) CodeError.Line + (CStr) ": error: " + (CStr) CodeError.Description);
		    }
		    PILL_FreeResources(CodeContext);
		    return(FALSE);
	    }*/
//        DestroyLUAContext(ScriptContext);
  //  }
/*
	PILL_FreeResources(CodeContext);
    if(Args) free(Args);*/
    return(1);

//	return(ReturnCode);
}

// -----------------------------------------------------------------------
// Output an error mesage into the output box
void PrintScriptError(lua_State *L)
{
    char *Err_String;
    CStr Output_String;
    int Pos_CBracket;

    Err_String = (char *) lua_tostring(L, -1);
    if(Err_String == NULL) Err_String = "unrecognized LUA error";    
    Output_String = Err_String;
    Pos_CBracket = Output_String.In_Str(1, "]");
    Output_String = &Err_String[Pos_CBracket];
    if(Output_String.Left(1) != (CStr) ":") Output_String = ": " + Output_String;
    Output_String = (CStr) L->Script_Name + Output_String;
    WriteToStatus(Output_String);
}

// -----------------------------------------------------------------------
// Remove the resources allocated by a script
void Script_FreeResources(lua_State *L)
{
	int i;
	HWND Act_Window;
	CStr Act_WinClass;
    CList <HWND> *Win = (CList <HWND> *) L->Windows;
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

    if(L->Obj_Stack) free(L->Obj_Stack);

	// Free dialogs
	for(i = 0; i < Win->Amount(); i++)
	{
		DestroyWindow(Win->Get(i)->Content);
	}

	// Free libraries
	for(i = 0; i < Lib->Amount(); i++)
	{
		FreeLibrary(Lib->Get(i)->Content);
	}

	// Zombie dialog ?
	try
	{
		if(L->In_Dialog == 1)
		{
			Act_Window = GetActiveWindow();
			Act_WinClass = WAControlGetClassName(Act_Window);
			if(strcmp(Act_WinClass.Left(2).Upper_Case().Get_String(), "WA") != 0)
			{
				WAControlClose(Act_Window);
			}
			else
			{
				// Means something went wrong during WM_INITDIALOG
				EndDialog(LastDialog, 0);
				LastDialog = 0;
			}
		}
	}
	catch(...)
	{
	}
}

// -----------------------------------------------------------------------
// Kill the remaining dialog
// (i can't remember what was the purpose of that one)
void CALLBACK WAMMEnumKillWindows(void)
{
	EnumChildWindows(hMDIform.hWnd, &EnumKill, 0);
}
                    
int CALLBACK EnumKill(HWND hWnd, long lParam)
{
	if(WAControlIsEnabled(hWnd))
	{
		DestroyWindow(hWnd);	
		return(0);
	}
    return(1);
}

// -----------------------------------------------------------------------
// Functions available for the scripts
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Special version of these routines for scripts
// (mainly for resources tracking because we can't let a dialog around
//  if the script contains errors or the interpreter crashes)
int Script_WACreateStockModalDialog(lua_State *L)
{
	int TemplateNumber = luaL_checkint(L, 1);
	int DLeft = luaL_checkint(L, 2);
	int DTop = luaL_checkint(L, 3);
	int DWidth = luaL_checkint(L, 4);
	int DHeight = luaL_checkint(L, 5);
	HWND hParent = (HWND) luaL_checkint(L, 6);
	char *DTitle = (char *) luaL_checkstring(L, 7);
	DLGPROC WinProc = (DLGPROC) luaL_checkint(L, 8);
	int Centered = luaL_checkint(L, 9);

    long ReturnValue;

	// We need to freeze our internal timer
    FreezeTimer = 1;
    L->In_Dialog = 1;

	switch(TemplateNumber)
    {
		case MODALDLG_STOCK_WIZARD:
			ReturnValue = WACreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, (long) MAKEINTRESOURCE(MBMP_BASE + MBMP_WIZARD));
			break;
		default:
			ReturnValue = WACreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, 0);
			break;
	}
    FreezeTimer = 0;
    L->In_Dialog = 0;

	lua_pushnumber(L, ReturnValue);
	return(1);
}

// -----------------------------------------------------------------------
int Script_MsgBox(lua_State *L)
{
    int ReturnValue;

	HWND hParent = (HWND) luaL_checkint(L, 1);
	char *MBText = (char *) luaL_checkstring(L, 2);
	char *MBTitle = (char *) luaL_checkstring(L, 3);
	int MBType = luaL_checkint(L, 4);

    ReturnValue = MessageBox(hParent, MBText, MBTitle, MBType);
	lua_pushnumber(L, ReturnValue);
	return(1);
}

// -----------------------------------------------------------------------
int Script_ObtainExtendedFunctions(lua_State *L)
{
    luaL_error(L, "reserved for future use");
    return(1);
}

// -----------------------------------------------------------------------
int Script_InsertText(lua_State *L)
{
    HWND hForm;
    if(NbForms == 0) return(1);

    hForm = (HWND) luaL_checkint(L, 1);
    if(hForm != 0) InsertText(hForm, (char *) luaL_checkstring(L, 2));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StringHex32(lua_State *L)
{
    char *Return_Value = Wrapper_StringHex32(luaL_checkint(L, 1));

    lua_pushstring(L, Return_Value);
    return(1);
}

// -----------------------------------------------------------------------
int Script_StringCat(lua_State *L)
{
    char *Return_Value = Wrapper_StringCat((char *) luaL_checkstring(L, 1),
                                             (char *) luaL_checkstring(L, 2));
    lua_pushstring(L, Return_Value);
    return(1);
}

// -----------------------------------------------------------------------
int Script_LoadLibrary(lua_State *L)
{
	HMODULE NewLibrary;
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

	NewLibrary = LoadLibrary(luaL_checkstring(L, 1));
	if(NewLibrary) Lib->Add(NewLibrary);

    lua_pushinteger(L, (int) NewLibrary);
	return(1);
}

// -----------------------------------------------------------------------
int Script_FreeLibrary(lua_State *L)
{
	int i;
	HMODULE Library_Base = (HMODULE) luaL_checkint(L, 1);
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

	// Free the entry in the list
	for(i = 0; i < Lib->Amount(); i++)
	{
		if(Lib->Get(i)->Content == Library_Base)
		{
			Lib->Del(i);
			break;
		}
	}

    lua_pushboolean(L, FreeLibrary(Library_Base));
	return(1);
}

// -----------------------------------------------------------------------
int Script_DialogXUnitToPixel(lua_State *L)
{
    lua_pushinteger(L, DialogXUnitToPixel(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogYUnitToPixel(lua_State *L)
{
    lua_pushinteger(L, DialogYUnitToPixel(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogXPixelToUnit(lua_State *L)
{
    lua_pushinteger(L, DialogXPixelToUnit(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogYPixelToUnit(lua_State *L)
{
    lua_pushinteger(L, DialogYPixelToUnit(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogGetXUnit(lua_State *L)
{
    lua_pushinteger(L, DialogGetXUnit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogGetYUnit(lua_State *L)
{
    lua_pushinteger(L, DialogGetYUnit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogSetIcon(lua_State *L)
{
    lua_pushinteger(L, DialogSetIcon((HWND) luaL_checkint(L, 1),
                                     (HICON) luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_FrameRefreshToolbar(lua_State *L)
{
    lua_pushinteger(L, FrameRefreshToolbar((HWND) luaL_checkint(L, 1)));
    return(1);
}


// -----------------------------------------------------------------------
int Script_WinsockInit(lua_State *L)
{
    lua_pushinteger(L, (long) WinsockInit((LPWSADATA) luaL_checkint(L, 1),
                                          (long) luaL_checkint(L, 2)
                                         ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_WinsockUnInit(lua_State *L)
{
    lua_pushinteger(L, WinsockUnInit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_ProgressBarSetRange(lua_State *L)
{
    lua_pushinteger(L, ProgressBarSetRange((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2),
                                           (long) luaL_checkint(L, 3)
                                           ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ProgressBarSetRelPos(lua_State *L)
{
    lua_pushinteger(L, ProgressBarSetRelPos((HWND) luaL_checkint(L, 1),
                                            (long) luaL_checkint(L, 2)
                                            ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_CheckBoxGetState(lua_State *L)
{
    lua_pushinteger(L, CheckBoxGetState((HWND) luaL_checkint(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_CheckBoxSetState(lua_State *L)
{
    lua_pushinteger(L, CheckBoxSetState((HWND) luaL_checkint(L, 1),
                                        (long) luaL_checkint(L, 2)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ColorBoxGetColor(lua_State *L)
{
    lua_pushinteger(L, ColorBoxGetColor((HWND) luaL_checkint(L, 1)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ColorBoxSetColor(lua_State *L)
{
    lua_pushinteger(L, ColorBoxSetColor((HWND) luaL_checkint(L, 1),
                                        (long) luaL_checkint(L, 2)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarSetParts(lua_State *L)
{
    long Array[512];
    int i;
    int Top;
    HWND Handle = (HWND) luaL_checkint(L, 1);
    int Parts = luaL_checkint(L, 2);

    if(Parts > 511) Parts = 511;
    luaL_checktype(L, 3, LUA_TTABLE);
    for(i = 1; i <= Parts; i++)
    {
        lua_rawgeti(L, 3, i);
        Top = lua_gettop(L);
        Array[i - 1] = lua_tointeger(L, Top);
        lua_pop(L, 1);
    }

    lua_pushinteger(L, StatusBarSetParts(Handle, Parts, Array));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarSetText(lua_State *L)
{
    lua_pushinteger(L, Wrapper_StatusBarSetText((HWND) luaL_checkint(L, 1),
                                                (long) luaL_checkint(L, 2),
                                                (long) luaL_checkint(L, 3),
                                                (char *) luaL_checkstring(L, 4),
                                                (long) luaL_checkint(L, 5)
                                                ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarGetYSize(lua_State *L)
{
    lua_pushinteger(L, StatusBarGetYSize((HWND) luaL_checkint(L, 1)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_LabelGetImage(lua_State *L)
{
    lua_pushinteger(L, (int) LabelGetImage((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_PictureBoxChangeIcon(lua_State *L)
{
    lua_pushinteger(L, PictureBoxChangeIcon((HWND) luaL_checkint(L, 1),
                                            (HICON) luaL_checkint(L, 2)
                                            ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_PictureBoxChangeImage(lua_State *L)
{
    lua_pushinteger(L, PictureBoxChangeImage((HWND) luaL_checkint(L, 1),
                                             (HANDLE) luaL_checkint(L, 2)
                                             ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_RadioButtonGetState(lua_State *L)
{
    lua_pushinteger(L, RadioButtonGetState((HWND) luaL_checkint(L, 1)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_RadioButtonSetState(lua_State *L)
{
    lua_pushinteger(L, RadioButtonSetState((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------

/*HWND CALLBACK PILL_WACreateDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType) {
	HWND NewhWnd;
    
	NewhWnd = WACreateDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WADTitle), WAInitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType);
	PILL_Windows.Add(NewhWnd);
	return(NewhWnd);
}

HWND CALLBACK PILL_WACreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass) {
	HWND NewhWnd;

    NewhWnd = WACreateMDIChildDialog(WAHParent, StringConvertToCStr(WACTitle), WAhIcon, WAInitProc, WAWinProc, WAExtraStyle, WAZeroPos, StringConvertToCStr(WACustomClass));
	PILL_Windows.Add(NewhWnd);
	return(NewhWnd);
}

HWND CALLBACK PILL_WACreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType) {
	HWND NewhWnd;

    NewhWnd = WACreateMDIDialog(WAMDILeft, WAMDITop, WAMDIWidth, WAMDIHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WAMDITitle), WAInitProc, WAWinProc, WAExtraStyle, WAShowType);
	PILL_Windows.Add(NewhWnd);
	return(NewhWnd);
}

HWND CALLBACK PILL_WACreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType) {
	HWND NewhWnd;

    NewhWnd = WACreateSplashDialog(WADLeft, WADTop, WADWidth, WADHeight, StringConvertToCStr(WADTitle), WAHParent, WAhMenu, WAhIcon, WAInitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType);
	PILL_Windows.Add(NewhWnd);
	return(NewhWnd);
}
*/

// -----------------------------------------------------------------------
// Static methods definitions for Chromatic table
luaL_reg Chromatic_Methods[] =
{
    { "CreateStockModalDialog", Script_WACreateStockModalDialog },

    { "MsgBox", Script_MsgBox },
    { "ObtainExtendedFunctions", Script_ObtainExtendedFunctions },

    // ---------------

    { "InsertText", Script_InsertText },

    { "StringHex32", Script_StringHex32 },
    { "StringCat", Script_StringCat },
    { "LoadLibrary", Script_LoadLibrary },
    { "FreeLibrary", Script_FreeLibrary },
    
    { "DialogXUnitToPixel", Script_DialogXUnitToPixel },
    { "DialogYUnitToPixel", Script_DialogYUnitToPixel },
    { "DialogXPixelToUnit", Script_DialogXPixelToUnit },
    { "DialogYPixelToUnit", Script_DialogYPixelToUnit },
    { "DialogGetXUnit", Script_DialogGetXUnit },
    { "DialogGetYUnit", Script_DialogGetYUnit },
    { "DialogSetIcon", Script_DialogSetIcon },

    { "FrameRefreshToolbar", Script_FrameRefreshToolbar },

    { "WinsockInit", Script_WinsockInit },
    { "WinsockUnInit", Script_WinsockUnInit },
/*	FillPILLConstant(CodeContext, "SocketCreate", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketDestroy", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketConnect", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketGetPort", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketListen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketAcceptConnection", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketSendASCIIDatasCR", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketSendUnknownDatas", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketGetRevcLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketReceiveASCIIDatas", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "SocketReceiveUnknownDatas", i, PILL_STORE_ENFORCE, 1);
*/

    { "ProgressBarSetRange", Script_ProgressBarSetRange },
    { "ProgressBarSetRelPos", Script_ProgressBarSetRelPos },

    { "CheckBoxGetState", Script_CheckBoxGetState },
    { "CheckBoxSetState", Script_CheckBoxSetState },

    { "ColorBoxGetColor", Script_ColorBoxGetColor },
    { "ColorBoxSetColor", Script_ColorBoxSetColor },

    { "StatusBarSetParts", Script_StatusBarSetParts },
    { "StatusBarSetText", Script_StatusBarSetText },
    { "StatusBarGetYSize", Script_StatusBarGetYSize },

    { "LabelGetImage", Script_LabelGetImage },

    { "PictureBoxChangeIcon", Script_PictureBoxChangeIcon },
    { "PictureBoxChangeImage", Script_PictureBoxChangeImage },

    { "RadioButtonGetState", Script_RadioButtonGetState },
    { "RadioButtonSetState", Script_RadioButtonSetState },

/*    { "GotoTop", Script_GotoTop },
    { "GotoBottom", Script_GotoBottom },
    { "GotoLine", Script_GotoLine },
    { "GotoMatchingBracket", Script_GotoMatchingBracket },
    { "GotoPrevWord", Script_GotoPrevWord },
    { "GotoNextWord", Script_GotoNextWord },
    { "GotoPrevLetter", Script_GotoPrevLetter },
    { "GotoNextLetter", Script_GotoNextLetter },
    { "GotoPrevLine", Script_GotoPrevLine },
    { "GotoNextLine", Script_GotoNextLine },
    { "GotoPrevPage", Script_GotoPrevPage },
    { "GotoNextPage", Script_GotoNextPage },
    { "GotoFileLine", Script_GotoFileLine },
*/
    { NULL, NULL }
};

// -----------------------------------------------------------------------
// Register the WALib into PILL
/*void InsertPILLWALib(long CodeContext) {
	// Necessary
	PILL_Add_Variable(CodeContext, "EndDialog", (long) &EndDialog, 0 },
	PILL_Add_Variable(CodeContext, "GetDlgItem", (long) &GetDlgItem, 0 },
	PILL_Add_Variable(CodeContext, "GetProcAddress", (long) &GetProcAddress, 0 },

	FillPILLConstant(CodeContext, "WAMiscClipBoardCopyText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscClipBoardPasteText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscClipBoardIsEmpty", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscShellAddFileToRecents", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscWaitEvents", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscSendBroadCastMsg", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscDoEvents", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscObtainGUID", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscAllocMem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscFreeMem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscCopyMemLoop", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMiscCopyMemWithOffsets", i, PILL_STORE_ENFORCE, 1);


	FillPILLConstant(CodeContext, "StringIsLabel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsHexaDecimal", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsDecimal", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsLabelChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsDigitChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsHexaChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsAlphaChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsBlankChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsOpenBracketChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsUnderLineChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsDotChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsCommaChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsQuoteChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsArobasChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsEOL", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringNumberComplement", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringHexNumberComplement", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringBinNumberComplement", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringReplace", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringHex16", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringHex8", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringSplit", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringGetSplitUBound", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringGetSplitElement", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringReleaseSplit", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringJoin", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFree", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringDecToString", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringStringToDec", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringCountLines", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringGetLinesOffsets", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFilterNonPrintableChars", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringLeft", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringRight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringTrim", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringLTrim", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringRTrim", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringInStr", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringMid", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringUpperCase", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringLowerCase", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringReverse", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringSpace", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringString", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringIsNumeric", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFillHexBytes", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFillHexWords", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFillHexDWords", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "StringFileNameToLabel", i, PILL_STORE_ENFORCE, 1);


	i = 0;
	FillPILLConstant(CodeContext, "WACreateSplashDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateModalDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateSplitter", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateMDIDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateMDIChildDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateToolBar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateImageList", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateSysTab", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateTrackBar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateUpDown", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateProgressBar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateStatusBar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateColorBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateGripBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateLabel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreatePictureBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateFrame", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateIPBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateRichTextBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateHexBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateCheckBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateRebar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreatePager", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateClient", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateRadioButton", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateListView", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateTreeView", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateComboBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateListBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateScrollBar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateDumpBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACreateTextBox", i, PILL_STORE_ENFORCE, 1);
	
    FillPILLConstant(CodeContext, "WAToolBarAddButton", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButton", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonXSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonYSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetRowsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetImagelist", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarAddSeparator", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetNotifiedToolTip", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetNotifiedDropDownItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarDisplayToolTip", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarResize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarSetButtonPressed", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarSetButtonChecked", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarSetButtonEnabled", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarIsButtonPressed", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarIsButtonChecked", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarDisplayPopupMenu", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetXSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetYSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonXPosition", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetXYSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetRealPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonXYPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetButtonIndexXSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetXPadding", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetYPadding", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarGetNotifiedHotItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAToolBarRemoveButton", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WASysTabAddItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabGetImagelist", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetCurrentItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetFocusItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabHighLightItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabItemsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabGetRowsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetSeparators", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetItemsSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabRemoveItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetMinWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WASysTabSetPadding", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WATrackBarSetRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATrackBarSetPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATrackBarGetPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATrackBarSetTicks", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATrackBarGetToolTips", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "GripBoxGetColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "GripBoxSetColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "GripBoxClose", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "GripBoxVisible", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "GripBoxResize", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "IPBoxClearIP", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "IPBoxGetIP", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "IPBoxSetIP", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "IPBoxSetFocus", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "IPBoxSetRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "IPBoxIsBlank", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WATextBoxGetLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxSetMaxLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetMaxLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetTextHandle", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxAddText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxSelText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxRemoveSel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxSetCaretPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxLinesCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetMarginWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetFirstVisibleLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxIsModified", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetCurrentOptions", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetPasswordChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxSetPasswordChar", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetSelection", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetScrollPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxGetBreakProcAddress", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxCanPaste", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATextBoxCanUndo", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WARebarAddBand", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarRemoveBand", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetXSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetYSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarResize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarSetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarSetTextColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarBandVisible", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetBandsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetBackGroundColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetRowsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetToolTips", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetTextColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetPalette", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARebarGetImageList", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAPagerDisplaySetSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerMaximizeX", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerMaximizeY", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetNotifiedChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerSetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetBorderSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetButtonSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetScrollPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerGetOrientation", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerSetOrientation", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAPagerChangeOrientation", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAClientResize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientGetActiveChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientSetNextChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientSetPreviousChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientTileHorizontal", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientTileVertical", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientTileCascade", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAClientTileArrangeIconic", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAListViewAddCol", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetColWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetColWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetScroll", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewClear", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewDetachImageList", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewAddItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewDeleteItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetSubItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetSubItemImage", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetSubItemImage", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewEditValidated", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetEditResult", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewItemCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSelItemCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewMultiSelFirstItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetItemSelState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetEditModeOn", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetItemSel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetItemHighlight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetItemFocus", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetItemUnderCursor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetFirstFocusItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetSelItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetSelItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetSelItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewCheckBoxItemDoubleClick", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetItemIcon", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetNotifiedColumnIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSort", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewReOrder", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetHeaderLabel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetHeaderPosition", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetHeaderPosition", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetTextBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetTextColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetToolTips", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetTopIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetCountPerPage", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetEditControl", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetExStyle", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetSysHeader", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetHotCursor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetImageList", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewSetItemCheckbox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewGetItemCheckbox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewIsCheckboxNotify", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewFindItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewFindSubItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewBeginDrag", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewMoveDrag", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewEndDrag", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListViewPasteAutoEdit", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WATreeViewAddItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetToolTips", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetToolTips", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetIndent", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewRemoveItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetExpandingState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetExpandingItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetDeletedItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetItemIcon", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetItemExpandedState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemExpandedState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetSingleExpandState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetSelectedItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemFromPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetSelectedItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSetItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemParent", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetRoot", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetVisibleCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetFirstItemChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetNextItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetPreviousItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSearchItemText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewSearchChildPartialText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetChildItemsCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetSelChildItemPos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemLevel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetBackColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetEditControl", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetImageList", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetIndent", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetInsertMarkColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetLineColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetTextColor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetScrollTime", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemTextRect", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WATreeViewGetItemRect", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAComboBoxAddItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxDeleteItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxReset", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxSetMaxLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxGetIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxSetIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxGetItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxItemExist", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxGetItemData", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxSaveInIniFile", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAComboBoxFillFromIniFile", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAListBoxAddItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxReset", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSetHorzScrollWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSetColumnsWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxCount", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxGetTopIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSetTopIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSetItemData", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxGetItemData", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxIsItemSelected", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxGetSelItemIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSetIndex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxGetItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxDeleteItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxReplaceItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxReplaceSelItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxProcessDrag", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSelItemDown", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSelItemUp", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxSelItemRemove", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxItemExist", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAListBoxItemUnderCursor", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAScrollBarGetMinRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAScrollBarGetMaxRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAScrollBarGetPageRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAScrollBarSetPageRange", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAScrollBarShowHide", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAScrollBarSetMinMaxRange", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WADumpBoxResize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WADumpBoxScrollDown", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WADumpBoxScrollUp", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WADumpBoxGetVisibleLines", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WADumpBoxGetVisibleColumns", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMenuGetString", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMenuEnable", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMenuSetDefaultItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMenuSetItemState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMenuSetItemType", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WACursorSetWait", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACursorSetNo", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACursorSetCross", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACursorSetNormal", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACursorDisable", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WACursorEnable", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAIniReadKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAIniReadBoolKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAIniWriteSection", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAIniDeleteKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAIniWriteKey", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAFileGetWriteTime", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetCreationTime", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetAccessedTime", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetExtension", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileRemoveExtension", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileReplaceExtension", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetDirectory", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetFileName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileOpenR", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileOpenW", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileOpenWAppend", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileClose", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileCreateEmpty", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileWriteBuffer", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileWriteLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileIsDirectory", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileGetSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileExist", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileIsReadOnly", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileIsUnix", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileLoadIntoMem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileSaveFromMem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFileDir", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WARegistryUpdateKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARegistryDeleteKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WARegistryGetKeyValue", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAGDIFillWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIFillWindowFromBrush", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIFillWindowFromBrushAndhDC", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDICreateColorBrush", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIObtainFont", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDrawHorzSep", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDrawVertSep", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDrawLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDrawLineXOR", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDrawPixel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIWriteText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIWriteClippedText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetTextWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetTextHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetFontWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetFontHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDICreateBackDC", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIDestroyBackDC", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIBlitBackDC", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIColorCalcLuminosity", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetSerif", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetSerif10", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAGDIGetCourierNew9", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAControlSendMessage", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlClientTop", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlClientLeft", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlClientWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlClientHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlBringToTop", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlBringToBottom", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlSetTopMost", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlRemTopMost", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlRefresh", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlRefreshLocal", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlRepaint", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlResize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlFreeze", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlBound", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlEnable", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlVisible", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlIsVisible", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlIsEnabled", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetTextLen", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlSetText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetNotifiedMsg", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetNotifiedhWnd", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetNotifiedID", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlTop", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlLeft", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlWidth", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlHeight", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetHwndFromPoint", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetNotifiedCommand", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetXMousePos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetYMousePos", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetFont", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetIcon", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetHotKey", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlSetFont", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlSetFontNoRedraw", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlIsWindowChild", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetClassName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlSetClassCursor", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlGetWindowState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAControlClose", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAFTPInitiatePort", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFTPGetEntryDate", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFTPGetEntryFileName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFTPGetEntryFileAttributes", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFTPGetEntryFileSize", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAFTPIsEntryDir", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMGetWindowLangGUID", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangInclude", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangDoubleSlash", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangComment", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangMultiComment", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangMultiCommentEnd", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangProc", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangEndp", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangProcInterlace", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangProcOrphan", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangProcOpenBlock", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangProcCloseBlock", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangFileName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangIncludeName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowLangIncludeNameFoot", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMDecodeRelativePaths", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRemoveRelativePaths", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMEncodeRelativePaths", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRunUserCommand", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRunCommandScript", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRunDOSCmd", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRunDOSCmdCapture", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDisplayHelpFile", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetWindowPath", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMIsFileWorkSpace", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMIsFileProject", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCurrentChildState", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMFocusOnFile", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetFileNameFromTreeViewItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetLabelFromTreeViewItem", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMOpenUnknownFile", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMHexToDec", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDecToHex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDecToBin", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMHexToBin", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMBinToDec", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMBinToHex", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMHexToFlt", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMFltToHex", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMGetProceduresList", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetLineFromFile", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCreateNewWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMClearWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMReloadWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCloseWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCloseAllWindows", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSaveAllWindows", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMPrintWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSaveWindow", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMUndo", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRedo", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCut", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCopy", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCopyFilePathName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCopyEntireText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMPaste", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDeleteSel", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDeleteLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDeleteToStart", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMDeleteToEnd", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSelectLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSelectAll", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSelectProc", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMFindNext", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMFindPrevious", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMFindSelection", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMGetIniFilePathName", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetQuickDirectory", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetContext", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetcurrentLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSelectCurrentWord", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCurrentWord", i, PILL_STORE_ENFORCE, 1);

	FillPILLConstant(CodeContext, "WAMMInsertLine", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMInsertLines", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSetText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCaretRealXCoordinate", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCaretRealYCoordinate", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSetFocusInsideWindow", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMRestoreMDIFromSysTray", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMInstallHookProc", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMUnInstallHookProc", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCurrentLineNumber", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCurrentColNumber", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetCurrentLineText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMGetLineText", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMWriteToOutputBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMEnumSourceWindows", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMAddInKillZombie", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSpAddInIsAvailable", i, PILL_STORE_ENFORCE, 1);
	
    FillPILLConstant(CodeContext, "WAMMMiscInputBox", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMIsChildReadOnly", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMSetChildReadOnly", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCreateCustomChildDialog", i, PILL_STORE_ENFORCE, 1);
	FillPILLConstant(CodeContext, "WAMMCreateCodeMax", i, PILL_STORE_ENFORCE, 1);
}
*/
