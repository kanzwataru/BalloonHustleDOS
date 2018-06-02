#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "src/hustle~1/types.h"

typedef struct {
    void (*update_callback)(void);
    void (*render_callback)(void);
    bool (*input_handler)(void);
    void (*exit_handler)(void);
} CoreData;

void engine_start(CoreData cd);

#endif