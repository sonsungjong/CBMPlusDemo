#include "pch.h"

#include "EnvironmentModel.h"
#include <mysql.h>
#include "DBFile.h"

/*
wstring 을 utf8의 string으로 변환
*/
std::string DBFile::wstring_to_utf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
}

/*
utf8 문자열(char*)을 wstring으로 변환
*/
std::wstring DBFile::utf8_to_wstring(const char* utf8str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(utf8str);
    return wstr;
}

/*
유니코드에서 UTF-8로 변환
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

    // 쿼리 작성
    std::wstring query = _T("SELECT * FROM anom.user WHERE id=\'")+a_account.user_id + _T("\' AND pw=\'")+a_account.user_pw + _T("\'");

    // 쿼리 전송
    if (mysql_query(connection, wstring_to_utf8(query).c_str()))
    {
        std::cerr << "Error: failed to execute query: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }

    // 결과 받아옴
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
    
    // 쿼리 작성
    std::wstring query = _T("SELECT * FROM anom.user");

    // 쿼리 전송
    if (mysql_query(connection, wstring_to_utf8(query).c_str()))
    {
        std::cerr << "Error: failed to execute query: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }

    // 결과 받아옴
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        std::cerr << "Error: failed to store result: " << mysql_error(connection) << std::endl;
        function_result = -1;
        return function_result;
    }

    std::wstring field;

    // 한줄씩 나눔
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
    // 환경정보 파일(데이터베이스) 를 읽어 모델에 저장한다.
    TCHAR file_path[MAX_PATH] = L".\\data\\환경정보.csv";
    EnvironmentModel* p_object = ap;
    TCHAR one_line_string[128] = { 0, };
    TCHAR* p_pos;
    TCHAR str[32] = { 0, };
    TCHAR delimiters[] = _T(",\n");

    FILE* p_file = NULL;
    if (0 == _tfopen_s(&p_file, file_path, _T("rt")))
    {
        // 첫 줄(제목)을 읽는다
        if (NULL != fgetws(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), p_file)) {
            /* no actions */
        }

        // 파일 끝까지 데이터를 읽는다
        while (NULL != fgetws(one_line_string, sizeof(one_line_string) / sizeof(one_line_string[0]), p_file))
        {
            // 한줄의 시작이 , 또는 \n 또는 NULL로 시작하면 없는 줄이기 때문에 스킵
            if (one_line_string[0] == L',' || one_line_string[0] == L'\n' || one_line_string[0] == NULL) {
                break;
            }

            p_pos = one_line_string;
            // 1. 시리얼 넘버
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetSN(str);

            // 2. 일시
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetTime(str);

            // 3. 온도
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetTemp(_ttof(str));

            // 4. 습도
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetHumi(_ttoi(str));

            // 5. 진동
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetVib(_ttof(str));

            // 6. 위도
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetLat(_ttof(str));

            // 7. 경도
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetLon(_ttof(str));

            // 8. 리더기 분석값
            memset(str, 0, sizeof(str));
            p_pos = GetNextString(p_pos, _T(','), str);             // 쉼표 기준으로 잘라 str에 저장하고 p_pos의 위치를 변경한다
            p_object->SetRel(_ttof(str));

            p_object++;           // 그 다음 라인을 저장할 객체로 이동
        }

        fclose(p_file);
    }



    return func_result;
}

TCHAR* DBFile::GetNextString(TCHAR* ap_src_str, TCHAR a_delimiter, TCHAR* ap_buffer)
{
    // NULL 또는 a_delimiter 가 아니면 버퍼에 저장
    while (*ap_src_str != NULL && *ap_src_str != a_delimiter)
    {
        *ap_buffer++ = *ap_src_str++;
    }

    if (*(ap_buffer - 1) == '\n') { *(ap_buffer - 1) = 0; }
    else { *ap_buffer = 0; }

    if (*ap_src_str == a_delimiter) { ap_src_str++; }

    return ap_src_str;
}



// 파일에서 필요한 데이터를 읽어 변수로 저장한다
void DBFile::ReadSettingFile(TCHAR* ap_pathname)
{
}
