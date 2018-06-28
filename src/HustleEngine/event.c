#include "src/hustle~1/event.h"

struct Event {
    EventCallback  callback;
    void          *arg;
    uint           timer;
};

static struct Event event_stack[MAX_EVENTS];
static EventID top;

EventID event_add(EventCallback callback, void *arg, uint time_amount)
{
    struct Event e;
    e.callback = callback;
    e.arg = arg;
    e.timer = time_amount;

    event_stack[++top] = e;

    return top;
}

void event_remove(EventID id)
{
    memset(event_stack + id, 0, sizeof(struct Event));
}

void event_update(void)
{
    int i;

    for(i = 0; i <= top; ++i) {
        if(event_stack[i].timer-- == 0)
            if(event_stack[i].callback)
                event_stack[i].callback(event_stack[i].arg);
    }
}
