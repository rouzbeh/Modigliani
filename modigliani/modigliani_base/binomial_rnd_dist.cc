/**
 * @file binomial_rnd_dist.cc
 * Binomial_rnd_dist class implementation
 * @author Ahmed Aldo Faisal &copy; created 19.6.2001
 * NetTrader - visualisation, scientific and financial analysis and simulation system
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "binomial_rnd_dist.h"

using namespace modigliani_base;

#define M_LOGGAMMATABLEMAX 10000001

/*vector<Real> M_make_vector_func(Size_t dim) {
 vector<Real> tmpVec;
 tmpVec.resize(dim);
 return tmpVec;
 }*/

Binomial_rnd_dist::Initializer Binomial_rnd_dist::static_table;

Binomial_rnd_dist::Initializer::Initializer() {
  logGammaTable = std::vector<Real>(M_LOGGAMMATABLEMAX);
  debug_print(
      "%s\n",
      "Binomial_rnd_dist::Binomial_rnd_dist - Talk : This may take a while, populating LogGamma tables");
  for (Size ll = 0; ll < M_LOGGAMMATABLEMAX; ll++) {
    logGammaTable[ll] = MLogGammaFunction(Real(ll));
  }
}

//bool Binomial_rnd_dist::init = false;

/* ***      CONSTRUCTORS    ***/
/** Create a Binomial_rnd_dist */
Binomial_rnd_dist::Binomial_rnd_dist(Real pp, Size num)
    : Rnd_dist() {
  M_ASSERT((pp >= 0) && (pp <= 1));
  p = pp;
  M_ASSERT(num > 0);
  n = num;
}

/* ***      COPY AND ASSIGNMEM    ***/
Binomial_rnd_dist::Binomial_rnd_dist(const Binomial_rnd_dist& original)
    : Rnd_dist()  {
  p = original.p;
  n = original.n;
}

const Binomial_rnd_dist&
Binomial_rnd_dist::operator=(const Binomial_rnd_dist & right) {
  if (this == &right) return (*this);  // Gracefully handle self assignment
// add assignment code here
  return (*this);
}

/* ***      DESTRUCTOR        ***/
Binomial_rnd_dist::~Binomial_rnd_dist() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @return     Random value
 */
inline Real Binomial_rnd_dist::RndVal() const {
  /*    This is algorithm BTPE from:
   Kachitvichyanukul, V. and Schmeiser, B. W.
   Binomial Random Variate Generation.
   Communications of the ACM, 31, 2
   (February, 1988) 216.
   **********************************************************************
   SUBROUTINE BTPEC(N,P,ISEED,JX)
   BINOMIAL RANDOM VARIATE GENERATOR
   MEAN .LT. 30 -- INVERSE CDF
   MEAN .GE. 30 -- ALGORITHM BTPE:  ACCEPTANCE-REJECTION VIA
   FOUR REGION COMPOSITION.  THE FOUR REGIONS ARE A TRIANGLE
   (SYMMETRIC IN THE CEMER), A PAIR OF PARALLELOGRAMS (ABOVE
   THE TRIANGLE), AND EXPONEMIAL LEFT AND RIGHT TAILS.
   BTPE REFERS TO BINOMIAL-TRIANGLE-PARALLELOGRAM-EXPONEMIAL.
   BTPEC REFERS TO BTPE AND "COMBINED."  THUS BTPE IS THE
   RESEARCH AND BTPEC IS THE IMPLEMEMATION OF A COMPLETE
   USABLE ALGORITHM.
   REFERENCE:  VORATAS KACHITVICHYANUKUL AND BRUCE SCHMEISER,
   "BINOMIAL RANDOM VARIATE GENERATION,"
   COMMUNICATIONS OF THE ACM, FORTHCOMING
   WRITTEN:  SEPTEMBER 1980.
   LAST REVISED:  MAY 1985, JULY 1987
   REQUIRED SUBPROGRAM:  RAND() -- A UNIFORM (0,1) RANDOM NUMBER
   GENERATOR
   ARGUMEMS
   N : NUMBER OF BERNOULLI TRIALS            (INPUT)
   P : PABILITY OF SUCCESS IN EACH TRIAL (INPUT)
   ISEED:  RANDOM NUMBER SEED                (INPUT AND OUTPUT)
   JX:  RANDOMLY GENERATED OBSERVATION       (OUTPUT)
   VARIABLES
   PSAVE: VALUE OF P FROM THE LAST CALL TO BTPEC
   NSAVE: VALUE OF N FROM THE LAST CALL TO BTPEC
   XNP:  VALUE OF THE MEAN FROM THE LAST CALL TO BTPEC
   P: PABILITY USED IN THE GENERATION PHASE OF BTPEC
   FFM: TEMPORARY VARIABLE EQUAL TO XNP + P
   M:  IMEGER VALUE OF THE CURREM MODE
   FM:  FLOATING POIM VALUE OF THE CURREM MODE
   XNPQ: TEMPORARY VARIABLE USED IN SETUP AND SQUEEZING STEPS
   P1:  AREA OF THE TRIANGLE
   C:  HEIGHT OF THE PARALLELOGRAMS
   XM:  CEMER OF THE TRIANGLE
   XL:  LEFT END OF THE TRIANGLE
   XR:  RIGHT END OF THE TRIANGLE
   AL:  TEMPORARY VARIABLE
   XLL:  RATE FOR THE LEFT EXPONEMIAL TAIL
   XLR:  RATE FOR THE RIGHT EXPONEMIAL TAIL
   P2:  AREA OF THE PARALLELOGRAMS
   P3:  AREA OF THE LEFT EXPONEMIAL TAIL
   P4:  AREA OF THE RIGHT EXPONEMIAL TAIL
   U:  A U(0,P4) RANDOM VARIATE USED FIRST TO SELECT ONE OF THE
   FOUR REGIONS AND THEN CONDITIONALLY TO GENERATE A VALUE
   FROM THE REGION
   V:  A U(0,1) RANDOM NUMBER USED TO GENERATE THE RANDOM VALUE
   (REGION 1) OR TRANSFORMED IMO THE VARIATE TO ACCEPT OR
   REJECT THE CANDIDATE VALUE
   IX:  IMEGER CANDIDATE VALUE
   X:  PRELIMINARY COMINUOUS CANDIDATE VALUE IN REGION 2 LOGIC
   AND A FLOATING POIM IX IN THE ACCEPT/REJECT LOGIC
   K:  ABSOLUTE VALUE OF (IX-M)
   F:  THE HEIGHT OF THE SCALED DENSITY FUNCTION USED IN THE
   ACCEPT/REJECT DECISION WHEN BOTH M AND IX ARE SMALL
   ALSO USED IN THE INVERSE TRANSFORMATION
   R: THE RATIO P/Q
   G: CONSTAM USED IN CALCULATION OF PABILITY
   MP:  MODE PLUS ONE, THE LOWER INDEX FOR EXPLICIT CALCULATION
   OF F WHEN IX IS GREATER THAN M
   IX1:  CANDIDATE VALUE PLUS ONE, THE LOWER INDEX FOR EXPLICIT
   CALCULATION OF F WHEN IX IS LESS THAN M
   I:  INDEX FOR EXPLICIT CALCULATION OF F FOR BTPE
   AMAXP: MAXIMUM ERROR OF THE LOGARITHM OF NORMAL BOUND
   YNORM: LOGARITHM OF NORMAL BOUND
   ALV:  NATURAL LOGARITHM OF THE ACCEPT/REJECT VARIATE V
   X1,F1,Z,W,Z2,X2,F2, AND W2 ARE TEMPORARY VARIABLES TO BE
   USED IN THE FINAL ACCEPT/REJECT TEST
   QN: PABILITY OF NO SUCCESS IN N TRIALS
   REMARK
   IX AND JX COULD LOGICALLY BE THE SAME VARIABLE, WHICH WOULD
   SAVE A MEMORY POSITION AND A LINE OF CODE.  HOWEVER, SOME
   COMPILERS (E.G.,CDC MNF) OPTIMIZE BETTER WHEN THE ARGUMEMS
   ARE NOT INVOLVED.
   ISEED NEEDS TO BE DOUBLE PRECISION IF THE IMSL ROUTINE
   GGUBFS IS USED TO GENERATE UNIFORM RANDOM NUMBER, OTHERWISE
   TYPE OF ISEED SHOULD BE DICTATED BY THE UNIFORM GENERATOR
   **********************************************************************
   *****DETERMINE APROPRIATE ALGORITHM AND WHETHER SETUP IS NECESSARY
   */

  static float psave = -1.0;
  static long nsave = -1;
  static long ignbin, i, ix, ix1, k, m, mp, T1;
  static float al, alv, amaxp, c, f, f1, f2, ffm, fm, g, pp, p1, p2, p3, p4, q,
      qn, r, u, v, w, w2, x, x1, x2, xl, xll, xlr, xm, xnp, xnpq, xr, ynorm, z,
      z2;

  if (p != psave) goto S10;
  if (n != nsave) goto S20;
  if (xnp < 30.0) goto S150;
  goto S30;
  S10:
  /*
   *****SETUP, PERFORM ONLY WHEN PARAMETERS CHANGE
   */
  psave = p;
  pp = M_Min(double(psave), 1.0 - psave);
  q = 1.0 - pp;
  S20: xnp = n * pp;
  nsave = n;
  if (xnp < 30.0) goto S140;
  ffm = xnp + pp;
  m = ffm;
  fm = m;
  xnpq = xnp * q;
  p1 = (long) (2.195 * sqrt(xnpq) - 4.6 * q) + 0.5;
  xm = fm + 0.5;
  xl = xm - p1;
  xr = xm + p1;
  c = 0.134 + 20.5 / (15.3 + fm);
  al = (ffm - xl) / (ffm - xl * pp);
  xll = al * (1.0 + 0.5 * al);
  al = (xr - ffm) / (xr * q);
  xlr = al * (1.0 + 0.5 * al);
  p2 = p1 * (1.0 + c + c);
  p3 = p2 + c / xll;
  p4 = p3 + c / xlr;
  S30:
  /*
   *****GENERATE VARIATE
   */
  u = uniformMT() * p4;
  v = uniformMT();
  /*
   TRIANGULAR REGION
   */
  if (u > p1) goto S40;
  ix = xm - p1 * v + u;
  goto S170;
  S40:
  /*
   PARALLELOGRAM REGION
   */
  if (u > p2) goto S50;
  x = xl + (u - p1) / c;
  v = v * c + 1.0 - M_Abs(xm - x) / p1;
  if (v > 1.0 || v <= 0.0) goto S30;
  ix = x;
  goto S70;
  S50:
  /*
   LEFT TAIL
   */
  if (u > p3) goto S60;
  ix = xl + log(v) / xll;
  if (ix < 0) goto S30;
  v *= ((u - p2) * xll);
  goto S70;
  S60:
  /*
   RIGHT TAIL
   */
  ix = xr - log(v) / xlr;
  if (ix > n) goto S30;
  v *= ((u - p3) * xlr);
  S70:
  /*
   *****DETERMINE APROPRIATE WAY TO PERFORM ACCEPT/REJECT TEST
   */
  k = M_Abs(ix - m);
  if (k > 20 && k < xnpq / 2 - 1) goto S130;
  /*
   EXPLICIT EVALUATION
   */
  f = 1.0;
  r = pp / q;
  g = (n + 1) * r;
  T1 = m - ix;
  if (T1 < 0) goto S80;
  else if (T1 == 0) goto S120;
  else goto S100;
  S80: mp = m + 1;
  for (i = mp; i <= ix; i++)
    f *= (g / i - r);
  goto S120;
  S100: ix1 = ix + 1;
  for (i = ix1; i <= m; i++)
    f /= (g / i - r);
  S120: if (v <= f) goto S170;
  goto S30;
  S130:
  /*
   SQUEEZING USING UPER AND LOWER BOUNDS ON ALOG(F(X))
   */
  amaxp = k / xnpq * ((k * (k / 3.0 + 0.625) + 0.1666666666666) / xnpq + 0.5);
  ynorm = -(k * k / (2.0 * xnpq));
  alv = log(v);
  if (alv < ynorm - amaxp) goto S170;
  if (alv > ynorm + amaxp) goto S30;
  /*
   STIRLING'S FORMULA TO MACHINE ACCURACY FOR
   THE FINAL ACCEPTANCE/REJECTION TEST
   */
  x1 = ix + 1.0;
  f1 = fm + 1.0;
  z = n + 1.0 - fm;
  w = n - ix + 1.0;
  z2 = z * z;
  x2 = x1 * x1;
  f2 = f1 * f1;
  w2 = w * w;
  if (alv
      <= xm * log(f1 / x1) + (n - m + 0.5) * log(z / w)
          + (ix - m) * log(w * pp / (x1 * q))
          + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / f2) / f2) / f2) / f2)
              / f1 / 166320.0
          + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / z2) / z2) / z2) / z2)
              / z / 166320.0
          + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / x2) / x2) / x2) / x2)
              / x1 / 166320.0
          + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / w2) / w2) / w2) / w2)
              / w / 166320.0) goto S170;
  goto S30;
  S140:
  /*
   INVERSE CDF LOGIC FOR MEAN LESS THAN 30
   */

  /* NOTE: ADDED (double) q SINCE SOME COMPILER COMPLAINS */
  qn = pow((double) q, (double) n);
  r = pp / q;
  g = r * (n + 1);
  S150: ix = 0;
  f = qn;
  u = uniformMT();
  S160: if (u < f) goto S170;
  if (ix > 110) goto S150;
  u -= f;
  ix += 1;
  f *= (g / ix - r);
  goto S160;
  S170: if (psave > 0.5) ix = n - ix;
  ignbin = ix;
  return (ignbin);

}

Real Binomial_rnd_dist::Binomial(float pp, int new_n) const {
  if (0 == new_n) return (0);

  int j;
  static int nold = (-1);
  float am, em, g, angle, new_p, bnl, sq, t, y;
  static float pold = (-1.0);
  static float pc, plog, pclog, en, oldg;

  new_p = (pp <= 0.5 ? pp : 1.0 - pp);

  am = new_n * new_p;
  if (new_n < 25) {
    bnl = 0.0;
    for (j = 1; j <= new_n; j++)
      if (uniformMT() < new_p) ++bnl;
  } else if (am < 1.0) {
    g = exp(-am);
    t = 1.0;
    for (j = 0; j <= new_n; j++) {
      t *= uniformMT();
      if (t < g) break;
    }
    bnl = (j <= new_n ? j : new_n);
  } else {
    if (new_n != nold) {
      en = new_n;
      oldg = LogGamma(en + 1.0);
      nold = new_n;
    }
    if (new_p != pold) {
      pc = 1.0 - new_p;
      plog = log(new_p);
      pclog = log(pc);
      pold = new_p;
    }
    sq = sqrt(2.0 * am * pc);
    do {
      do {
        angle = M_PI * uniformMT();
        y = tan(angle);
        em = sq * y + am;
      } while ((em < 0.0) || (em >= (en + 1.0)));
      em = floor(em);
      t = 1.2 * sq * (1.0 + y * y)
          * exp(
              oldg - LogGamma(em + 1.0) - LogGamma(en - em + 1.0) + em * plog
                  + (en - em) * pclog);
    } while (uniformMT() > t);
    bnl = em;
  }
  if (new_p != pp) bnl = new_n - bnl;
  return (bnl);
}

inline Real Binomial_rnd_dist::LogGamma(Real val) const {
  int rVal = (int) val;
  if ((val == (Real) rVal) && (rVal < M_LOGGAMMATABLEMAX)) {
    return (static_table.logGammaTable[rVal]);
  } else {
    return (MLogGammaFunction(val));
  }
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenMObj version 0.7. */
/** Identical to method 'bnldev' from Numerical Recipes in C (Press et al.) */
