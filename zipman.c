#include <stdio.h>
#include <string.h>
#include <zlib.h>

#define CHUNK_SIZE 16384

int compressFile(const char *inputFilename, const char *outputFilename) {
    FILE *input = fopen(inputFilename, "rb");
    if (!input) {
        perror("[ERROR] failed to open input file");
        return 1;
    }

    FILE *output = fopen(outputFilename, "wb");
    if (!output) {
        perror("[ERROR] failed to open output file");
        fclose(input);
        return 1;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
        fclose(input);
        fclose(output);
        return 1;
    }

    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    do {
        stream.avail_in = fread(in, 1, CHUNK_SIZE, input);
        if (ferror(input)) {
            deflateEnd(&stream);
            fclose(input);
            fclose(output);
            return 1;
        }

        if (stream.avail_in == 0)
            break;
        stream.next_in = in;

        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = out;

            if (deflate(&stream, Z_NO_FLUSH) != Z_OK) {
                deflateEnd(&stream);
                fclose(input);
                fclose(output);
                return 1;
            }

            unsigned have = CHUNK_SIZE - stream.avail_out;
            if (fwrite(out, 1, have, output) != have || ferror(output)) {
                deflateEnd(&stream);
                fclose(input);
                fclose(output);
                return 1;
            }
        } while (stream.avail_out == 0);
    } while (1);

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        fclose(input);
        fclose(output);
        return 1;
    }

    unsigned have = CHUNK_SIZE - stream.avail_out;
    if (fwrite(out, 1, have, output) != have || ferror(output)) {
        deflateEnd(&stream);
        fclose(input);
        fclose(output);
        return 1;
    }

    deflateEnd(&stream);
    fclose(input);
    fclose(output);
    return 0;
}

int extractFile(const char *inputFilename, const char *outputFilename) {
    FILE *input = fopen(inputFilename, "rb");
    if (!input) {
        perror("[ERROR] failed to open input file");
        return 1;
    }

    FILE *output = fopen(outputFilename, "wb");
    if (!output) {
        perror("[ERROR] failed to open output file");
        fclose(input);
        return 1;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if (inflateInit(&stream) != Z_OK) {
        fclose(input);
        fclose(output);
        return 1;
    }

    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    int ret;
    do {
        stream.avail_in = fread(in, 1, CHUNK_SIZE, input);
        if (ferror(input)) {
            inflateEnd(&stream);
            fclose(input);
            fclose(output);
            return 1;
        }

        if (stream.avail_in == 0)
            break;
        stream.next_in = in;

        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = out;

            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&stream);
                fclose(input);
                fclose(output);
                return 1;
            }

            unsigned have = CHUNK_SIZE - stream.avail_out;
            if (fwrite(out, 1, have, output) != have || ferror(output)) {
                inflateEnd(&stream);
                fclose(input);
                fclose(output);
                return 1;
            }
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(input);
    fclose(output);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("[?] usage: %s [compress|extract] [input_file] [output_file]\n",
                argv[0]);
        return 1;
    }

    const char *operation = argv[1];
    const char *inputFilename = argv[2];
    const char *outputFilename = argv[3];

    if (strcmp(operation, "compress") == 0) {
        if (compressFile(inputFilename, outputFilename) == 0) {
            printf("[SUCCESS] compression completed successfully\n");
        } else {
            printf("[FAILED] compression failed\n");
        }
    } else if (strcmp(operation, "extract") == 0) {
        if (extractFile(inputFilename, outputFilename) == 0) {
            printf("[FAILED] extraction completed successfully\n");
        } else {
            printf("[FAILED] extraction failed\n");
        }
    } else {
        printf("[ERROR] invalid operation -> use 'compress' or 'extract'\n");
        return 1;
    }

    return 0;
}
