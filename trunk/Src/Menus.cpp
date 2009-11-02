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
// Menus.cpp: Menus related functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Menus.h"
#include "AddIns.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"

// -----------------------------------------------------------------------
// Variables
HMENU hMainMenu;
HMENU hFileMenu;

HMENU hFileMenuNew;
HMENU hFileMenuOpen;
HMENU hFileMenuInclude;
HMENU hFileMenuSave;
HMENU hFileMenuConsole;
HMENU hFileMenuRunProg;
HMENU hFileMenuFavorites;
HMENU hFileMenuFilters;
HMENU hFileMenuIncludeFilters;
HMENU hEditMenuCopy;
HMENU hEditMenuPaste;
HMENU hEditMenuDelete;
HMENU hEditMenuBookmark;
HMENU hEditMenuBreakpoint;
HMENU hEditMenuProc;
HMENU hEditMenuGoto;
HMENU hEditMenuBlock;
HMENU hMenuRecent = 0;
HMENU hMenuRecentPrj = 0;
HMENU hViewMenuDisplay;
HMENU SubMenusArray[MAXSUBMENUS + 1];

HMENU hEditMenu;
HMENU hViewMenu;
HMENU hProjectMenu;
HMENU hProjectAddToMenu;
HMENU hProjectAddResToMenu;
HMENU hWindowMenu;
HMENU hEditAddInsMenu;

long EntryInRecent;
char *MenuFileComments[MENU_FILE_LAST_ID + 1];
char *MenuEditComments[MENU_EDIT_LAST_ID + 1];
char *MenuViewComments[MENU_VIEW_LAST_ID + 1];
char *MenuProjectComments[MENU_PROJECT_LAST_ID + 1];
char *MenuAddInComments[MENU_ADDINS_MANAGER_ID + 1];
char *MenuWindowComments[MENU_WINDOW_LAST_ID + 1];
char *MenuCodeMaxComments[MENU_CONTEXT_LAST_ID + 1];

char *MenuFileToolTips[MENU_FILE_LAST_ID + 1];
char *MenuEditToolTips[MENU_EDIT_LAST_ID + 1];
char *MenuViewToolTips[MENU_VIEW_LAST_ID + 1];
char *MenuProjectToolTips[MENU_PROJECT_LAST_ID + 1];
char *MenuWindowToolTips[MENU_WINDOW_LAST_ID + 1];

CList <int> MenuCommandsID;
CList <int> MenuRealCommandsID;
CStr MainMenusRetVal;
CM_HOTKEY CHotKey;

// -----------------------------------------------------------------------
// Create the main menu bar
void CreateMenuBar(HWND hParent)
{
    hMainMenu = CreateMenu();
    SetMenu(hParent, hMainMenu);
    CreateFileMenu(hMainMenu, "&File", MENU_FILE_IDBASE);
    CreateEditMenu(hMainMenu, "&Edit", MENU_EDIT_IDBASE);
    CreateViewMenu(hMainMenu, "&View", MENU_VIEW_IDBASE);
    CreateProjectMenu(hMainMenu, "&Project", MENU_PROJECT_IDBASE);
    CreateAddInsMenu(hMainMenu, "&AddIns", MENU_ADDINS_IDBASE);
    CreateWindowMenu(hMainMenu, "&Window", MENU_WINDOW_IDBASE);
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
    SetMenus();
    SubMenusArray[0] = hFileMenuOpen;
    SubMenusArray[1] = hFileMenuSave;
    SubMenusArray[2] = hEditMenuCopy;
    SubMenusArray[3] = hEditMenuDelete;
    SubMenusArray[4] = hFileMenuConsole;
    SubMenusArray[5] = hEditMenuBookmark;
    SubMenusArray[6] = hEditMenuProc;
    SubMenusArray[7] = hEditMenuGoto;
    SubMenusArray[8] = hEditMenuBlock;
    SubMenusArray[9] = hMenuRecent;
    SubMenusArray[10] = hMenuRecentPrj;
    SubMenusArray[11] = hProjectAddToMenu;
    SubMenusArray[12] = hFileMenuInclude;
    SubMenusArray[13] = hFileMenuNew;
    SubMenusArray[14] = hEditMenuPaste;
    SubMenusArray[15] = hFileMenuRunProg;
    SubMenusArray[16] = hFileMenuFavorites;
    SubMenusArray[17] = hFileMenuFilters;
    SubMenusArray[18] = hFileMenuIncludeFilters;
    SubMenusArray[19] = hEditMenuBreakpoint;
    SubMenusArray[20] = hViewMenuDisplay;
}

// -----------------------------------------------------------------------
// Create the "File" menu
void CreateFileMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hFileMenu = CreatePopupMenu();
    CreateNewMenu(BaseID);
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuNew, "New");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_CLEAR_ID, "Clear");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_DUPLICATEFILE_ID, "Duplicate file");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_RELOADFILE_ID, "Reload file\tCtrl+Shift+N");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");

	// ------
    hFileMenuOpen = CreatePopupMenu();
    AppendMenu(hFileMenuOpen, MF_STRING, BaseID + MENU_FILE_OPENFILE_ID, "Source file(s)...\tCtrl+O");
    WAMenuSetDefaultItem(hFileMenuOpen, 0);
    hFileMenuFilters = CreatePopupMenu();
    CreateFiltersMenu(hFileMenuFilters, 17, BaseID + MENU_FILE_FILTERS_ID, "Laboratory...\tCtrl+Shift+O", 0);
	AppendMenu(hFileMenuOpen, MF_POPUP, (UINT) hFileMenuFilters, "Filters");
    AppendMenu(hFileMenuOpen, MF_STRING, BaseID + MENU_FILE_OPENWORKSPACE_ID, "Workspace...");
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuOpen, "Open");
    
	// ------
	hFileMenuInclude = CreatePopupMenu();
    AppendMenu(hFileMenuInclude, MF_STRING, BaseID + MENU_FILE_INCLUDEFILE_ID, "Source file(s)...\tCtrl+I");
    WAMenuSetDefaultItem(hFileMenuInclude, 0);
	hFileMenuIncludeFilters = CreatePopupMenu();
    CreateFiltersMenu(hFileMenuIncludeFilters, 18, BaseID + MENU_FILE_INCLUDEFILTERS_ID, "Laboratory...\tCtrl+Shift+I", 1);
    AppendMenu(hFileMenuInclude, MF_POPUP, (UINT) hFileMenuIncludeFilters, "Filters");
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuInclude, "Include");
	// ------

    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_FTPMANAGER_ID, "FTP manager...");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_CLOSE_ID, "Close\tCtrl+F4");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_CLOSEALL_ID, "Close all");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    hFileMenuSave = CreatePopupMenu();
    AppendMenu(hFileMenuSave, MF_STRING, BaseID + MENU_FILE_SAVE_ID, "Current file\tCtrl+S");
    WAMenuSetDefaultItem(hFileMenuSave, 0);
    AppendMenu(hFileMenuSave, MF_STRING, BaseID + MENU_FILE_SAVEAS_ID, "To new file...\tAlt+Shift+S");
    AppendMenu(hFileMenuSave, MF_STRING, BaseID + MENU_FILE_SAVEALL_ID, "All opened");
    AppendMenu(hFileMenuSave, MF_STRING, BaseID + MENU_FILE_SAVEWORKSPACE_ID, "To workspace...");
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuSave, "Save");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_REGISTERASSNIPPET_ID, "Register as code snippet...\tCtrl+H");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_REGISTERASTEMPLATE_ID, "Register as source template...\tCtrl+Shift+H");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_PRINTFILE_ID, "Print file...\tCtrl+P");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    hFileMenuConsole = CreatePopupMenu();
    AppendMenu(hFileMenuConsole, MF_STRING, BaseID + MENU_FILE_CONSOLE_RUN_ID, "Run\tF5");
    WAMenuSetDefaultItem(hFileMenuConsole, 0);
    AppendMenu(hFileMenuConsole, MF_STRING, BaseID + MENU_FILE_CONSOLE_CHANGE_ID, "Change...");
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuConsole, "Console prompt");
    hFileMenuRunProg = CreatePopupMenu();
    CreateRunProgMenu();
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuRunProg, "External programs");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    hFileMenuFavorites = CreatePopupMenu();
    CreateFavoritesMenu();
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuFavorites, "Favorite files");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_SETWORKINGDIRECTORY_ID, "Set working directory...\tF12");
    AppendMenu(hFileMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hFileMenu, MF_STRING, BaseID + MENU_FILE_EXIT_ID, "Exit\tCtrl+Q");
    AppendMenu(hParent, MF_POPUP, (UINT) hFileMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "New" menu
void CreateNewMenu(long BaseID)
{
    hFileMenuNew = CreatePopupMenu();
    AppendMenu(hFileMenuNew, MF_STRING, BaseID + MENU_FILE_NEW_ID, "Empty source\tCtrl+N");
    WAMenuSetDefaultItem(hFileMenuNew, 0);
    CreateTemplateMenu(hFileMenuNew);
    SubMenusArray[13] = hFileMenuNew;
}

// -----------------------------------------------------------------------
// Create the "Edit" menu
void CreateEditMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hEditMenu = CreatePopupMenu();
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_UNDO_ID, "Undo\tCtrl+Z");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_REDO_ID, "Redo\tCtrl+Y");
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_CUT_ID, "Cut\tCtrl+X");
    hEditMenuCopy = CreatePopupMenu();
    AppendMenu(hEditMenuCopy, MF_STRING, BaseID + MENU_EDIT_COPY_ID, "Copy selection\tCtrl+C");
    WAMenuSetDefaultItem(hEditMenuCopy, 0);
    AppendMenu(hEditMenuCopy, MF_STRING, BaseID + MENU_EDIT_COPYFILEPATHNAME_ID, "Copy file path/name");
    AppendMenu(hEditMenuCopy, MF_STRING, BaseID + MENU_EDIT_COPYENTIRETEXT_ID, "Copy entire text");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuCopy, "Copy");
    
    hEditMenuPaste = CreatePopupMenu();
    AppendMenu(hEditMenuPaste, MF_STRING, BaseID + MENU_EDIT_PASTE_ID, "To current window\tCtrl+V");
    WAMenuSetDefaultItem(hEditMenuPaste, 0);
    AppendMenu(hEditMenuPaste, MF_STRING, BaseID + MENU_EDIT_PASTE_NEWWINDOW_ID, "To new window\tCtrl+Shift+V");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuPaste, "Paste");
    
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    hEditMenuDelete = CreatePopupMenu();
    AppendMenu(hEditMenuDelete, MF_STRING, BaseID + MENU_EDIT_DELETE_DELETESELECTION_ID, "Delete selection\tDel");
    WAMenuSetDefaultItem(hEditMenuDelete, 0);
    AppendMenu(hEditMenuDelete, MF_STRING, BaseID + MENU_EDIT_DELETE_DELETELINE_ID, "Delete line\tCtrl+E");
    AppendMenu(hEditMenuDelete, MF_STRING, BaseID + MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID, "Delete to start of line\tAlt+Bksp");
    AppendMenu(hEditMenuDelete, MF_STRING, BaseID + MENU_EDIT_DELETE_DELETETOENDOFLINE_ID, "Delete to end of line\tAlt+Del");
    AppendMenu(hEditMenuDelete, MF_STRING, BaseID + MENU_EDIT_DELETE_DELETEPARAGRAPH_ID, "Delete paragraph\tCtrl+Alt+K");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuDelete, "Delete");
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_SELECTLINE_ID, "Select line");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_SELECTALL_ID, "Select all\tCtrl+A");
    hEditMenuProc = CreatePopupMenu();
    AppendMenu(hEditMenuProc, MF_STRING, BaseID + MENU_EDIT_SELECTPROCEDURE_ID, "Select\tCtrl+Shift+A");
    WAMenuSetDefaultItem(hEditMenuProc, 0);
    AppendMenu(hEditMenuProc, MF_STRING, BaseID + MENU_EDIT_SELECTPROCANDCOLLAPSE_ID, "Select and collapse\tCtrl+Shift+Q");
    AppendMenu(hEditMenuProc, MF_STRING, BaseID + MENU_EDIT_UNCOLLAPSEPROCEDURE_ID, "Expand\tCtrl+Shift+W");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuProc, "Procedure");
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_FIND_ID, "Find...\tCtrl+F");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_FINDSELECTION_ID, "Find fast\tCtrl+Shift+F3");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_FINDNEXT_ID, "Find next\tF3");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_FINDPREVIOUS_ID, "Find previous\tCtrl+F3");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_REPLACE_ID, "Replace...\tF4");
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_PROCVARIABLESSEARCH_ID, "Proc/Var/Const search...\tCtrl+T");
    AppendMenu(hEditMenu, MF_STRING, BaseID + MENU_EDIT_SEARCHINFILES_ID, "Search in files...\tCtrl+B");
    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    hEditMenuBookmark = CreatePopupMenu();
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_TOGGLE_ID, "Toggle\tF2");
    WAMenuSetDefaultItem(hEditMenuBookmark, 0);
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_FIRST_ID, "First\tCtrl+F7");
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_PREVIOUS_ID, "Previous\tShift+F2");
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_NEXT_ID, "Next\tCtrl+F2");
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_LAST_ID, "Last\tCtrl+Shift+F7");
    AppendMenu(hEditMenuBookmark, MF_STRING, BaseID + MENU_EDIT_BOOKMARKS_CLEARALL_ID, "Clear all\tCtrl+Shift+F2");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBookmark, "Bookmarks");

    hEditMenuBreakpoint = CreatePopupMenu();
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_TOGGLE_ID, "Toggle\tF10");
    WAMenuSetDefaultItem(hEditMenuBreakpoint, 0);
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_FIRST_ID, "First\tCtrl+F11");
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_PREVIOUS_ID, "Previous\tShift+F10");
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_NEXT_ID, "Next\tCtrl+F10");
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_LAST_ID, "Last\tCtrl+Shift+F11");
    AppendMenu(hEditMenuBreakpoint, MF_STRING, BaseID + MENU_EDIT_BREAKPOINTS_CLEARALL_ID, "Clear all\tCtrl+Shift+F10");
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBreakpoint, "Code breakpoints");

    AppendMenu(hEditMenu, MF_SEPARATOR, (UINT) -1, "-");
    hEditMenuGoto = CreatePopupMenu();
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_TOP_ID, "Top\tCtrl+Home");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_BOTTOM_ID, "Bottom\tCtrl+End");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_LINE_ID, "Line\tCtrl+G");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_MATCHINGBRACKET_ID, "Matching bracket\tCtrl+[");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PREVIOUSWORD_ID, "Previous word\tCtrl+Left");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_NEXTWORD_ID, "Next word\tCtrl+Right");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID, "Previous procedure\tAlt+Up");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_NEXTPROCEDURE_ID, "Next procedure\tAlt+Down");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID, "Previous paragraph\tCtrl+Alt+Left");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_NEXTPARAGRAPH_ID, "Next paragraph\tCtrl+Alt+Right");
    AppendMenu(hEditMenuGoto, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_VARIABLEDECLARATION_ID, "Variable declaration\tCtrl+Alt+S");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_VARIABLENEXTUSE_ID, "Variable next use");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID, "Procedure declaration");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID, "Procedure entry point\tCtrl+Alt+F");
    AppendMenu(hEditMenuGoto, MF_STRING, BaseID + MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID, "Procedure next use");
    WAMenuSetDefaultItem(hEditMenuGoto, 0);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuGoto, "Goto");
    hEditMenuBlock = CreatePopupMenu();
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_INDENT_ID, "Indent\tTab");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_UNINDENT_ID, "Unindent\tShift+Tab");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_UPPERCASE_ID, "Upper case\tCtrl+U");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_LOWERCASE_ID, "Lower case\tCtrl+L");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_TOGGLECASE_ID, "Toggle case\tCtrl+Shift+U");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_COMMENT_ID, "Comment\tCtrl+K");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_UNCOMMENT_ID, "Uncomment\tCtrl+M");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_TABIFY_ID, "Tabify\tCtrl+Shift+T");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_UNTABIFY_ID, "Untabify\tCtrl+Alt+T");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_SPACESTOTABS_ID, "Spaces to tabs\tCtrl+Alt+R");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_REGISTERS_ID, "Registers\tCtrl+R");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_BLKCOMMENT_ID, "Comment (multi lines)\tCtrl+Shift+K");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID, "Custom comment...\tCtrl+Shift+F8");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_TRIMSPACES_ID, "Trim trailing spaces\tCtrl+Alt+G");
    AppendMenu(hEditMenuBlock, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID, "Convert to include...\tCtrl+Shift+L");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_SAVEAS_ID, "Save as...\tCtrl+W");
    AppendMenu(hEditMenuBlock, MF_STRING, BaseID + MENU_EDIT_BLOCK_PRINT_ID, "Print...\tCtrl+Shift+P");
    WAMenuSetDefaultItem(hEditMenuBlock, 0);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBlock, "Block");
    AppendMenu(hParent, MF_POPUP, (UINT) hEditMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "View" menu
void CreateViewMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hViewMenu = CreatePopupMenu();
    AppendMenu(hViewMenu, MF_STRING, BaseID + MENU_VIEW_STATUSBAR_ID, "Status bar");
    hViewMenuDisplay = CreatePopupMenu();
	AppendMenu(hViewMenuDisplay, MF_STRING, BaseID + MENU_VIEW_PROJECTDOCK_ID, "Project\tAlt+1");
	AppendMenu(hViewMenuDisplay, MF_STRING, BaseID + MENU_VIEW_WINDOWSDOCK_ID, "Windows\tAlt+2");
    AppendMenu(hViewMenuDisplay, MF_STRING, BaseID + MENU_VIEW_OUTPUTDOCK_ID, "Output\tAlt+3");
	AppendMenu(hViewMenuDisplay, MF_SEPARATOR, (UINT) -1, "-");
    // Disabled by default at startup (debugger related)
    AppendMenu(hViewMenuDisplay, MF_STRING | MF_GRAYED, BaseID + MENU_VIEW_DEBUGDOCK_ID, "Process datas\tAlt+4");
	AppendMenu(hViewMenuDisplay, MF_STRING | MF_GRAYED, BaseID + MENU_VIEW_REGISTERSDOCK_ID, "Registers\tAlt+5");
    AppendMenu(hViewMenu, MF_POPUP, (UINT) hViewMenuDisplay, "Display");
    AppendMenu(hViewMenu, MF_STRING, BaseID + MENU_VIEW_FULLSCREEN_ID, "Full screen\tAlt+Shift+Z");
    AppendMenu(hViewMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hViewMenu, MF_STRING, BaseID + MENU_VIEW_PROPERTIES_ID, "Properties...\tF8");
    AppendMenu(hViewMenu, MF_STRING, BaseID + MENU_VIEW_USERMENUSEDITOR_ID, "User menus editor...\tF9");
    AppendMenu(hViewMenu, MF_STRING, BaseID + MENU_VIEW_USERTOOLBARSEDITOR_ID, "Toolbars manager...\tShift+F9");
    AppendMenu(hParent, MF_POPUP, (UINT) hViewMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "Project" menu
void CreateProjectMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hProjectMenu = CreatePopupMenu();
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_CREATENEWPROJECT_ID, "Create new project...");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_OPENPROJECT_ID, "Open project...\tCtrl+Alt+O");
    AppendMenu(hProjectMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_SAVEPROJECT_ID, "Save project\tCtrl+Shift+S");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_SAVEPROJECTAS_ID, "Save project as...");
    AppendMenu(hProjectMenu, MF_SEPARATOR, (UINT) -1, "-");
    hProjectAddToMenu = CreatePopupMenu();
    AppendMenu(hProjectAddToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDINCLUDE_ID, "Include file(s)...");
    AppendMenu(hProjectAddToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDLIBRARY_ID, "Library file(s)...");
    AppendMenu(hProjectAddToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDMODULE_ID, "Module file(s)...");
    AppendMenu(hProjectAddToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDOBJECT_ID, "Object file(s)...");
    hProjectAddResToMenu = CreatePopupMenu();
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDRESOURCE_ID, "Rc file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDICON_ID, "Icon file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDCURSOR_ID, "Cursor file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDBITMAP_ID, "Bitmap file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDSTRINGS_ID, "Strings file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDACCELERATORS_ID, "Accelerators file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDMENUS_ID, "Menus file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDDIALOG_ID, "Dialog file(s)...");
    AppendMenu(hProjectAddResToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDRAWDATA_ID, "Raw datas file(s)...");
    AppendMenu(hProjectAddToMenu, MF_POPUP, (UINT) hProjectAddResToMenu, "Resources");
    AppendMenu(hProjectAddToMenu, MF_STRING, BaseID + MENU_PROJECT_ADDTEXT_ID, "Document file(s)...");
    AppendMenu(hProjectMenu, MF_POPUP, (UINT) hProjectAddToMenu, "Add to project");
    AppendMenu(hProjectMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_PROJECTPROPERTIES_ID, "Project properties...");
    AppendMenu(hProjectMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_ASSEMBLEPROJECT_ID, "Build project\tShift+F5");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_RUNPROJECT_ID, "Run project\tCtrl+F5");
    AppendMenu(hProjectMenu, MF_STRING, BaseID + MENU_PROJECT_DEBUGPROJECT_ID, "Debug project\tCtrl+Shift+F5");
    AppendMenu(hParent, MF_POPUP, (UINT) hProjectMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "AddIns" menu
void CreateAddInsMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hEditAddInsMenu = CreatePopupMenu();
    AppendMenu(hEditAddInsMenu, MF_STRING, BaseID + MENU_ADDINS_MANAGER_ID, "AddIns manager...");
    AppendMenu(hParent, MF_POPUP, (UINT) hEditAddInsMenu, MenuName.Get_String());
    FillAddInsArrays();
    FillAddInsMenu();
}

// -----------------------------------------------------------------------
// Create the "Window" menu
void CreateWindowMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hWindowMenu = CreatePopupMenu();
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_CONTEXTHELP_ID, "Context help\tF1");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_MSDNHELP_ID, "MSDN/Platform SDK help\tCtrl+F1");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_DDKHELP_ID, "Windows DDK help\tCtrl+Shift+F1");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_DIRECTXSDKHELP_ID, "DirectX SDK help\tAlt+Shift+F1");
	AppendMenu(hWindowMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_TIPOFTHEDAY_ID, "Tip of the day...");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_ABOUT_ID, "About...");
    AppendMenu(hWindowMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_SPLIT_ID, "Split");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_TILEHORIZONTALLY_ID, "Tile horizontally");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_TILEVERTICALLY_ID, "Tile vertically");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_CASCADE_ID, "Cascade");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_ARRANGEICONS_ID, "Arrange icons");
    AppendMenu(hWindowMenu, MF_SEPARATOR, (UINT) -1, "-");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_NEXTWINDOW_ID, "Next window\tCtrl+F6 / Ctrl+Tab");
    AppendMenu(hWindowMenu, MF_STRING, BaseID + MENU_WINDOW_PREVIOUSWINDOW_ID, "Previous window\tCtrl+Shift+F6 / Ctrl+Shift+Tab");
    AppendMenu(hParent, MF_POPUP, (UINT) hWindowMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the recent files menu
void SetRecentFiles(HMENU hParent)
{
    HMENU hMenu = 0;
    long NbrRc = 0;
    int i = 0;

    EntryInRecent = 0;
    Recents.Erase();
    // Read recent opened files
    hMenu = hParent;
    // Already loaded ?
    if(hMenuRecent != 0)
    {
        // Delete it
        DeleteMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION);
    }
    NbrRc = 0;
    hMenuRecent = CreateMenu();
    for(i = 0; i <= (long) MaxRecent - 1; i++)
    {
        MainMenusRetVal = WAIniReadKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        Recents.Add(MainMenusRetVal.Get_String());
        AppendMenu(hMenuRecent, MF_STRING, MENU_RECENTFILES_IDBASE + i, MainMenusRetVal.Get_String());
        NbrRc++;
    }
    if(NbrRc != 0)
    {
        AppendMenu(hMenuRecent, MF_SEPARATOR, (UINT) -1, "-");
        AppendMenu(hMenuRecent, MF_STRING, MENU_RECENTFILESPURGE_IDBASE, "Remove obsoletes");
        AppendMenu(hMenuRecent, MF_STRING, MENU_RECENTFILESPURGEALL_IDBASE, "Clear all entries");
        InsertMenu(hMenu, 27 - 6 + 2, MF_POPUP + MF_BYPOSITION, (UINT) hMenuRecent, "Recent files");
        EntryInRecent = 1;
    }
    else
    {
        DestroyMenu(hMenuRecent);
        hMenuRecent = 0;
        EntryInRecent = 0;
    }
    SubMenusArray[9] = hMenuRecent;
}

// -----------------------------------------------------------------------
// Create the recent projects menu
void SetRecentProjects(HMENU hParent)
{
    HMENU hMenu = 0;
    long NbrRc = 0;
    int i = 0;

    RecentsPrj.Erase();
    // Read recent opened projects
    hMenu = hParent;
    // No recent projects ?
    if(hMenuRecentPrj != 0)
    {
        if(EntryInRecent == 0) DeleteMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION);
        else DeleteMenu(hMenu, 28 - 6 + 2, MF_BYPOSITION);
    }
    NbrRc = 0;
    hMenuRecentPrj = CreateMenu();
    for(i = 0; i <= (long) MaxRecentPrj - 1; i++)
    {
        MainMenusRetVal = WAIniReadKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        RecentsPrj.Add(MainMenusRetVal.Get_String());
        AppendMenu(hMenuRecentPrj, MF_STRING, MENU_RECENTPROJECTS_IDBASE + i, MainMenusRetVal.Get_String());
        NbrRc++;
    }
    if(NbrRc != 0)
    {
        AppendMenu(hMenuRecentPrj, MF_SEPARATOR, (UINT) -1, "-");
        AppendMenu(hMenuRecentPrj, MF_STRING, MENU_RECENTPROJECTSPURGE_IDBASE, "Remove obsoletes");
        AppendMenu(hMenuRecentPrj, MF_STRING, MENU_RECENTPROJECTSPURGEALL_IDBASE, "Clear all entries");
        // Recent file list there ?
        if(EntryInRecent == 1) InsertMenu(hMenu, 28 - 6 + 2, MF_BYPOSITION + MF_POPUP, (UINT) hMenuRecentPrj, "Recent projects");
        else InsertMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION + MF_POPUP, (UINT) hMenuRecentPrj, "Recent projects");
    }
    else
    {
        DestroyMenu(hMenuRecentPrj);
        hMenuRecentPrj = 0;
    }
    SubMenusArray[10] = hMenuRecentPrj;
}

// -----------------------------------------------------------------------
// Add an entry in recent files list
void AddRecentFile(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;
    CStr TmpFName;

    if(ForceRemove == 1) goto SaveRecent;
    TmpFName = FName.Left(FName.Len());
	if(Recents.Amount() != 0)
	{
		FRcnt = 0;
		for(i = 0; i < Recents.Amount(); i++)
		{
			if(strcmpi(Recents.Get(i)->Content, TmpFName.Get_String()) == 0)
			{
				FRcnt = 1;
				break;
			}
		}
		if(AddFile == 0)
		{
			if(FRcnt != 0) Recents.Set(i, "");
			goto SaveRecent;
		}
		if(FRcnt == 0)
		{
			Recents.Add("");
			// Move'em down
			for(j = (Recents.Amount() - 2); j >= 0; j--)
			{
				Recents.Set(j + 1, Recents.Get(j)->Content);
			}
			// Put new entry on top
			Recents.Set(0, TmpFName.Get_String());
			if((long) MaxRecent < (long) Recents.Amount()) Recents.Del(Recents.Amount() - 1);
		}
		else
		{
			// Found: move it in first position
			// Already in first position
			if(i != 0)
			{
				for(j = i; j >= 1; j--)
				{
					TmpR = Recents.Get(j - 1)->Content;
					Recents.Set(j - 1, Recents.Get(j)->Content);
					Recents.Set(j, TmpR.Get_String());
				}
			}
			// Correction of Upper_Case
			Recents.Set(0, TmpFName.Get_String());
		}
		AddInWinRecent(TmpFName);
		goto SaveRecent;
	}
    // First filename to be added
    Recents.Add(TmpFName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if(WAIniReadKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile).Len() != 0)
        {
            WAIniDeleteKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < Recents.Amount(); i++)
    {
        if(strlen(Recents.Get(i)->Content) != 0)
        {
            WAIniWriteKey("Recents", "Rcnt" + (CStr) StringNumberComplement(j, 3).Get_String(), Recents.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
}

// -----------------------------------------------------------------------
// Add an entry in recent files list
void AddRecentPrj(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;

    if(ForceRemove == 1) goto SaveRecent;
    FRcnt = 0;
	if(RecentsPrj.Amount() != 0)
	{
		for(i = 0; i < RecentsPrj.Amount(); i++)
		{
			if(strcmpi(RecentsPrj.Get(i)->Content, FName.Get_String()) == 0)
			{
				FRcnt = 1;
				break;
			}
		}
		if(AddFile == 0)
		{
			if(FRcnt != 0) RecentsPrj.Set(i, "");
			goto SaveRecent;
		}
		if(FRcnt == 0)
		{
			RecentsPrj.Add("");
			for(j = (RecentsPrj.Amount() - 2); j >= 0; j--)
			{
				RecentsPrj.Set(j + 1, RecentsPrj.Get(j)->Content);
			}
			RecentsPrj.Set(0, FName.Get_String());
			if((long) MaxRecentPrj < (long) RecentsPrj.Amount()) RecentsPrj.Del(RecentsPrj.Amount() - 1);
		}
		else
		{
			// Found: move it in first position
			// Already in first position
			if(i != 0)
			{
				for(j = i; j >= 1; j--)
				{
					TmpR = RecentsPrj.Get(j - 1)->Content;
					RecentsPrj.Set(j - 1, RecentsPrj.Get(j)->Content);
					RecentsPrj.Set(j, TmpR.Get_String());
				}
			}
			// Correction of Upper_Case
			RecentsPrj.Set(0, FName.Get_String());
		}
		AddInWinRecent(FName);
		goto SaveRecent;
	}

    // First filename to be added
    RecentsPrj.Add(FName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if((WAIniReadKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile)).Len() != 0)
        {
            WAIniDeleteKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < RecentsPrj.Amount(); i++)
    {
        if(strlen(RecentsPrj.Get(i)->Content) != 0)
        {
            WAIniWriteKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(j, 3).Get_String(),
                          RecentsPrj.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
}

// -----------------------------------------------------------------------
// Set the user menus
void SetMenus(void)
{
    // Create the menus
    int i = 0;
    CStr LocalMenuName;

    NbrCmds = 0;
    MainMenuH.Erase();
    MenusCmdsLbl.Erase();
    MenusCmds.Erase();
    MenusCmdsComments.Erase();
    MenusCmdsHelp.Erase();
    for(i = 0; i <= 999; i++)
    {
        MainMenusRetVal = WAIniReadKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        LocalMenuName = MainMenusRetVal;
        CreateMenuEntry(LocalMenuName);
    }
    DrawMenuBar(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Create a menu
void CreateMenuEntry(CStr MName)
{
    int i = 0;
    HMENU hMenu = 0;
    HMENU hSubMenu = 0;
    CStr MenuName;
    long *ArEntry = 0;
    CStr SecondElement;
    CStr RecognizedPlug;
    CStr MMName;
    long MPlug = 0;
    CStr PlugToCheck;
	CStr BufString;

    hMenu = hMainMenu;
    hSubMenu = CreateMenu();
    for(i = 0; i <= 999; i++)
    {
        MainMenusRetVal = WAIniReadKey(MName, "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        MainMenusRetVal = MainMenusRetVal.Trim();
        ArEntry = StringSplit(MainMenusRetVal, " | ");
        if(StringGetSplitUBound(ArEntry) == 0)
        {
            MenusCmds.Add("");
            MenusCmdsLbl.Add("");
            MenusCmdsComments.Add("");
            MenusCmdsHelp.Add(0L);
            RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
            goto PlaceOnly;
        }
        SecondElement = StringReplace(StringGetSplitElement(MainMenusRetVal, ArEntry, 1), "$", " ", 1, -1, Binary_Compare);
        if(StringGetSplitElement(MainMenusRetVal, ArEntry, 0).Len() != 0)
        {
            // Command menu
            MPlug = 0;
            BufString = SecondElement.Left(4).Upper_Case();
            if(BufString == "PLUG") MPlug = 1;
            MenusCmdsLbl.Add(StringGetSplitElement(MainMenusRetVal, ArEntry, 0).Trim().Get_String());
            MenusCmds.Add(SecondElement.Get_String());
            if(StringGetSplitUBound(ArEntry) == 2)
            {
                BufString = SecondElement + " (Help file)";
				MenusCmdsComments.Add(BufString.Get_String());
				MenusCmdsHelp.Add(1L);
                RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
            }
            else
            {
				MenusCmdsComments.Add(SecondElement.Get_String());
				MenusCmdsHelp.Add(0L);
                // Try to recognize a plugin
                if(MPlug == 1)
                {
                    PlugToCheck = SecondElement.Mid(6).Trim();
                    PlugToCheck = ChangeRelativePaths(PlugToCheck);
                    PlugToCheck = StringReplace(PlugToCheck, "\"", "", 1, -1, Binary_Compare);
                    if(GetPlugInType(PlugToCheck) == 1) RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
					else RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
                }
                else
                {
                    RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
                }
            }
        }
        else
        {
            // Simple menu (no command)
            RecognizedPlug = "";
            MenusCmds.Add("");
            MenusCmdsLbl.Add("");
            MenusCmdsComments.Add("");
            MenusCmdsHelp.Add(0L);
        }
PlaceOnly:
        StringReleaseSplit(ArEntry);
        if(strcmp(MainMenusRetVal.Get_String(), "-") == 0)
        {
            if(AppendMenu(hSubMenu, MF_SEPARATOR, (UINT) -1, "-") == 0) break;
        }
        else
        {
            if(AppendMenu(hSubMenu, MF_STRING, NbrCmds + 1 + MENU_USER_IDBASE, RecognizedPlug.Get_String()) == 0) break;
        }
        NbrCmds++;
    }
    // Empty menu
    if(i == 0) return;
    MMName = MName.Trim();
    AppendMenu(hMenu, MF_POPUP, (UINT) hSubMenu, MMName.Get_String());
    MainMenuH.Add(hSubMenu);
}

// -----------------------------------------------------------------------
// Clear user and scripts menus
void ClearMenus(void)
{
    HMENU hMenu = 0;
    int i = 0;
    long ChildState = 0;
    
	hMenu = GetMenu(hMDIform.hWnd);
    ChildState = WAControlGetWindowState(CurrentForm);
    for(i = 0; i < MainMenuH.Amount(); i++)
    {
        if(NbForms == 0)
        {
            DeleteMenu(hMenu, 6, MF_BYPOSITION);
        }
        else
        {
            if(ChildState == SW_SHOWMAXIMIZED) DeleteMenu(hMenu, 7, MF_BYPOSITION);
            else DeleteMenu(hMenu, 6, MF_BYPOSITION);
        }
    }
    DrawMenuBar(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Init comments blocks for factory tooltips
void InitMenuToolTips(void)
{
    MenuFileToolTips[MENU_FILE_NEW_ID] = "New (Ctrl+N)";
    MenuFileToolTips[MENU_FILE_CLEAR_ID] = "Clear";
    MenuFileToolTips[MENU_FILE_DUPLICATEFILE_ID] = "Duplicate file";
    MenuFileToolTips[MENU_FILE_OPENFILE_ID] = "Open source file(s) (Ctrl+O)";
    MenuFileToolTips[MENU_FILE_FILTERS_ID] = "Open file(s) and apply filters (Ctrl+Shift+O)";
    MenuFileToolTips[MENU_FILE_OPENWORKSPACE_ID] = "Open Workspace file";
    MenuFileToolTips[MENU_FILE_INCLUDEFILE_ID] = "Include file(s) (Ctrl+I)";
    MenuFileToolTips[MENU_FILE_FTPMANAGER_ID] = "FTP manager";
    MenuFileToolTips[MENU_FILE_CLOSE_ID] = "Close (Ctrl+F4)";
    MenuFileToolTips[MENU_FILE_CLOSEALL_ID] = "Close all";
    MenuFileToolTips[MENU_FILE_SAVE_ID] = "Save current file (Ctrl+S)";
    MenuFileToolTips[MENU_FILE_SAVEAS_ID] = "Save to new file (Shift+S)";
    MenuFileToolTips[MENU_FILE_SAVEALL_ID] = "Save all opened";
    MenuFileToolTips[MENU_FILE_SAVEWORKSPACE_ID] = "Save to workspace";
    MenuFileToolTips[MENU_FILE_REGISTERASSNIPPET_ID] = "Register as code snippet (Ctrl+H)";
    MenuFileToolTips[MENU_FILE_PRINTFILE_ID] = "Print file (Ctrl+P)";
    MenuFileToolTips[MENU_FILE_CONSOLE_RUN_ID] = "Run (F5)";
    MenuFileToolTips[MENU_FILE_CONSOLE_CHANGE_ID] = "Change console handler";
    MenuFileToolTips[MENU_FILE_RUNEXTERNALPROGRAM_ID] = "Run an external program (F6)";
    MenuFileToolTips[MENU_FILE_SETWORKINGDIRECTORY_ID] = "Set working directory (F12)";
    MenuFileToolTips[MENU_FILE_EXIT_ID] = "Exit (Ctrl+Q)";
    MenuFileToolTips[MENU_FILE_RELOADFILE_ID] = "Reload file (Ctrl+Shift+N)";
    MenuFileToolTips[MENU_FILE_INCLUDEFILTERS_ID] = "Include a file and apply filters (Ctrl+Shift+I)";
    MenuFileToolTips[MENU_FILE_REGISTERASTEMPLATE_ID] = "Register as source template (Ctrl+Shift+H)";
    MenuFileToolTips[MENU_FILE_FAVORITES_ID] = "Organize favorites";
    MenuFileToolTips[MENU_FILE_ADD_FAVORITE_ID] = "Add to favorites";

    MenuEditToolTips[MENU_EDIT_UNDO_ID] = "Undo (Ctrl+Z)";
    MenuEditToolTips[MENU_EDIT_REDO_ID] = "Redo (Ctrl+Y)";
    MenuEditToolTips[MENU_EDIT_CUT_ID] = "Cut (Ctrl+X)";
    MenuEditToolTips[MENU_EDIT_COPY_ID] = "Copy selection (Ctrl+C)";
    MenuEditToolTips[MENU_EDIT_COPYFILEPATHNAME_ID] = "Copy file path/name";
    MenuEditToolTips[MENU_EDIT_COPYENTIRETEXT_ID] = "Copy entire text";
    MenuEditToolTips[MENU_EDIT_PASTE_ID] = "Paste to current window (Ctrl+V)";
    MenuEditToolTips[MENU_EDIT_PASTE_NEWWINDOW_ID] = "Paste to new window (Ctrl+Shift+V)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETESELECTION_ID] = "Delete selection (Del)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETELINE_ID] = "Delete line (Ctrl+E)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID] = "Delete to start of line (Alt+Bksp)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETETOENDOFLINE_ID] = "Delete to end of line (Alt+Del)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETEPARAGRAPH_ID] = "Delete paragraph (Ctrl+Alt+K)";
    MenuEditToolTips[MENU_EDIT_SELECTLINE_ID] = "Select line (Ctrl+Alt+F8)";
    MenuEditToolTips[MENU_EDIT_SELECTALL_ID] = "Select all (Ctrl+A)";
    MenuEditToolTips[MENU_EDIT_SELECTPROCEDURE_ID] = "Select procedure (Ctrl+Shift+A)";
    MenuEditToolTips[MENU_EDIT_SELECTPROCANDCOLLAPSE_ID] = "Select and collapse procedure (Ctrl+Shift+Q)";
    MenuEditToolTips[MENU_EDIT_UNCOLLAPSEPROCEDURE_ID] = "Expand procedure (Ctrl+Shift+W)";
    MenuEditToolTips[MENU_EDIT_FIND_ID] = "Find (Ctrl+F)";
    MenuEditToolTips[MENU_EDIT_FINDSELECTION_ID] = "Find fast (Ctrl+Shift+F3)";
    MenuEditToolTips[MENU_EDIT_FINDNEXT_ID] = "Find next (F3)";
    MenuEditToolTips[MENU_EDIT_FINDPREVIOUS_ID] = "Find previous (Ctrl+F3)";
    MenuEditToolTips[MENU_EDIT_REPLACE_ID] = "Replace (F4)";
    MenuEditToolTips[MENU_EDIT_PROCVARIABLESSEARCH_ID] = "Proc/Var/Const search (Ctrl+T)";
    MenuEditToolTips[MENU_EDIT_SEARCHINFILES_ID] = "Search in files (Ctrl+B)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark (F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_FIRST_ID] = "First bookmark (Ctrl+F7)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_PREVIOUS_ID] = "Previous bookmark (Shift+F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_NEXT_ID] = "Next bookmark (Ctrl+F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_LAST_ID] = "Last bookmark (Ctrl+Shift+F7)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks (Ctrl+Shift+F2)";
    MenuEditToolTips[MENU_EDIT_GOTO_TOP_ID] = "Goto top (Ctrl+Home)";
    MenuEditToolTips[MENU_EDIT_GOTO_BOTTOM_ID] = "Goto bottom (Ctrl+End)";
    MenuEditToolTips[MENU_EDIT_GOTO_LINE_ID] = "Goto line (Ctrl+G)";
    MenuEditToolTips[MENU_EDIT_GOTO_MATCHINGBRACKET_ID] = "Goto matching bracket (Ctrl+[)";
    MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSWORD_ID] = "Goto previous word (Ctrl+Left)";
    MenuEditToolTips[MENU_EDIT_GOTO_NEXTWORD_ID] = "Goto next word (Ctrl+Right)";
    MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID] = "Goto previous procedure (Alt+Up)";
    MenuEditToolTips[MENU_EDIT_GOTO_NEXTPROCEDURE_ID] = "Goto next procedure (Alt+Down)";
    MenuEditToolTips[MENU_EDIT_GOTO_VARIABLEDECLARATION_ID] = "Goto variable declaration (Ctrl+Alt+S)";
    MenuEditToolTips[MENU_EDIT_GOTO_VARIABLENEXTUSE_ID] = "Goto variable next use";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID] = "Goto procedure declaration (Ctrl+Alt+F)";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID] = "Goto procedure entry point";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID] = "Goto procedure next use";
	MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID] = "Goto previous paragraph (Ctrl+Alt+Left)";
	MenuEditToolTips[MENU_EDIT_GOTO_NEXTPARAGRAPH_ID] = "Goto next paragraph (Ctrl+Alt+Right)";
	MenuEditToolTips[MENU_EDIT_BLOCK_INDENT_ID] = "Indent block (Tab)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNINDENT_ID] = "Unindent block (Shift+Tab)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UPPERCASE_ID] = "Upper case block (Ctrl+U)";
    MenuEditToolTips[MENU_EDIT_BLOCK_LOWERCASE_ID] = "Lower case block (Ctrl+L)";
    MenuEditToolTips[MENU_EDIT_BLOCK_TOGGLECASE_ID] = "Toggle case block (Ctrl+Shift+U)";
    MenuEditToolTips[MENU_EDIT_BLOCK_COMMENT_ID] = "Comment block (Ctrl+K)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNCOMMENT_ID] = "Uncomment block (Ctrl+M)";
    MenuEditToolTips[MENU_EDIT_BLOCK_TABIFY_ID] = "Tabify block (Ctrl+Shift+T)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNTABIFY_ID] = "Untabify block (Ctrl+Alt+T)";
    MenuEditToolTips[MENU_EDIT_BLOCK_SPACESTOTABS_ID] = "Spaces to tabs (Ctrl+Alt+R)";
    MenuEditToolTips[MENU_EDIT_BLOCK_REGISTERS_ID] = "Registers (Ctrl+R)";
    MenuEditToolTips[MENU_EDIT_BLOCK_BLKCOMMENT_ID] = "Comment (multi lines) (Ctrl+Shift+K)";
    MenuEditToolTips[MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID] = "Custom comment (Ctrl+Shift+F8)";
	MenuEditToolTips[MENU_EDIT_BLOCK_TRIMSPACES_ID] = "Trim trailing spaces (Ctrl+Alt+G)";
    MenuEditToolTips[MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID] = "Convert to include (Ctrl+Shift+L)";
    MenuEditToolTips[MENU_EDIT_BLOCK_SAVEAS_ID] = "Save block (Ctrl+W)";
    MenuEditToolTips[MENU_EDIT_BLOCK_PRINT_ID] = "Print block (Ctrl+Shift+P)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint (F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_FIRST_ID] = "First code breakpoint (Ctrl+F11)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_PREVIOUS_ID] = "Previous code breakpoint (Shift+F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_NEXT_ID] = "Next code breakpoint (Ctrl+F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_LAST_ID] = "Last code breakpoint (Ctrl+Shift+F11)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints (Ctrl+Shift+F10)";

    MenuViewToolTips[MENU_VIEW_STATUSBAR_ID] = "Status bar on/off";
    MenuViewToolTips[MENU_VIEW_PROJECTDOCK_ID] = "Project window on/off (Alt+1)";
    MenuViewToolTips[MENU_VIEW_WINDOWSDOCK_ID] = "Windows list on/off (Alt+2)";
    MenuViewToolTips[MENU_VIEW_OUTPUTDOCK_ID] = "Output window on/off (Alt+3)";
    MenuViewToolTips[MENU_VIEW_DEBUGDOCK_ID] = "Process datas window on/off (Alt+4)";
    MenuViewToolTips[MENU_VIEW_REGISTERSDOCK_ID] = "Registers window on/off (Alt+5)";
    MenuViewToolTips[MENU_VIEW_FULLSCREEN_ID] = "Full screen mode on/off (Alt+Shift+Z)";
    MenuViewToolTips[MENU_VIEW_PROPERTIES_ID] = "Properties (F8)";
    MenuViewToolTips[MENU_VIEW_USERMENUSEDITOR_ID] = "User menus editor (F9)";
    MenuViewToolTips[MENU_VIEW_USERTOOLBARSEDITOR_ID] = "Toolbars manager (Shift+F9)";

    MenuProjectToolTips[MENU_PROJECT_CREATENEWPROJECT_ID] = "Create new project";
    MenuProjectToolTips[MENU_PROJECT_OPENPROJECT_ID] = "Open project (Ctrl+Alt+O)";
    MenuProjectToolTips[MENU_PROJECT_SAVEPROJECT_ID] = "Save project (Ctrl+Shift+S)";
    MenuProjectToolTips[MENU_PROJECT_SAVEPROJECTAS_ID] = "Save project as";
    MenuProjectToolTips[MENU_PROJECT_ADDINCLUDE_ID] = "Add include file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDLIBRARY_ID] = "Add library file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDMODULE_ID] = "Add module file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDOBJECT_ID] = "Add object file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDRESOURCE_ID] = "Add resource file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDTEXT_ID] = "Add RC file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDICON_ID] = "Add icon file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDCURSOR_ID] = "Add cursor file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDBITMAP_ID] = "Add bitmap file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDSTRINGS_ID] = "Add strings file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDACCELERATORS_ID] = "Add accelerators file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDMENUS_ID] = "Add menus file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDDIALOG_ID] = "Add dialog file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDRAWDATA_ID] = "Add raw datas file(s)";
    MenuProjectToolTips[MENU_PROJECT_PROJECTPROPERTIES_ID] = "Project properties";
    MenuProjectToolTips[MENU_PROJECT_ASSEMBLEPROJECT_ID] = "Build project (Shift+F5)";
    MenuProjectToolTips[MENU_PROJECT_RUNPROJECT_ID] = "Run project (Ctrl+F5)";
    MenuProjectToolTips[MENU_PROJECT_DEBUGPROJECT_ID] = "Debug project (Ctrl+Shift+F5)";

    MenuWindowToolTips[MENU_WINDOW_CONTEXTHELP_ID] = "Context help (F1)";
    MenuWindowToolTips[MENU_WINDOW_MSDNHELP_ID] = "MSDN/Platform SDK help (Ctrl+F1)";
    MenuWindowToolTips[MENU_WINDOW_DDKHELP_ID] = "Windows DDK help (Ctrl+Shift+F1)";
    MenuWindowToolTips[MENU_WINDOW_DIRECTXSDKHELP_ID] = "DirectX SDK help (Alt+Shift+F1)";
    MenuWindowToolTips[MENU_WINDOW_ABOUT_ID] = "About";
    MenuWindowToolTips[MENU_WINDOW_SPLIT_ID] = "Split";
    MenuWindowToolTips[MENU_WINDOW_TILEHORIZONTALLY_ID] = "Tile horizontally";
    MenuWindowToolTips[MENU_WINDOW_TILEVERTICALLY_ID] = "Tile vertically";
    MenuWindowToolTips[MENU_WINDOW_CASCADE_ID] = "Cascade";
    MenuWindowToolTips[MENU_WINDOW_ARRANGEICONS_ID] = "Arrange icons";
    MenuWindowToolTips[MENU_WINDOW_NEXTWINDOW_ID] = "Next window (Ctrl+F6 / Ctrl+Tab)";
    MenuWindowToolTips[MENU_WINDOW_PREVIOUSWINDOW_ID] = "Previous window (Ctrl+Shift+F6 / Ctrl+Shift+Tab)";
    MenuWindowToolTips[MENU_WINDOW_TIPOFTHEDAY_ID] = "Tip of the day";
}

// -----------------------------------------------------------------------
// Init comments blocks for factory menus
void InitMenuComments(void)
{
	CStr BufString;

    MenuFileComments[MENU_FILE_NEW_ID] = "Create a new empty file";
    MenuFileComments[MENU_FILE_CLEAR_ID] = "Clear the current file";
    MenuFileComments[MENU_FILE_DUPLICATEFILE_ID] = "Create a new instance of the current file";
    MenuFileComments[MENU_FILE_OPENFILE_ID] = "Open file(s) from local resource";
    MenuFileComments[MENU_FILE_FILTERS_ID] = "Open file(s) and apply filters";
    MenuFileComments[MENU_FILE_OPENWORKSPACE_ID] = "Open a workspace file";
    MenuFileComments[MENU_FILE_INCLUDEFILE_ID] = "Insert file(s) at current position";
    MenuFileComments[MENU_FILE_FTPMANAGER_ID] = "Manage files on FTP resources";
    MenuFileComments[MENU_FILE_CLOSE_ID] = "Close the current file";
    MenuFileComments[MENU_FILE_CLOSEALL_ID] = "Close all files";
    MenuFileComments[MENU_FILE_SAVE_ID] = "Save current file";
    MenuFileComments[MENU_FILE_SAVEAS_ID] = "Save current file under a new name";
    MenuFileComments[MENU_FILE_SAVEALL_ID] = "Save all opened files";
    MenuFileComments[MENU_FILE_SAVEWORKSPACE_ID] = "Save all files as a workspace";
    MenuFileComments[MENU_FILE_REGISTERASSNIPPET_ID] = "Register current file as code snippet";
    MenuFileComments[MENU_FILE_PRINTFILE_ID] = "Print the current file";
    MenuFileComments[MENU_FILE_CONSOLE_RUN_ID] = "Open a console handler";
    MenuFileComments[MENU_FILE_CONSOLE_CHANGE_ID] = "Change the console handler";
    MenuFileComments[MENU_FILE_RUNEXTERNALPROGRAM_ID] = "Run a selected program";
    MenuFileComments[MENU_FILE_SETWORKINGDIRECTORY_ID] = "Set current directory";
    BufString = "Quit " + (CStr) AppTitle;
	MenuFileComments[MENU_FILE_EXIT_ID] = strdup(BufString.Get_String());
    MenuFileComments[MENU_FILE_RELOADFILE_ID] = "Reload current file";
    MenuFileComments[MENU_FILE_INCLUDEFILTERS_ID] = "Insert filtered file(s) at current position";
    MenuFileComments[MENU_FILE_REGISTERASTEMPLATE_ID] = "Register current file as source template";
    MenuFileComments[MENU_FILE_FAVORITES_ID] = "Organize favorites documents";
    MenuFileComments[MENU_FILE_ADD_FAVORITE_ID] = "Add current document to favorites";

    MenuEditComments[MENU_EDIT_UNDO_ID] = "Undo last command";
    MenuEditComments[MENU_EDIT_REDO_ID] = "Redo last command";
    MenuEditComments[MENU_EDIT_CUT_ID] = "Cut and copy selected text into clipboard";
    MenuEditComments[MENU_EDIT_COPY_ID] = "Copy selected text into clipboard";
    MenuEditComments[MENU_EDIT_COPYFILEPATHNAME_ID] = "Copy file path and name into clipboard";
    MenuEditComments[MENU_EDIT_COPYENTIRETEXT_ID] = "Copy entire text into clipboard";
    MenuEditComments[MENU_EDIT_PASTE_ID] = "Paste text from clipboard into current window";
    MenuEditComments[MENU_EDIT_DELETE_DELETESELECTION_ID] = "Delete selected text";
    MenuEditComments[MENU_EDIT_DELETE_DELETELINE_ID] = "Delete current line";
    MenuEditComments[MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID] = "Delete text to start of line from current position";
    MenuEditComments[MENU_EDIT_DELETE_DELETETOENDOFLINE_ID] = "Delete text to end of line from current position";
    MenuEditComments[MENU_EDIT_SELECTLINE_ID] = "Select current line";
    MenuEditComments[MENU_EDIT_SELECTALL_ID] = "Select entire text";
    MenuEditComments[MENU_EDIT_SELECTPROCEDURE_ID] = "Select a procedure from caret position";
    MenuEditComments[MENU_EDIT_SELECTPROCANDCOLLAPSE_ID] = "Select a procedure and collapse it into a file";
    MenuEditComments[MENU_EDIT_UNCOLLAPSEPROCEDURE_ID] = "Expand a procedure from a file";
    MenuEditComments[MENU_EDIT_FIND_ID] = "Find in text";
    MenuEditComments[MENU_EDIT_FINDSELECTION_ID] = "Search with current selection or word under caret";
    MenuEditComments[MENU_EDIT_FINDNEXT_ID] = "Find next occurence";
    MenuEditComments[MENU_EDIT_FINDPREVIOUS_ID] = "Find previous occurence";
    MenuEditComments[MENU_EDIT_REPLACE_ID] = "Replace in text";
    MenuEditComments[MENU_EDIT_PROCVARIABLESSEARCH_ID] = "Search for procedures/variables/constants in windows or project";
    MenuEditComments[MENU_EDIT_SEARCHINFILES_ID] = "Search for procedures/variables in external files";
    MenuEditComments[MENU_EDIT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark at current line";
    MenuEditComments[MENU_EDIT_BOOKMARKS_FIRST_ID] = "Jump to first bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_PREVIOUS_ID] = "Jump to previous bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_NEXT_ID] = "Jump to next bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_LAST_ID] = "Jump to last bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks";
    MenuEditComments[MENU_EDIT_GOTO_TOP_ID] = "Goto top of file";
    MenuEditComments[MENU_EDIT_GOTO_BOTTOM_ID] = "Goto bottom of file";
    MenuEditComments[MENU_EDIT_GOTO_LINE_ID] = "Goto specified line";
    MenuEditComments[MENU_EDIT_GOTO_MATCHINGBRACKET_ID] = "Goto bracket matching current one";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSWORD_ID] = "Position caret on previous word";
    MenuEditComments[MENU_EDIT_GOTO_NEXTWORD_ID] = "Position caret on next word";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID] = "Position caret on previous procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_NEXTPROCEDURE_ID] = "Position caret on next procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_VARIABLEDECLARATION_ID] = "Search for variable declaration";
    MenuEditComments[MENU_EDIT_GOTO_VARIABLENEXTUSE_ID] = "Search for next variable use";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID] = "Search for procedure declaration (prototype)";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID] = "Search for procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID] = "Search for next procedure use";
    MenuEditComments[MENU_EDIT_BLOCK_INDENT_ID] = "Indent selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNINDENT_ID] = "Unindent selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UPPERCASE_ID] = "Convert selected text to upper case";
    MenuEditComments[MENU_EDIT_BLOCK_LOWERCASE_ID] = "Convert selected text to lower case";
    MenuEditComments[MENU_EDIT_BLOCK_TOGGLECASE_ID] = "Toggle chars case in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_COMMENT_ID] = "Comment selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNCOMMENT_ID] = "Uncomment selected text";
    MenuEditComments[MENU_EDIT_BLOCK_TABIFY_ID] = "Tabify selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNTABIFY_ID] = "Untabify selected text";
    MenuEditComments[MENU_EDIT_BLOCK_SPACESTOTABS_ID] = "Convert spaces into tabulations in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_REGISTERS_ID] = "Search for used registers in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_BLKCOMMENT_ID] = "Put selected text into multiline comment";
    MenuEditComments[MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID] = "Comment block with user specified text";
    MenuEditComments[MENU_EDIT_BLOCK_TRIMSPACES_ID] = "Remove trailing spaces from selected text";
    MenuEditComments[MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID] = "Save current selection and convert it to include statement";
    MenuEditComments[MENU_EDIT_BLOCK_SAVEAS_ID] = "Save current selection in a file";
    MenuEditComments[MENU_EDIT_BLOCK_PRINT_ID] = "Print current selection";
    MenuEditComments[MENU_EDIT_PASTE_NEWWINDOW_ID] = "Paste text from clipboard into a new window";
    MenuEditComments[MENU_EDIT_DELETE_DELETEPARAGRAPH_ID] = "Delete current paragraph";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID] = "Position caret on previous paragraph";
    MenuEditComments[MENU_EDIT_GOTO_NEXTPARAGRAPH_ID] = "Position caret on next paragraph";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint at current line";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_FIRST_ID] = "Jump to first code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_PREVIOUS_ID] = "Jump to previous code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_NEXT_ID] = "Jump to next code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_LAST_ID] = "Jump to last code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints";

    MenuViewComments[MENU_VIEW_STATUSBAR_ID] = "Turn statusbar on/off";
    MenuViewComments[MENU_VIEW_PROJECTDOCK_ID] = "Turn project window on/off";
    MenuViewComments[MENU_VIEW_WINDOWSDOCK_ID] = "Turn windows list on/off";
    MenuViewComments[MENU_VIEW_OUTPUTDOCK_ID] = "Turn output window on/off";
    MenuViewComments[MENU_VIEW_DEBUGDOCK_ID] = "Turn debugged process datas window on/off";
    MenuViewComments[MENU_VIEW_REGISTERSDOCK_ID] = "Turn registers window on/off";
    MenuViewComments[MENU_VIEW_FULLSCREEN_ID] = "Turn full screen mode on/off";
    BufString = "Modify " + (CStr) AppTitle + (CStr) "'s properties";
	MenuViewComments[MENU_VIEW_PROPERTIES_ID] = strdup(BufString.Get_String());
    MenuViewComments[MENU_VIEW_USERMENUSEDITOR_ID] = "Modify user's menus";
    MenuViewComments[MENU_VIEW_USERTOOLBARSEDITOR_ID] = "Modify and create toolbars";
    
    MenuProjectComments[MENU_PROJECT_CREATENEWPROJECT_ID] = "Create a new project";
    MenuProjectComments[MENU_PROJECT_OPENPROJECT_ID] = "Open an existing project";
    MenuProjectComments[MENU_PROJECT_SAVEPROJECT_ID] = "Save current project";
    MenuProjectComments[MENU_PROJECT_SAVEPROJECTAS_ID] = "Save project under a new name";
    MenuProjectComments[MENU_PROJECT_ADDINCLUDE_ID] = "Add new include(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDLIBRARY_ID] = "Add new library(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDMODULE_ID] = "Add new module(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDOBJECT_ID] = "Add new object(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDRESOURCE_ID] = "Add new resource file(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDTEXT_ID] = "Add new document(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDICON_ID] = "Add new icon file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDCURSOR_ID] = "Add new cursor file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDBITMAP_ID] = "Add new bitmap file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDSTRINGS_ID] = "Add new strings file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDACCELERATORS_ID] = "Add new accelerators file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDMENUS_ID] = "Add new menus file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDDIALOG_ID] = "Add new dialog file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDRAWDATA_ID] = "Add new raw datas file(s) to project";
    MenuProjectComments[MENU_PROJECT_PROJECTPROPERTIES_ID] = "Modify project properties";
    MenuProjectComments[MENU_PROJECT_ASSEMBLEPROJECT_ID] = "Build complete project";
    MenuProjectComments[MENU_PROJECT_RUNPROJECT_ID] = "Build and run complete project";
    MenuProjectComments[MENU_PROJECT_DEBUGPROJECT_ID] = "Build and debug complete project";

    MenuAddInComments[MENU_ADDINS_MANAGER_ID] = "Manage Chromatic AddIns";
    
    MenuWindowComments[MENU_WINDOW_CONTEXTHELP_ID] = "Search for a keyword in help files";
    MenuWindowComments[MENU_WINDOW_MSDNHELP_ID] = "Search for a keyword in MSDN or Platform SDK";
    MenuWindowComments[MENU_WINDOW_DDKHELP_ID] = "Search for a keyword in Windows DDK";
    MenuWindowComments[MENU_WINDOW_DIRECTXSDKHELP_ID] = "Search for a keyword in DirectX SDK";
    BufString = "About " + (CStr) AppTitle;
    MenuWindowComments[MENU_WINDOW_ABOUT_ID] = strdup(BufString.Get_String());
    MenuWindowComments[MENU_WINDOW_SPLIT_ID] = "Split current window";
    MenuWindowComments[MENU_WINDOW_TILEHORIZONTALLY_ID] = "Tile windows horizontally";
    MenuWindowComments[MENU_WINDOW_TILEVERTICALLY_ID] = "Tile windows vertically";
    MenuWindowComments[MENU_WINDOW_CASCADE_ID] = "Cascade windows";
    MenuWindowComments[MENU_WINDOW_ARRANGEICONS_ID] = "Arrange minimized windows";
    MenuWindowComments[MENU_WINDOW_NEXTWINDOW_ID] = "Activate next window";
    MenuWindowComments[MENU_WINDOW_PREVIOUSWINDOW_ID] = "Activate previous window";
    MenuWindowComments[MENU_WINDOW_TIPOFTHEDAY_ID] = "Display the 'Tip of the day' window";

    MenuCodeMaxComments[MENU_CONTEXT_CONTEXTHELP_ID] = "Search for a keyword in help files";
    MenuCodeMaxComments[MENU_CONTEXT_MSDNHELP_ID] = "Search for a keyword in MSDN or Platform SDK";
    MenuCodeMaxComments[MENU_CONTEXT_DDKHELP_ID] = "Search for a keyword in Windows DDK";
    MenuCodeMaxComments[MENU_CONTEXT_DIRECTXSDKHELP_ID] = "Search for a keyword in DirectX SDK";
    MenuCodeMaxComments[MENU_CONTEXT_OPENFILEUNDERCURSOR_ID] = "Open file referenced under caret";
    MenuCodeMaxComments[MENU_CONTEXT_EXECFILEUNDERCURSOR_ID] = "Execute file referenced under caret";
    MenuCodeMaxComments[MENU_CONTEXT_DECTOHEX_ID] = "Convert decimal number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_DECTOBIN_ID] = "Convert decimal number into binary";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTODEC_ID] = "Convert hexadecimal number into decimal";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTOBIN_ID] = "Convert hexadecimal number into binary";
    MenuCodeMaxComments[MENU_CONTEXT_BINTODEC_ID] = "Convert binary number into decimal";
    MenuCodeMaxComments[MENU_CONTEXT_BINTOHEX_ID] = "Convert binary number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTOFLT_ID] = "Convert hexadecimal number into floating point";
    MenuCodeMaxComments[MENU_CONTEXT_FLTTOHEX_ID] = "Convert floating point number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_UNDO_ID] = "Undo last command";
    MenuCodeMaxComments[MENU_CONTEXT_REDO_ID] = "Redo last command";
    MenuCodeMaxComments[MENU_CONTEXT_CUT_ID] = "Cut and copy selected text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPY_ID] = "Copy selected text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPYFILEPATHNAME_ID] = "Copy file path and name into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPYENTIRETEXT_ID] = "Copy entire text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_PASTE_ID] = "Paste text from clipboard into current window";
    MenuCodeMaxComments[MENU_CONTEXT_PASTENEWWINDOW_ID] = "Paste text from clipboard into a new window";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTLINE_ID] = "Select current line";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTALL_ID] = "Select entire text";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTPROCEDURE_ID] = "Select a procedure from caret position";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID] = "Select a procedure and collapse it into a file";
    MenuCodeMaxComments[MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID] = "Expand a procedure from a file";
    MenuCodeMaxComments[MENU_CONTEXT_FIND_ID] = "Find in text";
    MenuCodeMaxComments[MENU_CONTEXT_REPLACE_ID] = "Replace in text";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark at current line";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_FIRST_ID] = "Jump to first bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID] = "Jump to previous bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_NEXT_ID] = "Jump to next bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_LAST_ID] = "Jump to last bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint at current line";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_FIRST_ID] = "Jump to first code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID] = "Jump to previous code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_NEXT_ID] = "Jump to next code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_LAST_ID] = "Jump to last code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_TOP_ID] = "Goto top of file";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_BOTTOM_ID] = "Goto bottom of file";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_LINE_ID] = "Goto specified line";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID] = "Goto bracket matching current one";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSWORD_ID] = "Position caret on previous word";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTWORD_ID] = "Position caret on next word";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID] = "Position caret on previous paragraph";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID] = "Position caret on next paragraph";
	MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID] = "Position caret on previous procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID] = "Position caret on next procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID] = "Search for variable declaration";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID] = "Search for next variable use";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID] = "Search for procedure declaration (prototype)";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID] = "Search for procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID] = "Search for next procedure use";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_INDENT_ID] = "Indent selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNINDENT_ID] = "Unindent selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UPPERCASE_ID] = "Convert selected text to upper case";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_LOWERCASE_ID] = "Convert selected text to lower case";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TOGGLECASE_ID] = "Toggle chars case in selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_COMMENT_ID] = "Comment selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNCOMMENT_ID] = "Uncomment selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TABIFY_ID] = "Tabify selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNTABIFY_ID] = "Untabify selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_SPACESTOTABS_ID] = "Convert spaces into tabulations in selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_REGISTERS_ID] = "Search for used registers";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_BLKCOMMENT_ID] = "Put selected text into multiline comment";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID] = "Comment block with user specified text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TRIMSPACES_ID] = "Remove trailing spaces from selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID] = "Save current selection and convert it to include statement";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_SAVEAS_ID] = "Save current selection in a file";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_PRINT_ID] = "Print current selection";
    MenuCodeMaxComments[MENU_CONTEXT_NEW_ID] = "Create a new empty file";

    MenuCodeMaxComments[MENU_CONTEXT_INCLUDEFILE_ID] = "Insert file(s) at current position";
    MenuCodeMaxComments[MENU_CONTEXT_INCLUDEFILTERS_ID] = "Insert filtered file(s) at current position";

    MenuCodeMaxComments[MENU_CONTEXT_DUPLICATEFILE_ID] = "Create a new instance of the current file";
    MenuCodeMaxComments[MENU_CONTEXT_RELOADFILE_ID] = "Reload current file";
    BufString = "Modify " + (CStr) AppTitle + (CStr) "'s properties";
    MenuCodeMaxComments[MENU_CONTEXT_PROPERTIES_ID] = strdup(BufString.Get_String());
    RegCodeMaxLocalCommands();
}

// -----------------------------------------------------------------------
// Register commands in codemax
void RegCodeMaxLocalCommands(void)
{
    // Ctrl+Enter
    RegCMCom("Open file under caret", MenuCodeMaxComments[MENU_CONTEXT_OPENFILEUNDERCURSOR_ID], 1000, 2, 13);
    // Ctrl+Shift+Enter
    RegCMCom("Execute file under caret", MenuCodeMaxComments[MENU_CONTEXT_EXECFILEUNDERCURSOR_ID], 1001, 2 + 1, 13);
    // Ctrl+Space
    RegCMCom("Insert snippet", "Insert a code snippet at current position", 1002, 2 + 4, 32);
    RegCMCom("Dec to hex", MenuCodeMaxComments[MENU_CONTEXT_DECTOHEX_ID], 1003, 2, 49);
    RegCMCom("Dec to bin", MenuCodeMaxComments[MENU_CONTEXT_DECTOBIN_ID], 1004, 2, 50);
    RegCMCom("Hex to dec", MenuCodeMaxComments[MENU_CONTEXT_HEXTODEC_ID], 1005, 2, 51);
    RegCMCom("Hex to bin", MenuCodeMaxComments[MENU_CONTEXT_HEXTOBIN_ID], 1006, 2, 52);
    RegCMCom("Bin to dec", MenuCodeMaxComments[MENU_CONTEXT_BINTODEC_ID], 1007, 2, 53);
    RegCMCom("Bin to hex", MenuCodeMaxComments[MENU_CONTEXT_BINTOHEX_ID], 1008, 2, 54);
    RegCMCom("Hex to flt", MenuCodeMaxComments[MENU_CONTEXT_HEXTOFLT_ID], 1009, 2, 55);
    RegCMCom("Flt to Hex", MenuCodeMaxComments[MENU_CONTEXT_FLTTOHEX_ID], 1010, 2, 56);
    RegCMCom("Scripts", "Execute a script at current position", 1011, 2 + 1, 32);
    RegCMCom("Display APIs", "Display APIs database names", 1012, 2, 32);
}

// -----------------------------------------------------------------------
// Register a command into CodeMax
void RegCMCom(CStr CmdName, CStr CmdComment, long CmdNumber, long CmdModif, long CmdKey)
{
    hCodeMaxCMRegisterCommand(CmdNumber, CmdName.Get_String(), CmdComment.Get_String());
    CHotKey.byModifiers1 = (BYTE) CmdModif;
    CHotKey.byModifiers2 = 0;
    CHotKey.nVirtKey1 = CmdKey;
    CHotKey.nVirtKey2 = 0;
    hCodeMaxCMRegisterHotKey((long) &CHotKey, CmdNumber);
}

// -----------------------------------------------------------------------
// Retrieve a plugin type
// 0=not recognized
// 1=QEditor
long GetPlugInType(CStr PlugInName)
{
    long ReturnValue = 0;
	HMODULE hPlug = 0;
    
	hPlug = LoadLibrary(PlugInName.Get_String());
    if(hPlug != 0)
    {
        if(GetProcAddress(hPlug, "QePlugIn") != 0) ReturnValue = 1;
        FreeLibrary(hPlug);
    }
	return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a filename into windows recent files list
void AddInWinRecent(CStr FName)
{
    CStr RcntVal;

    RcntVal = WAIniReadKey("Layout", "WinRecent", MainIniFile);
    if(RcntVal.Len() == 0) return;
    if(RcntVal == "1") WAMiscShellAddFileToRecents(FName);
}

// -----------------------------------------------------------------------
// Create the history of the run programs
void CreateRunProgMenu(void)
{
    int i = 0;
    int k = 0;
    CStr ProgRetVal;
    long FoundProg = 0;
    
	RunProgArray.Erase();
    k = 0;
    while(DeleteMenu(hFileMenuRunProg, 0, MF_BYPOSITION) != 0)
    {
    
    }
    AppendMenu(hFileMenuRunProg, MF_STRING, MENU_FILE_IDBASE + MENU_FILE_RUNEXTERNALPROGRAM_ID, "Run...\tF6");
    WAMenuSetDefaultItem(hFileMenuRunProg, 0);
    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = WAIniReadKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ProgRetVal.Len() == 0) break;
        ProgRetVal = ChangeRelativePaths(ProgRetVal);
        // Check the file
        if(WAFileExist(ProgRetVal) != 0)
        {
            // Add a separator at first place
            if(FoundProg == 0) AppendMenu(hFileMenuRunProg, MF_SEPARATOR, -1, "-");
            FoundProg = 1;
            // Get the language
            AppendMenu(hFileMenuRunProg, MF_STRING, k + MENU_RECENTAPPLICATIONS_IDBASE, ProgRetVal.Get_String());
            RunProgArray.Add(ProgRetVal.Get_String());
            k++;
        }
    }
    if(k != 0)
    {
        AppendMenu(hFileMenuRunProg, MF_SEPARATOR, (UINT) -1, "-");
        AppendMenu(hFileMenuRunProg, MF_STRING, MENU_RUNPROGCLEAR_IDBASE, "Clear all entries");
    }
    SubMenusArray[15] = hFileMenuRunProg;
}

// -----------------------------------------------------------------------
// Add an entry in run programs history
void AddRecentRunProg(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;
    CStr TmpFName;

    if(ForceRemove == 1) goto SaveRecent;
    TmpFName = FName.Left(FName.Len());
	if(RunProgArray.Amount() != 0)
	{
		FRcnt = 0;
		for(i = 0; i < RunProgArray.Amount(); i++)
		{
			if(strcmpi(RunProgArray.Get(i)->Content, TmpFName.Get_String()) == 0)
			{
				FRcnt = 1;
				break;
			}
		}
		if(AddFile == 0)
		{
			if(FRcnt != 0) RunProgArray.Set(i, "");
			goto SaveRecent;
		}
		if(FRcnt == 0)
		{
			RunProgArray.Add("");
			// Move'em down
			for(j = (RunProgArray.Amount() - 2); j >= 0; j--)
			{
				RunProgArray.Set(j + 1, RunProgArray.Get(j)->Content);
			}
			RunProgArray.Set(0, TmpFName.Get_String());
			if((long) MaxRecent < (long) RunProgArray.Amount())
			{
			    RunProgArray.Del(RunProgArray.Amount() - 1);
		    }
		}
		else
		{
			// Found: move it in first position
			// Already in first position
			if(i != 0)
			{
				for(j = i; j >= 1; j--)
				{
					TmpR = RunProgArray.Get(j - 1)->Content;
					RunProgArray.Set(j - 1, RunProgArray.Get(j)->Content);
					RunProgArray.Set(j, TmpR.Get_String());
				}
			}
			// Correction of Upper_Case
			RunProgArray.Set(0, TmpFName.Get_String());
		}
		AddInWinRecent(TmpFName);
		goto SaveRecent;
	}
    // First filename to be added
    RunProgArray.Add(TmpFName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if(WAIniReadKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile).Len() != 0)
        {
            WAIniDeleteKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < RunProgArray.Amount(); i++)
    {
        if(RunProgArray.Get(i)->Content != 0)
        {
            WAIniWriteKey("RunProg", "Name" + (CStr) StringNumberComplement(j, 3).Get_String(),
                          RunProgArray.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    CreateRunProgMenu();
}

// -----------------------------------------------------------------------
// Create the list of favorites files
void CreateFavoritesMenu(void)
{
    int i = 0;
    int k = 0;
    CStr ProgRetVal;
    long FoundProg = 0;
    
	FavoritesArray.Erase();
    k = 0;
    while(DeleteMenu(hFileMenuFavorites, 0, MF_BYPOSITION) != 0)
    {
    }
    AppendMenu(hFileMenuFavorites, MF_STRING, MENU_FILE_IDBASE + MENU_FILE_ADD_FAVORITE_ID, "Add to favorites");
    AppendMenu(hFileMenuFavorites, MF_STRING, MENU_FILE_IDBASE + MENU_FILE_FAVORITES_ID, "Organize favorites...");
    WAMenuSetDefaultItem(hFileMenuFavorites, 1);
    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = WAIniReadKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ProgRetVal.Len() == 0) break;
        ProgRetVal = ChangeRelativePaths(ProgRetVal);
        // Check the file
        if(WAFileExist(ProgRetVal) != 0)
        {
            // Add a separator at first place
            if(FoundProg == 0) AppendMenu(hFileMenuFavorites, MF_SEPARATOR, -1, "-");
            FoundProg = 1;
            // Get the language
            AppendMenu(hFileMenuFavorites, MF_STRING, k + MENU_FAVORITESFILES_IDBASE, ProgRetVal.Get_String());
            FavoritesArray.Add(ProgRetVal.Get_String());
            k++;
        }
    }
    SubMenusArray[16] = hFileMenuFavorites;
}

// -----------------------------------------------------------------------
// Create the list of filters profiles
void CreateFiltersMenu(HMENU Dest_Menu, int Index, int Lab_Index, CStr Label, int Include_Menu)
{
    int i = 0;
    int k = 0;
    CStr ProgRetVal;
    long FoundProg = 0;

    FiltersArray.Erase();
	k = 0;
    while(DeleteMenu(Dest_Menu, 0, MF_BYPOSITION) != 0)
    {
    }
    AppendMenu(Dest_Menu, MF_STRING, Lab_Index, Label.Get_String());
    WAMenuSetDefaultItem(Dest_Menu, 0);

    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = WAIniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(ProgRetVal.Len() == 0) break;
        // Add a separator at first place
        if(FoundProg == 0) AppendMenu(Dest_Menu, MF_SEPARATOR, -1, "-");
        FoundProg = 1;
		if(Include_Menu) AppendMenu(Dest_Menu, MF_STRING, i + MENU_FILTERS_INCLUDE_IDBASE, ProgRetVal.Get_String());
        else AppendMenu(Dest_Menu, MF_STRING, i + MENU_FILTERS_IDBASE, ProgRetVal.Get_String());
		FiltersArray.Add(ProgRetVal.Get_String());
    }
	if(Index != 0) SubMenusArray[Index] = Dest_Menu;
}
