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
        in_file.open ("test1.pdf", ios::in); 
	char c;
	
        
	
	while(!in_file.eof())
	{
	       ofstream file1("dest_test1.pdf", ios::out|ios::app);
	       c= in_file.get();
	       file1.put(c);
	       file1.close();
	}
	in_file.close();
	
	return 0;
}
