#ifndef SOCKET_CLASS_H_
#define SOCKET_CLASS_H_

class SocketClass
{
protected:
	SOCKET mh_socket;		// 통신 소켓 핸들
	INT m_is_connected;			// 서버 접속 상태 (0:미접속, 1:접속 시도 중, 2: 접속)

	INT m_connect_msg_id;			// 접속할 때 사용할 ID
	INT m_read_close_msg_id;			// 데이터 수신 및 접속 종료시 메시지

	UCHAR m_key;
	UCHAR m_net_msg_id;				// 메시지 아이디
	USHORT m_net_body_size;			// Body 데이터 크기
	CHAR* mp_net_body_data;			// Body 정보
	CWnd* mp_notify_wnd;				// socket 메시지를 받을 윈도우 객체

public:
	// 보안용 기본키 '\0'
	explicit SocketClass();
	// 보안용 기본키 변경
	explicit SocketClass(UCHAR a_key);
	virtual ~SocketClass();

	void InitObject(UCHAR a_key);
	// 0: 미접속, 1: 접속, 2: 기본값
	INT IsConnect();
	// 서버로 패킷 전송
	INT SendFrameData(CHAR a_msg_id, const void* ap_data, LONG64 a_size);
	// 서버에 접속시도
	void ConnectToServer(const TCHAR* ap_ip_address, INT a_port, CWnd* ap_notify_wnd, INT a_connect_id = 26001, INT a_read_close_id = 26002);
	// 소켓 제거
	void CloseSocket();
	// 서버 데이터 수신용
	INT ReceiveData(CHAR* ap_data, INT a_size);
	// 보안용 헤더인포 1:정상헤더, 0:비정상헤더
	INT ProcessHeaderInfo();
	// 수신 데이터 프레임 단위로 읽는 함수 (헤더,바디)
	INT BeginNetworkProcess();
	// 수신 완료시 FD_READ재설정 및 메모리 해제
	void EndNetworkProcess();
	// 소켓 비동기 메시지 처리
	void MessageProc(UINT msg, WPARAM wParam, LPARAM lParam);
	// 서버 접속 완료 시 호출되는 재정의 함수
	virtual void ConnectedProcess();
	// 데이터 수신 후 호출되는 재정의 함수, 1:성공, 0:실패
	virtual INT ProcessNetMessage();
	// 접속 해제 시 호출되는 재정의 함수, 0: 서버에서 해제, 1: 오류발생으로 해제
	virtual void ClosedProcess(INT a_error_flag);
};

#endif