/*
 * pyspiro - C Implementation of Python Turtle Spirograph Program
 * Converted by human from Python to native C (no AI co-authoring)
 */

#include "spiro/spiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

void init_spir(struct spiro_state *s, int argc, char **argv) {
    srand((unsigned)time(NULL));
    
    // Default values
    s->R_param = 100.0f;
    s->r_param = 50.0f;
    s->l_normalized = 0.5f;
    s->color_rgb.r = 0; // Changed to black for visibility
    s->color_rgb.g = 0;
    s->color_rgb.b = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--sparam") == 0) {
            if (i + 3 < argc) {
                s->R_param = atof(argv[i+1]);
                s->r_param = atof(argv[i+2]);
                s->l_normalized = atof(argv[i+3]);
                i += 3;
            } else {
                fprintf(stderr, "Error: --sparam requires 3 arguments: R r l\n");
                exit(1);
            }
        }
    }

    s->k_factor = s->r_param / s->R_param;
    
    s->nRotations_needed = 10; // placeholder
}
