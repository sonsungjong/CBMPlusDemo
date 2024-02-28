#include "pch.h"
#define DB_IP                     "127.0.0.1"
#define DB_PORT                     3306
#define SCHEMA_NAME             "anom"

#include "DBFile.h"


/*
wstring �� utf8�� string���� ��ȯ
*/
std::string DBFile::wstring_to_utf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
}

/*
utf8 ���ڿ�(char*)�� wstring���� ��ȯ
*/
std::wstring DBFile::utf8_to_wstring(const char* utf8str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(utf8str);
    return wstr;
}

/*
�����ڵ忡�� UTF-8�� ��ȯ
*/
void DBFile::W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

int DBFile::SelectAllUser(UA a_account)
{
    int function_result = 0;

    // MariaDB Init Connection
    MYSQL* connection = mysql_init(NULL);
    if (!mysql_real_connect(connection, DB_IP, "root", "root", SCHEMA_NAME, DB_PORT, NULL, 0)) {
        std::cerr << "Error: failed to connect to the database: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }
    
    // ���� �ۼ�
    std::wstring query = _T("SELECT * FROM anom.user");

    // ���� ����
    if (mysql_query(connection, wstring_to_utf8(query).c_str()))
    {
        std::cerr << "Error: failed to execute query: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }

    // ��� �޾ƿ�
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        std::cerr << "Error: failed to store result: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }

    std::wstring field;

    // ���پ� ����
    MYSQL_ROW row;
    char idok = 0;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < mysql_num_fields(result); i++) {
            field = utf8_to_wstring(row[i]);
            if (0 == wcscmp(field.c_str(), a_account.user_id)) {
                idok = 1;
            }
            if (idok == 1 && 0 == wcscmp(field.c_str(), a_account.user_pw)) {
                function_result = 1;
                return function_result;
            }
        }
    }

    return function_result;
}

// ���Ͽ��� �ʿ��� �����͸� �о� ������ �����Ѵ�
void DBFile::ReadSettingFile(TCHAR* ap_pathname)
{
}
