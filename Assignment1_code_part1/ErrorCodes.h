#ifndef __ErrorCodes_h
#define __ErrorCodes_h

/*
 * Return and error codes for OS functions.
 *
 * JL, 22-12-2010
 */

#define E_SUCCESS   0   /* return code: success                             */

#define E_BOUNDS    1   /* return code: bound error                         */
#define E_BUSY      2   /* return code: task busy                           */
#define E_NOTASK    3   /* return code: no such task                        */
#define E_BUF       4   /* return code: buffer over- or underflow           */
#define E_WRONGPAR  5   /* return code: parameter error                     */

#define E_SEMFAIL   6   /* return code: failing Semaphore operation         */

#define E_NOMEM     7   /* return code: out of memory                       */
#endif
