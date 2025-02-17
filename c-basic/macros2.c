#include <stdio.h>


// __DATE__	The date of compilation—like when you’re compiling this file—in Mmm dd yyyy format
// __TIME__	The time of compilation in hh:mm:ss format
// __FILE__	A string containing this file’s name
// __LINE__	The line number of the file this macro appears on
// __func__	The name of the function this appears in, as a string129
// __STDC__	Defined with 1 if this is a standard C compiler
// __STDC_HOSTED__	This will be 1 if the compiler is a hosted implementation130, otherwise 0
// __STDC_VERSION__	This version of C, a constant long int in the form yyyymmL, e.g. 201710L

int main(void)
{
    printf("This function: %s\n", __func__);
    printf("This file: %s\n", __FILE__);
    printf("This line: %d\n", __LINE__);
    printf("Compiled on: %s; %s\n", __DATE__, __TIME__);
    printf("C Version: %ld\n", __STDC_VERSION__);
}

// This function: main
// This file: macros1.c
// This line: 17
// Compiled on: Feb  4 2025; 21:58:10
// C Version: 201710