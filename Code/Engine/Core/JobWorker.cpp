#include "JobSystem.hpp"
#include "JobWorker.hpp"

JobWorker::JobWorker(JobSystem* owner, int threadID)
	:m_jobSyetem(owner)
	,m_threadID(threadID)
{
	m_thread = new std::thread(&JobWorker::ThreadMain, this, this);
}

JobWorker::~JobWorker()
{
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

void JobWorker::ThreadMain(JobWorker* worker)
{
	while (!worker->m_jobSyetem->IsQuitting())
	{
		Job* jobToExecute = m_jobSyetem->RetrieveJobToExecute();
		if (jobToExecute != nullptr)
		{
			m_jobSyetem->MoveToExecutingJobs(jobToExecute);
			jobToExecute->Execute();
			m_jobSyetem->RemoveFromExecutingJobs(jobToExecute);
			m_jobSyetem->MoveToCompletedJobs(jobToExecute);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}

