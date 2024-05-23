/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

#include "pbm.h"
#include <stdio.h>
#include <stdlib.h>

// Allocates the memory for a new ppm image
PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PPMImage *newPPM = (PPMImage*) malloc(sizeof(PPMImage));
    newPPM->height = h;
    newPPM->width = w;
    newPPM->max = m;

    // Allocate the memory for 3 2D arrays (for the red, green, and blue values)
    for(unsigned int i = 0; i < 3; i++) {
        newPPM->pixmap[i] = (unsigned int**) malloc(h * sizeof(unsigned int*));

        for(unsigned int j = 0; j < h; j++) {
            newPPM->pixmap[i][j] = (unsigned int *) malloc(w * sizeof(unsigned int)); 
        }
    }

    return newPPM;
}

// Allocates the memory for a new pbm image
PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage *newPBM = (PBMImage*) malloc(sizeof(PBMImage));
    newPBM->height = h;
    newPBM->width = w;

    //Allocate memory for the pixmap
    newPBM->pixmap = (unsigned int **) malloc(h * sizeof(unsigned int*));
    for(unsigned int i = 0; i < h; i++) {
        newPBM->pixmap[i] = (unsigned int *) malloc(w * sizeof(unsigned int*)); 
    }

    return newPBM;
}

// Allocates the memory for a new pgm image
PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PGMImage *newPGM = (PGMImage *) malloc(sizeof(PGMImage));
    newPGM->width = w;
    newPGM->height = h;
    newPGM->max = m;

    //Allocate memory for the pixmap
    newPGM->pixmap = (unsigned int **) malloc(h * sizeof(unsigned int*));
    for(unsigned int i = 0; i < h; i++) {
        newPGM->pixmap[i] = (unsigned int *) malloc(w * sizeof(unsigned int*)); 
    }
   
    return newPGM;
}

// Frees ppm image
void del_ppmimage( PPMImage * p )
{
    // Free each of the 3 2D arrays
    unsigned int height = p->height;
    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < height; j++) {
            free(p->pixmap[i][j]);
        }

        free(p->pixmap[i]);
    }

    free(p);
}

// Frees pgm memory (including the struct pointer)
void del_pgmimage( PGMImage * p )
{
    // Free the 2D array
    unsigned int height = p->height; 
    for(unsigned int i = 0; i < height; i++) {
        free(p->pixmap[i]);
    }
    free(p->pixmap);

    free(p);
}

// Frees pbm memory (including the struct pointer)
void del_pbmimage( PBMImage * p )
{
    // Free the 2D array
    unsigned int height = p->height; 
    for(unsigned int i = 0; i < height; i++) {
        free(p->pixmap[i]);
    }
    free(p->pixmap);

    free(p);
}