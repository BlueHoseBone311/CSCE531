
/*
@name Christian Merchant
@class: Compilers (CSCE 531)
@version 2015-02-13 (c)
A hash table "dictionary" implementation
*/
#include <stdlib.h>
#include <string.h>
#include "defs.h"

#define INITIAL_HASH_SIZE 8
#define MAX_LOAD_FACTOR 2
#define SCALE_FACTOR 2

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
    DR p = (DR) malloc(sizeof(DICT_REC));
    p->in_cycle = FALSE;
    p->key = key;
    p->tag = INT_CONST;
    p->u.intconstval = val;
    if (insert_or_update(p) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %ld at line %d\n",
                key, val, line_no);
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
    if (insert_or_update(entry) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %s at line %d\n",key, val, line_no);
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
    if (insert_or_update(entry) == 0)
    {    
        fprintf(stderr, "Warning: redefinition of %s to %s at line %d\n",key, val, line_no);
    }    
}

// Output the substituted value of a defined ID to the output stream
void output_substitution(FILE *stream, const char *id)
{
    DR item = get_item(id);
    while (item != NULL && item->in_cycle == FALSE && item->tag == 2) 
    {
	item = get_item(item->u.idval);
    }
    if (item->in_cycle || item == NULL)
    {
	fprintf(stream, "%s", id);
    }
    else
    {    
        switch(item->tag)
        {
            case 0: 
                fprintf(stream,"%ld", item->u.intconstval);
                break; 
            case 1:
                fprintf(stream,"%s", item->u.strconstval);
                break; 
            case 2:
                fprintf(stream,"%s", item->u.idval);
                break;    
        } 
   }
}


/* Local routines */

/* Returns NULL if item not found */
static DR get_item(const char *key)
{
    int index = hash(key);
    DR p = hash_tab[index];
    debug1("get_item: p %s NULL\n", p==NULL?"==":"!=");
    //printf("key:%s",key);
    while (p!=NULL && strcmp(key,p->key))
    {    
        p = p->next;
    } 
       
    return p;
}

/* Returns the number of new items inserted: 1 if new insertion; 0 otherwise */
static int insert_or_update(DR new_item)
{
    int ret;
    const char *key = new_item->key;
    int index = hash(key);
    DR p = hash_tab[index], *prev = hash_tab+index;
    
    debug1("insert_or_update(key=%s) called\n", new_item->key);
    while (p!=NULL && strcmp(key,p->key)) 
    {
        prev = &p->next;
        p = p->next;
    }

    if (p == NULL) 
    {
            /* Insertion */
        debug1("  p is NULL -- inserting at index %d\n", index);
        insert_at_front(prev, new_item);
        debug("    done\n");
        if (num_items++ > MAX_LOAD_FACTOR*h_size) 
        {
            debug("    resizing\n");
            resize(SCALE_FACTOR*h_size);
        }
        ret = 1;
    }
    else 
    {
            /* Update */
        debug1("  p is not NULL -- updating at index %d\n", index);
        unmark_cycle(p);
        free(remove_from_front(prev));
        insert_at_front(prev, new_item);
        ret = 0;
    }

    mark_cycle(new_item);
    debug("    cycle marked\n");
    return ret;
}

// Marks the new cycle, if there is one
static void mark_cycle(DR item)
{
   DR p = item;
   DR temp = NULL; 
    while (p->tag == 2 && !p->in_cycle && (temp = get_item(p->u.idval)) != NULL && temp !=item)
    {
	p = temp;
        printf("key: %s item key %s\n", p->key, item->key); 
    }
    if (temp == item)
    {
	for (; item->in_cycle != TRUE; item = get_item(item->u.idval))
	{
		item->in_cycle = TRUE;
	}
    }

}

// Unmark an existing cycle
static void unmark_cycle(DR item)
{
   DR temp = item; 
   while (temp != NULL)
   { 
      if (temp->in_cycle == TRUE)
      {
	temp ->in_cycle = FALSE;
	if (temp->tag == 2)
	{	
		temp = get_item(temp->u.idval);
	}
      }
       
       if (!temp->in_cycle || temp->tag ==1 || temp->tag ==0)
       {
         break;
        }
	
     }
 }

static int hash(const char *key)
{
    int sum = 0;
    for (; *key; key++)
        sum = (37*sum + *key) % h_size;
    return sum;
}

static void insert_at_front(DR *list, DR new_item)
{
    new_item->next = *list;
    *list = new_item;
}

static DR remove_from_front(DR *list)
{
    DR ret = *list;
    *list = (*list)->next;
    return ret;
}

static void resize(int size)
{
    int i;
    
    DR *temp = hash_tab;
    int temp_size = h_size;

    h_size = size;
    hash_tab = (DR *) malloc(h_size * sizeof(DR));
    for (i=0; i<h_size; i++)
    {    
        hash_tab[i] = NULL;
    }    

        // This only occurs on the initial sizing, with empty dictionary
    if (temp == NULL)
        return;
    
    for (i=0; i<temp_size; i++)
        while (temp[i] != NULL) 
        {
            int index = hash(temp[i]->key);
            insert_at_front(hash_tab+index, remove_from_front(temp+i));
        }
    free(temp);
}
