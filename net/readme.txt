This repository comprises four folders: baseline, AWF, AFSC, and AFMC. 
Each folder contains C code for a convolution layer with 256 input channels, 384 output channels, and a 3x3 filter size, 
where the filters are   3x3x256x384. 
The input size for this layer is 13x13.

To run the code 
gcc net.c -o net -std=c99
./net
