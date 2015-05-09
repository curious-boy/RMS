#ifndef _CLOCK_H_
#define _CLOCK_H_

/*
*��Զ��̲߳���ͬʱ���ʵĶ���,��Ҫ�ж�lock����ֵ
*/
class CLock
{
private:
	typedef void* HANDLE;
	
	volatile unsigned int _status; 
	const HANDLE _event;

	CLock(const CLock& rhs);
	CLock &operator = (const CLock& rhs);
public:
	CLock();
	~CLock();

	int lock();
	int unlock();
};

/*
*���������߳����ڷ��ʶ���ʱ��������ִ�У�ʹ�����£�
if(CTry.have_a_try())
{
	//do something

	CTry.finish();
}
*/
class CTry
{
private:
	volatile unsigned int _status;
public:
	CTry():_status(1){}
	
	int have_a_try();
	void finish();
};


#endif