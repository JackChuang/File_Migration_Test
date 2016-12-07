/* fflush example */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* mybuffer;
const int read_len = 255;
char* line1= "x86: written by x86 - 1\n";
char* line2= "arm: written by arm - 1\n";
char* line3= "x86: written by x86 - 2\n";
char* line4= "arm: written by arm - 2\n";
char* line9= "JackJack by arm\n";

int fd;
volatile int _a=101, _b=102;

#pragma popcorn
int a(int fd, char *_mybuffer, int* aa, int* bb){ 
	printf("arm: *****migrated from x86, now it's on [arm]*****\n"); 
	volatile int aaa=*aa+*bb; aaa++;  
    static char str[255] = "arm: a()";
    printf("arm: static char str[255] = \"%s\"\n", str); // this is good
    printf("arm: strlen = %d\n", strlen(str));
    printf("arm: fd = %d\n", fd);
    fsync(fd); fflush(stdout); fflush(stdin);

/*
    volatile FILE* f_arm;
   	f_arm = fopen ("/mnt/nfs/var/nfs/arm_example.txt","w+");
    //ARM <<<<<<< write >>>>>>>>
    fprintf(f_arm, "%s\n", str);
    printf("arm: printf(file): write \"< %s >\" to file\n", str);
    fputs (str,f_arm); //BUG: second time doing this function and reach here, it fails
    printf("fputs(file): write \"< %s >\" to file\n", str);
    strcat(str,"-\0");
    //sleep(1); fflush (f_arm); fsync (f_arm); fflush(stdout); fflush(stdin); sleep(1);
    sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
    fclose (f_arm);
*/
       
    //ARM <<<<<<< write >>>>>>>>
	    printf("arm: printf(file): write \" %s \" to file\n", str);
    //strcat(str, ""); // for \n
    write(fd, str, strlen(str));
    write(fd, "\n", strlen("n"));
    //fprintf(file, "%s\n", str);
    //fputs (str,file); //BUG: second time doing this function and reach here, it fails
    //    printf("fputs(file): write \"< %s >\" to file\n", str);
	//sleep(1); fflush (fd); fsync (fd); fflush(stdout); fflush(stdin); sleep(1);
    sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
    //sleep(60);

    strcat(str,"-\0");

    printf("arm: *****going to migrate back to x86 from [arm]*****\n");  
    printf("\n");  
	return aaa;
	//return 0;
}

int main()
{
    mybuffer = (char *) malloc(read_len);

    //volatile FILE* pFile;
    //pFile = fopen ("/mnt/nfs/var/nfs/example.txt","w+");
    fd = open ("/mnt/nfs/var/nfs/example.txt", O_RDWR|O_CREAT, 0666);
    printf("printf(\"start\") testing multiple migration points\n");
    puts("puts(\"start\") testing multiple migration points");
    
    //fprintf(pFile, "fprintf(\"start\")\n");
    char sstr[255] = "x86: main() ";
    write(fd, sstr, strlen(sstr));
    write(fd, sstr, strlen(sstr));
    printf("sstr=\"%s\" strlen = %d\n", sstr); // str size (lenghth)
    printf("write() * 2 done\n");
    printf("fd=%d\n\n\n", fd);
    sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
	
    int i=0;
    for (i=0; i<10; i++) {
        // test5: do wrtie (cooperation on both sides)
        // ......
        //sleep(1);
        printf("*****goint to migrate to arm from [x86]*****\n");
        #pragma popcorn
	    _a = a(fd, mybuffer, &_a, &_b); //to arm
	    //_a = a(pFile, mybuffer, &_a, &_b); //to arm
        printf("*****from arm back to [x86]*****\n");
        sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
        //printf("x86: pFile=%d\n\n\n", pFile);
        printf("x86: fd=%d\n\n\n", fd);
        sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
        strcat(sstr,"-");
        printf("sstr=\"%s\" strlen = %d\n", sstr); // str size (lenghth)
        write(fd, sstr, strlen(sstr));
        printf("\n");
        sleep(1); fsync(fd); fflush(stdout); fflush(stdin); sleep(1);
    }
    
    close (fd);
    printf("ALL DONE\n");
	return 0;
}
