// Win32Project2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project2.h"


#define MAX_LOADSTRING 100
// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
//设置字体图形变换矩阵。
void				SetMat(LPMAT2 lpMat);
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
// 获取字体边框
VOID				GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo);
// 浮点数据转换为固定浮点数。
FIXED				FixedFromDouble(double d);   

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	TCHAR StorageFile[MAX_ARGV_VALUE_LENGTH] = { 0 };
	TCHAR TargetFontText[MAX_ARGV_VALUE_LENGTH] = { 0 };

	ProcessCmdLineArgvs(lpCmdLine, StorageFile, TargetFontText);

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
	//TCHAR * TextDemo = L"日行千里的骏马拼命地跟在后面追赶";
	// L"丿了刂丨阝阝卩丶厶匚乊亻丷"
	GetFontGlyph(hWnd, L"电");

	//PrintFont("丶");
	return TRUE;
}

//
//获取字模信息。
void GetFontGlyph(HWND m_hWnd, TCHAR * TextDemo)
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
	errno_t error_t =  fopen_s(&DistFfp,"font.bin","wb");
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
				OutputFontGlyphFixSizeToByteArrayToFile(gm, lpBuf,45, DistFfp);
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

VOID OutputFontGlyphFixSizeToByteArrayToFile(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length, FILE * distFp)
{
	//计算图形每行占用的字节数。
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
			OutputDebugString(_T("○"));
		}
		OutputDebugString(_T("\r\n"));
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
			OutputDebugString(_T("○"));
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
					OutputDebugString(_T("●"));
					FontByteBuf[Cursor] |= 1;
				}
				else
				{
					OutputDebugString(_T("○"));
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
		// 循环中已左移一次
		FontByteBuf[Cursor++] <<= (8 - ByteCount - 1);
		for (INT j = 0; j < ceil((XRight - (8 - ByteCount)) / 8.0); j++)
		{
			Cursor++;
		}
		
		for (INT j = 0; j < XRight; j++)
		{
			OutputDebugString(_T("○"));
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
			OutputDebugString(_T("○"));
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

	OutputFontGlyphXY(TargetWidth,TargetHeight, FontByteBuf);
	//OutputFontGlyphXY(gm.gmBlackBoxX, gm.gmBlackBoxY, lpBuf);
	size_t InSize = fwrite(FontByteBuf, sizeof(FontByteBuf[0]), dwNeedSize,distFp);
	wsprintf(MSG, L"InSize = %d\n", InSize);
	OutputDebugString(MSG);
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


