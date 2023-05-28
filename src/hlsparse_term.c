#include "parse.h"
#include "mem.h"

#include <memory.h>
#include <stdio.h>

HLSCode hlsparse_multivariant_playlist_term(multivariant_playlist_t *dest)
{
    if(!dest) {
        return HLS_ERROR;
    }

    if(dest->uri) {
        char **params[] = {
            &dest->uri
        };
        hlsparse_param_term(params, 1);
    }

    hlsparse_string_list_term(&dest->custom_tags);
    hlsparse_session_data_list_term(&dest->session_data);
    hlsparse_media_list_term(&dest->media);
    hlsparse_stream_inf_list_term(&dest->stream_infs);
    hlsparse_iframe_stream_inf_list_term(&dest->iframe_stream_infs);

    return HLS_OK;
}

HLSCode hlsparse_media_playlist_term(media_playlist_t *dest)
{
    if(!dest) {
        return HLS_ERROR;
    }

    if(dest->uri) {
        char **params[] = {
            &dest->uri
        };
        hlsparse_param_term(params, 1);
    }
    
    hlsparse_string_list_term(&dest->custom_tags);
    hlsparse_segment_list_term(&dest->segments);
    hlsparse_map_list_term(&dest->maps);
    hlsparse_daterange_list_term(&dest->dateranges);
    
    return HLS_OK;
}

/**
 * Helper function for cleanup up tags which have been allocated on the heap
 *
 * @param params The list of parameters than need to be freed if they exist
 * @param size The number of items in params
 */
void hlsparse_param_term(char ***params, int size)
{
    for(int i=0; i<size; ++i) {
        if(params[i] && *params[i]) {
            hls_free(*params[i]);
            *params[i] = NULL;
        }
    }
}

/**
 * Helper function for terminating a define_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_define_list_term(define_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hls_free(dest->data);
        }

        define_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_define_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a keylist_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_key_list_term(key_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_key_term(dest->data);
            hls_free(dest->data);
        }

        key_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_key_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a map_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_map_list_term(map_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_map_term(dest->data);
            hls_free(dest->data);
        }

        map_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_map_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating an ext_inf_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param ext_inf The ext_inf to cleanup
 */
void hlsparse_ext_inf_term(ext_inf_t *ext_inf)
{
    if(ext_inf) {
        char **params[] = { &ext_inf->title };
        hlsparse_param_term(params, 1);
    }
}

/**
 * Helper function for terminating an iframe_stream_inf_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param stream_inf The stream_inf to cleanup
 */
void hlsparse_iframe_stream_inf_term(iframe_stream_inf_t *stream_inf)
{
    if(stream_inf) {
        char **params[] = { 0, 0, 0};

        int i = 0;
        if(stream_inf->codecs) params[i++] = &stream_inf->codecs;
        if(stream_inf->video) params[i++] = &stream_inf->video;
        if(stream_inf->uri) params[i++] = &stream_inf->uri;

        hlsparse_param_term(params, i);
    }
}

/**
 * Helper function for terminating a stream_inf_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param stream_inf The stream_inf to cleanup
 */
void hlsparse_stream_inf_term(stream_inf_t *stream_inf)
{
    if(stream_inf) {
        char **params[] = { 0, 0, 0, 0, 0, 0, 0 };

        int i = 0;
        if(stream_inf->codecs) params[i++] = &stream_inf->codecs;
        if(stream_inf->video) params[i++] = &stream_inf->video;
        if(stream_inf->audio) params[i++] = &stream_inf->audio;
        if(stream_inf->subtitles) params[i++] = &stream_inf->subtitles;
        if(stream_inf->closed_captions) params[i++] = &stream_inf->closed_captions;
        if(stream_inf->video) params[i++] = &stream_inf->video;
        if(stream_inf->uri) params[i++] = &stream_inf->uri;

        hlsparse_param_term(params, i);
    }
}

/**
 * Helper function for terminating an hls_key_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_key_term(hls_key_t *dest)
{
    if(dest) {
        char **params[] = { 0, 0, 0, 0 };

        int i = 0;
        if(dest->uri) params[i++] = &dest->uri;
        if(dest->iv) params[i++] = &dest->iv;
        if(dest->key_format) params[i++] = &dest->key_format;
        if(dest->key_format_versions) params[i++] = &dest->key_format_versions;

        hlsparse_param_term(params, i);
    }
}

/**
 * Helper function for terminating a map_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_map_term(map_t *dest)
{
    if(dest) {
        if(dest->uri) {
            char **params[] = {
                &dest->uri
            };

            hlsparse_param_term(params, 1);
        }
    }
}

/**
 * Helper function for terminating a daterange_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_daterange_term(daterange_t *dest)
{
    if(dest) {
        char **params[] = {
            &dest->id,
            &dest->klass,
            &dest->scte35_cmd,
            &dest->scte35_out,
            &dest->scte35_in,
        };

        dest->scte35_cmd_size = 0;
        dest->scte35_out_size = 0;
        dest->scte35_in_size = 0;

        hlsparse_param_term(params, 5);
        hlsparse_param_list_term(&dest->client_attributes);
    }
}

/**
 * Helper function for terminating a map_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_media_term(media_t *dest)
{
    if(dest) {
        char **params[] = { 0, 0, 0, 0, 0, 0, 0 };
        int i = 0;
        if (dest->group_id) params[i++] = &dest->group_id;
        if (dest->name) params[i++] = &dest->name;
        if (dest->language) params[i++] = &dest->language;
        if (dest->assoc_language) params[i++] = &dest->assoc_language;
        if (dest->uri) params[i++] = &dest->uri;
        if (dest->characteristics) params[i++] = &dest->characteristics;
        if (dest->channels) params[i++] = &dest->channels;
        hlsparse_param_term(params, i);
    }
}

/**
 * Helper function for terminating a segment_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_segment_term(segment_t *dest)
{
    if(dest) {
        char ** params[] = { 0 , 0 };
        int i = 0;
        if(dest->title) params[i++] = &dest->title;
        if(dest->uri) params[i++] = &dest->uri;

        hlsparse_param_term(params, i);
        hlsparse_string_list_term(&dest->custom_tags);
    }
}

/**
 * Helper function for terminating a session_data_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_session_data_term(session_data_t *dest)
{
    if(dest) {
        char ** params[] = { 0, 0, 0, 0 };
        int i = 0;
        if(dest->data_id) params[i++] = &dest->data_id;
        if(dest->uri) params[i++] = &dest->uri;
        if(dest->language) params[i++] = &dest->language;
        if(dest->value) params[i++] = &dest->value;

        hlsparse_param_term(params, i);
    }
}

/**
 * Helper function for terminating a define_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_define_term(define_t *dest)
{
    if(dest) {
        dest->type = DEFINE_TYPE_INVALID;

        char ** params[] = { 0, 0 };
        int i = 0;
        if(dest->key) params[i++] = &dest->key;
        if(dest->value) params[i++] = &dest->value;

        hlsparse_param_term(params, i);
    }
}


/**
 * Helper function for terminating a segmentlist_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_segment_list_term(segment_list_t *dest)
{

    if(dest) {
        if(dest->data) {
            hlsparse_segment_term(dest->data);
            hls_free(dest->data);
            dest->data = NULL;
        }

        if(dest->next) {
            hlsparse_segment_list_term(dest->next);
            hls_free(dest->next);
            dest->next = NULL;
        }
    }
}

/**
 * Helper function for terminating a session_data_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_session_data_list_term(session_data_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_session_data_term(dest->data);
            hls_free(dest->data);
        }

        session_data_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_session_data_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a media_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_media_list_term(media_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_media_term(dest->data);
            hls_free(dest->data);
        }

        media_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_media_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a daterange_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_daterange_list_term(daterange_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_daterange_term(dest->data);
            hls_free(dest->data);
        }

        daterange_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_daterange_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a iframe_stream_inf_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_iframe_stream_inf_list_term(iframe_stream_inf_list_t *dest)
{

    if(dest) {
        if(dest->data) {
            hlsparse_iframe_stream_inf_term(dest->data);
            hls_free(dest->data);
        }

        iframe_stream_inf_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_iframe_stream_inf_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a stream_inf_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_stream_inf_list_term(stream_inf_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hlsparse_stream_inf_term(dest->data);
            hls_free(dest->data);
        }

        stream_inf_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_stream_inf_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a string_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_string_list_term(string_list_t *dest)
{
    if(dest) {
        if(dest->data) {
            hls_free(dest->data);
        }

        string_list_t *ptr = dest->next;

        if(ptr) {
            hlsparse_string_list_term(ptr);
            hls_free(ptr);
        }
    }
}

/**
 * Helper function for terminating a param_list_t.
 * This will free any properties on the object resetting the values, but won't
 * try to free the object itself.
 * To reuse the object, call the corresponding init function after termination.
 *
 * @param dest The object to cleanup
 */
void hlsparse_param_list_term(param_list_t *dest)
{
    if(dest) {
        if(dest->key) {
            hls_free(dest->key);
            dest->key = NULL;
        }
        if(dest->value.data && (dest->value_type == PARAM_TYPE_DATA || dest->value_type == PARAM_TYPE_STRING)) {
            hls_free(dest->value.data);
        }
        dest->value.data = NULL;
        dest->value_type = PARAM_TYPE_NONE;
        dest->value_size = 0;

        param_list_t *ptr = dest->next;
        dest->next = NULL;

        if(ptr) {
            hlsparse_param_list_term(ptr);
            hls_free(ptr);
        }
    }
}