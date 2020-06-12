/*
 ============================================================================
 Name		: DumberdoreApplication.h
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __DUMBERDOREAPPLICATION_H__
#define __DUMBERDOREAPPLICATION_H__

// INCLUDES
#include "Dumberdore.hrh"
#include <aknapp.h>

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidDumberdoreApp = {_UID3};

// CLASS DECLARATION

/**
 * CDumberdoreApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CDumberdoreApplication is the application part of the
 * AVKON application framework for the Dumberdore example application.
 */
class CDumberdoreApplication : public CAknApplication {
public:
  // Functions from base classes

  /**
   * From CApaApplication, AppDllUid.
   * @return Application's UID (KUidDumberdoreApp).
   */
  TUid AppDllUid() const;

protected:
  // Functions from base classes

  /**
   * From CApaApplication, CreateDocumentL.
   * Creates CDumberdoreDocument document object. The returned
   * pointer in not owned by the CDumberdoreApplication object.
   * @return A pointer to the created document object.
   */
  CApaDocument *CreateDocumentL();
};

#endif // __DUMBERDOREAPPLICATION_H__
// End of File
