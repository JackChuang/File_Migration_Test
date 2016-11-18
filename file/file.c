/* fflush example */
#include <stdio.h>
const int read_len = 80;
char* mybuffer;
char* line1= "written by x86 - 1\n";
char* line2= "written by arm - 1\n";
char* line3= "written by x86 - 2\n";
char* line9= "JackJack\n";
#pragma popcorn
int a(int aa){ volatile aaa = aa; aaa++; return aaa;}
//#pragma popcorn
//int b(int bb){ volatile bbb = bb; bbb++; return bbb;}
int main()
{
   volatile int _a=101, _b=102;
   mybuffer = (char *) malloc(read_len);
   FILE * pFile;
   pFile = fopen ("/mnt/nfs/var/nfs/example.txt","w+");
   if (pFile == NULL) perror ("Error opening file");
   else {
     printf("printf(\"start\"), puts() does NOT work!!!!!!!!!!!!!\n");
     //puts("puts(\"start\")");
      
     fputs (line1,pFile); //SIGMENTATION FAULT
     fputs (line1,pFile); //SIGMENTATION FAULT
     printf("fput() * 2\n");
     
     printf("%s\n",line1);
     printf("try to fflush()\n");
     fflush (pFile);   // flushing to kernel buffer
     printf("try to fsync()\n");
     fsync (pFile);    // flushing to disk
     printf("fflush() and fsync() done\n");

     //migration point 1
     printf("*****starting migrate to arm*****"); 
     #pragma popcorn
     _a = a(_a); //to arm
     printf("*****from x86, now on arm*****");  
     
     
     fputs (line9,pFile); //expect: no effect due to WRITE do nothing for file migration
     printf("%s", line9);
     fflush (pFile); fsync (pFile);

     fseek (pFile, 0, SEEK_SET);
     fgets (mybuffer, read_len, pFile); // but READ does!
     printf("\t");	printf("%s\n", mybuffer);

     fputs (line2,pFile);
     printf("%s\n", line2);
     fflush(pFile); fsync (pFile);

     fseek (pFile, strlen(line1), SEEK_SET); // expect &assmue the first wrtie doesn't
     fgets (mybuffer, read_len, pFile);	// take effect, and the second write takes effect.
     printf("\t");	printf("%s\n", mybuffer);			// So we should see line1 and lin2.
     // expect to see on ARM:
     // 	line1 
     // 	line2 
     //
     // line1 is written on X86
     // on ARM, first write (line9) doesn't take effect
     // on ARM, second write (line2) takes effect


     //migration point 2
     printf("*****starting to migrate back to x86*****");
     #pragma popcorn
     _b = a(_b); //to x86
     printf("*****from arm back to x86*****");
      
     fputs (line3,pFile);
     printf("%s\n", line3);
     fflush(pFile); fsync (pFile);    // flushing to disk

     fseek (pFile, strlen(line1), SEEK_SET);
     fgets (mybuffer, read_len, pFile);
     printf("\t");	printf("%s\n", mybuffer);

     fseek (pFile, strlen(line1)+strlen(line2), SEEK_SET);
     fgets (mybuffer, read_len, pFile);
     printf("\t");	printf("%s\n", mybuffer); //printf
     fflush(pFile); fsync (pFile);    // flushing to disk

     printf("line1_ofs=%d, line2_ofs=%d, line3_ofs=%d\n", strlen(line1), strlen(line2), strlen(line3));

     fclose (pFile);
     return 0;
  }
}
