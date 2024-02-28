// 버퍼사이즈가 꽉 차면 읽기를 일시정지하고 DB에 입력
// 테이블에 맞게 코드를 수정해야함
// MariaDB 서버와 MariaDB Connector C 64bit를 사용

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
	// 문자열 변환
	std::string wstring_to_utf8(const std::wstring& wstr);
	std::wstring utf8_to_wstring(const char* utf8str);
	void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char);

	// DB 컨트롤
	void InsertDB();
	void SelectDB();
	void UpdateDB();
	void DeleteDB();

	int LoginUser(UA a_account);
	int SelectAllUser(UA a_account);

	int GetEnvironmentData(EnvironmentModel* ap);
	TCHAR* GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer);

	// 로그
	void RecordLog(TCHAR* ap_pathname);
	
	// 파일 읽기, 쓰기, 삭제, (폴더 만들기)
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