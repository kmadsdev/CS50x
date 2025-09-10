#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL) {
        return 1;
    }

    BYTE buffer[512];

    FILE *outfile = NULL;

    char filename[8];
    int filecount = 0;

    while (fread(buffer, sizeof(BYTE), 512, infile) == 512) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            if (outfile != NULL) {
                fclose(outfile);
            }

            sprintf(filename, "%03i.jpg", filecount);
            outfile = fopen(filename, "w");
            filecount++;
        }

        if (outfile != NULL) {
            fwrite(buffer, sizeof(BYTE), 512, outfile);
        }
    }

    if (outfile != NULL) {
        fclose(outfile);
    }

    fclose(infile);
    return 0;
}
