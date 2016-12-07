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
char* line5= "Jack: written by ???\n";
char* line9= "JackJack by arm\n";

FILE* pFile;
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
   
	FILE *arm_f;
///*
	arm_f = fopen ("/mnt/nfs/var/nfs/arm_printout.txt","w+");
	if (arm_f == NULL) perror ("Error opening file on ARM");
	else {
	
		fputs("arm fputs(): Starting to file w/r on ARM\n", arm_f);
		fprintf(arm_f, "arm fprintf(): Starting to file w/r on ARM\n");
		fflush(stdout); fflush(stdin);    

		system("cat \/mnt\/nfs\/var\/nfs\/example.txt > \/mnt\/nfs\/var\/nfs\/arm_sys_out");
		fflush(stdout); fflush(stdin);     

		//sleep(1);
 		//ARM <<<<< read x86 write ARM >>>>>>>>>
		//fseek (file, 0, SEEK_SET);
		fgets (_mybuffer, read_len, file); // but READ does!
		fprintf(arm_f, "\t");	fprintf(arm_f, "%s\n", _mybuffer);
		fputs("\t", arm_f);	fputs(_mybuffer, arm_f);
		printf("\n");
		printf("\t");	printf("%s\n", _mybuffer);
		sleep(1); fflush (file); fsync (file); fflush (arm_f); fsync (arm_f); fflush(stdout); fflush(stdin); sleep(1);
//flush arm_f?
	    
        //ARM <<<<<<< write its -1 >>>>>>>>
        fputs (line2,file);
		printf(arm_f, "%s\n", line2);
		printf("%s\n", line2);
		//fflush(file); fsync (file);
		//sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);

		//ARM <<<<< write JACJAC >>>>>
		fseek (file, 0, SEEK_END);
		fputs (line9,file); //expect: no effect due to WRITE do nothing for file migration
		fprintf(arm_f, "%s", line9);
		printf("%s", line9);
		sleep(1); fflush (file); fsync (file); fflush (arm_f); fsync (arm_f); fflush(stdout); fflush(stdin); sleep(1);
		//sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
		//fflush (file); fsync (file);
		//ARM read from head
		fseek (file, 0, SEEK_SET); // can change to read previous write
		fgets (_mybuffer, read_len, file); // but READ does!
		fprintf(arm_f, "\t");	fprintf(arm_f, "%s\n", _mybuffer);
		printf("\t");	printf("%s\n", _mybuffer);
		sleep(1); fflush (file); fsync (file); fflush (arm_f); fsync (arm_f); fflush(stdout); fflush(stdin); sleep(1);
		//sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);

	    //ARM <<<<<<< write its -2 >>>>>>>>
		fputs (line4,file);
		printf(arm_f, "%s\n", line4);
		printf("%s\n", line4);
		//fflush(file); fsync (file);
		//sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
		sleep(1); fflush (file); fsync (file); fflush (arm_f); fsync (arm_f); fflush(stdout); fflush(stdin); sleep(1);

	
		fseek (file, strlen(line1), SEEK_SET); // expect &assmue the first wrtie doesn't
		fgets (_mybuffer, read_len, file);	// take effect, and the second write takes effect.
		fprintf(arm_f, "\t");	fprintf(arm_f, "%s\n", _mybuffer); // So we should see line1 and lin2.
		printf("\t");	printf("%s\n", _mybuffer); // So we should see line1 and lin2.
		sleep(1); fflush (file); fsync (file); fflush (arm_f); fsync (arm_f); fflush(stdout); fflush(stdin); sleep(1);
		//sleep(1); fflush (file); fsync (file); fflush(stdout); fflush(stdin); sleep(1);
		// expect to see on ARM:
		// 	line1 
		// 	line2 
		//
		// line1 is written on X86
		// on ARM, first write (line9) doesn't take effect
		// on ARM, second write (line2) takes effect
        
        // interleaving check the fd in kernel space
	    arm_f = fopen ("/mnt/nfs/var/nfs/arm_printout2.txt","w+");

        // done
		printf("*****starting to migrate back to x86*****\n");
	}
//*/
	fprintf(arm_f, "fprintf(): going to migrate back to x86\n");
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

    fputs (line5,pFile);
    printf("%s\n", line5);
    fflush(pFile); fsync (pFile);    // flushing to disk
	sleep(1); fflush (pFile); fsync (pFile); fflush(stdout); fflush(stdin); sleep(1);
/*

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
