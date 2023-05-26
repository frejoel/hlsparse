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