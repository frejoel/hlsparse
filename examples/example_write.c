#include <hlsparse.h>
#include <stdio.h>
#include <string.h>

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

    // edit some multivariant playlist vaues
    myMultivariant.version = 4;
    myMultivariant.independent_segments = HLS_FALSE;

    // add some stream infs to the multivariant playlist
    stream_inf_t inf[2];
    hlsparse_stream_inf_init(&inf[0]);
    hlsparse_stream_inf_init(&inf[1]);

    inf[0].bandwidth = 800000;
    inf[0].avg_bandwidth = 780000;
    inf[0].codecs = "mp4a.40.2,avc1.4d401e";
    inf[0].resolution.width = 1280;
    inf[0].resolution.height = 720;
    inf[0].frame_rate = 29.97f;
    inf[0].hdcp_level = HDCP_LEVEL_NONE;
    inf[0].audio = "group-one";
    inf[0].video = "group-two";
    inf[0].subtitles = "group-three";
    inf[0].closed_captions = "cc";
    inf[0].uri = "http://www.example.com/variant_01.m3u8";

    // set the first stream inf data
    myMultivariant.stream_infs.data = &inf[0];

    // for the 2nd stream inf we'll need to create a list item
    stream_inf_list_t inf_list;
    inf[1].bandwidth = 1200000;
    inf[1].avg_bandwidth = 1150000;
    inf[1].codecs = "mp4a.40.2,avc1.4d401e";
    inf[1].resolution.width = 1280;
    inf[1].resolution.height = 720;
    inf[1].frame_rate = 29.97f;
    inf[1].hdcp_level = HDCP_LEVEL_TYPE0;
    inf[1].audio = "group-one";
    inf[1].video = "group-two";
    inf[1].subtitles = "group-three";
    inf[1].closed_captions = "cc";
    inf[1].uri = "http://www.example.com/variant_02.m3u8";

    // set the stream inf data onto the link list node
    inf_list.data = &inf[1];
    // this is the last item in the link list, so make sure next is set to NULL to
    // indicate this
    inf_list.next = NULL;
    // link the 2nd node on to the first node
    myMultivariant.stream_infs.next = &inf_list;

    // pointer to store the output of the HLS playlist text
    char *out = NULL;
    // variable telling us how many bytes are being used in the playlist
    int size = 0;

    // generate the HLS text from our multivariant_playlist_t
    res = hlswrite_multivariant_playlist(&out, &size, &myMultivariant);
    if(res != HLS_OK) {
        fprintf(stderr, "failed to write the playlist\n");
        return -1;
    }

    printf("myMultivariant playlist is %d bytes long\n", size);
    if(size > 0) {
        printf("\n%s\n", out);
    }

    return 0;
}