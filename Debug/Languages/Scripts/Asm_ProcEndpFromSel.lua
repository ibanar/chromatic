(defvar Context,(call WALib(WAMiscAllocMem),WAContext_Size))
(if !Context,(end))
(call WALib(WAMMGetContext),Context)
(defvar Word,(call WALib(WAMMSelectCurrentWord),*Context#WAContext_hCurrentChild))
(if !(call WALib(WAStringLen),Word),(end))
(defvar WordPaste,(call WALib(WAStringCat),Word,"\tproc\r\n\tret\r\n"))
(setvar WordPaste,(call WALib(WAStringCat),WordPaste,Word))
(setvar WordPaste,(call WALib(WAStringCat),WordPaste,"\tendp\r\n"))
(call WALib(WAMMInsertText),*Context#WAContext_hCurrentChild,WordPaste)
(call WALib(WAMiscFreeMem),Context)
