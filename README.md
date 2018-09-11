# chap
Lightweight C implementation of hashmap for maps of arbitrary data size

## Compilation

```text
make all
```

Eventually there will be an install in the make file that will place it in the appropriate system library and include directory.

## Example Usage

See the [header file](./src/chap.h) to get detailed descriptions for all the functions in the chap library.

**main.c**
```c
#include <chap.h>

int main(int argc, char const *argv[])
{
    // Create a new map
    chap_map_t *map = chap_map_new();

    // Put a key/value pair into the map
    chap_put(map, "key", "value");

    // Get a key/value pair from the map
    char *sval = chap_get(map, "key");

    // Get an integer value from the map
    chap_put(map, "number", "42");
    int ival;
    chap_get_int(map, "number", &ival);

    // Provide a default to insert into the map if the value is not found when
    // performing the get
    char *dval = chap_get_default(map, "dkey", "default");

    // Remove all entries from the map
    chap_clear(map);

    // Destroy the map to prevent memory leaks
    chap_map_destroy(map);

    return 0;
}
```

```text
gcc -o out main.c -lchap
```