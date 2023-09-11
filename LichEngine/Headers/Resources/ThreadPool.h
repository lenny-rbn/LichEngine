#pragma once
#undef min
#undef max
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>

#include <Define.h>
#include <Resources/WrapperAssimp.h>


struct Task
{
public:
	Task() {};

	std::string filePath;
	std::function<void(std::string)> func;

	Task(std::function<void(std::string)> _func, std::string _filePath)
	{
		func = _func;
		filePath = _filePath;
	}
};

namespace Resources
{
	class LICH_ENGINE ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		void MultiThread();
		void MultiThreadAssimp();
		void EnableThreads();
		void AddTasks(Task task);
		void AddTasksAssimp(Task task);

		void DeleteTasks();
		bool IsAllTaskDone() { return (m_tasks.size() == 0 && m_tasksAssimp.size() == 0); }

	private:
		bool m_threadEnabled;

		int m_maxThread;
		int m_maxThreadAssimp;

		std::queue<Task> m_tasks;
		std::queue<Task> m_tasksAssimp;

		std::vector<std::thread> m_workers;
		std::vector<std::thread> m_workersAssimp;

		std::atomic_flag m_signal;
		std::atomic_flag m_signalAssimp;
	};
}