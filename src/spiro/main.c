#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "spiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMG_WIDTH  800
#define IMG_HEIGHT 800

void save_png(const char *filename, unsigned char *pixels, int width, int height) {
    if (stbi_write_png(filename, width, height, 3, pixels, width * 3) == 0) {
        printf("Failed to save PNG to %s\n", filename);
    } else {
        printf("Image saved to %s\n", filename);
    }
}

void draw_pixel(unsigned char *pixels, int width, int height, int x, int y, rgb_color_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = (y * width + x) * 3;
        pixels[idx] = (unsigned char)color.r;
        pixels[idx+1] = (unsigned char)color.g;
        pixels[idx+2] = (unsigned char)color.b;
    }
}

void draw_line(unsigned char *pixels, int width, int height, int x0, int y0, int x1, int y1, rgb_color_t color) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        draw_pixel(pixels, width, height, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

int main(int argc, char **argv) {
    struct spiro_state state;
    init_spir(&state, argc, argv);

    printf("Spirogram initialized. Starting execution...\n");

    long R_int = (long)round(state.R_param);
    long r_int = (long)round(state.r_param);
    
    if (R_int > 0 && r_int > 0) {
        long diff = R_int - r_int;
        if (diff > 0) {
            for (int n = 1; n <= 100; n++) {
                if ((n * diff) % r_int == 0) {
                    state.nRotations_needed = n;
                    break;
                }
            }
        } else {
            state.nRotations_needed = 1;
        }
    } else {
        state.nRotations_needed = 10;
    }

    printf("Required rotations: %d\n", state.nRotations_needed);

    unsigned char *pixels = calloc(IMG_WIDTH * IMG_HEIGHT * 3, 1);
    if (!pixels) {
        perror("Failed to allocate memory for pixels");
        return 1;
    }

    // Fill background with white
    for (int i = 0; i < IMG_WIDTH * IMG_HEIGHT * 3; i += 3) {
        pixels[i] = 255;
        pixels[i+1] = 255;
        pixels[i+2] = 255;
    }

    float angle_step = 1.0f; // Smaller step for smoother lines
    float angle_rad = angle_step * M_PI / 180.0f;

    double total_angle = state.nRotations_needed * 2.0 * M_PI;
    double current_angle = 0;

    double scale = (IMG_WIDTH / 2.0) / (state.R_param * 1.2);

    double prev_x = 0, prev_y = 0;
    int first_point = 1;
    int points_drawn = 0;

    while (current_angle <= total_angle) {
        double a = current_angle;
        double x = state.R_param * ((1.0 - state.k_factor) * cos(a) + state.l_normalized * state.k_factor * cos((1.0 - state.k_factor) * a / state.k_factor));
        double y = state.R_param * ((1.0 - state.k_factor) * sin(a) - state.l_normalized * state.k_factor * sin((1.0 - state.k_factor) * a / state.k_factor));

        int px = (int)(IMG_WIDTH / 2.0 + x * scale);
        int py = (int)(IMG_HEIGHT / 2.0 - y * scale); // y is inverted in image coordinates

        if (!first_point) {
            draw_line(pixels, IMG_WIDTH, IMG_HEIGHT, (int)prev_x, (int)prev_y, px, py, state.color_rgb);
        }

        prev_x = px;
        prev_y = py;
        first_point = 0;
        points_drawn++;
        current_angle += angle_rad;
    }

    printf("Generated %d points.\n", points_drawn);

    save_png("images/spirograph.png", pixels, IMG_WIDTH, IMG_HEIGHT);

    free(pixels);
    return 0;
}
