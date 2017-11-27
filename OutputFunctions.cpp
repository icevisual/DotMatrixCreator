#include "stdafx.h"

VOID OutputFontGlyph(GLYPHMETRICS gm, LPBYTE lpBuf)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	int nByteCount = ((gm.gmBlackBoxX + 31) >> 5) << 2;
	TCHAR MSG[100] = { 0 };
	wsprintf(MSG, L"gm.gmBlackBoxX = %d,gm.gmBlackBoxY = %d\n", gm.gmBlackBoxX, gm.gmBlackBoxY);
	OutputDebugString(MSG);
	//��ʾÿ��ͼ�ε����ݡ�
	for (int i = 0; i < gm.gmBlackBoxY; i++)
	{
		INT LineCount = 0;
		for (int j = 0; j < nByteCount; j++)
		{
			BYTE btCode = lpBuf[i* nByteCount + j];
			//���ֽ����ÿ������ݡ�
			for (int k = 0; k < 8; k++)
			{
				if (LineCount == gm.gmBlackBoxX)
					break;
				if (btCode & (0x80 >> k))
				{
					OutputDebugString(_T("��"));
				}
				else
				{
					OutputDebugString(_T("��"));
				}
				LineCount++;
			}

		}
		//
		OutputDebugString(_T("\r\n"));
	}
}


VOID OutputFontGlyphFixSize(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	int nByteCount = ((gm.gmBlackBoxX + 31) >> 5) << 2;
	TCHAR MSG[100] = { 0 };


	INT TargetWidth = Length;
	INT XLeft = floor((TargetWidth - gm.gmBlackBoxX) / 2.0);
	INT XRight = TargetWidth - XLeft - gm.gmBlackBoxX;

	INT TargetHeight = Length;
	INT YUp = floor((TargetHeight - gm.gmBlackBoxY) / 2.0);
	INT YDown = TargetHeight - YUp - gm.gmBlackBoxY;

	wsprintf(MSG, L"gmX = %d,gmY = %d,XL = %d,XR = %d,YU = %d,YD = %d\n",
		gm.gmBlackBoxX, gm.gmBlackBoxY,
		XLeft, XRight, YUp, YDown);
	OutputDebugString(MSG);
	for (INT i = 0; i < YUp; i++)
	{
		for (INT j = 0; j < TargetWidth; j++)
		{
			OutputDebugString(_T("��"));
		}
		OutputDebugString(_T("\r\n"));
	}

	//��ʾÿ��ͼ�ε����ݡ�
	for (int i = 0; i < gm.gmBlackBoxY; i++)
	{
		INT LineCount = 0;

		for (INT j = 0; j < XLeft; j++)
		{
			OutputDebugString(_T("��"));
		}
		for (int j = 0; j < nByteCount; j++)
		{

			BYTE btCode = lpBuf[i* nByteCount + j];

			//���ֽ����ÿ������ݡ�
			for (int k = 0; k < 8; k++)
			{
				if (LineCount == gm.gmBlackBoxX)
					break;
				if (btCode & (0x80 >> k))
				{
					OutputDebugString(_T("��"));
				}
				else
				{
					OutputDebugString(_T("��"));
				}
				LineCount++;
			}

		}
		for (INT j = 0; j < XRight; j++)
		{
			OutputDebugString(_T("��"));
		}
		//
		OutputDebugString(_T("\r\n"));
	}

	for (INT i = 0; i < YDown; i++)
	{
		for (INT j = 0; j < TargetWidth; j++)
		{
			OutputDebugString(_T("��"));
		}
		OutputDebugString(_T("\r\n"));
	}
}

VOID OutputFontGlyphXY(INT width, INT height, LPBYTE lpBuf)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	INT nByteCount = (INT)(ceil(width / 8.0)); //((width + 31) >> 5) << 2;
	TCHAR MSG[100] = { 0 };
	wsprintf(MSG, L"gm.gmBlackBoxX = %d,gm.gmBlackBoxY = %d\n", width, height);
	OutputDebugString(MSG);
	//��ʾÿ��ͼ�ε����ݡ�
	for (int i = 0; i < height; i++)
	{
		INT LineCount = 0;

		for (int j = 0; j < nByteCount; j++)
		{

			BYTE btCode = lpBuf[i* nByteCount + j];

			//���ֽ����ÿ������ݡ�
			for (int k = 0; k < 8; k++)
			{
				if (LineCount == width)
					break;
				if (btCode & (0x80 >> k))
				{
					OutputDebugString(_T("��"));
				}
				else
				{
					OutputDebugString(_T("��"));
				}
				LineCount++;
			}

		}
		//
		OutputDebugString(_T("\r\n"));
	}
}



VOID OutputFontGlyphFixSizeToByteArray(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	INT nByteCount = ((gm.gmBlackBoxX + 31) >> 5) << 2;
	TCHAR MSG[100] = { 0 };

	INT TargetWidth = Length;
	INT XLeft = floor((TargetWidth - gm.gmBlackBoxX) / 2.0);
	INT XRight = TargetWidth - XLeft - gm.gmBlackBoxX;

	INT TargetHeight = Length;
	INT YUp = floor((TargetHeight - gm.gmBlackBoxY) / 2.0);
	INT YDown = TargetHeight - YUp - gm.gmBlackBoxY;

	wsprintf(MSG, L"gmX = %d,gmY = %d,XL = %d,XR = %d,YU = %d,YD = %d\n",
		gm.gmBlackBoxX, gm.gmBlackBoxY,
		XLeft, XRight, YUp, YDown);
	OutputDebugString(MSG);

	DWORD dwNeedSize = TargetHeight * (DWORD)(ceil(TargetWidth / 8.0));
	//BYTE FontBlock[TargetHeight ];
	LPBYTE FontByteBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
	// HeapFree(GetProcessHeap(), 0, FontByteBuf);
	INT Cursor = 0;
	for (INT i = 0; i < YUp; i++)
	{
		for (INT j = 0; j < (INT)(ceil(TargetWidth / 8.0)); j++)
		{
			FontByteBuf[Cursor++] = 0x00;
		}

		for (INT j = 0; j < TargetWidth; j++)
		{
			OutputDebugString(_T("��"));
		}
		OutputDebugString(_T("\r\n"));
	}

	//��ʾÿ��ͼ�ε����ݡ�
	for (INT i = 0; i < gm.gmBlackBoxY; i++)
	{
		// ÿ������ĵ������ﵽ gm.gmBlackBoxY �������
		INT PointCount = 0;

		INT ByteCount = 0;
		for (INT j = 0; j < XLeft; j++)
		{
			ByteCount++;
			OutputDebugString(_T("��"));
		}

		for (INT j = 0; j < nByteCount; j++)
		{

			BYTE btCode = lpBuf[i* nByteCount + j];

			//���ֽ����ÿ������ݡ�
			for (int k = 0; k < 8; k++)
			{
				if (PointCount == gm.gmBlackBoxX)
					break;
				if (btCode & (0x80 >> k))
				{
					OutputDebugString(_T("��"));
					FontByteBuf[Cursor] |= 1;
				}
				else
				{
					OutputDebugString(_T("��"));
				}
				ByteCount++;
				if (ByteCount == 8)
				{
					Cursor++;
					ByteCount = 0;
				}
				else
				{
					FontByteBuf[Cursor] = FontByteBuf[Cursor] << 1;
				}
				PointCount++;
			}

		}
		// ѭ����������һ��
		FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}

		for (INT j = 0; j < XRight; j++)
		{
			OutputDebugString(_T("��"));
		}
		//
		OutputDebugString(_T("\r\n"));
	}

	for (INT i = 0; i < YDown; i++)
	{
		for (INT j = 0; j < (INT)(ceil(TargetWidth / 8.0)); j++)
		{
			FontByteBuf[Cursor++] = 0x00;
		}
		for (INT j = 0; j < TargetWidth; j++)
		{
			OutputDebugString(_T("��"));
		}
		OutputDebugString(_T("\r\n"));
	}

	for (INT i = 0; i < TargetHeight; i++)
	{
		INT LineCount = (INT)(ceil(TargetWidth / 8.0));
		for (INT j = 0; j < LineCount; j++)
		{
			wsprintf(MSG, L"0x%02X ", FontByteBuf[i * LineCount + j]);
			OutputDebugString(MSG);
		}
		OutputDebugString(_T("\r\n"));
	}

	OutputFontGlyphXY(TargetWidth, TargetHeight, FontByteBuf);
	//OutputFontGlyphXY(gm.gmBlackBoxX, gm.gmBlackBoxY, lpBuf);


	HeapFree(GetProcessHeap(), 0, FontByteBuf);
}




#define HoldLength  128
#define FontWidth 32

int PrintFont(char * incode)
{
	int    i, j, k;
	//unsigned    char    incode[3] = "د";    //    Ҫ�����ĺ���   
	unsigned    char    qh, wh;
	unsigned    long    offset;
	//    ռ�����ֽ�,    ȡ����λ��   
	qh = incode[0] - 0xa0; // �������
	wh = incode[1] - 0xa0; // ���λ��
	offset = (94 * (qh - 1) + (wh - 1)) * HoldLength; /*�õ�ƫ��λ��*/

	FILE    * HZK = NULL;
	char    mat[HoldLength];
	errno_t error = fopen_s(&HZK, "HZK32", "rb");

	if (error != 0)
	{
		return 0;
	}
	fseek(HZK, offset, SEEK_SET);
	fread(mat, HoldLength, 1, HZK);
	TCHAR MSG[100] = { 0 };

	int EachBitCount = FontWidth / 8;
	//��ʾ
	for (j = 0;j<FontWidth;j++)
	{
		for (i = 0;i<EachBitCount;i++)
		{
			for (k = 0;k < 8;k++)
			{
				if (mat[EachBitCount * j + i] & (0x80 >> k))
				{
					OutputDebugString(L"��");
				}
				else
				{
					OutputDebugString(L"��");
				}
			}
		}
		OutputDebugString(L"\n");
	}
	fclose(HZK);
	return 0;
}