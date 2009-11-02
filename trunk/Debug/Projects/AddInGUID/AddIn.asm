; ------------------------------------------
; Title: AddInGUID
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
;
; Version: 1.0
; Lastest revision: 05/05/2003
; ------------------------------------------

; --------------- Procedures prototypes
ReplaceGUIDElement32    proto   :dword,:dword
ReplaceGUIDElement16    proto   :dword,:dword
ReplaceGUIDElement8     proto   :dword,:dword

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
                        popad
                        mov     eax,ADDIN_FINISHED
                        ret
AddInLoad               endp

; --- AddInUnLoad()
; In: N/A
; Out: N/A
AddInUnLoad             proc
                        pushad
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
                        WALIBINVOKE WAMMGetContext,addr AddInContext
                        .if AddInContext.NbrChilds != 0
                                WALIBINVOKE WAMMGetWindowLangGUID,AddInContext.hCurrentChild
                                mov     GUIDSkew,eax
                                WALIBINVOKE WAStringLen,GUIDSkew
                                .if eax == 0
                                        WALIBINVOKE WAMiscMsgBox,AddInContext.hMDI,addr MsgGUIDError,MB_ERROR
                                        jmp     NoForms
                                .endif
EmptyGUIDDef:                   WALIBINVOKE WAMiscObtainGUID,addr GUIDToInsert
                                INVOKE  ReplaceGUIDElement32,GUIDToInsert.Data1,addr MsgGUIDPart1
                                movzx   eax,GUIDToInsert.Data2
                                INVOKE  ReplaceGUIDElement16,eax,addr MsgGUIDPart2
                                movzx   eax,GUIDToInsert.Data3
                                INVOKE  ReplaceGUIDElement16,eax,addr MsgGUIDPart3
                                movzx   eax,GUIDToInsert.Data4[0]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart4
                                movzx   eax,GUIDToInsert.Data4[1]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart5
                                movzx   eax,GUIDToInsert.Data4[2]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart6
                                movzx   eax,GUIDToInsert.Data4[3]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart7
                                movzx   eax,GUIDToInsert.Data4[4]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart8
                                movzx   eax,GUIDToInsert.Data4[5]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart9
                                movzx   eax,GUIDToInsert.Data4[6]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart10
                                movzx   eax,GUIDToInsert.Data4[7]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart11
                                WALIBINVOKE WAMMInsertText,AddInContext.hCurrentChild,GUIDSkew
                        .else
                                WALIBINVOKE WAMiscMsgBox,AddInContext.hMDI,addr MsgChildError,MB_ERROR
                        .endif
NoForms:                popad
                        mov     eax,ADDIN_DIE
                        ret
AddInMenu               endp

; --- ReplaceGUIDElement32()
; Replace a 32 bits GUID element in the GUID skeleton
ReplaceGUIDElement32    proc    GUIDDat:dword,MatchingPattern:dword
                        WALIBINVOKE WAStringHex32,GUIDDat
                        WALIBINVOKE WAStringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement32    endp


; --- ReplaceGUIDElement16()
; Replace a 16 bits GUID element in the GUID skeleton
ReplaceGUIDElement16    proc    GUIDDat:dword,MatchingPattern:dword
                        WALIBINVOKE WAStringHex16,GUIDDat
                        WALIBINVOKE WAStringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement16    endp

; --- ReplaceGUIDElement8()
; Replace a 8 bits GUID element in the GUID skeleton
ReplaceGUIDElement8     proc    GUIDDat:dword,MatchingPattern:dword
                        local   GUIDAsciiDat:dword
                        WALIBINVOKE WAStringHex8,GUIDDat
                        WALIBINVOKE WAStringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement8     endp

end LibMain
