/*
 ============================================================================
 Name		: DumberdoreAppView.cpp
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include "DumberdoreAppView.h"
#include <coemain.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDumberdoreAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDumberdoreAppView *CDumberdoreAppView::NewL(const TRect &aRect) {
  CDumberdoreAppView *self = CDumberdoreAppView::NewLC(aRect);
  CleanupStack::Pop(self);
  return self;
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDumberdoreAppView *CDumberdoreAppView::NewLC(const TRect &aRect) {
  CDumberdoreAppView *self = new (ELeave) CDumberdoreAppView;
  CleanupStack::PushL(self);
  self->ConstructL(aRect);
  return self;
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppView::ConstructL(const TRect &aRect) {
  // Create a window for this application view
  CreateWindowL();

  // Set the windows size
  SetRect(aRect);

  // Activate the window, which makes it ready to be drawn
  ActivateL();
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::CDumberdoreAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CDumberdoreAppView::CDumberdoreAppView() {
  // No implementation required
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::~CDumberdoreAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CDumberdoreAppView::~CDumberdoreAppView() {
  // No implementation required
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppView::Draw(const TRect & /*aRect*/) const {
  // Get the standard graphics context
  CWindowGc &gc = SystemGc();

  // Gets the control's extent
  TRect drawRect(Rect());

  // Clears the screen
  gc.Clear(drawRect);
}

// -----------------------------------------------------------------------------
// CDumberdoreAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppView::SizeChanged() { DrawNow(); }

// -----------------------------------------------------------------------------
// CDumberdoreAppView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs,
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppView::HandlePointerEventL(
    const TPointerEvent &aPointerEvent) {

  // Call base class HandlePointerEventL()
  CCoeControl::HandlePointerEventL(aPointerEvent);
}

// End of File
