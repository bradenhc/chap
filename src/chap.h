#include <stdlib.h>

/// The initial size of the hash map
#define CHAP_INITIAL_HASHSIZE 64

/// A single entry in the chap table, represented as a key/value pair
typedef struct chap_entry_t {
    struct chap_entry_t *next;
    char *key;
    char *val;
} chap_entry_t;

/// The data structure that holds our map.
typedef struct chap_map_t {
    chap_entry_t **table;
    int cap;
} chap_map_t;

/// chap_hash will create the hash value for the string key to insert into the table. This is not a collision
/// resistant hash, but chap will account for collisions by adding an offset to the key. If the offset collides
/// with too many existing entries, the table will be reallocated and all values will be rehashed with the new
/// value
unsigned chap_hash(char* s, int size);

/// chap_strdup creates a duplicate of the provided string
char* chap_strdup(char *s);

/// chap_map_new creates a new map and returns the allocated map to the caller.
chap_map_t* chap_map_new();

/// chap_clear will remove all entries in the map and free all key/value pairs, resetting the size of the hashmap
/// to the default size of map created by chap_map_destroy
void chap_clear(chap_map_t *map);

/// chap_map_destroy destroys an existing map
void chap_map_destroy(chap_map_t* map);

/// chap_insert will insert the key/value pair into the map. WARNING: this does not check to see if the value already
/// exists. It assumes the value does not exist and simply writes the new value. If chap_insert is used to update an
/// entry in the map, a memory-leak will occur as the previous value will not be freed.
chap_entry_t* chap_insert(chap_map_t *map, char *key, char *val);

/// chap_put places a value inside the map. If the value doesn't exist, it will be added to the map. If it does exist, 
/// the value will be updated.
int chap_put(chap_map_t *map, char *key, char *val);

/// chap_find locates and retrieves an entry in the map if it exists. If it doesn't exist, this function will return
/// NULL.
chap_entry_t* chap_find(chap_map_t *map, char *key);

/// chap_get retrieves a value from and entry in the map with the associated key. If the value doesn't exist, this 
/// function will return NULL.
char* chap_get(chap_map_t *map, char *key);

/// chap_get_default attempts to retrieve the value in the map with the associated key. If no entry is found, it
/// will insert the default value into the map and return it.
char* chap_get_default(chap_map_t *map, char *key, char *def);

/// chap_get_int attempts to retrieve the value with the associated key from the map and convert it into an integer
/// value. The resulting integer value will be stored in the 'val' pointer. If the function finds 
/// the entry and successfully converts to an int, it will return 0. If an entry with the key was not found, or if 
/// the conversion failed,the function will  return -1 and the value of 'val' will remain unchanged.
int chap_get_int(chap_map_t *map, char *key, int *val);