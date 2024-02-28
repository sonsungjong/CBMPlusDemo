#ifndef ENVIRONMENT_MODEL_H_
#define ENVIRONMENT_MODEL_H_

/*
�������� ȯ�������� �����ϰų� �����ͺ��̽����� �����ͼ� ������ �� ����ϴ� ��
*/
class EnvironmentModel
{
private:
	wchar_t serial_number[32];				// ����ź �Ϸù�ȣ
	std::tm tm;									// �ð�
	float temperature;							// �µ�
	int humidity;									// ����
	float vibration;								// ����
	float latitude;									// ����
	float longitude;								// �浵
	float reliability;								// ������ �м� �ŷڼ�

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