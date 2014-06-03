#pragma once

#ifndef Tr2RenderJobs_h_
#define Tr2RenderJobs_h_


#include "Tr2DeviceResource.h"

#if 0
#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#endif

BLUE_DECLARE( TriRenderJob );
BLUE_DECLARE_VECTOR( TriRenderJob );

BLUE_CLASS( Tr2RenderJobs ) : public IRoot
{
public:
	EXPOSE_TO_BLUE();

	Tr2RenderJobs( IRoot* lockobj = 0 );
	~Tr2RenderJobs();
	
	void Run( Be::Time realTime, Be::Time simTime );
	void RunUpdate( Be::Time realTime, Be::Time simTime );

#ifdef _WIN32
	void		SetThreadPoolSize( uint32_t	poolSize );
	uint32_t	GetThreadPoolSize() { return (uint32_t)m_threads.size(); }
#endif
	
	PTriRenderJobVector m_scheduledRecurring;
	PTriRenderJobVector m_scheduledOnce;
	PTriRenderJobVector m_scheduledChained;
	PTriRenderJobVector m_updateRecurring;

private:
	// Used to run the jobs:
	typedef std::vector<TriRenderJobPtr>	RenderJobVector;
	RenderJobVector m_copyOfJobs;  // we need to copy jobs because lists can change when executed
	Be::Time		m_simTime;
	Be::Time		m_realTime;
	bool			m_stop;

	void	RunJobs( uint32_t affinity );

#ifdef _WIN32
	struct TRenderThread
	{
		TRenderThread()
		: m_handle( 0 ),
		m_kickThreadEvent( 0 ),
		m_listReadyEvent( 0 )
		{}
		Tr2RenderContextPtr		m_renderContext;		
		Tr2RenderJobsPtr		m_owner;
		uint32_t				m_threadId;

		HANDLE					m_handle;
		HANDLE					m_kickThreadEvent;
		HANDLE					m_listReadyEvent;
	};
	std::vector<TRenderThread>	m_threads;

	void	ClearThreads();
	void	KickAllThreads();
	void	WaitForAllThreads();

	static uint32_t __stdcall RunJobThreadThunk( void* userData );
#endif
};

TYPEDEF_BLUECLASS( Tr2RenderJobs )
BLUE_DECLARE_VECTOR( Tr2RenderJobs )

#endif	// Tr2RenderJobs_h_
