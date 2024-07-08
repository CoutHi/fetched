#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // Ensure math.h is included

// Define the ASCII characters to map from dark to light
const char *ASCII_CHARS = " %#*+=-:. ";

// Function to convert an image to grayscale
unsigned char* convert_to_grayscale(unsigned char* img, int width, int height, int channels) {
    int img_size = width * height;
    unsigned char* gray_img = (unsigned char*)malloc(img_size);
    
    for (int i = 0; i < img_size; ++i) {
        int r = img[i * channels];
        int g = img[i * channels + 1];
        int b = img[i * channels + 2];
        // Use the luminosity method to convert to grayscale
        gray_img[i] = (unsigned char)(0.21 * r + 0.72 * g + 0.07 * b);
    }

    return gray_img;
}

// Function to resize an image using nearest-neighbor scaling
unsigned char* resize_image(unsigned char* img, int orig_width, int orig_height, int new_width, int new_height) {
    unsigned char* resized_img = (unsigned char*)malloc(new_width * new_height);
    float x_ratio = (float)orig_width / new_width;
    float y_ratio = (float)orig_height / new_height;
    
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            int orig_x = (int)(x * x_ratio);
            int orig_y = (int)(y * y_ratio);
            resized_img[y * new_width + x] = img[orig_y * orig_width + orig_x];
        }
    }
    
    return resized_img;
}

// Function to map grayscale values to ASCII characters
void map_to_ascii(unsigned char* gray_img, int width, int height) {
    int ascii_length = strlen(ASCII_CHARS) - 1;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixel_index = y * width + x;
            unsigned char pixel_value = gray_img[pixel_index];
            char ascii_char = ASCII_CHARS[(pixel_value * ascii_length) / 255];
            putchar(ascii_char);
        }
        putchar('\n');
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <image_path> <output_width> <output_height>\n", argv[0]);
        return 1;
    }

    int output_width = atoi(argv[2]);
    int output_height = atoi(argv[3]);

    int width, height, channels;
    unsigned char* img = stbi_load(argv[1], &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error loading image: %s\n", stbi_failure_reason());
        return 1;
    }

    unsigned char* gray_img = convert_to_grayscale(img, width, height, channels);
    unsigned char* resized_gray_img = resize_image(gray_img, width, height, output_width, output_height);

    map_to_ascii(resized_gray_img, output_width, output_height);

    stbi_image_free(img);
    free(gray_img);
    free(resized_gray_img);

    return 0;
}

