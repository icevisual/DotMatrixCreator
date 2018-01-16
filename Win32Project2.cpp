// Win32Project2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project2.h"

#define MAX_LOADSTRING 100
// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

INT g_UsingMessageBox = 0;
INT g_HoldFileTime = 0;

//设置字体图形变换矩阵。
void				SetMat(LPMAT2 lpMat);
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
// 获取字体边框
VOID				GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile);
// 浮点数据转换为固定浮点数。
FIXED				FixedFromDouble(double d);   
// 获取标题字体边框（含中文，大小写英文，数字，生成半个字存储）
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

	hInst = hInstance; // 将实例句柄存储在全局变量中
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}
	TCHAR * TextDemo = L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890-='\"?<>,.()";
	if (TitleLength > 0)
		GetTitleFontGlyph(hWnd, TitleText, StorageFile);

	//TCHAR * TextDemo = L"日行千里的骏马拼命地跟在后面追赶";
	// L"丿了刂丨阝阝卩丶厶匚乊亻丷"
	GetFontGlyph(hWnd, TargetFontText, StorageFile);

	if(g_UsingMessageBox)
		MessageBox(0,L"字库生成成功！",L"提示", MB_ICONINFORMATION);

	//PrintFont("丶");
	return TRUE;
}

//
//获取字模信息。
void GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile)
{
	HDC hDC = ::GetDC(m_hWnd);
	//创建字体。
	HFONT hFont = CreateFont(23, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, GB2312_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"黑体");
	//设置字体到当前设备。
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	//设置字体图形变换矩阵        
	MAT2 mat2;
	SetMat(&mat2);


	GLYPHMETRICS gm;

	FILE * DistFfp = NULL;
	errno_t error_t = _wfopen_s(&DistFfp, StorageFile,L"wb");

	if (error_t != 0)
	{

		return;
	}
//	TCHAR * TextDemo = L"在此处添加使用的任何绘图代码";
	for (int i = 0; i < _tcslen(TextDemo); i++)
	{
		//获取这个字符图形需要的字节的大小。
		DWORD dwNeedSize = GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, 0, NULL, &mat2);
		if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
		{
			//按需要分配内存。
			LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
			if (lpBuf)
			{
				//获取字符图形的数据到缓冲区。
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
//获取字模信息。
void GetTitleFontGlyph(HWND m_hWnd, TCHAR * TextDemo, TCHAR * StorageFile)
{
	HDC hDC = ::GetDC(m_hWnd);
	//创建字体。
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, GB2312_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"黑体");
	//设置字体到当前设备。
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	//设置字体图形变换矩阵        
	MAT2 mat2;
	SetMat(&mat2);


	GLYPHMETRICS gm;

	FILE * DistFfp = NULL;
	errno_t error_t = _wfopen_s(&DistFfp, StorageFile, L"wb");

	if (error_t != 0)
	{

		return;
	}
	//	TCHAR * TextDemo = L"在此处添加使用的任何绘图代码";
	for (int i = 0; i < _tcslen(TextDemo); i++)
	{
		//获取这个字符图形需要的字节的大小。
		DWORD dwNeedSize = GetGlyphOutline(hDC, TextDemo[i], GGO_BITMAP, &gm, 0, NULL, &mat2);
		if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
		{
			//按需要分配内存。
			LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeedSize);
			if (lpBuf)
			{
				//获取字符图形的数据到缓冲区。
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
	//计算图形每行占用的字节数。
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
			DP0("○");
		}
		DP0("\r\n");
	}

	//显示每行图形的数据。
	for (INT i = 0; i < gm.gmBlackBoxY; i++)
	{
		// 每行输出的点数，达到 gm.gmBlackBoxY 不再输出
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
			DP0("○");
		}

		for (INT j = 0; j < nByteCount; j++)
		{

			BYTE btCode = lpBuf[i* nByteCount + j];

			//按字节输出每点的数据。
			for (int k = 0; k < 8; k++)
			{
				if (PointCount == gm.gmBlackBoxX)
					break;
				if (btCode & (0x80 >> k))
				{
					DP0("●");
					//	FontByteBuf[Cursor] |= 1;
					FontByteBuf[Cursor] |= (1 << ByteCount);
				}
				else
				{
					DP0("○");
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
		// 循环中已左移一次
		//	FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		Cursor++;
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}

		for (INT j = 0; j < XRight; j++)
		{
			DP0("○");
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
			DP0("○");
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
	//计算图形每行占用的字节数。
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
			DP0("○");
		}
		DP0("\r\n");
	}

	//显示每行图形的数据。
	for (INT i = 0; i < gm.gmBlackBoxY; i++)
	{
		// 每行输出的点数，达到 gm.gmBlackBoxY 不再输出
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
			DP0("○");
		}

		for (INT j = 0; j < nByteCount; j++)
		{

			BYTE btCode = lpBuf[i* nByteCount + j];

			//按字节输出每点的数据。
			for (int k = 0; k < 8; k++)
			{
				if (PointCount == gm.gmBlackBoxX)
					break;
				if (btCode & (0x80 >> k))
				{
					DP0("●");
					//	FontByteBuf[Cursor] |= 1;
					FontByteBuf[Cursor] |= (1 << ByteCount);
				}
				else
				{
					DP0("○");
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
		// 循环中已左移一次
		//	FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		Cursor++;
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}

		for (INT j = 0; j < XRight; j++)
		{
			DP0("○");
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
			DP0("○");
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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


//浮点数据转换为固定浮点数。
FIXED FixedFromDouble(double d)
{
	long l;
	l = (long)(d * 65536L);
	return *(FIXED *)&l;
}

//设置字体图形变换矩阵。
void SetMat(LPMAT2 lpMat)
{
	lpMat->eM11 = FixedFromDouble(2);
	lpMat->eM12 = FixedFromDouble(0);
	lpMat->eM21 = FixedFromDouble(0);
	lpMat->eM22 = FixedFromDouble(2);
}


