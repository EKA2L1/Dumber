/*
 ============================================================================
 Name		: DumberdoreDocument.h
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __DUMBERDOREDOCUMENT_h__
#define __DUMBERDOREDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CDumberdoreAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CDumberdoreDocument application class.
 * An instance of class CDumberdoreDocument is the Document part of the
 * AVKON application framework for the Dumberdore example application.
 */
class CDumberdoreDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CDumberdoreDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CDumberdoreDocument.
	 */
	static CDumberdoreDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CDumberdoreDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CDumberdoreDocument.
	 */
	static CDumberdoreDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CDumberdoreDocument
	 * Virtual Destructor.
	 */
	virtual ~CDumberdoreDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CDumberdoreAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CDumberdoreDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CDumberdoreDocument(CEikApplication& aApp);

	};

#endif // __DUMBERDOREDOCUMENT_h__
// End of File
