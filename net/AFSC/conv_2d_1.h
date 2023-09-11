
#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>
#define PAD_SIZE 1

#define IMAGE_WIDTH 13
#define IMAGE_HEIGHT 13
#define NUM_channel0 256
//////////////////////////////////////////////////////////////////////i

   

#define FILTER_SIZE1 3
#define NUM_FILTERS1 192
#define NUM_FILTERS01 384

#define INPUT_SIZE1 13
#define PAD_SIZE 1
#define PADDING_VALUE 0


void padding_L1(uint8_t input[INPUT_SIZE1][INPUT_SIZE1][NUM_channel0], uint8_t padded_input[(INPUT_SIZE1 + 2 * PAD_SIZE) ][(INPUT_SIZE1 + 2 * PAD_SIZE)][NUM_channel0])
{
    int padded_height = INPUT_SIZE1 + 2;
    int padded_width = INPUT_SIZE1 + 2;
    
    // Initialize output array with zeros on the borders
for (int c = 0; c < NUM_channel0; c++) {
    for (int i = 0; i < padded_height; i++) {
        for (int j = 0; j < padded_width; j++) {
            if (i == 0 || j == 0 || i == padded_height-1 || j == padded_width-1) {
                padded_input[i][j][c] = 0;
            } else {
                padded_input[i][j][c] = input[i-1][j-1][c];
            }
        }
    }  
 }
}





void conv2d_1(uint8_t  ifmap1[IMAGE_WIDTH+ 2 * PAD_SIZE][IMAGE_HEIGHT+ 2 * PAD_SIZE][NUM_channel0], int8_t  filter1[NUM_FILTERS1][NUM_channel0][FILTER_SIZE1][FILTER_SIZE1], uint8_t  output1[IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][NUM_FILTERS01]) {
   int i, j, k, l, m, n, c, f,sum10,sum11,sum12,sum13;
    int sum1;
    int sum2;
      int f1;
    for (i = 0; i < IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE; i++) {
        for (j = 0; j < IMAGE_HEIGHT-FILTER_SIZE1+1+ 2 * PAD_SIZE; j++) {
            f1=0;

            for (f = 0; f < NUM_FILTERS1; f+= 2) {
                sum1 = 0;
                sum2 = 0;
                for (c = 0; c < NUM_channel0; c++) {
                    for (m = 0; m < FILTER_SIZE1; m++) {
                        for (n = 0; n < FILTER_SIZE1; n++) {
                            sum1 += ifmap1[i+m][j+n][c] * filter1[f][c][m][n];
                            sum2 += ifmap1[i+m][j+n][c] * filter1[f+1][c][m][n];
                        }
                    }
                }
                sum10= sum1;
                sum11= sum1+sum2;
                sum12 = sum1+2*sum2;
                sum13 = sum1+4*sum2;

                               if (sum10>0) {
                                 output1[i][j][f1] = sum10;
                                 } else {
                                            output1[i][j][f1]=0;
                                  }
                               if (sum11>0) {
                                 output1[i][j][f1+1] = sum11;
                                 } else {
                                            output1[i][j][f1+1]=0;
                                  }


                               if (sum12>0) {
                                 output1[i][j][f1+2] = sum12;
                                 } else {
                                            output1[i][j][f1+2]=0;
                                  }


                               if (sum13>0) {
                                 output1[i][j][f1+3] = sum13;
                                 } else {
                                            output1[i][j][f1+3]=0;
                                  }

                f1=f1+4;
            }


        }
    }
}










void load_matrix_if1(uint8_t  ifmap1 [IMAGE_WIDTH][IMAGE_HEIGHT][NUM_channel0], const char* filename) {

uint8_t  ifmap0[IMAGE_WIDTH*NUM_channel0][IMAGE_HEIGHT];

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    for (int i = 0; i < IMAGE_WIDTH*NUM_channel0; i++) {
        for (int j = 0; j < IMAGE_HEIGHT; j++) {
            int temp;
            fscanf(fp, "%d", &temp);
            ifmap0[i][j] = (uint8_t)temp;
        }
    }

    fclose(fp);





   // buuild the array  for all the ifmaps

int i1=0;

         for (int c = 0; c < NUM_channel0; c++){
               for (int i = 0; i < IMAGE_WIDTH; i++){

                     for (int j = 0; j < IMAGE_HEIGHT; j++){

                           ifmap1 [i][j][c]=ifmap0[i1][j];
                           // printf("%i ", (uint8_t)ifmap1 [c][i][j]);

                     }
                //  printf("\n");
                  i1=i1+1;
                }

          }





}





void load_matrix_k1(int8_t  filter1[NUM_FILTERS1][NUM_channel0][FILTER_SIZE1][FILTER_SIZE1],  const char* filename2) {

int8_t  filter0[FILTER_SIZE1*NUM_FILTERS1*NUM_channel0][FILTER_SIZE1];

    FILE* fp = fopen(filename2, "r");
    if (fp == NULL) {
        printf("Error opening file %s.\n", filename2);
        return;
    }

    for (int i = 0; i < FILTER_SIZE1*NUM_FILTERS1*NUM_channel0; i++) {
        for (int j = 0; j < FILTER_SIZE1; j++) {
            int temp;
            fscanf(fp, "%d", &temp);
            filter0[i][j] = (int8_t)temp;
        }
    }

    fclose(fp);





int i1=0;
    for (int p = 0; p < NUM_FILTERS1; p++){

         for (int c = 0; c < NUM_channel0; c++){
               for (int i = 0; i < FILTER_SIZE1; i++){

                     for (int j = 0; j < FILTER_SIZE1; j++){

                           filter1 [p][c][i][j]=filter0[i1][j];
                         // printf("%hhd ", filter1 [p][c][i][j]);
                           

                     }
                 // printf("\n");
                  i1=i1+1;
                }

          }

      }





}


