#include <stdlib.h>

/// The initial size of the hash map
#define CHAP_INITIAL_HASHSIZE 64

/// 
#define CHAP_OFFSET_TOLERANCE 10

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

/// chap_map_destroy destroys an existing map
void chap_map_destroy();

/// chap_put places a value inside the map. If the value doesn't exist, it will be added to the map. If it does exist, the
/// value will be updated.
int chap_put(chap_map_t *map, char *key, char *val);

/// chap_get retrieves a value from the map. If the value doesn't exist, this function will return NULL.
char* chap_get(chap_map_t *map, char *key);

chap_entry_t* chap_find(chap_map_t *map, char *key);


char* chap_get_default(chap_map_t *map, char *key, char *def);

