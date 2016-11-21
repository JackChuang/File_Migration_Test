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
//#pragma popcorn
//int b(int bb){ volatile bbb = bb; bbb++; return bbb;}
//int a(FILE *file, char *_mybuffer, int* aa, int* bb){ 
#pragma popcorn
int a(volatile int *file, char *_mybuffer, int* aa, int* bb){ 
//int a(char *_mybuffer, int* aa, int* bb){ 
//int a(int* aa, int* bb){ 
	//printf("I'm on ARM");
	volatile int aaa=*aa+*bb; aaa++;  
	printf("arm: *****migrated from x86, now it's on [arm]*****\n"); 
    
    static char str[255] = "padding";
        printf("arm: static char str[255] = \"%s\"\n", str); // this is good
        printf("arm: strlen = %d\n", strlen(str));
   	
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

/*       

    //ARM <<<<<<< write >>>>>>>>
    //fprintf(file, "%s\n", str);
	//    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,file); //BUG: second time doing this function and reach here, it fails
        printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
	sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
    
    sleep(60);
    
    //ARM <<<<<<< write >>>>>>>>
    //fprintf(file, "%s\n", str);
	//    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,file); //BUG: second time doing this function and reach here, it fails
        printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
	sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);


    //ARM <<<<<<< write >>>>>>>>
    //fprintf(file, "%s\n", str);
	//    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,file); //BUG: second time doing this function and reach here, it fails
        printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
	sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
    sleep(30);
    
    //ARM <<<<<<< write >>>>>>>>
    //fprintf(file, "%s\n", str);
	//    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,file); //BUG: second time doing this function and reach here, it fails
        printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
	sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
    sleep(30);
  */  
    
    strcat(str,"-\0");
    
    printf("arm: *****going to migrate back to x86 from [arm]*****\n");  
    printf("\n");  
	return aaa;
	//return 0;
}

int main()
{
   volatile int _a=101, _b=102;
   mybuffer = (char *) malloc(read_len);

   //migration point 1
   //printf("*****starting migrate to arm -1*****"); 
   //#pragma popcorn
   //_a = a(&_a, &_b); //to arm
   //printf("*****from x86, now on arm -1*****");  
/*
   //migration point 2
   printf("*****starting to migrate back to x86 -1*****");
   #pragma popcorn
   _b = a(&_b, &_a); //to x86
   printf("*****from arm back to x86 -1*****");
*/
/*
   //migration point 3
   printf("*****starting migrate to arm -2*****"); 
   #pragma popcorn
   _a = a(&_a, &_b); //to arm
   printf("*****from x86, now on arm -2*****");  

   //migration point 4
   printf("*****starting to migrate back to x86 -2*****");
   #pragma popcorn
   _b = a(&_b, &_a); //to x86
   printf("*****from arm back to x86 -2*****");
*/

   volatile FILE* pFile;
   //int * pFile;
   pFile = fopen ("/mnt/nfs/var/nfs/example.txt","w+");
//fclose (pFile);
   //pFile = open ("/mnt/nfs/var/nfs/example.txt", O_RDWR|O_CREAT , 0666);
   //if (pFile == NULL) perror ("Error opening file");
   //else {
     printf("printf(\"start\") testing multiple migration points\n");
     puts("puts(\"start\") testing multiple migration points");

     fprintf(pFile, "fprintf(\"start\")\n");
     printf("strlen = %d\n",strlen("fprintf(\"start\")\n"));

     fputs (line1,pFile); //SIGMENTATION FAULT
     fputs (line1,pFile); //SIGMENTATION FAULT
     printf("strlen = %d\n", strlen(line1));
     printf("fputs() * 2 done\n");
/*
     printf("%s\n",line1);
     printf("try to fflush()\n");
     fflush (pFile);   // flushing to kernel buffer
     printf("try to fsync()\n");
     fsync (pFile);    // flushing to disk
     printf("fflush() and fsync() done\n");

	system("cat \/mnt\/nfs\/var\/nfs\/example.txt > x86_test_stdin_out");

     //migration to & back
     printf("*****starting migrate to arm*****\n");
	//before migration - flush everythig for debuging and safety
	fflush(stdout);     
*/
	printf("pFile=%d\n\n\n", pFile);
	sleep(1); fflush (pFile); fsync (pFile); fflush(stdout); fflush(stdin); sleep(1);
//fclose (pFile);
	
    int i=0;
    for (i=0; i<10; i++) {
        // test5: do wrtie (cooperation on both sides)
        // ......
sleep(1);
        printf("*****goint to migrate to arm from [x86]*****\n");
        #pragma popcorn
	    _a = a(pFile, mybuffer, &_a, &_b); //to arm
        printf("*****from arm back to [x86]*****\n");
        printf("\n");
    }
	//_a = a(mybuffer, &_a, &_b); //to arm
	//_a = a(&_a, &_b); //to arm
/*
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
*/
    fclose (pFile);
    printf("ALL DONE\n");
	return 0;
  //}

}
