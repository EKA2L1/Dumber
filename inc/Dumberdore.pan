/*
 ============================================================================
 Name		: Dumberdore.pan
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __DUMBERDORE_PAN__
#define __DUMBERDORE_PAN__

/** Dumberdore application panic codes */
enum TDumberdorePanics
	{
	EDumberdoreUi = 1
	// add further panics here
	};

inline void Panic(TDumberdorePanics aReason)
	{
	_LIT(applicationName, "Dumberdore");
	User::Panic(applicationName, aReason);
	}

#endif // __DUMBERDORE_PAN__
