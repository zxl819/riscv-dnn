#ifndef MLA_SINCOS_H
#define MLA_SINCOS_H

float my_sin(float x){
    return x - (x*x*x)/6 + (x*x*x*x*x)/120;
}

float my_cos(float x) {
    return 1 - (x*x)/2 + (x*x*x*x)/24;
}

#endif

