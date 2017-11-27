// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>

//#define _DEBUG

#ifdef _DEBUG1

#define DP0(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);}    
#define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);}    
#define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);}    
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);}    
#define DP4(fmt,var1,var2,var3,var4) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3,var4);OutputDebugString(sOut);}    
#define DP5(fmt,var1,var2,var3,var4,var5) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3,var4,var5);OutputDebugString(sOut);}    
#define DP6(fmt,var1,var2,var3,var4,var5,var6) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3,var4,var5,var6);OutputDebugString(sOut);}    

#endif    
#ifndef _DEBUG1   

#define DP0(fmt) ;    
#define DP1(fmt, var) ;    
#define DP2(fmt,var1,var2) ;    
#define DP3(fmt,var1,var2,var3) ;    
#define DP4(fmt,var1,var2,var3,var4) ;   
#define DP5(fmt,var1,var2,var3,var4,var5) ;   
#define DP6(fmt,var1,var2,var3,var4,var5,var6) ;   
#endif  

// 存储的文件名最大长度
#define MAX_FILE_NAME_LENGTH 50  
// 存储的命令行参数值的最大长度
#define MAX_ARGV_KEY_LENGTH 50  
// 存储的命令行参数值的最大长度
#define MAX_ARGV_VALUE_LENGTH 255  
// 从命令行获取参数
BOOL				ProcessCmdLineArgvs(LPWSTR lpCmdLine, LPWSTR StorageFile, LPWSTR TargetText);
// 从点阵文件读取字体点阵输出
INT					PrintFont(char * incode);
// 输出边框字体，传入长宽
VOID				OutputFontGlyphXY(INT width, INT height, LPBYTE lpBuf);
// 输出边框字体
VOID				OutputFontGlyph(GLYPHMETRICS gm, LPBYTE lpBuf);
// 输出边框字体 45 * 45
VOID				OutputFontGlyphFixSize(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length);
// 输出边框字体 45 * 45 转 Byte Array
VOID				OutputFontGlyphFixSizeToByteArray(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length);
// 输出边框字体 45 * 45 转 Byte Array，写入文件
VOID				OutputFontGlyphFixSizeToByteArrayToFile(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length,FILE * distFp);