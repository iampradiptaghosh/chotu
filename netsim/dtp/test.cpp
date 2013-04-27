#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <math.h>
#include <stdio.h>


using namespace std;

int main(void)
{
        ifstream in_file;
        in_file.open ("test5", ios::in); 
	char c;
	
        int i=0;
	FILE *file1=fopen("dest_test1", "w");
	while(!in_file.eof())
	{
	       i++;
	       
	       c= in_file.get();
	       fprintf(file1,"%02x ", (unsigned char) c);//put(c);
	       
	       if(i%8==0)
	       fprintf(file1,"\n");
	}
	in_file.close();
	fclose(file1);
	return 0;
}
