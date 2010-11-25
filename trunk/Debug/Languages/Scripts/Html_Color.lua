(defvar Context,(call WALib(WAMiscAllocMem),WAContext_Size))
(if !Context,(end))
(call WALib(WAMMGetContext),Context)
(defvar Color)
(if !(call WALib(WAComDlgChooseColor),*Context#WAContext_hMDI,&Color),(end))
(defvar WordPaste,(call WALib(WAStringCat),"\"#", (call WALib(WAStringHexNumberComplement),Color,6)))
(setvar WordPaste,(call WALib(WAStringCat),WordPaste,"\""))
(call WALib(WAMMInsertText),*Context#WAContext_hCurrentChild,WordPaste)
(call WALib(WAMiscFreeMem),Context)