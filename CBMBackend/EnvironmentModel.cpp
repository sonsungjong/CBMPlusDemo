#include "pch.h"
#include "EnvironmentModel.h"

EnvironmentModel::EnvironmentModel()
{
	// 우선 모두 0으로 초기화 (프로그램 시작시 최대수치로 동적할당을 한다)
	memset(serial_number, 0, sizeof(serial_number));
	memset(&tm, 0, sizeof(tm));
	temperature = 0.0f;
	humidity = 0;
	vibration = 0.0f;
	latitude = 0.0f;
	longitude = 0.0f;
	reliability = 0.0f;
}

EnvironmentModel::~EnvironmentModel()
{
}

// 모든 값을 셋팅 (정렬 필요)
void EnvironmentModel::SetData(const wchar_t* _serial_number, const wchar_t* _wstr_time, float _temperature, int _humidity, float _vibration, float _latitude, float _longitude, float _reliability)
{
	SetSN(_serial_number);
	SetTime(_wstr_time);
	SetTemp(_temperature);
	SetHumi(_humidity);
	SetVib(_vibration);
	SetLat(_latitude);
	SetLon(_longitude);
	SetRel(_reliability);
}

const wchar_t* EnvironmentModel::GetSN() const
{
	return serial_number;
}

void EnvironmentModel::SetSN(const wchar_t* a_serial_number)
{
	const size_t buffer_size = sizeof(serial_number) / sizeof(wchar_t);
	const size_t length = wcslen(a_serial_number);
	if (length < buffer_size) {
		wcscpy_s(serial_number, buffer_size, a_serial_number);
	}
	else {
		/* 시리얼 번호가 배열크기를 초과했을 때*/
	}
}

// 시간을 문자열 L"yyyy-MM-dd hh-mm-ss"로 반환
const std::wstring EnvironmentModel::GetTimeStr() const
{
	wchar_t buffer[20];
	std::wcsftime(buffer, sizeof(buffer) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &tm);
	return std::wstring(buffer);
}

const std::tm& EnvironmentModel::GetTime() const
{
	return tm;
}

// 문자열 L"y-M-d h-m-s" 를 받아 시간으로 저장
void EnvironmentModel::SetTime(const wchar_t* a_wstr_time)
{
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	int fields = swscanf_s(a_wstr_time, L"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

	if (fields < 6) {
		second = 0;
	}

	if (fields < 5) {
		minute = 0;
	}

	if (fields < 4) {
		hour = 0;
	}

	tm.tm_year = year >= 100 ? year - 1900 : year + 100;			// 2자리는 4자리로 변환
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;
}

float EnvironmentModel::GetTemp() const
{
	return temperature;
}

void EnvironmentModel::SetTemp(float a_temperature)
{
	temperature = a_temperature;
}

int EnvironmentModel::GetHumi() const
{
	return humidity;
}

void EnvironmentModel::SetHumi(int a_humidity)
{
	humidity = a_humidity;
}

float EnvironmentModel::GetVib() const
{
	return vibration;
}

void EnvironmentModel::SetVib(float a_vibration)
{
	vibration = a_vibration;
}

float EnvironmentModel::GetLat() const
{
	return latitude;
}

void EnvironmentModel::SetLat(float a_latitude)
{
	latitude = a_latitude;
}

float EnvironmentModel::GetLon() const
{
	return longitude;
}

void EnvironmentModel::SetLon(float a_longitude)
{
	longitude = a_longitude;
}

float EnvironmentModel::GetRel() const
{
	return reliability;
}

void EnvironmentModel::SetRel(float a_reliability)
{
	reliability = a_reliability;
}
