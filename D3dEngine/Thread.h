#pragma once
#include <functional>
struct ThreadParam {
	std::function<void()> func;
	bool* complete;
};
class Thread
{
public:
	Thread(std::function<void()> func);
	~Thread();
	void CloseIfComplete();
	bool GetCompleteState() const { return m_complete; };
private:
	HANDLE m_handle;
	DWORD* m_id;
	ThreadParam m_param;
	bool m_complete;
	bool m_isClosed;
};

