/* Dictionary implementation */

#include <stdlib.h>
#include <string.h>
#include "defs.h"

#define INITIAL_HASH_SIZE 8
#define MAX_LOAD_FACTOR 0.75
#define SCALE_FACTOR 4

static DR get_item(const char *key);
static int insert_or_update(DR new_item);
static void check_or_mark_cycle (DR item);
static void unmark_cycle(DR item);
static int hash(const char *key);
static void insert_at_front(DR *list, DR new_item);
static DR remove_from_front(DR *list);
static void resize(int size);

int h_size;
int num_items;
DR *hash_tab;

void init_dict()
{
    h_size = 0;
    hash_tab = NULL;
    resize(INITIAL_HASH_SIZE);
    num_items = 0;
}

// Add a key with an integer constant value to the dictionary
void add_int_to_dict(const char *key, long val)
{
    DR entry = (DR) malloc(sizeof(DICT_REC));
    entry->in_cycle = FALSE;
    entry->key = key;
    entry->tag = INT_CONST;
    entry->u.intconstval = val;
    if (insert_or_update(entry, hash_tab) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %ld at line %d\n",key, val, line_no);
    }  
    else
    {
        ++num_items;
        if (num_items/h_size>MAX_LOAD_FACTOR)
       {
            resize(h_size*SCALE_FACTOR);
       }  
    }  
}

// Add a key with a string constant value to the dictionary
void add_str_to_dict(const char *key, const char *val)
{
    DR entry = (DR) malloc(sizeof(DICT_REC));
    entry->in_cycle = FALSE;
    entry->key = key;
    entry->tag = STR_CONST;
    entry->u.strconstval = val;
    if (insert_or_update(entry, hash_tab) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %ld at line %d\n",key, val, line_no);
    }
    else
    {
        ++num_items; 
        if (num_items/h_size>MAX_LOAD_FACTOR)
       {
            resize(h_size*SCALE_FACTOR);
       }  
    }    
}

// Add a key with an identifier value to the dictionary
void add_id_to_dict(const char *key, const char *val)
{
    DR entry = (DR) malloc(sizeof(DICT_REC));
    entry->in_cycle = FALSE; /*marked approprately once added*/
    entry->key = key;
    entry->tag = ID;
    entry->u.idval = val;
    if (insert_or_update(entry, hash_tab) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %ld at line %d\n",key, val, line_no);
    }    
    else
    {
        ++num_items; 
        if ((num_items/h_size)>MAX_LOAD_FACTOR)
       {
            resize(h_size*SCALE_FACTOR);
       }  
    }    
}

// Output the substituted value of a defined ID to the output stream
void output_substitution(FILE *stream, const char *id)
{
        // redacted
}


/* Local routines */
DR get_item(const char *key)
{   
    int index = hash(key);
    DR *entry;
    for (entry = hash_tab[index]; entry != NULL; entry = entry->next)
    {
        if (strcmp(key, entry->key) == 0)
        {
            return entry; 
        }       
    }    
    return NULL; 
}

int insert_or_update(DR new_item, DR *table);
{
    int status = 0;
    int index = hash(new_item->key);
    int id_val = 2; 
    DR *curr_item;

    if ((curr_item = get_item(new_item->key)) != NULL) /*a key exists that is the same as the new item - update data*/
    {
        if (curr_item->in_cycle)
        {
            unmark_cycle(curr_item, curr_item->key);
        }    
        curr_item->tag = new_item->tag
        switch(curr_item->tag)
        {
        case 0:
            curr_item->u.intconstval = new_item->u.intconstval;
            break;
        case 1:
            curr_item->u.strconstval = new_item->u.strconstval;
            break;
        case 2:
            curr_item->u.idtag = new_item->u.idval; 
            break;
        }
        if (curr_item->tag == id_val)
        {
            check_or_mark_cycle(curr_item);  
        } 
    }
    else if (table[index] == NULL) /*this bucket has not been hashed to - fill it */ 
    {
       int hashval = hash(index);
       new_item->next = table[hashval];
       table[hashval] = new_item;
       if (new_item->tag == id_val)
       {
            check_or_mark_cycle(new_item);  
       }
       status = 1; 
    }
    else /*a key exists but it is not the same as the new_item key - add to end*/
    {
        DR entry = table[index];
        while(entry->next != NULL)
        {
            entry = entry->next; 
        } 
        ptr->next = new_item;
        if (ptr->tag == id_val)
        {
            check_or_mark_cycle(curr_item);  
        }
        status = 1;     
    }

    return status;   
}

void check_or_mark_cycle(DR item, const char *key)
{
    int int_val = 0;
    int str_val = 1; 
     
    if (item->tag == int_val || item->tag == str_val || item->in_cycle)
    {
        return;
    } 
    if (strcmp(item->u.idval, key) == 0)
    {
       item = item->u.idval;  
	   while (strcmp(item->u.idval,key) != 0)
       { 
           item->in_cycle = TRUE;
    	   item = item->u.idval;
       }
       item->in_cycle = TRUE;    		
       return; 
    }
      
    item = item->u.idval;
    check_or_mark_cycle(item,key);
}

void unmark_cycle(DR item, const char *key);
{  
    while(strcmp(item->u.idval, key) != 0)
    {
	item->in_cycle = FALSE;
	item = item->u.idval; 		     	
    }	
}

int hash(const char *key)
{
    int sum = 0;
    for (; *key; key++)
    {    
        sum = (37*sum + *key) % h_size;
    }      
    return sum;
}

#ifdef
void insert_at_front(DR *list, DR new_item)
{
    new_item-> next = *list; 
    *list = new_item; 
}
DR remove_from_front(DR *list)
{
    DR entry = *list; 
    *list = *list->next; 
    return entry; 
}
#endif

void resize(int new_size)
{ 
    int old_size = h_size; 
    h_size = new_size;
    DR *new_table = (DR *) malloc(h_size*sizeof(DR)); 
    for (int i = 0; i <h_size; i++)
    {
        new_table[i] = NULL;
    }

    if (hash_tab == NULL) /*initial dictionary*/
    {    
        hash_tab = new_table;    
    } 
    else
    {
        for(int i = 0; i < old_size; i++)
        {
            DR *entry;
            for (entry = hash_tab[i]; entry != NULL; entry = entry->next)
            {   
                insert_or_update(entry, new_table);
            }       
        } 
        free(hash_tab);
        hash_tab = new_table; 
    }  
}
#ifdef
void free_table(DR *hashtable, int tablesize)
{
    int i;
    DR *entry, *temp;

    if (hashtable==NULL) 
    {    
        return;
    }   

    for(i=0; i<tablesize; i++) 
    {
        entry = hashtable[i];
        
        while(entry!=NULL) {
            temp = entry;
            entry = entry->next;
            free(temp->in_cycle);
            free(temp->key);
            free(temp->tag);
            free(temp->u);
            free (temp);
        }
    }
    free(hashtable);
}
#endif
