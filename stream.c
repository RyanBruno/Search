/* Ryan Bruno */
#include "stream.h"

int stream_first(FILE *file_ptr, char* buffer_r,
        char split_token, char stop_token)
{
    char c;

    buffer_r[0] = '\0';
    while ((c = getc(file_ptr)) != EOF) {

        if (c == split_token) {
            return 1;               // TODO fix returns
        }

        if (c == stop_token) {
            return 0;           // TODO fix returns
        }
        if (isdigit(c) || c == '.')
            strncat(buffer_r, &c, 1);
    }
    return 0;
}

void stream_each(FILE *file_ptr, char* buffer,
        char split_token, char stop_token,
        stream_callback cb, void* cb_data)
{
    while (stream_first(file_ptr, buffer, split_token, stop_token) != 0) {
        if (cb != NULL) {
            cb(cb_data);
        }
    }
}
