/*
 * Copyright 2015 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include "tests.h"
#include "hlsparse.h"

void playlist_init_test(void)
{
    master_t master;
    HLSCode res = hlsparse_master_init(&master);
    CU_ASSERT_EQUAL(res, HLS_OK);
    CU_ASSERT_EQUAL(master.version, 0);
    CU_ASSERT_EQUAL(master.uri, 0); 
    CU_ASSERT_EQUAL(master.m3u, HLS_FALSE); 
    CU_ASSERT_EQUAL(master.independent_segments, HLS_FALSE); 
    CU_ASSERT_EQUAL(master.start.time_offset, 0.f); 
    CU_ASSERT_EQUAL(master.start.precise, HLS_FALSE); 
    CU_ASSERT_EQUAL(master.session_data.data, NULL); 
    CU_ASSERT_EQUAL(master.session_data.next, NULL); 
    CU_ASSERT_EQUAL(master.media.data, NULL); 
    CU_ASSERT_EQUAL(master.media.next, NULL); 
    CU_ASSERT_EQUAL(master.stream_infs.data, NULL); 
    CU_ASSERT_EQUAL(master.stream_infs.next, NULL); 
    CU_ASSERT_EQUAL(master.iframe_stream_infs.data, NULL); 
    CU_ASSERT_EQUAL(master.iframe_stream_infs.next, NULL); 
    CU_ASSERT_EQUAL(master.custom_tags.data, NULL); 
    CU_ASSERT_EQUAL(master.custom_tags.next, NULL); 
    
    hlsparse_master_init(NULL);
}

void playlist_term_test(void)
{
    master_t master;
    HLSCode res = hlsparse_master_init(&master);
    CU_ASSERT_EQUAL(res, HLS_OK);

    res = hlsparse_master_term(&master);
    CU_ASSERT_EQUAL(res, HLS_OK);

    res = hlsparse_master_term(NULL);
    CU_ASSERT_EQUAL(res, HLS_ERROR);
}

void playlist_parse_test(void)
{
    master_t master;
    hlsparse_master_init(&master);

    const char *src = "#EXTM3U\n"\
"#EXT-X-VERSION:7\n"\
"#EXT-X-MEDIA:TYPE=VIDEO,URI=\"http://www.test.com\",GROUP-ID=\"groupId\",NAME=\"media\"\n"\
"#EXT-X-STREAM-INF:BANDWIDTH=900000,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=1280x720,HDCP-LEVEL=TYPE-0\n"\
"/path/to/stream_inf/900.m3u8\n"\
"#EXT-X-STREAM-INF:BANDWIDTH=1500000,CODECS=\"mp4a.40.2.avc1.4d401e\",RESOLUTION=1920x1080,HDCP-LEVEL=NONE\n"\
"/path/to/stream_inf/1500.m3u8\n"\
"#EXT-X-I-FRAME-STREAM-INF:URI=\"http://www.test.com\",BANDWIDTH=900000\n"\
"#EXT-X-SESSION-DATA:DATA-ID=\"com.example.movie.title\",VALUE=\"value\",URI=\"http://www.test.com/data.json\"\n"\
"#EXT-X-SESSION-KEY:METHOD=AES-128,URI=\"http://www.test.com/key\""\
"#EXT-X-INDEPENDENT-SEGMENTS\n"\
"#EXT-X-START:TIME-OFFSET=10.0,PRECISE=YES\n";

    size_t size = strlen(src);
    int res = hlsparse_master(src, size, &master);    
    CU_ASSERT_EQUAL(res, size);
    CU_ASSERT_EQUAL(master.version, 7);
    CU_ASSERT_NOT_EQUAL(master.media.data, NULL);
    CU_ASSERT_EQUAL(master.media.next, NULL);
    CU_ASSERT_NOT_EQUAL(master.stream_infs.data, NULL);
    CU_ASSERT_NOT_EQUAL(master.stream_infs.next, NULL);
    CU_ASSERT_NOT_EQUAL(master.iframe_stream_infs.data, NULL);
    CU_ASSERT_EQUAL(master.iframe_stream_infs.next, NULL);
    CU_ASSERT_EQUAL(master.custom_tags.data, NULL);
    CU_ASSERT_EQUAL(master.nb_session_keys, 1);
    CU_ASSERT_EQUAL(master.session_keys.next, NULL);
    CU_ASSERT_EQUAL(master.nb_stream_infs, 2);
    CU_ASSERT_EQUAL(master.nb_iframe_stream_infs, 1);

    hls_key_t *key = master.session_keys.data;
    CU_ASSERT_EQUAL(key->method, KEY_METHOD_AES128);
    CU_ASSERT_EQUAL(strcmp(key->uri, "http://www.test.com/key"), 0);

    hlsparse_master_term(&master);
    
    // make sure parsing NULL's etc. works
    res = hlsparse_master(NULL, 0, NULL);
    CU_ASSERT_EQUAL(res, 0);
    
    res = hlsparse_master(src, size, NULL);
    CU_ASSERT_EQUAL(res, size);

    hlsparse_master_init(&master);
    res = hlsparse_master(NULL, 0, &master);
    hlsparse_master_term(&master);
}

void media_playlist_init_test(void)
{
    media_playlist_t playlist;
    HLSCode res = hlsparse_media_playlist_init(&playlist);
    CU_ASSERT_EQUAL(res, HLS_OK);
    CU_ASSERT_EQUAL(playlist.version, 0);
    CU_ASSERT_EQUAL(playlist.media_sequence, 0);
    CU_ASSERT_EQUAL(playlist.next_segment_media_sequence, 0);
    CU_ASSERT_EQUAL(playlist.nb_segments, 0);
    CU_ASSERT_EQUAL(playlist.nb_keys, 0);
    CU_ASSERT_EQUAL(playlist.nb_maps, 0);
    CU_ASSERT_EQUAL(playlist.nb_custom_tags, 0);
    CU_ASSERT_EQUAL(playlist.playlist_type, PLAYLIST_TYPE_INVALID);
    CU_ASSERT_EQUAL(playlist.discontinuity_sequence, 0);
    CU_ASSERT_EQUAL(playlist.target_duration, 0.f);
    CU_ASSERT_EQUAL(playlist.duration, 0.f);
    CU_ASSERT_EQUAL(playlist.uri, NULL);
    CU_ASSERT_EQUAL(playlist.m3u, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.independent_segments, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.allow_cache, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.iframes_only, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.next_segment_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.end_list, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.next_segment_pdt, 0);
    CU_ASSERT_EQUAL(playlist.start.time_offset, 0.f);
    CU_ASSERT_EQUAL(playlist.start.precise, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.next_segment_byterange.n, 0);
    CU_ASSERT_EQUAL(playlist.next_segment_byterange.o, 0);
    CU_ASSERT_EQUAL(playlist.segments.data, NULL);
    CU_ASSERT_EQUAL(playlist.segments.next, NULL);
    CU_ASSERT_EQUAL(playlist.keys.data, NULL);
    CU_ASSERT_EQUAL(playlist.keys.next, NULL);
    CU_ASSERT_EQUAL(playlist.maps.data, NULL);
    CU_ASSERT_EQUAL(playlist.maps.next, NULL);
    CU_ASSERT_EQUAL(playlist.custom_tags.data, NULL);
    CU_ASSERT_EQUAL(playlist.custom_tags.next, NULL);
    CU_ASSERT_EQUAL(playlist.last_segment, NULL);
    
    res = hlsparse_media_playlist_init(NULL);
    CU_ASSERT_EQUAL(res, HLS_ERROR);
}

void media_playlist_term_test(void)
{
    media_playlist_t playlist;
    HLSCode res = hlsparse_media_playlist_init(&playlist);
    CU_ASSERT_EQUAL(res, HLS_OK);
    
    res = hlsparse_media_playlist_term(&playlist);
    CU_ASSERT_EQUAL(res, HLS_OK);
    res = hlsparse_media_playlist_term(NULL);
    CU_ASSERT_EQUAL(res, HLS_ERROR);
}

void media_playlist_parse_test(void)
{
    media_playlist_t playlist;
    hlsparse_media_playlist_init(&playlist);

    const char *src = "#EXTM3U\n"\
"#EXT-X-VERSION:4\n"\
"#EXT-X-TARGETDURATION:5\n"\
"#EXT-X-MEDIA-SEQUENCE:34\n"\
"#EXT-X-DISCONTINUITY-SEQUENCE:2\n"\
"#EXT-X-PLAYLIST-TYPE:VOD\n"\
"#EXT-X-INDEPENDENT-SEGMENTS\n"\
"#EXT-X-I-FRAMES-ONLY\n"\
"#EXT-X-START:TIME-OFFSET=12.5,PRECISE=YES\n"\
"#EXT-X-PROGRAM-DATE-TIME:2010-02-19T12:00:00.000+08:00\n"\
"#EXTINF:6.123,segment0\n"\
"http://www.example.com/segment0.ts\n"\
"#EXT-X-BYTERANGE:321\n"\
"#EXTINF:3.887,\n"\
"http://www.example.com/segment1.ts\n"\
"#EXT-X-KEY:METHOD=AES-128,URI=\"http://www.example.com/segment2.key\"\n"\
"#EXTINF:5,segment2\n"\
"http://www.example.com/segment2.ts\n"\
"#EXT-X-BYTERANGE:128@256\n"\
"#EXTINF:7.80,segment3\n"\
"http://www.example.com/segment3.ts\n"\
"#EXT-X-KEY:METHOD=NONE\n"\
"#EXT-X-PROGRAM-DATE-TIME:2010-02-19T12:01:00.000+08:00\n"\
"#EXT-X-DISCONTINUITY\n"\
"#EXTINF:1.2,segment4\n"\
"http://www.example.com/segment4.ts\n"\
"#EXTINF:5,segment5\n"\
"http://www.example.com/segment5.ts\n"\
"#EXT-X-ENDLIST\n";

    int res = hlsparse_media_playlist(src, strlen(src), &playlist);
    CU_ASSERT_EQUAL(res, strlen(src))
    CU_ASSERT_EQUAL(playlist.m3u, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.target_duration, 5);
    CU_ASSERT_EQUAL(playlist.media_sequence, 34);
    CU_ASSERT_EQUAL(playlist.discontinuity_sequence, 2);
    CU_ASSERT_EQUAL(playlist.playlist_type, PLAYLIST_TYPE_VOD);
    CU_ASSERT_EQUAL(playlist.independent_segments, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.iframes_only, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.start.time_offset, 12.5f);
    CU_ASSERT_EQUAL(playlist.start.precise, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.nb_segments, 6);
    CU_ASSERT_EQUAL(playlist.nb_maps, 0);
    CU_ASSERT_EQUAL(playlist.nb_keys, 2);
    CU_ASSERT_EQUAL(playlist.nb_dateranges, 0);

    segment_list_t *seg = &playlist.segments;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment0.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 0);
    CU_ASSERT_EQUAL(seg->data->key_index, -1);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    assert_string_equal(seg->data->title, "segment0", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552000000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552006123);
    CU_ASSERT_EQUAL(seg->data->duration, 6.123f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);
    
    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg, NULL);
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment1.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 1);
    CU_ASSERT_EQUAL(seg->data->key_index, -1);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552006123);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552010010);
    CU_ASSERT_EQUAL(seg->data->duration, 3.887f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 321);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg, NULL);
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment2.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 2);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    assert_string_equal(seg->data->title, "segment2", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552010010);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552015010);
    CU_ASSERT_EQUAL(seg->data->duration, 5.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg, NULL);
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment3.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 3);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    assert_string_equal(seg->data->title, "segment3", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552015010);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552022810);
    CU_ASSERT_EQUAL(seg->data->duration, 7.8f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 128);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 256);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg, NULL);
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment4.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 4);
    CU_ASSERT_EQUAL(seg->data->key_index, 1);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    assert_string_equal(seg->data->title, "segment4", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_TRUE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_TRUE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552060000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552061200);
    CU_ASSERT_EQUAL(seg->data->duration, 1.2f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg, NULL);
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment5.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 5);
    CU_ASSERT_EQUAL(seg->data->key_index, 1);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    assert_string_equal(seg->data->title, "segment5", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552061200);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552066200);
    CU_ASSERT_EQUAL(seg->data->duration, 5.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);
    
    seg = seg->next;
    CU_ASSERT_EQUAL(seg, NULL);

    hlsparse_media_playlist_term(&playlist);
}

void media_playlist_parse_test2(void)
{
    media_playlist_t playlist;
    hlsparse_media_playlist_init(&playlist);

    const char *src = "#EXTM3U\n"\
"#EXT-X-VERSION:7\n"\
"#EXT-X-TARGETDURATION:10\n"\
"#EXT-X-MEDIA-SEQUENCE:0\n"\
"#EXT-X-PLAYLIST-TYPE:EVENT\n"\
"#EXT-X-PROGRAM-DATE-TIME:2010-02-19T12:00:00.000+08:00\n"\
"#EXT-X-KEY:METHOD=AES-128,URI=\"http://www.example.com/segment2.key\"\n"\
"#EXT-X-DATERANGE:ID=\"splice-6FFFFFF0\",START-DATE=\"2010-02-19T04:00:00Z\",PLANNED-DURATION=59.993,SCTE35-OUT=0xFC002F0000000000FF000014056FFFFFF000E011622DCAFF000052636200000000000A0008029896F50000008700000000\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment0.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment1.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment2.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment3.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment4.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment5.ts\n"\
"#EXT-X-DATERANGE:ID=\"splice-6FFFFFF0\",DURATION=59.993,SCTE35-IN=0xFC002A0000000000FF00000F056FFFFFF000401162802E6100000000000A0008029896F50000008700000000\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment6.ts\n"\
"#EXTINF:10,\n"\
"http://www.example.com/segment7.ts\n";

    int res = hlsparse_media_playlist(src, strlen(src), &playlist);
    CU_ASSERT_EQUAL(res, strlen(src))
    CU_ASSERT_EQUAL(playlist.m3u, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.target_duration, 10);
    CU_ASSERT_EQUAL(playlist.media_sequence, 0);
    CU_ASSERT_EQUAL(playlist.discontinuity_sequence, 0);
    CU_ASSERT_EQUAL(playlist.playlist_type, PLAYLIST_TYPE_EVENT);
    CU_ASSERT_EQUAL(playlist.independent_segments, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.iframes_only, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.start.time_offset, 0.f);
    CU_ASSERT_EQUAL(playlist.start.precise, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.nb_segments, 8);
    CU_ASSERT_EQUAL(playlist.nb_maps, 0);
    CU_ASSERT_EQUAL(playlist.nb_dateranges, 2);
    CU_ASSERT_EQUAL(playlist.end_list, HLS_FALSE);

    daterange_t *drange = playlist.dateranges.data;
    CU_ASSERT_EQUAL(drange->pdt, 1266552000000);
    assert_string_equal(drange->id, "splice-6FFFFFF0", __func__, __LINE__);
    CU_ASSERT_EQUAL(drange->planned_duration, 59.993f);
    CU_ASSERT_EQUAL(drange->duration, 0.f);
    CU_ASSERT_NOT_EQUAL(drange->scte35_out, NULL);
    CU_ASSERT_EQUAL(drange->scte35_out_size, 49);
    CU_ASSERT_EQUAL(drange->scte35_in, NULL);
    CU_ASSERT_EQUAL(drange->scte35_in_size, 0);
    CU_ASSERT_EQUAL(drange->scte35_cmd, NULL);
    CU_ASSERT_EQUAL(drange->scte35_cmd_size, 0);
    CU_ASSERT_EQUAL(drange->klass, NULL);
    CU_ASSERT_EQUAL(drange->client_attributes.value_type, PARAM_TYPE_NONE);

    drange = playlist.dateranges.next->data;
    CU_ASSERT_EQUAL(drange->pdt, 1266552060000);
    assert_string_equal(drange->id, "splice-6FFFFFF0", __func__, __LINE__);
    CU_ASSERT_EQUAL(drange->planned_duration, 0.f);
    CU_ASSERT_EQUAL(drange->duration, 59.993f);
    CU_ASSERT_EQUAL(drange->scte35_out, NULL);
    CU_ASSERT_EQUAL(drange->scte35_out_size, 0);
    CU_ASSERT_NOT_EQUAL(drange->scte35_in, NULL);
    CU_ASSERT_EQUAL(drange->scte35_in_size, 44);
    CU_ASSERT_EQUAL(drange->scte35_cmd, NULL);
    CU_ASSERT_EQUAL(drange->scte35_cmd_size, 0);
    CU_ASSERT_EQUAL(drange->klass, NULL);
    CU_ASSERT_EQUAL(drange->client_attributes.value_type, PARAM_TYPE_NONE);

    segment_list_t *seg = &playlist.segments;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment0.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 0);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552000000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552010000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);
    
    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment1.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 1);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552010000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552020000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment2.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 2);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552020000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552030000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment3.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 3);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552030000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552040000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment4.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 4);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552040000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552050000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment5.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 5);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 0);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552050000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552060000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment6.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 6);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552060000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552070000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "http://www.example.com/segment7.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 7);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, 1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1266552070000);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1266552080000);
    CU_ASSERT_EQUAL(seg->data->duration, 10.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_EQUAL(seg, NULL);

    hlsparse_media_playlist_term(&playlist);
}

void media_playlist_parse_test3(void)
{
    media_playlist_t playlist;
    hlsparse_media_playlist_init(&playlist);

    const char *src = "#EXTM3U\n\
#EXT-X-VERSION:3\n\
#EXT-X-TARGETDURATION:6\n\
#EXT-X-MEDIA-SEQUENCE:1\n\
#EXT-X-PLAYLIST-TYPE:VOD\n\
#EXT-X-PROGRAM-DATE-TIME:2018-08-11T21:42:39.900Z\n\
#EXT-X-ASSET-START:id=987,pop=\n\
#EXT-X-KEY:METHOD=AES-128,URI=\"https://key-service.com/key?id=123\",IV=0x3CEA3CC17B919213B8E7CFB5D10D4CAE\n\
#EXTINF:0.033,\n\
ADAP/00060/1001_ADAP_00001.ts\n\
#EXT-X-KEY:METHOD=NONE\n\
#EXT-X-CUE-OUT:_params=\"abc=d&efg=MIDROLL&pop=1\"\n\
#EXT-X-CUE-IN\n\
#EXT-X-KEY:METHOD=AES-128,URI=\"https://key-service.com/key?id=124\",IV=0x3CEA3CC17B919213B8E7CFB5D10D4CAF\n\
#EXTINF:4.972,\n\
ADAP/00060/1001_ADAP_00002.ts\n\
#EXTINF:5.005,\n\
ADAP/00060/1001_ADAP_00003.ts\n\
#EXTINF:4.605,\n\
ADAP/00060/1001_ADAP_00004.ts\n\
#EXT-X-KEY:METHOD=NONE\n\
#EXT-X-CUE-OUT:_fw_params=\"abc=a&efg=POSTROLL&pop=4\"\n\
#EXT-X-CUE-IN\n\
#EXT-X-ENDLIST\n";

    int res = hlsparse_media_playlist(src, strlen(src), &playlist);
    CU_ASSERT_EQUAL(res, strlen(src))
    CU_ASSERT_EQUAL(playlist.m3u, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.target_duration, 6);
    CU_ASSERT_EQUAL(playlist.media_sequence, 1);
    CU_ASSERT_EQUAL(playlist.discontinuity_sequence, 0);
    CU_ASSERT_EQUAL(playlist.playlist_type, PLAYLIST_TYPE_VOD);
    CU_ASSERT_EQUAL(playlist.independent_segments, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.iframes_only, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.start.time_offset, 0.f);
    CU_ASSERT_EQUAL(playlist.start.precise, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.nb_segments, 5);
    CU_ASSERT_EQUAL(playlist.nb_maps, 0);
    CU_ASSERT_EQUAL(playlist.nb_dateranges, 0);
    CU_ASSERT_EQUAL(playlist.end_list, HLS_TRUE);

    segment_list_t *seg = &playlist.segments;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "ADAP/00060/1001_ADAP_00001.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 0);
    CU_ASSERT_EQUAL(seg->data->key_index, 0);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1534023759900);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1534023759933);
    CU_ASSERT_EQUAL(seg->data->duration, 0.033f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_NOT_EQUAL(seg->data->custom_tags.data, NULL);
    assert_string_equal(seg->data->custom_tags.data, "EXT-X-ASSET-START:id=987,pop=", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);
    
    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "ADAP/00060/1001_ADAP_00002.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 1);
    CU_ASSERT_EQUAL(seg->data->key_index, 2);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1534023759933);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1534023764905);
    CU_ASSERT_EQUAL(seg->data->duration, 4.972f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_NOT_EQUAL(seg->data->custom_tags.data, NULL);
    assert_string_equal(seg->data->custom_tags.data, "EXT-X-CUE-OUT:_params=\"abc=d&efg=MIDROLL&pop=1\"", __func__, __LINE__);
    CU_ASSERT_NOT_EQUAL(seg->data->custom_tags.next, NULL);
    assert_string_equal(seg->data->custom_tags.next->data, "EXT-X-CUE-IN", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next->next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "ADAP/00060/1001_ADAP_00003.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 2);
    CU_ASSERT_EQUAL(seg->data->key_index, 2);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1534023764905);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1534023769910);
    CU_ASSERT_EQUAL(seg->data->duration, 5.005f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    assert_string_equal(seg->data->uri, "ADAP/00060/1001_ADAP_00004.ts", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 3);
    CU_ASSERT_EQUAL(seg->data->key_index, 2);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1534023769910);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1534023774515);
    CU_ASSERT_EQUAL(seg->data->duration, 4.605f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_EQUAL(seg->data->custom_tags.data, NULL);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next, NULL);

    seg = seg->next;
    CU_ASSERT_NOT_EQUAL(seg->data, NULL);
    CU_ASSERT_EQUAL(seg->data->uri, NULL);
    CU_ASSERT_EQUAL(seg->data->sequence_num, 4);
    CU_ASSERT_EQUAL(seg->data->key_index, 3);
    CU_ASSERT_EQUAL(seg->data->map_index, -1);
    CU_ASSERT_EQUAL(seg->data->daterange_index, -1);
    CU_ASSERT_EQUAL(seg->data->title, NULL);
    CU_ASSERT_EQUAL(seg->data->discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt_discontinuity, HLS_FALSE);
    CU_ASSERT_EQUAL(seg->data->pdt, 1534023774515);
    CU_ASSERT_EQUAL(seg->data->pdt_end, 1534023774515);
    CU_ASSERT_EQUAL(seg->data->duration, 0.f);
    CU_ASSERT_EQUAL(seg->data->byte_range.n, 0);
    CU_ASSERT_EQUAL(seg->data->byte_range.o, 0);
    CU_ASSERT_NOT_EQUAL(seg->data->custom_tags.data, NULL);
    assert_string_equal(seg->data->custom_tags.data, "EXT-X-CUE-OUT:_fw_params=\"abc=a&efg=POSTROLL&pop=4\"", __func__, __LINE__);
    CU_ASSERT_NOT_EQUAL(seg->data->custom_tags.next, NULL);
    assert_string_equal(seg->data->custom_tags.next->data, "EXT-X-CUE-IN", __func__, __LINE__);
    CU_ASSERT_EQUAL(seg->data->custom_tags.next->next, NULL);

    seg = seg->next;
    CU_ASSERT_EQUAL(seg, NULL);

    hlsparse_media_playlist_term(&playlist);
}

void media_playlist_parse_test4(void)
{
    media_playlist_t playlist;
    hlsparse_media_playlist_init(&playlist);

    const char *src = "#EXTM3U\n\
#EXT-X-VERSION:12\n\
#EXT-X-TARGETDURATION:6\n\
#EXT-X-PART-INF:PART-TARGET=3.1\n\
#EXT-X-MEDIA-SEQUENCE:1\n\
#EXT-X-PLAYLIST-TYPE:VOD\n\
#EXT-X-DEFINE:NAME=\"var1\",VALUE=\"val1\"\n\
#EXT-X-DEFINE:IMPORT=\"var2\",VALUE=\"val2\"\n\
#EXTINF:0.033,\n\
ADAP/00060/1001_ADAP_00001.ts\n\
#EXTINF:4.972,\n\
ADAP/00060/1001_ADAP_00002.ts\n\
#EXT-X-ENDLIST\n";

    int res = hlsparse_media_playlist(src, strlen(src), &playlist);
    CU_ASSERT_EQUAL(res, strlen(src))
    CU_ASSERT_EQUAL(playlist.m3u, HLS_TRUE);
    CU_ASSERT_EQUAL(playlist.version, 12);
    CU_ASSERT_EQUAL(playlist.target_duration, 6);
    CU_ASSERT_EQUAL(playlist.part_target_duration, 3.1f);
    CU_ASSERT_EQUAL(playlist.media_sequence, 1);
    CU_ASSERT_EQUAL(playlist.discontinuity_sequence, 0);
    CU_ASSERT_EQUAL(playlist.playlist_type, PLAYLIST_TYPE_VOD);
    CU_ASSERT_EQUAL(playlist.independent_segments, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.iframes_only, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.start.time_offset, 0.f);
    CU_ASSERT_EQUAL(playlist.start.precise, HLS_FALSE);
    CU_ASSERT_EQUAL(playlist.nb_segments, 2);
    CU_ASSERT_EQUAL(playlist.nb_maps, 0);
    CU_ASSERT_EQUAL(playlist.nb_dateranges, 0);
    CU_ASSERT_EQUAL(playlist.nb_defines, 2);
    CU_ASSERT_EQUAL(playlist.end_list, HLS_TRUE);

    CU_ASSERT_EQUAL(strcmp("var1", playlist.defines.data->key), 0);
    CU_ASSERT_EQUAL(strcmp("val1", playlist.defines.data->value), 0);
    CU_ASSERT_EQUAL(playlist.defines.data->type, DEFINE_TYPE_NAME);

    CU_ASSERT_EQUAL(strcmp("var2", playlist.defines.next->data->key), 0);
    CU_ASSERT_EQUAL(strcmp("val2", playlist.defines.next->data->value), 0);
    CU_ASSERT_EQUAL(playlist.defines.next->data->type, DEFINE_TYPE_IMPORT);

    hlsparse_media_playlist_term(&playlist);
}

void setup(void)
{
    hlsparse_global_init();

    suite("parser_playlist", NULL, NULL);
    test("playlist_init", playlist_init_test);
    test("playlist_term", playlist_term_test);
    test("playlist_parse", playlist_parse_test);
    test("media_playlist_init", media_playlist_init_test);
    test("media_playlist_term", media_playlist_term_test);
    test("media_playlist_parse", media_playlist_parse_test);
    test("media_playlist_parse2", media_playlist_parse_test2);
    test("media_playlist_parse3", media_playlist_parse_test3);
    test("media_playlist_parse4", media_playlist_parse_test4);
}

