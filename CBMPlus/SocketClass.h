#ifndef SOCKET_CLASS_H_
#define SOCKET_CLASS_H_

class SocketClass
{
protected:
	SOCKET mh_socket;		// ��� ���� �ڵ�
	INT m_is_connected;			// ���� ���� ���� (0:������, 1:���� �õ� ��, 2: ����)

	INT m_connect_msg_id;			// ������ �� ����� ID
	INT m_read_close_msg_id;			// ������ ���� �� ���� ����� �޽���

	UCHAR m_key;
	UCHAR m_net_msg_id;				// �޽��� ���̵�
	USHORT m_net_body_size;			// Body ������ ũ��
	CHAR* mp_net_body_data;			// Body ����
	CWnd* mp_notify_wnd;				// socket �޽����� ���� ������ ��ü

public:
	// ���ȿ� �⺻Ű '\0'
	explicit SocketClass();
	// ���ȿ� �⺻Ű ����
	explicit SocketClass(UCHAR a_key);
	virtual ~SocketClass();

	void InitObject(UCHAR a_key);
	// 0: ������, 1: ����, 2: �⺻��
	INT IsConnect();
	// ������ ��Ŷ ����
	INT SendFrameData(CHAR a_msg_id, const void* ap_data, LONG64 a_size);
	// ������ ���ӽõ�
	void ConnectToServer(const TCHAR* ap_ip_address, INT a_port, CWnd* ap_notify_wnd, INT a_connect_id = 26001, INT a_read_close_id = 26002);
	// ���� ����
	void CloseSocket();
	// ���� ������ ���ſ�
	INT ReceiveData(CHAR* ap_data, INT a_size);
	// ���ȿ� ������� 1:�������, 0:���������
	INT ProcessHeaderInfo();
	// ���� ������ ������ ������ �д� �Լ� (���,�ٵ�)
	INT BeginNetworkProcess();
	// ���� �Ϸ�� FD_READ�缳�� �� �޸� ����
	void EndNetworkProcess();
	// ���� �񵿱� �޽��� ó��
	void MessageProc(UINT msg, WPARAM wParam, LPARAM lParam);
	// ���� ���� �Ϸ� �� ȣ��Ǵ� ������ �Լ�
	virtual void ConnectedProcess();
	// ������ ���� �� ȣ��Ǵ� ������ �Լ�, 1:����, 0:����
	virtual INT ProcessNetMessage();
	// ���� ���� �� ȣ��Ǵ� ������ �Լ�, 0: �������� ����, 1: �����߻����� ����
	virtual void ClosedProcess(INT a_error_flag);
};

#endif