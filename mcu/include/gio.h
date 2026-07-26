#ifndef GIO_H
#define GIO_H

#define GOUT    0 // i2c
#define GIN     0 // i2c

int helloworld(char *arr); // write an array to gout
char *hellogort(int n, char stop); // read n chars from gin, if n = 0, until stop

#endif