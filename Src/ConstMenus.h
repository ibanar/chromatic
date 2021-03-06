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
// ConstMenus.h: Constants for builtin menus
// -----------------------------------------------------------------------

#ifndef _CONSTMENUS_H_
#define _CONSTMENUS_H_

#define MENU_RECENTFILES_IDBASE 2000
#define MENU_RECENTPROJECTS_IDBASE 2200
#define MENU_RECENTAPPLICATIONS_IDBASE 2400
#define MENU_FAVORITESFILES_IDBASE 2600
#define MENU_FILTERS_IDBASE 2800
#define MENU_FILTERS_INCLUDE_IDBASE 3000

// These values MUST follow
#define MENU_RUNPROGCLEAR_IDBASE 3995
#define MENU_RECENTFILESPURGEALL_IDBASE 3996
#define MENU_RECENTPROJECTSPURGEALL_IDBASE 3997
#define MENU_RECENTFILESPURGE_IDBASE 3998
#define MENU_RECENTPROJECTSPURGE_IDBASE 3999

#define MENU_CONTEXT_IDBASE 5000

#define MENU_FILE_IDBASE 4000
#define MENU_EDIT_IDBASE 4500
#define MENU_VIEW_IDBASE 5000
#define MENU_PROJECT_IDBASE 5500
#define MENU_WINDOW_IDBASE 6000
#define MENU_USER_IDBASE 7000
#define MENU_MAXIMA_IDBASE 8000

#define MENU_SEPS_IDBASE 9999
#define MENU_SNIPPET_IDBASE 10000
#define MENU_TEMPLATES_IDBASE 10000
#define MENU_CODEMAXIMA_IDBASE 11000

#define MENU_OUTBOX_IDBASE 12000
#define MENU_DEBUGBOX_IDBASE 12500
#define MENU_WINDOWSBOX_IDBASE 12600

#define MENU_SCRIPTINSERT_IDBASE 13000
#define SYSTRAY_MSG 14000
#define MENU_SYSTRAY_RESTORE 14001
#define MENU_SYSTRAY_EXIT 14002

#define MENU_ADDINS_IDBASE 16000

#define MENU_FILE_NEW_ID 0
#define MENU_FILE_CLEAR_ID 1
#define MENU_FILE_DUPLICATEFILE_ID 2
#define MENU_FILE_OPENFILE_ID 3
#define MENU_FILE_FILTERS_ID 4
#define MENU_FILE_OPENWORKSPACE_ID 5
#define MENU_FILE_INCLUDEFILE_ID 6
#define MENU_FILE_FTPMANAGER_ID 7
#define MENU_FILE_CLOSE_ID 8
#define MENU_FILE_CLOSEALL_ID 9
#define MENU_FILE_SAVE_ID 10
#define MENU_FILE_SAVEAS_ID 11
#define MENU_FILE_SAVEALL_ID 12
#define MENU_FILE_SAVEWORKSPACE_ID 13
#define MENU_FILE_REGISTERASSNIPPET_ID 14
#define MENU_FILE_PRINTFILE_ID 15
#define MENU_FILE_CONSOLE_RUN_ID 16
#define MENU_FILE_CONSOLE_CHANGE_ID 17
#define MENU_FILE_RUNEXTERNALPROGRAM_ID 18
#define MENU_FILE_SETWORKINGDIRECTORY_ID 19
#define MENU_FILE_EXIT_ID 20
#define MENU_FILE_RELOADFILE_ID 21
#define MENU_FILE_INCLUDEFILTERS_ID 22
#define MENU_FILE_REGISTERASTEMPLATE_ID 23
#define MENU_FILE_FAVORITES_ID 24
#define MENU_FILE_ADD_FAVORITE_ID 25
#define MENU_FILE_LAST_ID MENU_FILE_ADD_FAVORITE_ID

#define MENU_EDIT_UNDO_ID 0
#define MENU_EDIT_REDO_ID 1
#define MENU_EDIT_CUT_ID 2
#define MENU_EDIT_COPY_ID 3
#define MENU_EDIT_COPYFILEPATHNAME_ID 4
#define MENU_EDIT_COPYENTIRETEXT_ID 5
#define MENU_EDIT_PASTE_ID 6
#define MENU_EDIT_DELETE_DELETESELECTION_ID 7
#define MENU_EDIT_DELETE_DELETELINE_ID 8
#define MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID 9
#define MENU_EDIT_DELETE_DELETETOENDOFLINE_ID 10
#define MENU_EDIT_SELECTLINE_ID 11
#define MENU_EDIT_SELECTALL_ID 12
#define MENU_EDIT_SELECTPROCEDURE_ID 13
#define MENU_EDIT_SELECTPROCANDCOLLAPSE_ID 14
#define MENU_EDIT_UNCOLLAPSEPROCEDURE_ID 15
#define MENU_EDIT_FIND_ID 16
#define MENU_EDIT_FINDSELECTION_ID 17
#define MENU_EDIT_FINDNEXT_ID 18
#define MENU_EDIT_FINDPREVIOUS_ID 19
#define MENU_EDIT_REPLACE_ID 20
#define MENU_EDIT_PROCVARIABLESSEARCH_ID 21
#define MENU_EDIT_SEARCHINFILES_ID 22
#define MENU_EDIT_BOOKMARKS_TOGGLE_ID 23
#define MENU_EDIT_BOOKMARKS_FIRST_ID 24
#define MENU_EDIT_BOOKMARKS_PREVIOUS_ID 25
#define MENU_EDIT_BOOKMARKS_NEXT_ID 26
#define MENU_EDIT_BOOKMARKS_LAST_ID 27
#define MENU_EDIT_BOOKMARKS_CLEARALL_ID 28
#define MENU_EDIT_GOTO_TOP_ID 29
#define MENU_EDIT_GOTO_BOTTOM_ID 30
#define MENU_EDIT_GOTO_LINE_ID 31
#define MENU_EDIT_GOTO_MATCHINGBRACKET_ID 32
#define MENU_EDIT_GOTO_PREVIOUSWORD_ID 33
#define MENU_EDIT_GOTO_NEXTWORD_ID 34
#define MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID 35
#define MENU_EDIT_GOTO_NEXTPROCEDURE_ID 36
#define MENU_EDIT_GOTO_VARIABLEDECLARATION_ID 37
#define MENU_EDIT_GOTO_VARIABLENEXTUSE_ID 38
#define MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID 39
#define MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID 40
#define MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID 41
#define MENU_EDIT_BLOCK_INDENT_ID 42
#define MENU_EDIT_BLOCK_UNINDENT_ID 43
#define MENU_EDIT_BLOCK_UPPERCASE_ID 44
#define MENU_EDIT_BLOCK_LOWERCASE_ID 45
#define MENU_EDIT_BLOCK_COMMENT_ID 46
#define MENU_EDIT_BLOCK_UNCOMMENT_ID 47
#define MENU_EDIT_BLOCK_TABIFY_ID 48
#define MENU_EDIT_BLOCK_UNTABIFY_ID 49
#define MENU_EDIT_BLOCK_REGISTERS_ID 50
#define MENU_EDIT_BLOCK_SAVEAS_ID 51
#define MENU_EDIT_BLOCK_PRINT_ID 52
#define MENU_EDIT_BLOCK_BLKCOMMENT_ID 53
#define MENU_EDIT_BLOCK_TOGGLECASE_ID 54
#define MENU_EDIT_BLOCK_SPACESTOTABS_ID 55
#define MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID 56
#define MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID 57
#define MENU_EDIT_PASTE_NEWWINDOW_ID 58
#define MENU_EDIT_DELETE_DELETEPARAGRAPH_ID 59
#define MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID 60
#define MENU_EDIT_GOTO_NEXTPARAGRAPH_ID 61
#define MENU_EDIT_BLOCK_TRIMSPACES_ID 62
#define MENU_EDIT_BREAKPOINTS_TOGGLE_ID 63
#define MENU_EDIT_BREAKPOINTS_FIRST_ID 64
#define MENU_EDIT_BREAKPOINTS_PREVIOUS_ID 65
#define MENU_EDIT_BREAKPOINTS_NEXT_ID 66
#define MENU_EDIT_BREAKPOINTS_LAST_ID 67
#define MENU_EDIT_BREAKPOINTS_CLEARALL_ID 68
#define MENU_EDIT_LAST_ID MENU_EDIT_BREAKPOINTS_CLEARALL_ID

#define MENU_VIEW_STATUSBAR_ID 0
#define MENU_VIEW_PROJECTDOCK_ID 1
#define MENU_VIEW_WINDOWSDOCK_ID 2
#define MENU_VIEW_OUTPUTDOCK_ID 3
#define MENU_VIEW_DEBUGDOCK_ID 4
#define MENU_VIEW_REGISTERSDOCK_ID 5
#define MENU_VIEW_PROPERTIES_ID 6
#define MENU_VIEW_USERMENUSEDITOR_ID 7
#define MENU_VIEW_USERTOOLBARSEDITOR_ID 8
#define MENU_VIEW_FULLSCREEN_ID 9
#define MENU_VIEW_LAST_ID MENU_VIEW_FULLSCREEN_ID

#define MENU_PROJECT_CREATENEWPROJECT_ID 0
#define MENU_PROJECT_OPENPROJECT_ID 1
#define MENU_PROJECT_SAVEPROJECT_ID 2
#define MENU_PROJECT_SAVEPROJECTAS_ID 3
#define MENU_PROJECT_ADDINCLUDE_ID 4
#define MENU_PROJECT_ADDLIBRARY_ID 5
#define MENU_PROJECT_ADDMODULE_ID 6
#define MENU_PROJECT_ADDOBJECT_ID 7
#define MENU_PROJECT_ADDRESOURCE_ID 8
#define MENU_PROJECT_ADDTEXT_ID 9
#define MENU_PROJECT_ADDICON_ID 10
#define MENU_PROJECT_ADDCURSOR_ID 11
#define MENU_PROJECT_ADDBITMAP_ID 12
#define MENU_PROJECT_ADDSTRINGS_ID 13
#define MENU_PROJECT_ADDACCELERATORS_ID 14
#define MENU_PROJECT_ADDMENUS_ID 15
#define MENU_PROJECT_ADDDIALOG_ID 16
#define MENU_PROJECT_ADDRAWDATA_ID 17
#define MENU_PROJECT_PROJECTPROPERTIES_ID 18
#define MENU_PROJECT_ASSEMBLEPROJECT_ID 19
#define MENU_PROJECT_RUNPROJECT_ID 20
#define MENU_PROJECT_DEBUGPROJECT_ID 21
#define MENU_PROJECT_LAST_ID MENU_PROJECT_DEBUGPROJECT_ID

#define MENU_ADDINS_MANAGER_ID 0
// Start of ordinal enumeration
#define MENU_ADDINS_FIRSTADDIN_ID 1

#define MENU_WINDOW_CONTEXTHELP_ID 0
#define MENU_WINDOW_MSDNHELP_ID 1
#define MENU_WINDOW_ABOUT_ID 2
#define MENU_WINDOW_SPLIT_ID 3
#define MENU_WINDOW_TILEHORIZONTALLY_ID 4
#define MENU_WINDOW_TILEVERTICALLY_ID 5
#define MENU_WINDOW_CASCADE_ID 6
#define MENU_WINDOW_ARRANGEICONS_ID 7
#define MENU_WINDOW_NEXTWINDOW_ID 8
#define MENU_WINDOW_PREVIOUSWINDOW_ID 9
#define MENU_WINDOW_DDKHELP_ID 10
#define MENU_WINDOW_DIRECTXSDKHELP_ID 11
#define MENU_WINDOW_TIPOFTHEDAY_ID 12
#define MENU_WINDOW_LAST_ID MENU_WINDOW_TIPOFTHEDAY_ID

#define PROJECTTOOLBAR_SAVE 1
#define PROJECTTOOLBAR_ASSEMBLE 2
#define PROJECTTOOLBAR_RUN 3
#define PROJECTTOOLBAR_DEBUG 4

#define WINDOWSTOOLBAR_PREVIOUS 1
#define WINDOWSTOOLBAR_NEXT 2
#define WINDOWSTOOLBAR_RELOAD 3

#define STATUSTOOLBAR_SAVE 1
#define STATUSTOOLBAR_NEW 2
#define STATUSTOOLBAR_CLEAR 5

#define MENU_CONTEXT_CONTEXTHELP_ID 0
#define MENU_CONTEXT_MSDNHELP_ID 1
#define MENU_CONTEXT_DDKHELP_ID 2
#define MENU_CONTEXT_DIRECTXSDKHELP_ID 3
#define MENU_CONTEXT_OPENFILEUNDERCURSOR_ID 4
#define MENU_CONTEXT_EXECFILEUNDERCURSOR_ID 5
#define MENU_CONTEXT_DECTOHEX_ID 6
#define MENU_CONTEXT_DECTOBIN_ID 7
#define MENU_CONTEXT_HEXTODEC_ID 8
#define MENU_CONTEXT_HEXTOBIN_ID 9
#define MENU_CONTEXT_BINTODEC_ID 10
#define MENU_CONTEXT_BINTOHEX_ID 11
#define MENU_CONTEXT_HEXTOFLT_ID 12
#define MENU_CONTEXT_FLTTOHEX_ID 13
#define MENU_CONTEXT_UNDO_ID 14
#define MENU_CONTEXT_REDO_ID 15
#define MENU_CONTEXT_CUT_ID 16
#define MENU_CONTEXT_COPY_ID 17
#define MENU_CONTEXT_COPYFILEPATHNAME_ID 18
#define MENU_CONTEXT_COPYENTIRETEXT_ID 19
#define MENU_CONTEXT_PASTE_ID 20
#define MENU_CONTEXT_PASTENEWWINDOW_ID 21
#define MENU_CONTEXT_SELECTLINE_ID 22
#define MENU_CONTEXT_SELECTALL_ID 23
#define MENU_CONTEXT_SELECTPROCEDURE_ID 24
#define MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID 25
#define MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID 26
#define MENU_CONTEXT_FIND_ID 27
#define MENU_CONTEXT_REPLACE_ID 28
#define MENU_CONTEXT_BOOKMARKS_TOGGLE_ID 29
#define MENU_CONTEXT_BOOKMARKS_FIRST_ID 30
#define MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID 31
#define MENU_CONTEXT_BOOKMARKS_NEXT_ID 32
#define MENU_CONTEXT_BOOKMARKS_LAST_ID 33
#define MENU_CONTEXT_BOOKMARKS_CLEARALL_ID 34
#define MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID 35
#define MENU_CONTEXT_BREAKPOINTS_FIRST_ID 36
#define MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID 37
#define MENU_CONTEXT_BREAKPOINTS_NEXT_ID 38
#define MENU_CONTEXT_BREAKPOINTS_LAST_ID 39
#define MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID 40
#define MENU_CONTEXT_GOTO_TOP_ID 41
#define MENU_CONTEXT_GOTO_BOTTOM_ID 42
#define MENU_CONTEXT_GOTO_LINE_ID 43
#define MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID 44
#define MENU_CONTEXT_GOTO_PREVIOUSWORD_ID 45
#define MENU_CONTEXT_GOTO_NEXTWORD_ID 46
#define MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID 47
#define MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID  48
#define MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID 49
#define MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID  50
#define MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID 51
#define MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID 52
#define MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID 53
#define MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID 54
#define MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID 55
#define MENU_CONTEXT_BLOCK_INDENT_ID 56
#define MENU_CONTEXT_BLOCK_UNINDENT_ID 57
#define MENU_CONTEXT_BLOCK_UPPERCASE_ID 58
#define MENU_CONTEXT_BLOCK_LOWERCASE_ID 59
#define MENU_CONTEXT_BLOCK_TOGGLECASE_ID 60
#define MENU_CONTEXT_BLOCK_COMMENT_ID 61
#define MENU_CONTEXT_BLOCK_UNCOMMENT_ID 62
#define MENU_CONTEXT_BLOCK_TABIFY_ID 63
#define MENU_CONTEXT_BLOCK_UNTABIFY_ID 64
#define MENU_CONTEXT_BLOCK_SPACESTOTABS_ID 65
#define MENU_CONTEXT_BLOCK_REGISTERS_ID 66
#define MENU_CONTEXT_BLOCK_BLKCOMMENT_ID 67
#define MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID 68
#define MENU_CONTEXT_BLOCK_TRIMSPACES_ID 69
#define MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID 70
#define MENU_CONTEXT_BLOCK_SAVEAS_ID 71
#define MENU_CONTEXT_BLOCK_PRINT_ID 72
#define MENU_CONTEXT_NEW_ID 73
#define MENU_CONTEXT_INCLUDEFILE_ID 74
#define MENU_CONTEXT_INCLUDEFILTERS_ID 75
#define MENU_CONTEXT_DUPLICATEFILE_ID 76
#define MENU_CONTEXT_RELOADFILE_ID 77
#define MENU_CONTEXT_PROPERTIES_ID 78
#define MENU_CONTEXT_LAST_ID MENU_CONTEXT_PROPERTIES_ID

#endif
