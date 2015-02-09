%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
%}

digit           [0-9]
int             {digit}+
directive       "#"[ \t]*"define"[ \t]+
identifier      [_A-Za-z][_A-Za-z0-9]*
val_int			[+-]?{int}
val_str			\"[ \t]*[_A-Za-z0-9]+[ \t]*\"

/* internal flag */
	int define_flag = FALSE;
	int key_flag = FALSE;

/* key match storage */				
	char * key_str;
	char* val_string;

	line_no = 0;
%%
\n 				  ++line_no;
{directive}   	define_flag = TRUE; printf("A directive has been discovered on line %d:\n", line_no);  

{val_int}		{
					if (define_flag == TRUE && key_flag == TRUE)
					{
						printf("An integer constant: %s\n", yytext);
						define_flag = FALSE;
						key_flag = FALSE;
					}
				}

{val_str}		{
					if (define_flag == TRUE && key_flag == TRUE)
					{
						val_string = (char*)malloc(strlen(yytext)-1);
						strncpy(val_string, &(yytext[1]), strlen(yytext)-2);
						val_string[strlen(yytext)-2] = (char) 0;
						printf("A string constant: %s on line %d\n", val_string, line_no);
						define_flag = FALSE;
						key_flag = FALSE; 
					}
				}

{identifier}	{ 
				   if (define_flag == TRUE && key_flag == FALSE)
				   {
				   key_str = (char*)malloc(strlen(yytext)+1);
				   strcpy(key_str,yytext); 	
				   
					printf( "An identifier: %s on line: %d\n", key_str,line_no);
					key_flag == TRUE; 
					}
					if (define_flag == TRUE && key_flag == TRUE)
					{
						printf("An identifier value: %s on line %d\n" ,yytext,line_no);
						define_flag = FALSE;
						key_flag = FALSE;
					}
				 }

"{"[^}\n]*"}"    /* eat up one-line comments */

[ \t\n]+         /*consume whitespace*/

.              /*consume anything else*/  

%%
main( int argc, char **argv )
{
    ++argv, --argc;     /* skip over program name */
    if ( argc > 0 )
        yyin = fopen( argv[0], "r" );
    else
        yyin = stdin;

    yylex();
}
