/*	
	VERONICA UNCOMPRESS ALGORITHM

	Version:	v0.2	2008.2.17
	Version:	v0.1	2008.1.30
	Author:		kid
*/

#include "File.h"

enum VERONICA_COMPRESS_TYPE
{
	VERONICA_DIRECT,
	VERONICA_WINDOW,
	VERONICA_RLE
};

//const UCHAR *s;

inline int ReadOneBit(UCHAR* &src)
{
	static int num = 1;
	static int bitnum = 0;
	static unsigned char i = 1;
	static unsigned char flag = *src++;
	if (i > 8)
	{
		flag = *src++;
		num++;
		//printf("flag = %x , %x  readtime=%d,%d\n",flag,src - s - 1,num,bitnum);
		i = 1;
	}

	//printf("read %d\n",i);
	if (flag & 1)
	{
		i++;
		bitnum++;
		flag >>= 1;
		return 1;
	}
	else
	{
		i++;
		bitnum++;
		flag >>= 1;
		return 0;
	}
}

inline VERONICA_COMPRESS_TYPE GetFlagType(UCHAR* &src,unsigned int &num,unsigned int &pos)
{
	if (ReadOneBit(src))
	{
		return VERONICA_DIRECT;
	}
	else
	{
		if (ReadOneBit(src))		//10	3 bytes
		{
			//printf("%x %x %x type:%x\n",*src,*(src+1),*(src+2),*src & 0x7);
			switch (*src & 0x7)
			{
				case 0x0:
					pos = (~(*(WORD*)src >> 3) & 0x1fff) + 1;
					src += 2;
					num = *src++ + 1;
					break;

				default:
					pos = (~(*(WORD*)src >> 3) & 0x1fff) + 1;
					num =( *src & 0x7) +2;
					src += 2;
					break;
			}
			return VERONICA_RLE;
		}
		else						//read 4 bytes
		{
			num = (ReadOneBit(src) << 1) + ReadOneBit(src) + 2;
			pos = (UCHAR)~(*src++) + 1;
			return VERONICA_WINDOW;
		}
	}
}

UCHAR * uncompress(UCHAR *src,unsigned long srcsize, unsigned long &dstsize)
{
	UCHAR *dst = new UCHAR[dstsize * 100];		// make sure big enough
	memset(dst, 0,dstsize);
	UCHAR *p = dst;
	const UCHAR *srcend = src + srcsize;
	const UCHAR *b = src;
	unsigned int num = 0, pos = 0;
	unsigned long size = 0;

	while (src < srcend)
	{
		switch (GetFlagType(src,num,pos))
		{
			case VERONICA_DIRECT:
				*p++ = *src++;
				size++;
				break;

			case VERONICA_WINDOW:
			case VERONICA_RLE:
				if (src > srcend)
					break;
				for (unsigned int i = 0; i < num; i++)
					*p++ = *(p - pos);
				size += num;
				break;

			default:
				break;				
		}

	}

	dstsize = size;
	return dst;
}

