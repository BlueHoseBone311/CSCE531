/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
#include "defs.h"
%}

digit           [0-9]
int             {digit}+
real            {int}"."{int}([eE][+-]?{int})?
whtspc          [ \t] 
ident           "#"{whtspc}*"define"{whtspc}+
key             [A-Za-z{digit}]+
val				

flag			0

%%

{int}            printf( "An integer: %s (%d)\n", yytext, atoi( yytext ) );

{real}           printf( "A real: %s (%g)\n", yytext, atof( yytext ) );

if|then|begin|end|procedure|function        {
                     printf( "A keyword: %s\n", yytext );
                 }

{ident}          


"{"[^}\n]*"}"    /* eat up one-line comments */

[ \t\n]+         /* eat up whitespace */

.                printf( "Unrecognized character: %s\n", yytext );

%%
