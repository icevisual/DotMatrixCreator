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



BOOL ProcessCmdLineArgvs(
	LPWSTR lpCmdLine, 
	LPWSTR StorageFile, 
	LPWSTR TargetText,
	INT * UsingMessageBox)
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
		if (_tcscmp(Key, L"reminder") == 0)
		{
			*UsingMessageBox = 1;
		//	_tcsncpy_s(TargetText, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[OutPoint[2]]), OutPoint[3]);
		}
		DP2("Key = %s Value = %s\n", Key, Value);

	}

	if (MatchCount < 2)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL ConvertChar2Int(TCHAR * Buf,INT OffSet,INT Length,INT * Output)
{
	INT Sum = 0;
	for (int i = OffSet; i < OffSet + Length; i++)
	{
		if (Buf[i] >= '0' && Buf[i] <= '9')
		{
			Sum = Sum * 10 + (Buf[i] - '0');
		}
		else
		{
			return FALSE;
		}
	}
	*Output = Sum;
	return TRUE;
}


INT  ProcessCmdLineArgvs3(
	_In_ LPWSTR lpCmdLine,
	_In_ INT offset,
	_In_ INT length,
	INT(*ParamsPoint)[4])
{
	//	INT ParamsPoint[20][4] = {0};
	INT ParamsCount = 0;
	// --file=asd        02 04    07 03 
	// --file="asd"
	// --file=asd --name="asd asd ad" 
	INT MaxLength = offset + length;
	INT i = offset;
	while (i < MaxLength)
	{
		// 去除空格和横杠
		while (i < MaxLength && (lpCmdLine[i] == ' ' || lpCmdLine[i] == '-')) i++;
		if (i < MaxLength)
		{
			INT KeyStartPoint = i;
			INT KeyLength = 0;
			while (i < MaxLength && lpCmdLine[i] != '=')
			{
				KeyLength++;
				i++;
			}
			i++;// =
			INT ValueStartPoint = i;
			INT ValueLength = 0;
			while (i < MaxLength && lpCmdLine[i] != ' ')
			{
				ValueLength++;
				i++;
			}
			if (KeyLength > 0 && ValueLength > 0)
			{
				if (lpCmdLine[ValueStartPoint] == '"')
				{
					ValueStartPoint++;
					ValueLength-=2;
				}
				ParamsPoint[ParamsCount][0] = KeyStartPoint;
				ParamsPoint[ParamsCount][1] = KeyLength;
				ParamsPoint[ParamsCount][2] = ValueStartPoint;
				ParamsPoint[ParamsCount][3] = ValueLength;
				//	WCHAR w = lpCmdLine[ParamsPoint[ParamsCount][2]];
				ParamsCount++;
			}
		}
	}
	return ParamsCount;
}

BOOL ProcessCmdLineArgvsH(
	LPWSTR lpCmdLine,
	LPWSTR StorageFile,
	LPWSTR TargetText,
	LPWSTR TitleText,
	INT * UsingMessageBox,
	INT * HoldTime)
{
	size_t Length = _tcslen(lpCmdLine);
	TCHAR Key[MAX_ARGV_KEY_LENGTH] = { 0 };
	TCHAR Value[MAX_ARGV_VALUE_LENGTH] = { 0 };
	INT MatchCount = 0;
	INT ParamsPoint[20][4] = { 0 };
	INT ParamsCount = ProcessCmdLineArgvs3(lpCmdLine, 0, Length,ParamsPoint);
	for (INT i = 0; i < ParamsCount; i++)
	{
		_tcsncpy_s(Key, MAX_ARGV_KEY_LENGTH, &(lpCmdLine[ParamsPoint[i][0]]), ParamsPoint[i][1]);
		_tcsncpy_s(Value, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[ParamsPoint[i][2]]), ParamsPoint[i][3]);
		if (_tcscmp(Key, L"name") == 0)
		{
			MatchCount++;
			_tcsncpy_s(StorageFile, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[ParamsPoint[i][2]]), ParamsPoint[i][3]);
		}
		if (_tcscmp(Key, L"title") == 0)
		{
			MatchCount++;
			_tcsncpy_s(TitleText, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[ParamsPoint[i][2]]), ParamsPoint[i][3]);
		}
		if (_tcscmp(Key, L"text") == 0)
		{
			MatchCount++;
			_tcsncpy_s(TargetText, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[ParamsPoint[i][2]]), ParamsPoint[i][3]);
		}
		if (_tcscmp(Key, L"reminder") == 0)
		{
			*UsingMessageBox = 1;
			//		_tcsncpy_s(TargetText, MAX_ARGV_VALUE_LENGTH, &(lpCmdLine[OutPoint[2]]), OutPoint[3]);
		}
		if (_tcscmp(Key, L"hold") == 0)
		{
			TCHAR HoldTimeBuf[50] = { 0 };
			_tcsncpy_s(HoldTimeBuf, 50, &(lpCmdLine[ParamsPoint[i][2]]), ParamsPoint[i][3]);
			if (ConvertChar2Int(HoldTimeBuf, 0, ParamsPoint[i][3], HoldTime) == FALSE)
			{
				*HoldTime = 0;
			}
		}
		DP2("Key = [%s] Value = [%s]\n", Key, Value);
	}

	if (MatchCount < 2)
	{
		return FALSE;
	}
	return TRUE;
}





void debug_log(char * msg) {
	// if file "DEBUG.CFG" do not exists ,do not write file log 

	char logFileName[20] = { 0 };
	strcpy_s(logFileName, "fplog.txt");
	FILE * fplog;
	int msgLength = strlen(msg);
	char * msgContent = (char*)malloc(msgLength * sizeof(char) + 40);

	char timeStr[64] = { 0 };

	sprintf_s(msgContent, msgLength * sizeof(char) + 40, "%s\n", msg);
	errno_t error_t = fopen_s(&fplog, logFileName, "a");
	if (error_t != 0)
	{

		return;
	}
	fputs(msgContent, fplog);
	fclose(fplog);
	free(msgContent);
}
