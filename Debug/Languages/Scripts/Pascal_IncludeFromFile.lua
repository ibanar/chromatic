(defvar Context,(call WALib(WAMiscAllocMem),WAContext_Size))
(if !Context,(end))
(call WALib(WAMMGetContext),Context)
(defvar File,(call WALib(WAComDlgGetOpenFileName),*Context#WAContext_hMDI,"All files (*.*)|*.*", (call WALib(WAMMGetQuickDirectory),DIR_INCLUDE),0))
(if !(call WALib(WAStringLen),File),(end))
(defvar WordPaste,(call WALib(WAStringCat),"{$include ", File))
(setvar WordPaste,(call WALib(WAStringCat),WordPaste,"}\r\n"))
(call WALib(WAMMInsertText),*Context#WAContext_hCurrentChild,WordPaste)
(call WALib(WAMiscFreeMem),Context)
