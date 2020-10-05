/*
 ============================================================================
 Name		: DumberdoreApplication.cpp
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "Dumberdore.hrh"
#include "DumberdoreDocument.h"
#include "DumberdoreApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDumberdoreApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CDumberdoreApplication::CreateDocumentL()
	{
	// Create an Dumberdore document, and return a pointer to it
	return CDumberdoreDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CDumberdoreApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CDumberdoreApplication::AppDllUid() const
	{
	// Return the UID for the Dumberdore application
	return KUidDumberdoreApp;
	}

// End of File
