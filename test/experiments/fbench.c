/*

        John Walker's Floating Point Benchmark, derived from...

	Marinchip Interactive Lens Design System

				     John Walker   December 1980

	By John Walker
	   http://www.fourmilab.ch/

	This  program may be used, distributed, and modified freely as
	long as the origin information is preserved.

	This  is  a  complete  optical	design	raytracing  algorithm,
	stripped of its user interface and recast into portable C.  It
	not only determines execution speed on an  extremely  floating
	point	(including   trig   function)	intensive   real-world
	application, it  checks  accuracy  on  an  algorithm  that  is
	exquisitely  sensitive	to  errors.   The  performance of this
	program is typically far more  sensitive  to  changes  in  the
	efficiency  of	the  trigonometric  library  routines than the
	average floating point program.

	The benchmark may be compiled in two  modes.   If  the	symbol
	INTRIG	is  defined,  built-in	trigonometric  and square root
	routines will be used for all calculations.  Timings made with
        INTRIG  defined  reflect  the  machine's  basic floating point
	performance for the arithmetic operators.  If  INTRIG  is  not
	defined,  the  system  library	<math.h>  functions  are used.
        Results with INTRIG not defined reflect the  system's  library
	performance  and/or  floating  point hardware support for trig
	functions and square root.  Results with INTRIG defined are  a
	good  guide  to  general  floating  point  performance,  while
	results with INTRIG undefined indicate the performance	of  an
	application which is math function intensive.

	Special  note  regarding  errors in accuracy: this program has
	generated numbers identical to the last digit it  formats  and
	checks on the following machines, floating point
	architectures, and languages:

	Marinchip 9900	  QBASIC    IBM 370 double-precision (REAL * 8) format

	IBM PC / XT / AT  Lattice C IEEE 64 bit, 80 bit temporaries
			  High C    same, in line 80x87 code
                          BASICA    "Double precision"
			  Quick BASIC IEEE double precision, software routines

	Sun 3		  C	    IEEE 64 bit, 80 bit temporaries,
				    in-line 68881 code, in-line FPA code.

        MicroVAX II       C         Vax "G" format floating point

	Macintosh Plus	  MPW C     SANE floating point, IEEE 64 bit format
				    implemented in ROM.

	Inaccuracies  reported	by  this  program should be taken VERY
	SERIOUSLY INDEED, as the program has been demonstrated	to  be
	invariant  under  changes in floating point format, as long as
	the format is a recognised double precision  format.   If  you
	encounter errors, please remember that they are just as likely
	to  be	in  the  floating  point  editing   library   or   the
	trigonometric  libraries  as  in  the low level operator code.

	The benchmark assumes that results are basically reliable, and
	only tests the last result computed against the reference.  If
        you're running on  a  suspect  system  you  can  compile  this
	program  with  ACCURACY defined.  This will generate a version
	which executes as an infinite loop, performing the  ray  trace
	and checking the results on every pass.  All incorrect results
	will be reported.

	Representative	timings  are  given  below.   All  have   been
	normalised as if run for 1000 iterations.

  Time in seconds		   Computer, Compiler, and notes
 Normal      INTRIG

 3466.00    4031.00	Commodore 128, 2 Mhz 8510 with software floating
			point.	Abacus Software/Data-Becker Super-C 128,
			version 3.00, run in fast (2 Mhz) mode.  Note:
			the results generated by this system differed
			from the reference results in the 8th to 10th
			decimal place.

 3290.00		IBM PC/AT 6 Mhz, Microsoft/IBM BASICA version A3.00.
                        Run with the "/d" switch, software floating point.

 2131.50		IBM PC/AT 6 Mhz, Lattice C version 2.14, small model.
			This version of Lattice compiles subroutine
			calls which either do software floating point
			or use the 80x87.  The machine on which I ran
			this had an 80287, but the results were so bad
			I wonder if it was being used.

 1598.00		Macintosh Plus, MPW C, SANE Software floating point.

 1582.13		Marinchip 9900 2 Mhz, QBASIC compiler with software
			floating point.  This was a QBASIC version of the
			program which contained the identical algorithm.

  404.00		IBM PC/AT 6 Mhz, Microsoft QuickBASIC version 2.0.
			Software floating point.

  165.15		IBM PC/AT 6 Mhz, Metaware High C version 1.3, small
			model.	This was compiled to call subroutines for
			floating point, and the machine contained an 80287
			which was used by the subroutines.

  143.20		Macintosh II, MPW C, SANE calls.  I was unable to
			determine whether SANE was using the 68881 chip or
			not.

  121.80		Sun 3/160 16 Mhz, Sun C.  Compiled with -fsoft switch
			which executes floating point in software.

   78.78     110.11	IBM RT PC (Model 6150).  IBM AIX 1.0 C compiler
			with -O switch.

   75.2      254.0	Microsoft Quick C 1.0, in-line 8087 instructions,
			compiled with 80286 optimisation on.  (Switches
			were -Ol -FPi87-G2 -AS).  Small memory model.

   69.50		IBM PC/AT 6Mhz, Borland Turbo BASIC 1.0.  Compiled
                        in "8087 required" mode to generate in-line
			code for the math coprocessor.

   66.96		IBM PC/AT 6Mhz, Microsoft QuickBASIC 4.0.  This
			release of QuickBASIC compiles code for the
			80287 math coprocessor.

   66.36     206.35	IBM PC/AT 6Mhz, Metaware High C version 1.3, small
			model.	This was compiled with in-line code for the
			80287 math coprocessor.  Trig functions still call
			library routines.

   63.07     220.43	IBM PC/AT, 6Mhz, Borland Turbo C, in-line 8087 code,
			small model, word alignment, no stack checking,
			8086 code mode.

   17.18		Apollo DN-3000, 12 Mhz 68020 with 68881, compiled
			with in-line code for the 68881 coprocessor.
			According to Apollo, the library routines are chosen
			at runtime based on coprocessor presence.  Since the
			coprocessor was present, the library is supposed to
			use in-line floating point code.

   15.55      27.56	VAXstation II GPX.  Compiled and executed under
			VAX/VMS C.

   15.14      37.93	Macintosh II, Unix system V.  Green Hills 68020
			Unix compiler with in-line code for the 68881
			coprocessor (-O -ZI switches).

   12.69		Sun 3/160 16 Mhz, Sun C.  Compiled with -fswitch,
			which calls a subroutine to select the fastest
			floating point processor.  This was using the 68881.

   11.74      26.73	Compaq Deskpro 386, 16 Mhz 80386 with 16 Mhz 80387.
			Metaware High C version 1.3, compiled with in-line
			for the math coprocessor (but not optimised for the
			80386/80387).  Trig functions still call library
			routines.

    8.43      30.49	Sun 3/160 16 Mhz, Sun C.  Compiled with -f68881,
			generating in-line MC68881 instructions.  Trig
			functions still call library routines.

    6.29      25.17	Sun 3/260 25 Mhz, Sun C.  Compiled with -f68881,
			generating in-line MC68881 instructions.  Trig
			functions still call library routines.

    4.57		Sun 3/260 25 Mhz, Sun FORTRAN 77.  Compiled with
			-O -f68881, generating in-line MC68881 instructions.
			Trig functions are compiled in-line.  This used
			the FORTRAN 77 version of the program, FBFORT77.F.

    4.00      14.20	Sun386i/25 Mhz model 250, Sun C compiler.

    4.00      14.00	Sun386i/25 Mhz model 250, Metaware C.

    3.10      12.00	Compaq 386/387 25 Mhz running SCO Xenix 2.
			Compiled with Metaware HighC 386, optimized
			for 386.

    3.00      12.00	Compaq 386/387 25MHZ optimized for 386/387.

    2.96       5.17	Sun 4/260, Sparc RISC processor.  Sun C,
			compiled with the -O2 switch for global
			optimisation.

    2.47		COMPAQ 486/25, secondary cache disabled, High C,
			486/387, inline f.p., small memory model.

    2.20       3.40	Data General Motorola 88000, 16 Mhz, Gnu C.

    1.56		COMPAQ 486/25, 128K secondary cache, High C, 486/387,
			inline f.p., small memory model.

    0.66       1.50	DEC Pmax, Mips processor.

    0.63       0.91	Sun SparcStation 2, Sun C (SunOS 4.1.1) with
                        -O4 optimisation and "/usr/lib/libm.il" inline
			floating point.

    0.60       1.07	Intel 860 RISC processor, 33 Mhz, Greenhills
			C compiler.

    0.40       0.90	Dec 3MAX, MIPS 3000 processor, -O4.

    0.31       0.90	IBM RS/6000, -O.

    0.1129     0.2119	Dell Dimension XPS P133c, Pentium 133 MHz,
			Windows 95, Microsoft Visual C 5.0.

    0.0883     0.2166	Silicon Graphics Indigo�, MIPS R4400,
                        175 Mhz, "-O3".

    0.0351     0.0561	Dell Dimension XPS R100, Pentium II 400 MHz,
			Windows 98, Microsoft Visual C 5.0.

    0.0312     0.0542	Sun Ultra 2, UltraSPARC V9, 300 MHz, Solaris
			2.5.1.
			
    0.00862    0.01074  Dell Inspiron 9100, Pentium 4, 3.4 GHz, gcc -O3.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef INTRIG
#include <math.h>
#endif

#define cot(x) (1.0 / tan(x))

#define TRUE  1
#define FALSE 0

#define max_surfaces 10

/*  Local variables  */

static char tbfr[132];

static short current_surfaces;
static short paraxial;

static double clear_aperture;

static double aberr_lspher;
static double aberr_osc;
static double aberr_lchrom;

static double max_lspher;
static double max_osc;
static double max_lchrom;

static double radius_of_curvature;
static double object_distance;
static double ray_height;
static double axis_slope_angle;
static double from_index;
static double to_index;

static double spectral_line[9];
static double s[max_surfaces][5];
static double od_sa[2][2];

static char outarr[8][80];	   /* Computed output of program goes here */

int itercount;			   /* The iteration counter for the main loop
				      in the program is made global so that
				      the compiler should not be allowed to
				      optimise out the loop over the ray
				      tracing code. */

#define ITERATIONS 100
int niter = ITERATIONS; 	   /* Iteration counter */

static char *refarr[] = {	   /* Reference results.  These happen to
				      be derived from a run on Microsoft 
				      Quick BASIC on the IBM PC/AT. */

        "   Marginal ray          47.09479120920   0.04178472683",
        "   Paraxial ray          47.08372160249   0.04177864821",
        "Longitudinal spherical aberration:        -0.01106960671",
        "    (Maximum permissible):                 0.05306749907",
        "Offense against sine condition (coma):     0.00008954761",
        "    (Maximum permissible):                 0.00250000000",
        "Axial chromatic aberration:                0.00448229032",
        "    (Maximum permissible):                 0.05306749907"
};

/* The	test  case  used  in  this program is the  design for a 4 inch
   achromatic telescope  objective  used  as  the  example  in  Wyld's
   classic  work  on  ray  tracing by hand, given in Amateur Telescope
   Making, Volume 3.  */

static double testcase[4][4] = {
	{27.05, 1.5137, 63.6, 0.52},
	{-16.68, 1, 0, 0.138},
	{-16.68, 1.6164, 36.7, 0.38},
	{-78.1, 1, 0, 0}
};

/*  Internal trig functions (used only if INTRIG is  defined).	 These
    standard  functions  may be enabled to obtain timings that reflect
    the machine's floating point performance rather than the speed  of
    its trig function evaluation.  */

#ifdef INTRIG

/*  The following definitions should keep you from getting intro trouble
    with compilers which don't let you redefine intrinsic functions.  */

#define sin I_sin
#define cos I_cos
#define tan I_tan
#define sqrt I_sqrt
#define atan I_atan
#define atan2 I_atan2
#define asin I_asin

#define fabs(x)  ((x < 0.0) ? -x : x)

#define pic 3.1415926535897932

/*  Commonly used constants  */

static double pi = pic,
	twopi =pic * 2.0,
	piover4 = pic / 4.0,
	fouroverpi = 4.0 / pic,
	piover2 = pic / 2.0;

/*  Coefficients for ATAN evaluation  */

static double atanc[] = {
	0.0,
	0.4636476090008061165,
	0.7853981633974483094,
	0.98279372324732906714,
	1.1071487177940905022,
	1.1902899496825317322,
	1.2490457723982544262,
	1.2924966677897852673,
	1.3258176636680324644
};

/*  aint(x)	  Return integer part of number.  Truncates towards 0	 */

double aint(x)
double x;
{
	long l;

	/*  Note that this routine cannot handle the full floating point
	    number range.  This function should be in the machine-dependent
	    floating point library!  */

	l = x;
	if ((int)(-0.5) != 0  &&  l < 0 )
	   l++;
	x = l;
	return x;
}

/*  sin(x)	  Return sine, x in radians  */

static double sin(x)
double x;
{
	int sign;
	double y, r, z;

	x = (((sign= (x < 0.0)) != 0) ? -x: x);

	if (x > twopi)
	   x -= (aint(x / twopi) * twopi);

	if (x > pi) {
	   x -= pi;
	   sign = !sign;
	}

	if (x > piover2)
	   x = pi - x;

	if (x < piover4) {
	   y = x * fouroverpi;
	   z = y * y;
	   r = y * (((((((-0.202253129293E-13 * z + 0.69481520350522E-11) * z -
	      0.17572474176170806E-8) * z + 0.313361688917325348E-6) * z -
	      0.365762041821464001E-4) * z + 0.249039457019271628E-2) * z -
	      0.0807455121882807815) * z + 0.785398163397448310);
	} else {
	   y = (piover2 - x) * fouroverpi;
	   z = y * y;
	   r = ((((((-0.38577620372E-12 * z + 0.11500497024263E-9) * z -
	      0.2461136382637005E-7) * z + 0.359086044588581953E-5) * z -
	      0.325991886926687550E-3) * z + 0.0158543442438154109) * z -
	      0.308425137534042452) * z + 1.0;
	}
	return sign ? -r : r;
}

/*  cos(x)	  Return cosine, x in radians, by identity  */

static double cos(x)
double x;
{
	x = (x < 0.0) ? -x : x;
	if (x > twopi)		      /* Do range reduction here to limit */
	   x = x - (aint(x / twopi) * twopi); /* roundoff on add of PI/2    */
	return sin(x + piover2);
}

/*  tan(x)	  Return tangent, x in radians, by identity  */

static double tan(x)
double x;
{
	return sin(x) / cos(x);
}

/*  sqrt(x)	  Return square root.  Initial guess, then Newton-
		  Raphson refinement  */

double sqrt(x)
double x;
{
	double c, cl, y;
	int n;

	if (x == 0.0)
	   return 0.0;

	if (x < 0.0) {
	   fprintf(stderr,
              "\nGood work!  You tried to take the square root of %g",
	     x);
	   fprintf(stderr,
              "\nunfortunately, that is too complex for me to handle.\n");
	   exit(1);
	}

	y = (0.154116 + 1.893872 * x) / (1.0 + 1.047988 * x);

	c = (y - x / y) / 2.0;
	cl = 0.0;
	for (n = 50; c != cl && n--;) {
	   y = y - c;
	   cl = c;
	   c = (y - x / y) / 2.0;
	}
	return y;
}

/*  atan(x)	  Return arctangent in radians,
		  range -pi/2 to pi/2  */

static double atan(x)
double x;
{
	int sign, l, y;
	double a, b, z;

	x = (((sign = (x < 0.0)) != 0) ? -x : x);
	l = 0;

	if (x >= 4.0) {
	   l = -1;
	   x = 1.0 / x;
	   y = 0;
	   goto atl;
	} else {
	   if (x < 0.25) {
	      y = 0;
	      goto atl;
	   }
	}

	y = aint(x / 0.5);
	z = y * 0.5;
	x = (x - z) / (x * z + 1);

atl:
	z = x * x;
	b = ((((893025.0 * z + 49116375.0) * z + 425675250.0) * z +
	    1277025750.0) * z + 1550674125.0) * z + 654729075.0;
	a = (((13852575.0 * z + 216602100.0) * z + 891080190.0) * z +
	    1332431100.0) * z + 654729075.0;
	a = (a / b) * x + atanc[y];
	if (l)
	   a=piover2 - a;
	return sign ? -a : a;
}

/*  atan2(y,x)	  Return arctangent in radians of y/x,
		  range -pi to pi  */

static double atan2(y, x)
double y, x;
{
	double temp;

	if (x == 0.0) {
	   if (y == 0.0)   /*  Special case: atan2(0,0) = 0  */
	      return 0.0;
	   else if (y > 0)
	      return piover2;
	   else
	      return -piover2;
	}
	temp = atan(y / x);
	if (x < 0.0) {
	   if (y >= 0.0)
	      temp += pic;
	   else
	      temp -= pic;
	}
	return temp;
}

/*  asin(x)	  Return arcsine in radians of x  */

static double asin(x)
double x;
{
	if (fabs(x)>1.0) {
	   fprintf(stderr,
              "\nInverse trig functions lose much of their gloss when");
	   fprintf(stderr,
              "\ntheir arguments are greater than 1, such as the");
	   fprintf(stderr,
              "\nvalue %g you passed.\n", x);
	   exit(1);
	}
	return atan2(x, sqrt(1 - x * x));
}
#endif

/*	      Calculate passage through surface

	      If  the variable PARAXIAL is true, the trace through the
	      surface will be done using the paraxial  approximations.
	      Otherwise,  the normal trigonometric trace will be done.

	      This routine takes the following inputs:

	      RADIUS_OF_CURVATURE	  Radius of curvature of surface
					  being crossed.  If 0, surface is
					  plane.

	      OBJECT_DISTANCE		  Distance of object focus from
					  lens vertex.	If 0, incoming
					  rays are parallel and
					  the following must be specified:

	      RAY_HEIGHT		  Height of ray from axis.  Only
					  relevant if OBJECT.DISTANCE == 0

	      AXIS_SLOPE_ANGLE		  Angle incoming ray makes with axis
					  at intercept

	      FROM_INDEX		  Refractive index of medium being left

	      TO_INDEX			  Refractive index of medium being
					  entered.

	      The outputs are the following variables:

	      OBJECT_DISTANCE		  Distance from vertex to object focus
					  after refraction.

	      AXIS_SLOPE_ANGLE		  Angle incoming ray makes with axis
					  at intercept after refraction.

*/

static void transit_surface() {
	double iang,		   /* Incidence angle */
	       rang,		   /* Refraction angle */
	       iang_sin,	   /* Incidence angle sin */
	       rang_sin,	   /* Refraction angle sin */
	       old_axis_slope_angle, sagitta;

	if (paraxial) {
	   if (radius_of_curvature != 0.0) {
	      if (object_distance == 0.0) {
		 axis_slope_angle = 0.0;
		 iang_sin = ray_height / radius_of_curvature;
	      } else
		 iang_sin = ((object_distance -
		    radius_of_curvature) / radius_of_curvature) *
		    axis_slope_angle;

	      rang_sin = (from_index / to_index) *
		 iang_sin;
	      old_axis_slope_angle = axis_slope_angle;
	      axis_slope_angle = axis_slope_angle +
		 iang_sin - rang_sin;
	      if (object_distance != 0.0)
		 ray_height = object_distance * old_axis_slope_angle;
	      object_distance = ray_height / axis_slope_angle;
	      return;
	   }
	   object_distance = object_distance * (to_index / from_index);
	   axis_slope_angle = axis_slope_angle * (from_index / to_index);
	   return;
	}

	if (radius_of_curvature != 0.0) {
	   if (object_distance == 0.0) {
	      axis_slope_angle = 0.0;
	      iang_sin = ray_height / radius_of_curvature;
	   } else {
	      iang_sin = ((object_distance -
		 radius_of_curvature) / radius_of_curvature) *
		 sin(axis_slope_angle);
	   }
	   iang = asin(iang_sin);
	   rang_sin = (from_index / to_index) *
	      iang_sin;
	   old_axis_slope_angle = axis_slope_angle;
	   axis_slope_angle = axis_slope_angle +
	      iang - asin(rang_sin);
	   sagitta = sin((old_axis_slope_angle + iang) / 2.0);
	   sagitta = 2.0 * radius_of_curvature*sagitta*sagitta;
	   object_distance = ((radius_of_curvature * sin(
	      old_axis_slope_angle + iang)) *
	      cot(axis_slope_angle)) + sagitta;
	   return;
	}

	rang = -asin((from_index / to_index) *
	   sin(axis_slope_angle));
	object_distance = object_distance * ((to_index *
	   cos(-rang)) / (from_index *
	   cos(axis_slope_angle)));
	axis_slope_angle = -rang;
}

/*  Perform ray trace in specific spectral line  */

static trace_line(line, ray_h)
int line;
double ray_h;
{
	int i;

	object_distance = 0.0;
	ray_height = ray_h;
	from_index = 1.0;

	for (i = 1; i <= current_surfaces; i++) {
	   radius_of_curvature = s[i][1];
	   to_index = s[i][2];
	   if (to_index > 1.0)
	      to_index = to_index + ((spectral_line[4] -
		 spectral_line[line]) /
		 (spectral_line[3] - spectral_line[6])) * ((s[i][2] - 1.0) /
		 s[i][3]);
	   transit_surface();
	   from_index = to_index;
	   if (i < current_surfaces)
	      object_distance = object_distance - s[i][4];
	}
}

/*  Initialise when called the first time  */

int
main(argc, argv)
int argc;
char *argv[];
{
	int i, j, k, errors;
	double od_fline, od_cline;
#ifdef ACCURACY
	long passes;
#endif

	spectral_line[1] = 7621.0;	 /* A */
	spectral_line[2] = 6869.955;	 /* B */
	spectral_line[3] = 6562.816;	 /* C */
	spectral_line[4] = 5895.944;	 /* D */
	spectral_line[5] = 5269.557;	 /* E */
	spectral_line[6] = 4861.344;	 /* F */
        spectral_line[7] = 4340.477;     /* G'*/
	spectral_line[8] = 3968.494;	 /* H */

	/* Process the number of iterations argument, if one is supplied. */

  niter = 4000000;

	/* Load test case into working array */

	clear_aperture = 4.0;
	current_surfaces = 4;
	for (i = 0; i < current_surfaces; i++)
	   for (j = 0; j < 4; j++)
	      s[i + 1][j + 1] = testcase[i][j];

#ifdef ACCURACY
        printf("Beginning execution of floating point accuracy test...\n");
	passes = 0;
#else
        printf("Ready to begin John Walker's floating point accuracy\n");
        printf("and performance benchmark.  %d iterations will be made.\n\n",
	   niter);

        printf("\nMeasured run time in seconds should be divided by %d\n", (int) niter);
        printf("to normalise for reporting results.  For archival results,\n");
        printf("adjust iteration count so the benchmark runs about five minutes.\n\n");
#endif

	/* Perform ray trace the specified number of times. */

#ifdef ACCURACY
	while (TRUE) {
	   passes++;
	   if ((passes % 100L) == 0) {
              printf("Pass %ld.\n", passes);
	   }
#else
	for (itercount = 0; itercount < niter; itercount++) {
#endif

	   for (paraxial = 0; paraxial <= 1; paraxial++) {

	      /* Do main trace in D light */

	      trace_line(4, clear_aperture / 2.0);
	      od_sa[paraxial][0] = object_distance;
	      od_sa[paraxial][1] = axis_slope_angle;
	   }
	   paraxial = FALSE;

	   /* Trace marginal ray in C */

	   trace_line(3, clear_aperture / 2.0);
	   od_cline = object_distance;

	   /* Trace marginal ray in F */

	   trace_line(6, clear_aperture / 2.0);
	   od_fline = object_distance;

	   aberr_lspher = od_sa[1][0] - od_sa[0][0];
	   aberr_osc = 1.0 - (od_sa[1][0] * od_sa[1][1]) /
	      (sin(od_sa[0][1]) * od_sa[0][0]);
	   aberr_lchrom = od_fline - od_cline;
	   max_lspher = sin(od_sa[0][1]);

	   /* D light */

	   max_lspher = 0.0000926 / (max_lspher * max_lspher);
	   max_osc = 0.0025;
	   max_lchrom = max_lspher;
#ifndef ACCURACY
	}
#endif

	return od_sa[1][0];
}
