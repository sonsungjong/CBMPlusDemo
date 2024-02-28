#pragma once

typedef struct SensorData
{
    wchar_t target_sn[50];
    wchar_t time[50];
    float temp;
    int humi;
    float vib;
    float lat;
    float lon;
} SD;

typedef struct BitData
{
    wchar_t datetime[20];
    wchar_t name[50];
    wchar_t str1[50];
    wchar_t str2[50];
    float float1;
    int int1;
} BD;

typedef struct AnalyzeData
{

} AD;

typedef struct UserAccount
{
	wchar_t user_id[50];
	wchar_t user_pw[50];
} UA;

typedef struct MilitaryDataFirst
{
    wchar_t name[50];
    wchar_t target_sn[50];
    int operation_time;
    int total_operation_time;
    wchar_t broken[50];
} MD1;

typedef struct MilitaryDataSecond
{

} MD2;

typedef struct MilitaryDataThird
{

} MD3;

typedef struct MilitaryDataForth
{

} MD4;