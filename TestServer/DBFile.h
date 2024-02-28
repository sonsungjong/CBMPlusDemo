#pragma once
#include "global_struct.h"

// ���ۻ���� �� ���� �б⸦ �Ͻ������ϰ� DB�� �Է�
// ���̺� �°� �ڵ带 �����ؾ���
// MariaDB ������ MariaDB Connector C 64bit�� ���

/*
<Include Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\include

<Libraries Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\lib

<Additional Dependencies>
libmariadb.lib
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <tchar.h>
#include <mysql.h>

class DBFile
{
private:
	wchar_t* mp_stock_data;

public:
	// ���ڿ� ��ȯ
	std::string wstring_to_utf8(const std::wstring& wstr);
	std::wstring utf8_to_wstring(const char* utf8str);
	void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char);

	// DB ��Ʈ��
	void InsertDB();
	void SelectDB();
	void UpdateDB();
	void DeleteDB();

	int SelectAllUser(UA account);

	// �α�
	void RecordLog(TCHAR* ap_pathname);
	
	// ���� �б�, ����, ����, (���� �����)
	void ReadCsvComma(TCHAR* ap_pathname);
	void ReadCsvQuote(TCHAR* ap_pathname);
	void ReadXlsx(TCHAR* ap_pathname);
	void CreateCsv(TCHAR* ap_pathname);
	void CreateXlsx(TCHAR* ap_pathname);
	void DeleteTargetFile(TCHAR* ap_pathname);
	void ReadIni(TCHAR* ap_pathname);
	void ReadSettingFile(TCHAR* ap_pathname);

private:

};

