#include <string>
#include <iostream>
#include <fstream>

using namespace std;

char * read_file(char *fname)
{
	char *text;
	int j = 0;
	int size;
	string line;
	ifstream file (fname);
	file.seekg(0, std::ios_base::end);
	size = (int) file.tellg();
	file.seekg(0, std::ios_base::beg);
	text = (char*) malloc(size*sizeof(char));
	while(file.good())
	{
		getline(file,line);
		for(unsigned int i=0;i<line.size();i++)
		{
			text[j] = line[i];
			j++;
		}
		text[j] = '\n';
		j++;
	}
	for(;j<size;j++) text[j] = NULL;
	text[size] = EOF;
	return text;
}
