# HLS Parse
Library for parsing an HLS master and media playlist into a C object structure.
Supports v13 of the 2nd edition specification.
See https://datatracker.ietf.org/doc/html/draft-pantos-hls-rfc8216bis-13 for details.

## Building
Run `make static` to build a static library. See the generated `bin` directory.
Run `make check` to build and run the tests.

## Example
For a more thorough example see `examples/example.c` in the source code.
```
    // Global initialization of the library.
    HLSCode res = hlsparse_global_init();

    // create a master playlist structure.
    master_t myMaster;
    res = hlsparse_master_init(&myMaster);

    // parse some playlist data into our master structure.
    masterSrc = /* char * utf8 master playlist string */
    int read = hlsparse_master(masterSrc, strlen(masterSrc), &myMaster);
```

## Notes
### EXT-X-GAP
Gaps are identified by the segment type.
```
segment_t* seg = ...
if (seg->type & SEGMENT_TYPE_GAP) {
    ...
```
### EXT-X-BITRATE
If the segment bitrate is specificed as `0`, it means no bitrate tag was specified.
```
segment_t* seg = ...
if (seg->bitrate > 0) {
    // bitrate is valid / specified
    ...
```

### EXT-X-DEFINE
Read the `type` of the `define_t` to understand whether the variable is a `NAME`, `IMPORT` or `QUERYPARAM` define. The `key` contains the value used in these 3 parameters. The `type` will be `DEFINE_TYPE_INVALID` if an unknown variable name is used or not specified.
```
define_t* def = ...
if (def->type == DEFINE_TYPE_IMPORT) {
    if (strcmp(def->key, "myKey") == 0) {
        printf("%s\n", def->value);
        ...
```