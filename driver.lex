/*
@author Christian Merchant
@project: CSCE531 Project 1
@date: 2015-02-13
@version: 1.0
@file: driver (lex file) 
*/

%{
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
%}

digit           [0-9]
int             {digit}+
directive       "#""define"[ \t]+
identifier      [_A-Za-z][_A-Za-z0-9]*
val_int			[+-]?{int}
val_str			\".*\"

/* internal flag */
	int define_flag = FALSE;
	int key_flag = FALSE;

/* key match storage */				
	char * key_str;
	char* val_string;

	int line_no = 1;
%%
{directive}   	{
					define_flag = TRUE; 
				}  
{identifier}	{ 
			        if (define_flag && !key_flag) /*key identifier*/
			        {
				   	 key_str = (char*)malloc(strlen(yytext)+1);
				   	 strcpy(key_str,yytext); 	
					 
					 key_flag = TRUE; 
					}
					else if (define_flag && key_flag) /*value identifier*/
					{
						add_id_to_dict(key_str, yytext);  
						define_flag = FALSE;
						key_flag = FALSE;
					}
					else
					{	
						output_substitution(yyout,yytext); 
					}
				 }					    
{val_int}		{
					if (define_flag && key_flag)
					{
						add_int_to_dict(key_str, atol(yytext));
						define_flag = FALSE;
						key_flag = FALSE;
					}
				}

{val_str}		{
					if (define_flag && key_flag)
					{
						val_string = (char*)malloc(strlen(yytext)-1);
						strncpy(val_string, &(yytext[1]), strlen(yytext)-2);
						val_string[strlen(yytext)-2] = (char) 0;
						add_str_to_dict(key_str, val_string);
						define_flag = FALSE;
						key_flag = FALSE; 
					}
				}	
\n 				++line_no;					
%%
main( int argc, char **argv )
{ 
    ++argv, --argc;     /* skip over program name */
    if ( argc > 0 )
    {	
        yyin = fopen( argv[0], "r" );
    }
    else
    {	
        yyin = stdin;
    }
    init_dict();
    yylex();
}
