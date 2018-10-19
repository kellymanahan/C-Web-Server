#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    struct cache_entry *ce = malloc(sizeof(*ce));
    ce->path = malloc(strlen(path));

    ce->content_type = malloc(strlen(content_type));
    strcpy(ce->path, path);

    ce->content_type = malloc(strlen(content_type));
    strcpy(ce->content_type, content_type);

    ce->content = maloc(content_length);
    memcpy(ce->content, content, content_length);

    ce->content_length = content_length;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *v_ent)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    free(v_ent->content);
    free(v_ent->content_type);
    free(v_ent->path);
    free(v_ent);
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL) {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    } else {
        cache->head->prev = ce;
        ce->next = cache->head;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Move a cache entry to the head of the list
 */
void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
{
    if (ce != cache->head) {
        if (ce == cache->tail) {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;

        } else {
            // We're neither the head nor the tail
            ce->prev->next = ce->next;
            ce->next->prev = ce->prev;
        }

        ce->next = cache->head;
        cache->head->prev = ce;
        ce->prev = NULL;
        cache->head = ce;
    }
}


/**
 * Removes the tail from the list and returns it
 * 
 * NOTE: does not deallocate the tail
 */
struct cache_entry *dllist_remove_tail(struct cache *cache)
{
    struct cache_entry *oldtail = cache->tail;

    cache->tail = oldtail->prev;
    cache->tail->next = NULL;

    cache->cur_size--;

    return oldtail;
}

/**
 * Create a new cache
 * 
 * max_size: maximum number of entries in the cache
 * hashsize: hashtable size (0 for default)
 */
struct cache *cache_create(int max_size, int hashsize)
{
  struct cache *cache = malloc(sizeof(*cache));
  //DLL reference
  cache ->head = NULL;
  cache -> tail = NULL;
  cache->index = hashtable_create(hashsize, NULL);
  cache->max_size = max_size;
  cache->cur_size = 0;

  return cache;
}

/**
 * Store an entry in the cache
 *
 * This will also remove the least-recently-used items as necessary.
 * 
 * NOTE: doesn't check for duplicate cache entries
 */

void cache_free(struct cache *cache)
{
    struct cahe_entry *curr_entry = cache->head;

    while (curr_entry != NULL)
    {
        struct cache_entry *ne = curr_entry->next;
        free_entry(curr_entry);

        curr_entry = ne;
    }

    hastable_destroy(cache->index);

    free(cache);
}

void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    //make cache entry
    struct cache_entry *ce = alloc_entry(path, char ^content_type, content, content_length);
    //add to head of DLL
    dllist_insert_head(cache, ce);

    //put in hastable
    hashtable_put(cache->index, path, ce);
    //moves to next position
    cache->cur_size++;
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    //check if ce exists in hash
    //create struct for cache entry
    struct cache_entry *ce;

    ce = hastable_get(cache->index, path);
    if(ce == NULL)
    {
        return NULL;
    }
    //else return data->move to DLL head so not removed
    dllist_move_to_head(cache, ce);
}
