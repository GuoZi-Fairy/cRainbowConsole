#include "ccRainbow.h"
#include<stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    chinese_support();
    VERSION();
    rainbow_print("{red}hello {white}how wonderful world the day is {green}%d\n",20200220);
    rainbow_print("{yellow}version:%d.%d.%d\n",10,1,2);
    return 0;
}
