#include <hlsparse.h>
#include <stdio.h>
#include <string.h>

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = (char *) malloc(sizeof(char) * (file_size + 1));
    if (buffer == NULL) {
        fclose(file);
        fprintf(stderr, "Failed to allocate memory for file '%s'\n", filename);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read != file_size) {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Failed to read file '%s'\n", filename);
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

int main()
{
    // Initialize the library
    HLSCode res = hlsparse_global_init();
    if (res != HLS_OK) {
        fprintf(stderr, "failed to initialize hlsparse");
        return -1;
    }

    // create a multivariant playlist structure
    multivariant_playlist_t myMultivariant;
    res = hlsparse_multivariant_playlist_init(&myMultivariant);
    if (res != HLS_OK) {
        fprintf(stderr, "failed to initialize multivariant playlist structure");
        return -1;
    }

    char *m3u8 = read_file("test_multivariant.m3u8");
    printf("test_multivariant.m3u8\n%s", m3u8);

    const char *multivariantSrc = m3u8;
    // parse the playlist information into our multivariant structure
    int read = hlsparse_multivariant_playlist(multivariantSrc, strlen(multivariantSrc), &myMultivariant);
    printf("read a total of %d bytes parsing the multivariant playlist source\n", read);

    printf("num of streams: %d, num of iframe_streams, %d, num of keys: %d\n", myMultivariant.nb_stream_infs, myMultivariant.nb_iframe_stream_infs, myMultivariant.nb_session_keys);

    // print out all the StreamInf bitrates that were found
    stream_inf_list_t *streamInf = &myMultivariant.stream_infs;
    int count = 0;
    while (streamInf && streamInf->data) {
        printf("StreamInf %d Uri: %s\n", count, streamInf->data->uri);
        printf("StreamInf %d Bandwidth: %f\n", count, streamInf->data->bandwidth);
        printf("StreamInf %d Codec: %s\n", count, streamInf->data->codecs);
        printf("StreamInf %d Audio: %s\n", count, streamInf->data->audio);
        printf("StreamInf %d Video: %s\n", count, streamInf->data->video);
        printf("StreamInf %d WxH: %dx%d\n", count, streamInf->data->resolution.width, streamInf->data->resolution.height);
        ++count;
        streamInf = streamInf->next;
    }

    media_list_t *mediaInf = &myMultivariant.media;
    count = 0;
    while (mediaInf && mediaInf->data) {
        printf("Media %d Uri: %s\n", count, mediaInf->data->uri);
        ++count;
        mediaInf = mediaInf->next;
    }

    session_data_list_t *sessInf= &myMultivariant.session_data;
    count = 0;
    while (sessInf && sessInf->data) {
        printf("Sess %d Uri: %s\n", count, sessInf->data->uri);
        ++count;
        sessInf = sessInf->next;
    }


    free(m3u8);

    return 0;
}
