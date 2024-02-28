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

#ifndef DB_FILE_H_
#define DB_FILE_H_

#define DB_IP                     "127.0.0.1"
#define DB_PORT                     3307
#define SCHEMA_NAME             "anom"

typedef struct UserAccount
{
	std::wstring user_id;
	std::wstring user_pw;
} UA;

class DBFile
{
private:
	

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

	int LoginUser(UA a_account);
	int SelectAllUser(UA a_account);

	int GetEnvironmentData(EnvironmentModel* ap);
	TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer);

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

#endif	DB_FILE_H_