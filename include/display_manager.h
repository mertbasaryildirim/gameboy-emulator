#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <stdint.h>

int dm_init(const char *title, int scale);
void dm_shutdown(void);
int dm_handle_events(void);
void dm_present(const uint32_t *framebuffer);

#endif