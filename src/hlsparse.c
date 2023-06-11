#include "parse.h"
#include "mem.h"

#include <memory.h>
#include <stdio.h>

int hlsparse_multivariant_playlist(const char *src, size_t size, multivariant_playlist_t *dest)
{
    int res = 0;

    // make sure we have some data
    if (src && *src != '\0' && src < &src[size]) {
        // go through each line parsing the tags
        const char *end = &src[size];
        const char *pt = src;
        // loop until we find a null terminator or hit the end of the data
        while (*pt != '\0' && pt < end) {
            if (*pt == '#') {
                ++pt;
                pt += parse_multivariant_playlist_tag(pt, size - (pt - src), dest);
            } else {
                ++pt;
            }
        }

        res = pt - src;
    }

    return res;
}

int hlsparse_media_playlist(const char *src, size_t size, media_playlist_t *dest)
{
    int res = 0;

    if (dest) {
        // reset the duration
        dest->duration = 0;
        // reset the segment byte range
        dest->next_segment_byterange.n = dest->next_segment_byterange.o = 0;
    }

    // make sure we have some data
    if (src && (src[0] != '\0') && size > 0) {
        // go through each line parsing the tags
        const char *pt = &src[0];
        while (*pt != '\0' && pt < &src[size]) {
            if (*pt == '#') {
                ++pt;
                pt += parse_media_playlist_tag(pt, size - (pt - src), dest);
            } else if (*pt == '\n' && pt[1] != '#') {
                ++pt;
                if (dest->last_segment) {
                    pt += parse_segment_uri(pt, size - (pt - src), dest);
                }
            } else {
                ++pt;
            }
        }

        res = pt - src;
    }

    // custom tags can exist after the last segment for things like pre-roll ad insertion
    // create a zero length segment and attach these custom tags to that segment
    string_list_t *tag_media = &(dest->custom_tags);
    if (tag_media && tag_media->data) {
        // create a new segment
        segment_t *segment = hls_malloc(sizeof(segment_t));
        hlsparse_segment_init(segment);

        // add the new segment to the playlist
        segment_list_t *next = &dest->segments;
        while (next) {
            if (!next->data) {
                next->data = segment;
                break;
            } else if (!next->next) {
                next->next = hls_malloc(sizeof(segment_list_t));
                hlsparse_segment_list_init(next->next);
                next->next->data = segment;
                break;
            }
            next = next->next;
        };

        dest->last_segment = segment;
        ++(dest->nb_segments);

        segment->key_index = dest->nb_keys - 1;
        segment->map_index = dest->nb_maps - 1;
        segment->daterange_index = dest->nb_dateranges - 1;

        segment->pdt = segment->pdt_end = dest->next_segment_pdt;
        segment->sequence_num = dest->next_segment_media_sequence;

        segment->custom_tags.data = dest->custom_tags.data;
        segment->custom_tags.next = dest->custom_tags.next;
        dest->custom_tags.data = NULL;
        dest->custom_tags.next = NULL;

        segment->discontinuity = dest->next_segment_discontinuity;
        // reset the discontinuity flag
        dest->next_segment_discontinuity = HLS_FALSE;
    }

    return res;
}
