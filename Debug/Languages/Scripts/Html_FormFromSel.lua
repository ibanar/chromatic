(defvar Context,(call WALib(WAMiscAllocMem),WAContext_Size))
(if !Context,(end))
(call WALib(WAMMGetContext),Context)
(defvar Word,(call WALib(WAMMSelectCurrentWord),*Context#WAContext_hCurrentChild))
(if !(call WALib(WAStringLen),Word),(end))
(defvar WordPaste,(call WALib(WAStringCat),"<form name=\"",Word))
(setvar WordPaste,(call WALib(WAStringCat),WordPaste,"\" method=\"post\" action=\"\">\r\n</form>"))
(call WALib(WAMMInsertText),*Context#WAContext_hCurrentChild,WordPaste)
(call WALib(WAMiscFreeMem),Context)
