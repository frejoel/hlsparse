/*
 * Copyright 2017 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include "hlsparse.h"
#include "../src/parse.h"
#include "tests.h"
#include <CUnit/Basic.h>

int init(void)
{
    return 0;
}

int clean(void)
{
    return 0;
}

void write_master_test(void)
{
    master_t master;
    hlsparse_master_init(&master);
    master.version = 4;
    master.independent_segments = HLS_TRUE;
    master.start.time_offset = -2.f;
    master.start.precise - HLS_FALSE;

    media_t media0;
    parse_media_init(&media0);

    media0.type = MEDIA_TYPE_VIDEO;
    media0.name = "name";
    media0.group_id = "group-one";
    media0.language = "en-US";
    media0.assoc_language = "en-GB";
    media0.instream_id = MEDIA_INSTREAMID_CC3;
    media0.characteristics = "public.accessibility.transcribes-spoken-dialog";
    media0.channels = "6";
    media0.forced = HLS_TRUE;
    media0.is_default = HLS_TRUE;
    media0.auto_select = HLS_TRUE;

    master.media.data = &media0;
    master.media.next = NULL;

    stream_inf_t inf_0;
    parse_stream_inf_init(&inf_0);
    inf_0.bandwidth = 800000;
    inf_0.avg_bandwidth = 780000;
    inf_0.codecs = "mp4a.40.2,avc1.4d401e";
    inf_0.resolution.width = 1280;
    inf_0.resolution.height = 720;
    inf_0.frame_rate = 29.97f;
    inf_0.hdcp_level = HDCP_LEVEL_NONE;
    inf_0.audio = "group-one";
    inf_0.video = "group-two";
    inf_0.subtitles = "group-three";
    inf_0.closed_captions = "cc";
    inf_0.uri = "http://www.example.com/variant_01.m3u8";
    master.stream_infs.data = &inf_0;

    stream_inf_list_t inf_list_1;
    stream_inf_t inf_1;
    parse_stream_inf_init(&inf_1);
    inf_1.bandwidth = 1200000;
    inf_1.avg_bandwidth = 1150000;
    inf_1.codecs = "mp4a.40.2,avc1.4d401e";
    inf_1.resolution.width = 1280;
    inf_1.resolution.height = 720;
    inf_1.frame_rate = 29.97f;
    inf_1.hdcp_level = HDCP_LEVEL_TYPE0;
    inf_1.audio = "group-one";
    inf_1.video = "group-two";
    inf_1.subtitles = "group-three";
    inf_1.closed_captions = "cc";
    inf_1.uri = "http://www.example.com/variant_02.m3u8";
    inf_list_1.data = &inf_1;
    inf_list_1.next = NULL;
    master.stream_infs.next = &inf_list_1;

    iframe_stream_inf_t ifinf_0;
    parse_iframe_stream_inf_init(&ifinf_0);
    ifinf_0.bandwidth = 800000;
    ifinf_0.avg_bandwidth = 780000;
    ifinf_0.codecs = "mp4a.40.2,avc1.4d401e";
    ifinf_0.resolution.width = 1280;
    ifinf_0.resolution.height = 720;
    ifinf_0.hdcp_level = HDCP_LEVEL_NONE;
    ifinf_0.video = "group-two";
    ifinf_0.uri = "http://www.example.com/iframe_variant_01.m3u8";
    master.iframe_stream_infs.data = &ifinf_0;

    session_data_t sess;
    sess.data_id = "com.example.move.trailer";
    sess.value = "this shouldn't be here if 'uri' is present";
    sess.uri = "http://www.example.com/session_info.json";
    sess.language = "en-US";
    master.session_data.data = &sess;
    master.session_data.next = NULL;

    hls_key_t key;
    key.method = KEY_METHOD_AES128;
    key.iv = (char[]){0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0xE, 0x0F, 0x10};
    key.uri = "http://www.example.com/keys/01.key";
    key.key_format = "identity";
    key.key_format_versions = "1/2/3";
    master.session_keys.data = &key;
    master.session_keys.next = NULL;

    char *out = NULL;
    int size = 0;

    HLSCode res = hlswrite_master(&out, &size, &master);

    printf("\n==========\n%s\n==========\n", out);
    CU_ASSERT(1);
}

void write_media_test(void)
{
    media_playlist_t media;
    hlsparse_media_playlist_init(&media);

    media.m3u = HLS_TRUE;
    media.version = 4;
    media.target_duration = 10;
    media.media_sequence = 1034;
    media.discontinuity_sequence = 2;
    media.playlist_type = PLAYLIST_TYPE_EVENT;
    media.iframes_only = HLS_TRUE;
    media.end_list = HLS_FALSE;

    media.nb_segments = 10;
    segment_t segs[10];
    segment_list_t seg_lists[9];
    timestamp_t pdt = 1512842986001;

    segment_list_t *seg_list = &media.segments;
    int i;
    for(i=0; i<media.nb_segments; ++i) {
        segment_t *seg = &segs[i];
        parse_segment_init(seg);

        if(i == 4) {
            // modify the 5th segment's PDT and add a DISCONTINUITY
            pdt += 1505LL;
            seg->duration = 8.495f;
            seg->discontinuity = HLS_TRUE;
        }else if(i == 7){
            // Add a BYTE Range Request to the 8th Segment
            seg->duration = 10.f;
            seg->byte_range.n = 940;
            seg->byte_range.o = 188;
        }else{
            seg->duration = 10.f;
        }

        seg->pdt = pdt;
        pdt += 10000LL;

        char uri[50];
        snprintf(uri, 50, "variant%d/segment%d.ts", 0, i);
        seg->uri = strdup(uri);
        pdt += (timestamp_t)seg->duration;
        seg_list->data = seg;
        if(i < 10) {
            seg_list->next = &seg_lists[i];
            seg_list = seg_list->next;
        }
    }

    char *out = NULL;
    int size = 0;

    HLSCode res = hlswrite_media(&out, &size, &media);
    printf("\n==========\n%s\n==========\n", out);

    CU_ASSERT(1);
}

void setup()
{
    hlsparse_global_init();
    
    suite("parse", init, clean);
    test("write_master", write_master_test);
    test("write_media", write_media_test);
}