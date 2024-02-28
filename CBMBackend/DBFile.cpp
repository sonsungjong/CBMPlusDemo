#include "pch.h"

#include "EnvironmentModel.h"
#include <mysql.h>
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



int DBFile::LoginUser(UA a_account)
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
    std::wstring query = _T("SELECT * FROM anom.user WHERE id=\'")+a_account.user_id + _T("\' AND pw=\'")+a_account.user_pw + _T("\'");

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

    return function_result;
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
            if (0 == wcscmp(field.c_str(), a_account.user_id.c_str())) {
                idok = 1;
            }
            if (idok == 1 && 0 == wcscmp(field.c_str(), a_account.user_pw.c_str())) {
                function_result = 1;
                return function_result;
            }
        }
    }

    return function_result;
}

int DBFile::GetEnvironmentData(EnvironmentModel* ap)
{
    int func_result = 0;
    // ȯ������ ����(�����ͺ��̽�) �� �о� �𵨿� �����Ѵ�.
    TCHAR file_path[MAX_PATH] = L".\\data\\ȯ������.csv";
    EnvironmentModel* p_object = ap;
    TCHAR one_line_string[128] = { 0, };
    TCHAR* p_pos;
    TCHAR str[32] = { 0, };
    TCHAR delimiters[] = _T(",\n");

    FILE* p_file = NULL;
    if (0 == _tfopen_s(&p_file, file_path, _T("rt")))
    {
        // ù ��(����)�� �д´�
        if (NULL != fgetws(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), p_file)) {
            /* no actions */
        }

        // ���� ������ �����͸� �д´�
        while (NULL != fgetws(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), p_file))
        {
            // ������ ������ , �Ǵ� \n �Ǵ� NULL�� �����ϸ� ���� ���̱� ������ ��ŵ
            if (one_line_string[0] == L',' || one_line_string[0] == L'\n' || one_line_string[0] == NULL) {
                break;
            }

            p_pos = one_line_string;
            // 1. �ø��� �ѹ�
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetSN(str);

            // 2. �Ͻ�
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetTime(str);

            // 3. �µ�
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetTemp(_ttof(str));

            // 4. ����
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetHumi(_ttoi(str));

            // 5. ����
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetVib(_ttof(str));

            // 6. ����
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetLat(_ttof(str));

            // 7. �浵
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetLon(_ttof(str));

            // 8. ������ �м���
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // ��ǥ �������� �߶� str�� �����ϰ� p_pos�� ��ġ�� �����Ѵ�
            p_object->SetRel(_ttof(str));

            p_object++;           // �� ���� ������ ������ ��ü�� �̵�
        }

        fclose(p_file);
    }



    return func_result;
}

TCHAR* DBFile::GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL �Ǵ� a_delimiter �� �ƴϸ� ���ۿ� ����
    while (*ap_src_str != NULL && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    return ap_src_str;
}



// ���Ͽ��� �ʿ��� �����͸� �о� ������ �����Ѵ�
void DBFile::ReadSettingFile(TCHAR* ap_pathname)
{
}
