#include "ws51f6240.h"

sbit P11 = P1 ^ 1;

void main(){

    P11F = 0x02;

    P11 = 1;

    while(1){
        
    }
}