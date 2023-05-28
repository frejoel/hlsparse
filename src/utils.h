/*
 * Copyright 2023 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "hlsparse.h"

char *str_utils_dup(const char *str);
char *str_utils_ndup(const char *str, size_t size);
char *str_utils_append(char *str, const char *append);
char *str_utils_nappend(char *str, const char *append, size_t size);
char *str_utils_join(const char *str, const char *join);
char *str_utils_njoin(const char *str, const char *join, size_t size);
string_list_t* str_utils_list_dup(const string_list_t* list);

char *path_combine(char **dest, const char *base, const char *path);
void add_segment_to_playlist(media_playlist_t *dest, segment_t *segment);

#endif // _UTILS_H