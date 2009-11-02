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
// AddIns.cpp: AddIns core handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WALib_WinSock.h"
#include "AddIns.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "Menus.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"
#include "Languages.h"
#include "CmdMenus.h"
#include "ProjectsBuilder.h"
#include "ConstCodeMax.h"
#include "ConstRes.h"
#include "Main.h"
#include "Toolbars.h"
#include "Splitters.h"
#include "Statusbar.h"
#include "ImageLists.h"
#include "Dlg_Args.h"
#include "Dlg_Addins.h"
#include "Dlg_EnterValue.h"

// -----------------------------------------------------------------------
// Variables
CList <HMODULE> AddInsDLL;
CList <HMODULE> OldAddInsDLL;
CList <char *> AddInsFiles;
CList <int> LoadedAddIns;
CList <int> RunningAddIns;
CList <int> OldRunningAddIns;
CList <char *> OldAddInsFiles;
CList <char *> AddInsDescriptions;
CList <FARPROC> AddInsLoad;
CList <FARPROC> AddInsUnLoad;
CList <FARPROC> AddInsMenu;
WALIB AddInLib;

CList <long> MDICreateHooks;
CList <DLGPROC> MDIHooks;
CList <DLGPROC> MDIClientHooks;
CList <long> ChildCreateHooks;
CList <DLGPROC> ChildHooks;
CList <DLGPROC> CodeMaxHooks;
CList <int> MDICreateHooksNbr;
CList <int> MDIHooksNbr;
CList <int> MDIClientHooksNbr;
CList <int> ChildCreateHooksNbr;
CList <int> ChildHooksNbr;
CList <int> CodeMaxHooksNbr;

//CList String_Garbage;

long HookNbr;

void (CALLBACK *CurCustChildInit)(HWND hwnd);
WNDPROC CurCustChildProc;
CStr CurCustChildTitle;

// -----------------------------------------------------------------------
// Functions declarations
void CALLBACK CustChildWinInitProc(HWND hWnd);
LRESULT CALLBACK CustChildWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// Fill the AddIns arrays with dll names and attributes
void FillAddInsArrays(void)
{
    int i = 0;
    long NbrAddin = 0;
    CStr AddInFileName;
    CStr AddInLoaded;
    CStr AddInRunning;
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    CStr Auth;
    CStr Description;
    CStr BufString;

    AddInsFiles.Erase();
    AddInsDescriptions.Erase();
    LoadedAddIns.Erase();
    AddInsLoad.Erase();
    AddInsUnLoad.Erase();
    AddInsMenu.Erase();
    RunningAddIns.Erase();
    AddInsDLL.Erase();
    
	NbrAddin = 0;
    for(i = 0; i <= 999; i++)
    {
        AddInFileName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = WAIniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = WAIniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInFileName;
		AddInLibMod = LoadLibrary(BufString.Get_String());
        if(AddInLibMod != 0)
        {
            AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
            AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
            AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
            AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
            AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
            // We need all procedures to run it correctly
            if(AddInAuth != 0 && AddInDesc != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
            {
                // Register dll name
                AddInsDLL.Add(AddInLibMod);
                AddInsFiles.Add(AddInFileName.Get_String());
                AddInsDescriptions.Add(GetDLLDescription(AddInFileName, AddInDesc).Get_String());
                // Register loaded state
                if(strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
                else LoadedAddIns.Add(0L);
				if(strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
                else RunningAddIns.Add(0L);
                AddInsLoad.Add(AddInLoad);
                AddInsUnLoad.Add(AddInUnload);
                AddInsMenu.Add(AddInMenu);
				WAIniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                WAIniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                WAIniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                WAIniWriteKey("AddIns", "File" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInFileName, MainIniFile);
                WAIniWriteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInLoaded, MainIniFile);
                WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInRunning, MainIniFile);
                NbrAddin++;
            }
            else
            {
                WAIniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                WAIniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                WAIniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            }
        }
        else
        {
            WAIniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
    }
}

// -----------------------------------------------------------------------
// Refresh the AddIns arrays with dll names and attributes
void ReFillAddInsArrays(void)
{
    int i = 0;
    int k = 0;
    long Revk = 0;
    long DllNewHandle = 0;
    CStr AddInFileName;
    CStr AddInLoaded;
    CStr AddInRunning;
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    CStr Auth;
    CStr Description;
    long NbrAddins = 0;
    long HandlePos = 0;
    CStr BufString;

    AddInsFiles.Erase();
    AddInsDescriptions.Erase();
    LoadedAddIns.Erase();
    AddInsLoad.Erase();
    AddInsUnLoad.Erase();
    AddInsMenu.Erase();
    RunningAddIns.Erase();
    
	for(i = 0; i<= 999; i++)
	{
        AddInFileName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = WAIniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = WAIniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        // Search if an addin had been added in the directory
        // before displaying the manager.
        DllNewHandle = i;
        HandlePos = SearchOldAddInName(AddInFileName);
        if(HandlePos == -1)
        {
            // Move entries down
            Revk = AddInsDLL.Amount();
            // Handle to be filled with library handle
            // We need to create a new empty entry in the handles table
            AddInsDLL.Add(0L);
			// Now we can move down the entries (if required)
            // (From top to bottom)
			for(k = DllNewHandle; k < (AddInsDLL.Amount() - 1); k++)
			{
                AddInsDLL.Set(Revk, AddInsDLL.Get(Revk - 1)->Content);
                Revk--;
            }
			// Clear the cache entry (to force load of the dll)
			AddInsDLL.Set(DllNewHandle, 0);
        }
        else
        {
            // Restore old handle
            AddInsDLL.Set(DllNewHandle, OldAddInsDLL.Get(HandlePos)->Content);
        }
        AddInLibMod = AddInsDLL.Get(DllNewHandle)->Content;
        if(AddInLibMod == 0)
        {
			// Reload it
            BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInFileName;
			AddInLibMod = LoadLibrary(BufString.Get_String());
            AddInsDLL.Set(DllNewHandle, AddInLibMod);
        }
        AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
        AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
        AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
        // We need all procedures to run it correctly
        if(AddInAuth != 0 && AddInDesc != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
        {
            // Register dll name
            AddInsFiles.Add(AddInFileName.Get_String());
            AddInsDescriptions.Add(GetDLLDescription(AddInFileName, AddInDesc).Get_String());
            // Register loaded state
            if(strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
            else LoadedAddIns.Add(0L);
			if(strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
            else RunningAddIns.Add(0L);
            AddInsLoad.Add(AddInLoad);
            AddInsUnLoad.Add(AddInUnload);
            AddInsMenu.Add(AddInMenu);
            WAIniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniWriteKey("AddIns", "File" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInFileName, MainIniFile);
            WAIniWriteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInLoaded, MainIniFile);
            WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInRunning, MainIniFile);
            NbrAddins++;
        }
        else
        {
            WAIniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            WAIniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
    }
    LoadedAddIns.Erase();
    RunningAddIns.Erase();
    for(i = 0; i<= 999; i++)
    {
        AddInFileName = WAIniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = WAIniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = WAIniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        if(strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
        else LoadedAddIns.Add(0L);
		if(strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
        else RunningAddIns.Add(0L);
    }
}

// -----------------------------------------------------------------------
// Start AddIns at startup if necessary
void SetAddInsRunningState(void)
{
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Must run now ?
        if(RunningAddIns.Get(i)->Content == 1)
        {
            if(AddInsLoad.Get(i)->Content != 0)
            {
                if(JumpToAddrArg(AddInsLoad.Get(i)->Content, (long) &AddInLib) == ADDIN_FINISHED)
                {
                    RunningAddIns.Set(i, 0);
                    // Unload it now
                    if(AddInsUnLoad.Get(i)->Content != 0)
                    {
						JumpToAddr(AddInsUnLoad.Get(i)->Content);
                    }
					// Stop it for good
                    WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", MainIniFile);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Stop all the running AddIns
void StopAddIns(void)
{
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Must run now ?
        if(RunningAddIns.Get(i)->Content == 1)
        {
			if(AddInsUnLoad.Get(i)->Content != 0)
			{
				JumpToAddr(AddInsUnLoad.Get(i)->Content);
			}
		}
		if(AddInsDLL.Get(i)->Content != 0)
		{
            FreeLibrary(AddInsDLL.Get(i)->Content);
            AddInsDLL.Set(i, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Shutdown an AddIn (Return old running state)
long ShutDownAddIn(long AddInNumber)
{
    long ReturnValue = 0;
	
	// Store old state
    ReturnValue = RunningAddIns.Get(AddInNumber)->Content;
    if(RunningAddIns.Get(AddInNumber)->Content == 1)
    {
		if(AddInsUnLoad.Get(AddInNumber)->Content != 0)
		{
			JumpToAddr(AddInsUnLoad.Get(AddInNumber)->Content);
		}
	}
	if(AddInsDLL.Get(AddInNumber)->Content != 0)
	{
        FreeLibrary(AddInsDLL.Get(AddInNumber)->Content);
        // Dlg_Filters.cpp for more infos
		#ifndef _DEBUG
            CloseHandle((HANDLE) AddInsDLL.Get(AddInNumber)->Content);
        #endif
        AddInsDLL.Set(AddInNumber, 0);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Restart an AddIn (Must use ShutDownAddIn before)
long RestartAddIn(long AddInNumber)
{
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
	long ReturnValue = 0;
	CStr BufString;

	BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInsFiles.Get(AddInNumber)->Content;
    AddInsDLL.Set(AddInNumber, LoadLibrary(BufString.Get_String()));
    if(AddInsDLL.Get(AddInNumber)->Content != 0)
    {
        AddInDesc = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInDescription");
        AddInAuth = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInLoad");
        AddInUnload = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInMenu");
        
		AddInsDescriptions.Set(AddInNumber, GetDLLDescription(AddInsFiles.Get(AddInNumber)->Content, AddInDesc).Get_String());
        AddInsLoad.Set(AddInNumber, AddInLoad);
        AddInsUnLoad.Set(AddInNumber, AddInUnload);
        AddInsMenu.Set(AddInNumber, AddInMenu);
        if(RunningAddIns.Get(AddInNumber)->Content == 1) RunAddIn(AddInNumber);
        ReturnValue = 1;
        WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(AddInNumber, 3).Get_String(), RunningAddIns.Get(AddInNumber)->Content, MainIniFile);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the AddIns menu with dll names and attributes
void FillAddInsMenu(void)
{
    long AddInSep = 0;
    int i = 0;

    // Remove them all
    while(DeleteMenu(hEditAddInsMenu, 1, MF_BYPOSITION) != 0)
    {
    }
    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Is it loaded by user ?
        if(LoadedAddIns.Get(i)->Content != 0)
        {
            if(AddInSep == 0)
            {
                AppendMenu(hEditAddInsMenu, MF_SEPARATOR, (UINT) -1, "-");
                AddInSep = 1;
            }
            AppendMenu(hEditAddInsMenu, MF_STRING, MENU_ADDINS_IDBASE + MENU_ADDINS_FIRSTADDIN_ID + i, AddInsDescriptions.Get(i)->Content);
        }
    }
}

// -----------------------------------------------------------------------
// Retrieve the index of an AddIn name or -1
long SearchAddInName(CStr AddInName)
{
    long ReturnValue = -1;
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        if(strcmpi(AddInsFiles.Get(i)->Content, AddInName.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the index of an old AddIn name or -1
long SearchOldAddInName(CStr AddInName)
{
    long ReturnValue = -1;
    int i = 0;

    for(i = 0; i < OldAddInsFiles.Amount(); i++)
    {
        if(strcmpi(OldAddInsFiles.Get(i)->Content, AddInName.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve description of an DLL (AddIn of Filter)
CStr GetDLLDescription(CStr AddInName, FARPROC DescriptionRoutine)
{
    long DescriptionLng = 0;
    long DescriptionLen = 0;
    CStr ReturnValue;
	CStr BufString;

    if(DescriptionRoutine == 0) goto CantGuessDescription;
    DescriptionLng = DescriptionRoutine();
    if(DescriptionLng == 0)
    {
CantGuessDescription:
        // Get filename if no description found
        DescriptionLen = AddInName.Len();
        BufString = BufString.String(DescriptionLen, 1);
        RtlCopyMemory(BufString.Get_String(), AddInName.Get_String(), DescriptionLen);
    }
    else
    {
        DescriptionLen = strlen((char *) DescriptionLng);
        BufString = BufString.String(DescriptionLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) DescriptionLng, DescriptionLen);
    }
    ReturnValue = BufString;
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve author name of a DLL (AddIn of Filter)
CStr GetDLLAuthor(FARPROC AuthorRoutine)
{
    long AuthLng = 0;
    long AuthLen = 0;
    CStr ReturnValue;
	CStr BufString;

    if(AuthorRoutine == 0) goto CantGuessAuthor;
    // Should return an author name string or 0
    AuthLng = AuthorRoutine();
    if(AuthLng == 0)
    {
CantGuessAuthor:
        // Get filename if no description found
        AuthLen = strlen("(The unknown soldier)");
        BufString = BufString.String(AuthLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) "(The unknown soldier)", AuthLen);
    }
    else
    {
        AuthLen = strlen((char *) AuthLng);
        BufString = BufString.String(AuthLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) AuthLng, AuthLen);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a file is a suitable addin
long IsAddIn(CStr AddInName)
{
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
	long ReturnValue = 0;

    AddInLibMod = LoadLibrary(AddInName.Get_String());
    if(AddInLibMod != 0)
    {
        AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
        AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
        AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
        // Must export all procedures
        if(AddInDesc != 0 && AddInAuth != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
        {
            // It is an AddIn
            ReturnValue = 1;
        }
        FreeLibrary(AddInLibMod);
        CloseHandle((HANDLE) AddInLibMod);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a file is a suitable support addin
long IsSpAddIn(CStr SpAddInName)
{
    HMODULE SpAddInLib = 0;
    FARPROC SpAddInDesc = 0;
    FARPROC SpAddInAuth = 0;
    FARPROC SpAddInLoad = 0;
    FARPROC SpAddInUnload = 0;
    FARPROC SpAddInMenu = 0;
	long ReturnValue = 0;
    
    SpAddInLib = LoadLibrary(SpAddInName.Get_String());
    if(SpAddInLib != 0)
    {
        SpAddInDesc = GetProcAddress(SpAddInLib, "SpAddInDescription");
        SpAddInAuth = GetProcAddress(SpAddInLib, "SpAddInAuthor");
        SpAddInLoad = GetProcAddress(SpAddInLib, "SpAddInLoad");
        SpAddInUnload = GetProcAddress(SpAddInLib, "SpAddInUnLoad");
        SpAddInMenu = GetProcAddress(SpAddInLib, "SpAddInAskResource");
        // Must export all procedures
        if(SpAddInDesc != 0 &&
           SpAddInAuth != 0 &&
           SpAddInLoad != 0 &&
           SpAddInUnload != 0 &&
           SpAddInMenu != 0)
        {
            // It is an AddIn
            ReturnValue = 1;
        }
        FreeLibrary(SpAddInLib);
        CloseHandle((HANDLE) SpAddInLib);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Running asked
long RunAddIn(long AddInToRun)
{
	long ReturnValue = 0;

    RunningAddIns.Set(AddInToRun, 0);
    // if addin is already running then will need to notify a possible menu
    // to perform the related command.
    // (auto update of an addin shouldn't run the menu command in case of a
    // persistant addin (which might be used for somekind of prefs).
    ReturnValue = ADDIN_PERSISTANT;
    if(AddInsLoad.Get(AddInToRun)->Content != 0)
    {
        switch(JumpToAddrArg(AddInsLoad.Get(AddInToRun)->Content, (long) &AddInLib))
        {
            case ADDIN_FINISHED:
                RunningAddIns.Set(AddInToRun, 0);
                // Now trigger the menu routine
                if(AddInsMenu.Get(AddInToRun)->Content != 0)
                {
                    RunningAddIns.Set(AddInToRun, 1);
                    switch(JumpToAddrArg(AddInsMenu.Get(AddInToRun)->Content, (long) &AddInLib))
                    {
                        case ADDIN_DIE:
                            if(AddInsUnLoad.Get(AddInToRun)->Content != 0)
                            {
								JumpToAddr(AddInsUnLoad.Get(AddInToRun)->Content);
                            }
							RunningAddIns.Set(AddInToRun, 0);
							break;
					}
                }
                ReturnValue = ADDIN_FINISHED;
				break;
            case ADDIN_PERSISTANT:
                RunningAddIns.Set(AddInToRun, 1);
                ReturnValue = ADDIN_PERSISTANT;
				break;
		}
    }
    // Store running state
    WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(AddInToRun, 3).Get_String(),
                  RunningAddIns.Get(AddInToRun)->Content, MainIniFile);
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the WALib datas
void FillWaLib(void)
{
    AddInLib.ButtonGetImage = (long) &ButtonGetImage;
    AddInLib.ButtonGetState = (long) &ButtonGetState;
    AddInLib.ButtonSetIcon = (long) &ButtonSetIcon;
    AddInLib.CheckBoxGetState = (long) &CheckBoxGetState;
    AddInLib.CheckBoxSetState = (long) &CheckBoxSetState;
    AddInLib.WAClientGetActiveChild = (long) &WAClientGetActiveChild;
    AddInLib.WAClientSetNextChild = (long) &WAClientSetNextChild;
    AddInLib.WAClientSetPreviousChild = (long) &WAClientSetPreviousChild;
    AddInLib.WAClientResize = (long) &WAClientResize;
    AddInLib.WAClientTileArrangeIconic = (long) &WAClientTileArrangeIconic;
    AddInLib.WAClientTileCascade = (long) &WAClientTileCascade;
    AddInLib.WAClientTileHorizontal = (long) &WAClientTileHorizontal;
    AddInLib.WAClientTileVertical = (long) &WAClientTileVertical;
    AddInLib.ColorBoxGetColor = (long) &ColorBoxGetColor;
    AddInLib.ColorBoxSetColor = (long) &ColorBoxSetColor;
    AddInLib.GripBoxGetColor = (long) &GripBoxGetColor;
    AddInLib.GripBoxSetColor = (long) &GripBoxSetColor;
    AddInLib.GripBoxClose = (long) &GripBoxClose;
    AddInLib.GripBoxVisible = (long) &GripBoxVisible;
    AddInLib.GripBoxResize = (long) &GripBoxResize;
	AddInLib.WAComboBoxAddItem = (long) &Wrapper_WAComboBoxAddItem;
    AddInLib.WAComboBoxCount = (long) &WAComboBoxCount;
    AddInLib.WAComboBoxDeleteItem = (long) &WAComboBoxDeleteItem;
    AddInLib.WAComboBoxGetCurrentItem = (long) &Wrapper_WAComboBoxGetCurrentItem;
    AddInLib.WAComboBoxGetIndex = (long) &WAComboBoxGetIndex;
    AddInLib.WAComboBoxGetItem = (long) &Wrapper_WAComboBoxGetItem;
    AddInLib.WAComboBoxGetItemData = (long) &WAComboBoxGetItemData;
    AddInLib.WAComboBoxItemExist = (long) &Wrapper_WAComboBoxItemExist;
    AddInLib.WAComboBoxReset = (long) &WAComboBoxReset;
    AddInLib.WAComboBoxSetIndex = (long) &WAComboBoxSetIndex;
    AddInLib.WAComboBoxSetMaxLen = (long) &WAComboBoxSetMaxLen;
    AddInLib.WAComboBoxSaveInIniFile = (long) &Wrapper_WAComboBoxSaveInIniFile;
    AddInLib.WAComboBoxFillFromIniFile = (long) &Wrapper_WAComboBoxFillFromIniFile;
    AddInLib.WAControlNextHook = (long) &WAControlNextHook;
    AddInLib.WAControlBound = (long) &WAControlBound;
    AddInLib.WAControlBringToBottom = (long) &WAControlBringToBottom;
    AddInLib.WAControlBringToTop = (long) &WAControlBringToTop;
    AddInLib.WAControlClientHeight = (long) &WAControlClientHeight;
    AddInLib.WAControlClientLeft = (long) &WAControlClientLeft;
    AddInLib.WAControlClientTop = (long) &WAControlClientTop;
    AddInLib.WAControlClientWidth = (long) &WAControlClientWidth;
    AddInLib.WAControlClose = (long) &WAControlClose;
    AddInLib.WAControlEnable = (long) &WAControlEnable;
    AddInLib.WAControlFreeze = (long) &WAControlFreeze;
    AddInLib.WAControlGetClassName = (long) &Wrapper_WAControlGetClassName;
    AddInLib.WAControlGetFont = (long) &WAControlGetFont;
    AddInLib.WAControlGetHotKey = (long) &WAControlGetHotKey;
    AddInLib.WAControlGetHwndFromPoint = (long) &WAControlGetHwndFromPoint;
    AddInLib.WAControlGetIcon = (long) &WAControlGetIcon;
    AddInLib.WAControlGetNotifiedCommand = (long) &WAControlGetNotifiedCommand;
    AddInLib.WAControlGetNotifiedhWnd = (long) &WAControlGetNotifiedhWnd;
    AddInLib.WAControlGetNotifiedID = (long) &WAControlGetNotifiedID;
    AddInLib.WAControlGetNotifiedMsg = (long) &WAControlGetNotifiedMsg;
    AddInLib.WAControlGetText = (long) &Wrapper_WAControlGetText;
    AddInLib.WAControlGetTextLen = (long) &WAControlGetTextLen;
    AddInLib.WAControlGetWindowState = (long) &WAControlGetWindowState;
    AddInLib.WAControlGetXMousePos = (long) &WAControlGetXMousePos;
    AddInLib.WAControlGetYMousePos = (long) &WAControlGetYMousePos;
    AddInLib.WAControlHeight = (long) &WAControlHeight;
    AddInLib.WAControlIsEnabled = (long) &WAControlIsEnabled;
    AddInLib.WAControlIsVisible = (long) &WAControlIsVisible;
    AddInLib.WAControlIsWindowChild = (long) &WAControlIsWindowChild;
    AddInLib.WAControlLeft = (long) &WAControlLeft;
    AddInLib.WAControlRefresh = (long) &WAControlRefresh;
    AddInLib.WAControlRefreshLocal = (long) &WAControlRefreshLocal;
    AddInLib.WAControlRemTopMost = (long) &WAControlRemTopMost;
    AddInLib.WAControlRepaint = (long) &WAControlRepaint;
    AddInLib.WAControlResize = (long) &WAControlResize;
    AddInLib.WAControlSendMessage = (long) &WAControlSendMessage;
    AddInLib.WAControlSetClassCursor = (long) &WAControlSetClassCursor;
    AddInLib.WAControlSetFont = (long) &WAControlSetFont;
    AddInLib.WAControlSetFontNoRedraw = (long) &WAControlSetFontNoRedraw;
    AddInLib.WAControlSetText = (long) &Wrapper_WAControlSetText;
    AddInLib.WAControlSetTopMost = (long) &WAControlSetTopMost;
    AddInLib.WAControlTop = (long) &WAControlTop;
    AddInLib.WAControlVisible = (long) &WAControlVisible;
    AddInLib.WAControlWidth = (long) &WAControlWidth;
    AddInLib.WACreateButton = (long) &Wrapper_WACreateButton;
    AddInLib.WACreateCheckBox = (long) &Wrapper_WACreateCheckBox;
    AddInLib.WACreateClient = (long) &WACreateClient;
    AddInLib.WACreateColorBox = (long) &WACreateColorBox;
    AddInLib.WACreateGripBox = (long) &WACreateGripBox;
    AddInLib.WACreateComboBox = (long) &Wrapper_WACreateComboBox;
    AddInLib.WACreateDialog = (long) &Wrapper_WACreateDialog;
    AddInLib.WACreateFrame = (long) &Wrapper_WACreateFrame;
    AddInLib.WACreateHexBox = (long) &Wrapper_WACreateHexBox;
    AddInLib.WACreateImageList = (long) &WACreateImageList;
    AddInLib.WACreateIPBox = (long) &WACreateIPBox;
    AddInLib.WACreateLabel = (long) &Wrapper_WACreateLabel;
    AddInLib.WACreateListBox = (long) &WACreateListBox;
    AddInLib.WACreateListView = (long) &WACreateListView;
    AddInLib.WACreateMDIChildDialog = (long) &Wrapper_WACreateMDIChildDialog;
    AddInLib.WACreateMDIDialog = (long) &Wrapper_WACreateMDIDialog;
    AddInLib.WACreateModalDialog = (long) &Wrapper_WACreateModalDialog;
    AddInLib.WACreatePager = (long) &WACreatePager;
    AddInLib.WACreatePictureBox = (long) &WACreatePictureBox;
    AddInLib.WACreateProgressBar = (long) &WACreateProgressBar;
    AddInLib.WACreateRadioButton = (long) &Wrapper_WACreateRadioButton;
    AddInLib.WACreateRebar = (long) &WACreateRebar;
    AddInLib.WACreateRichTextBox = (long) &Wrapper_WACreateRichTextBox;
    AddInLib.WACreateScrollBar = (long) &WACreateScrollBar;
    AddInLib.WACreateDumpBox = (long) &WACreateDumpBox;
    AddInLib.WACreateStockModalDialog = (long) &Wrapper_WACreateStockModalDialog;
    AddInLib.WADumpBoxScrollDown = (long) &WADumpBoxScrollDown;
    AddInLib.WADumpBoxScrollUp = (long) &WADumpBoxScrollUp;
    AddInLib.WADumpBoxGetVisibleLines = (long) &WADumpBoxGetVisibleLines;
    AddInLib.WADumpBoxGetVisibleColumns = (long) &WADumpBoxGetVisibleColumns;
    AddInLib.WADumpBoxResize = (long) &WADumpBoxResize;
    AddInLib.WACreateSplashDialog = (long) &Wrapper_WACreateSplashDialog;
    AddInLib.WACreateSplitter = (long) &WACreateSplitter;
    AddInLib.WACreateStatusBar = (long) &Wrapper_WACreateStatusBar;
    AddInLib.WACreateSysTab = (long) &WACreateSysTab;
    AddInLib.WACreateTextBox = (long) &Wrapper_WACreateTextBox;
    AddInLib.WACreateToolBar = (long) &WACreateToolBar;
    AddInLib.WACreateTrackBar = (long) &WACreateTrackBar;
    AddInLib.WACreateTreeView = (long) &WACreateTreeView;
    AddInLib.WACreateUpDown = (long) &WACreateUpDown;
    AddInLib.WACursorDisable = (long) &WACursorDisable;
    AddInLib.WACursorEnable = (long) &WACursorEnable;
    AddInLib.WACursorSetCross = (long) &WACursorSetCross;
    AddInLib.WACursorSetNo = (long) &WACursorSetNo;
    AddInLib.WACursorSetNormal = (long) &WACursorSetNormal;
    AddInLib.WACursorSetWait = (long) &WACursorSetWait;
    AddInLib.WADateGetDay = (long) &Wrapper_WADateGetDay;
    AddInLib.WADateGetFormat = (long) &Wrapper_WADateGetFormat;
    AddInLib.WADateGetHour = (long) &Wrapper_WADateGetHour;
    AddInLib.WADateGetMinutes = (long) &Wrapper_WADateGetMinutes;
    AddInLib.WADateGetMonth = (long) &Wrapper_WADateGetMonth;
    AddInLib.WADateGetNow = (long) &Wrapper_WADateGetNow;
    AddInLib.WADateGetSeconds = (long) &Wrapper_WADateGetSeconds;
    AddInLib.WADateGetTimeFormat = (long) &Wrapper_WADateGetTimeFormat;
    AddInLib.WADateGetYear = (long) &Wrapper_WADateGetYear;
    AddInLib.DialogGetXUnit = (long) &DialogGetXUnit;
    AddInLib.DialogGetYUnit = (long) &DialogGetYUnit;
    AddInLib.DialogSetIcon = (long) &DialogSetIcon;
    AddInLib.DialogXPixelToUnit = (long) &DialogXPixelToUnit;
    AddInLib.DialogXUnitToPixel = (long) &DialogXUnitToPixel;
    AddInLib.DialogYPixelToUnit = (long) &DialogYPixelToUnit;
    AddInLib.DialogYUnitToPixel = (long) &DialogYUnitToPixel;
    AddInLib.WAFileClose = (long) &WAFileClose;
    AddInLib.WAFileCreateEmpty = (long) &Wrapper_WAFileCreateEmpty;
    AddInLib.WAFileExist = (long) &Wrapper_WAFileExist;
    AddInLib.WAFileGetAccessedTime = (long) &Wrapper_WAFileGetAccessedTime;
    AddInLib.WAFileGetCreationTime = (long) &Wrapper_WAFileGetCreationTime;
    AddInLib.WAFileGetDirectory = (long) &Wrapper_WAFileGetDirectory;
    AddInLib.WAFileGetExtension = (long) &Wrapper_WAFileGetExtension;
    AddInLib.WAFileGetFileName = (long) &Wrapper_WAFileGetFileName;
    AddInLib.WAFileGetSize = (long) &Wrapper_WAFileGetSize;
    AddInLib.WAFileGetWriteTime = (long) &Wrapper_WAFileGetWriteTime;
    AddInLib.WAFileIsDirectory = (long) &Wrapper_WAFileIsDirectory;
    AddInLib.WAFileOpenR = (long) &Wrapper_WAFileOpenR;
    AddInLib.WAFileOpenW = (long) &Wrapper_WAFileOpenW;
    AddInLib.WAFileOpenWAppend = (long) &Wrapper_WAFileOpenWAppend;
    AddInLib.WAFileRemoveExtension = (long) &Wrapper_WAFileRemoveExtension;
    AddInLib.WAFileReplaceExtension = (long) &Wrapper_WAFileReplaceExtension;
    AddInLib.WAFileIsReadOnly = (long) &Wrapper_WAFileIsReadOnly;
    AddInLib.WAFileIsUnix = (long) &Wrapper_WAFileIsUnix;
    AddInLib.WAFileWriteBuffer = (long) &WAFileWriteBuffer;
    AddInLib.WAFileWriteLine = (long) &Wrapper_WAFileWriteLine;
    AddInLib.WAFileLoadIntoMem = (long) &Wrapper_WAFileLoadIntoMem;
    AddInLib.WAFileSaveFromMem = (long) &Wrapper_WAFileSaveFromMem;
    AddInLib.WAFileDir = (long) &Wrapper_WAFileDir;
    AddInLib.FrameRefreshToolbar = (long) &FrameRefreshToolbar;
    AddInLib.WAFTPGetEntryDate = (long) &Wrapper_WAFTPGetEntryDate;
    AddInLib.WAFTPGetEntryFileAttributes = (long) &Wrapper_WAFTPGetEntryFileAttributes;
    AddInLib.WAFTPGetEntryFileName = (long) &Wrapper_WAFTPGetEntryFileName;
    AddInLib.WAFTPGetEntryFileSize = (long) &Wrapper_WAFTPGetEntryFileSize;
    AddInLib.WAFTPInitiatePort = (long) &WAFTPInitiatePort;
    AddInLib.WAFTPIsEntryDir = (long) &Wrapper_WAFTPIsEntryDir;
    AddInLib.WAGDIDrawHorzSep = (long) &WAGDIDrawHorzSep;
    AddInLib.WAGDIDrawLine = (long) &WAGDIDrawLine;
    AddInLib.WAGDIDrawLineXOR = (long) &WAGDIDrawLineXOR;
    AddInLib.WAGDIDrawPixel = (long) &WAGDIDrawPixel;
    AddInLib.WAGDIDrawVertSep = (long) &WAGDIDrawVertSep;
    AddInLib.WAGDIFillWindow = (long) &WAGDIFillWindow;
    AddInLib.WAGDIFillWindowFromBrush = (long) &WAGDIFillWindowFromBrush;
    AddInLib.WAGDIFillWindowFromBrushAndhDC = (long) &WAGDIFillWindowFromBrushAndhDC;
    AddInLib.WAGDICreateColorBrush = (long) &WAGDICreateColorBrush;
    AddInLib.WAGDIGetTextHeight = (long) &Wrapper_WAGDIGetTextHeight;
    AddInLib.WAGDIGetTextWidth = (long) &Wrapper_WAGDIGetTextWidth;
    AddInLib.WAGDIObtainFont = (long) &Wrapper_WAGDIObtainFont;
    AddInLib.WAGDIWriteClippedText = (long) &Wrapper_WAGDIWriteClippedText;
    AddInLib.WAGDIWriteText = (long) &Wrapper_WAGDIWriteText;
    AddInLib.WAGDIGetFontWidth = (long) &WAGDIGetFontWidth;
    AddInLib.WAGDIGetFontHeight = (long) &WAGDIGetFontHeight;
    AddInLib.WAGDICreateBackDC = (long) &WAGDICreateBackDC;
    AddInLib.WAGDIDestroyBackDC = (long) &WAGDIDestroyBackDC;
    AddInLib.WAGDIBlitBackDC = (long) &WAGDIBlitBackDC;
    AddInLib.WAGDIColorCalcLuminosity = (long) &WAGDIColorCalcLuminosity;
    AddInLib.WAGDIGetSerif = (long) &WAGDIGetSerif;
    AddInLib.WAGDIGetSerif10 = (long) &WAGDIGetSerif10;
    AddInLib.WAGDIGetCourierNew9 = (long) &WAGDIGetCourierNew9;
    AddInLib.ImageListAddIcon = (long) &ImageListAddIcon;
    AddInLib.ImageListDestroy = (long) &ImageListDestroy;
    AddInLib.ImageListGetIcon = (long) &ImageListGetIcon;
    AddInLib.ImageListGetIconsCount = (long) &ImageListGetIconsCount;
    AddInLib.WAIniDeleteKey = (long) &Wrapper_WAIniDeleteKey;
    AddInLib.WAIniReadKey = (long) &Wrapper_WAIniReadKey;
    AddInLib.WAIniReadBoolKey = (long) &Wrapper_WAIniReadBoolKey;
    AddInLib.WAIniWriteKey = (long) &Wrapper_WAIniWriteKey;
    AddInLib.WAIniWriteSection = (long) &Wrapper_WAIniWriteSection;
    AddInLib.IPBoxClearIP = (long) &IPBoxClearIP;
    AddInLib.IPBoxGetIP = (long) &IPBoxGetIP;
    AddInLib.IPBoxIsBlank = (long) &IPBoxIsBlank;
    AddInLib.IPBoxSetFocus = (long) &IPBoxSetFocus;
    AddInLib.IPBoxSetIP = (long) &IPBoxSetIP;
    AddInLib.IPBoxSetRange = (long) &IPBoxSetRange;
    AddInLib.LabelGetImage = (long) &LabelGetImage;
    AddInLib.WAListBoxAddItem = (long) &Wrapper_WAListBoxAddItem;
    AddInLib.WAListBoxCount = (long) &WAListBoxCount;
    AddInLib.WAListBoxDeleteItem = (long) &WAListBoxDeleteItem;
    AddInLib.WAListBoxGetCurrentItem = (long) &Wrapper_WAListBoxGetCurrentItem;
    AddInLib.WAListBoxGetItem = (long) &Wrapper_WAListBoxGetItem;
    AddInLib.WAListBoxGetItemData = (long) &WAListBoxGetItemData;
    AddInLib.WAListBoxGetSelItemIndex = (long) &WAListBoxGetSelItemIndex;
    AddInLib.WAListBoxGetTopIndex = (long) &WAListBoxGetTopIndex;
    AddInLib.WAListBoxIsItemSelected = (long) &WAListBoxIsItemSelected;
    AddInLib.WAListBoxItemExist = (long) &Wrapper_WAListBoxItemExist;
    AddInLib.WAListBoxProcessDrag = (long) &WAListBoxProcessDrag;
    AddInLib.WAListBoxReplaceItem = (long) &Wrapper_WAListBoxReplaceItem;
    AddInLib.WAListBoxReplaceSelItem = (long) &Wrapper_WAListBoxReplaceSelItem;
    AddInLib.WAListBoxReset = (long) &WAListBoxReset;
    AddInLib.WAListBoxSelItemDown = (long) &WAListBoxSelItemDown;
    AddInLib.WAListBoxSelItemRemove = (long) &WAListBoxSelItemRemove;
    AddInLib.WAListBoxSelItemUp = (long) &WAListBoxSelItemUp;
    AddInLib.WAListBoxSetColumnsWidth = (long) &WAListBoxSetColumnsWidth;
    AddInLib.WAListBoxSetHorzScrollWidth = (long) &WAListBoxSetHorzScrollWidth;
    AddInLib.WAListBoxSetIndex = (long) &WAListBoxSetIndex;
    AddInLib.WAListBoxSetItemData = (long) &WAListBoxSetItemData;
    AddInLib.WAListBoxSetTopIndex = (long) &WAListBoxSetTopIndex;
    AddInLib.WAListBoxItemUnderCursor = (long) &WAListBoxItemUnderCursor;
    AddInLib.WAListViewAddCol = (long) &Wrapper_WAListViewAddCol;
    AddInLib.WAListViewAddItem = (long) &Wrapper_WAListViewAddItem;
    AddInLib.WAListViewBeginDrag = (long) &WAListViewBeginDrag;
    AddInLib.WAListViewCheckBoxItemDoubleClick = (long) &WAListViewCheckBoxItemDoubleClick;
    AddInLib.WAListViewClear = (long) &WAListViewClear;
    AddInLib.WAListViewDeleteItem = (long) &WAListViewDeleteItem;
    AddInLib.WAListViewDetachImageList = (long) &WAListViewDetachImageList;
    AddInLib.WAListViewEditValidated = (long) &WAListViewEditValidated;
    AddInLib.WAListViewEndDrag = (long) &WAListViewEndDrag;
    AddInLib.WAListViewFindItem = (long) &Wrapper_WAListViewFindItem;
    AddInLib.WAListViewFindSubItem = (long) &Wrapper_WAListViewFindSubItem;
    AddInLib.WAListViewGetBackColor = (long) &WAListViewGetBackColor;
    AddInLib.WAListViewGetColWidth = (long) &WAListViewGetColWidth;
    AddInLib.WAListViewGetCountPerPage = (long) &WAListViewGetCountPerPage;
    AddInLib.WAListViewGetEditControl = (long) &WAListViewGetEditControl;
    AddInLib.WAListViewGetEditResult = (long) &Wrapper_WAListViewGetEditResult;
    AddInLib.WAListViewGetExStyle = (long) &WAListViewGetExStyle;
    AddInLib.WAListViewGetFirstFocusItem = (long) &WAListViewGetFirstFocusItem;
    AddInLib.WAListViewGetHeaderLabel = (long) &Wrapper_WAListViewGetHeaderLabel;
    AddInLib.WAListViewGetHeaderPosition = (long) &WAListViewGetHeaderPosition;
    AddInLib.WAListViewGetHotCursor = (long) &WAListViewGetHotCursor;
    AddInLib.WAListViewGetImageList = (long) &WAListViewGetImageList;
    AddInLib.WAListViewGetItemCheckbox = (long) &WAListViewGetItemCheckbox;
    AddInLib.WAListViewGetItemIcon = (long) &WAListViewGetItemIcon;
    AddInLib.WAListViewGetItemSelState = (long) &WAListViewGetItemSelState;
    AddInLib.WAListViewGetItemText = (long) &Wrapper_WAListViewGetItemText;
    AddInLib.WAListViewGetItemUnderCursor = (long) &WAListViewGetItemUnderCursor;
    AddInLib.WAListViewGetNotifiedColumnIndex = (long) &WAListViewGetNotifiedColumnIndex;
    AddInLib.WAListViewGetSelItem = (long) &WAListViewGetSelItem;
    AddInLib.WAListViewGetSelItemText = (long) &Wrapper_WAListViewGetSelItemText;
    AddInLib.WAListViewGetSubItemImage = (long) &WAListViewGetSubItemImage;
    AddInLib.WAListViewGetSysHeader = (long) &WAListViewGetSysHeader;
    AddInLib.WAListViewGetTextBackColor = (long) &WAListViewGetTextBackColor;
    AddInLib.WAListViewGetTextColor = (long) &WAListViewGetTextColor;
    AddInLib.WAListViewGetToolTips = (long) &WAListViewGetToolTips;
    AddInLib.WAListViewGetTopIndex = (long) &WAListViewGetTopIndex;
    AddInLib.WAListViewIsCheckboxNotify = (long) &WAListViewIsCheckboxNotify;
    AddInLib.WAListViewItemCount = (long) &WAListViewItemCount;
    AddInLib.WAListViewMoveDrag = (long) &WAListViewMoveDrag;
    AddInLib.WAListViewMultiSelFirstItem = (long) &WAListViewMultiSelFirstItem;
    AddInLib.WAListViewPasteAutoEdit = (long) &WAListViewPasteAutoEdit;
    AddInLib.WAListViewReOrder = (long) &WAListViewReOrder;
    AddInLib.WAListViewSelItemCount = (long) &WAListViewSelItemCount;
    AddInLib.WAListViewSetBackColor = (long) &WAListViewSetBackColor;
    AddInLib.WAListViewSetColWidth = (long) &WAListViewSetColWidth;
    AddInLib.WAListViewSetEditModeOn = (long) &WAListViewSetEditModeOn;
    AddInLib.WAListViewSetHeaderPosition = (long) &WAListViewSetHeaderPosition;
    AddInLib.WAListViewSetItemCheckbox = (long) &WAListViewSetItemCheckbox;
    AddInLib.WAListViewSetItemFocus = (long) &WAListViewSetItemFocus;
    AddInLib.WAListViewSetItemSel = (long) &WAListViewSetItemSel;
    AddInLib.WAListViewSetItemHighlight = (long) &WAListViewSetItemHighlight;
    AddInLib.WAListViewSetScroll = (long) &WAListViewSetScroll;
    AddInLib.WAListViewSetSelItemText = (long) &Wrapper_WAListViewSetSelItemText;
    AddInLib.WAListViewSetSubItem = (long) &Wrapper_WAListViewSetSubItem;
    AddInLib.WAListViewSetSubItemImage = (long) &WAListViewSetSubItemImage;
    AddInLib.WAListViewSort = (long) &WAListViewSort;
    AddInLib.WAMenuEnable = (long) &WAMenuEnable;
    AddInLib.WAMenuGetString = (long) &Wrapper_WAMenuGetString;
    AddInLib.WAMenuSetDefaultItem = (long) &WAMenuSetDefaultItem;
    AddInLib.WAMenuSetItemState = (long) &WAMenuSetItemState;
    AddInLib.WAMenuSetItemType = (long) &WAMenuSetItemType;
    AddInLib.WAMenuSetItemText = (long) &Wrapper_WAMenuSetItemText;
    AddInLib.WAPagerDisplaySetSize = (long) &WAPagerDisplaySetSize;
    AddInLib.WAPagerGetBackColor = (long) &WAPagerGetBackColor;
    AddInLib.WAPagerGetBorderSize = (long) &WAPagerGetBorderSize;
    AddInLib.WAPagerGetButtonSize = (long) &WAPagerGetButtonSize;
    AddInLib.WAPagerGetNotifiedChild = (long) &WAPagerGetNotifiedChild;
    AddInLib.WAPagerGetScrollPos = (long) &WAPagerGetScrollPos;
    AddInLib.WAPagerGetOrientation = (long) &WAPagerGetOrientation;
    AddInLib.WAPagerSetOrientation = (long) &WAPagerSetOrientation;
    AddInLib.WAPagerChangeOrientation = (long) &WAPagerChangeOrientation;
    AddInLib.WAPagerMaximizeX = (long) &WAPagerMaximizeX;
    AddInLib.WAPagerMaximizeY = (long) &WAPagerMaximizeY;
    AddInLib.WAPagerSetBackColor = (long) &WAPagerSetBackColor;
    AddInLib.PictureBoxChangeIcon = (long) &PictureBoxChangeIcon;
    AddInLib.PictureBoxChangeImage = (long) &PictureBoxChangeImage;
    AddInLib.ProgressBarSetRelPos = (long) &ProgressBarSetRelPos;
    AddInLib.ProgressBarSetRange = (long) &ProgressBarSetRange;
    AddInLib.RadioButtonGetState = (long) &RadioButtonGetState;
    AddInLib.RadioButtonSetState = (long) &RadioButtonSetState;
    AddInLib.WARebarAddBand = (long) &Wrapper_WARebarAddBand;
    AddInLib.WARebarBandVisible = (long) &WARebarBandVisible;
    AddInLib.WARebarGetBackGroundColor = (long) &WARebarGetBackGroundColor;
    AddInLib.WARebarGetBandsCount = (long) &WARebarGetBandsCount;
    AddInLib.WARebarGetHeight = (long) &WARebarGetHeight;
    AddInLib.WARebarGetImageList = (long) &WARebarGetImageList;
    AddInLib.WARebarGetPalette = (long) &WARebarGetPalette;
    AddInLib.WARebarGetRowsCount = (long) &WARebarGetRowsCount;
    AddInLib.WARebarGetTextColor = (long) &WARebarGetTextColor;
    AddInLib.WARebarGetToolTips = (long) &WARebarGetToolTips;
    AddInLib.WARebarGetXSize = (long) &WARebarGetXSize;
    AddInLib.WARebarGetYSize = (long) &WARebarGetYSize;
    AddInLib.WARebarRemoveBand = (long) &WARebarRemoveBand;
    AddInLib.WARebarResize = (long) &WARebarResize;
    AddInLib.WARebarSetBackColor = (long) &WARebarSetBackColor;
    AddInLib.WARebarSetTextColor = (long) &WARebarSetTextColor;
    AddInLib.WARegistryDeleteKey = (long) &Wrapper_WARegistryDeleteKey;
    AddInLib.WARegistryGetKeyValue = (long) &Wrapper_WARegistryGetKeyValue;
    AddInLib.WARegistryUpdateKey = (long) &Wrapper_WARegistryUpdateKey;
    AddInLib.ScreenHeight = (long) &ScreenHeight;
    AddInLib.ScreenWidth = (long) &ScreenWidth;
    AddInLib.ScreenRectToClient = (long) &ScreenRectToClient;
    AddInLib.WAScrollBarGetMaxRange = (long) &WAScrollBarGetMaxRange;
    AddInLib.WAScrollBarGetMinRange = (long) &WAScrollBarGetMinRange;
    AddInLib.WAScrollBarGetPageRange = (long) &WAScrollBarGetPageRange;
    AddInLib.WAScrollBarSetPageRange = (long) &WAScrollBarSetPageRange;
    AddInLib.WAScrollBarShowHide = (long) &WAScrollBarShowHide;
    AddInLib.WAScrollBarSetMinMaxRange = (long) &WAScrollBarSetMinMaxRange;
    AddInLib.MathGenerateRandomNumber = (long) &MathGenerateRandomNumber;
    AddInLib.SocketAcceptConnection = (long) &SocketAcceptConnection;
    AddInLib.SocketConnect = (long) &Wrapper_SocketConnect;
    AddInLib.SocketCreate = (long) &SocketCreate;
    AddInLib.SocketDestroy = (long) &SocketDestroy;
    AddInLib.SocketGetPort = (long) &SocketGetPort;
    AddInLib.SocketGetRevcLen = (long) &SocketGetRevcLen;
    AddInLib.SocketListen = (long) &SocketListen;
    AddInLib.SocketReceiveASCIIDatas = (long) &Wrapper_SocketReceiveASCIIDatas;
    AddInLib.SocketReceiveUnknownDatas = (long) &SocketReceiveUnknownDatas;
    AddInLib.SocketSendASCIIDatasCR = (long) &Wrapper_SocketSendASCIIDatasCR;
    AddInLib.SocketSendUnknownDatas = (long) &SocketSendUnknownDatas;
    AddInLib.StatusBarGetYSize = (long) &StatusBarGetYSize;
    AddInLib.StatusBarSetParts = (long) &StatusBarSetParts;
    AddInLib.StatusBarSetText = (long) &Wrapper_StatusBarSetText;
    AddInLib.StringCount = (long) &Wrapper_StringCount;
    AddInLib.StringReplace = (long) &Wrapper_StringReplace;
    AddInLib.StringGetSplitElement = (long) &Wrapper_StringGetSplitElement;
    AddInLib.StringGetSplitUBound = (long) &StringGetSplitUBound;
    AddInLib.StringHex16 = (long) &Wrapper_StringHex16;
    AddInLib.StringHex32 = (long) &Wrapper_StringHex32;
    AddInLib.StringHex8 = (long) &Wrapper_StringHex8;
    AddInLib.StringIsAlphaChar = (long) &Wrapper_StringIsAlphaChar;
    AddInLib.StringIsArobasChar = (long) &Wrapper_StringIsArobasChar;
    AddInLib.StringIsBlankChar = (long) &Wrapper_StringIsBlankChar;
    AddInLib.StringIsDecimal = (long) &Wrapper_StringIsDecimal;
    AddInLib.StringIsDigitChar = (long) &Wrapper_StringIsDigitChar;
    AddInLib.StringIsDotChar = (long) &Wrapper_StringIsDotChar;
    AddInLib.StringIsCommaChar = (long) &Wrapper_StringIsCommaChar;
    AddInLib.StringIsQuoteChar = (long) &Wrapper_StringIsQuoteChar;
    AddInLib.StringIsEOL = (long) &Wrapper_StringIsEOL;
    AddInLib.StringIsHexaChar = (long) &Wrapper_StringIsHexaChar;
    AddInLib.StringIsHexaDecimal = (long) &Wrapper_StringIsHexaDecimal;
    AddInLib.StringIsLabel = (long) &Wrapper_StringIsLabel;
    AddInLib.StringIsLabelChar = (long) &Wrapper_StringIsLabelChar;
    AddInLib.StringIsOpenBracketChar = (long) &Wrapper_StringIsOpenBracketChar;
    AddInLib.StringIsUnderLineChar = (long) &Wrapper_StringIsUnderLineChar;
    AddInLib.StringJoin = (long) &Wrapper_StringJoin;
    AddInLib.StringNumberComplement = (long) &Wrapper_StringNumberComplement;
    AddInLib.StringHexNumberComplement = (long) &Wrapper_StringHexNumberComplement;
    AddInLib.StringBinNumberComplement = (long) &Wrapper_StringBinNumberComplement;
    AddInLib.StringReleaseSplit = (long) &StringReleaseSplit;
    AddInLib.StringSplit = (long) &Wrapper_StringSplit;
    AddInLib.StringFree = (long) &StringFree;
    AddInLib.StringLen = (long) &StringLen;
    AddInLib.StringCat = (long) &Wrapper_StringCat;
    AddInLib.StringDecToString = (long) &Wrapper_StringDecToString;
    AddInLib.StringStringToDec = (long) &Wrapper_StringStringToDec;
    AddInLib.StringStringToHex = (long) &Wrapper_StringStringToHex;
    AddInLib.StringCountLines = (long) &StringCountLines;
    AddInLib.StringGetLinesOffsets = (long) &StringGetLinesOffsets;
    AddInLib.StringFilterNonPrintableChars = (long) &StringFilterNonPrintableChars;
	AddInLib.StringLeft = (long) &StringLeft;
	AddInLib.StringRight = (long) &StringRight;
	AddInLib.StringTrim = (long) &StringTrim;
	AddInLib.StringLTrim = (long) &StringLTrim;
	AddInLib.StringRTrim = (long) &StringRTrim;
	AddInLib.StringInStr = (long) &StringInStr;
	AddInLib.StringMid = (long) &StringMid;
	AddInLib.StringUpperCase = (long) &StringUpperCase;
	AddInLib.StringLowerCase = (long) &StringLowerCase;
	AddInLib.StringReverse = (long) &StringReverse;
	AddInLib.StringSpace = (long) &StringSpace;
	AddInLib.StringString = (long) &StringString;
	AddInLib.StringIsNumeric = (long) &StringIsNumeric;
	AddInLib.StringFillHexBytes = (long) &StringFillHexBytes;
	AddInLib.StringFillHexWords = (long) &StringFillHexWords;
	AddInLib.StringFillHexDWords = (long) &StringFillHexDWords;
	AddInLib.StringFileNameToLabel = (long) &StringFileNameToLabel;
	AddInLib.WASysTabAddItem = (long) &Wrapper_WASysTabAddItem;
    AddInLib.WASysTabGetCurrentItem = (long) &WASysTabGetCurrentItem;
    AddInLib.WASysTabGetImagelist = (long) &WASysTabGetImagelist;
    AddInLib.WASysTabGetRowsCount = (long) &WASysTabGetRowsCount;
    AddInLib.WASysTabHighLightItem = (long) &WASysTabHighLightItem;
    AddInLib.WASysTabItemsCount = (long) &WASysTabItemsCount;
    AddInLib.WASysTabSetCurrentItem = (long) &WASysTabSetCurrentItem;
    AddInLib.WASysTabSetFocusItem = (long) &WASysTabSetFocusItem;
    AddInLib.WASysTabSetSeparators = (long) &WASysTabSetSeparators;
    AddInLib.WASysTabSetItemsSize = (long) &WASysTabSetItemsSize;
    AddInLib.WASysTabRemoveItem = (long) &WASysTabRemoveItem;
	AddInLib.WASysTabSetMinWidth = (long) &WASysTabSetMinWidth;
    AddInLib.WASysTabSetPadding = (long) &WASysTabSetPadding;
    AddInLib.WATextBoxGetLen = (long) &WATextBoxGetLen;
    AddInLib.WATextBoxAddText = (long) &Wrapper_WATextBoxAddText;
    AddInLib.WATextBoxCanPaste = (long) &WATextBoxCanPaste;
    AddInLib.WATextBoxCanUndo = (long) &WATextBoxCanUndo;
    AddInLib.WATextBoxGetBreakProcAddress = (long) &WATextBoxGetBreakProcAddress;
    AddInLib.WATextBoxGetCurrentOptions = (long) &WATextBoxGetCurrentOptions;
    AddInLib.WATextBoxGetFirstVisibleLine = (long) &WATextBoxGetFirstVisibleLine;
    AddInLib.WATextBoxGetMarginWidth = (long) &WATextBoxGetMarginWidth;
    AddInLib.WATextBoxGetMaxLen = (long) &WATextBoxGetMaxLen;
    AddInLib.WATextBoxGetPasswordChar = (long) &Wrapper_WATextBoxGetPasswordChar;
    AddInLib.WATextBoxSetPasswordChar = (long) &Wrapper_WATextBoxSetPasswordChar;
	AddInLib.WATextBoxGetScrollPos = (long) &WATextBoxGetScrollPos;
    AddInLib.WATextBoxGetSelection = (long) &WATextBoxGetSelection;
    AddInLib.WATextBoxGetTextHandle = (long) &WATextBoxGetTextHandle;
    AddInLib.WATextBoxIsModified = (long) &WATextBoxIsModified;
    AddInLib.WATextBoxLinesCount = (long) &WATextBoxLinesCount;
    AddInLib.WATextBoxRemoveSel = (long) &WATextBoxRemoveSel;
    AddInLib.WATextBoxSelText = (long) &WATextBoxSelText;
    AddInLib.WATextBoxSetCaretPos = (long) &WATextBoxSetCaretPos;
    AddInLib.WATextBoxSetMaxLen = (long) &WATextBoxSetMaxLen;
    AddInLib.WAToolBarAddButton = (long) &Wrapper_WAToolBarAddButton;
    AddInLib.WAToolBarAddSeparator = (long) &WAToolBarAddSeparator;
    AddInLib.WAToolBarDisplayPopupMenu = (long) &WAToolBarDisplayPopupMenu;
    AddInLib.WAToolBarDisplayToolTip = (long) &Wrapper_WAToolBarDisplayToolTip;
    AddInLib.WAToolBarGetButton = (long) &WAToolBarGetButton;
    AddInLib.WAToolBarGetXPadding = (long) &WAToolBarGetXPadding;
    AddInLib.WAToolBarGetYPadding = (long) &WAToolBarGetYPadding;
    AddInLib.WAToolBarGetButtonIndexXSize = (long) &WAToolBarGetButtonIndexXSize;
    AddInLib.WAToolBarGetButtonsCount = (long) &WAToolBarGetButtonsCount;
    AddInLib.WAToolBarGetButtonXPosition = (long) &WAToolBarGetButtonXPosition;
    AddInLib.WAToolBarGetButtonXSize = (long) &WAToolBarGetButtonXSize;
    AddInLib.WAToolBarGetButtonXYPos = (long) &WAToolBarGetButtonXYPos;
    AddInLib.WAToolBarGetButtonYSize = (long) &WAToolBarGetButtonYSize;
    AddInLib.WAToolBarGetImagelist = (long) &WAToolBarGetImagelist;
    AddInLib.WAToolBarGetNotifiedDropDownItem = (long) &WAToolBarGetNotifiedDropDownItem;
    AddInLib.WAToolBarGetNotifiedHotItem = (long) &WAToolBarGetNotifiedHotItem;
    AddInLib.WAToolBarGetNotifiedToolTip = (long) &WAToolBarGetNotifiedToolTip;
    AddInLib.WAToolBarGetRealPos = (long) &WAToolBarGetRealPos;
    AddInLib.WAToolBarGetRowsCount = (long) &WAToolBarGetRowsCount;
    AddInLib.WAToolBarGetXSize = (long) &WAToolBarGetXSize;
    AddInLib.WAToolBarGetXYSize = (long) &WAToolBarGetXYSize;
    AddInLib.WAToolBarGetYSize = (long) &WAToolBarGetYSize;
    AddInLib.WAToolBarIsButtonChecked = (long) &WAToolBarIsButtonChecked;
    AddInLib.WAToolBarIsButtonPressed = (long) &WAToolBarIsButtonPressed;
    AddInLib.WAToolBarResize = (long) &WAToolBarResize;
    AddInLib.WAToolBarSetButtonChecked = (long) &WAToolBarSetButtonChecked;
    AddInLib.WAToolBarSetButtonEnabled = (long) &WAToolBarSetButtonEnabled;
    AddInLib.WAToolBarSetButtonPressed = (long) &WAToolBarSetButtonPressed;
    AddInLib.WAToolBarRemoveButton = (long) &WAToolBarRemoveButton;
    AddInLib.WAToolBarSetButtonBitmap = (long) &WAToolBarSetButtonBitmap;
    AddInLib.WATrackBarGetPos = (long) &WATrackBarGetPos;
    AddInLib.WATrackBarSetPos = (long) &WATrackBarSetPos;
    AddInLib.WATrackBarSetRange = (long) &WATrackBarSetRange;
    AddInLib.WATrackBarSetTicks = (long) &WATrackBarSetTicks;
    AddInLib.WATrackBarGetToolTips = (long) &WATrackBarGetToolTips;
    AddInLib.WATreeViewAddItem = (long) &Wrapper_WATreeViewAddItem;
    AddInLib.WATreeViewGetBackColor = (long) &WATreeViewGetBackColor;
    AddInLib.WATreeViewGetChildItemsCount = (long) &WATreeViewGetChildItemsCount;
    AddInLib.WATreeViewGetDeletedItem = (long) &WATreeViewGetDeletedItem;
    AddInLib.WATreeViewGetEditControl = (long) &WATreeViewGetEditControl;
    AddInLib.WATreeViewGetExpandingItem = (long) &WATreeViewGetExpandingItem;
    AddInLib.WATreeViewGetExpandingState = (long) &WATreeViewGetExpandingState;
    AddInLib.WATreeViewGetFirstItemChild = (long) &WATreeViewGetFirstItemChild;
    AddInLib.WATreeViewGetImageList = (long) &WATreeViewGetImageList;
    AddInLib.WATreeViewGetIndent = (long) &WATreeViewGetIndent;
    AddInLib.WATreeViewGetInsertMarkColor = (long) &WATreeViewGetInsertMarkColor;
    AddInLib.WATreeViewGetItemCount = (long) &WATreeViewGetItemCount;
    AddInLib.WATreeViewGetItemFromPos = (long) &WATreeViewGetItemFromPos;
    AddInLib.WATreeViewGetItemHeight = (long) &WATreeViewGetItemHeight;
    AddInLib.WATreeViewGetItemLevel = (long) &WATreeViewGetItemLevel;
    AddInLib.WATreeViewGetItemParent = (long) &WATreeViewGetItemParent;
    AddInLib.WATreeViewGetItemText = (long) &Wrapper_WATreeViewGetItemText;
    AddInLib.WATreeViewGetLineColor = (long) &WATreeViewGetLineColor;
    AddInLib.WATreeViewGetNextItem = (long) &WATreeViewGetNextItem;
    AddInLib.WATreeViewGetPreviousItem = (long) &WATreeViewGetPreviousItem;
    AddInLib.WATreeViewGetRoot = (long) &WATreeViewGetRoot;
    AddInLib.WATreeViewGetScrollTime = (long) &WATreeViewGetScrollTime;
    AddInLib.WATreeViewGetSelChildItemPos = (long) &WATreeViewGetSelChildItemPos;
    AddInLib.WATreeViewGetSelectedItem = (long) &WATreeViewGetSelectedItem;
    AddInLib.WATreeViewGetTextColor = (long) &WATreeViewGetTextColor;
    AddInLib.WATreeViewGetToolTips = (long) &WATreeViewGetToolTips;
    AddInLib.WATreeViewGetVisibleCount = (long) &WATreeViewGetVisibleCount;
    AddInLib.WATreeViewRemoveItem = (long) &WATreeViewRemoveItem;
    AddInLib.WATreeViewSearchChildPartialText = (long) &Wrapper_WATreeViewSearchChildPartialText;
    AddInLib.WATreeViewSearchItemText = (long) &Wrapper_WATreeViewSearchItemText;
    AddInLib.WATreeViewSetBackColor = (long) &WATreeViewSetBackColor;
    AddInLib.WATreeViewSetIndent = (long) &WATreeViewSetIndent;
    AddInLib.WATreeViewSetItemExpandedState = (long) &WATreeViewSetItemExpandedState;
    AddInLib.WATreeViewGetItemExpandedState = (long) &WATreeViewGetItemExpandedState;
    AddInLib.WATreeViewSetSingleExpandState = (long) &WATreeViewSetSingleExpandState;
	AddInLib.WATreeViewSetItemIcon = (long) &WATreeViewSetItemIcon;
    AddInLib.WATreeViewSetItemText = (long) &Wrapper_WATreeViewSetItemText;
    AddInLib.WATreeViewSetSelectedItem = (long) &WATreeViewSetSelectedItem;
    AddInLib.WATreeViewSetToolTips = (long) &WATreeViewSetToolTips;
    AddInLib.WATreeViewGetItemTextRect = (long) &WATreeViewGetItemTextRect;
    AddInLib.WATreeViewGetItemRect = (long) &WATreeViewGetItemRect;
    AddInLib.WACreateDockingBox = (long) &Wrapper_WACreateDockingBox;
    AddInLib.DockingBoxGetState = (long) &DockingBoxGetState;
    AddInLib.DockingBoxRemove = (long) &DockingBoxRemove;
    AddInLib.DockingBoxShow = (long) &Wrapper_DockingBoxShow;
    AddInLib.DockingBoxWasVisible = (long) &Wrapper_DockingBoxWasVisible;
	AddInLib.WinsockInit = (long) &WinsockInit;
    AddInLib.WinsockUnInit = (long) &WinsockUnInit;
    AddInLib.WAMiscClipBoardCopyText = (long) &Wrapper_WAMiscClipBoardCopyText;
    AddInLib.WAMiscClipBoardPasteText = (long) &Wrapper_WAMiscClipBoardPasteText;
    AddInLib.WAMiscClipBoardIsEmpty = (long) &WAMiscClipBoardIsEmpty;
    AddInLib.WAMiscDoEvents = (long) &WAMiscDoEvents;
    AddInLib.WAMiscMsgBox = (long) &Wrapper_WAMiscMsgBox;
    AddInLib.WAMiscSendBroadCastMsg = (long) &WAMiscSendBroadCastMsg;
    AddInLib.WAMiscWaitEvents = (long) &WAMiscWaitEvents;
    AddInLib.WAMiscObtainGUID = (long) &WAMiscObtainGUID;
    AddInLib.WAMiscAllocMem = (long) &WAMiscAllocMem;
    AddInLib.WAMiscFreeMem = (long) &WAMiscFreeMem;
    AddInLib.WAMiscCopyMemLoop = (long) &WAMiscCopyMemLoop;
    AddInLib.WAMiscCopyMemWithOffsets = (long) &WAMiscCopyMemWithOffsets;
    AddInLib.WAMiscShellAddFileToRecents = (long) &Wrapper_WAMiscShellAddFileToRecents;
    AddInLib.WAMiscGetOSClass = (long) &WAMiscGetOSClass;
	AddInLib.WAMiscObtainExtendedFunctions = (long) &WAMiscObtainExtendedFunctions;
    AddInLib.WAMMGetWindowLangGUID = (long) &WAMMGetWindowLangGUID;
    AddInLib.WAMMGetWindowLangInclude = (long) &WAMMGetWindowLangInclude;
    AddInLib.WAMMGetWindowLangDoubleSlash = (long) &WAMMGetWindowLangDoubleSlash;
    AddInLib.WAMMGetWindowLangComment = (long) &WAMMGetWindowLangComment;
    AddInLib.WAMMGetWindowLangMultiComment = (long) &WAMMGetWindowLangMultiComment;
    AddInLib.WAMMGetWindowLangMultiCommentEnd = (long) &WAMMGetWindowLangMultiCommentEnd;
    AddInLib.WAMMGetWindowLangProc = (long) &WAMMGetWindowLangProc;
    AddInLib.WAMMGetWindowLangEndp = (long) &WAMMGetWindowLangEndp;
    AddInLib.WAMMGetWindowLangProcInterlace = (long) &WAMMGetWindowLangProcInterlace;
    AddInLib.WAMMGetWindowLangProcOrphan = (long) &WAMMGetWindowLangProcOrphan;
    AddInLib.WAMMGetWindowLangProcOpenBlock = (long) &WAMMGetWindowLangProcOpenBlock;
    AddInLib.WAMMGetWindowLangProcCloseBlock = (long) &WAMMGetWindowLangProcCloseBlock;
    AddInLib.WAMMGetWindowLangFileName = (long) &WAMMGetWindowLangFileName;
    AddInLib.WAMMGetWindowLangIncludeName = (long) &WAMMGetWindowLangIncludeName;
    AddInLib.WAMMGetWindowLangIncludeNameFoot = (long) &WAMMGetWindowLangIncludeNameFoot;
    AddInLib.WAMMDecodeRelativePaths = (long) &WAMMDecodeRelativePaths;
    AddInLib.WAMMRemoveRelativePaths = (long) &WAMMRemoveRelativePaths;
    AddInLib.WAMMEncodeRelativePaths = (long) &WAMMEncodeRelativePaths;
    AddInLib.WAMMRunUserCommand = (long) &WAMMRunUserCommand;
    AddInLib.WAMMRunCommandScript = (long) &WAMMRunCommandScript;

    AddInLib.WAComDlgChooseColor = (long) &Wrapper_WAComDlgChooseColor;
    AddInLib.WAComDlgGetOpenFileName = (long) &Wrapper_WAComDlgGetOpenFileName;
    AddInLib.WAComDlgGetSaveFileName = (long) &Wrapper_WAComDlgGetSaveFileName;
    AddInLib.WAComDlgChooseFont = (long) &Wrapper_WAComDlgChooseFont;
    AddInLib.WAComDlgBrowseForFolder = (long) &Wrapper_WAComDlgBrowseForFolder;
    AddInLib.WAComDlgParseMultiFilesSelection = (long) &Wrapper_WAComDlgParseMultiFilesSelection;

    AddInLib.WAMMDisplayHelpFile = (long) &WAMMDisplayHelpFile;
    AddInLib.WAMMRunDOSCmd = (long) &WAMMRunDOSCmd;
    AddInLib.WAMMRunDOSCmdCapture = (long) &WAMMRunDOSCmdCapture;
    AddInLib.WAMMGetWindowPath = (long) &WAMMGetWindowPath;
    AddInLib.WAMMIsFileWorkSpace = (long) &WAMMIsFileWorkSpace;
    AddInLib.WAMMIsFileProject = (long) &WAMMIsFileProject;
    AddInLib.WAMMGetCurrentChildState = (long) &WAMMGetCurrentChildState;
    AddInLib.GotoFileLine = (long) &GotoFileLine;
    AddInLib.WAMMFocusOnFile = (long) &WAMMFocusOnFile;
    AddInLib.WAMMGetFileNameFromTreeViewItem = (long) &WAMMGetFileNameFromTreeViewItem;
    AddInLib.WAMMGetLabelFromTreeViewItem = (long) &WAMMGetLabelFromTreeViewItem;
    AddInLib.WAMMOpenUnknownFile = (long) &WAMMOpenUnknownFile;
    AddInLib.WAMMHexToDec = (long) &WAMMHexToDec;
    AddInLib.WAMMDecToHex = (long) &WAMMDecToHex;
    AddInLib.WAMMDecToBin = (long) &WAMMDecToBin;
    AddInLib.WAMMHexToBin = (long) &WAMMHexToBin;
    AddInLib.WAMMBinToDec = (long) &WAMMBinToDec;
    AddInLib.WAMMBinToHex = (long) &WAMMBinToHex;
    AddInLib.WAMMHexToFlt = (long) &WAMMHexToFlt;
    AddInLib.WAMMFltToHex = (long) &WAMMFltToHex;
    AddInLib.WAMMGetProceduresList = (long) &WAMMGetProceduresList;
    AddInLib.WAMMGetLineFromFile = (long) &WAMMGetLineFromFile;
    AddInLib.WAMMCreateNewWindow = (long) &WAMMCreateNewWindow;
    AddInLib.WAMMClearWindow = (long) &WAMMClearWindow;
    AddInLib.WAMMReloadWindow = (long) &WAMMReloadWindow;
    AddInLib.WAMMCloseWindow = (long) &WAMMCloseWindow;
    AddInLib.WAMMCloseAllWindows = (long) &WAMMCloseAllWindows;
    AddInLib.WAMMSaveAllWindows = (long) &WAMMSaveAllWindows;
    AddInLib.WAMMPrintWindow = (long) &WAMMPrintWindow;
    AddInLib.WAMMSaveWindow = (long) &WAMMSaveWindow;
    AddInLib.WAMMUndo = (long) &WAMMUndo;
    AddInLib.WAMMRedo = (long) &WAMMRedo;
    AddInLib.WAMMCut = (long) &WAMMCut;
    AddInLib.WAMMCopy = (long) &WAMMCopy;
    AddInLib.WAMMCopyFilePathName = (long) &WAMMCopyFilePathName;
    AddInLib.WAMMCopyEntireText = (long) &WAMMCopyEntireText;
    AddInLib.WAMMPaste = (long) &WAMMPaste;
    AddInLib.WAMMDeleteSel = (long) &WAMMDeleteSel;
    AddInLib.WAMMDeleteLine = (long) &WAMMDeleteLine;
    AddInLib.WAMMDeleteToStart = (long) &WAMMDeleteToStart;
    AddInLib.WAMMDeleteToEnd = (long) &WAMMDeleteToEnd;
    AddInLib.WAMMSelectLine = (long) &WAMMSelectLine;
    AddInLib.WAMMSelectAll = (long) &WAMMSelectAll;
    AddInLib.WAMMSelectProc = (long) &WAMMSelectProc;
    AddInLib.WAMMFindNext = (long) &WAMMFindNext;
    AddInLib.WAMMFindPrevious = (long) &WAMMFindPrevious;
    AddInLib.WAMMFindSelection = (long) &WAMMFindSelection;
    AddInLib.GotoTop = (long) &GotoTop;
    AddInLib.GotoBottom = (long) &GotoBottom;
    AddInLib.GotoLine = (long) &GotoLine;
    AddInLib.GotoMatchingBracket = (long) &GotoMatchingBracket;
    AddInLib.GotoPrevWord = (long) &GotoPrevWord;
    AddInLib.GotoNextWord = (long) &GotoNextWord;
    AddInLib.GotoPrevLetter = (long) &GotoPrevLetter;
    AddInLib.GotoNextLetter = (long) &GotoNextLetter;
    AddInLib.GotoPrevLine = (long) &GotoPrevLine;
    AddInLib.GotoNextLine = (long) &GotoNextLine;
    AddInLib.GotoPrevPage = (long) &GotoPrevPage;
    AddInLib.GotoNextPage = (long) &GotoNextPage;
    AddInLib.WAMMGetMiniChildStruct = (long) &WAMMGetMiniChildStruct;
    AddInLib.WAMMGetIniFilePathName = (long) &WAMMGetIniFilePathName;
    AddInLib.WAMMGetQuickDirectory = (long) &WAMMGetQuickDirectory;
    AddInLib.WAMMGetContext = (long) &WAMMGetContext;
    AddInLib.WAMMGetcurrentLine = (long) &WAMMGetcurrentLine;
    AddInLib.WAMMSelectCurrentWord = (long) &WAMMSelectCurrentWord;
    AddInLib.WAMMGetCurrentWord = (long) &WAMMGetCurrentWord;
    AddInLib.InsertText = (long) &InsertText;
    AddInLib.WAMMInsertLine = (long) &WAMMInsertLine;
    AddInLib.WAMMInsertLines = (long) &WAMMInsertLines;
    AddInLib.WAMMSetText = (long) &WAMMSetText;
    AddInLib.WAMMGetCaretRealXCoordinate = (long) &WAMMGetCaretRealXCoordinate;
    AddInLib.WAMMGetCaretRealYCoordinate = (long) &WAMMGetCaretRealYCoordinate;
    AddInLib.WAMMSetFocusInsideWindow = (long) &WAMMSetFocusInsideWindow;
    AddInLib.WAMMRestoreMDIFromSysTray = (long) &WAMMRestoreMDIFromSysTray;
    AddInLib.WAMMInstallHookProc = (long) &WAMMInstallHookProc;
    AddInLib.WAMMUnInstallHookProc = (long) &WAMMUnInstallHookProc;
    AddInLib.WAMMGetCurrentLineNumber = (long) &WAMMGetCurrentLineNumber;
    AddInLib.WAMMGetCurrentColNumber = (long) &WAMMGetCurrentColNumber;
    AddInLib.WAMMGetCurrentLineText = (long) &WAMMGetCurrentLineText;
    AddInLib.WAMMGetLineText = (long) &WAMMGetLineText;
    AddInLib.WAMMWriteToOutputBox = (long) &WAMMWriteToOutputBox;
    AddInLib.WAMMEnumSourceWindows = (long) &WAMMEnumSourceWindows;
    AddInLib.WAMMAddInKillZombie = (long) &WAMMAddInKillZombie;
    AddInLib.WAMMSpAddInIsAvailable = (long) &WAMMSpAddInIsAvailable;
    AddInLib.WAMMMiscInputBox = (long) &WAMMMiscInputBox;
    AddInLib.WAMMIsChildReadOnly = (long) &IsChildReadOnly;
    AddInLib.WAMMSetChildReadOnly = (long) &SetChildReadOnly;
    AddInLib.WAMMCreateCustomChildDialog = (long) &Wrapper_WAMMCreateCustomChildDialog;
    AddInLib.WAMMCreateCodeMax = (long) &WAMMCreateCodeMax;
}

// -----------------------------------------------------------------------
// Create a stock dialog
long CALLBACK Wrapper_WACreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight,
                                               HWND hParent, char *DTitle, DLGPROC WinProc, long Centered)
{
	int RetValue = 0;

	// We need to freeze our internal timer
    FreezeTimer = 1;
	switch(TemplateNumber)
	{
		case MODALDLG_STOCK_WIZARD:
			RetValue = WACreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, (long) MAKEINTRESOURCE(MBMP_BASE + MBMP_WIZARD));
			break;
		default:
			RetValue = WACreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, 0);
			break;
	}
	// And restart it
    FreezeTimer = 0;
	return(RetValue);
}

// -----------------------------------------------------------------------
// Chromatic AddIns Extra Services
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Get GUID from window
char * CALLBACK WAMMGetWindowLangGUID(HWND hForm)
{
    CStr ReturnValue;

	if(hForm != 0) ReturnValue = GetCMLangGUID(hForm);
	return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get Include from window
char * CALLBACK WAMMGetWindowLangInclude(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangInclude(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get Double slash from window
char * CALLBACK WAMMGetWindowLangDoubleSlash(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangDoubleSlash(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get comment from window 
char * CALLBACK WAMMGetWindowLangComment(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangComment(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get multi-comment from window
char * CALLBACK WAMMGetWindowLangMultiComment(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangMultiComment(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get multi-comment end from window
char * CALLBACK WAMMGetWindowLangMultiCommentEnd(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangMultiCommentEnd(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get proc from window
char * CALLBACK WAMMGetWindowLangProc(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangProc(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get endp from window
char * CALLBACK WAMMGetWindowLangEndp(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangEndp(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get interlace from window
long CALLBACK WAMMGetWindowLangProcInterlace(HWND hForm)
{
    if(hForm != 0) return(GetCMLangProcInterlace(hForm));
	else return(0);
}

// -----------------------------------------------------------------------
// Get orphan from window
long CALLBACK WAMMGetWindowLangProcOrphan(HWND hForm)
{
    if(hForm != 0) return(GetCMLangProcOrphan(hForm));
	else return(0);
}

// -----------------------------------------------------------------------
// Get openblock from window
char * CALLBACK WAMMGetWindowLangProcOpenBlock(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) ReturnValue = GetCMLangProcOpenBlock(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get closeblock from window
char * CALLBACK WAMMGetWindowLangProcCloseBlock(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) return(StringCStrConvertToString(GetCMLangProcCloseBlock(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get closeblock from window
char * CALLBACK WAMMGetWindowLangFileName(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) return(StringCStrConvertToString(GetCMLangFileName(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get include name from window
char * CALLBACK WAMMGetWindowLangIncludeName(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) return(StringCStrConvertToString(GetCMLangIncludeNameByhWnd(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get include name from window
char * CALLBACK WAMMGetWindowLangIncludeNameFoot(HWND hForm)
{
    CStr ReturnValue;
    
	if(hForm != 0) return(StringCStrConvertToString(GetCMLangIncludeNameFootByhWnd(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Decode relative paths
char * CALLBACK WAMMDecodeRelativePaths(char *PathName)
{
    CStr ReturnValue;
    
	ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = ChangeRelativePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Remove relative paths
char * CALLBACK WAMMRemoveRelativePaths(char *PathName)
{
    CStr ReturnValue;
    
	ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = RemoveRelativePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Encode relative paths
char * CALLBACK WAMMEncodeRelativePaths(char *PathName)
{
    CStr ReturnValue;

    ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = ChangeAbsolutePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Run an user command
void CALLBACK WAMMRunUserCommand(long CmdNumber)
{
    RunUserCommand(CmdNumber);
}

// -----------------------------------------------------------------------
// Run a script command
void CALLBACK WAMMRunCommandScript(char *CommandScript, long SaveFiles)
{
    CStr TmpCom;
    
	FRMArgsEntered = 0;
    if(SaveFiles == 1) MCMD_SaveAll();
    TmpCom = TranslateUserCmd(StringConvertToCStr(CommandScript));
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        return;
    }
    TmpCom = StringReplace(TmpCom, "\t", " ", 1, -1, Binary_Compare);
    Exec(&TmpCom, StringReplace(TmpCom, "$", " ", 1, -1, Binary_Compare), 0);
}

// -----------------------------------------------------------------------
// Display a color selector
long CALLBACK Wrapper_WAComDlgChooseColor(HWND hWnd, long *ColorToGet)
{
    long ReturnValue = 0;

    if(WAComDlgChooseColor(hWnd, ColorToGet[0]) != 0)
    {
        ColorToGet[0] = MyColor.rgbResult;
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display an open file selector
char * CALLBACK Wrapper_WAComDlgGetOpenFileName(HWND hWnd, char *FilesFilter, char *PrimaryDir, long MultiSelect)
{
    CStr LdFile;
    CStr ReturnValue;

    LdFile = WAComDlgGetOpenFileName(hWnd, StringConvertToCStr(FilesFilter), StringConvertToCStr(PrimaryDir), MultiSelect, CurrentDir);
    if(LdFile.Len() != 0) ReturnValue.Set_String(LdFile.Get_String());
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Display a multi files selector
char * CALLBACK Wrapper_WAComDlgParseMultiFilesSelection(char *SelectedFiles, long (CALLBACK *EnumProc)(char *, long),
                                                         long SearchDirection, long UserValue)
{
	return(StringCStrConvertToString(WAComDlgParseMultiFilesSelection(StringConvertToCStr(SelectedFiles), EnumProc, SearchDirection, UserValue)));
}

// -----------------------------------------------------------------------
// Display a save file selector
char * CALLBACK Wrapper_WAComDlgGetSaveFileName(HWND hWnd, char *FilesFilter, char *PrimaryDir)
{
    CStr LdFile;
    CStr ReturnValue;

    LdFile = WAComDlgGetSaveFileName(hWnd, StringConvertToCStr(FilesFilter), StringConvertToCStr(PrimaryDir), CurrentDir);
    if(LdFile.Len() != 0) ReturnValue = LdFile;
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Choose a font
long CALLBACK Wrapper_WAComDlgChooseFont(HWND hWnd, char *FntName, long FntSize,
                                         long FixedOnly, LPLOGFONT WALogFont,
                                         LPCHOOSEFONT WAMyFont)
{
    int i = 0;
    HDC FhDC = 0;
    CStr LocFntName;
	long ReturnValue = 0;

    LocFntName = StringConvertToCStr(FntName);
    FhDC = GetDC(hWnd);
    WALogFont->lfHeight = -((long) (FntSize * GetDeviceCaps(FhDC, LOGPIXELSY)) / 72);
    WALogFont->lfWidth = 0;
    WALogFont->lfEscapement = 0;
    WALogFont->lfOrientation = 0;
    WALogFont->lfWeight = FW_NORMAL;
    WALogFont->lfItalic = 0;
    WALogFont->lfUnderline = 0;
    WALogFont->lfStrikeOut = 0;
    WALogFont->lfCharSet = DEFAULT_CHARSET;
    WALogFont->lfOutPrecision = OUT_DEFAULT_PRECIS;
    WALogFont->lfClipPrecision = CLIP_DEFAULT_PRECIS;
    WALogFont->lfQuality = DEFAULT_QUALITY;
    WALogFont->lfPitchAndFamily = DEFAULT_PITCH;
    for(i = 0; i <= LF_FACESIZE - 1; i++)
    {
        if(i == LocFntName.Len()) break;
        WALogFont->lfFaceName[i] = LocFntName.Mid(i + 1, 1).Asc();
    }
    WALogFont->lfFaceName[i] = 0;
    
    hMemFont = calloc(sizeof(MyLogFont), 1);
    if(hMemFont)
    {
        RtlCopyMemory(hMemFont, WALogFont, sizeof(MyLogFont));
        WAMyFont->lStructSize = sizeof(MyFont);
        WAMyFont->hwndOwner = hWnd;
        WAMyFont->lpLogFont = (LPLOGFONT) hMemFont;
        WAMyFont->iPointSize = FntSize * 10;
        if(FixedOnly == 1) WAMyFont->Flags = CF_FIXEDPITCHONLY | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
        else WAMyFont->Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
        WAMyFont->rgbColors = 0;
        WAMyFont->lCustData = 0;
        WAMyFont->lpfnHook = 0;
        WAMyFont->lpTemplateName = "";
        WAMyFont->hInstance = 0;
        WAMyFont->lpszStyle = "";
        WAMyFont->nFontType = REGULAR_FONTTYPE;
        WAMyFont->nSizeMin = 8;
        WAMyFont->nSizeMax = 72;
        ReturnValue = ChooseFont(WAMyFont);
        RtlCopyMemory(WALogFont, (void *) hMemFont, sizeof(MyLogFont));
        free(hMemFont);
    }
    ReleaseDC(hWnd, FhDC);
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Browse directories
char * CALLBACK Wrapper_WAComDlgBrowseForFolder(HWND hParent, char *BrowseTitle)
{
	CStr ReturnValue;

    ReturnValue = WAComDlgBrowseForFolder(hParent, StringConvertToCStr(BrowseTitle));
    if(ReturnValue.Len() == 0)
    {
        return(0);
    }
    else
    {
        return(StringCStrConvertToString(ReturnValue));
    }
}

// -----------------------------------------------------------------------
// Load a .hlp/.chm/.col file
long CALLBACK WAMMDisplayHelpFile(char *HelpFile, char *HelpWord)
{
    return(OpenContextHelpFile(StringConvertToCStr(HelpFile), StringConvertToCStr(HelpWord)));
}

// -----------------------------------------------------------------------
// Run a DOS command
long CALLBACK WAMMRunDOSCmd(char *CmdName, char *CmdArgs, char *CmdDir, char *CmdOutputFile)
{
    return(RunDOSCmd(StringConvertToCStr(CmdName), StringConvertToCStr(CmdArgs), StringConvertToCStr(CmdDir),
                     StringConvertToCStr(CmdOutputFile)));
}

// -----------------------------------------------------------------------
// Run a DOS command and capture output
void CALLBACK WAMMRunDOSCmdCapture(char *CmdName, char *CmdArgs, char *CmdDir)
{
    CaptureDOSOutput(StringConvertToCStr(CmdName), StringConvertToCStr(CmdArgs), StringConvertToCStr(CmdDir));
}

// -----------------------------------------------------------------------
// Focus on a CodeMax control
void CALLBACK WAMMSetFocusInsideWindow(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    SetFocus(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Retrieve the path of the current child window
char * CALLBACK WAMMGetWindowPath(void)
{
    return(StringCStrConvertToString(GetCurrentFileDir()));
}

// -----------------------------------------------------------------------
// Check if a given filename is a workspace
long CALLBACK WAMMIsFileWorkSpace(char *FileName)
{
    return(CheckWorkSpaceFile(StringConvertToCStr(FileName)));
}

// -----------------------------------------------------------------------
// Check if a given filename is a project
long CALLBACK WAMMIsFileProject(char *FileName)
{
    return(CheckProjectFile(StringConvertToCStr(FileName)));
}

// -----------------------------------------------------------------------
// Retrieve the current state of the current child window
long CALLBACK WAMMGetCurrentChildState(void)
{
    return(GetChildState());
}

// -----------------------------------------------------------------------
// Goto a line number of a given filename
HWND CALLBACK GotoFileLine(char *FileName, long LineNbr, long AddInRecents)
{
    return(GoToLine(StringConvertToCStr(FileName), LineNbr, AddInRecents));
}

// -----------------------------------------------------------------------
// Focus on a given file
void CALLBACK WAMMFocusOnFile(char *FileName, long AddInRecents)
{
    FocusFile(StringConvertToCStr(FileName), AddInRecents);
}

// -----------------------------------------------------------------------
// Retrieve a filename from a treeview item
char * CALLBACK WAMMGetFileNameFromTreeViewItem(HTREEITEM hItem)
{
    return(StringCStrConvertToString(GetFileNameFromTreeView(hItem)));
}

// -----------------------------------------------------------------------
// Retrieve a label from a treeview item
char * CALLBACK WAMMGetLabelFromTreeViewItem(HTREEITEM hItem)
{
    return(StringCStrConvertToString(GetLabelFromTreeView(hItem)));
}

// -----------------------------------------------------------------------
// Open an alien file
long CALLBACK WAMMOpenUnknownFile(char *FileName, int AddInRecent)
{
    return(OpenUnknownFile(StringConvertToCStr(FileName), AddInRecent));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into decimal
char * CALLBACK WAMMHexToDec(char *HexNumber)
{
    return(StringCStrConvertToString(HexToDec(StringConvertToCStr(HexNumber))));
}

// -----------------------------------------------------------------------
// Convert a decimal number into hexadecimal
char * CALLBACK WAMMDecToHex(char *DecNumber)
{
    return(StringCStrConvertToString(DecToHex(StringConvertToCStr(DecNumber))));
}

// -----------------------------------------------------------------------
// Convert a decimal number into binary
char * CALLBACK WAMMDecToBin(char *DecNumber)
{
    return(StringCStrConvertToString(DecToBin(StringConvertToCStr(DecNumber))));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into binary
char * CALLBACK WAMMHexToBin(char *HexNumber)
{
    return(StringCStrConvertToString(HexToBin(StringConvertToCStr(HexNumber))));
}

// -----------------------------------------------------------------------
// Convert a binary number into decimal
char * CALLBACK WAMMBinToDec(char *BinNumber)
{
    return(StringCStrConvertToString(BinToDec(StringConvertToCStr(BinNumber))));
}

// -----------------------------------------------------------------------
// Convert a binary number into hexadecimal
char * CALLBACK WAMMBinToHex(char *BinNumber)
{
    return(StringCStrConvertToString(BinToHex(StringConvertToCStr(BinNumber))));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into a floating point
char * CALLBACK WAMMHexToFlt(char *BinNumber)
{
    return(StringCStrConvertToString(HexToFlt(StringConvertToCStr(BinNumber))));
}

// -----------------------------------------------------------------------
// Convert a floating point number into hexadecimal
char * CALLBACK WAMMFltToHex(char *BinNumber)
{
    return(StringCStrConvertToString(FltToHex(StringConvertToCStr(BinNumber))));
}

// -----------------------------------------------------------------------
// Retrieve the procedure list contained in a file
char * CALLBACK WAMMGetProceduresList(char *FileName)
{
    return(StringCStrConvertToString(GetProcedureList(StringConvertToCStr(FileName))));
}

// -----------------------------------------------------------------------
// Retrieve a text line from a file
char * CALLBACK WAMMGetLineFromFile(HANDLE hFile, long LineNumber)
{
    CStr ReturnValue;
    long FlSize = 0;

    if(hFile != 0)
    {
        FlSize = GetFileSize(hFile, NULL);
        ReturnValue = GetTxtLineFromFile((long) hFile, FlSize, LineNumber);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Create a new window
HWND CALLBACK WAMMCreateNewWindow(void)
{
    return(MCMD_New());
}

// -----------------------------------------------------------------------
// Clear a window
void CALLBACK WAMMClearWindow(HWND hForm)
{
    if(hForm != 0) ClearFile(hForm);
}

// -----------------------------------------------------------------------
// Reload a window
void CALLBACK WAMMReloadWindow(HWND hForm)
{
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(ChildStruct->FileLoaded == 1)
        {
            ClearFile(hForm);
            LoadFile(hForm);
        }
    }
}

// -----------------------------------------------------------------------
// Close a window
void CALLBACK WAMMCloseWindow(HWND hForm)
{
    if(NbForms != 0) if(hForm != 0) WAControlClose(hForm);
    if(NbForms != 0) WAClientSetPreviousChild(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// Close all windows
void CALLBACK WAMMCloseAllWindows(void)
{
    MCMD_CloseAll();
}

// -----------------------------------------------------------------------
// Save all windows
void CALLBACK WAMMSaveAllWindows(void)
{
    MCMD_SaveAll();
}
    
// -----------------------------------------------------------------------
// Print a window
void CALLBACK WAMMPrintWindow(HWND hForm, long PrintSelection)
{
    if(NbForms != 0) if(hForm != 0) PrintFile(hForm, PrintSelection);
}

// -----------------------------------------------------------------------
// Save a window
char * CALLBACK WAMMSaveWindow(HWND hForm)
{
    CStr ReturnValue;

    if(NbForms != 0) if(hForm != 0) ReturnValue = SaveIt(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Undo typing
void CALLBACK WAMMUndo(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Undo(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Redo typing
void CALLBACK WAMMRedo(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Redo(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Cut command
void CALLBACK WAMMCut(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Cut(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Copy command
void CALLBACK WAMMCopy(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Copy(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Copy filepathname command
long CALLBACK WAMMCopyFilePathName(HWND hForm)
{
	long ReturnValue = 0;

    if(NbForms == 0) return(0);
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(ChildStruct->FileLoaded == 0) return(ReturnValue);
        ReturnValue = WAMiscClipBoardCopyText(CMGetRealFile(ChildStruct->RFile));
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Copy entire text command
long CALLBACK WAMMCopyEntireText(HWND hForm)
{
    CStr EntText;
	long ReturnValue = 0;

    if(NbForms == 0) return(0);
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        EntText = EntText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
        CM_GetTextAll(ChildStruct->hChildCodeMax, EntText.Get_String());
        ReturnValue = WAMiscClipBoardCopyText(EntText);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Paste command
void CALLBACK WAMMPaste(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Paste(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Delete selection command
void CALLBACK WAMMDeleteSel(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_DeleteSel(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Delete line command
void CALLBACK WAMMDeleteLine(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        LoadCurrentSel(ChildStruct->hChildCodeMax);
        CM_DeleteLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine);
    }
}

// -----------------------------------------------------------------------
// Delete to start of line command
void CALLBACK WAMMDeleteToStart(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOSTART, -1);
    }
}

// -----------------------------------------------------------------------
// Delete to end of line command
void CALLBACK WAMMDeleteToEnd(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOEND, 0);
    }
}

// -----------------------------------------------------------------------
// Select line command
void CALLBACK WAMMSelectLine(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTLINE, 0);
    }
}

// -----------------------------------------------------------------------
// Select all command
void CALLBACK WAMMSelectAll(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTALL, 0);
    }
}

// -----------------------------------------------------------------------
// Select a procedure in current window
long CALLBACK WAMMSelectProc(HWND hForm)
{
    if(NbForms == 0) if(hForm != 0) return(SelectProcedure(hForm));
	return(0);
}

// -----------------------------------------------------------------------
// Find next
void CALLBACK WAMMFindNext(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
    }
}

// -----------------------------------------------------------------------
// find previous
void CALLBACK WAMMFindPrevious(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDPREV, 0);
    }
}

// -----------------------------------------------------------------------
// Find selection
void CALLBACK WAMMFindSelection(HWND hForm)
{
    CStr TxtToFind;

    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(GetCurrentLogicalSelLen(ChildStruct->hChildCodeMax) == 0)
        {
            TxtToFind = GetCurrentWord(ChildStruct->hChildCodeMax);
        }
        else
        {
            TxtToFind = GetCurrentLogicalSelText(ChildStruct->hChildCodeMax);
        }
        CM_SetFindText(ChildStruct->hChildCodeMax, TxtToFind.Get_String());
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
    }
}

// -----------------------------------------------------------------------
// Goto top
void CALLBACK GotoTop(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
    }
}

// -----------------------------------------------------------------------
// Goto bottom
void CALLBACK GotoBottom(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, CM_GetLineCount(ChildStruct->hChildCodeMax) - 1);
    }
}

// -----------------------------------------------------------------------
// Goto line
void CALLBACK GotoLine(HWND hForm, long LineNumber)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, LineNumber);
    }
}

// -----------------------------------------------------------------------
// Goto matching bracket
void CALLBACK GotoMatchingBracket(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOMATCHBRACE, 0);
    }
}

// -----------------------------------------------------------------------
// Goto previous word
void CALLBACK GotoPrevWord(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDLEFTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next word
void CALLBACK GotoNextWord(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDRIGHT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDRIGHTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous letter
void CALLBACK GotoPrevLetter(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARLEFTEXTEND, 0);
        }
    }
} 

// -----------------------------------------------------------------------
// Goto next letter
void CALLBACK GotoNextLetter(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARRIGHT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARRIGHTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous line
void CALLBACK GotoPrevLine(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEUP, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEUPEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next line
void CALLBACK GotoNextLine(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDOWN, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDOWNEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous page
void CALLBACK GotoPrevPage(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEUP, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEUPEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next page
void CALLBACK GotoNextPage(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEDOWN, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEDOWNEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Get Mini child structure
void CALLBACK WAMMGetMiniChildStruct(HWND hForm, LPMINICHILDDAT MiniChildStruct)
{
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        MiniChildStruct->hChildCodeMax = ChildStruct->hChildCodeMax;
        MiniChildStruct->FileLoaded = ChildStruct->FileLoaded;
        if(ChildStruct->Language->Len() == 0)
        {
            MiniChildStruct->Language = "";
        }
        else
        {
            MiniChildStruct->Language = StringCStrConvertToString(ChildStruct->Language);
        }
        MiniChildStruct->ModifFlag = ChildStruct->ModifFlag;
        if(ChildStruct->ReadOnly == 1)
        {
            MiniChildStruct->ReadOnly = 1;
        }
        else
        {
            MiniChildStruct->ReadOnly = 0;
        }
        if(ChildStruct->Language->Len() == 0)
        {
            MiniChildStruct->FileName = "";
        }
        else
        {
            MiniChildStruct->FileName = StringCStrConvertToString(ChildStruct->RFile);
        }
    }
}

// -----------------------------------------------------------------------
// Retrieve the filename of an ini file
char * CALLBACK WAMMGetIniFilePathName(long FileNumber)
{
    CStr BufString;
    CStr ReturnValue;

    switch(FileNumber)
    {
        case INIFILE_MAIN:
            BufString = MainIniFile;
			break;
		case INIFILE_CURRENTUSERMENUS:
            BufString = UserMenusIniFile;
			break;
        case INIFILE_TOOLBARS:
            BufString = ToolbarsIniFile;
			break;
        case INIFILE_COLORS:
            BufString = ColorsIniFile;
			break;
        case INIFILE_FTPACCOUNTS:
            BufString = FtpAccountsIniFile;
			break;
        case INIFILE_SKINS:
            BufString = SkinsIniFile;
			break;
        case INIFILE_PROJECTSTYPES:
            BufString = ProjectsIniFile;
			break;
        case INIFILE_LANGUAGES:
            BufString = LanguagesIniFile;
			break;
        case INIFILE_DATES:
            BufString = DatesIniFile;
			break;
        case INIFILE_CGREP:
            BufString = cGrepIniFile;
			break;
        case INIFILE_FILTERS:
            BufString = FiltersIniFile;
			break;
        default:
            BufString = "";
			break;
    }
	ReturnValue = BufString;
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Retrieve a stock directory
char * CALLBACK WAMMGetQuickDirectory(long DirNumber)
{
    switch(DirNumber)
    {
        case DIR_ROOT:
            return(Dirs[DirNumber]);
        case DIR_BIN:
            return(Dirs[DirNumber]);
        case DIR_INCLUDE:
            return(Dirs[DirNumber]);
        case DIR_LIB:
            return(Dirs[DirNumber]);
        case DIR_HELP:
            return(Dirs[DirNumber]);
        case DIR_MAIN:
            return(Dirs[DirNumber]);
        case DIR_WIN:
            return(Dirs[DirNumber]);
        case DIR_WINSYS:
            return(Dirs[DirNumber]);
        case DIR_TEMP:
            return(Dirs[DirNumber]);
        case DIR_TEMPLATES:
            return(Dirs[DirNumber]);
        case DIR_PROJECTS:
            return(Dirs[DirNumber]);
        case DIR_MAINHELP:
            return(Dirs[DirNumber]);
        case DIR_SNIPPETS:
            return(Dirs[DirNumber]);
        case DIR_DATABASES:
            return(Dirs[DirNumber]);
        case DIR_SKINS:
            return(Dirs[DirNumber]);
        case DIR_LANGUAGES:
            return(Dirs[DirNumber]);
        case DIR_DOWNLOADS:
            return(Dirs[DirNumber]);
        case DIR_ADDINS:
            return(Dirs[DirNumber]);
        case DIR_CONFIG:
            return(Dirs[DirNumber]);
        case DIR_MENUS:
            return(Dirs[DirNumber]);
        case DIR_WIZARDS:
            return(Dirs[DirNumber]);
        case DIR_FILTERS:
            return(Dirs[DirNumber]);
        case DIR_EXTRACODE:
            return(Dirs[DirNumber]);
        case DIR_LANGSCRIPTS:
            return(Dirs[DirNumber]);
        case DIR_TOOLS:
            return(Dirs[DirNumber]);
        case DIR_SCRIPTS:
            return(Dirs[DirNumber]);
        default:
            return("");
    }
}

// -----------------------------------------------------------------------
// Fill main context
void CALLBACK WAMMGetContext(LPADDINCONTEXT VarsStruct)
{
    VarsStruct->Version = AppRevision | (AppVersion << 8);
    VarsStruct->NbrChilds = NbForms;
    VarsStruct->hInstance = ApphInstance;
    VarsStruct->ActiveProject = ProjectOn;

    VarsStruct->OSType = OSTYPE_WINDOWS_9X;
    if(WindowsNT) VarsStruct->OSType = OSTYPE_WINDOWS_NT;
	if(Windows2K) VarsStruct->OSType = OSTYPE_WINDOWS_2K;
    if(WindowsXP) VarsStruct->OSType = OSTYPE_WINDOWS_XP;

	if(FullScreenMode == 1) VarsStruct->FullScreenMode = 1;
    else VarsStruct->FullScreenMode = 0;

    if(SysTrayMode == 1) VarsStruct->SysTrayMode = 1;
    else VarsStruct->SysTrayMode = 0;

    VarsStruct->hMDI = hMDIform.hWnd;
    VarsStruct->hClient = hMDIform.hClient;
    VarsStruct->hAccelerators = hGlobAccelerators;
    
	if(NbForms == 0)
	{
        VarsStruct->hCurrentChild = 0;
        VarsStruct->hCurrentCodeMax = 0;
	}
	else
	{
        VarsStruct->hCurrentChild = CurrentForm;
        ChildStruct = LoadStructure(CurrentForm);
        VarsStruct->hCurrentCodeMax = ChildStruct->hChildCodeMax;
    }
    VarsStruct->hRebar = hRebar;
    VarsStruct->hTreeView = hTreeView;
    VarsStruct->hWindowsList = hWindowsListView;
    VarsStruct->hStatusBar = hStatusBar;
    VarsStruct->hOutputBox = hStatusBox;
    VarsStruct->hDebugBox = hDebugBox;
    VarsStruct->hOutputSysTab = hOutputSysTab;
	VarsStruct->hEmptyImageList = GlobalImageList3;
    VarsStruct->hMainImageList = GlobalImageList1;
    VarsStruct->hLittleImageList = GlobalImageList2;
    VarsStruct->hControlsImageList = GlobalImageList4;
    VarsStruct->hDialogEdImageList = GlobalImageList5;
    VarsStruct->MDIhWndProc = CurrentMDIProc;
    VarsStruct->ChildCreateProc = CurrentMDIChildInitProc;
    VarsStruct->ChildhWndProc = CurrentMDIChildProc;
    VarsStruct->CodeMaxhWndProc = CurrentCodeMaxProc;
    VarsStruct->hProjectToolbar = hProjectToolbar;
    VarsStruct->hProjectPager = hProjectPager;
    VarsStruct->hWindowsToolbar = hWindowsToolbar;
    VarsStruct->hWindowsPager = hWindowsPager;
    VarsStruct->hOutputBoxToolbar = hStatusToolBar;
    VarsStruct->hOutputBoxPager = hStatusPager;
	VarsStruct->ColdStart = FirstStart;
}

// -----------------------------------------------------------------------
// Fill main context
int CALLBACK WAMMGetContextElement(int Idx)
{
    switch(Idx)
    {
        case 0:
            return(0);

        case 1:
            return(AppRevision | (AppVersion << 8));

        case 2:
            return(NbForms);

        case 3:
            return(ProjectOn);

        case 4:
            return((int) ApphInstance);

        case 5:
            if(WindowsNT) return(OSTYPE_WINDOWS_NT);
	        if(Windows2K) return(OSTYPE_WINDOWS_2K);
            if(WindowsXP) return(OSTYPE_WINDOWS_XP);
            else return(OSTYPE_WINDOWS_9X);

        case 6:
	        if(FullScreenMode == 1) return(1);
            else return(0);

        case 7:
            if(SysTrayMode == 1) return(1);
            else return(0);

        case 8:
            return((int) hMDIform.hWnd);

        case 9:
            return((int) hMDIform.hClient);

        case 10:
            return((int) hGlobAccelerators);
    
        case 11:
            return((int) hTreeView);

        case 12:
            return((int) hWindowsListView);

        case 13:
            return((int) hStatusBar);

        case 14:
            return((int) hStatusBox);

        case 15:
            return((int) hDebugBox);

        case 16:
            return((int) hOutputSysTab);

        case 17:
	        if(NbForms == 0) {
                return(0);
            } else {
                return((int) CurrentForm);
            }
            break;

        case 18:
	        if(NbForms == 0)
	        {
                return(0);
	        }
	        else
	        {
                ChildStruct = LoadStructure(CurrentForm);
                return((int) ChildStruct->hChildCodeMax);
            }
            break;

        case 19:
            return((int) hRebar);

        case 20:
	        return((int) GlobalImageList3);

        case 21:
            return((int) GlobalImageList1);

        case 22:
            return((int) GlobalImageList2);

        case 23:
            return((int) GlobalImageList4);

        case 24:
            return((int) GlobalImageList5);

        case 25:
            return((int) CurrentMDIProc);

        case 26:
            return((int) CurrentMDIChildInitProc);

        case 27:
            return((int) CurrentMDIChildProc);

        case 28:
            return((int) CurrentCodeMaxProc);

        case 29:
            return((int) hProjectToolbar);

        case 30:
            return((int) hProjectPager);

        case 31:
            return((int) hWindowsToolbar);

        case 32:
            return((int) hWindowsPager);

        case 33:
            return((int) hStatusToolBar);

        case 34:
            return((int) hStatusPager);

        case 35:
	        return(FirstStart);

        default:
	        return(0);
    }
}

// -----------------------------------------------------------------------
// Get current line
char * CALLBACK WAMMGetcurrentLine(HWND hForm)
{
    CStr ReturnValue;
    
    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = RetrieveCurrentLine(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Select current word
char * CALLBACK WAMMSelectCurrentWord(HWND hForm)
{
    CStr ReturnValue;

    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = SelectCurrentWord(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Retrieve current word
char * CALLBACK WAMMGetCurrentWord(HWND hForm)
{
    CStr ReturnValue;
    
    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = GetCurrentWord(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Insert a text at current position
void CALLBACK InsertText(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsert(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Insert a text at current position and jump to next line
void CALLBACK WAMMInsertLine(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsertLine(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Insert a file at current position
void CALLBACK WAMMInsertLines(HWND hForm, long Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsertFile(hForm, Txt);
}

// -----------------------------------------------------------------------
// Set the text in a window
void CALLBACK WAMMSetText(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SendTextToChild(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Restore the MDI form
void CALLBACK WAMMRestoreMDIFromSysTray(void) {
    SysTrayMode = 0;
    RestoreMDI(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Hook chaining
long CALLBACK WAMMInstallHookProc(long HookAddress, long HookType)
{
    int i = 0;
    long FoundHook = 0;

    switch(HookType)
    {
        case HOOK_MDICREATE:
			for(i = 0; i < MDICreateHooks.Amount(); i++)
			{
                if(MDICreateHooks.Get(i)->Content == 0)
                {
                    if((MDICreateHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				MDICreateHooks.Set(i, HookAddress);
				MDICreateHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
				MDICreateHooks.Add(0L);
				MDICreateHooksNbr.Add(0L);
            }
			break;
        case HOOK_MDI:
            for(i = 0; i < MDIHooks.Amount(); i++)
            {
                if(MDIHooks.Get(i)->Content == 0)
                {
                    if((MDIHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				MDIHooks.Set(i, (DLGPROC) HookAddress);
				MDIHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
				MDIHooks.Add(0L);
				MDIHooksNbr.Add(0L);
            }
			break;
        case HOOK_MDICLIENT:
            for(i = 0; i < MDIClientHooks.Amount(); i++)
            {
                if(MDIClientHooks.Get(i)->Content == 0)
                {
                    if((MDIClientHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				MDIClientHooks.Set(i, (DLGPROC) HookAddress);
				MDIClientHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
                MDIClientHooks.Add(0L);
                MDIClientHooksNbr.Add(0L);
			}
			break;
        case HOOK_CHILDCREATE:
            for(i = 0; i < ChildCreateHooks.Amount(); i++)
            {
                if(ChildCreateHooks.Get(i)->Content == 0)
                {
                    if((ChildCreateHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				ChildCreateHooks.Set(i, HookAddress);
				ChildCreateHooksNbr.Set(i, HookNbr);
            }
            else
            {
                ChildCreateHooks.Add(0L);
                ChildCreateHooksNbr.Add(0L);
			}
			break;
        case HOOK_CHILD:
            for(i = 0; i < ChildHooks.Amount(); i++)
            {
                if(ChildHooks.Get(i)->Content == 0)
                {
                    if((ChildHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				ChildHooks.Set(i, (DLGPROC) HookAddress);
				ChildHooksNbr.Set(i, HookNbr);
            }
            else
            {
                ChildHooks.Add(0L);
                ChildHooksNbr.Add(0L);
			}
			break;
        case HOOK_CMAXXX:
            for(i = 0; i < CodeMaxHooks.Amount(); i++)
            {
                if(CodeMaxHooks.Get(i)->Content == 0)
                {
                    if((CodeMaxHooks.Amount() -1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
				CodeMaxHooks.Set(i, (DLGPROC) HookAddress);
				CodeMaxHooksNbr.Set(i, HookNbr);
            }
            else
            {
                CodeMaxHooks.Add(0L);
                CodeMaxHooksNbr.Add(0L);
			}
			break;
	}
    return(HookNbr);
}

void CALLBACK WAMMUnInstallHookProc(long HookAddress, long HookType, long HookNumber)
{
    int i = 0;
    long FoundHook = 0;

    switch(HookType)
    {
        case HOOK_MDICREATE:
            for(i = 0; i < MDICreateHooks.Amount(); i++)
            {
                if(MDICreateHooks.Get(i)->Content == HookAddress)
                {
                    // Good hook number ?
                    if(MDICreateHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDICreateHooks.Set(i, 0L);
			break;
		case HOOK_MDI:
            for(i = 0; i < MDIHooks.Amount(); i++)
            {
                if(MDIHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    // Good hook number ?
                    if(MDIHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDIHooks.Set(i, 0L);
			break;
        case HOOK_MDICLIENT:
            for(i = 0; i < MDIClientHooks.Amount(); i++)
            {
                if(MDIClientHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    // Good hook number ?
                    if(MDIClientHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDIClientHooks.Set(i, 0L);
			break;
        case HOOK_CHILDCREATE:
            for(i = 0; i < ChildCreateHooks.Amount(); i++)
            {
                if(ChildCreateHooks.Get(i)->Content == HookAddress)
                {
                    if(ChildCreateHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) ChildCreateHooks.Set(i, 0L);
			break;
        case HOOK_CHILD:
            for(i = 0; i < ChildHooks.Amount(); i++)
            {
                if(ChildHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    if(ChildHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) ChildHooks.Set(i, 0L);
			break;
		case HOOK_CMAXXX:
            for(i = 0; i < CodeMaxHooks.Amount(); i++)
            {
                if(CodeMaxHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    if(CodeMaxHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) CodeMaxHooks.Set(i, 0L);
			break;
	}
}

// -----------------------------------------------------------------------
// Retrieve the current line of a window
long CALLBACK WAMMGetCurrentLineNumber(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(CodeMaxCurRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Retrieve the current col of a window
long CALLBACK WAMMGetCurrentColNumber(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(CodeMaxCurRange.posStart.nCol);
}

// -----------------------------------------------------------------------
// Retrieve the current line text of a window
char * CALLBACK WAMMGetCurrentLineText(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(StringCStrConvertToString(CMGetCurrentLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine)));
}

// -----------------------------------------------------------------------
// Retrieve the current line text of a window
char * CALLBACK WAMMGetLineText(HWND hWnd, long Line)
{
    ChildStruct = LoadStructure(hWnd);
    return(StringCStrConvertToString(CMGetCurrentLine(ChildStruct->hChildCodeMax, Line)));
}

// -----------------------------------------------------------------------
// Write a text into output box
void CALLBACK WAMMWriteToOutputBox(char *WAText)
{
    WriteToStatus(StringConvertToCStr(WAText));
}

// -----------------------------------------------------------------------
// Enumerate the childs windows
void CALLBACK WAMMEnumSourceWindows(long CallBack)
{
    if(CallBack != 0)
    {
        if(NbForms != 0) EnumChildWindows(hMDIform.hWnd, &EnumChilds, CallBack);
    }
}
                    
int CALLBACK EnumChilds(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(WAControlIsWindowChild(hWnd) == 0) goto NoEnumChildsProc;
        if(GetWindowLong(hWnd, GWL_USERDATA) == 0) goto NoEnumChildsProc;
        if(lParam != 0) return(JumpToAddrArg((FARPROC) lParam, (long) hWnd));
NoEnumChildsProc:
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// An AddIn requested a kill
void CALLBACK WAMMAddInKillZombie(long CallBack)
{
    int i = 0;
    long FoundAddInProc = 0;

    if(CallBack == 0) return;
    // Search Unload routine address
    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        if(AddInsUnLoad.Get(i)->Content == (FARPROC) CallBack)
        {
            FoundAddInProc = 1;
            break;
        }
    }
    if(FoundAddInProc == 0) return;
    if(AddInsUnLoad.Get(i)->Content != 0)
    {
		JumpToAddr(AddInsUnLoad.Get(i)->Content);
    }
	RunningAddIns.Set(i, 0L);
    WAIniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), RunningAddIns.Get(i)->Content, MainIniFile);
    // Change icon
    if(FRMAddInsListView != 0) WAListViewSetSubItemImage(FRMAddInsListView, ICON_ADDINR, i, 0);
}
                    
// -----------------------------------------------------------------------
// Check if a DLL is a support AddIn
long CALLBACK WAMMSpAddInIsAvailable(char *AlienDllName)
{
    CStr BufString;

	BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) StringConvertToCStr(AlienDllName);
	return(IsSpAddIn(BufString));
}

// -----------------------------------------------------------------------
// Chromatic standard inputbox
char * CALLBACK WAMMMiscInputBox(HWND hParent, char *NameTitle, long Icon, char *ValueToPass,
                                 long PromptLen, long PromptType, char *PromptIniKey)
{
    return(StringCStrConvertToString(MiscInputBox(hParent, StringConvertToCStr(NameTitle), Icon, StringConvertToCStr(ValueToPass), PromptLen, PromptType, StringConvertToCStr(PromptIniKey))));
}

// -----------------------------------------------------------------------
// WA Library wrappers for wide strings conversions
// -----------------------------------------------------------------------
    
long CALLBACK Wrapper_WAMenuSetItemText(HMENU hMenu, char *Wrapper_ItemText, long ItemID)
{
    return(WAMenuSetItemText(hMenu, StringConvertToCStr(Wrapper_ItemText), ItemID));
}

long CALLBACK Wrapper_WAMiscClipBoardCopyText(char *Wrapper_WATxt)
{
    return(WAMiscClipBoardCopyText(StringConvertToCStr(Wrapper_WATxt)));
}

char * CALLBACK Wrapper_WAMiscClipBoardPasteText(void)
{
    return(StringCStrConvertToString(WAMiscClipBoardPasteText()));
}

long CALLBACK Wrapper_WAComboBoxAddItem(HWND Wrapper_WAhCB, char *Wrapper_WACBItemText, long Wrapper_WACBItemIndex)
{
    return(WAComboBoxAddItem(Wrapper_WAhCB, StringConvertToCStr(Wrapper_WACBItemText), Wrapper_WACBItemIndex));
}

char * CALLBACK Wrapper_WAComboBoxGetCurrentItem(HWND Wrapper_WAhCB)
{
    return(StringCStrConvertToString(WAComboBoxGetCurrentItem(Wrapper_WAhCB)));
}

char * CALLBACK Wrapper_WAComboBoxGetItem(HWND Wrapper_WAhCB, long Wrapper_WACBIndex)
{
    return(StringCStrConvertToString(WAComboBoxGetItem(Wrapper_WAhCB, Wrapper_WACBIndex)));
}

long CALLBACK Wrapper_WAComboBoxItemExist(HWND Wrapper_WAhCB, char *Wrapper_WACBItem)
{
    return(WAComboBoxItemExist(Wrapper_WAhCB, StringConvertToCStr(Wrapper_WACBItem)));
}

void CALLBACK Wrapper_WAComboBoxSaveInIniFile(HWND Wrapper_WAhCB, char *EntryToAdd, char *IniKey, char *IniFile)
{
    WAComboBoxSaveInIniFile(Wrapper_WAhCB, StringConvertToCStr(EntryToAdd), StringConvertToCStr(IniKey), StringConvertToCStr(IniFile));
	return;
}

void CALLBACK Wrapper_WAComboBoxFillFromIniFile(HWND Wrapper_WAhCB, char *IniKey, char *IniFile)
{
    WAComboBoxFillFromIniFile(Wrapper_WAhCB, StringConvertToCStr(IniKey), StringConvertToCStr(IniFile));
	return;
}

char * CALLBACK Wrapper_WAControlGetClassName(HWND Wrapper_WAhWnd)
{
    return(StringCStrConvertToString(WAControlGetClassName(Wrapper_WAhWnd)));
}

char * CALLBACK Wrapper_WAControlGetText(HWND Wrapper_WAhWnd)
{
    return(StringCStrConvertToString(WAControlGetText(Wrapper_WAhWnd)));
}

long CALLBACK Wrapper_WAControlSetText(HWND Wrapper_WAhWnd, char *WAATextToSet)
{
    return(WAControlSetText(Wrapper_WAhWnd, StringConvertToCStr(WAATextToSet)));
}

HWND CALLBACK Wrapper_WACreateButton(long Wrapper_WABLeft, long Wrapper_WABTop, long Wrapper_WABWidth, long Wrapper_WABHeight,
                                     HWND Wrapper_WAhParent, char *Wrapper_WABText, long Wrapper_WACtrlID, long Wrapper_WAImgType,
                                     HICON Wrapper_WAhImage, WNDPROC Wrapper_WAWinProc, long Wrapper_WAExtraStyle, long Wrapper_WAExtraExStyle)
{
    return(WACreateButton(Wrapper_WABLeft, Wrapper_WABTop, Wrapper_WABWidth, Wrapper_WABHeight, Wrapper_WAhParent, StringConvertToCStr(Wrapper_WABText), Wrapper_WACtrlID, Wrapper_WAImgType, Wrapper_WAhImage, Wrapper_WAWinProc, Wrapper_WAExtraStyle, Wrapper_WAExtraExStyle | Buttons_StaticEdge));
}

long CALLBACK Wrapper_WAMiscMsgBox(HWND WAHParent, char *WAMBText, long WAMBType)
{
    return(WAMiscMsgBox(WAHParent, StringConvertToCStr(WAMBText), WAMBType, Requesters));
}

HWND CALLBACK Wrapper_WACreateCheckBox(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WAUnderState)
{
    return(WACreateCheckBox(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle, WAUnderState));
}

HWND CALLBACK Wrapper_WACreateComboBox(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(WACreateComboBox(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_WACreateDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType)
{
    return(WACreateDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WADTitle), WAInitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType));
}

HWND CALLBACK Wrapper_WACreateFrame(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(WACreateFrame(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_WACreateHexBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, long WAMaxDigits, long WAExtraStyle, long WAExtraExStyle)
{
    return(WACreateHexBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAMaxDigits, WAExtraStyle, WAExtraExStyle));
}

HWND CALLBACK Wrapper_WACreateLabel(long WALLeft, long WALTop, long WALWidth, long WALHeight, HWND WAHParent, char *WALText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, HFONT WAhFont)
{
    return(WACreateLabel(WALLeft, WALTop, WALWidth, WALHeight, WAHParent, StringConvertToCStr(WALText), WACtrlID, WAWinProc, WAExtraStyle, WAhFont));
}

HWND CALLBACK Wrapper_WACreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass)
{
    return(WACreateMDIChildDialog(WAHParent, StringConvertToCStr(WACTitle), WAhIcon, WAInitProc, WAWinProc, WAExtraStyle, WAZeroPos, StringConvertToCStr(WACustomClass)));
}

HWND CALLBACK Wrapper_WACreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType)
{
    return(WACreateMDIDialog(WAMDILeft, WAMDITop, WAMDIWidth, WAMDIHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WAMDITitle), WAInitProc, WAWinProc, WAExtraStyle, WAShowType));
}

long CALLBACK Wrapper_WACreateModalDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, DLGPROC WAWinProc, long WAExtraStyle, long WACentered)
{
    return(WACreateModalDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAWinProc, WAExtraStyle, WACentered));
}

HWND CALLBACK Wrapper_WACreateRadioButton(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(WACreateRadioButton(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_WACreateRichTextBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WASetborder)
{
    return(WACreateRichTextBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAWinProc, WAExtraStyle, WASetborder));
}

HWND CALLBACK Wrapper_WACreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *WAInitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType)
{
    return(WACreateSplashDialog(WADLeft, WADTop, WADWidth, WADHeight, StringConvertToCStr(WADTitle), WAHParent, WAhMenu, WAhIcon, WAInitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType));
}

HWND CALLBACK Wrapper_WACreateStatusBar(char *WASBText, long WARaiseType, HWND WAHParent, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(WACreateStatusBar(StringConvertToCStr(WASBText), WARaiseType, WAHParent, WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_WACreateTextBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WAExtraExStyle)
{
    return(WACreateTextBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAWinProc, WAExtraStyle, WAExtraExStyle));
}

char * CALLBACK Wrapper_WADateGetDay(void)
{
    return(StringCStrConvertToString(WADateGetDay()));
}

char * CALLBACK Wrapper_WADateGetFormat(char *WADateformat)
{
    return(StringCStrConvertToString(WADateGetFormat(StringConvertToCStr(WADateformat))));
}

char * CALLBACK Wrapper_WADateGetHour(void)
{
    return(StringCStrConvertToString(WADateGetHour()));
}

char * CALLBACK Wrapper_WADateGetMinutes(void)
{
    return(StringCStrConvertToString(WADateGetMinutes()));
}

char * CALLBACK Wrapper_WADateGetMonth(void)
{
    return(StringCStrConvertToString(WADateGetMonth()));
}

char * CALLBACK Wrapper_WADateGetNow(long WAReportSeconds)
{
    return(StringCStrConvertToString(WADateGetNow(WAReportSeconds)));
}

char * CALLBACK Wrapper_WADateGetSeconds(void)
{
    return(StringCStrConvertToString(WADateGetSeconds()));
}

char * CALLBACK Wrapper_WADateGetTimeFormat(char *WADateformat)
{
    return(StringCStrConvertToString(WADateGetTimeFormat(StringConvertToCStr(WADateformat))));
}

char * CALLBACK Wrapper_WADateGetYear(void)
{
    return(StringCStrConvertToString(WADateGetYear()));
}

HANDLE CALLBACK Wrapper_WAFileCreateEmpty(char *WAFileName, long WASecurity)
{
    return(WAFileCreateEmpty(StringConvertToCStr(WAFileName), WASecurity));
}

long CALLBACK Wrapper_WAFileExist(char *WAFileToSearch)
{
    return(WAFileExist(StringConvertToCStr(WAFileToSearch)));
}

long CALLBACK Wrapper_WAFileGetAccessedTime(char *WAFileName, LPFILETIME WAFileAccessedTime)
{
    return(WAFileGetAccessedTime(StringConvertToCStr(WAFileName), WAFileAccessedTime));
}

long CALLBACK Wrapper_WAFileGetCreationTime(char *WAFileName, LPFILETIME WAFileCreationTime)
{
    return(WAFileGetCreationTime(StringConvertToCStr(WAFileName), WAFileCreationTime));
}

char * CALLBACK Wrapper_WAFileGetDirectory(char *WAFileName)
{
    return(StringCStrConvertToString(WAFileGetDirectory(StringConvertToCStr(WAFileName))));
}

char * CALLBACK Wrapper_WAFileGetExtension(char *WAFileName)
{
    return(StringCStrConvertToString(WAFileGetExtension(StringConvertToCStr(WAFileName))));
}

char * CALLBACK Wrapper_WAFileGetFileName(char *WAFileName)
{
    return(StringCStrConvertToString(WAFileGetFileName(StringConvertToCStr(WAFileName))));
}

long CALLBACK Wrapper_WAFileGetSize(char *WAFileName)
{
    return(WAFileGetSize(StringConvertToCStr(WAFileName)));
}

long CALLBACK Wrapper_WAFileGetWriteTime(char *WAFileName, LPFILETIME WAFileWriteTime)
{
    return(WAFileGetWriteTime(StringConvertToCStr(WAFileName), WAFileWriteTime));
}

long CALLBACK Wrapper_WAFileIsDirectory(char *WAFileToSearch)
{
    return(WAFileIsDirectory(StringConvertToCStr(WAFileToSearch)));
}

HANDLE CALLBACK Wrapper_WAFileOpenR(char *WAFileName) {
    return(WAFileOpenR(StringConvertToCStr(WAFileName)));
}

HANDLE CALLBACK Wrapper_WAFileOpenW(char *WAFileName, long WAFilePos)
{
    return(WAFileOpenW(StringConvertToCStr(WAFileName), WAFilePos));
}

HANDLE CALLBACK Wrapper_WAFileOpenWAppend(char *WAFileName)
{
    return(WAFileOpenWAppend(StringConvertToCStr(WAFileName)));
}

char * CALLBACK Wrapper_WAFileRemoveExtension(char *WAFileName)
{
    return(StringCStrConvertToString(WAFileRemoveExtension(StringConvertToCStr(WAFileName))));
}

char * CALLBACK Wrapper_WAFileReplaceExtension(char *WAFileName, char *WANewExtension)
{
    return(StringCStrConvertToString(WAFileReplaceExtension(StringConvertToCStr(WAFileName), StringConvertToCStr(WANewExtension))));
}

long CALLBACK Wrapper_WAFileIsReadOnly(char *WAFileToCheck)
{
    return(WAFileIsReadOnly(StringConvertToCStr(WAFileToCheck)));
}

long CALLBACK Wrapper_WAFileIsUnix(char *WAFileToCheck)
{
    return(WAFileIsUnix(StringConvertToCStr(WAFileToCheck)));
}

HGLOBAL CALLBACK Wrapper_WAFileLoadIntoMem(char *WAFileToLoad, long *BytesRd)
{
    return(WAFileLoadIntoMem(StringConvertToCStr(WAFileToLoad), BytesRd));
}

long CALLBACK Wrapper_WAFileSaveFromMem(char *WAFileToSave, long MemToSave, long LenToSave)
{
    return(WAFileSaveFromMem(StringConvertToCStr(WAFileToSave), MemToSave, LenToSave));
}

char * CALLBACK Wrapper_WAFileDir(char *WAWildCard, long FileType)
{
    return(StringCStrConvertToString(WAFileDir(&(CStr) StringConvertToCStr(WAWildCard), FileType)));
}

long CALLBACK Wrapper_WAFileWriteLine(HANDLE WAFileHandle, char *WALineToWrite)
{
    return(WAFileWriteLine(WAFileHandle, StringConvertToCStr(WALineToWrite)));
}

char * CALLBACK Wrapper_WAFTPGetEntryDate(char *WADirEntry)
{
    return(StringCStrConvertToString(WAFTPGetEntryDate(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_WAFTPGetEntryFileAttributes(char *WADirEntry)
{
    return(StringCStrConvertToString(WAFTPGetEntryFileAttributes(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_WAFTPGetEntryFileName(char *WADirEntry)
{
    return(StringCStrConvertToString(WAFTPGetEntryFileName(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_WAFTPGetEntryFileSize(char *WADirEntry)
{
    return(StringCStrConvertToString(WAFTPGetEntryFileSize(StringConvertToCStr(WADirEntry))));
}

long CALLBACK Wrapper_WAFTPIsEntryDir(char *WADirEntry)
{
    return(WAFTPIsEntryDir(StringConvertToCStr(WADirEntry)));
}

long CALLBACK Wrapper_WAGDIGetTextHeight(HWND WAhWnd, HFONT WAhFont, char *WATxt)
{
    return(WAGDIGetTextHeight(WAhWnd, WAhFont, StringConvertToCStr(WATxt)));
}

long CALLBACK Wrapper_WAGDIGetTextWidth(HWND WAhWnd, HFONT WAhFont, char *WATxt)
{
    return(WAGDIGetTextWidth(WAhWnd, WAhFont, StringConvertToCStr(WATxt)));
}

HFONT CALLBACK Wrapper_WAGDIObtainFont(char *WAFontNameToObtain, long WAFontSizeToObtain, HWND WAhWnd, long WABold, long WAItalic)
{
    return(WAGDIObtainFont(StringConvertToCStr(WAFontNameToObtain), WAFontSizeToObtain, WAhWnd, WABold, WAItalic));
}

void CALLBACK Wrapper_WAGDIWriteClippedText(HDC WAhDC, long WAx, long WAy, long WAWidth, long WAHeight, char *WAText, long WATextColor, HFONT WAhTextFont, long WATextTransparent, long WABackGroundColor)
{
    WAGDIWriteClippedText(WAhDC, WAx, WAy, WAWidth, WAHeight, StringConvertToCStr(WAText), WATextColor, WAhTextFont, WATextTransparent, WABackGroundColor);
}

void CALLBACK Wrapper_WAGDIWriteText(HDC WAhDC, long WAx, long WAy, char *WAText, long WATextColor, HFONT WAhTextFont, long WATextTransparent, long WABackGroundColor)
{
    WAGDIWriteText(WAhDC, WAx, WAy, StringConvertToCStr(WAText), WATextColor, WAhTextFont, WATextTransparent, WABackGroundColor);
}

long CALLBACK Wrapper_WAIniDeleteKey(char *WASection, char *WAKey, char *WAFile)
{
    return(WAIniDeleteKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAFile)));
}

char * CALLBACK Wrapper_WAIniReadKey(char *WASection, char *WAKey, char *WAFile)
{
    return(StringCStrConvertToString(WAIniReadKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAFile))));
}

long CALLBACK Wrapper_WAIniReadBoolKey(char *WASection, char *WAKey, char *WAFile)
{
    return(WAIniReadBoolKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAFile)));
}

long CALLBACK Wrapper_WAIniWriteKey(char *WASection, char *WAKey, char *WAValue, char *WAFile)
{
    return(WAIniWriteKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAValue), StringConvertToCStr(WAFile)));
}

long CALLBACK Wrapper_WAIniWriteSection(char *WASection, char *WAKey, char *WAFile)
{
    return(WAIniWriteSection(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAFile)));
}

long CALLBACK Wrapper_WAListBoxAddItem(HWND WAhLB, char *WALBItemText, long WALBItemIndex)
{
    return(WAListBoxAddItem(WAhLB, StringConvertToCStr(WALBItemText), WALBItemIndex));
}

char * CALLBACK Wrapper_WAListBoxGetCurrentItem(HWND WAhLB)
{
    return(StringCStrConvertToString(WAListBoxGetCurrentItem(WAhLB)));
}

char * CALLBACK Wrapper_WAListBoxGetItem(HWND WAhLB, long WALBIndex)
{
    return(StringCStrConvertToString(WAListBoxGetItem(WAhLB, WALBIndex)));
}

long CALLBACK Wrapper_WAListBoxItemExist(HWND WAhListBox, char *WACBItem)
{
    return(WAListBoxItemExist(WAhListBox, StringConvertToCStr(WACBItem)));
}

long CALLBACK Wrapper_WAListBoxReplaceItem(HWND WAhLB, long WALBIndex, char *WALBNewItem)
{
    return(WAListBoxReplaceItem(WAhLB, WALBIndex, StringConvertToCStr(WALBNewItem)));
}

long CALLBACK Wrapper_WAListBoxReplaceSelItem(HWND WAhLB, char *WBLBNewItemText)
{
    return(WAListBoxReplaceSelItem(WAhLB, StringConvertToCStr(WBLBNewItemText)));
}

long CALLBACK Wrapper_WAListViewAddCol(HWND WAhListView, char *WALVColText, long WALVWidth, long WAColPosition)
{
    return(WAListViewAddCol(WAhListView, StringConvertToCStr(WALVColText), WALVWidth, WAColPosition));
}

long CALLBACK Wrapper_WAListViewAddItem(HWND WAhListView, char *WALVItemText, long WALVItemPos, long WALVImage)
{
    return(WAListViewAddItem(WAhListView, StringConvertToCStr(WALVItemText), WALVItemPos, WALVImage));
}

long CALLBACK Wrapper_WAListViewFindItem(HWND WAhListView, char *WALVItemText)
{
    return(WAListViewFindItem(WAhListView, StringConvertToCStr(WALVItemText)));
}

long CALLBACK Wrapper_WAListViewFindSubItem(HWND WAhListView, char *WATextToFind, long WASubItemNbr, long WASearchCase)
{
    return(WAListViewFindSubItem(WAhListView, StringConvertToCStr(WATextToFind), WASubItemNbr, WASearchCase));
}

char * CALLBACK Wrapper_WAListViewGetEditResult(long WAlParam)
{
    return(StringCStrConvertToString(WAListViewGetEditResult(WAlParam)));
}

char * CALLBACK Wrapper_WAListViewGetHeaderLabel(HWND WAhListView, long WALVColNumber)
{
    return(StringCStrConvertToString(WAListViewGetHeaderLabel(WAhListView, WALVColNumber)));
}

char * CALLBACK Wrapper_WAListViewGetItemText(HWND WAhListView, long WALVItemIndex, long WALVSubItemNumber)
{
    return(StringCStrConvertToString(WAListViewGetItemText(WAhListView, WALVItemIndex, WALVSubItemNumber)));
}

char * CALLBACK Wrapper_WAListViewGetSelItemText(HWND WAhListView, long WALVSubItemNumber)
{
    return(StringCStrConvertToString(WAListViewGetSelItemText(WAhListView, WALVSubItemNumber)));
}

long CALLBACK Wrapper_WAListViewSetSelItemText(HWND WAhListView, char *WALVItemText, long WALVSubItemNumber)
{
    return(WAListViewSetSelItemText(WAhListView, StringConvertToCStr(WALVItemText), WALVSubItemNumber));
}

long CALLBACK Wrapper_WAListViewSetSubItem(HWND WAhListView, char *WALVItemText, long WALVItemPos, long WALVSubItemPos)
{
    return(WAListViewSetSubItem(WAhListView, StringConvertToCStr(WALVItemText), WALVItemPos, WALVSubItemPos));
}

char * CALLBACK Wrapper_WAMenuGetString(HMENU WAhMenu, long WAItemID)
{
    return(StringCStrConvertToString(WAMenuGetString(WAhMenu, WAItemID)));
}

long CALLBACK Wrapper_WARebarAddBand(HWND WAhRebar, HWND WAhChild, char *WARBText, long WARBYSize, long WAExtraStyle, long WAMinSize)
{
    return(WARebarAddBand(WAhRebar, WAhChild, StringConvertToCStr(WARBText), WARBYSize, WAExtraStyle, WAMinSize));
}

long CALLBACK Wrapper_WARegistryDeleteKey(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyName, long WAKeyType)
{
    return(WARegistryDeleteKey(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyName), WAKeyType));
}

char * CALLBACK Wrapper_WARegistryGetKeyValue(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyRef)
{
    return(StringCStrConvertToString(WARegistryGetKeyValue(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyRef))));
}

long CALLBACK Wrapper_WARegistryUpdateKey(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyName, char *WASubKeyValue, long WAKeyType)
{
    return(WARegistryUpdateKey(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyName), StringConvertToCStr(WASubKeyValue), WAKeyType));
}

void CALLBACK Wrapper_WAMiscShellAddFileToRecents(char *WAFileName)
{
    WAMiscShellAddFileToRecents(StringConvertToCStr(WAFileName));
}

long CALLBACK Wrapper_SocketConnect(SOCKET WAhSock, LPSOCKADDR_IN WASockStruct, long WAMessage, HWND WAhWnd, char *WAIP, long WAPort, long WATimeOut)
{
    return(SocketConnect(WAhSock, WASockStruct, WAMessage, WAhWnd, StringConvertToCStr(WAIP), WAPort, WATimeOut));
}

char * CALLBACK Wrapper_SocketReceiveASCIIDatas(SOCKET WAhSock)
{
    return(StringCStrConvertToString(SocketReceiveASCIIDatas(WAhSock)));
}

long CALLBACK Wrapper_SocketSendASCIIDatasCR(SOCKET WAhSock, char *WABuffer)
{
    return(SocketSendASCIIDatasCR(WAhSock, StringConvertToCStr(WABuffer)));
}

long CALLBACK Wrapper_StatusBarSetText(HWND WAhStatusBar, long WASBPart, long WARaiseType, char *WASBText, long WAOwnerDatas)
{
    return(StatusBarSetText(WAhStatusBar, WASBPart, WARaiseType, StringConvertToCStr(WASBText), WAOwnerDatas));
}

long CALLBACK Wrapper_StringCount(char *WAMaIn_String, char *StringToCount, long WACountStart, long WACompareType)
{
    return(StringCount(StringConvertToCStr(WAMaIn_String), StringConvertToCStr(StringToCount), WACountStart, WACompareType));
}

char * CALLBACK Wrapper_StringReplace(char *WAMaIn_String, char *StringToFind, char *StringToReplace, long WAReplaceStart, long WARepCount, long WACompareType)
{
    return(StringCStrConvertToString(StringReplace(StringConvertToCStr(WAMaIn_String), StringConvertToCStr(StringToFind), StringConvertToCStr(StringToReplace), WAReplaceStart, WARepCount, WACompareType)));
}

char * CALLBACK Wrapper_StringGetSplitElement(char *WAStrFromSplit, long *WASplitMem, long WAElement)
{
    return(StringCStrConvertToString(StringGetSplitElement(StringConvertToCStr(WAStrFromSplit), WASplitMem, WAElement)));
}

char * CALLBACK Wrapper_StringHex16(long WANumber)
{
    return(StringCStrConvertToString(StringHex16(WANumber)));
}

char * CALLBACK Wrapper_StringHex32(long WANumber)
{
    return(StringCStrConvertToString(StringHex32(WANumber)));
}

char * CALLBACK Wrapper_StringHex8(long WANumber)
{
    return(StringCStrConvertToString(StringHex8(WANumber)));
}

long CALLBACK Wrapper_StringIsAlphaChar(char *WAALetter)
{
    return(StringIsAlphaChar(StringConvertToCStr(WAALetter)));
}

long CALLBACK Wrapper_StringIsArobasChar(char *WAPLetter)
{
    return(StringIsArobasChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsBlankChar(char *WABLetter)
{
    return(StringIsBlankChar(StringConvertToCStr(WABLetter)));
}

long CALLBACK Wrapper_StringIsDecimal(char *WADecText, long WAMaxLen)
{
    return(StringIsDecimal(StringConvertToCStr(WADecText), WAMaxLen));
}

long CALLBACK Wrapper_StringIsDigitChar(char *WADLetter)
{
    return(StringIsDigitChar(StringConvertToCStr(WADLetter)));
}

long CALLBACK Wrapper_StringIsDotChar(char *WAPLetter)
{
    return(StringIsDotChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsCommaChar(char *WAPLetter)
{
    return(StringIsCommaChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsQuoteChar(char *WAPLetter)
{
    return(StringIsQuoteChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsEOL(char *WAEOLetter)
{
    return(StringIsEOL(StringConvertToCStr(WAEOLetter)));
}

long CALLBACK Wrapper_StringIsHexaChar(char *WAALetter)
{
    return(StringIsHexaChar(StringConvertToCStr(WAALetter)));
}

long CALLBACK Wrapper_StringIsHexaDecimal(char *WAHexaText)
{
    return(StringIsHexaDecimal(StringConvertToCStr(WAHexaText)));
}

long CALLBACK Wrapper_StringIsLabel(char *WAAsmText)
{
    return(StringIsLabel(StringConvertToCStr(WAAsmText)));
}

long CALLBACK Wrapper_StringIsLabelChar(char *WAVCLetter)
{
    return(StringIsLabelChar(StringConvertToCStr(WAVCLetter)));
}

long CALLBACK Wrapper_StringIsOpenBracketChar(char *WAOLetter)
{
    return(StringIsOpenBracketChar(StringConvertToCStr(WAOLetter)));
}

long CALLBACK Wrapper_StringIsUnderLineChar(char *WAULetter)
{
    return(StringIsUnderLineChar(StringConvertToCStr(WAULetter)));
}

char * CALLBACK Wrapper_StringJoin(char *WAStrFromSplit, long *WASplitMem, char *WAJoIn_String, long WAFirstElement, long WAElementLimit)
{
    return(StringCStrConvertToString(StringJoin(StringConvertToCStr(WAStrFromSplit), WASplitMem, StringConvertToCStr(WAJoIn_String), WAFirstElement, WAElementLimit)));
}

char * CALLBACK Wrapper_StringNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringNumberComplement(WANbr, WAComplement)));
}

char * CALLBACK Wrapper_StringHexNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringHexNumberComplement(WANbr, WAComplement)));
}

char * CALLBACK Wrapper_StringBinNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringBinNumberComplement(WANbr, WAComplement)));
}

long * CALLBACK Wrapper_StringSplit(char *WAStrToSplit, char *WAStrSplitter)
{
    return(StringSplit(StringConvertToCStr(WAStrToSplit), StringConvertToCStr(WAStrSplitter)));
}

char * CALLBACK Wrapper_StringCat(char *String1, char *String2)
{
    return(StringCStrConvertToString(StringCat(StringConvertToCStr(String1), StringConvertToCStr(String2))));
}

char * CALLBACK Wrapper_StringDecToString(long WADecimalNbr)
{
    return(StringCStrConvertToString(StringDecToString(WADecimalNbr)));
}

long CALLBACK Wrapper_StringStringToDec(char *WADecString)
{
    return(StringStringToDec(StringConvertToCStr(WADecString)));
}

long CALLBACK Wrapper_StringStringToHex(char *WAHexString)
{
    return(StringStringToHex(StringConvertToCStr(WAHexString)));
}

long CALLBACK Wrapper_WASysTabAddItem(HWND WAhTab, char *WATabText, long WATabIndex, long WAImgIndex)
{
    return(WASysTabAddItem(WAhTab, StringConvertToCStr(WATabText), WATabIndex, WAImgIndex));
}

void CALLBACK Wrapper_WATextBoxAddText(HWND WAhTextBox, char *WATbText)
{
    WATextBoxAddText(WAhTextBox, StringConvertToCStr(WATbText));
}

char * CALLBACK Wrapper_WATextBoxGetPasswordChar(HWND WAhTextBox)
{
    return(StringCStrConvertToString(WATextBoxGetPasswordChar(WAhTextBox)));
}

long CALLBACK Wrapper_WATextBoxSetPasswordChar(HWND WAhTextBox, char *PassChar)
{
    return(WATextBoxSetPasswordChar(WAhTextBox, StringConvertToCStr(PassChar)));
}

long CALLBACK Wrapper_WAToolBarAddButton(HWND WAhToolbar, char *ButtonText, long WATBButtonID, long WAImgIndex, long ButtonStyle, long ButtonState, long WANoImage)
{
    return(WAToolBarAddButton(WAhToolbar, StringConvertToCStr(ButtonText), WATBButtonID, WAImgIndex, ButtonStyle, ButtonState, WANoImage));
}

void CALLBACK Wrapper_WAToolBarDisplayToolTip(char *WATextToShow, long WAlParam)
{
    WAToolBarDisplayToolTip(StringConvertToCStr(WATextToShow), WAlParam);
}

HTREEITEM CALLBACK Wrapper_WATreeViewAddItem(HWND WAhTreeView, char *WATVItemText, HTREEITEM WATVParent, HTREEITEM WATVChildPos, long WATVImage, long WATVSelImage, long WAExtraStyle, long WARefreshParentAfterAdd)
{
    return(WATreeViewAddItem(WAhTreeView, StringConvertToCStr(WATVItemText), WATVParent, WATVChildPos, WATVImage, WATVSelImage, WAExtraStyle, WARefreshParentAfterAdd));
}

char * CALLBACK Wrapper_WATreeViewGetItemText(HWND WAhTreeView, HTREEITEM WAhItem)
{
    return(StringCStrConvertToString(WATreeViewGetItemText(WAhTreeView, WAhItem)));
}

long CALLBACK Wrapper_WATreeViewSearchChildPartialText(HWND WAhTreeView, HTREEITEM WAhNode, char *WAItemText)
{
    return(WATreeViewSearchChildPartialText(WAhTreeView, WAhNode, StringConvertToCStr(WAItemText)));
}

long CALLBACK Wrapper_WATreeViewSearchItemText(HWND WAhTreeView, HTREEITEM WAhNode, char *WAItemText)
{
    return(WATreeViewSearchItemText(WAhTreeView, WAhNode, StringConvertToCStr(WAItemText)));
}

long CALLBACK Wrapper_WATreeViewSetItemText(HWND WAhTreeView, HTREEITEM WAhItem, char *WAItemText)
{
    return(WATreeViewSetItemText(WAhTreeView, WAhItem, StringConvertToCStr(WAItemText)));
}

char * CALLBACK StringLeft(char *String, long Dat)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Left(Dat)));
}

char * CALLBACK StringRight(char *String, long Dat)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Right(Dat)));
}

char * CALLBACK StringTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Trim()));
}

char * CALLBACK StringLTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).L_Trim()));
}

char * CALLBACK StringRTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).R_Trim()));
}

long CALLBACK StringInStr(char *String, long Start, char *WASearchString, long CompType)
{
	return(StringConvertToCStr(String).In_Str(Start, WASearchString, CompType));
}

char * CALLBACK StringMid(char *String, long Start, long Len)
{
	return(StringCStrConvertToString(StringConvertToCStr(String).Mid(Start, Len)));
}

char * CALLBACK StringUpperCase(char *String)
{
	return(StringCStrConvertToString(StringConvertToCStr(String).Upper_Case()));
}

char * CALLBACK StringLowerCase(char *String)
{
	return(StringCStrConvertToString(StringConvertToCStr(String).Lower_Case()));
}

char * CALLBACK StringReverse(char *String)
{
	return(StringCStrConvertToString(StringConvertToCStr(String).Str_Reverse()));
}

char * CALLBACK StringSpace(long Len)
{
	CStr BufString;
	return(StringCStrConvertToString(BufString.Space(Len)));
}

char * CALLBACK StringString(long Len, long Dat)
{
	CStr BufString;
	return(StringCStrConvertToString(BufString.String(Len, Dat)));
}

long CALLBACK StringIsNumeric(char *String)
{
	return(StringConvertToCStr(String).Is_Numeric());
}

// Future extensions
long CALLBACK WAMiscObtainExtendedFunctions(void)
{
    return(0);
}

HWND CALLBACK Wrapper_WACreateDockingBox(char *Title, WNDPROC WindowProc)
{
	return(WACreateDockingBox(StringConvertToCStr(Title), WindowProc, hMDIform.hDock, MainIniFile.Get_String()));
}

long CALLBACK Wrapper_DockingBoxWasVisible(char *DockName)
{
	return(DockingBoxWasVisible(StringConvertToCStr(DockName), MainIniFile));
}

void CALLBACK Wrapper_DockingBoxShow(HWND hDock, int DefaultXSize, int DefaultYSize, int DefaultState)
{
	DockingBoxShow(hDock, GetMDIWidth(), GetMDIHeight(), DefaultXSize, DefaultYSize, DefaultState);
}

// -----------------------------------------------------------------------
// Create a custom child Window
HWND CALLBACK Wrapper_WAMMCreateCustomChildDialog(char *Title, void (CALLBACK *CallBackInit)(HWND hwnd), WNDPROC CallBackProc)
{
    return(WAMMCreateCustomChildDialog(StringConvertToCStr(Title), CallBackInit, CallBackProc));
}

HWND WAMMCreateCustomChildDialog(CStr Title, void (CALLBACK *CallBackInit)(HWND hwnd), WNDPROC CallBackProc)
{
	HWND hWnd;

	CurCustChildInit = CallBackInit;
	CurCustChildProc = CallBackProc;
	CurCustChildTitle = Title;
	// Create a new child window
    if(NbForms != 0) if(WAControlGetWindowState(CurrentForm) == SW_SHOWMAXIMIZED) goto ForceMaximize;
	// Save arguments
    switch(AutoMaximizeChilds)
    {
        case 0:
            hWnd = WACreateMDIChildDialog(hMDIform.hClient, "", LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_SNAP)), 
                                          CustChildWinInitProc, CustChildWinProc, 0, 1, "");
			break;
		case 1:
ForceMaximize:
            hWnd = WACreateMDIChildDialog(hMDIform.hClient, "", LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_SNAP)),
                                          CustChildWinInitProc, CustChildWinProc, WS_MAXIMIZE, 1, "");
			break;
    }
    DrawMenuBar(hMDIform.hWnd);
	return(hWnd);
}

// -----------------------------------------------------------------------
// Custom child window initialization
void CALLBACK CustChildWinInitProc(HWND hWnd)
{
    LPCHILDDAT NewChildMem;

    NewChildMem = (LPCHILDDAT) AllocMem(sizeof(CHILDDAT));
    NewChildMem->RFile = new CStr;
    NewChildMem->Language = new CStr;
	NewChildMem->oldAPIFnc = new CStr;
	NewChildMem->oldAPILineFnc = new CStr;
	NewChildMem->FileDateOpen = (LPFILETIME) AllocMem(sizeof(FILETIME));
	NewChildMem->hChildCodeMax = 0;
	NewChildMem->FileLoaded = 0;
	// (Necessary)
	NewChildMem->ReadOnly = 1;
	// Save user callback in child space
	NewChildMem->CurCallBackProc = CurCustChildProc;
	// Set title
	NewChildMem->RFile->Set_String(CurCustChildTitle.Get_String());
	// Set the child text
	WAControlSetText(hWnd, CurCustChildTitle);
    CurrentForm = hWnd;
	// Now we can set the pointer to the structure
    SetWindowLong(hWnd, GWL_USERDATA, (long) NewChildMem);
    NbForms++;
    SendMessage(hMDIform.hClient, CHILD_MSG_CREATED, ICON_WINDOWLOCK, (LPARAM) hWnd);
	// Run user init now
	if(CurCustChildInit != NULL) CurCustChildInit(hWnd);
}

// -----------------------------------------------------------------------
// Child Winproc
LRESULT CALLBACK CustChildWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CHILDACTIVATE:
            CurrentForm = hWnd;
			break;
        case WM_SIZE:
            CurrentForm = hWnd;
			break;
        case WM_MOUSEACTIVATE:
            if(WAClientGetActiveChild(hMDIform.hClient) != hWnd)
            {
                CurrentForm = hWnd;
                SetFocus(CurrentForm);
				// Notify user in all cases
				ChildStruct = LoadStructure(hWnd);
				if(ChildStruct->CurCallBackProc != NULL)
				{
					// If callback returns 1 then abort the process
					if(ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam) == 1) return(0);
				}
                return(MA_ACTIVATE);
            }
			break;
        case WM_PAINT:
            CurrentForm = WAClientGetActiveChild(hMDIform.hClient);
			break;
		case WM_CLOSE:
			ChildStruct = LoadStructure(hWnd);
			if(ChildStruct->CurCallBackProc != NULL)
			{
				// If callback returns 1 then abort the process
				if(ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam) == 1) return(0);
			}
			// Delete them manually
			delete ChildStruct->RFile;
			delete ChildStruct->Language;
			delete ChildStruct->oldAPIFnc;
			delete ChildStruct->oldAPILineFnc;
			FreeMem((long) ChildStruct->FileDateOpen);
            // Free the child structure
            FreeMem(GetWindowLong(hWnd, GWL_USERDATA));
			// Put a fictive structure for following messages
			// (Re-use global one since it never changes)
			SetWindowLong(hWnd, GWL_USERDATA, (long) &ChildStructBack);
			// (User callback called above)
			return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
	    case WM_DESTROY:
            NbForms--;
            if(NbForms == 0) ClearStatusBarParts();
            // Notify the parent
			SendMessage(hMDIform.hClient, CHILD_MSG_DESTROYED, 0, (long) hWnd);
            return(0);
	}
	ChildStruct = LoadStructure(hWnd);
	if(ChildStruct->CurCallBackProc != NULL)
	{
		ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam);
    }
	return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Attach a codemax control to a child window
HWND CALLBACK WAMMCreateCodeMax(long Left, long Top, long Width, long Height, HWND hWnd, void (CALLBACK *CodeMaxPropertiesHook)(HWND))
{
	HWND hCodeMax;
	
	hCodeMax = CreateCodeMax(Left, Top, Width, Height, hWnd, ApphInstance, 1);
	if(hCodeMax == NULL) return(NULL);
	ChildStruct = LoadStructure(hWnd);
	ChildStruct->hChildCodeMax = hCodeMax;
	ChildStruct->CodeMaxPropertiesHook = CodeMaxPropertiesHook;
	// Init basic behaviours / styles
	InitMinimumCodeMax(hCodeMax);
	SetCodeMaxColors(hCodeMax);
    SetCodeMaxFont(hCodeMax);
    CM_SetLanguage(hCodeMax, "Assembler");
	DragAcceptFiles(hCodeMax, 0);
	// Run user props now
	CodeMaxPropertiesHook(hCodeMax);
	return(hCodeMax);
}
