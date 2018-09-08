// CHAP - Hashmap implementation in C
// Copyright (c) 2018 Braden Hitchcock
#include "chap.h"
#include <string.h>

unsigned chap_hash(char *s, int cap) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval % cap;
}

chap_map_t* chap_map_new(){
    chap_map_t* map = (chap_map_t*) malloc(sizeof(chap_map_t));
    map->table = (chap_entry_t**) malloc(CHAP_INITIAL_HASHSIZE * sizeof(chap_entry_t*));
    map->cap = CHAP_INITIAL_HASHSIZE;
    return map;
}

void chap_map_destroy(chap_map_t *map){
    // Free all the entries in the map
    int i;
    for(i = 0; i < map->cap; i++){
        chap_entry_t *entry = map->table[i];
        chap_entry_t *last;
        if(entry != NULL){
            do{
                last = entry;
                entry = entry->next;
                free(last->key);
                free(last->val);
                free(last);
            } while(entry != NULL);
        }
    }
    // Free the table pointer
    free(map->table);
    // Free the map pointer
    free(map);
    map = NULL;
}

int chap_put(chap_map_t *map, char *key, char *val) {
    chap_entry_t *entry;
    unsigned hashval;
    if ((entry = chap_find(map, key)) == NULL) {
        // Entry was not found. Hash the key
        entry = (chap_entry_t*) malloc(sizeof(chap_entry_t));
        if (entry == NULL || (entry->key = strdup(key)) == NULL)
          return -1;
        hashval = chap_hash(key, map->cap);
        entry->next = map->table[hashval];
        map->table[hashval] = entry;
    } else {
        free(entry->val);
    }
    if ((entry->val = strdup(val)) == NULL) {
       return -1;
    }
    return 0;
}

char* chap_get(chap_map_t *map, char *key){
    chap_entry_t *entry = chap_find(map, key);
    if(entry != NULL){
        return entry->val;
    }
    return NULL;
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

char* chap_strdup(char *s) {
    char *p;
    p = (char *) malloc(strlen(s)+1);
    if (p != NULL)
       strcpy(p, s);
    return p;
}
