#ifndef _ASYNC_SOCKET_CLIENT_H_
#define _ASYNC_SOCKET_CLIENT_H_
#include <WinSock2.h>
#include "media_protocal.h"
#include <vector>

using namespace std;

struct DataBuffer
{
	int len;//
	char* data;//
};

class AsyncSocketClient
{
public:
	AsyncSocketClient(){}
	AsyncSocketClient(SOCKET s);
	void release();
	
	SOCKET getSocket() {return _socket;}
	//��ȡ��Ϣ�󷵻�TRUE�����򷵻�FALSE ,�ڴ���Ҫ�ֶ��ͷ�(delete [] *buf)
	int recvMSG(char** buf);

	//������Ϣ���ɹ�����TRUE��δ��ȫ���ͷ���FALSE��������-1
	int sendMSG(char* const buf, const int len);

	//����δ������ɵ����ݣ���ȷ���������
	int flushSendData() {return sendData();}

private:
	bool _finish_head;
	bool _finish_body;
	int _num_readed;
	int _num_message;
	PacketHeader* _header;
	char* _recvbuf;//���ջ���
	char* _testbuf;
	
	bool _finish_send;//��ǰ���ķ���״̬
	int _num_send;//��ǰ���ѷ��͵Ĵ�С
	vector<DataBuffer> _sendbuf_vector;//���ͻ���

	SOCKET _socket;

private:
	int readData(char* const buf, const int len);
	bool finishHeader();
	void beginHeader();

	int sendData();
};




#endif