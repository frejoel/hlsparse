/*
 * Copyright 2015 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include "tests.h"
#include "hlsparse.h"
#include "../src/parse.h"
#include "../src/utils.h"
#include "../src/mem.h"
#include <CUnit/Basic.h>

void byte_range_init_test(void)
{
    byte_range_t byte_range;
    hlsparse_byte_range_init(&byte_range);
    CU_ASSERT_EQUAL(byte_range.n, 0);
    CU_ASSERT_EQUAL(byte_range.o, 0);

    hlsparse_byte_range_init(NULL);
}

void ext_inf_init_test(void)
{
    ext_inf_t ext_inf;
    hlsparse_ext_inf_init(&ext_inf);
    CU_ASSERT_EQUAL(ext_inf.duration, 0.f);
    CU_ASSERT_EQUAL(ext_inf.title, NULL);

    hlsparse_ext_inf_init(NULL);
}

void resolution_init_test(void)
{
    resolution_t res;
    hlsparse_resolution_init(&res);
    CU_ASSERT_EQUAL(res.width, 0);
    CU_ASSERT_EQUAL(res.height, 0);

    hlsparse_resolution_init(NULL);
}

void key_init_test(void)
{
    hls_key_t res;
    hlsparse_key_init(&res);
    CU_ASSERT_EQUAL(res.method, KEY_METHOD_UNDEFINED);
    CU_ASSERT_EQUAL(res.key_format, NULL);
    CU_ASSERT_EQUAL(res.iv, NULL);
    CU_ASSERT_EQUAL(res.uri, NULL);
    CU_ASSERT_EQUAL(res.key_format_versions, NULL);

    hlsparse_key_init(NULL);
}

void map_init_test(void)
{
    map_t map;
    hlsparse_map_init(&map);
    CU_ASSERT_EQUAL(map.uri, NULL);
    CU_ASSERT_EQUAL(map.byte_range.n, 0);
    CU_ASSERT_EQUAL(map.byte_range.o, 0);

    hlsparse_map_init(NULL);
}

void daterange_init_test(void)
{
    daterange_t daterange;
    hlsparse_daterange_init(&daterange);
    CU_ASSERT_EQUAL(daterange.id, NULL);
    CU_ASSERT_EQUAL(daterange.klass, NULL);
    CU_ASSERT_EQUAL(daterange.start_date, 0);
    CU_ASSERT_EQUAL(daterange.end_date, 0);
    CU_ASSERT_EQUAL(daterange.duration, 0);
    CU_ASSERT_EQUAL(daterange.planned_duration, 0);
    CU_ASSERT_EQUAL(daterange.client_attributes.key, NULL);
    CU_ASSERT_EQUAL(daterange.client_attributes.value_type, PARAM_TYPE_NONE);
    CU_ASSERT_EQUAL(daterange.client_attributes.next, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_cmd, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_out, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_in, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_cmd_size, 0);
    CU_ASSERT_EQUAL(daterange.scte35_out_size, 0);
    CU_ASSERT_EQUAL(daterange.scte35_in_size, 0);
    CU_ASSERT_EQUAL(daterange.end_on_next, HLS_FALSE);

    hlsparse_map_init(NULL);
}

void media_init_test(void)
{
    media_t media;
    hlsparse_media_init(&media);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_NONE);
    CU_ASSERT_EQUAL(media.service_n, 0);
    CU_ASSERT_EQUAL(media.uri, NULL);
    CU_ASSERT_EQUAL(media.assoc_language, NULL);
    CU_ASSERT_EQUAL(media.language, NULL);
    CU_ASSERT_EQUAL(media.is_default,HLS_FALSE);
    CU_ASSERT_EQUAL(media.forced, HLS_FALSE);
    CU_ASSERT_EQUAL(media.auto_select, HLS_FALSE );
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.group_id, NULL);
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_NONE);

    hlsparse_media_init(NULL);
}

void stream_inf_init_test(void)
{
    stream_inf_t stream_inf;
    hlsparse_stream_inf_init(&stream_inf);
    CU_ASSERT_EQUAL(stream_inf.bandwidth, 0.f);
    CU_ASSERT_EQUAL(stream_inf.avg_bandwidth, 0.f);
    CU_ASSERT_EQUAL(stream_inf.codecs, NULL);
    CU_ASSERT_EQUAL(stream_inf.resolution.width, 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.height, 0);
    CU_ASSERT_EQUAL(stream_inf.audio, NULL);
    CU_ASSERT_EQUAL(stream_inf.subtitles, NULL);
    CU_ASSERT_EQUAL(stream_inf.closed_captions, NULL);
    CU_ASSERT_EQUAL(stream_inf.video, NULL);
    CU_ASSERT_EQUAL(stream_inf.uri, NULL);
    CU_ASSERT_EQUAL(stream_inf.frame_rate, 0);
    CU_ASSERT_EQUAL(stream_inf.hdcp_level, HDCP_LEVEL_UNDEFINED);

    hlsparse_stream_inf_init(NULL);
}
void iframe_stream_inf_init_test(void)
{
    iframe_stream_inf_t stream_inf;
    hlsparse_iframe_stream_inf_init(&stream_inf);
    CU_ASSERT_EQUAL(stream_inf.bandwidth, 0.f);
    CU_ASSERT_EQUAL(stream_inf.avg_bandwidth, 0.f);
    CU_ASSERT_EQUAL(stream_inf.codecs, NULL);
    CU_ASSERT_EQUAL(stream_inf.resolution.width, 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.height, 0);
    CU_ASSERT_EQUAL(stream_inf.video, NULL);
    CU_ASSERT_EQUAL(stream_inf.uri, NULL);
    CU_ASSERT_EQUAL(stream_inf.hdcp_level, HDCP_LEVEL_UNDEFINED);

    hlsparse_iframe_stream_inf_init(NULL);
}

void segment_init_test(void)
{
    segment_t segment;
    hlsparse_segment_init(&segment);
    CU_ASSERT_EQUAL(segment.title, NULL);
    CU_ASSERT_EQUAL(segment.uri, NULL);
    CU_ASSERT_EQUAL(segment.duration, 0.f);
    CU_ASSERT_EQUAL(segment.sequence_num, 0);
    CU_ASSERT_EQUAL(segment.discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(segment.pdt, 0);
    CU_ASSERT_EQUAL(segment.pdt_end, 0);
    CU_ASSERT_EQUAL(segment.byte_range.n, 0);
    CU_ASSERT_EQUAL(segment.byte_range.o, 0);
    CU_ASSERT_EQUAL(segment.key_index, -1);
    CU_ASSERT_EQUAL(segment.map_index, -1);
    CU_ASSERT_EQUAL(segment.daterange_index, -1);
    CU_ASSERT_EQUAL(segment.custom_tags.data, NULL);
    CU_ASSERT_EQUAL(segment.custom_tags.next, NULL);
    CU_ASSERT_EQUAL(segment.skipped_segments, 0);
    CU_ASSERT_EQUAL(segment.recently_removed_dateranges, NULL);

    hlsparse_segment_init(NULL);
}

void session_data_init_test(void)
{
    session_data_t session;
    hlsparse_session_data_init(&session);
    CU_ASSERT_EQUAL(session.data_id, NULL);
    CU_ASSERT_EQUAL(session.value, NULL);
    CU_ASSERT_EQUAL(session.uri,NULL);
    CU_ASSERT_EQUAL(session.language, NULL);

    hlsparse_session_data_init(NULL);
}

void start_init_test(void)
{
    start_t start;
    hlsparse_start_init(&start);
    CU_ASSERT_EQUAL(start.time_offset, 0.f);
    CU_ASSERT_EQUAL(start.precise, HLS_FALSE);

    hlsparse_start_init(NULL);
}

void define_init_test(void)
{
    define_t def;
    hlsparse_define_init(&def);
    CU_ASSERT_EQUAL(def.key, NULL);
    CU_ASSERT_EQUAL(def.value, NULL);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_INVALID);

    hlsparse_define_init(NULL);
}

void ext_inf_term_test(void)
{
    ext_inf_t ext_inf;
    hlsparse_ext_inf_init(&ext_inf);
    hlsparse_ext_inf_term(&ext_inf);

    hlsparse_ext_inf_term(NULL);

    hlsparse_ext_inf_init(&ext_inf);
    ext_inf.title = str_utils_dup("title");
    hlsparse_ext_inf_term(&ext_inf);
    CU_ASSERT_EQUAL(NULL, ext_inf.title);
}

void stream_inf_term_test(void)
{
    stream_inf_t stream_inf;
    hlsparse_stream_inf_init(&stream_inf);
    hlsparse_stream_inf_term(&stream_inf);

    hlsparse_stream_inf_term(NULL);

    hlsparse_stream_inf_init(&stream_inf);
    stream_inf.codecs = str_utils_dup("codecs");
    stream_inf.video = str_utils_dup("video");
    stream_inf.audio = str_utils_dup("audio");
    stream_inf.uri = str_utils_dup("uri");
    stream_inf.subtitles = str_utils_dup("subtitles");
    stream_inf.closed_captions = str_utils_dup("closed captions");
    hlsparse_stream_inf_term(&stream_inf);
    CU_ASSERT_EQUAL(stream_inf.codecs, NULL);
    CU_ASSERT_EQUAL(stream_inf.video, NULL);
    CU_ASSERT_EQUAL(stream_inf.audio, NULL);
    CU_ASSERT_EQUAL(stream_inf.uri, NULL);
    CU_ASSERT_EQUAL(stream_inf.closed_captions, NULL);
    CU_ASSERT_EQUAL(stream_inf.subtitles, NULL);
}

void iframe_stream_inf_term_test(void)
{
    iframe_stream_inf_t stream_inf;
    hlsparse_iframe_stream_inf_init(&stream_inf);
    hlsparse_iframe_stream_inf_term(&stream_inf);

    hlsparse_iframe_stream_inf_term(NULL);

    hlsparse_iframe_stream_inf_init(&stream_inf);
    stream_inf.codecs = str_utils_dup("codecs");
    stream_inf.video = str_utils_dup("video");
    stream_inf.uri = str_utils_dup("uri");
    hlsparse_iframe_stream_inf_term(&stream_inf);
    CU_ASSERT_EQUAL(stream_inf.codecs, NULL);
    CU_ASSERT_EQUAL(stream_inf.video, NULL);
    CU_ASSERT_EQUAL(stream_inf.uri, NULL);
}

void key_term_test(void)
{
    hls_key_t key;
    hlsparse_key_init(&key);

    // make sure it doesn't crash
    hlsparse_key_term(NULL);

    // test it running against an empty initialized key
    hlsparse_key_term(&key);

    key.uri = str_utils_dup("uri");
    key.iv = str_utils_dup("iv_data");
    key.key_format = str_utils_dup("key_format");
    key.key_format_versions = str_utils_dup("key_format_versions");

    hlsparse_key_term(&key);
    CU_ASSERT_EQUAL(key.uri, NULL);
    CU_ASSERT_EQUAL(key.iv, NULL);
    CU_ASSERT_EQUAL(key.key_format, NULL);
    CU_ASSERT_EQUAL(key.key_format_versions, NULL);
}

void map_term_test(void)
{
    map_t map;
    hlsparse_map_init(&map);

    // make sure we don't crash
    hlsparse_map_term(NULL);
    hlsparse_map_term(&map);

    hlsparse_map_init(&map);
    map.uri = str_utils_dup("uri");

    hlsparse_map_term(&map);
    CU_ASSERT_EQUAL(map.uri, NULL);
}

void daterange_term_test(void)
{
    daterange_t daterange;
    hlsparse_daterange_init(&daterange);

    // make sure we don't crash
    hlsparse_daterange_term(NULL);
    hlsparse_daterange_term(&daterange);

    hlsparse_daterange_init(&daterange);
    daterange.id = str_utils_dup("id");
    daterange.klass = str_utils_dup("klass");
    daterange.client_attributes.key = str_utils_dup("key");
    daterange.client_attributes.value.number = 4.f;
    daterange.client_attributes.value_type = PARAM_TYPE_FLOAT;
    daterange.client_attributes.next = (param_list_t*) hls_malloc(sizeof(param_list_t));
    hlsparse_param_list_init(daterange.client_attributes.next);
    daterange.client_attributes.next->key = str_utils_dup("key");
    daterange.client_attributes.next->value_type = PARAM_TYPE_STRING;
    daterange.client_attributes.next->value.data = str_utils_dup("value");
    daterange.scte35_cmd = str_utils_dup("scte35_cmd");
    daterange.scte35_cmd_size = strlen(daterange.scte35_cmd) + 1;
    daterange.scte35_out = str_utils_dup("scte35_out");
    daterange.scte35_out_size = strlen(daterange.scte35_out) + 1;
    daterange.scte35_in = str_utils_dup("scte35_in");
    daterange.scte35_in_size = strlen(daterange.scte35_in) + 1;

    hlsparse_daterange_term(&daterange);
    CU_ASSERT_EQUAL(daterange.id, NULL);
    CU_ASSERT_EQUAL(daterange.klass, NULL);
    CU_ASSERT_EQUAL(daterange.client_attributes.key, NULL);
    CU_ASSERT_EQUAL(daterange.client_attributes.value_type, PARAM_TYPE_NONE);
    CU_ASSERT_EQUAL(daterange.client_attributes.value.data, NULL);
    CU_ASSERT_EQUAL(daterange.client_attributes.next, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_cmd, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_cmd_size, 0);
    CU_ASSERT_EQUAL(daterange.scte35_out, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_out_size, 0);
    CU_ASSERT_EQUAL(daterange.scte35_in, NULL);
    CU_ASSERT_EQUAL(daterange.scte35_in_size, 0);
}

void media_term_test(void)
{
    media_t media;
    hlsparse_media_init(&media);

    // make sure we don't crash
    hlsparse_media_term(NULL);
    hlsparse_media_term(&media);

    hlsparse_media_init(&media);
    media.group_id = str_utils_dup("groupid");
    media.name = str_utils_dup("name");
    media.language = str_utils_dup("language");
    media.assoc_language = str_utils_dup("assoc_language");
    media.uri = str_utils_dup("uri");
    media.characteristics = str_utils_dup("characteristics");
    media.channels = str_utils_dup("6");

    hlsparse_media_term(&media);
    CU_ASSERT_EQUAL(media.group_id, NULL);
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.language, NULL);
    CU_ASSERT_EQUAL(media.assoc_language, NULL);
    CU_ASSERT_EQUAL(media.uri, NULL);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
}

void segment_term_test(void)
{
    segment_t segment;
    hlsparse_segment_init(&segment);

    hlsparse_segment_term(NULL);
    hlsparse_segment_term(&segment);

    hlsparse_segment_init(&segment);
    segment.uri = str_utils_dup("uri");
    segment.title = str_utils_dup("title");

    hlsparse_segment_term(&segment);
    CU_ASSERT_EQUAL(segment.uri, NULL);
    CU_ASSERT_EQUAL(segment.title, NULL);
}

void session_data_term_test(void)
{
    session_data_t session;
    hlsparse_session_data_init(&session);

    // make sure we don't crash
    hlsparse_session_data_term(NULL);
    hlsparse_session_data_term(&session);

    hlsparse_session_data_init(&session);
    session.data_id = str_utils_dup("data_id");
    session.value = str_utils_dup("value");
    session.uri = str_utils_dup("uri");
    session.language = str_utils_dup("language");

    hlsparse_session_data_term(&session);
    CU_ASSERT_EQUAL(session.data_id, NULL);
    CU_ASSERT_EQUAL(session.value, NULL);
    CU_ASSERT_EQUAL(session.uri, NULL);
    CU_ASSERT_EQUAL(session.language, NULL);
}

void define_term_test(void)
{
    define_t def;
    hlsparse_define_init(&def);

    // make sure we don't crash
    hlsparse_define_term(NULL);
    hlsparse_define_term(&def);

    hlsparse_define_init(&def);
    def.key = str_utils_dup("key");
    def.value = str_utils_dup("value");
    def.type = DEFINE_TYPE_NAME;

    hlsparse_define_term(&def);
    CU_ASSERT_EQUAL(def.key, NULL);
    CU_ASSERT_EQUAL(def.value, NULL);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_INVALID);
}

void parse_byte_range_test(void)
{
    byte_range_t byte_range;
    hlsparse_byte_range_init(&byte_range);

    int res = parse_byte_range("1024@20", 7, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_byte_range(NULL, 0, &byte_range);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_byte_range(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    parse_byte_range("2048@32", 7, &byte_range);
    CU_ASSERT_EQUAL(byte_range.n, 2048);
    CU_ASSERT_EQUAL(byte_range.o, 32);

    hlsparse_byte_range_init(&byte_range);
    parse_byte_range("128", 3, &byte_range);
    CU_ASSERT_EQUAL(byte_range.n, 128);
    CU_ASSERT_EQUAL(byte_range.o, 0);

    parse_byte_range("#EXT-X-BYTERANGE:1024@64", 24, &byte_range);
    CU_ASSERT_EQUAL(byte_range.n, 1024);
    CU_ASSERT_EQUAL(byte_range.o, 64);
}

void parse_stream_inf_test(void)
{
    stream_inf_t stream_inf;
    hlsparse_stream_inf_init(&stream_inf);

    int res = parse_stream_inf("", 0, &stream_inf);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_stream_inf(NULL, 0, &stream_inf);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "invalid stream inf";
    res = parse_stream_inf(src, strlen(src), &stream_inf);
    CU_ASSERT_EQUAL(res, strlen(src));

    const char *src2 = "#EXT-X-STREAM-INF:BANDWIDTH=86000,AVERAGE-BANDWIDTH=90000,FRAME-RATE=29.97,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=320x240,VIDEO=\"video_value\",AUDIO=\"audio_value\",SUBTITLES=\"subtitles\",CLOSED-CAPTIONS=NONE,HDCP-LEVEL=TYPE-0";

    res = parse_stream_inf(src2, strlen(src2), &stream_inf);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(stream_inf.bandwidth, 86000);
    CU_ASSERT_EQUAL(stream_inf.avg_bandwidth, 90000);
    CU_ASSERT_EQUAL(stream_inf.frame_rate, 29.97f);
    CU_ASSERT_EQUAL(strcmp("mp4a.40.2,avc1.4d401e", stream_inf.codecs), 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.width, 320);
    CU_ASSERT_EQUAL(stream_inf.resolution.height, 240);
    CU_ASSERT_EQUAL(strcmp("audio_value", stream_inf.audio), 0);
    CU_ASSERT_EQUAL(strcmp("video_value", stream_inf.video), 0);
    CU_ASSERT_EQUAL(strcmp("NONE", stream_inf.closed_captions), 0);
    CU_ASSERT_EQUAL(strcmp("subtitles", stream_inf.subtitles), 0);
    CU_ASSERT_EQUAL(stream_inf.hdcp_level, HDCP_LEVEL_TYPE0);

    hlsparse_stream_inf_term(&stream_inf);

    const char *src3 = "#EXT-X-STREAM-INF:BANDWIDTH=43000,AVERAGE-BANDWIDTH=45000,FRAME-RATE=60,CODECS=\"mp4a.40.2,avc1.4d401e\",CLOSED-CAPTIONS=\"captions\",HDCP-LEVEL=NONE";
    hlsparse_stream_inf_init(&stream_inf);
    res = parse_stream_inf(src3, strlen(src3), &stream_inf);
    CU_ASSERT_EQUAL(res, strlen(src3));
    CU_ASSERT_EQUAL(stream_inf.bandwidth, 43000);
    CU_ASSERT_EQUAL(stream_inf.avg_bandwidth, 45000);
    CU_ASSERT_EQUAL(stream_inf.frame_rate, 60.f);
    CU_ASSERT_EQUAL(strcmp("mp4a.40.2,avc1.4d401e", stream_inf.codecs), 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.width, 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.height, 0);
    CU_ASSERT_EQUAL(stream_inf.audio, NULL);
    CU_ASSERT_EQUAL(stream_inf.video, NULL);
    CU_ASSERT_EQUAL(strcmp("captions", stream_inf.closed_captions), 0);
    CU_ASSERT_EQUAL(stream_inf.subtitles, NULL);
    CU_ASSERT_EQUAL(stream_inf.hdcp_level, HDCP_LEVEL_NONE);

    hlsparse_stream_inf_term(&stream_inf);
}

void parse_iframe_stream_inf_test(void)
{
    iframe_stream_inf_t stream_inf;
    hlsparse_iframe_stream_inf_init(&stream_inf);

    int res = parse_iframe_stream_inf("", 0, &stream_inf);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_iframe_stream_inf(NULL, 0, &stream_inf);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "invalid iframe stream inf";
    res = parse_iframe_stream_inf(src, strlen(src), &stream_inf);
    CU_ASSERT_EQUAL(res, strlen(src));

    const char *src2 = "#EXT-X-I-FRAME-STREAM-INF:BANDWIDTH=86000,URI=\"low/iframe.m3u8\",AVERAGE-BANDWIDTH=90000,FRAME-RATE=29.97,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=320x240,VIDEO=\"video_value\",HDCP-LEVEL=TYPE-0";

    res = parse_iframe_stream_inf(src2, strlen(src2), &stream_inf);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(stream_inf.bandwidth, 86000);
    CU_ASSERT_EQUAL(stream_inf.avg_bandwidth, 90000);
    CU_ASSERT_EQUAL(stream_inf.frame_rate, 29.97f);
    CU_ASSERT_EQUAL(strcmp("mp4a.40.2,avc1.4d401e", stream_inf.codecs), 0);
    CU_ASSERT_EQUAL(stream_inf.resolution.width, 320);
    CU_ASSERT_EQUAL(stream_inf.resolution.height, 240);
    CU_ASSERT_EQUAL(strcmp("video_value", stream_inf.video), 0);
    CU_ASSERT_EQUAL(stream_inf.hdcp_level, HDCP_LEVEL_TYPE0);

    hlsparse_iframe_stream_inf_term(&stream_inf);
}

void parse_resolution_test(void)
{
    int res = parse_resolution("", 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_resolution("640x480", 8, NULL);
    CU_ASSERT_EQUAL(res, 7);

    resolution_t resolution;
    res = parse_resolution("1280x720", 9, &resolution);
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(resolution.width, 1280);
    CU_ASSERT_EQUAL(resolution.height, 720);

    resolution.width = resolution.height = 0;
    res = parse_resolution("invalid resolution", 19, &resolution);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(resolution.width, 0);
    CU_ASSERT_EQUAL(resolution.height, 0);
}

void parse_key_test(void)
{
    int res = parse_key("", 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "METHOD=AES-128,URI=\"http://www.key.com/key.key\",IV=0x48656C6C6F,KEYFORMAT=\"format\",KEYFORMATVERSIONS=\"versions\"";
    res = parse_key(src, strlen(src), NULL);
    CU_ASSERT_EQUAL(res, 0);

    hls_key_t key;
    hlsparse_key_init(&key);
    res = parse_key(src, strlen(src), &key);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(key.method, KEY_METHOD_AES128);
    CU_ASSERT_EQUAL(strcmp(key.uri, "http://www.key.com/key.key"), 0);
    CU_ASSERT_EQUAL(strcmp(key.iv, "Hello"), 0);
    CU_ASSERT_EQUAL(strcmp(key.key_format, "format"), 0);
    CU_ASSERT_EQUAL(strcmp(key.key_format_versions, "versions"), 0);
    hlsparse_key_term(&key);

    // test typical key methods
    const char *src2 = "METHOD=NONE";
    hls_key_t key2;
    hlsparse_key_init(&key2);
    res = parse_key(src2, strlen(src2), &key2);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(key2.method, KEY_METHOD_NONE);
    hlsparse_key_term(&key2);

    const char *src3 = "METHOD=THROUGH_THE_FIRE";
    hlsparse_key_init(&key);
    res = parse_key(src3, strlen(src3), &key2);
    CU_ASSERT_EQUAL(res, strlen(src3));
    CU_ASSERT_EQUAL(key2.method, KEY_METHOD_INVALID);

    const char *src4 = "METHOD=SAMPLE-AES";
    hlsparse_key_init(&key);
    res = parse_key(src4, strlen(src4), &key2);
    CU_ASSERT_EQUAL(res, strlen(src4));
    CU_ASSERT_EQUAL(key2.method, KEY_METHOD_SAMPLEAES);
}

void parse_map_test(void)
{
    map_t map;
    hlsparse_map_init(&map);

    int res = parse_map(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_map(NULL, 0, &map);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_map("URI=\"www\",BYTERANGE=\"666\"", 23, NULL);
    CU_ASSERT_EQUAL(res, 23);

    const char *src = "URI=\"http://www.example.com\",BYTERANGE=\"123@45\"";
    res = parse_map(src, strlen(src), &map);
    CU_ASSERT_EQUAL(strcmp(map.uri, "http://www.example.com"), 0);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(map.byte_range.n, 123);
    CU_ASSERT_EQUAL(map.byte_range.o, 45);
    hlsparse_map_term(&map);
}

void parse_daterange_test(void)
{
    daterange_t daterange;
    hlsparse_daterange_init(&daterange);

    int res = parse_daterange(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_daterange(NULL, 0, &daterange);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "ID=\"one\",CLASS=\"myClass:K,myValue=2\",START-DATE=2017-01-01T12:00:10.000+08:00,END-DATE=2017-01-01T12:00:20.000+08:00,DURATION=10.00,PLANNED-DURATION=10.50,X-COM-TEST-ONE=\"com.test.one\",X-COM-TEST-TWO=0xAABBCCDD,X-COM-TEST-THREE=1.234,SCTE35-CMD=0x01020304,SCTE35-OUT=0x05060708,SCTE35-IN=0x090A0B0C,END-ON-NEXT=YES";
    int len = strlen(src);
    res = parse_daterange(src, len, NULL);
    CU_ASSERT_EQUAL(res, len);

    res = parse_daterange(src, strlen(src), &daterange);
    CU_ASSERT_EQUAL(strcmp(daterange.id, "one"), 0);
    CU_ASSERT_EQUAL(res, len);
    CU_ASSERT_EQUAL(strcmp(daterange.klass, "myClass:K,myValue=2"), 0);
    CU_ASSERT_EQUAL(daterange.start_date, 1483243210000);
    CU_ASSERT_EQUAL(daterange.end_date, 1483243220000);
    CU_ASSERT_EQUAL(daterange.duration, 10.f);
    CU_ASSERT_EQUAL(daterange.planned_duration, 10.5f);
    const char cmd[] = { 0x01, 0x02, 0x03, 0x04 };
    CU_ASSERT_EQUAL(memcmp(daterange.scte35_cmd, cmd, 4), 0);
    CU_ASSERT_EQUAL(daterange.scte35_cmd_size, 4);
    const char out[] = { 0x05, 0x06, 0x07, 0x08 };
    CU_ASSERT_EQUAL(memcmp(daterange.scte35_out, out, 4), 0);
    CU_ASSERT_EQUAL(daterange.scte35_out_size, 4);
    const char in[] = { 0x09, 0x0A, 0x0B, 0x0C };
    CU_ASSERT_EQUAL(memcmp(daterange.scte35_in, in, 4), 0);
    CU_ASSERT_EQUAL(daterange.scte35_in_size, 4);
    CU_ASSERT_EQUAL(daterange.end_on_next, HLS_TRUE);

    param_list_t *item = &daterange.client_attributes;
    CU_ASSERT_EQUAL(item->value_type, PARAM_TYPE_STRING);
    CU_ASSERT_EQUAL(strcmp(item->key, "X-COM-TEST-ONE"), 0);
    CU_ASSERT_EQUAL(strcmp(item->value.data, "com.test.one"), 0);
    CU_ASSERT_EQUAL(item->value_size, 13);
    item = item->next;
    CU_ASSERT_EQUAL(item->value_type, PARAM_TYPE_DATA);
    CU_ASSERT_EQUAL(strcmp(item->key, "X-COM-TEST-TWO"), 0);
    char xcomtwo[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    CU_ASSERT_EQUAL(memcmp(item->value.data, xcomtwo, 4), 0);
    CU_ASSERT_EQUAL(item->value_size, 4);
    item = item->next;
    CU_ASSERT_EQUAL(item->value_type, PARAM_TYPE_FLOAT);
    CU_ASSERT_EQUAL(strcmp(item->key, "X-COM-TEST-THREE"), 0);
    CU_ASSERT_EQUAL(item->value.number, 1.234f);
    CU_ASSERT_EQUAL(item->value_size, 0);
    CU_ASSERT_EQUAL(item->next, NULL);

    hlsparse_daterange_term(&daterange);
}

void parse_media_test(void)
{
    int res = parse_media(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_media("", 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    media_t media;
    hlsparse_media_init(&media);
    res = parse_media(NULL, 0, &media);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_media("", 0, &media);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "TYPE=AUDIO,URI=\"uri\",GROUP-ID=\"group_id\",LANGUAGE=\"en-US\",ASSOC-LANGUAGE=\"en-GB\",NAME=\"name\",DEFAULT=NO,AUTOSELECT=YES,FORCED=NO,INSTREAM-ID=\"CC1\",CHARACTERISTICS=\"one.one,two.two,three.three\",CHANNELS=\"6\"";
    int len  = strlen(src);

    res = parse_media(src, len, NULL);
    CU_ASSERT_EQUAL(res, len);

    res = parse_media(src, len, &media);
    CU_ASSERT_EQUAL(res, len);
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_AUDIO);
    CU_ASSERT_EQUAL(media.is_default, HLS_FALSE);
    CU_ASSERT_EQUAL(media.auto_select, HLS_TRUE);
    CU_ASSERT_EQUAL(media.forced, HLS_FALSE);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_CC1);
    CU_ASSERT_EQUAL(media.service_n, 0);
    CU_ASSERT_EQUAL(strcmp(media.uri, "uri"), 0)
    CU_ASSERT_EQUAL(strcmp(media.group_id, "group_id"), 0);
    CU_ASSERT_EQUAL(strcmp(media.language, "en-US"), 0);
    CU_ASSERT_EQUAL(strcmp(media.assoc_language, "en-GB"), 0);
    CU_ASSERT_EQUAL(strcmp(media.name, "name"), 0);
    CU_ASSERT_EQUAL(strcmp(media.characteristics, "one.one,two.two,three.three"), 0);
    CU_ASSERT_EQUAL(strcmp(media.channels, "6"), 0);
    hlsparse_media_term(&media);

    const char *src2 = "TYPE=VIDEO,DEFAULT=YES,AUTOSELECT=NO,FORCED=YES,INSTREAM-ID=\"CC2\"";
    hlsparse_media_init(&media);
    res = parse_media(src2, strlen(src2), &media);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_VIDEO);
    CU_ASSERT_EQUAL(media.is_default, HLS_TRUE);
    CU_ASSERT_EQUAL(media.auto_select, HLS_FALSE);
    CU_ASSERT_EQUAL(media.forced, HLS_TRUE);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_CC2);
    CU_ASSERT_EQUAL(media.service_n, 0);
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
    hlsparse_media_term(&media);

    const char *src3= "TYPE=SUBTITLES,DEFAULT=WHYNOT,AUTOSELECT=SURE,FORCED=YEP,INSTREAM-ID=\"CC3\"";
    hlsparse_media_init(&media);
    res = parse_media(src3, strlen(src3), &media);
    CU_ASSERT_EQUAL(res, strlen(src3));
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_SUBTITLES);
    CU_ASSERT_EQUAL(media.is_default, HLS_FALSE);
    CU_ASSERT_EQUAL(media.auto_select, HLS_FALSE);
    CU_ASSERT_EQUAL(media.forced, HLS_FALSE);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_CC3);
    CU_ASSERT_EQUAL(media.service_n, 0);
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
    hlsparse_media_term(&media);

    const char *src4= "TYPE=CLOSED-CAPTIONS,INSTREAM-ID=\"CC4\",NAME=\"\"";
    hlsparse_media_init(&media);
    res = parse_media(src4, strlen(src4), &media);
    CU_ASSERT_EQUAL(res, strlen(src4));
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_CLOSEDCAPTIONS);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_CC4);
    CU_ASSERT_EQUAL(media.service_n, 0);
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.group_id, NULL);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
    CU_ASSERT_EQUAL(media.language, NULL);
    CU_ASSERT_EQUAL(media.assoc_language, NULL);
    CU_ASSERT_EQUAL(media.is_default, HLS_FALSE);
    CU_ASSERT_EQUAL(media.auto_select, HLS_FALSE);
    CU_ASSERT_EQUAL(media.forced, HLS_FALSE);
    hlsparse_media_term(&media);

    const char *src5= "TYPE=SOMETHING-NEW,INSTREAM-ID=\"SERVICE5\"";
    hlsparse_media_init(&media);
    res = parse_media(src5, strlen(src5), &media);
    CU_ASSERT_EQUAL(res, strlen(src5));
    CU_ASSERT_EQUAL(media.type, MEDIA_TYPE_INVALID);
    CU_ASSERT_EQUAL(media.instream_id, MEDIA_INSTREAMID_SERVICE);
    CU_ASSERT_EQUAL(media.service_n, 5);
    CU_ASSERT_EQUAL(media.name, NULL);
    CU_ASSERT_EQUAL(media.group_id, NULL);
    CU_ASSERT_EQUAL(media.characteristics, NULL);
    CU_ASSERT_EQUAL(media.channels, NULL);
    CU_ASSERT_EQUAL(media.language, NULL);
    CU_ASSERT_EQUAL(media.assoc_language, NULL);
    hlsparse_media_term(&media);
}

void parse_segment_test(void)
{
    const char *src = "#EXTINF:5.004,foo";
    segment_t seg;
    hlsparse_segment_init(&seg);
     
    int res = parse_segment(NULL, 0, NULL); 
    CU_ASSERT_EQUAL(res, 0);

    res = parse_segment(NULL, 0, &seg); 
    CU_ASSERT_EQUAL(res, 0);

    res = parse_segment(src, strlen(src), NULL);
    CU_ASSERT_EQUAL(res, strlen(src));

    res = parse_segment(src, strlen(src), &seg);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(seg.duration, 5.004f);
    CU_ASSERT_EQUAL(strcmp(seg.title, "foo"), 0);

    const char *src2 = "#EXTINF:10.123,\nseg001.ts";
    hlsparse_segment_term(&seg);
    hlsparse_segment_init(&seg);
    res = parse_segment(src2, strlen(src2), &seg);
    CU_ASSERT_EQUAL(res, 15);
    CU_ASSERT_EQUAL(seg.duration, 10.123f);
    CU_ASSERT_EQUAL(seg.title, NULL);
    CU_ASSERT_EQUAL(seg.uri, NULL);

    const char *src3 = "#EXTINF:4,bar\n/seg01.ts";
    hlsparse_segment_term(&seg);
    hlsparse_segment_init(&seg);
    res = parse_segment(src3, strlen(src3), &seg);
    CU_ASSERT_EQUAL(res, 13);
    CU_ASSERT_EQUAL(seg.duration, 4.f);
    CU_ASSERT_EQUAL(strcmp(seg.title, "bar"), 0);
    CU_ASSERT_EQUAL(seg.uri, NULL);
}

void parse_session_data_test(void)
{
    session_data_t session;
    hlsparse_session_data_init(&session);

    int res = parse_session_data(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_session_data(NULL, 0, &session);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "DATA-ID=\"com.example.movie.title\",VALUE=\"value\",LANGUAGE=\"en-US\",URI=\"http://www.example.com\"";    
    res = parse_session_data(src, strlen(src), NULL);
    CU_ASSERT_EQUAL(res, strlen(src));
    hlsparse_session_data_term(&session);

    hlsparse_session_data_init(&session);
    res = parse_session_data(src, strlen(src), &session);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(strcmp(session.data_id, "com.example.movie.title"), 0);
    CU_ASSERT_EQUAL(strcmp(session.value, "value"), 0);
    CU_ASSERT_EQUAL(strcmp(session.uri, "http://www.example.com"), 0);
    CU_ASSERT_EQUAL(strcmp(session.language, "en-US"), 0);
    hlsparse_session_data_term(&session);
}

void parse_start_test(void)
{
    start_t start;
    hlsparse_start_init(&start);
    
    int res = parse_start(NULL, 0, NULL);    
    CU_ASSERT_EQUAL(res, 0);

    res = parse_start(NULL, 0, &start);
    CU_ASSERT_EQUAL(res, 0);

    const char *src = "TIME-OFFSET=34.567,PRECISE=YES";
    res = parse_start(src, strlen(src), NULL);
    CU_ASSERT_EQUAL(res, strlen(src));

    hlsparse_start_init(&start);
    res = parse_start(src, strlen(src), &start);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(start.time_offset, 34.567f);
    CU_ASSERT_EQUAL(start.precise, HLS_TRUE);

    hlsparse_start_init(&start);
    const char *src2 = "TIME-OFFSET=-68.0,PRECISE=NO";
    res = parse_start(src2, strlen(src2), &start);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(start.time_offset, -68.f);
    CU_ASSERT_EQUAL(start.precise, HLS_FALSE);

    hlsparse_start_init(&start);
    const char *src3 = "TIME-OFFSET=0.0";
    res = parse_start(src3, strlen(src3), &start);
    CU_ASSERT_EQUAL(res, strlen(src3));
    CU_ASSERT_EQUAL(start.time_offset, 0.f);
    CU_ASSERT_EQUAL(start.precise, HLS_FALSE);
}

void parse_define_test(void)
{
    define_t def;
    hlsparse_define_init(&def);

    const char *src = "NAME=\"ABC-123\",VALUE=\"789_zxy\"";
    int res = parse_define(src, strlen(src), &def);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(strcmp("ABC-123", def.key), 0);
    CU_ASSERT_EQUAL(strcmp("789_zxy", def.value), 0);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_NAME);

    hlsparse_define_term(&def);
    hlsparse_define_init(&def);

    const char *src2 = "VALUE=\"789_zxy\",IMPORT=\"imp9\"";
    res = parse_define(src2, strlen(src2), &def);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(strcmp("imp9", def.key), 0);
    CU_ASSERT_EQUAL(strcmp("789_zxy", def.value), 0);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_IMPORT);

    hlsparse_define_term(&def);
    hlsparse_define_init(&def);

    const char *src3 = "VALUE=\"789_zxy\",QUERYPARAM=\"query-param\"";
    res = parse_define(src3, strlen(src3), &def);
    CU_ASSERT_EQUAL(res, strlen(src3));
    CU_ASSERT_EQUAL(strcmp("query-param", def.key), 0);
    CU_ASSERT_EQUAL(strcmp("789_zxy", def.value), 0);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_QUERYPARAM);

    hlsparse_define_term(&def);
    hlsparse_define_init(&def);

    const char *src4 = "NAME=\"\",VALUE=\"\",IMPORT=\"\",QUERYPARAM=\"\"";
    res = parse_define(src4, strlen(src4), &def);
    CU_ASSERT_EQUAL(res, strlen(src4));
    CU_ASSERT_EQUAL(def.key, NULL);
    CU_ASSERT_EQUAL(def.value, NULL);

    hlsparse_define_term(&def);
    hlsparse_define_init(&def);

    const char *src5 = "VALUE=\"not-valid\",INVALID=\"\"";
    res = parse_define(src5, strlen(src5), &def);
    CU_ASSERT_EQUAL(res, strlen(src5));
    CU_ASSERT_EQUAL(def.key, NULL);
    CU_ASSERT_EQUAL(strcmp("not-valid", def.value), 0);
    CU_ASSERT_EQUAL(def.type, DEFINE_TYPE_INVALID);
}

void parse_skip_test(void)
{
    segment_t seg;
    hlsparse_segment_init(&seg);

    const char *src = "SKIP:SKIPPED-SEGMENTS=5,RECENTLY-REMOVED-DATERANGES=\"id0\x09id1\x09id2\"";
    int res = parse_skip(src, strlen(src), &seg);
    CU_ASSERT_EQUAL(res, strlen(src));
    CU_ASSERT_EQUAL(seg.skipped_segments, 5);
    CU_ASSERT_EQUAL(strcmp("id0\x09id1\x09id2", seg.recently_removed_dateranges), 0);

    hlsparse_segment_term(&seg);
    hlsparse_segment_init(&seg);

    const char *src2 = "SKIP:SKIPPED-SEGMENTS=123,RECENTLY-REMOVED-DATERANGES=\"\"";
    res = parse_skip(src2, strlen(src2), &seg);
    CU_ASSERT_EQUAL(res, strlen(src2));
    CU_ASSERT_EQUAL(seg.skipped_segments, 123);
    CU_ASSERT_EQUAL(seg.recently_removed_dateranges, NULL);
}

void setup(void)
{
    hlsparse_global_init();

    suite("parser_tags_init", NULL, NULL);
    test("byte_range_init", byte_range_init_test);
    test("ext_inf_init", ext_inf_init_test);
    test("stream_inf_init", stream_inf_init_test);
    test("iframe_stream_inf_init", iframe_stream_inf_init_test);
    test("resolution_init", resolution_init_test);
    test("key_init", key_init_test);
    test("map_init", map_init_test);
    test("daterange_init", daterange_init_test);
    test("media_init", media_init_test);
    test("segment_init", segment_init_test);
    test("session_data_init", session_data_init_test);
    test("start_init", start_init_test);
    test("define_init", define_init_test);

    suite("parser_tags_term", NULL, NULL);
    test("ext_inf_term", ext_inf_term_test);
    test("stream_inf_term", stream_inf_term_test);
    test("iframe_stream_inf_term", iframe_stream_inf_term_test);
    test("key_term", key_term_test);
    test("map_term", map_term_test);
    test("daterange_term", daterange_term_test);
    test("media_term", media_term_test);
    test("segment_term", segment_term_test);
    test("session_data_term", session_data_term_test);
    test("define_data_term", define_term_test);

    suite("parser_tags_parse", NULL, NULL);
    test("parse_byte_range", parse_byte_range_test);
    test("parse_stream_inf", parse_stream_inf_test);
    test("parse_iframe_stream_inf", parse_iframe_stream_inf_test);
    test("parse_resolution", parse_resolution_test);
    test("parse_key", parse_key_test);
    test("parse_map", parse_map_test);
    test("parse_daterange", parse_daterange_test);
    test("parse_media", parse_media_test);
    test("parse_segment", parse_segment_test);
    test("parse_session_data", parse_session_data_test);
    test("parse_start", parse_start_test);
    test("parse_define", parse_define_test);
    test("parse_skip", parse_skip_test);
}

