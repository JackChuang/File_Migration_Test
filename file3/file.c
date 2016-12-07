/* fflush example */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int read_len = 80;
char* mybuffer;
char* line1= "x86: written by x86 - 1\n";
char* line2= "arm: written by arm - 1\n";
char* line3= "x86: written by x86 - 2\n";
char* line4= "arm: written by arm - 2\n";
char* line9= "JackJack by arm\n";
/*
1.
fopen()
fclose()
migration()


and 
2.
open()
migration()

probably,
3. 

open()
migration()
close()

*/
//#pragma popcorn
//int b(int bb){ volatile bbb = bb; bbb++; return bbb;}
#pragma popcorn
//int a(FILE *file, char *_mybuffer, int* aa, int* bb){ 
int a(int *file, char *_mybuffer, int* aa, int* bb){ 
//int a(char *_mybuffer, int* aa, int* bb){ 
//int a(int* aa, int* bb){ 
	//printf("*****from x86, now on arm*****");  
	//printf("I'm on ARM");
	volatile int aaa=*aa+*bb; aaa++;  
	//printf(" - start\n");
   
		//sleep(1);
 		//ARM <<<<< read x86 write ARM >>>>>>>>>
		//fseek (file, 0, SEEK_SET);
		fgets (_mybuffer, read_len, file); // but READ does!
		printf("\n");
		printf("\t");	printf("%s\n", _mybuffer);
		sleep(1); fflush(file); fsync(file); fflush(stdout); fflush(stdin); sleep(1);
	    
        //ARM <<<<<<< write its -1 >>>>>>>>
        fputs (line2,file);
		printf("%s\n", line2);

		//ARM <<<<< write JACJAC >>>>>
		fseek (file, 0, SEEK_END);
		fputs (line9,file); //expect: no effect due to WRITE do nothing for file migration
		printf("%s", line9);
		sleep(1); fflush(file); fsync(file); fflush(stdout); fflush(stdin); sleep(1);
		//ARM read from head
		fseek (file, 0, SEEK_SET); // can change to read previous write
		fgets (_mybuffer, read_len, file); // but READ does!
		printf("\t");	printf("%s\n", _mybuffer);
		sleep(1); fflush(file); fsync(file); fflush(stdout); fflush(stdin); sleep(1);

	    //ARM <<<<<<< write its -2 >>>>>>>>
		fputs (line4,file);
		printf("%s\n", line4);
		sleep(1); fflush(file); fsync(file); fflush(stdout); fflush(stdin); sleep(1);
	
		fseek (file, strlen(line1), SEEK_SET); // expect &assmue the first wrtie doesn't
		fgets (_mybuffer, read_len, file);	// take effect, and the second write takes effect.

        // done
		printf("*****starting to migrate back to x86*****\n");
        
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

   FILE* pFile;
   //int * pFile;
   pFile = fopen ("/mnt/nfs/var/nfs/example.txt","w+");
//fclose (pFile);
   //pFile = open ("/mnt/nfs/var/nfs/example.txt", O_RDWR|O_CREAT , 0666);
   //if (pFile == NULL) perror ("Error opening file");
   //else {
     printf("printf(\"start\"), puts() does NOT work????????????\n");
     puts("puts(\"start\")");
     fprintf(pFile, "fprintf(\"start\"), fputs() SIGMENTATION FAULT????????\n");
     printf("strlen = %d\n",strlen("fprintf(\"start\"), fputs() SIGMENTATION FAULT????????\n"));

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
	#pragma popcorn
	_a = a(pFile, mybuffer, &_a, &_b); //to arm
	//_a = a(mybuffer, &_a, &_b); //to arm
	//_a = a(&_a, &_b); //to arm
	printf("*****from arm back to x86*****\n");
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
    //fclose (pFile);
    printf("ALL DONE\n");
	return 0;
  //}

}
