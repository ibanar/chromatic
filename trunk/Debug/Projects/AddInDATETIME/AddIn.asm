; ------------------------------------------
; Title: AddinASCII
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
;
; Version: 1.1
; Lastest revision: 19/07/2004
; ------------------------------------------

; --------------- Procedures prototypes
DisplayWindow           proto
DateWinHook             proto   :dword,:dword,:dword,:dword
DateInitProc            proto   :dword
PasteDate               proto
GetDateFrm              proto   :dword
GetTimeFrm              proto   :dword

; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions

; --- AddInDescription()
; In: N/A
; Out: Can return description string in eax or 0
AddInDescription        proc
                        mov     eax,offset MsgDescription
                        ret
AddInDescription        endp

; --- AddInAuthor()
; In: N/A
; Out: Can return author string in eax or 0
AddInAuthor             proc
                        mov     eax,offset MsgAuthor
                        ret
AddInAuthor             endp

; --- AddInLoad()
; In: Pointer to WALIB structure
; Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax
AddInLoad               proc    WALIBStruct:dword
                        pushad
                        mov     eax,WALIBStruct
                        mov     WAStructPtr,eax
                        WALIBINVOKE WAMMGetContext,addr AddInContext
                        mov     eax,AddInContext.ColdStart
                        test    eax,eax
                        jz      DisplayOnDemand
                        ; It's a coldstart so check if the window was displayed
                        ; in the previous session.
                        WALIBINVOKE WADockingBoxWasVisible,addr MsgTitle
                        test    eax,eax
                        jz      EndLoadSequence
DisplayOnDemand:        invoke  DisplayWindow
                        jz      EndLoadSequence
                        popad
                        ; Request for an augmented lifespan
                        mov     eax,ADDIN_PERSISTANT
                        ret
EndLoadSequence:        popad
                        mov     eax,ADDIN_FINISHED
                        ret
AddInLoad               endp

; --- AddInUnLoad()
; In: N/A
; Out: N/A
AddInUnLoad             proc
                        pushad
                        .if FlagActive != FALSE
                                WALIBINVOKE WADockingBoxRemove,hWndDateSel
                        .endif
                        popad
                        ret
AddInUnLoad             endp

; --- AddInMenu()
; In: Pointer to WALIB structure
; Out: Must return ADDIN_DIE or ADDIN_ZOMBIE in eax
AddInMenu               proc    WALIBStruct:dword
                        pushad
                        mov     eax,WALIBStruct
                        mov     WAStructPtr,eax
                        invoke  DisplayWindow
                        jz      EndMenuSequence
                        popad
                        mov     eax,ADDIN_ZOMBIE
                        ret
EndMenuSequence:        popad
                        mov     eax,ADDIN_DIE
                        ret
AddInMenu               endp

; --- Create the window and display it
; In: N/A
; Out: return -1 on success
DisplayWindow           proc
                        WALIBINVOKE WAMMGetContext,addr AddInContext
                        .if FlagActive == FALSE
                                WALIBINVOKE WACreateDockingBox,addr MsgTitle,addr DateWinHook
                                mov     hWndDateSel,eax
                                test    eax,eax
                                jz      ErrorWindow
                                invoke  DateInitProc,eax
                        .endif
                        WALIBINVOKE WADockingBoxShow,hWndDateSel,216,230,WADOCKINGBOX_RIGHT
                        mov     FlagActive,TRUE
                        or      eax,-1
ErrorWindow:            ret
DisplayWindow           endp

; --- Initialize window
; Return -1 on success
DateInitProc            proc    hWnd:dword
                        pushad
                        WALIBINVOKE WAGDIGetSerif
                        mov SerifFont,eax
                        WALIBINVOKE WACreateListBox,0,1,217,178,hWnd,0,0,0,WS_TABSTOP Or WS_HSCROLL,WS_EX_STATICEDGE
                        mov     hPropertiesLbFileExt,eax
                        WALIBINVOKE WAMMGetIniFilePathName,INIFILE_DATES
                        mov     DatesIniFile,eax
                        xor     ecx,ecx
RetrieveFormats:        push    ecx
                        invoke  GetDateFrm,ecx
                        mov     ecx,[esp]
                        invoke  GetTimeFrm,ecx
                        invoke  lstrlen,UserDateFormat
                        push    eax
                        invoke  lstrlen,UserTimeFormat
                        pop     ebx
                        add     ebx,eax
                        jnz     ContinueFormatReading
                        pop     ecx
                        jmp     StopFormatReading
ContinueFormatReading:  WALIBINVOKE WAStringReplace,UserTimeFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                        push    eax
                        WALIBINVOKE WAStringReplace,UserDateFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                        WALIBINVOKE WADateGetFormat,eax
                        pop     ecx
                        push    eax
                        WALIBINVOKE WADateGetTimeFormat,ecx
                        pop     ecx
                        WALIBINVOKE WAStringCat,ecx,eax
                        push    eax
                        WALIBINVOKE WAGDIGetTextWidth,hPropertiesLbFileExt,SerifFont,eax
                        .if     MaxText < eax
                                mov     MaxText,eax
                        .endif
                        pop     eax
                        WALIBINVOKE WAListBoxAddItem,hPropertiesLbFileExt,eax,-1
                        pop     ecx
                        inc     ecx
                        cmp     ecx,1000
                        jne     RetrieveFormats
StopFormatReading:      WALIBINVOKE WAListBoxCount,hPropertiesLbFileExt
                        test    eax,eax
                        jz      ListIsEmpty
                        WALIBINVOKE WAListBoxSetIndex,hPropertiesLbFileExt,0
ListIsEmpty:            popad
                        ret
DateInitProc            endp

; --- Window proc
DateWinHook             proc    hWnd:dword,uMsg:dword,wParam:dword,lParam:dword
                        local   PaintS:PAINTSTRUCT
                        pushad
                        .if uMsg == WM_COMMAND
                                mov     eax,lParam
                                .if eax == hPropertiesLbFileExt
                                        WALIBINVOKE WAControlGetNotifiedCommand,wParam
                                        .if eax == LBN_DBLCLK
                                                invoke  PasteDate
                                        .endif
                                        popad
                                        xor     eax,eax
                                        ret
                                .endif
                        .elseif uMsg == WM_PAINT
                                invoke BeginPaint,hWnd,addr PaintS
                                WALIBINVOKE WAControlWidth,hWnd
                                mov ebx,eax
                                WALIBINVOKE WAControlHeight,hWnd
                                invoke MoveWindow,hPropertiesLbFileExt,0,0,ebx,eax,1
                                WALIBINVOKE WAListBoxSetHorzScrollWidth,hPropertiesLbFileExt,MaxText
                                invoke EndPaint,hWnd,addr PaintS
                        ; Handle dockwindow messages
                        .elseif uMsg == WADOCKINGBOX_MSG_QUERY_STATE
                                popad
                                mov     eax,FlagActive
                                ret
                        .elseif uMsg == WM_DESTROY || uMsg == WADOCKINGBOX_MSG_CLOSE
                                mov     FlagActive,FALSE
                                WALIBINVOKE WAMMAddInKillZombie,addr AddInUnLoad
                                popad
                                xor     eax,eax
                                ret
                        .endif
                        popad
                        WALIBINVOKE WAControlNextHook,hWnd,uMsg,wParam,lParam
                        ret
DateWinHook             endp

; --- Paste the date in current text window
PasteDate               proc
                        local   CurrentLstIndex:dword
                        WALIBINVOKE WAMMGetContext,addr AddInContext
                        .if AddInContext.NbrChilds != 0 
                                WALIBINVOKE WAListBoxGetSelItemIndex,hPropertiesLbFileExt
                                mov     CurrentLstIndex,eax
                                invoke  GetDateFrm,CurrentLstIndex
                                invoke  GetTimeFrm,CurrentLstIndex
                                invoke  lstrlen,UserDateFormat
                                push    eax
                                invoke  lstrlen,UserTimeFormat
                                pop     ebx
                                add     ebx,eax
                                jz      StopFormatPasting
StopFormatPasting:              WALIBINVOKE WAStringReplace,UserTimeFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                                push    eax
                                WALIBINVOKE WAStringReplace,UserDateFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                                WALIBINVOKE WADateGetFormat,eax
                                pop     ecx
                                push    eax
                                WALIBINVOKE WADateGetTimeFormat,ecx
                                pop     ecx
                                WALIBINVOKE WAStringCat,ecx,eax
                                WALIBINVOKE WAMMInsertText,AddInContext.hCurrentChild,eax
                                WALIBINVOKE WAMMSetFocusInsideWindow,AddInContext.hCurrentChild
                        .else
                                WALIBINVOKE WAMiscMsgBox,AddInContext.hMDI,addr MsgChildError,MB_ERROR
                        .endif
                        ret
PasteDate               endp

; --- Retrieve date format from dates.ini file
GetDateFrm              proc    FormatNumber:dword
                        WALIBINVOKE WAStringNumberComplement,FormatNumber,3
                        WALIBINVOKE WAStringCat,addr MsgDateFormat,eax
                        WALIBINVOKE WAIniReadKey,addr MsgFormats,eax,DatesIniFile
                        mov     UserDateFormat,eax
                        ret
GetDateFrm              endp

; --- Retrieve time format from dates.ini file
GetTimeFrm              proc    FormatNumber:dword
                        WALIBINVOKE WAStringNumberComplement,FormatNumber,3
                        WALIBINVOKE WAStringCat,addr MsgTimeFormat,eax
                        WALIBINVOKE WAIniReadKey,addr MsgFormats,eax,DatesIniFile
                        mov     UserTimeFormat,eax
                        ret
GetTimeFrm              endp

end LibMain
