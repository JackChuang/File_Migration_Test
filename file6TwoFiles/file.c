/* fflush example */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ITERATION 100
char* mybuffer;
const int read_len = 80;
char* line1= "x86: written by x86 - 1\n";
char* line2= "arm: written by arm - 1\n";
char* line3= "x86: written by x86 - 2\n";
char* line4= "arm: written by arm - 2\n";
char* line9= "JackJack by arm\n";
int i;
volatile FILE* pFile;
volatile FILE* pFile2;

#pragma popcorn
int a( FILE *file, FILE *file2, char *_mybuffer, int* aa, int* bb){ 
	volatile int aaa=*aa+*bb; aaa++;  
    printf("arm: <<<<<<<<<< migrated from x86, now it's on [arm] <<<<<<<<<<\n\n");
    sleep(1);
    static char str[255] = "arm: a()";
    printf("arm: static char str[255] = \"%s\"\n", str); // this is good
    printf("arm: strlen = %d\n", strlen(str));
    printf("arm: file = %d\n", file);
   	
/*
    volatile FILE* f_arm;
   	f_arm = fopen ("/mnt/nfs/var/nfs/arm_example.txt","w+");
    //ARM <<<<<<< write >>>>>>>>
    fprintf(f_arm, "%s\n", str);
    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,f_arm); //BUG: second time doing this function and reach here, it fails
    printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
    sleep(1); fflush (f_arm); fsync (f_arm); fflush(stdout); fflush(stdin); sleep(1);
    fclose (f_arm);
*/

    //ARM <<<<<<< write 1st file >>>>>>>>
    fprintf(file, "%s\n", str);
    printf("arm: printf(file): write \" %s \" to file\n", str);
    fputs (str,file); fprintf(file, "\n");
    printf("arm: fputs(file): write \" %s \" to file\n", str);
    strcat(str,"-\0");
    //fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);

    //ARM <<<<<<< write 2nd file >>>>>>>>
    fprintf(file2, "%s\n", str);
    printf("arm: printf(file2): write \" %s \" to file2\n", str);
    fputs (str,file2); fprintf(file2, "\n");
    printf("arm: fputs(file2): write \" %s \" to file2\n", str);
    strcat(str,"--\0");
	//sleep(1); fflush (file2); fsync (file2); fflush(stdout); fflush(stdin); sleep(1);

    printf("arm: >>>>>>>>>> going to migrate back to x86 from [arm] >>>>>>>>>>\n\n\n");
    sleep(1);
	return aaa;
	//return 0;
}

int main()
{
    volatile int _a=101, _b=102;
    mybuffer = (char *) malloc(read_len);
    static char str[255] = "x86: main()";

    pFile = fopen ("/mnt/nfs/var/nfs/example.txt","w+");
    pFile2 = fopen ("/mnt/nfs/var/nfs/example2.txt","w+");
    printf("printf(\"start\") testing multiple migration points\n");
    puts("puts(\"start\") testing multiple migration points\n");

    char strr[50] = "fprintf(\"start\")\n";
    fprintf(pFile, strr);
    fprintf(pFile2, strr);
    printf("strlen = %d\n",strlen(strr));

    fputs (line1,pFile); fputs (line1,pFile);
    fputs (line1,pFile2); fputs (line1,pFile2);
    fputs ("----- Start -----\n",pFile);
    printf("strlen = %d\n", strlen(line1));
    printf("fputs() * 2 done\n");
     
    printf("pFile=%d\n", pFile);
    printf("pFile2=%d\n\n\n", pFile2);
    //sleep(1); fflush (pFile); fsync (pFile); fflush(stdout); fflush(stdin); sleep(1);
    //sleep(1); fflush (pFile2); fsync (pFile2); fflush(stdout); fflush(stdin); sleep(1);

    int i=0;
    for (i=0; i<10; i++) {
        printf("x86: <<<<<<<<<< goint to migrate to arm from [x86] <<<<<<<<<<\n\n\n"); sleep(1);
	    _a = a(pFile, pFile2, mybuffer, &_a, &_b); //to arm
        printf("x86: >>>>>>>>>> from arm back to [x86] >>>>>>>>>>\n"); sleep(1);
        
        printf("x86: pFile=%d\n", pFile);
        printf("x86: pFile2=%d\n", pFile2);
        fprintf(pFile, "%s\n", str);
        fprintf(pFile2, "%s\n", str);
        printf("%s (written)\n", str);
        strcat(str,"--\0");
    }
    
    fclose (pFile);
    fclose (pFile2);
    printf("ALL DONE\n");
	return 0;
}
