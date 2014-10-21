#ifndef __SIMPLE_PPM_H
#define __SIMPLE_PPM_H

/*
 * Methods to assist with creating a PPM image from a data array.
 * I (Dylan Sturgeon) did not create any of these methods.
 */

//write ppm
int simplePPM_write_ppm(char const * filename, unsigned int width, unsigned int height, unsigned char const * image);

//read ppm
//the returned pointer is the image data, caller is responseible for freeing the memory
//the width and height are stored in the passed unsigned int locations
unsigned char * simplePPM_read_ppm(char const * filename, unsigned int * width, unsigned int * height);

#endif
