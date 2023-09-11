#include <pch.h>
#include <Resources/ThreadPool.h>


namespace Resources
{
	ThreadPool::ThreadPool()
	{
		m_threadEnabled = true;
		m_maxThread = std::thread::hardware_concurrency() / 2;
		m_maxThreadAssimp = 2;
	}

	ThreadPool::~ThreadPool()
	{
		m_threadEnabled = false;

		for (int i = 0; i < m_workers.size(); ++i)
			m_workers[i].join();

		for (int i = 0; i < m_workersAssimp.size(); ++i)
			m_workersAssimp[i].join();
	}

	void ThreadPool::EnableThreads()
	{
		for (int i = 0; i < m_maxThread; ++i)
			m_workers.push_back(std::thread(&ThreadPool::MultiThread, this));

		for (int i = 0; i < m_maxThreadAssimp; ++i)
			m_workersAssimp.push_back(std::thread(&ThreadPool::MultiThreadAssimp, this));
	}

	void ThreadPool::MultiThread()
	{
		while (m_threadEnabled)
		{
			while (m_signal.test_and_set() && m_threadEnabled)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (m_tasks.size() != 0)
			{
				Task task = m_tasks.front();
				m_tasks.pop();
				m_signal.clear();
				task.func(task.filePath);
			}
			else
			{
				m_signal.clear();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}

	void ThreadPool::MultiThreadAssimp()
	{
		while (m_threadEnabled)
		{
			while (m_signalAssimp.test_and_set() && m_threadEnabled)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (m_tasksAssimp.size() != 0)
			{
				Task task = m_tasksAssimp.front();
				m_tasksAssimp.pop();
				m_signalAssimp.clear();
				task.func(task.filePath);
			}
			else
			{
				m_signalAssimp.clear();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}

	void ThreadPool::AddTasks(Task task)
	{
		while (m_signal.test_and_set()) { /*Wait*/ }

		m_tasks.push(task);

		m_signal.clear();
	}

	void ThreadPool::AddTasksAssimp(Task task)
	{
		while (m_signalAssimp.test_and_set()) { /*Wait*/ }

		m_tasksAssimp.push(task);

		m_signalAssimp.clear();
	}

	void ThreadPool::DeleteTasks()
	{
		while (m_signal.test_and_set()) { /*Wait*/ }

		while (!m_tasks.empty())
			m_tasks.pop();

		m_signal.clear();
	}
}