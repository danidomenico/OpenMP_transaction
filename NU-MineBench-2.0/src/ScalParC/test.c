#include<stdio.h>

int main(int argc, char *argv[])
{
   FILE *fp;
   int j,mm, nn, count=0;

   fp=fopen("../../datasets/para_F26-A32-D250K/F26-A32-D250K.tab.att.0","r");
  
    for (j=0; j<250000; j++) {
   fscanf(fp, "%d\n", &mm);
   fscanf(fp, "%d\n", &nn);
if (nn) count++;
   }

  printf("count=%d\n", count);
}
