#include "sofam.h"

int iauTdbtt(double tdb1, double tdb2, double dtr,
             double *tt1, double *tt2 )
/*
**  - - - - - - - - -
**   i a u T d b t t
**  - - - - - - - - -
**
**  Time scale transformation:  Barycentric Dynamical Time, TDB, to
**  Terrestrial Time, TT.
**
**  This function is part of the International Astronomical Union's
**  SOFA (Standards of Fundamental Astronomy) software collection.
**
**  Status:  canonical.
**
**  Given:
**     dtb1,tdb2  double    TDB as a 2-part Julian Date
**     dtr        double    TDB-TT in seconds
**
**  Returned:
**     tt1,tt2    double    TT as a 2-part Julian Date
**
**  Returned (function value):
**                int       status:  0 = OK
**
**  Notes:
**
**  1  tdb1+tdb2 is Julian Date, apportioned in any convenient way
**     between the two arguments, for example where tdb1 is the Julian
**     Day Number and tdb2 is the fraction of a day.  The returned
**     tt1,tt2 follow suit.
**
**  2  The argument dtr represents the quasi-periodic component of the
**     GR transformation between TT and TCB.  It is dependent upon the
**     adopted solar-system ephemeris, and can be obtained by numerical
**     integration, by interrogating a precomputed time ephemeris or by
**     evaluating a model such as that implemented in the SOFA function
**     iauDtdb.   The quantity is dominated by an annual term of 1.7 ms
**     amplitude.
**
**  3  TDB is essentially the same as Teph, the time argument for the
**     JPL solar system ephemerides.
**
**  References:
**
**     McCarthy, D. D., Petit, G. (eds.), IERS Conventions (2003),
**     IERS Technical Note No. 32, BKG (2004)
**
**     IAU 2006 Resolution 3
**
**  This revision:  2010 May 13
**
**  SOFA release 2010-12-01
**
**  Copyright (C) 2010 IAU SOFA Board.  See notes at end.
**
*/
{
   double dtrd;


/* Result, safeguarding precision. */
   dtrd = dtr / DAYSEC;
   if ( tdb1 > tdb2 ) {
      *tt1 = tdb1;
      *tt2 = tdb2 - dtrd;
   } else {
      *tt1 = tdb1 - dtrd;
      *tt2 = tdb2;
   }

/* Status (always OK). */
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
