#ifndef ENVIRONMENT_MODEL_H_
#define ENVIRONMENT_MODEL_H_

/*
리더기의 환경정보를 수신하거나 데이터베이스에서 가져와서 저장할 때 사용하는 모델
*/
class EnvironmentModel
{
private:
	wchar_t serial_number[32];				// 유도탄 일련번호
	std::tm tm;									// 시간
	float temperature;							// 온도
	int humidity;									// 습도
	float vibration;								// 진동
	float latitude;									// 위도
	float longitude;								// 경도
	float reliability;								// 리더기 분석 신뢰성

public:
	EnvironmentModel();
	~EnvironmentModel();
	void SetData(const wchar_t* _serial_number, const wchar_t* _wstr_time, float _temperature, int _humidity, float _vibration = 0.0f, float _latitude = 0.0f, float _longitude = 0.0f, float _reliability = 0.0f);

	const wchar_t* GetSN() const;
	void SetSN(const wchar_t* a_serial_number);

	const std::wstring GetTimeStr() const;
	const std::tm& GetTime() const;
	void SetTime(const wchar_t* a_wstr_time);				// L"y-M-d h:m:s"

	float GetTemp() const;
	void SetTemp(float a_temperature);

	int GetHumi() const;
	void SetHumi(int a_humidity);

	float GetVib() const;
	void SetVib(float a_vibration);

	float GetLat() const;
	void SetLat(float a_latitude);

	float GetLon() const;
	void SetLon(float a_longitude);

	float GetRel() const;
	void SetRel(float a_reliability);
};

#endif	ENVIRONMENT_MODEL_H_