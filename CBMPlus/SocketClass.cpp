#include "pch.h"
#include "SocketClass.h"

// 소켓 클래스
#include <WS2tcpip.h>

SocketClass::SocketClass()
{
	InitObject('\0');
}

SocketClass::SocketClass(UCHAR a_key)
{
	InitObject(a_key);
}

SocketClass::~SocketClass()
{
	// 사용중인 소켓 제거
	CloseSocket();
	// 수신용 메모리 해제
	if (mp_net_body_data != NULL) {
		delete[] mp_net_body_data;
	}

	WSACleanup();				// 소켓 라이브러리 중지
}

// 객체를 초기화하는 함수 (내부호출용)
void SocketClass::InitObject(UCHAR a_key)
{
	WSADATA temp;
	if (WSAStartup(0x0202, &temp) != 0) {
		AfxMessageBox(_T("소켓 라이브러리 초기화 실패"));
	}

	mh_socket = INVALID_SOCKET;
	m_is_connected = 0;
	m_connect_msg_id = 26001;
	m_read_close_msg_id = 26002;
	mp_notify_wnd = NULL;

	m_key = a_key;
	mp_net_body_data = NULL;
	m_net_body_size = 0;
}

// 정해진 Packet Frame으로 원하는 데이터를 전송하는 함수
// a_msg_id 로 ap_data주소에 a_size 만큼 값을 보냄
INT SocketClass::SendFrameData(CHAR a_msg_id, const void* ap_data, LONG64 a_size)
{
	INT result = -1;
	// 전송할 데이터 프레임의 메모리 할당 (4:헤더, a_size:바디)
	CHAR* p_data = new CHAR[a_size + 4];
	*p_data = m_key;				// 보안용 헤더
	*(p_data + 1) = a_msg_id;					// 메시지 아이디
	// 실제 데이터 크기
	*(USHORT*)(p_data + 2) = a_size;
	memcpy(p_data + 4, ap_data, a_size);
	// mh_socket으로 데이터를 a_size+4만큼 전송
	result = send(mh_socket, p_data, a_size + 4, 0);
	delete[] p_data;

	return result;
}

void SocketClass::ConnectToServer(const TCHAR* ap_ip_address, INT a_port, CWnd* ap_notify_wnd, INT a_connect_id, INT a_read_close_id)
{
	if (mh_socket == INVALID_SOCKET) {
		// 메시지 아이디 설정
		m_connect_msg_id = a_connect_id;
		m_read_close_msg_id = a_read_close_id;
		// 수신할 윈도우 주소
		mp_notify_wnd = ap_notify_wnd;

		// TCP 소켓생성
		mh_socket = socket(AF_INET, SOCK_STREAM, 0);

		// 포트 설정
		sockaddr_in addr_data = { AF_INET, htons(a_port), };
		//ap_ip_address 아이피 설정
		InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
		// 비동기 서버접속 설정
		// 서버 접속 결과에 따라 ah_notify_wnd에 a_connect_id(26001) 메시지 전송
		WSAAsyncSelect(mh_socket, ap_notify_wnd->m_hWnd, a_connect_id, FD_CONNECT);
		m_is_connected = 1;
		connect(mh_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	}
}

void SocketClass::CloseSocket()
{
	if (mh_socket != INVALID_SOCKET) {
		closesocket(mh_socket);						// 소켓 제거
		mh_socket = INVALID_SOCKET;				// 초기화
		m_is_connected = 0;				// 미접속
	}
}

INT SocketClass::ReceiveData(CHAR* ap_data, INT a_size)
{
	INT read_size = 0;
	INT total_size = 0;
	INT retry_count = 0;
	// a_size만큼 수신될 때까지 반복
	while (total_size < a_size) {
		// 남은 크기 수신 시도
		read_size = recv(mh_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {
			// 수신 실패
			Sleep(10);					// 10ms 대기
			retry_count++;			// 재시도 횟수 증가
			if (retry_count > 300) { break; }					// 재시도가 300회를 넘어가면 중단
		}
		else {
			// 수신 완료
			retry_count = 0;						// 재시도횟수 초기화
			total_size += read_size;				// 수신된 크기만큼 합산
		}
	}
	return total_size;				// 수신된 전체 크기를 리턴
}

// 수신 데이터 중 헤더를 읽어 처리
// 1: 정상, 0: 비정상
INT SocketClass::ProcessHeaderInfo()
{
	UCHAR key;
	INT result = 0;
	// 첫 바이트의 key를 읽음
	recv(mh_socket, (CHAR*)&key, 1, 0);
	// 수신 key가 지정된 m_key와 일치한지 비교
	if (key == m_key) {
		recv(mh_socket, (CHAR*)&m_net_msg_id, 1, 0);
		recv(mh_socket, (CHAR*)&m_net_body_size, 2, 0);
		result = 1;
	}
	return result;				// 일치하면 1, 다르면 0
}

INT SocketClass::BeginNetworkProcess()
{
	// 끊어읽기 및 재시도를 위해 FD_READ를 제외하고 다시 비동기 설정(중복방지)
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);

	INT recv_total_size = 0;
	// 헤더 읽기
	INT state = ProcessHeaderInfo();
	// 정상헤더 및 Body크기가 0보다 크면 Body데이터 수신
	if ((state != 0) && (m_net_body_size > 0)) {
		mp_net_body_data = new CHAR[m_net_body_size];
		// 데이터 수신
		recv_total_size = ReceiveData(mp_net_body_data, m_net_body_size);
		state = (m_net_body_size == recv_total_size);
	}
	return state;					// 수신 결과 리턴
}

// 수신 완료 시 호출되는 함수 (FD_READ 재설정 및 메모리 해제)
void SocketClass::EndNetworkProcess()
{
	// 다시 수신할 수 있도록 FD_READ 재설정
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
}

// 소켓 비동기 메시지 처리 함수
void SocketClass::MessageProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == m_connect_msg_id) {
		if (WSAGETSELECTERROR(lParam)) {
			CloseSocket();
		}
		else {
			m_is_connected = 2;					// 접속 상태로 변경
			// 데이터 수신 또는 접속 해제 에 대해 메시지 비동기 설정
			WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ|FD_CLOSE);
		}
		// 재정의 함수 : 서버 접속 시 호출되는 함수
		ConnectedProcess();
	}
	else if (msg == m_read_close_msg_id) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {				// 데이터 수신
			if (BeginNetworkProcess() != 0) {				// 헤더 읽음
				if (ProcessNetMessage() != 0) {
					// 정상 처리 후 마무리 작업
					EndNetworkProcess();
				}

				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			}
			else {
				// key값 불일치
				CloseSocket();			// 소켓 제거
				ClosedProcess(1);				// 추가작업 (오류로 인한 종료)
			}
		}
		else {				// FD_CLOSE 서버와 연결이 해제됨
			CloseSocket();
			ClosedProcess(0);				// 추가작업 (정상 종료)
		}
	}
}

INT SocketClass::IsConnect()
{
	return m_is_connected == 2;
}

void SocketClass::ConnectedProcess() 
{
	/* no actions */
}

INT SocketClass::ProcessNetMessage()
{
	return 1;
}

void SocketClass::ClosedProcess(INT a_error_flag) 
{
	/* no actions */
}