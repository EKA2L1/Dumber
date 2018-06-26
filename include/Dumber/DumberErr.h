#ifndef DUMBERERR_H_
#define DUMBERERR_H_

#include <e32cmn.h>

/*! \brief When an unknown open mode is given to ConstructL.
 * 
 */
const TInt KErrUnknownMode = -102;

/*! \brief Leave when an invalid request is sended.
 */
const TInt KErrInvalidRequest = -103;

/* PANIC */
const TInt KDumberOutOfRange = 0;

#endif
