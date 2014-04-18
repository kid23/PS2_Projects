// SRWZ.cpp : 定义控制台应用程序的入口点。
//
/*
	SRWZ Compresser

	Author:		kid, K_I_D@126.com

	Version:	v0.2	2008.10.2
	Version:	v0.21	2008.10.4	完成压缩
	Version:	v0.3	2008.10.17	参照gzip思想优化代码
	Version:	v0.31	2008.10.17	修正压缩时window size
*/

#include "stdafx.h"
#include "SRWZ.h"
#include "File.h"
#include <string>
#include "MWo3.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int defaultlevel = 7;

void Encode(const string& name, int level)
{
	ULONG srcsize = 0;
	UCHAR* src = ReadAllFromFile(name.c_str(), srcsize);
	if (!src) return;
	level = (level >= 0 && level <= 9) ? level : defaultlevel;

	UCHAR* dst = new UCHAR[srcsize];
	ULONG dstsize = 0;
	clock_t begin, end;
	begin = clock();
	compress(src, srcsize, dst, dstsize, level);
	//deflate();
	end = clock();

	printf("Compress %s success. use %ld ms, level %d.\n", name.c_str(), (end - begin), level);
	string outName(name, 0, name.find_last_of("."));
	outName += ".mwo";
	WriteToFile(outName.c_str(), dst, dstsize, 0, "wb+");

	delete[] src;
	delete[] dst;
}
void Decode(const string& name)
{
	ULONG srcsize = 0;
	UCHAR* src = ReadAllFromFile(name.c_str(), srcsize);
	if (!src) return;

	const UCHAR *p = src;
	int uncompresssize = 0;
	uncompresssize = getnum(p);

	int t = 0;
	t = getnum(p);
	
	int a1=t>>1;
	int v0=a1>>1;
	v0=v0&0xf;
	v0=v0+0x8;
	v0=1 << v0;
	
	if(v0<uncompresssize || (a1&0x21)!=1){
		if((a1&0x40)!=0){
			getnum(p);
		}
	}
	getnum(p);
	
	UCHAR* dst = new UCHAR[uncompresssize];
	uncompress(p, src + srcsize, dst, dst + uncompresssize);
	string outName = name + ".bin";
	if (WriteToFile(outName.c_str(), dst, uncompresssize, 0, "wb+"))
		printf("Uncompress %s to %s success.\n", name.c_str(), outName.c_str());
	
	delete[] src;
	delete[] dst;
}

void PrintHelp()
{
	_tprintf(_T("Error Argv.\n"));
	_tprintf(_T("SRWZ.exe [-c|-d] infile [level]\n"));
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		_tprintf(_T("SRWZ Tool v0.31 2008.10.18 debug\n"));
		_tprintf(_T("Copyright (C) 2008, kid, K_I_D@126.com\n\n"));

		if(argc < 3) {PrintHelp();return nRetCode;}	

		if (!_stricmp(argv[1], "-d"))
		{
			Decode(string(argv[2]));			
		}
		else if (!_stricmp(argv[1], "-c"))
		{
			Encode(string(argv[2]), ((argc >= 4) ? atoi(argv[3]) : defaultlevel));			
		}
		else
		{
			PrintHelp();
		}
	}

	return nRetCode;
}
