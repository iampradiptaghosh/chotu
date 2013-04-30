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
        FILE *file2=fopen("test1", "r");
        //in_file.open ("test5", ios::in); 
	char c[9];
	
        int i=0;
	FILE *file1=fopen("aaa", "w+");
	
	while(!feof(file2))//feof(file2))
	{
	       i++;
	       fgets(c,9,file2);
	       //c= getc(file2);
	       for(int j=0;j<9;j++)
	       {
	               if(c[j]=='\0')
	                break;  
	       fprintf(file1,"%02x ", (unsigned char) c[j]);//put(c);
                }	       
	       //if(i%8==0)
	       fprintf(file1,"\n");
	      // fgets(c,100,file2);
	       //c= getc(file2);
	}
	fclose(file2);
	fclose(file1);
	return 0;
}
