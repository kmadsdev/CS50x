#include "helpers.h"
#include <math.h>

void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    int avg;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            avg = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
        }
    }
}

void sepia(int height, int width, RGBTRIPLE image[height][width]) {
    int originalRed;
    int originalGreen;
    int originalBlue;

    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            originalRed = image[h][w].rgbtRed;
            originalGreen = image[h][w].rgbtGreen;
            originalBlue = image[h][w].rgbtBlue;

            sepiaRed = round(
                0.393 * originalRed +
                0.769 * originalGreen +
                0.189 * originalBlue
            );
            sepiaGreen = round(
                0.349 * originalRed +
                0.686 * originalGreen +
                0.168 * originalBlue
            );
            sepiaBlue = round(
                0.272 * originalRed +
                0.534 * originalGreen +
                0.131 * originalBlue
            );

            image[h][w].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            image[h][w].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[h][w].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        }
    }
}

void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width / 2; w++) {
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - 1 - w];
            image[h][width - 1 - w] = temp;
        }
    }
}

void blur(int height, int width, RGBTRIPLE image[height][width]) {
    RGBTRIPLE temp[height][width];
    int totalRed;
    int totalGreen;
    int totalBlue;
    int count;
    int nh;
    int nw;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            totalRed = 0, totalGreen = 0, totalBlue = 0;
            count = 0;

            for (int dh = -1; dh <= 1; dh++) {
                for (int dw = -1; dw <= 1; dw++) {
                    nh = h + dh;
                    nw = w + dw;

                    if (nh >= 0 && nh < height && nw >= 0 && nw < width) {
                        totalRed += image[nh][nw].rgbtRed;
                        totalGreen += image[nh][nw].rgbtGreen;
                        totalBlue += image[nh][nw].rgbtBlue;
                        count++;
                    }
                }
            }

            temp[h][w].rgbtRed = round((float) totalRed / count);
            temp[h][w].rgbtGreen = round((float) totalGreen / count);
            temp[h][w].rgbtBlue = round((float) totalBlue / count);
        }
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            image[h][w] = temp[h][w];
        }
    }
}
