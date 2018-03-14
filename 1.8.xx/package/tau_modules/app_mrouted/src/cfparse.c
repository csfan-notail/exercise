#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20120115

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "cfparse.y"
/*
 * Configuration file parser for mrouted.
 *
 * Written by Bill Fenner, NRL, 1994
 *
 * cfparse.y,v 3.8.4.30 1998/03/01 01:48:58 fenner Exp
 */
#include <stdio.h>
#include <stdarg.h>
#include "defs.h"
#include <netdb.h>
#include <ifaddrs.h>

/*
 * Local function declarations
 */
static void		fatal(char *fmt, ...);
static void		warn(char *fmt, ...);
static void		yyerror(char *s);
static char *		next_word(void);
static int		yylex(void);
static uint32_t		valid_if(char *s);
static const char *	ifconfaddr(uint32_t a);
int			yyparse(void);

static FILE *f;

char *configfilename = _PATH_MROUTED_CONF;

extern int cache_lifetime;
extern int prune_lifetime;

int allow_black_holes = 0;

static int lineno;

static struct uvif *v;

static int order, state;
static int noflood = 0;
static int rexmit = VIFF_REXMIT_PRUNES;

struct addrmask {
	uint32_t addr;
	int mask;
};

struct boundnam {
	char		*name;
	struct addrmask	 bound;
};

#define MAXBOUNDS 20

struct boundnam boundlist[MAXBOUNDS];	/* Max. of 20 named boundaries */
int numbounds = 0;			/* Number of named boundaries */

#line 61 "cfparse.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union
{
	int num;
	char *ptr;
	struct addrmask addrmask;
	uint32_t addr;
	struct vf_element *filterelem;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 93 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define CACHE_LIFETIME 257
#define PRUNE_LIFETIME 258
#define PRUNING 259
#define BLACK_HOLE 260
#define NOFLOOD 261
#define PHYINT 262
#define TUNNEL 263
#define NAME 264
#define DISABLE 265
#define ENABLE 266
#define IGMPV1 267
#define SRCRT 268
#define BESIDE 269
#define METRIC 270
#define THRESHOLD 271
#define RATE_LIMIT 272
#define BOUNDARY 273
#define NETMASK 274
#define ALTNET 275
#define ADVERT_METRIC 276
#define FILTER 277
#define ACCEPT 278
#define DENY 279
#define EXACT 280
#define BIDIR 281
#define REXMIT_PRUNES 282
#define REXMIT_PRUNES2 283
#define PASSIVE 284
#define ALLOW_NONPRUNERS 285
#define NOTRANSIT 286
#define BLASTER 287
#define FORCE_LEAF 288
#define PRUNE_LIFETIME2 289
#define NOFLOOD2 290
#define SYSNAM 291
#define SYSCONTACT 292
#define SYSVERSION 293
#define SYSLOCATION 294
#define BOOLEAN 295
#define NUMBER 296
#define STRING 297
#define ADDRMASK 298
#define ADDR 299
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,   10,   10,   11,   13,   11,   15,   11,   11,   11,
   11,   11,   11,   11,   11,   11,   11,   11,   11,   11,
   14,   14,   16,   16,   16,   16,   12,   12,   18,   18,
   18,   18,   18,   18,   18,   18,   18,   18,   17,   17,
   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,
   17,   17,   17,   17,   17,   17,   17,   17,   17,   17,
   17,    1,    1,    2,    2,    3,    3,    4,    5,    5,
    6,    6,    7,    7,    8,    8,    9,
};
static const short yylen[] = {                            2,
    1,    0,    2,    1,    0,    4,    0,    5,    2,    2,
    2,    1,    1,    1,    2,    3,    2,    2,    2,    2,
    0,    2,    1,    1,    2,    1,    0,    2,    1,    1,
    1,    1,    2,    1,    2,    1,    1,    2,    2,    1,
    2,    1,    2,    1,    2,    1,    2,    1,    1,    2,
    1,    1,    1,    1,    1,    2,    2,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    2,    1,
};
static const short yydefred[] = {                         2,
    0,    0,    4,    0,    0,    0,   12,   13,    0,    0,
    0,    0,    0,    0,    0,    0,    3,    9,   10,   11,
   63,   62,    5,    0,    0,   15,   17,   18,   19,   20,
   27,   65,   64,    7,   68,   16,    0,   21,   30,   31,
   32,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   61,    0,   51,   55,   53,   54,    0,    0,   52,   29,
   28,    0,   41,   39,   45,   67,   47,   66,   33,   69,
   70,   35,   43,   72,   77,   57,   71,    0,    0,   59,
   50,   38,   56,   26,    0,   22,   23,   74,   76,   25,
};
static const short yydgoto[] = {                          1,
   23,   34,   67,   36,   72,   76,   77,   78,   79,    2,
   17,   37,   31,   62,   38,   86,   60,   61,
};
static const short yysindex[] = {                         0,
    0, -254,    0, -296, -275, -273,    0,    0, -286, -286,
 -274, -271, -272, -270, -268, -267,    0,    0,    0,    0,
    0,    0,    0, -285, -266,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -224,    0,    0,    0,
    0, -265, -263, -262, -282, -285, -281, -261, -278, -278,
    0, -269,    0,    0,    0,    0, -259, -251,    0,    0,
    0, -199,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -245, -222,    0,
    0,    0,    0,    0, -239,    0,    0,    0,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,   67,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  555,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  598,    0,    0,    0,
    0,   44,   83,  122,  161,  399,  438,  200,  239,  278,
    0,  321,    0,    0,    0,    0,  477,    0,    0,    0,
    0,  637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  360,    1,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
   58,   29,    0,   31,    0,   28,    3,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   27,    0,
};
#define YYTABLESIZE 931
static const short yytable[] = {                         18,
   75,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   21,   32,   22,   33,   66,   35,   70,   71,   74,   75,
   19,   20,   25,   26,   27,   81,   28,   12,   29,   30,
   63,   35,   64,   65,   73,   82,   13,   14,   15,   16,
   39,   40,   41,   42,   83,   42,   43,   44,   45,   46,
   47,   48,   75,   49,   50,   90,   51,   89,   52,   53,
   54,   55,   56,   57,   58,   59,    1,   24,   84,   85,
   42,   43,   44,   45,   69,   68,   48,   80,   49,   50,
   88,   51,   40,   52,   53,   54,   55,   56,   87,   58,
   59,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   46,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   48,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   44,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   58,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   75,   75,   75,   75,
   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,
   75,   75,   75,   75,   75,   75,   75,   60,   75,   75,
    0,   75,   75,   75,   75,   75,   75,   75,   75,   75,
   75,   75,   75,   75,   75,    0,    0,    0,   75,   42,
   42,   42,   42,   42,   42,   42,   42,   42,   42,   42,
   42,   42,   42,   42,   42,   42,   42,   42,   42,   42,
   49,   42,   42,    0,   42,   42,   42,   42,   42,   42,
   42,   42,   42,   42,   42,   42,   42,   42,   40,   40,
   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
   40,   40,   40,   40,   40,   40,   40,   40,   40,   73,
   40,   40,    0,   40,   40,   40,   40,   40,   40,   40,
   40,   40,   40,   40,   40,   40,   40,   46,   46,   46,
   46,   46,   46,   46,   46,   46,   46,   46,   46,   46,
   46,   46,   46,   46,   46,   46,   46,   46,   34,   46,
   46,    0,   46,   46,   46,   46,   46,   46,   46,   46,
   46,   46,   46,   46,   46,   46,   48,   48,   48,   48,
   48,   48,   48,   48,   48,   48,   48,   48,   48,   48,
   48,   48,   48,   48,   48,   48,   48,   36,   48,   48,
    0,   48,   48,   48,   48,   48,   48,   48,   48,   48,
   48,   48,   48,   48,   48,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   44,   44,   37,   44,   44,    0,
   44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   44,   44,   44,   44,   58,   58,   58,   58,   58,   58,
   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
   58,   58,   58,   58,   58,   24,   58,   58,    0,   58,
   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
   58,   58,   58,   60,   60,   60,   60,   60,   60,   60,
   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
   60,   60,   60,   60,   14,   60,   60,    0,   60,   60,
   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
   60,   60,    0,    0,    0,    0,   49,   49,   49,   49,
   49,   49,   49,   49,   49,   49,   49,   49,   49,   49,
   49,   49,   49,   49,   49,   49,   49,    6,   49,   49,
    0,   49,   49,   49,   49,   49,   49,   49,   49,   49,
   49,   49,   49,   49,   49,   73,   73,   73,   73,   73,
   73,   73,   73,   73,   73,   73,   73,   73,   73,   73,
   73,   73,   73,   73,   73,   73,    8,   73,   73,    0,
   73,   73,   73,   73,   73,   73,   73,   73,   73,   73,
   73,   73,   73,   73,   34,   34,   34,   34,   34,   34,
   34,   34,   34,   34,   34,   34,    0,    0,   34,   34,
   34,   34,   34,   34,   34,    0,   34,   34,    0,   34,
   34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
   34,   34,   34,   36,   36,   36,   36,   36,   36,   36,
   36,   36,   36,   36,   36,    0,    0,   36,   36,   36,
   36,   36,   36,   36,    0,   36,   36,    0,   36,   36,
   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
   36,   36,   37,   37,   37,   37,   37,   37,   37,   37,
   37,   37,   37,   37,    0,    0,   37,   37,   37,   37,
   37,   37,   37,    0,   37,   37,    0,   37,   37,   37,
   37,   37,   37,   37,   37,   37,   37,   37,   37,   37,
   37,   24,   24,   24,   24,   24,   24,   24,   24,   24,
    0,    0,    0,   24,   24,   24,   24,   24,   24,    0,
    0,   24,    0,   24,   24,    0,   24,   24,   24,   24,
   24,   24,   24,    0,   24,   24,   24,   24,   24,   24,
   14,   14,   14,   14,   14,   14,   14,   14,   14,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,    0,    0,
    0,    0,    0,    0,    0,   14,   14,   14,   14,    0,
    0,    0,    0,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    6,
    0,    0,    0,    0,    0,    0,    0,    0,    6,    6,
    6,    6,    8,    8,    8,    8,    8,    8,    8,    8,
    8,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    8,    0,
    0,    0,    0,    0,    0,    0,    0,    8,    8,    8,
    8,
};
static const short yycheck[] = {                        296,
    0,  256,  257,  258,  259,  260,  261,  262,  263,  264,
  297,  297,  299,  299,  297,  298,  298,  299,  297,  298,
  296,  295,  297,  295,  297,  295,  297,  282,  297,  297,
  296,  298,  296,  296,  296,  295,  291,  292,  293,  294,
  265,  266,  267,    0,  296,  270,  271,  272,  273,  274,
  275,  276,  298,  278,  279,  295,  281,  280,  283,  284,
  285,  286,  287,  288,  289,  290,    0,   10,  268,  269,
  270,  271,  272,  273,   46,   45,  276,   50,  278,  279,
   78,  281,    0,  283,  284,  285,  286,  287,   62,  289,
  290,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  256,  257,  258,  259,
  260,  261,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,    0,  278,  279,
   -1,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,   -1,   -1,   -1,  298,  256,
  257,  258,  259,  260,  261,  262,  263,  264,  265,  266,
  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,
    0,  278,  279,   -1,  281,  282,  283,  284,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  256,  257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,  275,  276,    0,
  278,  279,   -1,  281,  282,  283,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  256,  257,  258,
  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  275,  276,    0,  278,
  279,   -1,  281,  282,  283,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  256,  257,  258,  259,
  260,  261,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,    0,  278,  279,
   -1,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  256,  257,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,    0,  278,  279,   -1,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  256,  257,  258,  259,  260,  261,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,  275,  276,    0,  278,  279,   -1,  281,
  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  256,  257,  258,  259,  260,  261,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  275,  276,    0,  278,  279,   -1,  281,  282,
  283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
  293,  294,   -1,   -1,   -1,   -1,  256,  257,  258,  259,
  260,  261,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,    0,  278,  279,
   -1,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  256,  257,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,    0,  278,  279,   -1,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  256,  257,  258,  259,  260,  261,
  262,  263,  264,  265,  266,  267,   -1,   -1,  270,  271,
  272,  273,  274,  275,  276,   -1,  278,  279,   -1,  281,
  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  256,  257,  258,  259,  260,  261,  262,
  263,  264,  265,  266,  267,   -1,   -1,  270,  271,  272,
  273,  274,  275,  276,   -1,  278,  279,   -1,  281,  282,
  283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
  293,  294,  256,  257,  258,  259,  260,  261,  262,  263,
  264,  265,  266,  267,   -1,   -1,  270,  271,  272,  273,
  274,  275,  276,   -1,  278,  279,   -1,  281,  282,  283,
  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  256,  257,  258,  259,  260,  261,  262,  263,  264,
   -1,   -1,   -1,  268,  269,  270,  271,  272,  273,   -1,
   -1,  276,   -1,  278,  279,   -1,  281,  282,  283,  284,
  285,  286,  287,   -1,  289,  290,  291,  292,  293,  294,
  256,  257,  258,  259,  260,  261,  262,  263,  264,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  291,  292,  293,  294,   -1,
   -1,   -1,   -1,  256,  257,  258,  259,  260,  261,  262,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  282,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,  292,
  293,  294,  256,  257,  258,  259,  260,  261,  262,  263,
  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,  292,  293,
  294,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 299
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CACHE_LIFETIME","PRUNE_LIFETIME",
"PRUNING","BLACK_HOLE","NOFLOOD","PHYINT","TUNNEL","NAME","DISABLE","ENABLE",
"IGMPV1","SRCRT","BESIDE","METRIC","THRESHOLD","RATE_LIMIT","BOUNDARY",
"NETMASK","ALTNET","ADVERT_METRIC","FILTER","ACCEPT","DENY","EXACT","BIDIR",
"REXMIT_PRUNES","REXMIT_PRUNES2","PASSIVE","ALLOW_NONPRUNERS","NOTRANSIT",
"BLASTER","FORCE_LEAF","PRUNE_LIFETIME2","NOFLOOD2","SYSNAM","SYSCONTACT",
"SYSVERSION","SYSLOCATION","BOOLEAN","NUMBER","STRING","ADDRMASK","ADDR",
};
static const char *yyrule[] = {
"$accept : conf",
"conf : stmts",
"stmts :",
"stmts : stmts stmt",
"stmt : error",
"$$1 :",
"stmt : PHYINT interface $$1 ifmods",
"$$2 :",
"stmt : TUNNEL interface addrname $$2 tunnelmods",
"stmt : CACHE_LIFETIME NUMBER",
"stmt : PRUNE_LIFETIME NUMBER",
"stmt : PRUNING BOOLEAN",
"stmt : BLACK_HOLE",
"stmt : NOFLOOD",
"stmt : REXMIT_PRUNES",
"stmt : REXMIT_PRUNES BOOLEAN",
"stmt : NAME STRING boundary",
"stmt : SYSNAM STRING",
"stmt : SYSCONTACT STRING",
"stmt : SYSVERSION STRING",
"stmt : SYSLOCATION STRING",
"tunnelmods :",
"tunnelmods : tunnelmods tunnelmod",
"tunnelmod : mod",
"tunnelmod : BESIDE",
"tunnelmod : BESIDE BOOLEAN",
"tunnelmod : SRCRT",
"ifmods :",
"ifmods : ifmods ifmod",
"ifmod : mod",
"ifmod : DISABLE",
"ifmod : ENABLE",
"ifmod : IGMPV1",
"ifmod : NETMASK addrname",
"ifmod : NETMASK",
"ifmod : ALTNET addrmask",
"ifmod : ALTNET",
"ifmod : FORCE_LEAF",
"ifmod : FORCE_LEAF BOOLEAN",
"mod : THRESHOLD NUMBER",
"mod : THRESHOLD",
"mod : METRIC NUMBER",
"mod : METRIC",
"mod : ADVERT_METRIC NUMBER",
"mod : ADVERT_METRIC",
"mod : RATE_LIMIT NUMBER",
"mod : RATE_LIMIT",
"mod : BOUNDARY bound",
"mod : BOUNDARY",
"mod : REXMIT_PRUNES2",
"mod : REXMIT_PRUNES2 BOOLEAN",
"mod : PASSIVE",
"mod : NOFLOOD2",
"mod : NOTRANSIT",
"mod : BLASTER",
"mod : ALLOW_NONPRUNERS",
"mod : PRUNE_LIFETIME2 NUMBER",
"mod : ACCEPT filter",
"mod : ACCEPT",
"mod : DENY filter",
"mod : DENY",
"mod : BIDIR",
"interface : ADDR",
"interface : STRING",
"addrname : ADDR",
"addrname : STRING",
"bound : boundary",
"bound : STRING",
"boundary : ADDRMASK",
"addrmask : ADDRMASK",
"addrmask : ADDR",
"filter : filtlist",
"filter : STRING",
"filtlist : filtelement",
"filtlist : filtelement filtlist",
"filtelement : filtelem",
"filtelement : filtelem EXACT",
"filtelem : ADDRMASK",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 637 "cfparse.y"
static void fatal(char *fmt, ...)
{
    va_list ap;
    char buf[MAXHOSTNAMELEN + 100];

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    logit(LOG_ERR, 0, "%s: %s near line %d", configfilename, buf, lineno);
}

static void warn(char *fmt, ...)
{
    va_list ap;
    char buf[200];

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    logit(LOG_WARNING, 0, "%s: %s near line %d", configfilename, buf, lineno);
}

static void yyerror(char *s)
{
    logit(LOG_ERR, 0, "%s: %s near line %d", configfilename, s, lineno);
}

static char *next_word(void)
{
    static char buf[1024];
    static char *p=NULL;
    char *q;

    while (1) {
        if (!p || !*p) {
            lineno++;
            if (fgets(buf, sizeof(buf), f) == NULL)
                return NULL;
            p = buf;
        }

        while (*p && (*p == ' ' || *p == '\t'))	/* skip whitespace */
            p++;

        if (*p == '#') {
            p = NULL;		/* skip comments */
            continue;
        }

        q = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            p++;		/* find next whitespace */
        *p++ = '\0';	/* null-terminate string */

        if (!*q) {
            p = NULL;
            continue;	/* if 0-length string, read another line */
        }

        return q;
    }
}

/*
 * List of keywords.  Must have an empty record at the end to terminate
 * list.  If a second value is specified, the first is used at the beginning
 * of the file and the second is used while parsing interfaces (e.g. after
 * the first "phyint" or "tunnel" keyword).
 */
static struct keyword {
	char	*word;
	int	val1;
	int	val2;
} words[] = {
	{ "cache_lifetime",	CACHE_LIFETIME, 0 },
	{ "prune_lifetime",	PRUNE_LIFETIME,	PRUNE_LIFETIME2 },
	{ "pruning",		PRUNING, 0 },
	{ "phyint",		PHYINT, 0 },
	{ "tunnel",		TUNNEL, 0 },
	{ "disable",		DISABLE, 0 },
	{ "enable",		ENABLE, 0 },
	{ "metric",		METRIC, 0 },
	{ "advert_metric",	ADVERT_METRIC, 0 },
	{ "threshold",		THRESHOLD, 0 },
	{ "rate_limit",		RATE_LIMIT, 0 },
	{ "force_leaf",		FORCE_LEAF, 0 },
	{ "srcrt",		SRCRT, 0 },
	{ "sourceroute",	SRCRT, 0 },
	{ "boundary",		BOUNDARY, 0 },
	{ "netmask",		NETMASK, 0 },
	{ "igmpv1",		IGMPV1, 0 },
	{ "altnet",		ALTNET, 0 },
	{ "name",		NAME, 0 },
	{ "accept",		ACCEPT, 0 },
	{ "deny",		DENY, 0 },
	{ "exact",		EXACT, 0 },
	{ "bidir",		BIDIR, 0 },
	{ "allow_nonpruners",	ALLOW_NONPRUNERS, 0 },
#ifdef ALLOW_BLACK_HOLES
	{ "allow_black_holes",	BLACK_HOLE, 0 },
#endif
	{ "noflood",		NOFLOOD, NOFLOOD2 },
	{ "notransit",		NOTRANSIT, 0 },
	{ "blaster",		BLASTER, 0 },
	{ "rexmit_prunes",	REXMIT_PRUNES, REXMIT_PRUNES2 },
	{ "passive",		PASSIVE, 0 },
	{ "beside",		BESIDE, 0 },
#if 0 /* Removed SNMP support */
	{ "sysName",		SYSNAM, 0 },
	{ "sysContact",		SYSCONTACT, 0 },
	{ "sysVersion",		SYSVERSION, 0 },
	{ "sysLocation",	SYSLOCATION, 0 },
#endif
	{ NULL,			0, 0 }
};


static int yylex(void)
{
    uint32_t addr, n;
    char *q;
    struct keyword *w;

    if ((q = next_word()) == NULL) {
        return 0;
    }

    for (w = words; w->word; w++)
        if (!strcmp(q, w->word))
            return (state && w->val2) ? w->val2 : w->val1;

    if (!strcmp(q,"on") || !strcmp(q,"yes")) {
        yylval.num = 1;
        return BOOLEAN;
    }

    if (!strcmp(q,"off") || !strcmp(q,"no")) {
        yylval.num = 0;
        return BOOLEAN;
    }

    if (!strcmp(q,"default")) {
        yylval.addrmask.mask = 0;
        yylval.addrmask.addr = 0;
        return ADDRMASK;
    }

    if (sscanf(q,"%[.0-9]/%u%c",s1,&n,s2) == 2) {
        if ((addr = inet_parse(s1,1)) != 0xffffffff) {
            yylval.addrmask.mask = n;
            yylval.addrmask.addr = addr;
            return ADDRMASK;
        }
        /* fall through to returning STRING */
    }

    if (sscanf(q,"%[.0-9]%c",s1,s2) == 1) {
        if ((addr = inet_parse(s1,4)) != 0xffffffff &&
            inet_valid_host(addr)) { 
            yylval.addr = addr;
            return ADDR;
        }
    }

    if (sscanf(q,"0x%8x%c", &n, s1) == 1) {
        yylval.addr = n;
        return ADDR;
    }

    if (sscanf(q,"%u%c",&n,s1) == 1) {
        yylval.num = n;
        return NUMBER;
    }

    yylval.ptr = q;

    return STRING;
}

void config_vifs_from_file(void)
{
    order = 0;
    state = 0;
    numbounds = 0;
    lineno = 0;

    if ((f = fopen(configfilename, "r")) == NULL) {
        if (errno != ENOENT)
            logit(LOG_ERR, errno, "Cannot open %s", configfilename);
        return;
    }

    yyparse();

    fclose(f);
}

static uint32_t valid_if(char *s)
{
    vifi_t vifi;
    struct uvif *v;

    for (vifi=0, v=uvifs; vifi<numvifs; vifi++, v++) {
        if (!strcmp(v->uv_name, s))
            return v->uv_lcl_addr;
    }

    return 0;
}

static const char *ifconfaddr(uint32_t a)
{
    static char ifname[IFNAMSIZ];
    struct ifaddrs *ifap, *ifa;

    if (getifaddrs(&ifap) != 0)
	return NULL;

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
	if (ifa->ifa_addr &&
	    ifa->ifa_addr->sa_family == AF_INET &&
	    ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr == a) {
	    strlcpy(ifname, ifa->ifa_name, sizeof(ifname));
	    freeifaddrs(ifap);

	    return ifname;
	}
    }

    freeifaddrs(ifap);

    return NULL;
}

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "ellemtel"
 *  c-basic-offset: 4
 * End:
 */
#line 807 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = data->s_mark - data->s_base;
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 5:
#line 102 "cfparse.y"
	{
	    vifi_t vifi;

	    state++;

	    if (order)
		fatal("phyints must appear before tunnels");

	    for (vifi = 0, v = uvifs; vifi < numvifs; ++vifi, ++v) {
		if (!(v->uv_flags & VIFF_TUNNEL) && yystack.l_mark[0].addr == v->uv_lcl_addr)
		    break;
	    }

	    if (vifi == numvifs && !missingok)
		fatal("%s is not a configured interface", inet_fmt(yystack.l_mark[0].addr, s1, sizeof(s1)));
	    if (vifi == numvifs)
		warn("%s is not a configured interface, continuing", inet_fmt(yystack.l_mark[0].addr, s1, sizeof(s1)));
	}
break;
case 7:
#line 122 "cfparse.y"
	{
	    const char *ifname;
	    struct ifreq ffr;
	    vifi_t vifi;

	    order++;

	    ifname = ifconfaddr(yystack.l_mark[-1].addr);
	    if (ifname == 0)
		fatal("Tunnel local address %s is not mine", inet_fmt(yystack.l_mark[-1].addr, s1, sizeof(s1)));

	    if (((ntohl(yystack.l_mark[-1].addr) & IN_CLASSA_NET) >> IN_CLASSA_NSHIFT) == IN_LOOPBACKNET)
		fatal("Tunnel local address %s is a loopback address", inet_fmt(yystack.l_mark[-1].addr, s1, sizeof(s1)));

	    if (ifconfaddr(yystack.l_mark[0].addr) != NULL)
		fatal("Tunnel remote address %s is one of mine", inet_fmt(yystack.l_mark[0].addr, s1, sizeof(s1)));

	    for (vifi = 0, v = uvifs; vifi < numvifs; ++vifi, ++v) {
		if (v->uv_flags & VIFF_TUNNEL) {
		    if (yystack.l_mark[0].addr == v->uv_rmt_addr)
			fatal("Duplicate tunnel to %s",
			      inet_fmt(yystack.l_mark[0].addr, s1, sizeof(s1)));
		} else if (!(v->uv_flags & VIFF_DISABLED)) {
		    if ((yystack.l_mark[0].addr & v->uv_subnetmask) == v->uv_subnet)
			fatal("Unnecessary tunnel to %s, same subnet as vif %d (%s)",
			      inet_fmt(yystack.l_mark[0].addr, s1, sizeof(s1)), vifi, v->uv_name);
		}
	    }

	    if (numvifs == MAXVIFS)
		fatal("too many vifs");

	    strlcpy(ffr.ifr_name, ifname, sizeof(ffr.ifr_name));
	    if (ioctl(udp_socket, SIOCGIFFLAGS, (char *)&ffr)<0)
		fatal("ioctl SIOCGIFFLAGS on %s", ffr.ifr_name);

	    v = &uvifs[numvifs];
	    zero_vif(v, 1);
	    v->uv_flags	= VIFF_TUNNEL | rexmit | noflood;
	    v->uv_flags |= VIFF_OTUNNEL; /*XXX*/
	    v->uv_lcl_addr	= yystack.l_mark[-1].addr;
	    v->uv_rmt_addr	= yystack.l_mark[0].addr;
	    v->uv_dst_addr	= yystack.l_mark[0].addr;
	    strlcpy(v->uv_name, ffr.ifr_name, sizeof(v->uv_name));

	    if (!(ffr.ifr_flags & IFF_UP)) {
		v->uv_flags |= VIFF_DOWN;
		vifs_down = TRUE;
	    }
	}
break;
case 8:
#line 173 "cfparse.y"
	{
	    if (!(v->uv_flags & VIFF_OTUNNEL))
		init_ipip_on_vif(v);

	    logit(LOG_INFO, 0, "installing tunnel from %s to %s as vif #%u - rate=%d",
		  inet_fmt(yystack.l_mark[-3].addr, s1, sizeof(s1)), inet_fmt(yystack.l_mark[-2].addr, s2, sizeof(s2)),
		  numvifs, v->uv_rate_limit);

	    ++numvifs;

	}
break;
case 9:
#line 185 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < MIN_CACHE_LIFETIME)
		warn("cache_lifetime %d must be at least %d", yystack.l_mark[0].num, MIN_CACHE_LIFETIME);
	    else
		cache_lifetime = yystack.l_mark[0].num;
	}
break;
case 10:
#line 192 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < MIN_PRUNE_LIFETIME)
		warn("prune_lifetime %d must be at least %d", yystack.l_mark[0].num, MIN_PRUNE_LIFETIME);
	    else
		prune_lifetime = yystack.l_mark[0].num;
	}
break;
case 11:
#line 199 "cfparse.y"
	{
	    if (yystack.l_mark[0].num != 1)
		warn("Disabling pruning is no longer supported");
	}
break;
case 12:
#line 204 "cfparse.y"
	{
#ifdef ALLOW_BLACK_HOLES
	    allow_black_holes = 1;
#endif
	}
break;
case 13:
#line 215 "cfparse.y"
	{
	    vifi_t vifi;

	    noflood = VIFF_NOFLOOD;
	    for (vifi = 0, v = uvifs; vifi < numvifs; ++vifi, ++v)
		v->uv_flags |= VIFF_NOFLOOD;
	}
break;
case 14:
#line 228 "cfparse.y"
	{
	    vifi_t vifi;

	    for (vifi = 0, v = uvifs; vifi < numvifs; ++vifi, ++v)
		v->uv_flags |= VIFF_REXMIT_PRUNES;
	}
break;
case 15:
#line 240 "cfparse.y"
	{
	    if (yystack.l_mark[0].num) {
		vifi_t vifi;

		for (vifi = 0, v = uvifs; vifi < numvifs; ++vifi, ++v)
		    v->uv_flags |= VIFF_REXMIT_PRUNES;
	    } else {
		rexmit = 0;
	    }
	}
break;
case 16:
#line 251 "cfparse.y"
	{
	    size_t len = strlen(yystack.l_mark[-1].ptr) + 1;
	    if (numbounds >= MAXBOUNDS) {
		fatal("Too many named boundaries (max %d)", MAXBOUNDS);
	    }

	    boundlist[numbounds].name = malloc(len);
	    strlcpy(boundlist[numbounds].name, yystack.l_mark[-1].ptr, len);
	    boundlist[numbounds++].bound = yystack.l_mark[0].addrmask;
	}
break;
case 17:
#line 262 "cfparse.y"
	{
	    /* Removed SNMP support */
	}
break;
case 18:
#line 266 "cfparse.y"
	{
	    /* Removed SNMP support */
	}
break;
case 19:
#line 270 "cfparse.y"
	{
	    /* Removed SNMP support */
	}
break;
case 20:
#line 274 "cfparse.y"
	{
	    /* Removed SNMP support */
	}
break;
case 24:
#line 285 "cfparse.y"
	{
	    v->uv_flags |= VIFF_OTUNNEL;
	}
break;
case 25:
#line 289 "cfparse.y"
	{
	    if (yystack.l_mark[0].num)
		v->uv_flags |= VIFF_OTUNNEL;
	    else
		v->uv_flags &= ~VIFF_OTUNNEL;
	}
break;
case 26:
#line 296 "cfparse.y"
	{
	    fatal("Source-route tunnels not supported");
	}
break;
case 30:
#line 306 "cfparse.y"
	{ v->uv_flags |= VIFF_DISABLED; }
break;
case 31:
#line 307 "cfparse.y"
	{ v->uv_flags &= ~VIFF_DISABLED; }
break;
case 32:
#line 308 "cfparse.y"
	{ v->uv_flags |= VIFF_IGMPV1; }
break;
case 33:
#line 310 "cfparse.y"
	{
	    uint32_t subnet, mask;

	    mask = yystack.l_mark[0].addr;
	    subnet = v->uv_lcl_addr & mask;
	    if (!inet_valid_subnet(subnet, mask))
		fatal("Invalid netmask");
	    v->uv_subnet = subnet;
	    v->uv_subnetmask = mask;
	    v->uv_subnetbcast = subnet | ~mask;
	}
break;
case 34:
#line 322 "cfparse.y"
	{
	    warn("Expected address after netmask keyword, ignored");
	}
break;
case 35:
#line 326 "cfparse.y"
	{
	    struct phaddr *ph;

	    ph = (struct phaddr *)malloc(sizeof(struct phaddr));
	    if (!ph) {
		fatal("out of memory");
		return 0;	/* Never reached */
	    }

	    if (yystack.l_mark[0].addrmask.mask) {
		VAL_TO_MASK(ph->pa_subnetmask, yystack.l_mark[0].addrmask.mask);
	    } else {
		ph->pa_subnetmask = v->uv_subnetmask;
	    }

	    ph->pa_subnet = yystack.l_mark[0].addrmask.addr & ph->pa_subnetmask;
	    ph->pa_subnetbcast = ph->pa_subnet | ~ph->pa_subnetmask;

	    if (yystack.l_mark[0].addrmask.addr & ~ph->pa_subnetmask)
		warn("Extra subnet %s/%d has host bits set",
		     inet_fmt(yystack.l_mark[0].addrmask.addr, s1, sizeof(s1)), yystack.l_mark[0].addrmask.mask);

	    ph->pa_next = v->uv_addrs;
	    v->uv_addrs = ph;
	}
break;
case 36:
#line 352 "cfparse.y"
	{
	    warn("Expected address after altnet keyword, ignored");
	}
break;
case 37:
#line 356 "cfparse.y"
	{
	    v->uv_flags |= VIFF_FORCE_LEAF;
	}
break;
case 38:
#line 360 "cfparse.y"
	{
	    if (yystack.l_mark[0].num)
		v->uv_flags |= VIFF_FORCE_LEAF;
	    else
		v->uv_flags &= ~VIFF_FORCE_LEAF;
	}
break;
case 39:
#line 369 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < 1 || yystack.l_mark[0].num > 255)
		fatal("Invalid threshold %d",yystack.l_mark[0].num);
	    v->uv_threshold = yystack.l_mark[0].num;
	}
break;
case 40:
#line 375 "cfparse.y"
	{
	    warn("Expected number after threshold keyword, ignored");
	}
break;
case 41:
#line 379 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < 1 || yystack.l_mark[0].num > UNREACHABLE)
		fatal("Invalid metric %d",yystack.l_mark[0].num);
	    v->uv_metric = yystack.l_mark[0].num;
	}
break;
case 42:
#line 385 "cfparse.y"
	{
	    warn("Expected number after metric keyword, ignored");
	}
break;
case 43:
#line 389 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < 0 || yystack.l_mark[0].num > UNREACHABLE - 1)
		fatal("Invalid advert_metric %d", yystack.l_mark[0].num);
	    v->uv_admetric = yystack.l_mark[0].num;
	}
break;
case 44:
#line 395 "cfparse.y"
	{
	    warn("Expected number after advert_metric keyword, ignored");
	}
break;
case 45:
#line 399 "cfparse.y"
	{
	    if (yystack.l_mark[0].num > MAX_RATE_LIMIT)
		fatal("Invalid rate_limit %d",yystack.l_mark[0].num);
	    v->uv_rate_limit = yystack.l_mark[0].num;
	}
break;
case 46:
#line 405 "cfparse.y"
	{
	    warn("Expected number after rate_limit keyword, ignored");
	}
break;
case 47:
#line 409 "cfparse.y"
	{
	    struct vif_acl *v_acl;

	    v_acl = (struct vif_acl *)malloc(sizeof(struct vif_acl));
	    if (!v_acl) {
		fatal("out of memory");
		return 0;	/* Never reached */
	    }

	    VAL_TO_MASK(v_acl->acl_mask, yystack.l_mark[0].addrmask.mask);
	    v_acl->acl_addr = yystack.l_mark[0].addrmask.addr & v_acl->acl_mask;
	    if (yystack.l_mark[0].addrmask.addr & ~v_acl->acl_mask)
		warn("Boundary spec %s/%d has host bits set",
		     inet_fmt(yystack.l_mark[0].addrmask.addr, s1, sizeof(s1)), yystack.l_mark[0].addrmask.mask);
	    v_acl->acl_next = v->uv_acl;
	    v->uv_acl = v_acl;
	}
break;
case 48:
#line 427 "cfparse.y"
	{
	    warn("Expected boundary spec after boundary keyword, ignored");
	}
break;
case 49:
#line 431 "cfparse.y"
	{
	    v->uv_flags |= VIFF_REXMIT_PRUNES;
	}
break;
case 50:
#line 435 "cfparse.y"
	{
	    if (yystack.l_mark[0].num)
		v->uv_flags |= VIFF_REXMIT_PRUNES;
	    else
		v->uv_flags &= ~VIFF_REXMIT_PRUNES;
	}
break;
case 51:
#line 442 "cfparse.y"
	{
	    v->uv_flags |= VIFF_PASSIVE;
	}
break;
case 52:
#line 446 "cfparse.y"
	{
	    v->uv_flags |= VIFF_NOFLOOD;
	}
break;
case 53:
#line 450 "cfparse.y"
	{
	    v->uv_flags |= VIFF_NOTRANSIT;
	}
break;
case 54:
#line 454 "cfparse.y"
	{
	    v->uv_flags |= VIFF_BLASTER;
	    blaster_alloc(v - uvifs);
	}
break;
case 55:
#line 459 "cfparse.y"
	{
		    v->uv_flags |= VIFF_ALLOW_NONPRUNERS;
	}
break;
case 56:
#line 463 "cfparse.y"
	{
	    if (yystack.l_mark[0].num < MIN_PRUNE_LIFETIME)
		warn("prune_lifetime %d must be at least %d", yystack.l_mark[0].num, MIN_PRUNE_LIFETIME);
	    else
		v->uv_prune_lifetime = yystack.l_mark[0].num;
	}
break;
case 57:
#line 470 "cfparse.y"
	{
	    if (v->uv_filter == NULL) {
		struct vif_filter *v_filter;

		v_filter = (struct vif_filter *)malloc(sizeof(struct vif_filter));
		if (!v_filter) {
		    fatal("out of memory");
		    return 0;	/* Never reached */
		}

		v_filter->vf_flags = 0;
		v_filter->vf_type = VFT_ACCEPT;
		v_filter->vf_filter = yystack.l_mark[0].filterelem;
		v->uv_filter = v_filter;
	    } else if (v->uv_filter->vf_type != VFT_ACCEPT) {
		fatal("Cannot accept and deny");
	    } else {
		struct vf_element *p;

		p = v->uv_filter->vf_filter;
		while (p->vfe_next)
		    p = p->vfe_next;
		p->vfe_next = yystack.l_mark[0].filterelem;
	    }
	}
break;
case 58:
#line 496 "cfparse.y"
	{
	    warn("Expected filter spec after accept keyword, ignored");
	}
break;
case 59:
#line 500 "cfparse.y"
	{
	    if (!v->uv_filter) {
		struct vif_filter *v_filter;

		v_filter = (struct vif_filter *)malloc(sizeof(struct vif_filter));
		if (!v_filter) {
		    fatal("out of memory");
		    return 0;	/* Never reached */
		}

		v_filter->vf_flags = 0;
		v_filter->vf_type = VFT_DENY;
		v_filter->vf_filter = yystack.l_mark[0].filterelem;
		v->uv_filter = v_filter;
	    } else if (v->uv_filter->vf_type != VFT_DENY) {
		fatal("Cannot accept and deny");
	    } else {
		struct vf_element *p;

		p = v->uv_filter->vf_filter;
		while (p->vfe_next)
		    p = p->vfe_next;
		p->vfe_next = yystack.l_mark[0].filterelem;
	    }
	}
break;
case 60:
#line 526 "cfparse.y"
	{
		warn("Expected filter spec after deny keyword, ignored");
	}
break;
case 61:
#line 530 "cfparse.y"
	{
	    if (!v->uv_filter) {
		fatal("bidir goes after filters");
		return 0;	/* Never reached */
	    }
	    v->uv_filter->vf_flags |= VFF_BIDIR;
	}
break;
case 62:
#line 540 "cfparse.y"
	{
	    yyval.addr = yystack.l_mark[0].addr;
	}
break;
case 63:
#line 544 "cfparse.y"
	{
	    yyval.addr = valid_if(yystack.l_mark[0].ptr);
	    if (yyval.addr == 0 && !missingok)
		fatal("Invalid interface name %s",yystack.l_mark[0].ptr);
	}
break;
case 64:
#line 552 "cfparse.y"
	{
	    yyval.addr = yystack.l_mark[0].addr;
	}
break;
case 65:
#line 556 "cfparse.y"
	{
	    struct hostent *hp;

	    if ((hp = gethostbyname(yystack.l_mark[0].ptr)) == NULL || hp->h_length != sizeof(yyval.addr)) {
		fatal("No such host %s", yystack.l_mark[0].ptr);
		return 0;	/* Never reached */
	    }

	    if (hp->h_addr_list[1])
		fatal("Hostname %s does not %s", yystack.l_mark[0].ptr, "map to a unique address");

	    memmove (&yyval.addr, hp->h_addr_list[0], hp->h_length);
	}
break;
case 66:
#line 571 "cfparse.y"
	{
	    yyval.addrmask = yystack.l_mark[0].addrmask;
	}
break;
case 67:
#line 575 "cfparse.y"
	{
	    int i;

	    for (i=0; i < numbounds; i++) {
		if (!strcmp(boundlist[i].name, yystack.l_mark[0].ptr)) {
		    yyval.addrmask = boundlist[i].bound;
		    break;
		}
	    }

	    if (i == numbounds)
		fatal("Invalid boundary name %s",yystack.l_mark[0].ptr);
	}
break;
case 68:
#line 591 "cfparse.y"
	{
#ifdef ALLOW_BLACK_HOLES
	    if (!allow_black_holes)
#endif
		if ((ntohl(yystack.l_mark[0].addrmask.addr) & 0xff000000) != 0xef000000) {
		    fatal("Boundaries must be 239.x.x.x, not %s/%d",
			  inet_fmt(yystack.l_mark[0].addrmask.addr, s1, sizeof(s1)), yystack.l_mark[0].addrmask.mask);
		}
	    yyval.addrmask = yystack.l_mark[0].addrmask;
	}
break;
case 69:
#line 603 "cfparse.y"
	{ yyval.addrmask = yystack.l_mark[0].addrmask; }
break;
case 70:
#line 604 "cfparse.y"
	{ yyval.addrmask.addr = yystack.l_mark[0].addr; yyval.addrmask.mask = 0; }
break;
case 71:
#line 607 "cfparse.y"
	{ yyval.filterelem = yystack.l_mark[0].filterelem; }
break;
case 72:
#line 608 "cfparse.y"
	{ fatal("named filters no implemented yet"); }
break;
case 73:
#line 611 "cfparse.y"
	{ yyval.filterelem = yystack.l_mark[0].filterelem; }
break;
case 74:
#line 612 "cfparse.y"
	{ yystack.l_mark[-1].filterelem->vfe_next = yystack.l_mark[0].filterelem; yyval.filterelem = yystack.l_mark[-1].filterelem; }
break;
case 75:
#line 615 "cfparse.y"
	{ yyval.filterelem = yystack.l_mark[0].filterelem; }
break;
case 76:
#line 616 "cfparse.y"
	{ yystack.l_mark[-1].filterelem->vfe_flags |= VFEF_EXACT; yyval.filterelem = yystack.l_mark[-1].filterelem; }
break;
case 77:
#line 620 "cfparse.y"
	{
	    struct vf_element *vfe;

	    vfe = (struct vf_element *)malloc(sizeof(struct vf_element));
	    if (!vfe) {
		fatal("out of memory");
		return 0;	/* Never reached */
	    }

	    vfe->vfe_addr = yystack.l_mark[0].addrmask.addr;
	    VAL_TO_MASK(vfe->vfe_mask, yystack.l_mark[0].addrmask.mask);
	    vfe->vfe_flags = 0;
	    vfe->vfe_next = NULL;

	    yyval.filterelem = vfe;
	}
break;
#line 1642 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
