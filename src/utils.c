/*
 * Copyright 2015 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include <string.h>
#include "parse.h"
#include "utils.h"

char *str_utils_dup(const char *str)
{
    char *ret = NULL;
    if(str) {
        size_t len = strlen(str);
        ret = hls_malloc(len + 1);
        strncpy(ret, str, len + 1);
    }
    return ret;
}

char *str_utils_ndup(const char *str, size_t size)
{
    char *ret = NULL;
    if(str) {
        ret = hls_malloc(size + 1);
        memcpy(ret, str, size);
        ret[size] = '\0';
    }
    return ret;
}

char *str_utils_append(char *str, const char *append)
{
    return str_utils_nappend(str, append, append ? strlen(append) : 0);
}

char *str_utils_nappend(char *str, const char *append, size_t append_size)
{
    char *out = str;

    if(str) {
        size_t len = strlen(str);
        size_t full_len = len + append_size;
        out = hls_malloc(full_len + 1);
        if(out) {
            if(len) {
                memcpy(out, str, len);
            }
            if(append_size) {
                memcpy(&out[len], append, append_size);
            }
            out[full_len] = '\0';
            hls_free(str);
        }
    }

    return out;
}

char *str_utils_join(const char *str, const char *join)
{
    if(!join) {
        return NULL;
    }

    return str_utils_njoin(str, join, strlen(join));
}

char *str_utils_njoin(const char *str, const char *join, size_t size)
{
    char *out = NULL;

    if(str) {
        size_t len = strlen(str);
        size_t full_len = len + size;
        out = hls_malloc(full_len + 1);
        if(out) {
            if(len) {
                memcpy(out, str, len);
            }
            if(size) {
                memcpy(&out[len], join, size);
            }
            out[full_len] = '\0';
        }
    }

    return out;
}

void add_segment_to_playlist(media_playlist_t *dest, segment_t *segment)
{
    // add the segment to the playlist
    segment_list_t *next = &dest->segments;

    while(next) {
        if(!next->data) {
            next->data = segment;
            break;
        } else if(!next->next) {
            next->next = hls_malloc(sizeof(segment_list_t));
            hlsparse_segment_list_init(next->next);
            next->next->data = segment;
            break;
        }
        next = next->next;
    };

    dest->last_segment = segment;
    ++(dest->nb_segments);
}

string_list_t* str_utils_list_dup(const string_list_t* list)
{
    if(!list) {
        return NULL;
    }

    string_list_t* new_list = (string_list_t*) hls_malloc(sizeof(string_list_t));
    hlsparse_string_list_init(new_list);

    if(list->data) {
        new_list->data = str_utils_dup(list->data);
    }
    if (list->next) {
        new_list->next = str_utils_list_dup(list->next);
    }
    
    return new_list;
}
