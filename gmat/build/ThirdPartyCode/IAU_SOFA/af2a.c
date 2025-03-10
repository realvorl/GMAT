#include "sofam.h"
#include <stdlib.h>

int iauAf2a(int s, int ideg, int iamin, double asec, double *rad)
/*
**  - - - - - - - -
**   i a u A f 2 a
**  - - - - - - - -
**
**  Convert degrees, arcminutes, arcseconds to radians.
**
**  This function is part of the International Astronomical Union's
**  SOFA (Standards of Fundamental Astronomy) software collection.
**
**  Status:  support function.
**
**  Given:
**     s         int     sign:  '-' = negative, otherwise positive
**     ideg      int     degrees
**     iamin     int     arcminutes
**     asec      double  arcseconds
**
**  Returned:
**     rad       double  angle in radians
**
**  Returned (function value):
**               int     status:  0 = OK
**                                1 = ideg outside range 0-359
**                                2 = iamin outside range 0-59
**                                3 = asec outside range 0-59.999...
**
**  Notes:
**
**  1)  The result is computed even if any of the range checks fail.
**
**  2)  Negative ideg, iamin and/or asec produce a warning status, but
**      the absolute value is used in the conversion.
**
**  This revision:  2010 August 27
**
**  SOFA release 2010-12-01
**
**  Copyright (C) 2010 IAU SOFA Board.  See notes at end.
*/
{

/* Compute the interval. */
   *rad  = ( s == '-' ? -1.0 : 1.0 ) *
           ( 60.0 * ( 60.0 * ( (double) abs(ideg) ) +
                             ( (double) abs(iamin) ) ) +
                                        fabs(asec) ) * DAS2R;

/* Validate arguments and return status. */
   if ( ideg < 0 || ideg > 359 ) return 1;
   if ( iamin < 0 || iamin > 59 ) return 2;
   if ( asec < 0.0 || asec >= 60.0 ) return 3;
   return 0;

/*----------------------------------------------------------------------
**
**  Copyright (C) 2010
**  Standards Of Fundamental Astronomy Board
**  of the International Astronomical Union.
**
**  =====================
**  SOFA Software License
**  =====================
**
**  NOTICE TO USER:
**
**  BY USING THIS SOFTWARE YOU ACCEPT THE FOLLOWING TERMS AND CONDITIONS
**  WHICH APPLY TO ITS USE.
**
**  1. The Software is owned by the IAU SOFA Board ("SOFA").
**
**  2. Permission is granted to anyone to use the SOFA software for any
**     purpose, including commercial applications, free of charge and
**     without payment of royalties, subject to the conditions and
**     restrictions listed below.
**
**  3. You (the user) may copy and distribute SOFA source code to others,
**     and use and adapt its code and algorithms in your own software,
**     on a world-wide, royalty-free basis.  That portion of your
**     distribution that does not consist of intact and unchanged copies
**     of SOFA source code files is a "derived work" that must comply
**     with the following requirements:
**
**     a) Your work shall be marked or carry a statement that it
**        (i) uses routines and computations derived by you from
**        software provided by SOFA under license to you; and
**        (ii) does not itself constitute software provided by and/or
**        endorsed by SOFA.
**
**     b) The source code of your derived work must contain descriptions
**        of how the derived work is based upon, contains and/or differs
**        from the original SOFA software.
**
**     c) The name(s) of all routine(s) in your derived work shall not
**        include the prefix "iau".
**
**     d) The origin of the SOFA components of your derived work must
**        not be misrepresented;  you must not claim that you wrote the
**        original software, nor file a patent application for SOFA
**        software or algorithms embedded in the SOFA software.
**
**     e) These requirements must be reproduced intact in any source
**        distribution and shall apply to anyone to whom you have
**        granted a further right to modify the source code of your
**        derived work.
**
**     Note that, as originally distributed, the SOFA software is
**     intended to be a definitive implementation of the IAU standards,
**     and consequently third-party modifications are discouraged.  All
**     variations, no matter how minor, must be explicitly marked as
**     such, as explained above.
**
**  4. In any published work or commercial products which includes
**     results achieved by using the SOFA software, you shall
**     acknowledge that the SOFA software was used in obtaining those
**     results.
**
**  5. You shall not cause the SOFA software to be brought into
**     disrepute, either by misuse, or use for inappropriate tasks, or
**     by inappropriate modification.
**
**  6. The SOFA software is provided "as is" and SOFA makes no warranty
**     as to its use or performance.   SOFA does not and cannot warrant
**     the performance or results which the user may obtain by using the
**     SOFA software.  SOFA makes no warranties, express or implied, as
**     to non-infringement of third party rights, merchantability, or
**     fitness for any particular purpose.  In no event will SOFA be
**     liable to the user for any consequential, incidental, or special
**     damages, including any lost profits or lost savings, even if a
**     SOFA representative has been advised of such damages, or for any
**     claim by any third party.
**
**  7. The provision of any version of the SOFA software under the terms
**     and conditions specified herein does not imply that future
**     versions will also be made available under the same terms and
**     conditions.
**
**  Correspondence concerning SOFA software should be addressed as
**  follows:
**
**      By email:  sofa@ukho.gov.uk
**      By post:   IAU SOFA Center
**                 HM Nautical Almanac Office
**                 UK Hydrographic Office
**                 Admiralty Way, Taunton
**                 Somerset, TA1 2DN
**                 United Kingdom
**
**--------------------------------------------------------------------*/
}
