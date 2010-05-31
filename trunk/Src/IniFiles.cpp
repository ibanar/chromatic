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
// IniFiles.cpp: Read ini files
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "APIDb.h"
#include "IniFiles.h"

// -----------------------------------------------------------------------
// Variables
CStr InitRetVal;

// -----------------------------------------------------------------------
// Read the Main.ini file
long ReadIniFile(void)
{
    int i = 0;
	long ReturnValue = 0;
    CStr TmpSysDir;
    CStr TmpExt;
    CStr BufString;
	
	RegisteredExts.Erase();
    // Read registered extensions
    for(i = 0; i <= 999; i++)
    {
        TmpExt = "Ext" + (CStr) StringNumberComplement(i, 3).Get_String();
        InitRetVal = WAIniReadKey("Extensions", TmpExt, MainIniFile);
        if(InitRetVal.Len() == 0) break;
        RegisteredExts.Add(InitRetVal.Get_String());
    }
    
    // Read working dir
    NotifiedCurrentdir = "";
    CurrentDir = AppPath;
    InitRetVal = WAIniReadKey("Dirs", "WORKDIR", MainIniFile);
	// Should be empty at first install: take application directory
	if(InitRetVal.Len() == 0) InitRetVal = AppPath;
    if(InitRetVal.Len() != 0)
    {
        CurrentDir = InitRetVal;
        ChDrive(CurrentDir);
        if(ChDir(CurrentDir) == 0)
        {
			NotifiedCurrentdir = CurrentDir;
			ReturnValue = 1;
			CurrentDir = "c:\\";
			WAIniWriteKey("Dirs", "WORKDIR", CurrentDir, MainIniFile);
			ChDrive(CurrentDir);
			ChDir(CurrentDir);
		}
    } 

    // Fill others directories + directories names replacements
    DirsRepl[DIR_ROOT] = "ROOTDIR";
    Dirs[DIR_ROOT] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_ROOT], MainIniFile).Get_String());
    DirsRepl[DIR_BIN] = "BINDIR";
    Dirs[DIR_BIN] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_BIN], MainIniFile).Get_String());
    DirsRepl[DIR_INCLUDE] = "INCLUDEDIR";
    Dirs[DIR_INCLUDE] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_INCLUDE], MainIniFile).Get_String());
    DirsRepl[DIR_LIB] = "LIBDIR";
    Dirs[DIR_LIB] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_LIB], MainIniFile).Get_String());
    DirsRepl[DIR_HELP] = "HELPDIR";
    Dirs[DIR_HELP] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_HELP], MainIniFile).Get_String());
    DirsRepl[DIR_PROJECTS] = "PROJECTSDIR";
    Dirs[DIR_PROJECTS] = strdup(WAIniReadKey("Dirs", DirsRepl[DIR_PROJECTS], MainIniFile).Get_String());
	// Perform a sanity check on the retrieved directory
    if(strlen(Dirs[DIR_PROJECTS]) == 0 || WAFileIsDirectory(Dirs[DIR_PROJECTS]) == 0)
    {
		free(Dirs[DIR_PROJECTS]);
		// Set default projects directory
		BufString = AppPath + (CStr) "\\Projects";
		Dirs[DIR_PROJECTS] = strdup(BufString.Get_String());
		// Update project dir
		// Note: at first install this directory must be empty.
		// (So it points to the correct one)
		WAIniWriteKey("Dirs", "PROJECTSDIR", BufString, MainIniFile);
    }
    
	// Application directory
    Dirs[DIR_MAIN] = strdup(AppPath.Get_String());
    DirsRepl[DIR_MAIN] = "MAINDIR";
    WAIniWriteKey("Dirs", "MAINDIR", Dirs[DIR_MAIN], MainIniFile);
    
	BufString = AppPath + (CStr) "\\Tools";
	Dirs[DIR_TOOLS] = strdup(BufString.Get_String());
    DirsRepl[DIR_TOOLS] = "TOOLSDIR";
    WAIniWriteKey("Dirs", "TOOLSDIR", Dirs[DIR_TOOLS], MainIniFile);

	BufString = AppPath + (CStr) "\\Templates";
	Dirs[DIR_TEMPLATES] = strdup(BufString.Get_String());
    DirsRepl[DIR_TEMPLATES] = "TEMPLATESDIR";
    WAIniWriteKey("Dirs", "TEMPLATESDIR", Dirs[DIR_TEMPLATES], MainIniFile);

	BufString = AppPath + (CStr) "\\Help";
    Dirs[DIR_MAINHELP] = strdup(BufString.Get_String());
    DirsRepl[DIR_MAINHELP] = "MAINHELPDIR";
    WAIniWriteKey("Dirs", "MAINHELPDIR", Dirs[DIR_MAINHELP], MainIniFile);

	BufString = AppPath + (CStr) "\\Snippets";
    Dirs[DIR_SNIPPETS] = strdup(BufString.Get_String());
    DirsRepl[DIR_SNIPPETS] = "SNIPPETSDIR";
    WAIniWriteKey("Dirs", "SNIPPETSDIR", Dirs[DIR_SNIPPETS], MainIniFile);

	BufString = AppPath + (CStr) "\\Databases";
    Dirs[DIR_DATABASES] = strdup(BufString.Get_String());
    DirsRepl[DIR_DATABASES] = "DATABASESDIR";
    WAIniWriteKey("Dirs", "DATABASESDIR", Dirs[DIR_DATABASES], MainIniFile);

	BufString = AppPath + (CStr) "\\Skins";
    Dirs[DIR_SKINS] = strdup(BufString.Get_String());
    DirsRepl[DIR_SKINS] = "SKINSDIR";
    WAIniWriteKey("Dirs", "SKINSDIR", Dirs[DIR_SKINS], MainIniFile);

	BufString = AppPath + (CStr) "\\Languages";
    Dirs[DIR_LANGUAGES] = strdup(BufString.Get_String());
    DirsRepl[DIR_LANGUAGES] = "LANGUAGESDIR";
    WAIniWriteKey("Dirs", "LANGUAGESDIR", Dirs[DIR_LANGUAGES], MainIniFile);
    
	BufString = AppPath + (CStr) "\\Languages\\Scripts";
    Dirs[DIR_LANGSCRIPTS] = strdup(BufString.Get_String());
    DirsRepl[DIR_LANGSCRIPTS] = "LANGSCRIPTSDIR";
    WAIniWriteKey("Dirs", "LANGSCRIPTSDIR", Dirs[DIR_LANGSCRIPTS], MainIniFile);

	BufString = AppPath + (CStr) "\\Downloads";
	Dirs[DIR_DOWNLOADS] = strdup(BufString.Get_String());
    DirsRepl[DIR_DOWNLOADS] = "DOWNLOADSDIR";
    WAIniWriteKey("Dirs", "DOWNLOADSDIR", Dirs[DIR_DOWNLOADS], MainIniFile);
    
	BufString = AppPath + (CStr) "\\AddIns";
	Dirs[DIR_ADDINS] = strdup(BufString.Get_String());
    DirsRepl[DIR_ADDINS] = "ADDINSDIR";
    WAIniWriteKey("Dirs", "ADDINSDIR", Dirs[DIR_ADDINS], MainIniFile);
    
	BufString = AppPath + (CStr) "\\Config";
    Dirs[DIR_CONFIG] = strdup(BufString.Get_String());
    DirsRepl[DIR_CONFIG] = "CONFIGDIR";
    WAIniWriteKey("Dirs", "CONFIGDIR", Dirs[DIR_CONFIG], MainIniFile);
    
	BufString = Dirs[DIR_CONFIG] + (CStr) "\\UserMenus";
    Dirs[DIR_MENUS] = strdup(BufString.Get_String());
    DirsRepl[DIR_MENUS] = "MENUSDIR";
    WAIniWriteKey("Dirs", "MENUSDIR", Dirs[DIR_MENUS], MainIniFile);

    BufString = AppPath + (CStr) "\\Wizards";
    Dirs[DIR_WIZARDS] = strdup(BufString.Get_String());
    DirsRepl[DIR_WIZARDS] = "WIZARDSDIR";
    WAIniWriteKey("Dirs", "WIZARDSDIR", Dirs[DIR_WIZARDS], MainIniFile);

    BufString = AppPath + (CStr) "\\Scripts";
    Dirs[DIR_SCRIPTS] = strdup(BufString.Get_String());
    DirsRepl[DIR_SCRIPTS] = "SCRIPTSDIR";
    WAIniWriteKey("Dirs", "SCRIPTSDIR", Dirs[DIR_SCRIPTS], MainIniFile);

    BufString = AppPath + (CStr) "\\Filters";
    Dirs[DIR_FILTERS] = strdup(BufString.Get_String());
    DirsRepl[DIR_FILTERS] = "FILTERSDIR";
    WAIniWriteKey("Dirs", "FILTERSDIR", Dirs[DIR_FILTERS], MainIniFile);

    BufString = Dirs[DIR_SKINS] + (CStr) "\\ExtraCode";
    Dirs[DIR_EXTRACODE] = strdup(BufString.Get_String());
    DirsRepl[DIR_EXTRACODE] = "EXTRACODEDIR";
    WAIniWriteKey("Dirs", "EXTRACODEDIR", Dirs[DIR_EXTRACODE], MainIniFile);

    /*BufString = Dirs[DIR_TOOLS] + (CStr) "\\Tools";
    Dirs[DIR_TOOLS] = strdup(BufString.Get_String());
    DirsRepl[DIR_TOOLS] = "TOOLSDIR";
    WAIniWriteKey("Dirs", "TOOLSDIR", Dirs[DIR_TOOLS], MainIniFile);*/

    // Handle databases
    APIFile = Dirs[DIR_DATABASES] + (CStr) "\\WinDetails.db";
    InitRetVal = WAIniReadKey("Databases", "WinDetail", MainIniFile);
    if(InitRetVal.Len() != 0) APIFile = InitRetVal;
    WAIniWriteKey("Databases", "WinDetail", APIFile, MainIniFile);
    APIFile = ChangeRelativePaths(APIFile);

    APIFncFile = Dirs[DIR_DATABASES] + (CStr) "\\WinFnc.db";
    InitRetVal = WAIniReadKey("Databases", "WinFnc", MainIniFile);
    if(InitRetVal.Len() != 0) APIFncFile = InitRetVal;
    WAIniWriteKey("Databases", "WinFnc", APIFncFile, MainIniFile);
    APIFncFile = ChangeRelativePaths(APIFncFile);
    
    APIConstFile = Dirs[DIR_DATABASES] + (CStr) "\\WinConst.db";
    InitRetVal = WAIniReadKey("Databases", "WinConst", MainIniFile);
    if(InitRetVal.Len() != 0) APIConstFile = InitRetVal;
    WAIniWriteKey("Databases", "WinConst", APIConstFile, MainIniFile);
    APIConstFile = ChangeRelativePaths(APIConstFile);

	// Set default value
    TipFileName = Dirs[DIR_MAIN] + (CStr) "\\Tips.txt";
    // Or take the one found ini main.ini
	InitRetVal = WAIniReadKey("Layout", "TipsFile", MainIniFile);
    if(InitRetVal.Len() != 0) TipFileName = InitRetVal;
    TipFileName = ChangeRelativePaths(TipFileName);
    
    // Addins directory by default
    LastAddInDir = Dirs[DIR_PROJECTS];
    
    // Retrieve windows directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetWindowsDirectory(TmpSysDir.Get_String(), MAX_PATH);
	DirsRepl[DIR_WIN] = "WINDIR";
    Dirs[DIR_WIN] = "";
    if(TmpSysDir.Len() != 0)
    {
        Dirs[DIR_WIN] = strdup(TmpSysDir.Get_String());
    }
    
    // Retrieve system directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetSystemDirectory(TmpSysDir.Get_String(), MAX_PATH);
    DirsRepl[DIR_WINSYS] = "WINSYSDIR";
    Dirs[DIR_WINSYS] = "";
    if(TmpSysDir.Len() != 0)
    {
        Dirs[DIR_WINSYS] = strdup(TmpSysDir.Get_String());
    }
    
    // Get temporary directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetTempPath(MAX_PATH, TmpSysDir.Get_String());
    DirsRepl[DIR_TEMP] = "TEMPDIR";
    Dirs[DIR_TEMP] = "";
    if(TmpSysDir.Len() != 0)
    {
        if(strcmp(TmpSysDir.Right(1).Get_String(), "\\") == 0) Dirs[DIR_TEMP] = strdup(TmpSysDir.Left(TmpSysDir.Len() - 1).Get_String());
        else Dirs[DIR_TEMP] = strdup(TmpSysDir.Get_String());
		Dirs[DIR_TEMP] = strdup(TmpSysDir.Get_String());
    }
    
    // Read various layouts
    ShowStatusbar = 1;
    InitRetVal = WAIniReadKey("Layout", "StatusBar", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowStatusbar = 0;
    
	ShowProjetDockWin = 1;
    InitRetVal = WAIniReadKey("Layout", "DockProjectVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowProjetDockWin = 0;

	ShowWindowsDockWin = 1;
    InitRetVal = WAIniReadKey("Layout", "DockWindowsVisible", MainIniFile);
	if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowWindowsDockWin = 0;

	ShowOutputDockWin = 1;
    InitRetVal = WAIniReadKey("Layout", "DockOutputVisible", MainIniFile);
	if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowOutputDockWin = 0;

	ShowDebugDockWin = 1;
    InitRetVal = WAIniReadKey("Layout", "DockDebugVisible", MainIniFile);
	if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowDebugDockWin = 0;
    
	ShowRegistersDockWin = 1;
    InitRetVal = WAIniReadKey("Layout", "DockRegistersVisible", MainIniFile);
	if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) ShowRegistersDockWin = 0;
    
	LeftM = 0;
    InitRetVal = WAIniReadKey("Layout", "Margin", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) LeftM = 1;
    White = 0;
    InitRetVal = WAIniReadKey("Layout", "WhiteSpace", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) White = 1;
    ShowH = 0;
    InitRetVal = WAIniReadKey("Layout", "Highlight", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) ShowH = 1;
    SyntaxH = 0;
    InitRetVal = WAIniReadKey("Layout", "Syntax", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) SyntaxH = 1;
    OutputDates = 0;
    InitRetVal = WAIniReadKey("Layout", "OutputDates", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) OutputDates = 1;
    IndentScope = 0;
    InitRetVal = WAIniReadKey("Layout", "IndentScope", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) IndentScope = 1;
    EnDragDrop = 1;
    InitRetVal = WAIniReadKey("Layout", "EnDragDrop", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "0") == 0) EnDragDrop = 0;
    Remember = 0;
    InitRetVal = WAIniReadKey("Layout", "Remember", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) Remember = 1;
    LineTTip = 0;
    InitRetVal = WAIniReadKey("Layout", "LineToolTip", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) LineTTip = 1;
    APITTip = 0;
    InitRetVal = WAIniReadKey("Layout", "APIToolTip", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) APITTip = 1;
    Smooth = 0;
    InitRetVal = WAIniReadKey("Layout", "SmoothScrolling", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) Smooth = 1;
    CursorFollow = 0;
    InitRetVal = WAIniReadKey("Layout", "FollowScrolling", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) CursorFollow = 1;
    AcceptFiles = 0;
    InitRetVal = WAIniReadKey("Layout", "AcceptFiles", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AcceptFiles = 1;
    ItalicFont = 0;
    InitRetVal = WAIniReadKey("Layout", "CommentItalic", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) ItalicFont = 1;
    Wrap = 0;
    InitRetVal = WAIniReadKey("Layout", "WordWrap", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) Wrap = 1;
    AutoSaveDelay = 0;
    InitRetVal = WAIniReadKey("Layout", "AutoSaveDelay", MainIniFile);
    if(InitRetVal.Len() != 0) AutoSaveDelay = InitRetVal.Get_Long();
    VerboseSave = 0;
    InitRetVal = WAIniReadKey("Layout", "VerboseSave", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) VerboseSave = 1;
    AutoSave = 0;
    InitRetVal = WAIniReadKey("Layout", "AutoSave", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AutoSave = 1;
    AutoExpand = 0;
    InitRetVal = WAIniReadKey("Layout", "ExpandTabs", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AutoExpand = 1;
    CreateBak = 0;
    InitRetVal = WAIniReadKey("Layout", "CreateBak", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) CreateBak = 1;
    DecorateBak = 0;
    InitRetVal = WAIniReadKey("Layout", "DecorateBak", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) DecorateBak = 1;
    HorizSplit = 0;
    InitRetVal = WAIniReadKey("Layout", "HorizSplit", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) HorizSplit = 1;
    DetectChange = 0;
    InitRetVal = WAIniReadKey("Layout", "DetectChange", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) DetectChange = 1;
    AutoCorrectKey = 0;
    InitRetVal = WAIniReadKey("Layout", "AutoCorrect", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AutoCorrectKey = 1;
    MaxRecent = 10;
    InitRetVal = WAIniReadKey("Layout", "Recent", MainIniFile);
    if(InitRetVal.Len() != 0) MaxRecent = InitRetVal.Get_Long();
    MaxRecentPrj = 5;
    InitRetVal = WAIniReadKey("Layout", "RecentPrj", MainIniFile);
    if(InitRetVal.Len() != 0) MaxRecentPrj = InitRetVal.Get_Long();
    LnStyle = 0;
    InitRetVal = WAIniReadKey("Layout", "LinesStyle", MainIniFile);
    if(InitRetVal.Len() != 0) LnStyle = InitRetVal.Get_Long();
	LnStart = 0;
    InitRetVal = WAIniReadKey("Layout", "LinesStart", MainIniFile);
    if(InitRetVal.Len() != 0) LnStart = InitRetVal.Get_Long();
    AutoMaximizeChilds = 0;
    InitRetVal = WAIniReadKey("Layout", "AutoMax", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AutoMaximizeChilds = 1;
    AutoClearOut = 0;
    InitRetVal = WAIniReadKey("Layout", "AutoClear", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) AutoClearOut = 1;
    Tabulations = 8;
    InitRetVal = WAIniReadKey("Layout", "Tabs", MainIniFile);
    if(InitRetVal.Len() != 0) Tabulations = InitRetVal.Get_Long();
    CurFontName = WAIniReadKey("Layout", "FontName", MainIniFile);
    InitRetVal = WAIniReadKey("Layout", "FontSize", MainIniFile);
    CurFontSize = 9;
    if(CurFontName.Len() == 0) CurFontName = "Courier New";
    if(InitRetVal.Len() != 0) CurFontSize = InitRetVal.Get_Long();
    CurFontBold = 0;
    InitRetVal = WAIniReadKey("Layout", "FontBold", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) CurFontBold = 1;
    if(CurFontName.Len() == 0) CurFontName = "Lucida console";
    if(CurFontSize == 0) CurFontSize = 8;
    UseWinRecent = 0;
    InitRetVal = WAIniReadKey("Layout", "WinRecent", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) UseWinRecent = 1;
    FullScreenMode = 0;
    InitRetVal = WAIniReadKey("Layout", "FullScreen", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) FullScreenMode = 1;
    // Load cgrep file
    cGrepIniFile = WAIniReadKey("Layout", "cGrepFile", MainIniFile);
    cGrepIniFile = ChangeRelativePaths(cGrepIniFile);
    ReadColorsDefinition();
    UseDbFnc = 0;
    InitRetVal = WAIniReadKey("Layout", "UseDbFnc", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) UseDbFnc = 1;
    UseDbConst = 0;
    InitRetVal = WAIniReadKey("Layout", "UseDbConst", MainIniFile);
    if(InitRetVal.Len() != 0) if(strcmpi(InitRetVal.Get_String(), "1") == 0) UseDbConst = 1;
    DbLinesNumbers = 6;
    InitRetVal = WAIniReadKey("Layout", "DbLinesNumbers", MainIniFile);
    if(InitRetVal.Len() != 0) DbLinesNumbers = InitRetVal.Get_Long();
    if(DbLinesNumbers < 1) DbLinesNumbers = 1;
    // User menus by default
    UserMenusIniFile = AppPath + "\\Config\\UserMenus\\Default.ume";
    InitRetVal = WAIniReadKey("Layout", "CurrentMenus", MainIniFile);
    InitRetVal = ChangeRelativePaths(InitRetVal);
    if(InitRetVal.Len() != 0) UserMenusIniFile = InitRetVal;
    if(WAFileExist(UserMenusIniFile) == 0) UserMenusIniFile = AppPath + "\\Config\\UserMenus\\Default.ume";
	UpdateResFiltersIni();
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve stand colors from ini file
void ReadColorsDefinition(void)
{
    CurBackColor = 0x543243;
    CurNormTextColor = 0xAAAAAA;
    CurHighTextColor = 0xFFFF;
    CurMarginColor = 0x433254;
    CurLineColor = 0xFFAAAA;
    // Defaults cols
    // (they sucks i know)
    LangNumbersColor = 0xFFAAAA;
    LangStringsColor = 0xAAFFAA;
    LangCommentsColor = 0x55FFAA;
    LangKeyWordsColor = 0xAAFF55;
    LangColorAColor = 0xFF55AA;
    LangColorBColor = 0xFF55FF;
    LangColorCColor = 0xFFAA55;
    LangColorDColor = 0xAA55FF;
    LangColorEColor = 0x55AAFF;
    APIColor = 0xFFFFFF;
    LangColorFColor = 0xFFFFAA;
    LangColorGColor = 0xAAFFFF;
    LangColorHColor = 0xFFFFFF;
    InitRetVal = WAIniReadKey("Layout", "BackColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurBackColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "NormText", MainIniFile);
    if(InitRetVal.Len() != 0) CurNormTextColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "HighText", MainIniFile);
    if(InitRetVal.Len() != 0) CurHighTextColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "MarginColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurMarginColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "LinesColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurLineColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "NumbersColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangNumbersColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "StringsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangStringsColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "CommentsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangCommentsColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "KeyWordsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangKeyWordsColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorAValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorAColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorBValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorBColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorCValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorCColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorDValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorDColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorEValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorEColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "APIHighColor", MainIniFile);
    if(InitRetVal.Len() != 0) APIColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorFValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorFColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorGValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorGColor = InitRetVal.Get_Hex();
    InitRetVal = WAIniReadKey("Layout", "ColorHValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorHColor = InitRetVal.Get_Hex();
}

// -----------------------------------------------------------------------
// Refresh stock resources files dialogs filters if necessary
// (Weren't present in version < 0.35)
void UpdateResFiltersIni(void)
{
    InitRetVal = WAIniReadKey("ExtrasExt", "AddInclude", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddInclude", "Source files|*.inc;*.a;*.asm;*.ash;*.asi;*.s;*.a86;*.c;*.cc;*.cpp;*.i;*.h;*.mac;*.hpp;*.pp;*.pas;*.bas|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddLibrary", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddLibrary", "Library files|*.lib;*.a;*.ppw;*.ow|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddModule", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddModule", "Source files|*.a;*.asm;*.s;*.a86;*.c;*.cc;*.cpp;*.pas;*.pp;*.bas|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddObject", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddObject", "Object files|*.obj;*.o|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddResource", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddResource", "Resources files|*.rc|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddText", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddText", "Text files|*.txt|Ini files|*.ini|Batch files|*.bat|Log files|*.log|Registry files|*.reg|All files|*.*", MainIniFile);
    InitRetVal = WAIniReadKey("ExtrasExt", "AddRawDatas", MainIniFile);
    if(InitRetVal.Len() == 0) WAIniWriteKey("ExtrasExt", "AddRawDatas", "All files|*.*", MainIniFile);
}
