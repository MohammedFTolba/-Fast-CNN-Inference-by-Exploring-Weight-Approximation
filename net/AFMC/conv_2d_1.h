
#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>
#define PAD_SIZE 1

#define IMAGE_WIDTH 13
#define IMAGE_HEIGHT 13
#define NUM_channel0 256
#define NUM_channel01 128
//////////////////////////////////////////////////////////////////////i

   

#define FILTER_SIZE1 3
#define NUM_FILTERS1 384
#define INPUT_SIZE1 13
#define PAD_SIZE 1
#define PADDING_VALUE 0








void padding_L1(uint8_t input[INPUT_SIZE1][INPUT_SIZE1][NUM_channel01], uint8_t padded_input[(INPUT_SIZE1 + 2 * PAD_SIZE) ][(INPUT_SIZE1 + 2 * PAD_SIZE)][NUM_channel01])
{
    int padded_height = INPUT_SIZE1 + 2;
    int padded_width = INPUT_SIZE1 + 2;
    
    // Initialize output array with zeros on the borders
for (int c = 0; c < NUM_channel01; c++) {
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





void conv2d_11(uint8_t  ifmap1[IMAGE_WIDTH+ 2 * PAD_SIZE][IMAGE_HEIGHT+ 2 * PAD_SIZE][NUM_channel01], int8_t  filter1[NUM_FILTERS1][NUM_channel01][FILTER_SIZE1][FILTER_SIZE1], uint8_t  output1[IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][NUM_FILTERS1]) {
   int i, j, k, l, m, n, c, f;
    int sum1;


    for (i = 0; i < IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE; i++) {
        for (j = 0; j < IMAGE_HEIGHT-FILTER_SIZE1+1+ 2 * PAD_SIZE; j++) {


            for (f = 0; f < NUM_FILTERS1; f+= 1) {
                sum1 = 0;

                for (c = 0; c < NUM_channel01; c++) {
                    for (m = 0; m < FILTER_SIZE1; m++) {
                        for (n = 0; n < FILTER_SIZE1; n++) {
                            sum1 += ifmap1[i+m][j+n][c] * filter1[f][c][m][n];
                        }
                    }
                }
                

                               if (sum1>0) {
                                 output1[i][j][f] = sum1;
                                 } else {
                                            output1[i][j][f]=0;
                                  }
                               


            }


        }
    }
}


void load_matrix_if1(uint8_t  ifmap1 [IMAGE_WIDTH][IMAGE_HEIGHT][NUM_channel01], const char* filename) {

uint8_t  ifmap0[IMAGE_WIDTH*NUM_channel0][IMAGE_HEIGHT];
uint8_t  ifmap00[IMAGE_WIDTH*NUM_channel01][IMAGE_HEIGHT];

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
int ct=0;
int ifmap000;
int ifmap001;


         for (int c = 0; c < NUM_channel01; c+= 2){
               for (int i = 0; i < IMAGE_WIDTH; i+= 1){

                     for (int j = 0; j < IMAGE_HEIGHT; j++){

                            ifmap000= ifmap0[i1][j] +ifmap0[i1+1][j]+ifmap0[i1+2][j]+ifmap0[i1+3][j];
                            ifmap001 = ifmap0[i1+1][j]+2*ifmap0[i1+2][j]+4*ifmap0[i1+3][j];

                           ifmap1 [i][j][c]=ifmap000;
                           ifmap1 [i][j][c+1]=ifmap001;
                           // printf("%i ", (uint8_t)ifmap1 [c][i][j]);

                     }
                //  printf("\n");
                  i1=i1+4;

                }


          }





}





void load_matrix_k1(int8_t  filter1[NUM_FILTERS1][NUM_channel01][FILTER_SIZE1][FILTER_SIZE1],  const char* filename2) {

int8_t  filter0[FILTER_SIZE1*NUM_FILTERS1*NUM_channel01][FILTER_SIZE1];

    FILE* fp = fopen(filename2, "r");
    if (fp == NULL) {
        printf("Error opening file %s.\n", filename2);
        return;
    }

    for (int i = 0; i < FILTER_SIZE1*NUM_FILTERS1*NUM_channel01; i++) {
        for (int j = 0; j < FILTER_SIZE1; j++) {
            int temp;
            fscanf(fp, "%d", &temp);
            filter0[i][j] = (int8_t)temp;
        }
    }

    fclose(fp);





int i1=0;
    for (int p = 0; p < NUM_FILTERS1; p++){

         for (int c = 0; c < NUM_channel01; c++){
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


