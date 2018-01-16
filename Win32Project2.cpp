// Win32Project2.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32Project2.h"

#define MAX_LOADSTRING 100
// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

INT g_UsingMessageBox = 0;
INT g_HoldFileTime = 0;

//��������ͼ�α任����
void				SetMat(LPMAT2 lpMat);
// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
// ��ȡ����߿�
VOID				GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile);
// ��������ת��Ϊ�̶���������
FIXED				FixedFromDouble(double d);   
// ��ȡ��������߿򣨺����ģ���СдӢ�ģ����֣����ɰ���ִ洢��
VOID				GetTitleFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile);

VOID OutputFontGlyphFixSizeToByteArrayToFileSeparate2(GLYPHMETRICS gm, LPBYTE lpBuf, INT Width, INT Height, FILE * distFp);

#define MAX_LOG_MSG_LENGTH  100

CHAR g_MSG[MAX_LOG_MSG_LENGTH] = { 0 };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	TCHAR StorageFile[MAX_ARGV_VALUE_LENGTH] = { 0 };
	TCHAR TargetFontText[MAX_ARGV_VALUE_LENGTH] = { 0 };
	TCHAR TitleText[MAX_ARGV_VALUE_LENGTH] = { 0 };
	
	if (FALSE == ProcessCmdLineArgvsH(
		lpCmdLine, 
		StorageFile, 
		TargetFontText,
		TitleText,
		&g_UsingMessageBox,
		&g_HoldFileTime)
		)
	{
		return -1;
	}

	size_t TitleLength = _tcslen(TitleText);
	// TODO : Remove
	sprintf_s(g_MSG, MAX_LOG_MSG_LENGTH,"g_UsingMessageBox[%d],g_HoldFileTime[%d]", g_UsingMessageBox, g_HoldFileTime);
	debug_log(g_MSG);
	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}
	TCHAR * TextDemo = L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890-='\"?<>,.()";
	if (TitleLength > 0)
		GetTitleFontGlyph(hWnd, TitleText, StorageFile);

	//TCHAR * TextDemo = L"����ǧ��Ŀ���ƴ���ظ��ں���׷��";
	// L"د����ح������ؼ���΁Y��R"
	GetFontGlyph(hWnd, TargetFontText, StorageFile);

	if(g_UsingMessageBox)
		MessageBox(0,L"�ֿ����ɳɹ���",L"��ʾ", MB_ICONINFORMATION);

	//PrintFont("ؼ");
	return TRUE;
}

//
//��ȡ��ģ��Ϣ��
void GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile)
{
	HDC hDC = ::GetDC(m_hWnd);
	//�������塣
	HFONT hFont = CreateFont(23, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, GB2312_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����");
	//�������嵽��ǰ�豸��
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	//��������ͼ�α任����        
	MAT2 mat2;
	SetMat(&mat2);


	GLYPHMETRICS gm;

	FILE * DistFfp = NULL;
	errno_t error_t = _wfopen_s(&DistFfp, StorageFile,L"wb");

	if (error_t != 0)
	{

		return;
	}
//	TCHAR * TextDemo = L"�ڴ˴����ʹ�õ��κλ�ͼ����";
	for (int i = 0; i < _tcslen(TextDemo); i++)
	{
		//��ȡ����ַ�ͼ����Ҫ���ֽڵĴ�С��
		DWORD dwNeedSize = GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, 0, NULL, &mat2);
		if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
		{
			//����Ҫ�����ڴ档
			LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
			if (lpBuf)
			{
				//��ȡ�ַ�ͼ�ε����ݵ���������
				GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, dwNeedSize, lpBuf, &mat2);
				OutputFontGlyphFixSizeToByteArrayToFile(gm, lpBuf,45, 45, DistFfp);
				//
				HeapFree(GetProcessHeap(), 0, lpBuf);
			}
		}
	}
	sprintf_s(g_MSG, MAX_LOG_MSG_LENGTH, "Sleep(g_HoldFileTime)[%d]", g_HoldFileTime);
	debug_log(g_MSG);
	if (g_HoldFileTime > 0)
	{
		Sleep(g_HoldFileTime);
	}

	fclose(DistFfp);
	//
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	//
	ReleaseDC(m_hWnd, hDC);
}


//
//��ȡ��ģ��Ϣ��
void GetTitleFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile)
{
	HDC hDC = ::GetDC(m_hWnd);
	//�������塣
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, GB2312_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����");
	//�������嵽��ǰ�豸��
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	//��������ͼ�α任����        
	MAT2 mat2;
	SetMat(&mat2);


	GLYPHMETRICS gm;

	FILE * DistFfp = NULL;
	errno_t error_t = _wfopen_s(&DistFfp, StorageFile, L"wb");

	if (error_t != 0)
	{

		return;
	}
	//	TCHAR * TextDemo = L"�ڴ˴����ʹ�õ��κλ�ͼ����";
	for (int i = 0; i < _tcslen(TextDemo); i++)
	{
		//��ȡ����ַ�ͼ����Ҫ���ֽڵĴ�С��
		DWORD dwNeedSize = GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, 0, NULL, &mat2);
		if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
		{
			//����Ҫ�����ڴ档
			LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
			if (lpBuf)
			{
				//��ȡ�ַ�ͼ�ε����ݵ���������
				GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, dwNeedSize, lpBuf, &mat2);
				
				if ((TextDemo[i] > 'a' && TextDemo[i] < 'z')
					|| (TextDemo[i] > 'A' && TextDemo[i] < 'Z')
					|| (TextDemo[i] > '0' && TextDemo[i] < '9')
					)
				{
					DP1("C=%c ", TextDemo[i]);
					OutputFontGlyphFixSizeToByteArrayToFile(gm, lpBuf, 19, 38, DistFfp);
				}
				else if (TextDemo[i] < 128)
				{
					OutputFontGlyphFixSizeToByteArrayToFile(gm, lpBuf, 19, 38, DistFfp);
				}
				else
				{
					OutputFontGlyphFixSizeToByteArrayToFileSeparate2(gm, lpBuf, 38, 38, DistFfp);
				}
				//
				HeapFree(GetProcessHeap(), 0, lpBuf);
			}
		}
	}

	fclose(DistFfp);
	//
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	//
	ReleaseDC(m_hWnd, hDC);
}

//
//BYTE ByteReverse(BYTE cByte)
//{
//	BYTE R = 0;
//	for (INT i = 0; i < 8; i++)
//	{
//
//	}
//}


VOID OutputFontGlyphFixSizeToByteArrayToFile(
	GLYPHMETRICS gm, LPBYTE lpBuf, INT Width, INT Height, FILE * distFp)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	INT nByteCount = ((gm.gmBlackBoxX + 31) >> 5) << 2;

	INT TargetWidth = Width;
	INT XLeft = floor((TargetWidth - gm.gmBlackBoxX) / 2.0);
	INT XRight = TargetWidth - XLeft - gm.gmBlackBoxX;

	INT TargetHeight = Height;
	INT YUp = floor((TargetHeight - gm.gmBlackBoxY) / 2.0);
	INT YDown = TargetHeight - YUp - gm.gmBlackBoxY;

	DP6("gmX = %d,gmY = %d,XL = %d,XR = %d,YU = %d,YD = %d\n",
		gm.gmBlackBoxX, gm.gmBlackBoxY,
		XLeft, XRight, YUp, YDown);

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
			DP0("��");
		}
		DP0("\r\n");
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
			if (ByteCount == 8)
			{
				Cursor++;
				ByteCount = 0;
			}
			DP0("��");
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
					DP0("��");
					//	FontByteBuf[Cursor] |= 1;
					FontByteBuf[Cursor] |= (1 << ByteCount);
				}
				else
				{
					DP0("��");
				}
				ByteCount++;
				if (ByteCount == 8)
				{
					Cursor++;
					ByteCount = 0;
				}
				else
				{
					//	FontByteBuf[Cursor] = FontByteBuf[Cursor] << 1;
				}
				PointCount++;
			}

		}
		// ѭ����������һ��
		//	FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		Cursor++;
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}

		for (INT j = 0; j < XRight; j++)
		{
			DP0("��");
		}
		//
		DP0("\r\n");
	}

	for (INT i = 0; i < YDown; i++)
	{
		for (INT j = 0; j < (INT)(ceil(TargetWidth / 8.0)); j++)
		{
			FontByteBuf[Cursor++] = 0x00;
		}
		for (INT j = 0; j < TargetWidth; j++)
		{
			DP0("��");
		}
		DP0("\r\n");
	}

	//for (INT i = 0; i < TargetHeight; i++)
	//{
	//	INT LineCount = (INT)(ceil(TargetWidth / 8.0));
	//	for (INT j = 0; j < LineCount; j++)
	//	{
	//		DP1("0x%02X ", FontByteBuf[i * LineCount + j]);
	//	}
	//	DP0("\r\n");
	//}

	//OutputFontGlyphXY(TargetWidth,TargetHeight, FontByteBuf);
	//OutputFontGlyphXY(gm.gmBlackBoxX, gm.gmBlackBoxY, lpBuf);
	size_t InSize = fwrite(FontByteBuf, sizeof(FontByteBuf[0]), dwNeedSize, distFp);
	DP1("fwrite Size = %d\n", InSize);
	HeapFree(GetProcessHeap(), 0, FontByteBuf);
}




VOID OutputFontGlyphFixSizeToByteArrayToFileSeparate2(
	GLYPHMETRICS gm, LPBYTE lpBuf, INT Width, INT Height, FILE * distFp)
{
	//����ͼ��ÿ��ռ�õ��ֽ�����
	INT nByteCount = ((gm.gmBlackBoxX + 31) >> 5) << 2;

	INT TargetWidth = Width;
	INT XLeft = floor((TargetWidth - gm.gmBlackBoxX) / 2.0);
	INT XRight = TargetWidth - XLeft - gm.gmBlackBoxX;

	INT TargetHeight = Height;
	INT YUp = floor((TargetHeight - gm.gmBlackBoxY) / 2.0);
	INT YDown = TargetHeight - YUp - gm.gmBlackBoxY;

	DP6("gmX = %d,gmY = %d,XL = %d,XR = %d,YU = %d,YD = %d\n",
		gm.gmBlackBoxX, gm.gmBlackBoxY,
		XLeft, XRight, YUp, YDown);

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
			DP0("��");
		}
		DP0("\r\n");
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
			if (ByteCount == 8)
			{
				Cursor++;
				ByteCount = 0;
			}
			DP0("��");
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
					DP0("��");
					//	FontByteBuf[Cursor] |= 1;
					FontByteBuf[Cursor] |= (1 << ByteCount);
				}
				else
				{
					DP0("��");
				}
				ByteCount++;
				if (ByteCount == 8)
				{
					Cursor++;
					ByteCount = 0;
				}
				else
				{
					//	FontByteBuf[Cursor] = FontByteBuf[Cursor] << 1;
				}
				PointCount++;
			}

		}
		// ѭ����������һ��
		//	FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		Cursor++;
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}

		for (INT j = 0; j < XRight; j++)
		{
			DP0("��");
		}
		//
		DP0("\r\n");
	}

	for (INT i = 0; i < YDown; i++)
	{
		for (INT j = 0; j < (INT)(ceil(TargetWidth / 8.0)); j++)
		{
			FontByteBuf[Cursor++] = 0x00;
		}
		for (INT j = 0; j < TargetWidth; j++)
		{
			DP0("��");
		}
		DP0("\r\n");
	}

	for (INT i = 0; i < TargetHeight; i++)
	{
		INT LineCount = (INT)(ceil(TargetWidth / 8.0));
		for (INT j = 0; j < LineCount; j++)
		{
			DP1("0x%02X ", FontByteBuf[i * LineCount + j]);
		}
		DP0("\r\n");
	}

	//OutputFontGlyphXY(TargetWidth,TargetHeight, FontByteBuf);
	//OutputFontGlyphXY(gm.gmBlackBoxX, gm.gmBlackBoxY, lpBuf);
	size_t InSize = fwrite(FontByteBuf, sizeof(FontByteBuf[0]), dwNeedSize, distFp);
	DP1("fwrite Size = %d\n", InSize);
	HeapFree(GetProcessHeap(), 0, FontByteBuf);
}


//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//��������ת��Ϊ�̶���������
FIXED FixedFromDouble(double d)
{
	long l;
	l = (long)(d * 65536L);
	return *(FIXED *)&l;
}

//��������ͼ�α任����
void SetMat(LPMAT2 lpMat)
{
	lpMat->eM11 = FixedFromDouble(2);
	lpMat->eM12 = FixedFromDouble(0);
	lpMat->eM21 = FixedFromDouble(0);
	lpMat->eM22 = FixedFromDouble(2);
}


