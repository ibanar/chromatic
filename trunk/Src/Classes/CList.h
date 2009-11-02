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
// CList.h: interface for the CList class
// -----------------------------------------------------------------------

#ifndef _CLIST_H_
#define _CLIST_H_

// -----------------------------------------------------------------------
// Includes
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------
// Class definition
template <typename T> class CList
{
    public:

        struct CList_Entry
        {
	        struct CList_Entry *Next;
	        struct CList_Entry *Previous;
            T Content;
        };

	    // Default Constructor/Destructor
        CList()
        {
            Free_Content = 0;
	        Nbr_Entries = 0;
	        Last_Entry = 0;
	        First_Entry = 0;
        }

        ~CList()
        {
	        this->Erase();
        }

        void Erase()
        {
	        CList_Entry *Entry_To_Free;

	        // Take first entry in the list
	        Entry_To_Free = this->First_Entry;

	        while(Entry_To_Free)
	        {
		        First_Entry = Entry_To_Free->Next;
		        Del(Entry_To_Free);
		        Entry_To_Free = First_Entry;
	        }
        }

        int Amount(void)
        {
	        return(Nbr_Entries);
        }

        CList_Entry *Add(T Datas, int Size)
        {
	        CList_Entry *New_Entry = Create_Entry(Size);
	        Set(New_Entry, Datas, Size);
	        return(New_Entry);
        }

        template<typename T> CList_Entry *Add(T Datas)
        {
	        CList_Entry *New_Entry = Create_Entry(0);
	        Set(New_Entry, Datas);
	        return(New_Entry);
        }

        template< > CList_Entry *Add<char *>(char *Datas)
        {
            int Size = strlen(Datas) + 1;
	        CList_Entry *New_Entry = Create_Entry(Size);
	        Set(New_Entry, Datas, Size);
	        return(New_Entry);
        }

        void MAdd(int Number, T Datas)
        {
	        while(Number)
	        {
		        Add(Datas);
		        Number--;
	        }
        }

        CList_Entry *Set(int Entry_Number, T Datas)
        {
	        CList_Entry *Entry_To_Set = Get(Entry_Number);
	        Set(Entry_To_Set, Datas);
	        return(Entry_To_Set);
        }

        template<typename T> CList_Entry *Set(CList_Entry *Entry_To_Set, T Datas)
        {
            Entry_To_Set->Content = (T) Datas;
            return(Entry_To_Set);
        }

        template< > CList_Entry *Set<char *>(CList_Entry *Entry_To_Set, char *Datas)
        {
            return(Set(Entry_To_Set, Datas, strlen(Datas) + 1));
        }

        CList_Entry *Set(int Entry_Number, T Datas, int Size)
        {
	        CList_Entry *Entry_To_Set = Get(Entry_Number);
	        Set(Entry_To_Set, Datas, Size);
	        return(Entry_To_Set);
        }

        CList_Entry *Set(CList_Entry *Entry_To_Set, T Datas, int Size)
        {
            free((void *) Entry_To_Set->Content);
            Entry_To_Set->Content = (T) calloc(Size, 1);
            memcpy((void *) Entry_To_Set->Content, (void *) Datas, Size);
            Free_Content = 1;
            return(Entry_To_Set);
        }

        CList_Entry *Get(CList_Entry *Entry_To_Get)
        {
	        CList_Entry *Current_Entry;

        	Current_Entry = this->First_Entry;
	        while(Current_Entry != Entry_To_Get)
	        {
		        Current_Entry = Current_Entry->Next;
	        }
	        return(Current_Entry);
        }

        CList_Entry *Get(int Entry_Number)
        {
	        CList_Entry *Entry_To_Get;

	        Entry_To_Get = First_Entry;
	        while(Entry_Number)
	        {
		        Entry_To_Get = Entry_To_Get->Next;
		        Entry_Number--;
	        }
	        return(Entry_To_Get);
        }

        void Del(CList_Entry *Entry_To_Delete)
        {
	        CList_Entry *Previous_Entry;
	        CList_Entry *Next_Entry;

	        if(!First_Entry)
	        {
		        Nbr_Entries = 0;
		        Last_Entry = 0;
		        First_Entry = 0;
		        return;
	        }
	        // First will be next
	        if(First_Entry == Entry_To_Delete) First_Entry = Entry_To_Delete->Next;
	        // Last will be previous
	        if(Last_Entry == Entry_To_Delete) Last_Entry = Entry_To_Delete->Previous;
	        // Get next/previous entries
	        Previous_Entry = Entry_To_Delete->Previous;
	        Next_Entry = Entry_To_Delete->Next;
	        if(Free_Content) free((void *) Entry_To_Delete->Content);
            free(Entry_To_Delete);
	        // Update previous/next entries
	        if(Next_Entry) Next_Entry->Previous = Previous_Entry;
	        if(Previous_Entry) Previous_Entry->Next = Next_Entry;
	        if(this->Nbr_Entries) this->Nbr_Entries--;
        }

        void Del(int Entry_Number)
        {
            Del((CList_Entry *) Get(Entry_Number));
        }

        CList_Entry *Search_Entry(T &Datas, int Start, int Length)
        {
	        CList_Entry *Entry_To_Get;

	        Entry_To_Get = First_Entry;
	        while(Entry_To_Get)
	        {
                if(memcmp(((uint8 *) &Datas) + Start,
                          ((uint8 *) &Entry_To_Get->Content) + Start,
                          Length) == 0)
                {
                    return(Entry_To_Get);
                }
                Entry_To_Get = Entry_To_Get->Next;
	        }
	        return(NULL);
        }

    private:

	    int Nbr_Entries;
        int Free_Content;
	    struct CList_Entry *Last_Entry;
	    struct CList_Entry *First_Entry;

        CList_Entry *Create_Entry(int Size)
        {
	        CList_Entry *New_Entry;
	        CList_Entry *Old_Entry;

	        New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	        if(!First_Entry) First_Entry = New_Entry;
	        if(Last_Entry)
	        {
		        // Update previous entry
		        Old_Entry = Last_Entry;
		        Old_Entry->Next = New_Entry;
	        }
	        // Last entry
	        New_Entry->Next = 0;
	        New_Entry->Previous = this->Last_Entry;
	        if(Size)
            {
	            New_Entry->Content = (T) calloc(Size, 1);
            }
            else
            {
                New_Entry->Content = 0;
            }
	        // Store newly created entry
	        Last_Entry = New_Entry;
	        // Increment number of entries
	        Nbr_Entries++;
	        return(New_Entry);
        }

        CList_Entry *Insert_Entry_Before(CList_Entry *Entry_To_Insert_Before)
        {
	        CList_Entry *New_Entry;
	        CList_Entry *Prev_Entry;

	        New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	        if(!First_Entry) First_Entry = New_Entry;
	        if(!Last_Entry) Last_Entry = New_Entry;
	        // Set headers
	        New_Entry->Next = Entry_To_Insert_Before;
	        // This one is the new next one in previous entry
	        Prev_Entry = Entry_To_Insert_Before->Previous;
	        New_Entry->Previous = Prev_Entry;
	        // Don't correct previous entry if this one is the first of the list
	        if(Prev_Entry) Prev_Entry->Next = New_Entry;
	        // This one is the new previous one in next entry
	        Entry_To_Insert_Before->Previous = New_Entry;
	        // Increment number of entries
	        Nbr_Entries++;
	        return(New_Entry);
        }

        CList_Entry *Insert_Entry_Before(CList_Entry *Entry_To_Insert_Before, T &Datas)
        {
	        if(!Entry_To_Insert_Before) return(Add(Datas));
	        CList_Entry *New_Entry = Insert_Entry_Before(Entry_To_Insert_Before);
	        Set(New_Entry, Datas);
	        return(New_Entry);
        }

        CList_Entry *Insert_Entry_After(CList_Entry *Entry_To_Insert_After)
        {
	        CList_Entry *New_Entry;
	        CList_Entry *Next_Entry;

	        New_Entry = (CList_Entry *) calloc(sizeof(CList_Entry), 1);
	        if(!First_Entry) First_Entry = New_Entry;
	        if(!Last_Entry) Last_Entry = New_Entry;
	        // Set new previous
	        New_Entry->Previous = Entry_To_Insert_After;
	        // Check if it was the last created one
	        if(Last_Entry == Entry_To_Insert_After) Last_Entry = New_Entry;
	        // Take latest next
	        Next_Entry = Entry_To_Insert_After->Next;
	        // Store it as new/old next
	        New_Entry->Next = Next_Entry;
	        if(Next_Entry) Next_Entry->Previous = New_Entry;
	        // This one is the new next one
	        Entry_To_Insert_After->Next = New_Entry;
	        // Increment number of entries
	        Nbr_Entries++;
	        return(New_Entry);
        }

        CList_Entry *Insert_Entry_After(CList_Entry *Entry_To_Insert_After, T &Datas)
        {
	        if(!Entry_To_Insert_After) return(Add(Datas));
	        CList_Entry *New_Entry = Insert_Entry_After(Entry_To_Insert_After);
	        Set(New_Entry, Datas);
	        return(New_Entry);
        }
};

#endif

/*// -----------------------------------------------------------------------
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
// CList.h: interface for the CList class
// -----------------------------------------------------------------------

#ifndef _CLIST_H_
#define _CLIST_H_

// -----------------------------------------------------------------------
// Includes
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------
// Structures
struct CList_Entry {
	struct CList_Entry *Next;
	struct CList_Entry *Previous;
	union {
		char *Char;
		long Long;
		float Float;
		double Double;
	} u;
};

// -----------------------------------------------------------------------
// Class definition
class CList {
public:
	// Default Constructor/Destructor
	CList();
	virtual ~CList();
	// Methods
	struct CList_Entry *Add(long Datas);
	struct CList_Entry *Add(int Datas)
    {
		return(this->Add_Entry((long) Datas));
	}
	struct CList_Entry *Add_Entry(char *Datas);
	struct CList_Entry *Add_Entry(float Datas);
	struct CList_Entry *Add_Entry(double Datas);
	void Add_Entries(long Number, long Datas);
	void Add_Entries(long Number, int Datas);
	void Add_Entries(long Number, char *Datas);
	void Add_Entries(long Number, float Datas);
	void Add_Entries(long Number, double Datas);
	long Get_Number_Of_Entries(void);
	struct CList_Entry *Is_Entry(long Datas);
	void Delete_Entry(struct CList_Entry *, bool Free_Datas);
	void Delete_Entry_By_Ordinal(long Entry_Number, bool Free_Datas);
	struct CList_Entry *Get_Entry_By_Ordinal(long Entry_Number);
	long Get_Entry_By_Ordinal_Long(long Entry_Number);
	char *Get_Entry_By_Ordinal_Char(long Entry_Number);
	float Get_Entry_By_Ordinal_Float(long Entry_Number);
	double Get_Entry_By_Ordinal_Double(long Entry_Number);
	long *Get_Entry_By_Ordinal_Addr(long Entry_Number);
	struct CList_Entry *Set_Entry_By_Ordinal(long Entry_Number, long Datas, bool Free_Datas);
	struct CList_Entry *Set_Entry_By_Ordinal(long Entry_Number, int Datas) {
		return(this->Set_Entry_By_Ordinal(Entry_Number, (long) Datas, false));
	}
	struct CList_Entry *Set_Entry_By_Ordinal(long Entry_Number, char *Datas);
	struct CList_Entry *Set_Entry_By_Ordinal(long Entry_Number, float Datas);
	struct CList_Entry *Set_Entry_By_Ordinal(long Entry_Number, double Datas);
	long Get_Entry_By_Address(CList_Entry *);
	struct CList_Entry *Set_Entry_By_Address(struct CList_Entry *Entry_To_Set, long Datas, bool Free_Datas);
	struct CList_Entry *Set_Entry_By_Address(struct CList_Entry *Entry_To_Set, int Datas) {
		return(this->Set_Entry_By_Address(Entry_To_Set, (long) Datas, false));
	}
	struct CList_Entry *Set_Entry_By_Address(struct CList_Entry *Entry_To_Set, char *Datas);
	struct CList_Entry *Set_Entry_By_Address(struct CList_Entry *Entry_To_Set, float Datas);
	struct CList_Entry *Set_Entry_By_Address(struct CList_Entry *Entry_To_Set, double Datas);
	struct CList_Entry *Insert_Entry_Before(struct CList_Entry *Entry_To_Set, long Datas);
	struct CList_Entry *Insert_Entry_After(struct CList_Entry *Entry_To_Set, long Datas);
	void Clear_All(bool);
	struct CList_Entry *Search_Entry(long Entry_Datas);
	struct CList_Entry *Search_Entry(char *Entry_Datas, long Case_Sensitive);
private:
	long Nbr_Entries;
	struct CList_Entry *Last_Entry;
	struct CList_Entry *First_Entry;
	CList_Entry *Create_Entry(void);
};

#endif
*/
