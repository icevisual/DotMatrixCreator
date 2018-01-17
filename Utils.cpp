

#include "stdafx.h"
#include "Strsafe.h"

//   控制台前景颜色
enum ConsoleForegroundColor
{
	enmCFC_Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
	enmCFC_Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	enmCFC_Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	enmCFC_Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	enmCFC_Purple = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	enmCFC_Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	enmCFC_Gray = FOREGROUND_INTENSITY,
	enmCFC_White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	enmCFC_HighWhite = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	enmCFC_Black = 0,
};

enum ConsoleBackGroundColor
{
	enmCBC_Red = BACKGROUND_INTENSITY | BACKGROUND_RED,
	enmCBC_Green = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	enmCBC_Blue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	enmCBC_Yellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	enmCBC_Purple = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	enmCBC_Cyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	enmCBC_White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	enmCBC_HighWhite = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	enmCBC_Black = 0,
};
// 设置控制台输出颜色
void SetConsoleColor(ConsoleForegroundColor foreColor = enmCFC_White, ConsoleBackGroundColor backColor = enmCBC_Black)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, foreColor | backColor);
}



DWORD char2Wchar(char * sBuf, int sBufSize, WCHAR * output)
{
	DWORD dBufSize = MultiByteToWideChar(CP_ACP, 0, sBuf, sBufSize, NULL, 0);
	WCHAR * dBuf = new WCHAR[dBufSize + 1];
	wmemset(dBuf, 0, dBufSize);
	int nRet = MultiByteToWideChar(CP_ACP, 0, sBuf, sBufSize, dBuf, dBufSize);
	dBuf[dBufSize] = '\0';
	if (nRet <= 0)
		return  GetLastError();
	else
		wcscpy_s(output, dBufSize + 1, dBuf);
	delete(dBuf);
	return 0;
}

VOID ShowLastErrorMsg(LPTSTR lpszFunction, DWORD dw)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	if (dw == 0)
		dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
	// printf("\x1b[%d;%dm%s\x1b[%dm", backcolor, frountcolor, str, control);
	// printf("\x1b[%d;%dm%s\x1b[%dm", 40, 31, str, 1);

	SetConsoleColor(enmCFC_Red, enmCBC_Black);
	_tprintf(TEXT("%s"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


INT GetMilliSecondOfDay()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	//printf("%4d-%02d-%02d %02d:%02d:%02d.%03d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	return sys.wHour * 3600000 + sys.wMinute * 60000 + sys.wSecond * 1000 + sys.wMilliseconds;
}

VOID GetHMS(TCHAR * output)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	wsprintf(output, _T("%02d:%02d:%02d.%03d"), sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
}


VOID LocalRegisterHotKey(_In_opt_ HWND hWnd, _In_ int id, _In_ UINT fsModifiers, _In_ UINT vk) {
	if (RegisterHotKey(hWnd, id, fsModifiers, vk)) //0x42 is 'b'
	{
		_tprintf(_T("Hotkey '%c' registered, using MOD_NOREPEAT flag\n"), vk);
	}
	else {
		_tprintf(_T("Hotkey '%c' register failed GetLastError = %ld\n"), vk, GetLastError());
		system("Pause");
		ExitProcess(2);
	}
}

VOID Trim(CHAR * string, INT length) {

	while (length > 0 && string[length - 1] == '\n')
	{
		string[length - 1] = 0;
		length--;
	}
}


BOOLEAN SimpleRegexMatch(CHAR * target, INT offset, INT length, CHAR * regex, INT rLength, INT * out) {
	// 00:00:10,000 --> 00:00:11,000
	CHAR Regex[] = "dd:dd:dd,ddd";
	INT ri = 0;
	for (int i = offset; i < length; i++)
	{
		if (regex[ri] == 'd') {
			if (target[i] >= '0' && target[i] <= '9') {
				if (ri == 0) out[0] = i;
				ri++;
				if (ri >= rLength) {
					out[1] = i;
					return TRUE;
				}
			}
			else {
				ri = 0;
			}
		}
		else
		{
			if (target[i] == regex[ri]) {
				if (ri == 0) out[0] = i;
				ri++;
				if (ri >= rLength) {
					out[1] = i;
					return TRUE;
				}
			}
			else {
				ri = 0;
			}
		}
	}
	return FALSE;
}


// 将 dd:dd:dd,ddd 转换成 毫秒
INT ConvertHMSF2Int(CHAR * target, INT offset) {
	// "dd:dd:dd,ddd"

	INT h = (target[offset] - '0') * 10 + target[offset + 1] - '0';
	INT m = (target[offset + 3] - '0') * 10 + target[offset + 4] - '0';
	INT s = (target[offset + 6] - '0') * 10 + target[offset + 7] - '0';
	INT f = (target[offset + 9] - '0') * 100 + (target[offset + 10] - '0') * 10 + target[offset + 11] - '0';
	return h * 3600000 + m * 60000 + s * 1000 + f;
}



// 获取 气味 ID ，（1 ~ +00 ）,字符串包含 #!#
INT GetSmellID(CHAR * taregt, INT length) {
	CHAR delimiter[] = "#!#";
	INT out[2] = { 0 };
	INT s = 0;
	if (SimpleRegexMatch(taregt, 0, length, delimiter, strlen(delimiter), out)) {
		for (int i = out[1] + 1; i < length; i++)
		{
			if (taregt[i] >= '0' && taregt[i] <= '9') {
				s = s * 10 + (taregt[i] - '0');
			}
		}
	}
	return s;
}

BOOLEAN GetSrtTimePeriod(CHAR * string, INT length, INT  * StartEnd) {
	CHAR Regex[] = "dd:dd:dd,ddd";
	INT rLength = strlen(Regex);
	INT out[2] = { 0 };
	if (SimpleRegexMatch(string, 0, length, Regex, rLength, out)) {
		INT millSecond = ConvertHMSF2Int(string, out[0]);
		//printf("Match1 %d %d millSecond = %d\n",out[0],out[1], millSecond);
		StartEnd[0] = millSecond;
		if (SimpleRegexMatch(string, out[1] + 1, length, Regex, rLength, out)) {
			millSecond = ConvertHMSF2Int(string, out[0]);
			//printf("Match2 %d %d millSecond = %d\n", out[0], out[1], millSecond);
			StartEnd[1] = millSecond;
			return TRUE;
		}
	}
	return FALSE;
}



BYTE ByteReverse(BYTE byte)
{
	BYTE Temp = byte;
	BYTE TempArray[8] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		if (Temp % 2)
		{
			TempArray[i] = 1;// ●○
		}
		else
		{
			TempArray[i] = 0;
		}
		Temp >>= 1;
	}
	Temp = 0;
	for (int i = 0; i < 8; i++)
	{
		Temp = (Temp << 1) + TempArray[i];
	}
	return Temp;
}
