/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Tommy Skodje */

#include "pbm.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

void convertToPBM(char *, char *);
void convertToPGM(char *, char *, int);
void isolateChannel(char *, char *, char *);
void removeChannel(char *, char *, char *);
void sepia(char *, char *);
void mirrorImage(char *, char *);
void thumbnail(char *, char *, unsigned int);
void tileThumbnails(char *, char *, unsigned int);


int main( int argc, char *argv[] )
{

    int getoptResult;
    int transformations;
    int flagB, flagG, flagI, flagR, flagS, flagM, flagT, flagN, flagO, input = 0;

    // Options for the ppm transformations
    char *gOption = malloc(100 * sizeof(char));
    char *iOption = malloc(100 * sizeof(char));
    char *rOption = malloc(100 * sizeof(char));
    char *tOption = malloc(100 * sizeof(char));
    char *nOption = malloc(100 * sizeof(char));
    char *outputFileName = malloc(100 * sizeof(char));
    char *inputFileName = malloc(100 * sizeof(char));

    // Take the user's input
    while((getoptResult = getopt(argc, argv, ":bg:i:r:smt:n:o:")) != -1) {

        switch(getoptResult) {
            case 'b':
                flagB = 1;
                transformations++;
                break;
            case 'g':
                flagG = 1;
                strcpy(gOption, optarg);
                transformations++;
                break;
            case 'i':
                flagI = 1;
                strcpy(iOption, optarg);
                transformations++;
                break;
            case 'r':
                flagR = 1;
                strcpy(rOption, optarg);
                transformations++;
                break;
            case 's':
                flagS = 1;
                transformations++;
                break;
            case 'm':
                flagM = 1;
                transformations++;
                break;
            case 't':
                flagT = 1;
                strcpy(tOption, optarg);
                transformations++;
                break;
            case 'n':
                flagN = 1;
                strcpy(nOption, optarg);
                transformations++;
                break;
            case 'o':
                flagO = 1;
                strcpy(outputFileName, optarg);
                break;

            //Malformed command line error
            case '?':
                fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(1);
                break;
            
            // Missing argument error
            case ':':
                fprintf(stderr, "Missing argument for option -%c\n", optopt);
                exit(1);
        }
    }

    // Error for invalid channel specification
    if(flagI == 1) {
        if(strcmp(iOption, "red") != 0 && strcmp(iOption, "green") != 0 && strcmp(iOption, "blue") != 0) {
                fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", iOption);
                exit(1);
        }
    }

    if(flagR == 1) {
        if(strcmp(rOption, "red") != 0 && strcmp(rOption, "green") != 0 && strcmp(rOption, "blue") != 0) {
                fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", rOption);
                exit(1);
        }
    }


    // Error for invalid maximum grayscale pixel value
    if(flagG == 1) {

        if(strtol(gOption, NULL, 10) > 65535 || strtol(gOption, NULL, 10) < 1) {
            fprintf(stderr, "Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", gOption);
            exit(1);
        }
    }


    // Error for invalid scale factor
    if(flagT == 1) {
        int errorNum = (int)strtol(tOption, NULL, 10);

        if(strtol(tOption, NULL, 10) > 8 || strtol(tOption, NULL, 10) < 1) {
            fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", errorNum);
            exit(1);
        }
    }

    if(flagN == 1) {

        if(strtol(nOption, NULL, 10) > 8 || strtol(nOption, NULL, 10) < 1) {
            fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", atoi(nOption));
            exit(1);
        }
    }

    // Error for no output file
    if(flagO == 0) {
        fprintf(stderr, "Error: No output file specified\n");
        exit(1);
    }

    // Error for no input file
    if(optind >= argc) {
        fprintf(stderr, "Error: No input file specified\n");
        exit(1);
    } 

    //Error for multiple transformations specified
    if(transformations > 1) {
        fprintf(stderr, "Error: Multiple transformations specified\n");
        exit(1);
    }
    

    strcpy(inputFileName, argv[optind]);

    // Default case (convert to PBM).
    if(flagB == 1 || ((flagB != 1) && (flagG != 1) && (flagI != 1) && (flagM != 1)
                   && (flagN != 1) && (flagR != 1) && (flagS != 1) && (flagT != 1))) { 
        convertToPBM(inputFileName, outputFileName);
    }

    // Convert to grayscale
    if(flagG == 1) {
        int maxGrayScale = (int)strtol(gOption, NULL, 10);
        convertToPGM(inputFileName, outputFileName, maxGrayScale);
    }
    // Isolate specified color channel
    if(flagI == 1) {
        isolateChannel(inputFileName, outputFileName, iOption);
    }

    // Remove specified color channel
    if(flagR == 1) {
        removeChannel(inputFileName, outputFileName, rOption);
    }

    // Add sepia filter
    if(flagS == 1) {
        sepia(inputFileName, outputFileName);
    }

    // Mirror the image
    if(flagM == 1) {
        mirrorImage(inputFileName, outputFileName);
    }

    // Create a thumbnail of the image
    if(flagT == 1) {
        unsigned int scaleFactor = (unsigned int)strtol(tOption, NULL, 10);
        thumbnail(inputFileName, outputFileName, scaleFactor);
    }

    // Tile thumbnails of the image
    if(flagN == 1) {
        unsigned int scaleFactor = (unsigned int)strtol(nOption, NULL, 10);
        tileThumbnails(inputFileName, outputFileName, scaleFactor);
    }

    free(gOption);
    free(iOption);
    free(rOption);
    free(tOption);
    free(nOption);
    free(outputFileName);
    free(inputFileName);
    
    return 0;
}

// Converts the input PPM file to PBM
void convertToPBM(char *inFileName, char *outFileName) {

    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    double halfMax = (inputImage->max / 2.0);
    unsigned int ppmAvgValue;
    unsigned int pbmPixelValue;

    PBMImage *outputImage = new_pbmimage(width, height);

        /* Calculate the PBM value by averaging the values of the RGB values from the PPM
           file, then compare that to half of the maximum ppm value.
        */
        for(unsigned int j = 0; j < height; j++) {
            for(unsigned int i = 0; i < width; i++) {
                double ppmAvgValue = (inputImage->pixmap[0][j][i]
                                    + inputImage->pixmap[1][j][i] 
                                    + inputImage->pixmap[2][j][i]) / 3.0;

                int ppmAvgInt = (int)ppmAvgValue;

                /* If the average value of the pixel is less than half
                   of the max ppm value of the input file, place a 1.
                   Otherwise, place a 0.
                */               
                unsigned int pixelValue = (ppmAvgValue < halfMax);
                outputImage->pixmap[j][i] = pixelValue;
            }
        }

        write_pbmfile(outputImage, outFileName);

        del_ppmimage(inputImage);
        del_pbmimage(outputImage);
}

// Converts the input PPM file to PGM using the specified maximum grayscale value.
void convertToPGM(char *inFileName, char *outFileName, int grayscaleValue) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int ppmMax = inputImage->max;

    PGMImage *outputImage = new_pgmimage(width, height, grayscaleValue);

    /* Calculate grayscale value by averaging the RGB channels from the PPM image, 
       then multiplying that by the inputted maximum grayscale value.
    */
    for(unsigned int j = 0; j < height; j++) {
            for(unsigned int i = 0; i < width; i++) {
                double ppmAvgValue = (inputImage->pixmap[0][j][i] + 
                inputImage->pixmap[1][j][i] + inputImage->pixmap[2][j][i]) / 3.0;

                outputImage->pixmap[j][i] = (ppmAvgValue / ppmMax) * grayscaleValue;
            }
        }

    write_pgmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_pgmimage(outputImage);
}


// Isolates the specified RGB channel (red, green, or blue).
void isolateChannel(char *inFileName, char *outFileName, char *color) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int max = inputImage->max;
    int channelToIsolate;

    PPMImage *outputImage = new_ppmimage(width, height, max);

    // Set the channel to isolate based on the user's input
    if(strcmp(color, "red") == 0) {
        channelToIsolate = 0;
    } else if(strcmp(color, "green") == 0) {
        channelToIsolate = 1;
    } else {
        channelToIsolate = 2;
    }

    /* Set the values of 2 of the 3 channels to 0.
       For the channel to isolate, copy the values from the input image. 
    */
    for(unsigned int k = 0; k < 3; k++) { 

        if(k != channelToIsolate) {
            for(unsigned int j = 0; j < height; j++) {
                    for(unsigned int i = 0; i < width; i++) {
                        outputImage->pixmap[k][j][i] = 0;
                    }
            }

        } else {
            for(unsigned int j = 0; j < height; j++) {
                for(unsigned int i = 0; i < width; i++) {
                    outputImage->pixmap[k][j][i] = inputImage->pixmap[k][j][i];
                }
            }
        }

    }

    write_ppmfile(outputImage, outFileName);

    
    del_ppmimage(inputImage);
    del_ppmimage(outputImage);
}

// Removes the specified RGB channel (red, green, or blue)
void removeChannel(char *inFileName, char *outFileName, char *color) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int max = inputImage->max;
    int channelToRemove;

    PPMImage *outputImage = new_ppmimage(width, height, max);

    // Set the channel to remove based on the user's input.
    if(strcmp(color, "red") == 0) {
        channelToRemove = 0;
    } else if(strcmp(color, "green") == 0) {
        channelToRemove = 1;
    } else {
        channelToRemove = 2;
    }


    /* Set the values for the channel to remove to 0.
       For the other 2 channels, copy the values from the input image. 
    */
    for(unsigned int k = 0; k < 3; k++) {
        if(k == channelToRemove) {
            for(unsigned int j = 0; j < height; j++) {
                    for(unsigned int i = 0; i < width; i++) {
                        outputImage->pixmap[k][j][i] = 0;
                    }
            }
        } else {
            for(unsigned int j = 0; j < height; j++) {
                for(unsigned int i = 0; i < width; i++) {
                    outputImage->pixmap[k][j][i] = inputImage->pixmap[k][j][i];
                }
            }
        }
    }

    write_ppmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_ppmimage(outputImage);

}

// Converts a ppm file to a sepia version of that image.
void sepia(char *inFileName, char *outFileName) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int max = inputImage->max;
    double newRed;
    double newGreen;
    double newBlue;

    PPMImage *outputImage = new_ppmimage(width, height, max);

        
        /* Apply the formula for the sepia transformations for each
           pixel in the ppm file.
        */
    for(unsigned int j = 0; j < height; j++) {
        for(unsigned int i = 0; i < width; i++) {
            newRed = (0.393 * inputImage->pixmap[0][j][i]) + 
            (0.769 * inputImage->pixmap[1][j][i]) + 
            (0.189 * inputImage->pixmap[2][j][i]);

            newGreen = (0.349 * inputImage->pixmap[0][j][i]) + 
            (0.686 * inputImage->pixmap[1][j][i]) + 
            (0.168 * inputImage->pixmap[2][j][i]);

            newBlue = (0.272 * inputImage->pixmap[0][j][i]) + 
            (0.534 * inputImage->pixmap[1][j][i]) + 
            (0.131 * inputImage->pixmap[2][j][i]);

            /* If any of the new color values exceed tha max, correct
               them by setting them to the max.
            */
            if(newRed > max) {
                newRed = max;
            }
            if(newGreen > max) {
                newGreen = max;
            }
            if(newBlue > max) {
                newBlue = max;
            }

            outputImage->pixmap[0][j][i] = newRed;
            outputImage->pixmap[1][j][i] = newGreen;
            outputImage->pixmap[2][j][i] = newBlue;
        }
    }

    write_ppmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_ppmimage(outputImage);
}

// Vertically mirrors the input image
void mirrorImage(char *inFileName, char *outFileName) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int max = inputImage->max;

    PPMImage *outputImage = new_ppmimage(width, height, max);

    /* Write the left side of the image normally, and then 
       copy the same pixel to the other side of the image.
    */

   // Case for if the width of the image is odd
   if(width % 2 == 1) {
        for(unsigned int k = 0; k < 3; k++) {
            for(unsigned int j = 0; j < height; j++) {
                for(unsigned int i = 0; i <= (width / 2); i++) {
                    outputImage->pixmap[k][j][i] = 
                    inputImage->pixmap[k][j][i];

                    outputImage->pixmap[k][j][width - i - 1]
                    = inputImage->pixmap[k][j][i];
                }
            }
        }

   // Case for if the width of the image is even
   } else {
        for(unsigned int k = 0; k < 3; k++) {
            for(unsigned int j = 0; j < height; j++) {
                for(unsigned int i = 0; i < (width / 2); i++) {
                    outputImage->pixmap[k][j][i]
                    = inputImage->pixmap[k][j][i];

                    outputImage->pixmap[k][j][width - i - 1]
                    = inputImage->pixmap[k][j][i];
                }
            }
        }
   }

    write_ppmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_ppmimage(outputImage);
}

// Creates a thumbnail that is (1 / scaleFactor) the size of the original image. 
void thumbnail(char *inFileName, char *outFileName, unsigned int scaleFactor) {
    PPMImage *inputImage = read_ppmfile(inFileName);
    float heightFloat = ((float)inputImage->height / (float)scaleFactor);
    float widthFloat = ((float)inputImage->width / (float)scaleFactor);
    unsigned int max = inputImage->max;

    // Round up the height and width if they're not exact integers.
    if(heightFloat - (int)heightFloat > 0.0) {
        heightFloat++;
    }
    if(widthFloat - (int)widthFloat > 0.0) {
        widthFloat++;
    }

    unsigned int height = (unsigned int)heightFloat;
    unsigned int width = (unsigned int)widthFloat;

    PPMImage *outputImage = new_ppmimage(width, height, max);

    /* These variables keep track of the pixel offset
       (which pixels are to be taken from the original image). 
     */
    unsigned int heightIndex, widthIndex = 0;

    // Print every nth pixel, where n is the scaleFactor.
    for(unsigned int k = 0; k < 3; k++) {
        for(unsigned int j = 0; j < height; j++) {
            heightIndex = j * scaleFactor;
            for(unsigned int i = 0; i < width; i++) {
                outputImage->pixmap[k][j][i] = 
                inputImage->pixmap[k][heightIndex][widthIndex];

                widthIndex += scaleFactor;
            }
            heightIndex += scaleFactor;
            widthIndex = 0;
        }
    }

    write_ppmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_ppmimage(outputImage);
}

/* Tiles thumbnails of the input ppm file based on the scale factor (1-8)
   in an nxn "grid" where n is the scale factor.
*/
void tileThumbnails(char *inFileName, char *outFileName, unsigned int scaleFactor) {

    PPMImage *inputImage = read_ppmfile(inFileName);
    unsigned int height = inputImage->height;
    unsigned int width = inputImage->width;
    unsigned int max = inputImage->max;
    float tileHeightFloat = (float)height / (float)scaleFactor;
    float tileWidthFloat = (float)width / (float)scaleFactor;

    // Round up the height and width if they're not exact integers.
    if(tileHeightFloat - (int)tileHeightFloat > 0.0) {
        tileHeightFloat++;
    }
    if(tileWidthFloat - (int)tileWidthFloat > 0.0) {
        tileWidthFloat++;
    } 

    unsigned int tileHeight = (unsigned int)tileHeightFloat;
    unsigned int tileWidth = (unsigned int)tileWidthFloat;

    PPMImage *outputImage = new_ppmimage(width, height, max);

    unsigned int heightIndex, widthIndex = 0;

    /* If the image isn't evenly divisible by the scale factor,
       some of the tiled thumbnails will need to be shortened by
       a pixel either vertically or horizontally (or both).
       vertShorten and horzShorten calculate how many of these tiles
       need to be shortened by a pixel.
    */
    unsigned int vertShorten = scaleFactor - ((tileHeight * scaleFactor) - height);
    unsigned int horzShorten = scaleFactor - ((tileWidth * scaleFactor) - width);

    /* Set to 0 if the image isn't shortened. 
       Set to 1 if the image is shortened.
    */
    unsigned int vertShortened = 0;
    unsigned int horzShortened = 0;


    unsigned int originalWidth = tileWidth;
    unsigned int horzShift = 0;
    unsigned int vertShift = 0;

    // Create a column of thumbnails
    for(unsigned int m = 0; m < scaleFactor; m++) {

        
        // Shorten the tile vertically if necessary
        if(m >= vertShorten && vertShortened != 1) {
            tileHeight--;
            vertShortened = 1;
        }

        // Create a row of thumbnails
        for(unsigned int l = 0; l < scaleFactor; l++) {

                // Shorten the tile horizontally if necessary.
                if(l >= horzShorten && horzShortened != 1) {
                    tileWidth--;
                    horzShortened = 1;
                }
                           
                // Create a thumbnail
                for(unsigned int k = 0; k < 3; k++) {
                    for(unsigned int j = 0; j < tileHeight; j++) {
                        heightIndex = j * scaleFactor;
                        for(unsigned int i = 0; i < tileWidth; i++) {
                            outputImage->pixmap[k][j + vertShift][i + horzShift] =
                            inputImage->pixmap[k][heightIndex][widthIndex];

                            widthIndex += scaleFactor;
                        }
                        heightIndex += scaleFactor;
                        widthIndex = 0;
                    }
                }

            horzShift += tileWidth;
        }
        tileWidth = originalWidth;
        horzShortened = 0;  
        horzShift = 0;
        
        vertShift+= tileHeight;
    }

    write_ppmfile(outputImage, outFileName);

    del_ppmimage(inputImage);
    del_ppmimage(outputImage);

}