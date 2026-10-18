#ifndef SPIRO_H
#define SPIRO_H

typedef struct {
    float r;
    float g;
    float b;
} rgb_color_t;

struct spiro_state {
    double xc, yc;
    float R_param;
    float r_param;
    float k_factor;
    float l_normalized;
    int nRotations_needed;
    unsigned int points_generated;
    float angle_degrees;
    rgb_color_t color_rgb;
};

void init_spir(struct spiro_state *s, int argc, char **argv);

#endif // SPIRO_H
