/*
@author Christian Merchant
@project: CSCE531 Project 1
@date: 2015-02-13
@version: 1.0
@file: Header File 
Header for Lexical analyzer
*/

#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>

// Change this to 1 for debugging output (hitting newline to continue)
#define DEBUG 0

#define debug(s)      if (DEBUG) fprintf(stderr,s),getchar()
#define debug1(s,t)   if (DEBUG) fprintf(stderr,s,t),getchar()
#define debug2(s,t,u) if (DEBUG) fprintf(stderr,s,t,u),getchar()

typedef int boolean;
#define FALSE 0
#define TRUE 1

typedef enum {
    INT_CONST, STR_CONST, ID 
} VAL_TYPE;

typedef struct dr 
{
    struct dr *next;
    boolean in_cycle;
    const char *key;
    VAL_TYPE tag;
    union 
    {
        long intconstval;
        const char *strconstval;
        const char *idval;
    } u;
}DICT_REC, *DR;

extern int line_no;

void init_lex(FILE *stream);
void init_dict();
DR get_item(const char *key);
void add_int_to_dict(const char *key, long val);
void add_str_to_dict(const char *key, const char *val);
void add_id_to_dict(const char *key, const char *val);
void output_substitution(FILE *stream, const char *id);

#endif
