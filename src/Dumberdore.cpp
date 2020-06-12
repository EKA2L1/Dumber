/*
 ============================================================================
 Name		: Dumberdore.cpp
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "DumberdoreApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CDumberdoreApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

