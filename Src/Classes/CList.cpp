// -----------------------------------------------------------------------
// Chrome
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
// CList.cpp: implementation of the CList class
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "CList.h"

// -----------------------------------------------------------------------
// Default constructor
CList::CList() {
	this->Nbr_Entries = 0;
	this->Last_Entry = 0;
	this->First_Entry = 0;
}

// -----------------------------------------------------------------------
// Default destructor
CList::~CList() {
	// Just in case
	this->Clear_All(false);
}

// -----------------------------------------------------------------------
// Return the current number of entries in the list
long CList::Get_Number_Of_Entries(void) {
	return(this->Nbr_Entries);
}

// -----------------------------------------------------------------------
// Check if an entry already exists
// Return corresponding LLEntry or 0
CList_Entry *CList::Is_Entry(long Datas) {
	CList_Entry *Entry_To_Test;

	// Take first entry in the list
	Entry_To_Test = this->First_Entry;
	while(Entry_To_Test) {
		if((Entry_To_Test->u.Long == Datas)) return Entry_To_Test;
		Entry_To_Test = Entry_To_Test->Next;
	}
	return(0);
}

// -----------------------------------------------------------------------
// Create a new entry
CList_Entry *CList::Create_Entry(void) {
	CList_Entry *New_Entry;
	CList_Entry *Old_Entry;

	New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	if(!this->First_Entry) First_Entry = New_Entry;
	if(this->Last_Entry) {
		// Update previous entry
		Old_Entry = this->Last_Entry;
		Old_Entry->Next = New_Entry;
	}
	// Last entry
	New_Entry->Next = 0;
	New_Entry->Previous = this->Last_Entry;
	// Store newly created entry
	this->Last_Entry = New_Entry;
	// Increment number of entries
	this->Nbr_Entries++;
	return(New_Entry);
}

CList_Entry *CList::Add_Entry(long Datas) {
	CList_Entry *New_Entry = Create_Entry();
	New_Entry->u.Long = Datas;
	return(New_Entry);
}

CList_Entry *CList::Add_Entry(char *Datas) {
	CList_Entry *New_Entry = Create_Entry();
	New_Entry->u.Char = strdup(Datas);
	return(New_Entry);
}

CList_Entry *CList::Add_Entry(float Datas) {
	CList_Entry *New_Entry = Create_Entry();
	New_Entry->u.Float = Datas;
	return(New_Entry);
}

CList_Entry *CList::Add_Entry(double Datas) {
	CList_Entry *New_Entry = Create_Entry();
	New_Entry->u.Double = Datas;
	return(New_Entry);
}

// -----------------------------------------------------------------------
// Create a several entries
void CList::Add_Entries(long Number, long Datas) {
	while(Number) {
		this->Add_Entry(Datas);
		Number--;
	}
}

// -----------------------------------------------------------------------
// Create a several entries
void CList::Add_Entries(long Number, char *Datas) {
	while(Number) {
		this->Add_Entry(Datas);
		Number--;
	}
}

// -----------------------------------------------------------------------
// Create a several entries
void CList::Add_Entries(long Number, float Datas) {
	while(Number) {
		this->Add_Entry(Datas);
		Number--;
	}
}

// -----------------------------------------------------------------------
// Create a several entries
void CList::Add_Entries(long Number, double Datas) {
	while(Number) {
		this->Add_Entry(Datas);
		Number--;
	}
}

// -----------------------------------------------------------------------
// Clear all entries
void CList::Clear_All(bool Free_Datas) {
	CList_Entry *Entry_To_Free;

	// Take first entry in the list
	Entry_To_Free = this->First_Entry;
	if(Free_Datas == true) {
		// Release the memory allocated inside datas too
		while(Entry_To_Free) {
			// Next one to free
			this->First_Entry = Entry_To_Free->Next;
			if(Entry_To_Free->u.Long) free((void *) Entry_To_Free->u.Long);
			free(Entry_To_Free);
			Entry_To_Free = this->First_Entry;
		}
	} else {
		while(Entry_To_Free) {
			// Next one to free
			this->First_Entry = Entry_To_Free->Next;
			free(Entry_To_Free);
			Entry_To_Free = this->First_Entry;
		}
	}
	this->Nbr_Entries = 0;
	this->Last_Entry = 0;
	this->First_Entry = 0;
	return;
}

// -----------------------------------------------------------------------
// Retrieve an entry from an ordinal position
CList_Entry *CList::Get_Entry_By_Ordinal(long Entry_Number) {
	CList_Entry *Entry_To_Get;

	// Take first entry in the list
	Entry_To_Get = this->First_Entry;
	// Take concerned entry
	while(Entry_Number) {
		Entry_To_Get = Entry_To_Get->Next;
		Entry_Number--;
	}
	return(Entry_To_Get);
}

long CList::Get_Entry_By_Ordinal_Long(long Entry_Number) {
	return(Get_Entry_By_Ordinal(Entry_Number)->u.Long);
}

char *CList::Get_Entry_By_Ordinal_Char(long Entry_Number) {
	return(Get_Entry_By_Ordinal(Entry_Number)->u.Char);
}

float CList::Get_Entry_By_Ordinal_Float(long Entry_Number) {
	return(Get_Entry_By_Ordinal(Entry_Number)->u.Float);
}

double CList::Get_Entry_By_Ordinal_Double(long Entry_Number) {
	return(Get_Entry_By_Ordinal(Entry_Number)->u.Double);
}

long *CList::Get_Entry_By_Ordinal_Addr(long Entry_Number) {
	return(&Get_Entry_By_Ordinal(Entry_Number)->u.Long);
}

// -----------------------------------------------------------------------
// Store datas to an ordinal position

CList_Entry *CList::Set_Entry_By_Ordinal(long Entry_Number, long Datas, bool Free_Datas) {
	CList_Entry *Entry_To_Get = Get_Entry_By_Ordinal(Entry_Number);
	if(Free_Datas == true) if(Entry_To_Get->u.Long) free((void *) Entry_To_Get->u.Long);
	Entry_To_Get->u.Long = Datas;
	return(Entry_To_Get);
}

CList_Entry *CList::Set_Entry_By_Ordinal(long Entry_Number, char *Datas) {
	CList_Entry *Entry_To_Get = Get_Entry_By_Ordinal(Entry_Number);
	if(Entry_To_Get->u.Char) free((void *) Entry_To_Get->u.Char);
	Entry_To_Get->u.Char = strdup(Datas);
	return(Entry_To_Get);
}

CList_Entry *CList::Set_Entry_By_Ordinal(long Entry_Number, float Datas) {
	CList_Entry *Entry_To_Get = Get_Entry_By_Ordinal(Entry_Number);
	Entry_To_Get->u.Float = Datas;
	return(Entry_To_Get);
}

CList_Entry *CList::Set_Entry_By_Ordinal(long Entry_Number, double Datas) {
	CList_Entry *Entry_To_Get = Get_Entry_By_Ordinal(Entry_Number);
	Entry_To_Get->u.Double = Datas;
	return(Entry_To_Get);
}

// -----------------------------------------------------------------------
// Retrieve an entry datas from it's CList_Entry
long CList::Get_Entry_By_Address(CList_Entry *Entry_To_Get) {
	CList_Entry *Current_Entry;

	// Take first entry in the list
	Current_Entry = this->First_Entry;
	// Take concerned entry
	while(Current_Entry != Entry_To_Get) {
		Current_Entry = Current_Entry->Next;
	}
	return(Current_Entry->u.Long);
}

// -----------------------------------------------------------------------
// Store datas from an address
CList_Entry *CList::Set_Entry_By_Address(CList_Entry *Entry_To_Set, long Datas, bool Free_Datas) {
	if(Free_Datas == true) if(Entry_To_Set->u.Long) free((void *) Entry_To_Set->u.Long);
	Entry_To_Set->u.Long = Datas;
	return(Entry_To_Set);
}

CList_Entry *CList::Set_Entry_By_Address(CList_Entry *Entry_To_Set, char *Datas) {
	if(Entry_To_Set->u.Char) free((void *) Entry_To_Set->u.Char);
	Entry_To_Set->u.Char = strdup(Datas);
	return(Entry_To_Set);
}

CList_Entry *CList::Set_Entry_By_Address(CList_Entry *Entry_To_Set, float Datas) {
	Entry_To_Set->u.Float = Datas;
	return(Entry_To_Set);
}

CList_Entry *CList::Set_Entry_By_Address(CList_Entry *Entry_To_Set, double Datas) {
	Entry_To_Set->u.Double = Datas;
	return(Entry_To_Set);
}

// -----------------------------------------------------------------------
// Delete an entry by ordinal position
void CList::Delete_Entry_By_Ordinal(long Entry_Number, bool Free_Datas) {
	this->Delete_Entry(this->Get_Entry_By_Ordinal(Entry_Number),Free_Datas);
}

// -----------------------------------------------------------------------
// Delete an entry
void CList::Delete_Entry(CList_Entry *Entry_To_Delete, bool Free_Datas) {
	CList_Entry *Previous_Entry;
	CList_Entry *Next_Entry;

	if(!this->First_Entry) {
		this->Nbr_Entries = 0;
		this->Last_Entry = 0;
		this->First_Entry = 0;
		return;
	}
	// First will be next
	if(this->First_Entry == Entry_To_Delete) this->First_Entry = Entry_To_Delete->Next;
	// Last will be previous
	if(this->Last_Entry == Entry_To_Delete) this->Last_Entry = Entry_To_Delete->Previous;
	// Get next/previous entries
	Previous_Entry = Entry_To_Delete->Previous;
	Next_Entry = Entry_To_Delete->Next;
	// Free it now
	if(Free_Datas == true) if(Entry_To_Delete->u.Long) free((void *) Entry_To_Delete->u.Long);
	free(Entry_To_Delete);
	// Update previous/next entries
	if(Next_Entry) Next_Entry->Previous = Previous_Entry;
	if(Previous_Entry) Previous_Entry->Next = Next_Entry;
	if(this->Nbr_Entries) this->Nbr_Entries--;
	return;
}

// -----------------------------------------------------------------------
// Insert a new entry before a specified one
CList_Entry *CList::Insert_Entry_Before(CList_Entry *Entry_To_Insert_Before, long Datas) {
	CList_Entry *New_Entry;
	CList_Entry *Prev_Entry;

	if(!Entry_To_Insert_Before) return(this->Add_Entry((long) Datas));
	New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	if(!this->First_Entry) this->First_Entry = New_Entry;
	if(!this->Last_Entry) this->Last_Entry = New_Entry;
	// Set headers
	New_Entry->Next = Entry_To_Insert_Before;
	// This one is the new next one in previous entry
	Prev_Entry = Entry_To_Insert_Before->Previous;
	New_Entry->Previous = Prev_Entry;
	// Don't correct previous entry if this one is the first of the list
	if(Prev_Entry) Prev_Entry->Next = New_Entry;
	// This one is the new previous one in next entry
	Entry_To_Insert_Before->Previous = New_Entry;
	// Set datas
	New_Entry->u.Long = Datas;
	// Increment number of entries
	this->Nbr_Entries++;
	return(New_Entry);
}

// -----------------------------------------------------------------------
// Insert a new entry after a specified one
CList_Entry *CList::Insert_Entry_After(CList_Entry *Entry_To_Insert_After, long Datas) {
	CList_Entry *New_Entry;
	CList_Entry *Next_Entry;

	if(!Entry_To_Insert_After) return(this->Add_Entry((long) Datas));
	New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	if(!this->First_Entry) this->First_Entry = New_Entry;
	if(!this->Last_Entry) this->Last_Entry = New_Entry;
	// Set new previous
	New_Entry->Previous = Entry_To_Insert_After;
	// Check if it was the last created one
	if(this->Last_Entry == Entry_To_Insert_After) this->Last_Entry = New_Entry;
	// Take latest next
	Next_Entry = Entry_To_Insert_After->Next;
	// Store it as new/old next
	New_Entry->Next = Next_Entry;
	if(Next_Entry) Next_Entry->Previous = New_Entry;
	// This one is the new next one
	Entry_To_Insert_After->Next = New_Entry;
	// Set datas
	New_Entry->u.Long = Datas;
	// Increment number of entries
	this->Nbr_Entries++;
	return(New_Entry);
}

// -----------------------------------------------------------------------
// Search datas in the list
CList_Entry *CList::Search_Entry(long Entry_Datas) {
	CList_Entry *Entry_To_Get;

	// Take first entry in the list
	Entry_To_Get = this->First_Entry;
	// Take concerned entry
	while(Entry_To_Get) {
		if(Entry_Datas == Entry_To_Get->u.Long) break;
		Entry_To_Get = Entry_To_Get->Next;
	}
	return(Entry_To_Get);
}

// -----------------------------------------------------------------------
// Search a string in the list
CList_Entry *CList::Search_Entry(char *Entry_Datas, long Case_Sensitive) {
	CList_Entry *Entry_To_Get;

	// Take first entry in the list
	Entry_To_Get = this->First_Entry;
	// Take concerned entry
	if(Case_Sensitive) {
		while(Entry_To_Get) {
			if(strcmp(Entry_Datas, Entry_To_Get->u.Char) == 0) break;
			Entry_To_Get = Entry_To_Get->Next;
		}
	} else {
		while(Entry_To_Get) {
			if(strcmpi(Entry_Datas, Entry_To_Get->u.Char) == 0) break;
			Entry_To_Get = Entry_To_Get->Next;
		}
	}
	return(Entry_To_Get);
}
