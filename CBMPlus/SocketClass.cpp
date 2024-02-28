#include "pch.h"
#include "SocketClass.h"

// ���� Ŭ����
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
	// ������� ���� ����
	CloseSocket();
	// ���ſ� �޸� ����
	if (mp_net_body_data != NULL) {
		delete[] mp_net_body_data;
	}

	WSACleanup();				// ���� ���̺귯�� ����
}

// ��ü�� �ʱ�ȭ�ϴ� �Լ� (����ȣ���)
void SocketClass::InitObject(UCHAR a_key)
{
	WSADATA temp;
	if (WSAStartup(0x0202, &temp) != 0) {
		AfxMessageBox(_T("���� ���̺귯�� �ʱ�ȭ ����"));
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

// ������ Packet Frame���� ���ϴ� �����͸� �����ϴ� �Լ�
// a_msg_id �� ap_data�ּҿ� a_size ��ŭ ���� ����
INT SocketClass::SendFrameData(CHAR a_msg_id, const void* ap_data, LONG64 a_size)
{
	INT result = -1;
	// ������ ������ �������� �޸� �Ҵ� (4:���, a_size:�ٵ�)
	CHAR* p_data = new CHAR[a_size + 4];
	*p_data = m_key;				// ���ȿ� ���
	*(p_data + 1) = a_msg_id;					// �޽��� ���̵�
	// ���� ������ ũ��
	*(USHORT*)(p_data + 2) = a_size;
	memcpy(p_data + 4, ap_data, a_size);
	// mh_socket���� �����͸� a_size+4��ŭ ����
	result = send(mh_socket, p_data, a_size + 4, 0);
	delete[] p_data;

	return result;
}

void SocketClass::ConnectToServer(const TCHAR* ap_ip_address, INT a_port, CWnd* ap_notify_wnd, INT a_connect_id, INT a_read_close_id)
{
	if (mh_socket == INVALID_SOCKET) {
		// �޽��� ���̵� ����
		m_connect_msg_id = a_connect_id;
		m_read_close_msg_id = a_read_close_id;
		// ������ ������ �ּ�
		mp_notify_wnd = ap_notify_wnd;

		// TCP ���ϻ���
		mh_socket = socket(AF_INET, SOCK_STREAM, 0);

		// ��Ʈ ����
		sockaddr_in addr_data = { AF_INET, htons(a_port), };
		//ap_ip_address ������ ����
		InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
		// �񵿱� �������� ����
		// ���� ���� ����� ���� ah_notify_wnd�� a_connect_id(26001) �޽��� ����
		WSAAsyncSelect(mh_socket, ap_notify_wnd->m_hWnd, a_connect_id, FD_CONNECT);
		m_is_connected = 1;
		connect(mh_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	}
}

void SocketClass::CloseSocket()
{
	if (mh_socket != INVALID_SOCKET) {
		closesocket(mh_socket);						// ���� ����
		mh_socket = INVALID_SOCKET;				// �ʱ�ȭ
		m_is_connected = 0;				// ������
	}
}

INT SocketClass::ReceiveData(CHAR* ap_data, INT a_size)
{
	INT read_size = 0;
	INT total_size = 0;
	INT retry_count = 0;
	// a_size��ŭ ���ŵ� ������ �ݺ�
	while (total_size < a_size) {
		// ���� ũ�� ���� �õ�
		read_size = recv(mh_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {
			// ���� ����
			Sleep(10);					// 10ms ���
			retry_count++;			// ��õ� Ƚ�� ����
			if (retry_count > 300) { break; }					// ��õ��� 300ȸ�� �Ѿ�� �ߴ�
		}
		else {
			// ���� �Ϸ�
			retry_count = 0;						// ��õ�Ƚ�� �ʱ�ȭ
			total_size += read_size;				// ���ŵ� ũ�⸸ŭ �ջ�
		}
	}
	return total_size;				// ���ŵ� ��ü ũ�⸦ ����
}

// ���� ������ �� ����� �о� ó��
// 1: ����, 0: ������
INT SocketClass::ProcessHeaderInfo()
{
	UCHAR key;
	INT result = 0;
	// ù ����Ʈ�� key�� ����
	recv(mh_socket, (CHAR*)&key, 1, 0);
	// ���� key�� ������ m_key�� ��ġ���� ��
	if (key == m_key) {
		recv(mh_socket, (CHAR*)&m_net_msg_id, 1, 0);
		recv(mh_socket, (CHAR*)&m_net_body_size, 2, 0);
		result = 1;
	}
	return result;				// ��ġ�ϸ� 1, �ٸ��� 0
}

INT SocketClass::BeginNetworkProcess()
{
	// �����б� �� ��õ��� ���� FD_READ�� �����ϰ� �ٽ� �񵿱� ����(�ߺ�����)
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);

	INT recv_total_size = 0;
	// ��� �б�
	INT state = ProcessHeaderInfo();
	// ������� �� Bodyũ�Ⱑ 0���� ũ�� Body������ ����
	if ((state != 0) && (m_net_body_size > 0)) {
		mp_net_body_data = new CHAR[m_net_body_size];
		// ������ ����
		recv_total_size = ReceiveData(mp_net_body_data, m_net_body_size);
		state = (m_net_body_size == recv_total_size);
	}
	return state;					// ���� ��� ����
}

// ���� �Ϸ� �� ȣ��Ǵ� �Լ� (FD_READ �缳�� �� �޸� ����)
void SocketClass::EndNetworkProcess()
{
	// �ٽ� ������ �� �ֵ��� FD_READ �缳��
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
}

// ���� �񵿱� �޽��� ó�� �Լ�
void SocketClass::MessageProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == m_connect_msg_id) {
		if (WSAGETSELECTERROR(lParam)) {
			CloseSocket();
		}
		else {
			m_is_connected = 2;					// ���� ���·� ����
			// ������ ���� �Ǵ� ���� ���� �� ���� �޽��� �񵿱� ����
			WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ|FD_CLOSE);
		}
		// ������ �Լ� : ���� ���� �� ȣ��Ǵ� �Լ�
		ConnectedProcess();
	}
	else if (msg == m_read_close_msg_id) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {				// ������ ����
			if (BeginNetworkProcess() != 0) {				// ��� ����
				if (ProcessNetMessage() != 0) {
					// ���� ó�� �� ������ �۾�
					EndNetworkProcess();
				}

				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			}
			else {
				// key�� ����ġ
				CloseSocket();			// ���� ����
				ClosedProcess(1);				// �߰��۾� (������ ���� ����)
			}
		}
		else {				// FD_CLOSE ������ ������ ������
			CloseSocket();
			ClosedProcess(0);				// �߰��۾� (���� ����)
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