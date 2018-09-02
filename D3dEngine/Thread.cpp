#include "stdafx.h"
#include "Thread.h"

DWORD WINAPI vertexThread(__in LPVOID lpParameter) {
	auto param = reinterpret_cast<ThreadParam*>(lpParameter);
	param->func();
	*param->complete = true;
	return 0;
}

Thread::Thread(std::function<void()> func):
	m_isClosed(false),
	m_complete(false)
{
	m_id = new DWORD{};
	m_complete = false;
	m_param = ThreadParam{
		func,
		&m_complete
	};
	m_handle = CreateThread(0, 0, vertexThread, &m_param, 0, m_id);
}

Thread::~Thread()
{
	if (!m_isClosed) {
		WaitForSingleObject(m_handle, INFINITE);
		CloseHandle(m_handle);
	}
	delete m_id;
}

void Thread::CloseIfComplete()
{
	DWORD result = WaitForSingleObject(m_handle, 0);
	if (m_complete && result == WAIT_OBJECT_0) {
		CloseHandle(m_handle);
		m_isClosed = true;
	}
}
