// CHAP - Hashmap implementation in C
// Copyright (c) 2018 Braden Hitchcock
#include "chap.h"
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <stdarg.h>

#include <stdio.h>

unsigned chap_hash(char *s, int cap) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval % cap;
}

char* chap_strdup(char *s) {
    char *p;
    p = (char *) calloc(strlen(s)+1, sizeof(char));
    if (p != NULL)
       strcpy(p, s);
    return p;
}

void chap_trace(const char* format, ...){
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

chap_map_t* chap_map_new(){
    chap_map_t* map = (chap_map_t*) calloc(1, sizeof(chap_map_t));
    map->table = (chap_entry_t**) calloc(CHAP_INITIAL_HASHSIZE, sizeof(chap_entry_t*));
    map->cap = CHAP_INITIAL_HASHSIZE;
    return map;
}

void chap_clear(chap_map_t *map){
    int i;
    for(i = 0; i < map->cap; i++){
        chap_entry_t *entry = map->table[i], *tmp;
        while(entry != NULL){
            tmp = entry;
            entry = entry->next;
            free(tmp->key);
            tmp->key = NULL;
            free(tmp->val);
            tmp->val = NULL;
            free(tmp);
        }
        map->table[i] = NULL;
    }
}

void chap_map_destroy(chap_map_t *map){
    // Free all the entries in the map
    chap_clear(map);
    // Free the table pointer
    free(map->table);
    // Free the map pointer
    free(map);
    map = NULL;
}

chap_entry_t* chap_insert(chap_map_t *map, char *key, char *val) {
    unsigned hashval;
    chap_entry_t *entry = (chap_entry_t*) calloc(1, sizeof(chap_entry_t));
    if (entry == NULL || (entry->key = strdup(key)) == NULL)
          return NULL;
    if ((entry->val = strdup(val)) == NULL) {
       return NULL;
    }
    hashval = chap_hash(key, map->cap);
    entry->next = map->table[hashval];
    map->table[hashval] = entry;
    return entry;
}

int chap_put(chap_map_t *map, char *key, char *val) {
    chap_entry_t *entry;
    // Attempt to find the key in the table. If it isn't there, we need to add a new entry. If it is, we will
    // update the existing entry's value
    if ((entry = chap_find(map, key)) == NULL) {
        // Entry was not found. Insert a new pair
        entry = chap_insert(map, key, val);
        if(entry == NULL){
            return -1;
        }
    } else {
        // Entry was found. Update its value
        free(entry->val);
        if ((entry->val = strdup(val)) == NULL) {
            return -1;
        }
    }
    return 0;
}

chap_entry_t* chap_find(chap_map_t *map, char *key){
    chap_entry_t *entry;
    for (entry = map->table[chap_hash(key, map->cap)]; entry != NULL; entry = entry->next){
        if (strcmp(key, entry->key) == 0){
            return entry;
        }
    }
    return NULL;
}

char* chap_get(chap_map_t *map, char *key){
    chap_entry_t *entry = chap_find(map, key);
    if(entry != NULL){
        return entry->val;
    }
    return NULL;
}

char* chap_get_default(chap_map_t *map, char *key, char *def){
    chap_entry_t *entry = chap_find(map,key);
    if(entry == NULL){
        entry = chap_insert(map, key, def);
    }
    return entry->val;
}

int chap_get_int(chap_map_t *map, char *key, int *val) {
    char* s = chap_get(map, key);
    if(s == NULL){
        return -1;
    }
    intmax_t num = strtoimax(s, NULL, 10);
    if(num == INTMAX_MAX && errno == ERANGE){
        return -1;
    }
    *val = (int)num;
    return 0;
}