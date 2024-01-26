#include "JobSystem.hpp"

JobSystem::JobSystem(JobSystemConfig config)
{
	m_config = config;
}

JobSystem::~JobSystem()
{

}

void JobSystem::Startup()
{
	if (static_cast<unsigned int>(m_config.m_numWorkerThreads) > std::thread::hardware_concurrency())
	{
		m_config.m_numWorkerThreads = std::thread::hardware_concurrency();
	}
	m_workerThreads.reserve(m_config.m_numWorkerThreads);
	
	for (int workerIndex = 0; workerIndex < m_config.m_numWorkerThreads; ++workerIndex)
	{
		JobWorker* newWorkerThread = new JobWorker(this, workerIndex);
		m_workerThreads.push_back(newWorkerThread);
	}
}

void JobSystem::BeginFrame()
{

}

void JobSystem::EndFrame()
{

}

void JobSystem::ShutDown()
{
	m_isQuitting = true;

	for (int i = 0; i < (int)m_workerThreads.size(); i++)
	{
		if (m_workerThreads[i])
		{
			delete m_workerThreads[i];
			m_workerThreads[i] = nullptr;
		}
	}
}

Job* JobSystem::RetrieveJobToExecute()
{
	m_jobsToDoListMutex.lock();
	Job* job = nullptr;
	if (!m_jobsToDoList.empty())
	{
		job = m_jobsToDoList.front();
		m_jobsToDoList.pop_front();
	}
	m_jobsToDoListMutex.unlock();
	return job;
}

Job* JobSystem::RetrieveCompletedJobs()
{
	m_completedJobsMutex.lock();
	Job* completedJob = nullptr;
	if (!m_completedJobs.empty())
	{
		completedJob = m_completedJobs.front();
		m_completedJobs.pop_front();
	}
	m_completedJobsMutex.unlock();
	return completedJob;
}

void JobSystem::MoveToCompletedJobs(Job* job)
{
	m_completedJobsMutex.lock();
	m_completedJobs.push_back(job);
	m_completedJobsMutex.unlock();
}

void JobSystem::MoveToExecutingJobs(Job* job)
{
	m_retrievedJobsMutex.lock();
	for (int jobIndex = 0; jobIndex < (int)m_retrievedJobs.size(); jobIndex++)
	{
		if (m_retrievedJobs[jobIndex] == nullptr)
		{
			m_retrievedJobs[jobIndex] = job;
			m_retrievedJobsMutex.unlock();
			return;
		}
	}
	m_retrievedJobs.push_back(job);
	m_retrievedJobsMutex.unlock();
	return;
}

void JobSystem::RemoveFromExecutingJobs(Job* job)
{
	m_retrievedJobsMutex.lock();
	for (int jobIndex = 0; jobIndex < (int)m_retrievedJobs.size(); jobIndex++)
	{
		if (m_retrievedJobs[jobIndex] == job)
		{
			m_retrievedJobs[jobIndex] = nullptr;
		}
	}
	m_retrievedJobsMutex.unlock();

}

void JobSystem::QueueJob(Job* jobToQueue)
{
	m_jobsToDoListMutex.lock();
	m_jobsToDoList.push_back(jobToQueue);
	m_jobsToDoListMutex.unlock();
}


JobSystemConfig JobSystem::GetConfig()
{
	return m_config;
}

bool JobSystem::IsQuitting() const
{
	return m_isQuitting;
}

