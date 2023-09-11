#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>

#include <./conv_2d_1.h>


//// first layer
#define IMAGE_WIDTH 13
#define IMAGE_HEIGHT 13
#define NUM_channel_1 256

#define FILTER_SIZE1 3
#define NUM_FILTERS_1 384

#define PAD_SIZE 1
#include <time.h>


int main( int argc, char *argv[] )  
{
     clock_t start_time, end_time;
    double time_elapsed;

    start_time = clock(); // record start time
  
  
   ////////////// load the input feature map and weights for  ///// Layer 1

  // load the weights
    int8_t  filter1[NUM_FILTERS_1][NUM_channel_1][FILTER_SIZE1][FILTER_SIZE1];
    const char* kernel_1 = "kernel_L1.txt";

    load_matrix_k1(filter1, kernel_1);

  // load the input feature map

    uint8_t  ifmap0 [IMAGE_WIDTH][IMAGE_HEIGHT][NUM_channel_1];
    const char* ifmap_1 = "ifmap_L1.txt";

    load_matrix_if1(ifmap0,ifmap_1);  //loading


    uint8_t  ifmap1 [IMAGE_WIDTH+ 2 * PAD_SIZE][IMAGE_HEIGHT+ 2 * PAD_SIZE][NUM_channel_1];

     padding_L1(ifmap0, ifmap1);   /// padding


uint8_t    output1[IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][NUM_FILTERS_1];

    // Initialize the image and filters here

    conv2d_1(ifmap1, filter1, output1);


 end_time = clock(); // record end time

    time_elapsed = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate time elapsed

    printf("Time elapsed: %f seconds\n", time_elapsed);
 
  return 0;
}



