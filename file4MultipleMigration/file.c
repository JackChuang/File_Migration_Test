/* fflush example */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* mybuffer;
const int read_len = 80;
char* line1= "x86: written by x86 - 1\n";
char* line2= "arm: written by arm - 1\n";
char* line3= "x86: written by x86 - 2\n";
char* line4= "arm: written by arm - 2\n";
char* line9= "JackJack by arm\n";

volatile FILE* pFile;

//#pragma popcorn
volatile int a(FILE *file, char *_mybuffer, int* aa, int* bb){ 
	volatile int aaa=*aa+*bb; aaa++;  
//static char str[255] = "arm: a()";
static char str[255] = "arm: a()";
	printf("arm: <<<<<<<<<< migrated from x86, now it's on [arm] <<<<<<<<<<\n\n"); 
    sleep(1);
    //fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
    printf("arm: static char str[255] = \"%s\"\n", str); // this is good
    printf("arm: strlen = %d\n", strlen(str));
    printf("arm: file = %x\n", file);
   	
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

    //ARM <<<<<<< write >>>>>>>>
    fprintf(file, "%s\n", str);
    printf("arm: printf(file): write \" %s \" to file\n", str);
    //fputs (str,file); //BUG: second time doing this function and reach here, it fails
    //    printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
	//fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(2);
    //sleep(60);

    strcat(str,"-\0");
    
    printf("arm: >>>>>>>>>> going to migrate back to x86 from [arm] >>>>>>>>>>\n\n\n");
    //fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(2);
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
    printf("printf(\"start\") testing multiple migration points\n");
    puts("puts(\"start\") testing multiple migration points");

    char strr[255] = "fprintf(\"start\")\n";
    fprintf(pFile, strr);
    printf("strlen = %d\n",strlen(strr));
    fputs (line1,pFile); fputs (line1,pFile); fputs ("----- Start -----",pFile);

    printf("strlen = %d\n", strlen(line1));
    printf("fputs() * 2 done\n");
     
    printf("pFile=%d\n\n\n", pFile);
    //fflush (pFile); fsync (pFile); fflush(stdout); fflush(stdin); sleep(1);
	
    int i=0;
    for (i=0; i<5; i++) {
        // test5: do wrtie (cooperation on both sides)
        // ......
        //sleep(1);
        printf("x86: <<<<<<<<<< goint to migrate to arm from [x86] <<<<<<<<<<\n\n\n"); sleep(1);
	    _a = a(pFile, mybuffer, &_a, &_b); //to arm
        printf("x86: >>>>>>>>>> from arm back to [x86] >>>>>>>>>>\n"); sleep(1);

        printf("x86: pFile=%x\n", pFile);
        fprintf(pFile, "%s\n", str);
        printf("%s (written)\n", str);
        strcat(str,"--\0");
    }
    
    fclose (pFile);
    printf("ALL DONE\n");
	return 0;
}
