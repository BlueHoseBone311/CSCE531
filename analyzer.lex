%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
%}

digit           [0-9]
int             {digit}+
string			[_A-Za-z0-9]
whtspc          [ \t] 
def_dir         "#"{whtspc}*"define"{whtspc}+
key             {string}+
val_int			[+-]?{int}
val_str			\"{whtspc}*{string}+{whtspc}*\"
val_id			{key}

/* internal flag */
	int def_flag = FALSE;
/* key match storage */				
	char * key_str;
	char* val_string;

	line_no = 0;
%%

{def_dir}   	def_flag = TRUE;       

{key}			{ 
				   if (def_flag == TRUE)
				   {
				   key_str = (char*)malloc(strlen(yytext)+1);
				   strcpy(key_str,yytext); 	
				   }
					printf( "An identifier: %s on line: %d\n", key_str,line_no);
				}

{val_int}		{
					if (def_flag == TRUE)
					{
						printf("An integer constant: %s\n", yytext);
						def_flag = FALSE;
					}
				}

{val_str}		{
					if (def_flag == TRUE)
					{
						val_string = (char*)malloc(strlen(yytext)-1)
						strncpy(val_string, &(yytext[1]), strlen(yytext)-2);
						val_string[strlen(yytext)-2] = (char) 0;
						printf("A string constant: %s on line %d\n", val_string, line_no);
						def_flag = FALSE;
					}
				}
{val_id}		{
					if (def_flag == TRUE)
					{
						printf("An identifier value: %s on line %d\n" ,yytext,line_no);
						def_flag = FALSE;
					}
				}							

"{"[^}\n]*"}"    /* eat up one-line comments */

[ \t\n]+         ++line_no;

.                

%%
