#pragma once
#include "hustle.h"

#define NULL_ENTITY 65535
typedef uint16_t entity_id;

static inline void entity_reserve(byte *roster, entity_id id)
{
    assert(id != NULL_ENTITY);
    assert(!roster[id]);
    roster[id] = 1;
}

static inline void entity_free(byte *roster, entity_id id)
{
    assert(id != NULL_ENTITY);
    assert(roster[id]);
    roster[id] = 0;
}

//TODO: make bulk version
static entity_id entity_find_available(const byte *roster, entity_id first, entity_id last)
{
    for(entity_id id = first; id < last; ++id) {
        if(!roster[id]) {
            return id;
        }
    }

    return NULL_ENTITY;
}
