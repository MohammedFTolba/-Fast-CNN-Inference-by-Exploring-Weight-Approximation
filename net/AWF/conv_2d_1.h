
#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>
#define PAD_SIZE 1

#define IMAGE_WIDTH 13
#define IMAGE_HEIGHT 13
#define NUM_channel0 256
//////////////////////////////////////////////////////////////////////i

   

#define FILTER_SIZE1 3
#define FILTER_SIZE2 2


#define NUM_FILTERS1 384
#define NUM_FILTERS 384
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





void conv2d_1(uint8_t  ifmap1[IMAGE_WIDTH+ 2 * PAD_SIZE][IMAGE_HEIGHT+ 2 * PAD_SIZE][NUM_channel0], int8_t  filter1[NUM_FILTERS][NUM_channel0][FILTER_SIZE1][FILTER_SIZE2], uint8_t  output1[IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE][NUM_FILTERS]) {

int8_t  output2[IMAGE_WIDTH-FILTER_SIZE1+1+ 3 * PAD_SIZE][IMAGE_WIDTH-FILTER_SIZE1+1+ 3 * PAD_SIZE][NUM_FILTERS];

  int i, j, k, l, m, n, c, f,d,ff;
    int sum1 [3][2];


    int sum2;
      int f1;


    for (i = 0; i < IMAGE_WIDTH-FILTER_SIZE1+1+ 2 * PAD_SIZE; i++) {
        for (j = 0; j < IMAGE_HEIGHT-FILTER_SIZE1+1+ 2 * PAD_SIZE; j++) {
            f1=0;
         for (ff = 0; ff < NUM_FILTERS; ff++) {
          output2[i][j][ff] =0;
           }

            for (c = 0; c < NUM_channel0; c++) {


                sum2 = 0;
                   for ( d = 0; d < 2; d++) {  

                          sum1[0][d]= 0;
                          sum1[1][d]= 0;
                          sum1[2][d]= 0;

                    }
                   for ( d = 0; d < 3; d++) {  

                          sum1[0][0]= sum1[0][0]+ifmap1[i][j+d][c];
                          sum1[1][0]= sum1[1][0]+ifmap1[i+1][j+d][c];
                          sum1[2][0]= sum1[2][0]+ifmap1[i+2][j+d][c];

                    }
                  for ( d = 0; d < 3; d++) {   
                          sum1[0][1]= sum1[0][1]+d*ifmap1[i][j+d][c];
                          sum1[1][1]= sum1[1][1]+d*ifmap1[i+1][j+d][c];
                          sum1[2][1]= sum1[2][1]+d*ifmap1[i+2][j+d][c];
                    }

                for (f = 0; f < NUM_FILTERS; f++) {   

                    sum2=0;
                    for (m = 0; m < FILTER_SIZE1; m++) {
                        for (n = 0; n < FILTER_SIZE2; n++) {

                            sum2 += sum1[m][n] * filter1[f][c][m][n];

                        }
                    }

                output2[i][j][f] = output2[i][j][f]+ sum2;

                }


            }

                           for (int ft = 0; ft < NUM_FILTERS; ft++) {   

                                if (output2[i][j][ft]>0) {
                                 output1[i][j][ft] = output2[i][j][ft];
                                 } else {
                                            output1[i][j][ft]=0;
                                  }  
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

                     }
                //  printf("\n");
                  i1=i1+1;
                }

          }





}





void load_matrix_k1(int8_t  filter1[NUM_FILTERS1][NUM_channel0][FILTER_SIZE1][FILTER_SIZE2],  const char* filename2) {

int8_t  filter0[FILTER_SIZE1*NUM_FILTERS1*NUM_channel0][FILTER_SIZE2];

    FILE* fp = fopen(filename2, "r");
    if (fp == NULL) {
        printf("Error opening file %s.\n", filename2);
        return;
    }

    for (int i = 0; i < FILTER_SIZE1*NUM_FILTERS1*NUM_channel0; i++) {
        for (int j = 0; j < FILTER_SIZE2; j++) {
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

                     for (int j = 0; j < FILTER_SIZE2; j++){

                           filter1 [p][c][i][j]=filter0[i1][j];
                         // printf("%hhd ", filter1 [p][c][i][j]);
                           

                     }
                 // printf("\n");
                  i1=i1+1;
                }

          }

      }





}


