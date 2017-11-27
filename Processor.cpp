#include "stdafx.h"
INT DetectOrder(INT Length)
{
	INT r = 1;
	while (Length > 0)
	{
		Length /= 10;
		r *= 10;
	}
	return r;
}
BOOL  ProcessCmdLineArgvs2(_In_ LPWSTR lpCmdLine, INT offset, INT * Point)
{
	size_t Length = _tcslen(lpCmdLine);

	if (offset >= Length - 1)
		return FALSE;

	// 以长度来定起始下标和长度的占位
	LPWSTR Argvs[2];// name , text
	TCHAR Key[20];

	INT i = offset, j = 0,Result = 0,Curor = 0;
	while (lpCmdLine[i] == L'-' || lpCmdLine[i] == L' ') i++;
	Point[Curor++] = i;
	// --file=asd        02 04    07 03 
	// --file="asd"
	// --file=asd --name="asd asd ad"
	for (; i < Length; i++)
	{
		if (lpCmdLine[i] == L'=')
		{
			Point[Curor++] = i - Point[Curor - 1];
			break;
		}
	}
	Point[Curor++] = ++i;
	CHAR hasQuotation = 0;
	if (lpCmdLine[i] == L'\"')
	{
		hasQuotation = 1;
		i++;
		Point[Curor - 1] ++;
	}

	for (; i < Length; i++)
	{
		if (hasQuotation)
		{
			if (lpCmdLine[i] == L'\"')
			{
				Point[Curor++] = i - Point[Curor - 1];
				break;
			}
		}
		else
		{
			if (lpCmdLine[i] == L' ' || lpCmdLine[i] == 0)
			{
				Point[Curor++] = i - Point[Curor - 1];
				break;
			}
		}
	}
	if (Point[0] && Point[1] && Point[2] && Point[3])
		return TRUE;
	return FALSE;
}

BOOL ProcessCmdLineArgvs(LPWSTR lpCmdLine, LPWSTR StorageFile, LPWSTR TargetText)
{
	size_t Length = _tcslen(lpCmdLine);
	INT OutPoint[4] = {0};
	TCHAR Key[MAX_ARGV_KEY_LENGTH] = { 0 };
	TCHAR Value[MAX_ARGV_VALUE_LENGTH] = { 0 };
	INT MatchCount = 0;
	while (ProcessCmdLineArgvs2(lpCmdLine, OutPoint[2] + OutPoint[3], OutPoint))
	{
		_tcsncpy_s(Key, MAX_ARGV_KEY_LENGTH, &(lpCmdLine[OutPoint[0]]), OutPoint[1]);
		_tcsncpy_s(Value, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[OutPoint[2]]), OutPoint[3]);
		if (_tcscmp(Key, L"name") == 0)
		{
			MatchCount++;
			_tcsncpy_s(StorageFile, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[OutPoint[2]]), OutPoint[3]);
		}
		if (_tcscmp(Key, L"text") == 0)
		{
			MatchCount++;
			_tcsncpy_s(TargetText, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[OutPoint[2]]), OutPoint[3]);
		}
		DP2("Key = %s Value = %s\n", Key, Value);

	}

	if (MatchCount < 2)
	{
		return FALSE;
	}
	return TRUE;
}