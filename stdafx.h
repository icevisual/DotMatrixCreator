// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
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

// �洢���ļ�����󳤶�
#define MAX_FILE_NAME_LENGTH 50  
// �洢�������в���ֵ����󳤶�
#define MAX_ARGV_KEY_LENGTH 50  
// �洢�������в���ֵ����󳤶�
#define MAX_ARGV_VALUE_LENGTH 255  
// �������л�ȡ����
BOOL				ProcessCmdLineArgvs(LPWSTR lpCmdLine, LPWSTR StorageFile, LPWSTR TargetText);
// �ӵ����ļ���ȡ����������
INT					PrintFont(char * incode);
// ����߿����壬���볤��
VOID				OutputFontGlyphXY(INT width, INT height, LPBYTE lpBuf);
// ����߿�����
VOID				OutputFontGlyph(GLYPHMETRICS gm, LPBYTE lpBuf);
// ����߿����� 45 * 45
VOID				OutputFontGlyphFixSize(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length);
// ����߿����� 45 * 45 ת Byte Array
VOID				OutputFontGlyphFixSizeToByteArray(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length);
// ����߿����� 45 * 45 ת Byte Array��д���ļ�
VOID				OutputFontGlyphFixSizeToByteArrayToFile(GLYPHMETRICS gm, LPBYTE lpBuf, INT Length,FILE * distFp);