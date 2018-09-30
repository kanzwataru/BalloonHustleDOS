#ifndef EVENT_H
#define EVENT_H

#include "src/hustle~1/platform.h"

#define MAX_EVENTS 64
typedef unsigned char EventID;
typedef void (*EventCallback)(void *);

void event_update(void);
EventID event_add(EventCallback callback, void *arg, uint time_amount);
void event_remove(EventID id);

#endif