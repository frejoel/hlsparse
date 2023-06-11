/*
 * Copyright 2023 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include <memory.h>

#include "parse.h"
#include "utils.h"
#include "mem.h"

/**
 * parses an HLS multivariant playlist src into a multivariant_playlist_t struct
 *
 * @param src The raw HLS src string
 * @param size The length of the HLS src string
 * @param dest The destination to write the properties to. The destiation must
 * already be initialized prior to calling parse.
 */
int parse_multivariant_playlist_tag(const char *src, size_t size, multivariant_playlist_t *dest)
{
    if (!dest) {
        return 0;
    }

    const char *pt = src;

    if (EQUAL(pt, EXTM3U)) {

        dest->m3u = HLS_TRUE;
    } else if (EQUAL(pt, EXTXMEDIA)) {

        media_t *media = hls_malloc(sizeof(media_t));
        hlsparse_media_init(media);

        pt += parse_media(pt, size - (pt - src), media);

        media_list_t *next = &dest->media;

        while (next) {
            if (!next->data) {
                next->data = media;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(media_list_t));
                hlsparse_media_list_init(next->next);
                next->next->data = media;
                break;
            }
            next = next->next;
        };

    } else if (EQUAL(pt, EXTXVERSION)) {
        ++pt; // get past the ':'
        // if the version is already specified, skip subsequent version tags
        pt += parse_str_to_int(pt, dest->version > 0 ? NULL : &dest->version, size - (pt - src));
    } else if (EQUAL(pt, EXTXINDEPENDENTSEGMENTS )) {

        dest->independent_segments = HLS_TRUE;
    } else if (EQUAL(pt, EXTXSTREAMINF)) {

        stream_inf_t *stream_inf = hls_malloc(sizeof(stream_inf_t));
        hlsparse_stream_inf_init(stream_inf);

        pt += parse_stream_inf(pt, size - (pt - src), stream_inf);

        // the line directly after a stream-inf must be the the uri
        // go to the end of the line
        while (*pt != '\n' && *pt != '\0') {
            ++pt;
        }
        // get over the newline
        if (*pt != '\0') {
            ++pt;
        }
        // get the uri
        char *path = NULL;
        pt += parse_line_to_str(pt, &path, size - (pt - src));
        path_combine(&stream_inf->uri, dest->uri, path);
        if (path) hls_free(path);

        stream_inf_list_t *next = &dest->stream_infs;

        while (next) {
            if (!next->data) {
                next->data = stream_inf;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(stream_inf_list_t));
                hlsparse_stream_inf_list_init(next->next);
                next->next->data = stream_inf;
                break;
            }
            next = next->next;
        };

        dest->nb_stream_infs++;

    } else if (EQUAL(pt, EXTXIFRAMESTREAMINF)) {

        iframe_stream_inf_t *stream_inf = hls_malloc(sizeof(iframe_stream_inf_t));
        hlsparse_iframe_stream_inf_init(stream_inf);
        pt += parse_iframe_stream_inf(pt, size - (pt - src), stream_inf);

        char *path = stream_inf->uri;
        path_combine(&stream_inf->uri, dest->uri, path);

        iframe_stream_inf_list_t *next = &dest->iframe_stream_infs;

        while (next) {
            if (!next->data) {
                next->data = stream_inf;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(iframe_stream_inf_list_t));
                hlsparse_iframe_stream_inf_list_init(next->next);
                next->next->data = stream_inf;
                break;
            }
            next = next->next;
        };

        dest->nb_iframe_stream_infs++;

    } else if (EQUAL(pt, EXTXSESSIONDATA)) {

        session_data_t *session_data = hls_malloc(sizeof(session_data_t));
        hlsparse_session_data_init(session_data);
        ;
        pt += parse_session_data(pt, size - (pt - src), session_data);

        session_data_list_t *next = &dest->session_data;

        while (next) {
            if (!next->data) {
                next->data = session_data;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(session_data_list_t));
                hlsparse_session_data_list_init(next->next);
                next->next->data = session_data;
                break;
            }
            next = next->next;
        };

    } else if (EQUAL(pt, EXTXSTART)) {

        ++pt;
        pt += parse_start(pt, size - (pt - src), &dest->start);
    } else if (EQUAL(pt, EXTXSESSIONKEY)) {

        ++pt;
        hls_key_t *key = hls_malloc(sizeof(hls_key_t));
        hlsparse_key_init(key);
        pt += parse_key(pt, size - (pt - src), key);

        if (dest->uri && key->method != KEY_METHOD_NONE && key->method != KEY_METHOD_INVALID) {
            path_combine_realloc(&key->uri, dest->uri, key->uri);
        }

        key_list_t *next = &dest->session_keys;

        while (next) {
            if (!next->data) {
                next->data = key;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(hls_key_t));
                hlsparse_key_list_init(next->next);
                next->next->data = key;
                break;
            }
            next = next->next;
        };

        ++(dest->nb_session_keys);
    } else if (EQUAL(pt, EXTXDEFINE)) {

        ++pt;
        define_t *def = hls_malloc(sizeof(define_t));
        hlsparse_define_init(def);
        pt += parse_define(pt, size -(pt - src), def);

        define_list_t *next = &dest->defines;
        while (next) {
            if (!next->data) {
                next->data = def;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(define_list_t));
                hlsparse_define_list_init(next->next);
                next->next->data = def;
                break;
            }
            next = next->next;
        }

        ++(dest->nb_defines);

    } else {

        // custom src
        char *custom_tag = NULL;
        pt += parse_line_to_str(pt, &custom_tag, size - (pt - src));
        if (custom_tag && *custom_tag != '\0') {

            string_list_t *next = &dest->custom_tags;

            while (next) {
                if (!next->data) {
                    next->data = custom_tag;;
                    break;
                } else if (!next->next) {
                    next->next = hls_malloc(sizeof(string_list_t));
                    hlsparse_string_list_init(next->next);
                    next->next->data = custom_tag;
                    break;
                }
                next = next->next;
            };
        }
    }

    // return how far we have moved along in the src
    return pt - src;
}

/**
 * parses an HLS media playlist src into a media_playlisy_t struct
 *
 * @param src The raw HLS src string
 * @param size The length of the src string
 * @param dest The destination to write the properties to. The destiation must
 * already be initialized prior to calling parse.
 */
int parse_media_playlist_tag(const char *src, size_t size, media_playlist_t *dest)
{
    if (!dest) {
        return 0;
    }

    const char *pt = src;

    if (EQUAL(pt, EXTM3U)) {
        dest->m3u = HLS_TRUE;
    } else if (EQUAL(pt, EXTXVERSION)) {
        ++pt; // get past the '=' sign
        pt += parse_str_to_int(pt, &dest->version, size - (pt - src));
    } else if (EQUAL(pt, EXTXTARGETDURATION)) {
        ++pt; // get past the '=' sign
        pt += parse_str_to_float(pt, &dest->target_duration, size - (pt - src));
    } else if (EQUAL(pt, EXTXPARTINF)) {
        ++pt; // get past ':'
        if (EQUAL(pt, PARTTARGET)) {
            ++pt; // get past the '=' sign
            pt += parse_str_to_float(pt, &dest->part_target_duration, size - (pt - src));
        }
    } else if (EQUAL(pt, EXTXINDEPENDENTSEGMENTS)) {
        ++pt; // get past the '=' sign
        dest->independent_segments = HLS_TRUE;
    } else if (EQUAL(pt, EXTXMEDIASEQUENCE)) {
        ++pt; // get past the '=' sign
        pt += parse_str_to_int(pt, &dest->media_sequence, size - (pt - src));
    } else if (EQUAL(pt, EXTXPLAYLISTTYPE)) {
        ++pt; // get past the '=' sign
        if (EQUAL(pt, VOD)) {
            dest->playlist_type = PLAYLIST_TYPE_VOD;
        } else if (EQUAL(pt, EVENT)) {
            dest->playlist_type = PLAYLIST_TYPE_EVENT;
        } else {
            dest->playlist_type = PLAYLIST_TYPE_INVALID;
        }
    } else if (EQUAL(pt, EXTXENDLIST)) {
        dest->end_list = HLS_TRUE;
    } else if (EQUAL(pt, EXTXPROGRAMDATETIME)) {
        ++pt; // get past the '=' sign
        pt += parse_date(pt, &dest->next_segment_pdt, size - (pt - src));
    } else if (EQUAL(pt, EXTXGAP)) {
        dest->next_segment_type |= SEGMENT_TYPE_GAP;
    } else if (EQUAL(pt, EXTXSKIP)) {
        dest->next_segment_type |= SEGMENT_TYPE_SKIP;

        ++pt; // get past the ':'
        segment_t *segment = hls_malloc(sizeof(segment_t));
        hlsparse_segment_init(segment);

        pt += parse_skip(pt, size - (pt - src), segment);
        if (segment->uri && dest->uri) {
            path_combine_realloc(&segment->uri, dest->uri, segment->uri);
        }

        segment->bitrate = dest->next_segment_bitrate;
        segment->sequence_num = dest->next_segment_media_sequence;

        // if the previous segment was a partial segment, use this to determine the pdt
        if (dest->last_segment && (dest->last_segment->type & SEGMENT_TYPE_PART) > 0) {
            segment->pdt = dest->last_segment->pdt_end;
        } else {
            segment->pdt = dest->next_segment_pdt;
        }
        segment->pdt_end = segment->pdt + (timestamp_t)(segment->duration * 1000.f);

        // if this isn't the first segment, check to see if there is a
        // discontinuity between this segment and the one before it
        segment->pdt_discontinuity = HLS_FALSE;

        if (dest->nb_segments > 0) {
            if (segment->pdt != dest->last_segment->pdt_end) {
                segment->pdt_discontinuity = HLS_TRUE;
            }
        }

        // add this segment to the playlists duration
        dest->duration += segment->duration;

        // properties from the next full segment apply to this segment too
        // we won't clear any of these until the full segment is available
        segment->key_index = dest->nb_keys - 1;
        segment->map_index = dest->nb_maps - 1;
        if (dest->next_segment_daterange_index >= 0) {
            segment->daterange_index = dest->next_segment_daterange_index;
        }
        segment->custom_tags.data = str_utils_dup(dest->custom_tags.data);
        segment->custom_tags.next = str_utils_list_dup(dest->custom_tags.next);

        add_segment_to_playlist(dest, segment);
    } else if (EQUAL(pt, EXTXALLOWCACHE)) {
        dest->allow_cache = HLS_TRUE;
    } else if (EQUAL(pt, EXTXDISCONTINUITYSEQ)) {
        ++pt; // get past the ':'
        pt += parse_str_to_int(pt, &dest->discontinuity_sequence, size - (pt - src));
    } else if (EQUAL(pt, EXTXDISCONTINUITY)) {
        dest->next_segment_discontinuity = HLS_TRUE;
    } else if (EQUAL(pt, EXTXIFRAMESONLY)) {
        dest->iframes_only = HLS_TRUE;
    } else if (EQUAL(pt, EXTXINDEPENDENTSEGMENTS)) {
        dest->independent_segments = HLS_TRUE;
    } else if (EQUAL(pt, EXTXSTART)) {
        ++pt;
        pt += parse_start(pt, size - (pt - src), &dest->start);
    } else if (EQUAL(pt, EXTXBITRATE)) {
        ++pt; // get past the ':'
        pt += parse_str_to_int(pt, &dest->next_segment_bitrate, size - (pt - src));
    } else if (EQUAL(pt, EXTXBYTERANGE)) {
        if (*pt == ':') {
            ++pt;
            pt += parse_str_to_int(pt,
                                   &dest->next_segment_byterange.n,
                                   size - (pt - src));
            if (*pt == '@') {
                ++pt;
                pt += parse_str_to_int(pt,
                                       &dest->next_segment_byterange.o,
                                       size - (pt - src));
            }
        }
    } else if (EQUAL(pt, EXTXDEFINE)) {
        if (*pt == ':') {
            ++pt;
            define_t *def = hls_malloc(sizeof(define_t));
            hlsparse_define_init(def);
            pt += parse_define(pt, size -(pt - src), def);

            define_list_t *next = &dest->defines;
            while (next) {
                if (!next->data) {
                    next->data = def;
                    break;
                } else if (!next->next) {
                    next->next = hls_malloc(sizeof(define_list_t));
                    hlsparse_define_list_init(next->next);
                    next->next->data = def;
                    break;
                }
                next = next->next;
            }

            ++(dest->nb_defines);
        }
    } else if (EQUAL(pt, EXTXPART)) {
        ++pt; // get past the ':'
        segment_t *segment = hls_malloc(sizeof(segment_t));
        hlsparse_segment_init(segment);

        pt += parse_partial_segment(pt, size - (pt - src), segment);
        if (segment->uri && dest->uri) {
            path_combine_realloc(&segment->uri, dest->uri, segment->uri);
        }

        segment->bitrate = dest->next_segment_bitrate;
        segment->sequence_num = dest->next_segment_media_sequence;

        // if the previous segment was a partial segment, use this to determine the pdt
        if (dest->last_segment && (dest->last_segment->type & SEGMENT_TYPE_PART) > 0) {
            segment->pdt = dest->last_segment->pdt_end;
        } else {
            segment->pdt = dest->next_segment_pdt;
        }
        segment->pdt_end = segment->pdt + (timestamp_t)(segment->duration * 1000.f);

        // if this isn't the first segment, check to see if there is a
        // discontinuity between this segment and the one before it
        segment->pdt_discontinuity = HLS_FALSE;

        if (dest->nb_segments > 0) {
            if (segment->pdt != dest->last_segment->pdt_end) {
                segment->pdt_discontinuity = HLS_TRUE;
            }
        }

        // add this segment to the playlists duration
        dest->duration += segment->duration;

        // properties from the next full segment apply to this segment too
        // we won't clear any of these until the full segment is available
        segment->key_index = dest->nb_keys - 1;
        segment->map_index = dest->nb_maps - 1;
        if (dest->next_segment_daterange_index >= 0) {
            segment->daterange_index = dest->next_segment_daterange_index;
        }
        segment->custom_tags.data = str_utils_dup(dest->custom_tags.data);
        segment->custom_tags.next = str_utils_list_dup(dest->custom_tags.next);

        add_segment_to_playlist(dest, segment);

    } else if (EQUAL(pt, EXTINF)) {
        ++pt;
        segment_t *segment = hls_malloc(sizeof(segment_t));
        hlsparse_segment_init(segment);

        pt += parse_segment(pt, size - (pt - src), segment);

        segment->type = dest->next_segment_type | SEGMENT_TYPE_FULL;
        // reset the segment type
        dest->next_segment_type = SEGMENT_TYPE_FULL;

        segment->bitrate = dest->next_segment_bitrate;
        segment->sequence_num = dest->next_segment_media_sequence;
        ++(dest->next_segment_media_sequence);

        segment->pdt = dest->next_segment_pdt;
        segment->pdt_end = segment->pdt + (timestamp_t)(segment->duration * 1000.f);
        // increase the segment PDT so that the next segment gets a valid value
        dest->next_segment_pdt = segment->pdt_end;

        // if this isn't the first segment, check to see if there is a
        // discontinuity between this segment and the one before it
        segment->pdt_discontinuity = HLS_FALSE;

        if (dest->nb_segments > 0) {
            if (segment->pdt != dest->last_segment->pdt_end) {
                segment->pdt_discontinuity = HLS_TRUE;
            }
        }

        // add this segment's duration to the playlists duration provided this segment
        // replaces partial segments
        if (!dest->last_segment || (dest->last_segment->type & SEGMENT_TYPE_PART) == 0) {
            dest->duration += segment->duration;
        }

        add_segment_to_playlist(dest, segment);

    } else if (EQUAL(pt, EXTXKEY)) {
        ++pt;
        hls_key_t *key = hls_malloc(sizeof(hls_key_t));
        hlsparse_key_init(key);
        pt += parse_key(pt, size - (pt - src), key);

        if (key->method != KEY_METHOD_NONE && key->method != KEY_METHOD_INVALID) {
            path_combine_realloc(&key->uri, dest->uri, key->uri);
        }

        // set the media sequnce that the key originated
        key_list_t *next = &dest->keys;

        while (next) {
            if (!next->data) {
                next->data = key;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(hls_key_t));
                hlsparse_key_list_init(next->next);
                next->next->data = key;
                break;
            }
            next = next->next;
        };

        ++(dest->nb_keys);

    } else if (EQUAL(pt, EXTXMAP)) {
        ++pt;
        map_t *map = hls_malloc(sizeof(map_t));;
        hlsparse_map_init(map);
        pt += parse_map(pt, size - (pt - src), map);
        map_list_t *next = &dest->maps;

        while (next) {
            if (!next->data) {
                next->data = map;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(map_t));
                hlsparse_map_list_init(next->next);
                next->next->data = map;
                break;
            }
            next = next->next;
        };

        ++(dest->nb_maps);

    } else if (EQUAL(pt, EXTXDATERANGE)) {
        ++pt;
        daterange_t *daterange = hls_malloc(sizeof(daterange_t));;
        hlsparse_daterange_init(daterange);
        pt += parse_daterange(pt, size - (pt - src), daterange);
        daterange->pdt = dest->next_segment_pdt;

        daterange_list_t *next = &dest->dateranges;

        while (next) {

            if (!next->data) {
                next->data = daterange;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(daterange_t));
                hlsparse_daterange_list_init(next->next);
                next->next->data = daterange;
                break;
            }
            next = next->next;
        };

        ++(dest->nb_dateranges);
        dest->next_segment_daterange_index = dest->nb_dateranges - 1;

    } else {
        // custom src
        char *custom_tag = NULL;

        pt += parse_line_to_str(pt, &custom_tag, size - (pt - src));

        if (custom_tag && *custom_tag != '\0') {

            string_list_t *next = &dest->custom_tags;

            while (next) {
                if (!next->data) {
                    next->data = custom_tag;;
                    break;
                } else if (!next->next) {
                    next->next = hls_malloc(sizeof(string_list_t));
                    hlsparse_string_list_init(next->next);
                    next->next->data = custom_tag;
                    break;
                }
                next = next->next;
            };

            ++(dest->nb_custom_tags);
        }
    }

    // return how far we have moved along in the src
    return pt - src;
}
