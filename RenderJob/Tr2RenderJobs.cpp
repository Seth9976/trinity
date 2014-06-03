#include "StdAfx.h"
#include "Tr2RenderJobs.h"
#include "TriRenderJob.h"
#include "Tr2PushPopDS.h"
#include "Tr2PushPopRT.h"

// Leave the 'device' in the name for compatibility
CCP_STATS_DECLARE( deviceRenderJobs, "Trinity/device/RenderJobs", true, CST_TIME, "Time taken for render job processing" );
CCP_STATS_DECLARE( deviceChainedRenderJobsCount, "Trinity/device/ChainedRenderJobsCount", true, CST_COUNTER_LOW, "Count of scheduled chained render jobs" );
CCP_STATS_DECLARE( deviceOnceRenderJobsCount, "Trinity/device/OnceRenderJobsCount", true, CST_COUNTER_LOW, "Count of scheduled once render jobs" );

Tr2RenderJobs::Tr2RenderJobs( IRoot* lockobj )
	: PARENTLOCK( m_scheduledOnce )
	, PARENTLOCK( m_scheduledRecurring )
	, PARENTLOCK( m_scheduledChained )
	, PARENTLOCK( m_updateRecurring )
	, m_stop( false )
{	
}

Tr2RenderJobs::~Tr2RenderJobs()
{		
#ifdef _WIN32
	if( !m_threads.empty() )
	{
		m_stop = true;

		KickAllThreads();
		WaitForAllThreads();
		ClearThreads();
	}
#endif
}

#ifdef _WIN32
void Tr2RenderJobs::KickAllThreads()
{
	for( size_t i = 0; i != m_threads.size(); ++i )
	{
		::SetEvent( m_threads[i].m_kickThreadEvent );
	}
}


void Tr2RenderJobs::WaitForAllThreads()
{
	//WaitForMultipleObjects( m_threads.size(), ... something , TRUE, INFINITE );

	for( size_t i = 0; i != m_threads.size(); ++i )
	{
		WaitForSingleObject( m_threads[i].m_listReadyEvent, INFINITE );
	}
}

uint32_t Tr2RenderJobs::RunJobThreadThunk( void* userData )
{
	TRenderThread *This = static_cast<TRenderThread*>( userData );
	while( !This->m_owner->m_stop )
	{
		::WaitForSingleObject( This->m_kickThreadEvent, INFINITE );

		if( !This->m_owner->m_stop )
		{
			This->m_owner->RunJobs( This->m_threadId );
		}

		::SetEvent( This->m_listReadyEvent );
    }
	return 0;
}

void Tr2RenderJobs::ClearThreads()
{
	for( size_t i = 0; i != m_threads.size(); ++i )
	{
		if( m_threads[i].m_handle )
		{
			::CloseHandle( m_threads[i].m_handle );
			::CloseHandle( m_threads[i].m_kickThreadEvent );
			::CloseHandle( m_threads[i].m_listReadyEvent );
		}
	}
	m_threads.clear();
}

namespace {

// magic!
// http://msdn.microsoft.com/en-us/library/xcb2z8hs%28v=vs.100%29.aspx
const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName( DWORD dwThreadID, char* threadName)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}

}

void Tr2RenderJobs::SetThreadPoolSize( uint32_t	poolSize )
{
	m_threads.resize( poolSize );
	for( uint32_t i = 0; i != poolSize; ++i )
	{
		TRenderThread &thread = m_threads[i];
		
		thread.m_owner = this;
		thread.m_threadId = i+1;

		thread.m_renderContext.CreateInstance();
		
		if( FAILED( thread.m_renderContext->CreateSecondaryContext() ) )
		{
			ClearThreads();
			return;
		}

		uint32_t threadId;
		thread.m_handle = HANDLE( _beginthreadex( 
			nullptr,
			0,
			RunJobThreadThunk,
			&m_threads[i],
			CREATE_SUSPENDED,
			&threadId ) );

		thread.m_kickThreadEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL );
		thread.m_listReadyEvent  = ::CreateEvent( NULL, FALSE, FALSE, NULL );

		char threadName[64];
		_snprintf_s( threadName, 64, "Tr2RenderJobs Worker %02d", i+1 );
		SetThreadName( threadId, threadName );

		::ResumeThread( thread.m_handle );
	}
}

void Tr2RenderJobs::RunJobs( uint32_t affinity )
{
	if( affinity )
	{
		m_threads[affinity-1].m_renderContext->BeginScene();
	}

	for( auto it = m_copyOfJobs.cbegin(); it != m_copyOfJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		if( m_threads.empty() ||
			( rj->GetThreadAffinity() % (m_threads.size()+1) ) == affinity )
		{
			TriRenderJobStatus status = 
								rj->Run(	m_realTime,
											m_simTime,
											affinity	? m_threads[affinity-1].m_renderContext
														: &static_cast<Tr2RenderContext&>( Tr2RenderContext_GetMainThreadRenderContext() )
								);

			CCP_ASSERT( status != RJ_FAILED );
		}
	}
	
	if( affinity )
	{
		m_threads[affinity-1].m_renderContext->EndScene();
		m_threads[affinity-1].m_renderContext->FinishCommandList();					
	}
}

#endif

void Tr2RenderJobs::Run( Be::Time realTime, Be::Time simTime )
{
	CCP_STATS_SCOPED_TIME( deviceRenderJobs );
	CCP_STATS_SET( deviceChainedRenderJobsCount, m_scheduledChained.size() );
	CCP_STATS_SET( deviceOnceRenderJobsCount, m_scheduledOnce.size() );

	D3DPERF_EVENT(L"RenderJobs");

	USE_MAIN_THREAD_RENDER_CONTEXT();

	Tr2PushPopRT pushPopRT( renderContext );
	Tr2PushPopDS pushPopDS( renderContext );

	m_copyOfJobs.insert( m_copyOfJobs.end(), m_scheduledRecurring.begin(), m_scheduledRecurring.end() );	
	ON_BLOCK_EXIT( [&]{ m_copyOfJobs.clear(); } );

	m_realTime = realTime;
	m_simTime = simTime;
	
#ifdef _WIN32
	KickAllThreads();
	RunJobs( 0 );
	WaitForAllThreads();
	for( size_t i = 0; i != m_threads.size(); ++i )
	{		
		m_threads[i].m_renderContext->ExecuteCommandList();
	}
#else
	for( auto it = m_copyOfJobs.cbegin(); it != m_copyOfJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		TriRenderJobStatus status = rj->Run( realTime, simTime );
        CCP_ASSERT( status != RJ_FAILED );
	}
#endif
    
	// Process jobs scheduled for one-off execution. Every job on this list is run,
	// jobs that are still in progress are continued next frame.
	m_copyOfJobs.clear();
	m_copyOfJobs.insert( m_copyOfJobs.end(), m_scheduledOnce.begin(), m_scheduledOnce.end() );	
		
	CTriRenderJobVector continuedJobs;
	for( auto it = m_copyOfJobs.cbegin(); it != m_copyOfJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		TriRenderJobStatus status = rj->Run( realTime, simTime );
		if( status == RJ_IN_PROGRESS )
		{
			continuedJobs.Insert( -1, rj );
		}
	}
	m_scheduledOnce.Remove( -1 );

	for( auto it = continuedJobs.cbegin(); it != continuedJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		m_scheduledOnce.Insert( -1, rj );
	}
	continuedJobs.Remove( -1 );

	// Process jobs scheduled for chained one-off execution. Jobs on this list are run
	// until a job is found still in progress. That job and the remaining jobs are then
	// continued on the next frame.
	m_copyOfJobs.clear();
	m_copyOfJobs.insert( m_copyOfJobs.end(), m_scheduledChained.begin(), m_scheduledChained.end() );	
	for( auto it = m_copyOfJobs.cbegin(); it != m_copyOfJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		TriRenderJobStatus status = rj->Run( realTime, simTime );
		if( status == RJ_IN_PROGRESS )
		{
			for( ; it != m_copyOfJobs.cend(); ++it )
			{
				rj = *it;
				continuedJobs.Insert( -1, rj );
			}
			break;
		}
	}
	m_scheduledChained.Remove( -1 );

	for( auto it = continuedJobs.cbegin(); it != continuedJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		m_scheduledChained.Insert( -1, rj );
	}
	continuedJobs.Remove( -1 );
}

void Tr2RenderJobs::RunUpdate( Be::Time realTime, Be::Time simTime )
{
	m_copyOfJobs.insert( m_copyOfJobs.end(), m_updateRecurring.begin(), m_updateRecurring.end() );	
	ON_BLOCK_EXIT( [&]{ m_copyOfJobs.clear(); } );
	for( auto it = m_copyOfJobs.cbegin(); it != m_copyOfJobs.cend(); ++it )
	{
		TriRenderJob* rj = *it;
		TriRenderJobStatus status = rj->Run( realTime, simTime );
        CCP_ASSERT( status != RJ_FAILED );
	}
}