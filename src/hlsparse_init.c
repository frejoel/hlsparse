#define MEM_EXTERN_ALLOC
#include "mem.h"
#include "hlsparse.h"
#include <memory.h>
#include <stdio.h>

HLSCode hlsparse_global_init(void)
{
    hls_malloc = (hlsparse_malloc_callback) malloc;
    hls_free = (hlsparse_free_callback) free;
    return HLS_OK;
}

HLSCode hlsparse_init_mem(hlsparse_malloc_callback m, hlsparse_free_callback f)
{
    if (!m || !f) {
        return HLS_ERROR;
    }
    hls_malloc = m;
    hls_free = f;
    return HLS_OK;
}

HLSCode hlsparse_multivariant_playlist_init(multivariant_playlist_t *dest)
{
    if (!dest) {
        return HLS_ERROR;
    }

    memset(dest, 0, sizeof(multivariant_playlist_t));
    return HLS_OK;
}

HLSCode hlsparse_media_playlist_init(media_playlist_t *dest)
{
    if (!dest) {
        return HLS_ERROR;
    }

    memset(dest, 0, sizeof(media_playlist_t));
    dest->next_segment_daterange_index = -1;
    return HLS_OK;
}

/**
 * Helper function for initializing a byte_range_t object
 *
 * @param byte_range The byte_range to initialize
 */
void hlsparse_byte_range_init(byte_range_t *byte_range)
{
    if (byte_range) {
        byte_range->n = 0;
        byte_range->o = 0;
    }
}

/**
 * Helper function for initializing a ext_inf_t
 *
 * @param ext_inf The ext_inf to initialize
 */
void hlsparse_ext_inf_init(ext_inf_t *ext_inf)
{
    if (ext_inf) {
        ext_inf->duration = 0.f;
        ext_inf->title = NULL;
    }
}

/**
 * Helper function for initializing a resolution_t
 *
 * @param resoution The resolution object to initialize
 */
void hlsparse_resolution_init(resolution_t *resolution)
{
    if (resolution) {
        resolution->width = 0;
        resolution->height = 0;
    }
}

/**
 * Helper function for initialzing an iframe_stream_inf
 *
 * @param stream_inf The iframe_stream_inf to initialize
 */
void hlsparse_iframe_stream_inf_init(iframe_stream_inf_t *stream_inf)
{
    if (stream_inf) {
        memset(stream_inf, 0, sizeof(iframe_stream_inf_t));
    }
}

/**
 * Helper function for initialzing a stream_inf
 *
 * @param stream_inf The stream_inf to initialize
 */
void hlsparse_stream_inf_init(stream_inf_t *stream_inf)
{
    if (stream_inf) {
        memset(stream_inf, 0, sizeof(stream_inf_t));
    }
}

/**
 * Helper function for initializing an hls_key_t
 *
 * @param dest The dest to initialize
 */
void hlsparse_key_init(hls_key_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(hls_key_t));
    }
}

/**
 * Helper function for initializing a map_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_map_init(map_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(map_t));
    }
}

/**
 * Helper function for initializing a daterange_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_daterange_init(daterange_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(daterange_t));
    }
}

/**
 * Helper function for initializing a map_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_media_init(media_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(media_t));
    }
}

/**
 * Helper function for initializing a segment_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_segment_init(segment_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(segment_t));
        dest->type = SEGMENT_TYPE_FULL;
        dest->daterange_index = -1;
        dest->key_index = -1;
        dest->map_index = -1;
    }
}

/**
 * Helper function for initializing a session_data_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_session_data_init(session_data_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(session_data_t));
    }
}

/**
 * Helper function for initializing a start_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_start_init(start_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(start_t));
    }
}

/**
 * Helper function for initializing a define_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_define_init(define_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(define_t));
    }
}

/**
 * Helper function for initializing a segment_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_segment_list_init(segment_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(segment_list_t));
    }
}

/**
 * Helper function for initializing a session_data_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_session_data_list_init(session_data_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(session_data_list_t));
    }
}

/**
 * Helper function for initializing a key_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_key_list_init(key_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(key_list_t));
    }
}

/**
 * Helper function for initializing a stream_inf_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_stream_inf_list_init(stream_inf_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(stream_inf_list_t));
    }
}

/**
 * Helper function for initializing a iframe_stream_inf_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_iframe_stream_inf_list_init(iframe_stream_inf_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(iframe_stream_inf_list_t));
    }
}

/**
 * Helper function for initializing a media_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_media_list_init(media_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(media_list_t));
    }
}

/**
 * Helper function for initializing a map_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_map_list_init(map_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(map_list_t));
    }
}

/**
 * Helper function for initializing a datrange_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_daterange_list_init(daterange_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(daterange_list_t));
    }
}

/**
 * Helper function for initializing a string_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_string_list_init(string_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(string_list_t));
    }
}

/**
 * Helper function for initializing a define_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_define_list_init(define_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(define_list_t));
    }
}

/**
 * Helper function for initializing a param_list_t object
 *
 * @param dest The dest object to initialize
 */
void hlsparse_param_list_init(param_list_t *dest)
{
    if (dest) {
        memset(dest, 0, sizeof(param_list_t));
    }
}