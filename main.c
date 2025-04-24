#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

typedef struct {
    char magic[4];
    int width;
    int height;
    int frame_count;
} ToyHeader;

const char *gen_file = "test.toy";
const int width = 256;
const int height = 256;
const int frame_count = 16;


void generate_toy() {
    printf("Generating a toy file\n");

    FILE *file = fopen(gen_file, "wb");
    if (!file) {
        printf("ERROR: Failed to open '%s'\n", gen_file);
        return;
    }

    ToyHeader hdr = {{'T', 'O', 'Y', '0'}, width, height, frame_count};
    fwrite(&hdr, sizeof(hdr), 1, file);
    uint8_t *buf = malloc(width*height);
    if (!buf) {
        printf("ERROR: Failed to allocate memory\n");
        return;
    }

    for (int i = 0; i < frame_count; i++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                buf[y * width + x] = (x % 256);
            }
        }
        fwrite(buf, width * height, 1, file);
    }

    free(buf);
    fclose(file);
    printf("File generated\n");
}

void play_toy(char *input) {
    printf("Reading file %s\n", input);

    FILE *file = fopen(input, "rb");
    if (!file) {
        printf("ERROR: Failed to open '%s' file\n", input);
        return;
    }

    ToyHeader hdr;
    if (fread(&hdr, sizeof(hdr), 1, file) != 1) {
        printf("ERROR: Failed to read header\n");
        return;
    }

    if (memcmp(hdr.magic, "TOY0", 4) != 0) {
        printf("ERROR: Failed to find toy magic\n");
        return;
    }

    int frame_size = hdr.width * hdr.height;
    uint8_t *buf = malloc(frame_size);
    Color *pixels = malloc(sizeof(Color) * frame_size);

    if (fread(buf, frame_size, 1, file) != 1) {
        if (feof(file)) {
            printf("ERROR: Hit end-of-file unexpectedly\n");
        }
        if (ferror(file)) {
            perror("ERROR: fread failed");
        }
        printf("ERROR: Unexpected end of file\n");
        return;
    }

    uint8_t c;
    for (int i = 0; i < frame_size; i++) {
        c = buf[i];
        pixels[i].r = c;
        pixels[i].g = c;
        pixels[i].b = c;
        pixels[i].a = 255;
    }


    InitWindow(hdr.width, hdr.height, "Clayer");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < hdr.height; y++) {
            for (int x = 0; x < hdr.width; x++) {
                Color pixel = pixels[y * hdr.width + x];
                DrawPixel(x, y, pixel);
            }
        }

        EndDrawing();
    }

    fclose(file);
    CloseWindow();
}

int main(int argc, char *argv[]) {
    const char *program = argv[0];

    if (argc < 2) {
        printf("Usage:\n");
        printf("Generate file: --generate\n");
        printf("Open file: %s <video_file>\n", program);
        return 1;
    }

    char *input = argv[1];

    if (strcmp(input, "--generate") == 0) {
        generate_toy();
        return 0;
    }

    play_toy(input);

    return 0;
}
