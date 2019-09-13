/* Ryan Bruno */
#ifndef STREAM_H
#define STREAM_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*stream_callback)(void** cb_data);

int stream_first(FILE *file_ptr, char* buffer_r,
        char split_token, char stop_token);
void stream_each(FILE *file_ptr, char* buffer,
        char split_token, char stop_token,
        stream_callback cb, void* cb_data);

#endif
