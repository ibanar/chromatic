/* ------------------------------------
Title: RawToC filter

Author: Franck Charlet

Purpose: Convert raw binary datas into C datas statements

Notes: Terminus filter

------------------------------------ */

/* --------------- DLL's entry point */

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

/* --------------- DLL's functions */

/* --- FilterDescription()
   In: N/A
   Out: Can return filter description or NULL */
char * __stdcall FilterDescription() {
	return(MsgDescription);
}

/* --- FilterAuthor()
   In: N/A
   Out: Can return author's name or NULL */
char * __stdcall FilterAuthor() {
	return(MsgAuthor);
}

/* --- FilterProc()
   In: Filename: Name of the file
       File: pointer to a WAFILTERFILE containing file infos
       WALIBStruct: WALibrary support entry point
   Out: Must return File or NULL */
LPWAFILTERFILE __stdcall FilterProc(char *FileName, LPWAFILTERFILE File, LPWALIB WALIBStruct) {
	char *Output_Buffer;
	char *Output_Buffer_End;
	char *Output_Buffer_Final;
	char *FileName_Label;
	int File_Len = File->FileLength;
	int File_Len16 = File_Len / 16;
	int File_Remainder = File_Len % 16;

	/* Set buffer to max len */
	Output_Buffer = WALIBStruct->WAStringString(File_Len16 * strlen(TabHex[0]), ' ');
	/* Remaining bytes */
	Output_Buffer_End = WALIBStruct->WAStringString(strlen(TabHex[0]), ' ');
	/* Fill the buffer */
	WALIBStruct->WAMiscCopyMemLoop(TabHex[0], Output_Buffer, strlen(TabHex[0]), 0, File_Len16);
	/* Fill the remaining buffer (TabHex is read only) */
	if(File_Remainder != 0) Output_Buffer_End = strdup(TabHex[16 - File_Remainder]);
	/* Convert main part */
	if(File_Len > 15) WALIBStruct->WAStringFillHexBytes(File->FileMem, Output_Buffer, 16, 3, 3, File_Len16, 3);
	/* Convert remaining bytes */
	if(File_Remainder != 0) WALIBStruct->WAStringFillHexBytes(File->FileMem + (File_Len - File_Remainder), Output_Buffer_End, File_Remainder, 3, 0, 1, 3);
    	/* Write the header */
    	Output_Buffer_Final = WALIBStruct->WAStringCat("/*\r\nCreated from file: ", FileName);
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "\r\n");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "Original length: ");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, WALIBStruct->WAStringDecToString(File_Len));
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, " byte");
    	if(File_Len > 1) Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "s");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "\r\n*/\r\n\r\n");
    	FileName_Label = WALIBStruct->WAStringReplace(WALIBStruct->WAFileReplaceExtension(WALIBStruct->WAFileGetFileName(FileName), ""), ".", "", 1, -1, NOT_CASE_SENSITIVE);
    	WALIBStruct->WAStringFileNameToLabel(FileName_Label);
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "#define ");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, FileName_Label);
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "_size ");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, WALIBStruct->WAStringDecToString(File_Len));
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "\r\n");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "unsigned char ");
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, FileName_Label);
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "_Dats[] = {\r\n");
	/* Write main buffer */
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, Output_Buffer);
	/* Write remaining bytes */
    	Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, WALIBStruct->WAStringRTrim(Output_Buffer_End));
    	if(File_Remainder == 0) Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "};\r\n");
	else Output_Buffer_Final = WALIBStruct->WAStringCat(Output_Buffer_Final, "\r\n};\r\n");
	/* Set new buffer */
	File->FileMem = Output_Buffer_Final;
	File->FileLength = strlen(Output_Buffer_Final);
	if(File_Remainder != 0) free(Output_Buffer_End);
	return(NULL);
}