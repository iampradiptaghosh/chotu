#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <math.h>
#include <stdio.h>



using namespace std;

int main(void)
{
        //ifstream in_file;
        FILE *file2=fopen("Dataset/test8.pdf", "r");
        //in_file.open ("test8", ios::in); 
	char c;
	
        int i=0;
	FILE *file1=fopen("Dataset/dest_test8.pdf.txt", "w+");
	c=fgetc(file2);
	while(!feof(file2))//feof(file2))
	{
	      
	       fprintf(file1,"%02x ", (unsigned char) c);//put(c);
                
	       //fgets(c,100,file2);
	       c= getc(file2);
	}
	fclose(file2);
	fclose(file1);
	return 0;
}
