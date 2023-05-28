/*
 * Copyright 2015 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#ifndef _PARSER_TAGS_H
#define _PARSER_TAGS_H

#include "hlsparse.h"

// compares a char* with a string literal HLS tag increasing the ptr by the length
// of the tag if successful
#define EQUAL(a,b)  (0 == strncmp((a), (b), sizeof(b) - 1) && (a += sizeof(b) - 1))

#ifdef __cplusplus
extern "C" {
#endif

// Tag parsing
int parse_line_to_str(const char *src, char **dest, size_t size);
int parse_str_to_int(const char *src, int *dest, size_t size);
int parse_str_to_float(const char *str, float *dest, size_t size);
int parse_date(const char *src, uint64_t *dest, size_t size);
int parse_attrib_str(const char *src, char **dest, size_t size);
int parse_attrib_data(const char *src, char **dest, size_t size);
int parse_line_to_str(const char *src, char **dest, size_t size);
int parse_str_to_int(const char *src, int *dest, size_t size);
int parse_str_to_float(const char *str, float *dest, size_t size);
int parse_date(const char *src, uint64_t *dest, size_t size);
int parse_attrib_str(const char *src, char **dest, size_t size);
int parse_attrib_data(const char *src, char **dest, size_t size);
int parse_multivariant_playlist_tag(const char *src, size_t size, multivariant_playlist_t *dest); 
int parse_media_playlist_tag(const char *src, size_t size, media_playlist_t *dest);

int parse_byte_range(const char *src, size_t size, byte_range_t *dest);
int parse_iframe_stream_inf(const char *src, size_t size, iframe_stream_inf_t *dest);
int parse_iframe_stream_inf_tag(const char *src, size_t size, iframe_stream_inf_t *dest);
int parse_stream_inf(const char *src, size_t size, stream_inf_t *dest);
int parse_stream_inf_tag(const char *src, size_t size, stream_inf_t *dest);
int parse_resolution(const char *src, size_t size, resolution_t *dest);
int parse_key(const char *src, size_t size, hls_key_t *key);
int parse_key_tag(const char *src, size_t size, hls_key_t *key);
int parse_map(const char *src, size_t size, map_t *map);
int parse_daterange_tag(const char *src, size_t size, daterange_t *daterange);
int parse_daterange(const char *src, size_t size, daterange_t *daterange);
int parse_map_tag(const char *src, size_t size, map_t *map);
int parse_media(const char *src, size_t size, media_t *media);
int parse_media_tag(const char *src, size_t size, media_t *media);
int parse_segment(const char *src, size_t size, segment_t *segment);
int parse_segment_tag(const char *src, size_t size, segment_t *segment);
int parse_segment_uri(const char *src, size_t size, media_playlist_t *dest);
int parse_partial_segment(const char *src, size_t size, segment_t *segment);
int parse_session_data(const char *src, size_t size, session_data_t *session_data);
int parse_session_data_tag(const char *src, size_t size,session_data_t *session_data);
int parse_start(const char *src, size_t size, start_t *start);
int parse_define(const char *src, size_t size, define_t *start);

#ifdef __cplusplus
}
#endif

#endif // _PARSER_TAGS_H
