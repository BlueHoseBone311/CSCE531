/* Dictionary implementation */

#include <stdlib.h>
#include <string.h>
#include "defs.h"

#define INITIAL_HASH_SIZE 8
#define MAX_LOAD_FACTOR 0.75
#define SCALE_FACTOR 4

static DR get_item(const char *key);
static int insert_or_update(DR new_item);
static void mark_cycle(DR item);
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
    entry->in_cycle = FALSE;
    entry->key = key;
    entry->tag = INT_CONST;
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
    char *id_val = ID; 
    DR *curr_item;

    if ((curr_item = get_item(new_item->key)) != NULL) /*a key exists that is the same as the new item - update data*/
    {
        if (curr_item->incycle)
        {
            unmark_cycle(curr_item);
        }    
        cur_item->tag = new_item->tag
        cur_item->u = new_item->u;
        if (strcmp(curr_item->tag,id_val) == 0)
        {
            check_or_mark_cycle(curr_item);  
        } 
    }
    else if (table[index] = NULL) /*this bucket has not been hashed to - fill it */ 
    {
       int hashval = hash(index);
       new_item->next = table[hashval];
       table[hashval] = new_item;
       if (strcmp(new_item->tag,id_val) == 0)
       {
            check_or_mark_cycle(new_item);  
       }
       status = 1; 
    }
    else /*a key exists but it is not the same as the new_item key - add to end*/
    {
        DR *ptr = table[index];
        while(ptr->next != NULL)
        {
            ptr = ptr->next; 
        } 
        ptr->next = new_item;
        if (strcmp(curr_item->tag,id_val) == 0)
        {
            check_or_mark_cycle(curr_item);  
        }
        status = 1;     
    }

    return status;   
}
boolean check_cycle (DR item, const char* key)
{
    char * int_val = INT_CONST;
    char * str_val = STR_CONST; 
     
    if (strcmp(item->tag, int_val) == 0 || strcmp(item->tag, str_val) == 0)|| item->in_cycle)
    {
        return FALSE;
    } 
    if (strcmp(item->u.idval, key))
    {
        return TRUE; 
    }
    DR *next_item;  
    next_item = item->u.idval;
    check_cycle(next_item,key);

}
void check_or_mark_cycle(DR item);
void unmark_cycle(DR item);
int hash(const char *key)
{
    int sum = 0;
    for (; *key; key++)
    {    
        sum = (37*sum + *key) % h_size;
    }
        
    return sum;
}
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
void resize(int size)
{ 
    int temp = h_size; 
    h_size = size;
    DR *new_table = (DR *) malloc(h_size*sizeof(DICT_REC)); 
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
        while(int i < temp)
        {
            DR *entry;
            for (entry = hash_tab[i]; entry != NULL; entry = entry->next)
            {   
                insert_or_update(entry, new_table);
            }       
        } 
        free_table(hash_tab, temp);
        hash_tab = new_table; 
    }       
}
void free_table(DR *hashtable, int tablesize)
{
    int i;
    DR *entry, *temp;

    if (hashtable==NULL) return;

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