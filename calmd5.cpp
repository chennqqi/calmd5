#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <iostream>
#include <string>
#include <fstream>
#include <openssl/md5.h> // 如果你直接拷贝我的程序运行， 那注定找不到md5.h

#ifdef __linux__
#define _stricmp strcasecmp
#endif

//#pragma comment(lib, "libeay32.lib")
//#pragma comment(lib, "ssleay32.lib")  // 在本程序中， 可以注释掉这句
//using namespace std;

static void showUseage()
{
	std::cout << "calmd5:  cal md5 from inputfile to outputfile" << std::endl;
	std::cout << "usage: " << std::endl;
	std::cout << "calmd5 -i input.txt -o output.txt" << std::endl;
}

static void digest2String(const unsigned char digest[16], char* pszOut)
{
    char tmp[3] = {0};
    int i = 0;
    for (; i < 16; i++ )
	{
		sprintf(pszOut+2*i,"%02X", digest[i]);
    }
	pszOut[2*i] = '\0';
}

int main(int argc, const char* argv[])
{
	if (argc < 1)
	{
		showUseage();
		return 0;
	}
	const char* pszInputFileName = NULL;
	const char* pszOutputFileName = NULL;
	
	for (int i=1; i<argc; i++)
	{
		if (_stricmp(argv[i], "-i")==0 && i+1<argc)
		{
			pszInputFileName = argv[i+1];
			i++;
		}
		else if (_stricmp(argv[i], "-o")==0 && i+1<argc)
		{
			pszOutputFileName = argv[i+1];
			i++;
		}		
	}
	
	if (pszInputFileName==NULL || pszOutputFileName==NULL)
	{
		showUseage();
		return 0;	
	}
	
	std::ifstream fin(pszInputFileName);
	std::ofstream fout(pszOutputFileName);
	if (!fin.is_open())
	{
		std::cout << "Error(" << errno << 
			") can't open " << pszInputFileName << std::endl;
		return -1;
	}
	
	if (!fout.is_open())
	{
		std::cout << "Error(" << errno << 
			") can't open " << pszOutputFileName << std::endl;
		return -2;
	}
	
	time_t tBegin = time(NULL);
	
	std::string line;
	unsigned long long idx = 0;
	while ( getline( fin, line))
	{
		MD5_CTX ctx = {0};
		MD5_Init(&ctx);
		MD5_Update(&ctx, line.c_str(), line.size());
		unsigned char digest[16] = {0};
		MD5_Final(digest, &ctx);
		
		char szMd5[32] = {0};
		digest2String(digest, szMd5);
	#if 0
		std::cout << "[" << idx << "]";
		std::cout << " MD5(";
		std::cout << line.c_str();
		std::cout << ")= " << szMd5 << std::endl;
	#endif
		fout << line << "\t\t" << szMd5 << std::endl;
		idx++;
	}
	fin.close();
	fout.close();
	
	time_t tEnd = time(NULL);
	unsigned int tCost = tEnd - tBegin;
	
	std::cout << "cal " << idx << "md5s cost" << tCost
		<< " seconds" << std::endl; 
	
    return 0;
}
