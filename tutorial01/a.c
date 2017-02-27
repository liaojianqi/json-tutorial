#include <stdlib.h>
#include <stdio.h>
 
int main(){
    //前首正号，可以有前首0, 可以直接以.开头,可以以.结尾
    char *s = "-";
    char *c;
    double d = strtod(s,&c);
    printf("%f\n",d);
    printf("%d\n",c-s);
    return 0;
}