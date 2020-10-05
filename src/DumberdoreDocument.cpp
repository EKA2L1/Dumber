/*
 ============================================================================
 Name		: DumberdoreDocument.cpp
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : CDumberdoreDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "DumberdoreAppUi.h"
#include "DumberdoreDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDumberdoreDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDumberdoreDocument* CDumberdoreDocument::NewL(CEikApplication& aApp)
	{
	CDumberdoreDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CDumberdoreDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDumberdoreDocument* CDumberdoreDocument::NewLC(CEikApplication& aApp)
	{
	CDumberdoreDocument* self = new (ELeave) CDumberdoreDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CDumberdoreDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDumberdoreDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CDumberdoreDocument::CDumberdoreDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CDumberdoreDocument::CDumberdoreDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CDumberdoreDocument::~CDumberdoreDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CDumberdoreDocument::~CDumberdoreDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CDumberdoreDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CDumberdoreDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CDumberdoreAppUi;
	}

// End of File
