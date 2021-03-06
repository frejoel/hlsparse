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
    master.start.precise = HLS_FALSE;

    media_t media0;
    hlsparse_media_init(&media0);

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
    hlsparse_stream_inf_init(&inf_0);
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
    hlsparse_stream_inf_init(&inf_1);
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
    hlsparse_iframe_stream_inf_init(&ifinf_0);
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

    const char *master_output = 
"#EXTM3U\n\
#EXT-X-VERSION:4\n\
#EXT-X-INDEPENDENT-SEGMENTS\n\
#EXT-X-START:TIME-OFFSET=-2.000,PRECISE=NO\n\
#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"group-one\",LANGUAGE=\"en-US\",ASSOC-LANGUAGE=\"en-US\",ASSOC-LANGUAGE=\"en-GB\",NAME=\"name\",DEFAULT=YES,AUTOSELECT=YES,FORCED=YES,INSTREAM-ID=\"CC3\",CHARACTERISTICS=\"public.accessibility.transcribes-spoken-dialog\",CHANNELS=\"6\"\n\
#EXT-X-STREAM-INF:BANDWIDTH=800000,AVERAGE-BANDWIDTH=780000,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=1280x720,FRAME-RATE=29.970,HDCP-LEVEL=NONE,AUDIO=\"group-one\",VIDEO=\"group-two\",SUBTITLES=\"group-three\",CLOSED-CAPTIONS=\"cc\"\n\
http://www.example.com/variant_01.m3u8\n\
#EXT-X-STREAM-INF:BANDWIDTH=1200000,AVERAGE-BANDWIDTH=1150000,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=1280x720,FRAME-RATE=29.970,HDCP-LEVEL=TYPE-0,AUDIO=\"group-one\",VIDEO=\"group-two\",SUBTITLES=\"group-three\",CLOSED-CAPTIONS=\"cc\"\n\
http://www.example.com/variant_02.m3u8\n\
#EXT-X-I-FRAME-STREAM-INF:BANDWIDTH=800000,AVERAGE-BANDWIDTH=780000,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=1280x720,HDCP-LEVEL=NONE,VIDEO=\"group-two\",URI=\"http://www.example.com/iframe_variant_01.m3u8\"\n\
#EXT-X-SESSION-DATA:DATA-ID=\"com.example.move.trailer\",VALUE=\"this shouldn\'t be here if \'uri\' is present\",URI=\"http://www.example.com/session_info.json\",LANGUAGE=\"en-US\"\n\
#EXT-X-SESSION-KEY:METHOD=AES-128,URI=\"http://www.example.com/keys/01.key\",IV=0x0102030405060708090A0B0C0D0E0F10,KEYFORMAT=\"identity\",KEYFORMATVERSIONS=\"1/2/3\"\n";
    
    CU_ASSERT_EQUAL(strcmp(master_output, out), 0);
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
    hls_key_t keys[2];
    segment_list_t seg_lists[9];
    key_list_t key_list_1;
    timestamp_t pdt = 1512842986001;

    hlsparse_key_init(&keys[0]);
    hlsparse_key_init(&keys[1]);

    media.keys.data = &keys[0];
    media.keys.next = &key_list_1;
    key_list_1.data = &keys[1];
    key_list_1.next = NULL;
    keys[0].method = KEY_METHOD_NONE;
    keys[1].iv = (char[]){0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    keys[1].uri = "https://www.example.com/key0.key";
    keys[1].method = KEY_METHOD_AES128;

    segment_list_t *seg_list = &media.segments;
    int i;
    for(i=0; i<media.nb_segments; ++i) {
        segment_t *seg = &segs[i];
        hlsparse_segment_init(seg);

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

        if(i >= 6) {
            seg->key_index = 1;
        }else{
            seg->key_index = 0;
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

    const char *media_output =
"#EXTM3U\n\
#EXT-X-VERSION:4\n\
#EXT-X-TARGETDURATION:10\n\
#EXT-X-MEDIA-SEQUENCE:1034\n\
#EXT-X-DISCONTINUITY-SEQUENCE:2\n\
#EXT-X-PLAYLIST-TYPE:EVENT\n\
#EXT-X-I-FRAMES-ONLY\n\
#EXT-X-PROGRAM-DATE-TIME:2017-12-09T18:09:46.001Z\n\
#EXT-X-KEY:METHOD=NONE\n\
#EXTINF:10.000,\n\
variant0/segment0.ts\n\
#EXTINF:10.000,\n\
variant0/segment1.ts\n\
#EXTINF:10.000,\n\
variant0/segment2.ts\n\
#EXTINF:10.000,\n\
variant0/segment3.ts\n\
#EXT-X-DISCONTINUITY\n\
#EXT-X-PROGRAM-DATE-TIME:2017-12-09T18:10:27.546Z\n\
#EXTINF:8.495,\n\
variant0/segment4.ts\n\
#EXTINF:10.000,\n\
variant0/segment5.ts\n\
#EXT-X-KEY:METHOD=AES-128,URI=\"https://www.example.com/key0.key\",IV=0x0102030405060708090A0B0C0D0E0F10\n\
#EXTINF:10.000,\n\
variant0/segment6.ts\n\
#EXT-X-BYTERANGE:940@188\n\
#EXTINF:10.000,\n\
variant0/segment7.ts\n\
#EXTINF:10.000,\n\
variant0/segment8.ts\n\
#EXTINF:10.000,\n\
variant0/segment9.ts\n";

    CU_ASSERT_EQUAL(strcmp(media_output, out), 0);
}

void write_media_test2(void)
{
    media_playlist_t media;
    hlsparse_media_playlist_init(&media);

    media.m3u = HLS_TRUE;
    media.version = 3;
    media.target_duration = 6;
    media.media_sequence = 1;
    media.discontinuity_sequence = 0;
    media.playlist_type = PLAYLIST_TYPE_VOD;
    media.iframes_only = HLS_FALSE;
    media.end_list = HLS_TRUE;

    media.nb_segments = 5;
    segment_t segs[5];
    hls_key_t keys[3];
    segment_list_t seg_lists[5];
    key_list_t key_lists[2];
    timestamp_t pdt = 1534023759900;

    hlsparse_key_init(&keys[0]);
    hlsparse_key_init(&keys[1]);
    hlsparse_key_init(&keys[2]);

    media.keys.data = &keys[0];
    media.keys.next = &key_lists[0];
    key_lists[0].data = &keys[1];
    key_lists[0].next = &key_lists[1];
    key_lists[1].data = &keys[2];
    key_lists[1].next = NULL;
    keys[0].method = KEY_METHOD_AES128;
    keys[0].iv = (char[]){0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89};
    keys[0].uri = "https://key-service.com/key?id=123";
    keys[1].method = KEY_METHOD_AES128;
    keys[1].iv = (char[]){0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
    keys[1].uri = "https://key-service.com/key?id=124";
    keys[2].method = KEY_METHOD_NONE;

    string_list_t tags[5];
    tags[0].data = "EXT-X-ASSET-START:id=987,pop=";
    tags[0].next = NULL;
    tags[1].data = "EXT-X-CUE-OUT:_params=\"abc=d&efg=MIDROLL&pop=1\"";
    tags[1].next = &tags[2];
    tags[2].data = "EXT-X-CUE-IN";
    tags[2].next = NULL;
    tags[3].data = "EXT-X-CUE-OUT:_fw_params=\"abc=a&efg=POSTROLL&pop=4\"";
    tags[3].next = &tags[4];
    tags[4].data = "EXT-X-CUE-IN";
    tags[4].next = NULL;

    segment_list_t *seg_list = &media.segments;
    int i;
    for(i=0; i<media.nb_segments; ++i) {
        segment_t *seg = &segs[i];
        hlsparse_segment_init(seg);

        seg->pdt = pdt;

        if(i == 0) {
            pdt += 0033LL;
            seg->duration = 0.033f;
            seg->key_index = 0;
            seg->custom_tags = tags[0];
        }else if(i == 1){
            pdt += 4972LL;
            seg->duration = 4.972f;
            seg->key_index = 1;
            seg->custom_tags = tags[1];
        }else if(i == 2){
            pdt += 5005LL;
            seg->duration = 5.005f;
            seg->key_index = 1;
        }else if(i == 3){
            pdt += 4605LL;
            seg->duration = 4.605f;
            seg->key_index = 1;
        }else if(i == 4){
            seg->key_index = 2;
            seg->custom_tags = tags[3];
        }

        if(i < 4) {
            char uri[50];
            snprintf(uri, 50, "ADAP/00060/1001_ADAP_0000%d.ts", i+1);
            seg->uri = strdup(uri);
        }

        pdt += (timestamp_t)seg->duration;
        seg_list->data = seg;
        if(i < 5) {
            seg_list->next = &seg_lists[i];
            seg_list = seg_list->next;
        }
    }

    char *out = NULL;
    int size = 0;

    HLSCode res = hlswrite_media(&out, &size, &media);

    const char *media_output = "#EXTM3U\n\
#EXT-X-VERSION:3\n\
#EXT-X-TARGETDURATION:6\n\
#EXT-X-MEDIA-SEQUENCE:1\n\
#EXT-X-DISCONTINUITY-SEQUENCE:0\n\
#EXT-X-PLAYLIST-TYPE:VOD\n\
#EXT-X-PROGRAM-DATE-TIME:2018-08-11T21:42:39.900Z\n\
#EXT-X-ASSET-START:id=987,pop=\n\
#EXT-X-KEY:METHOD=AES-128,URI=\"https://key-service.com/key?id=123\",IV=0xABCDEF0123456789ABCDEF0123456789\n\
#EXTINF:0.033,\n\
ADAP/00060/1001_ADAP_00001.ts\n\
#EXT-X-CUE-OUT:_params=\"abc=d&efg=MIDROLL&pop=1\"\n\
#EXT-X-CUE-IN\n\
#EXT-X-KEY:METHOD=AES-128,URI=\"https://key-service.com/key?id=124\",IV=0xCDEF0123456789ABCDEF0123456789AB\n\
#EXTINF:4.972,\n\
ADAP/00060/1001_ADAP_00002.ts\n\
#EXTINF:5.005,\n\
ADAP/00060/1001_ADAP_00003.ts\n\
#EXTINF:4.605,\n\
ADAP/00060/1001_ADAP_00004.ts\n\
#EXT-X-CUE-OUT:_fw_params=\"abc=a&efg=POSTROLL&pop=4\"\n\
#EXT-X-CUE-IN\n\
#EXT-X-ENDLIST\n";

    CU_ASSERT_EQUAL(strcmp(media_output, out), 0);
}
void setup()
{
    hlsparse_global_init();
    
    suite("parse", init, clean);
    test("write_master", write_master_test);
    test("write_media", write_media_test);
    test("write_media", write_media_test2);
}