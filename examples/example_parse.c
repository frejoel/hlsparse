#include <hlsparse.h>
#include <stdio.h>
#include <string.h>

const char *multivariantSrc = "#EXTM3U\n"\
"#EXT-X-VERSION:7\n"\
"#EXT-X-MEDIA:TYPE=VIDEO,URI=\"http://www.test.com\",GROUP-ID=\"groupId\",NAME=\"media\"\n"\
"#EXT-X-STREAM-INF:BANDWIDTH=900000,CODECS=\"mp4a.40.2,avc1.4d401e\",RESOLUTION=1280x720\n"\
"/path/to/stream_inf/900.m3u8\n"\
"#EXT-X-STREAM-INF:BANDWIDTH=1500000,CODECS=\"mp4a.40.2.avc1.4d401e\",RESOLUTION=1920x1080\n"\
"/path/to/stream_inf/1500.m3u8\n"\
"#EXT-X-I-FRAME-STREAM-INF:URI=\"http://www.test.com\",BANDWIDTH=900000\n"\
"#EXT-X-SESSION-DATA:DATA-ID=\"com.example.movie.title\",VALUE=\"value\",URI=\"http://www.test.com/data.json\""\
"#EXT-X-SESSION-KEY:METHOD=AES-128,URI=\"http://www.test.com/key\""\
"#EXT-X-INDEPENDENT-SEGMENTS\n"\
"#EXT-X-START:TIME-OFFSET=10.0,PRECISE=YES\n";

int main() {
    // Initialize the library
    HLSCode res = hlsparse_global_init();
    if(res != HLS_OK) {
        fprintf(stderr, "failed to initialize hlsparse");
        return -1;
    }

    // create a multivariant playlist structure
    multivariant_playlist_t myMultivariant;
    res = hlsparse_multivariant_playlist_init(&myMultivariant);
    if(res != HLS_OK) {
        fprintf(stderr, "failed to initialize multivariant playlist structure");
        return -1;
    }

    // parse the playlist information into our multivariant structure
    int read = hlsparse_multivariant_playlist(multivariantSrc, strlen(multivariantSrc), &myMultivariant);
    printf("read a total of %d bytes parsing the multivariant playlist source\n", read);

    // print out all the StreamInf bitrates that were found
    stream_inf_list_t *streamInf = &myMultivariant.stream_infs;
    int count = 0;
    while(streamInf) {
        printf("StreamInf %d Uri: %s\n", count, streamInf->data->uri);
        printf("StreamInf %d Bandwidth: %f\n", count, streamInf->data->bandwidth);
        ++count;
        streamInf = streamInf->next;
    }

    return 0;
}
