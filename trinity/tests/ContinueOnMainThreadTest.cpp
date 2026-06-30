#include "StdAfx.h"
#include "gtest/gtest.h"

#include "ContinueOnMainThread.h"

TEST( ContinueOnMainThread, Recursive )
{
	int firstRuns = 0;
	int secondRuns = 0;
	bool reentered = false;

	ContinueOnMainThread( [&] {
		firstRuns++;
		if( !reentered )
		{
			reentered = true;
			ExecuteMainThreadActions();
		}
	} );
	ContinueOnMainThread( [&] { secondRuns++; } );

	ExecuteMainThreadActions();
	ExecuteMainThreadActions();

	EXPECT_EQ( 1, firstRuns );
	EXPECT_EQ( 1, secondRuns );
}

TEST( ContinueOnMainThread, NonRecursive )
{
	int runs = 0;
	ContinueOnMainThread( [&] { runs++; } );
	ContinueOnMainThread( [&] { runs++; } );

	ExecuteMainThreadActions();
	ExecuteMainThreadActions();

	EXPECT_EQ( 2, runs );
}

TEST( ContinueOnMainThread, Crash )
{
	static int total = 0;
	bool reentered = false;

	ContinueOnMainThread( [&] {
		total++;
		if( reentered )
		{
			return;
		}
		reentered = true;

		ContinueOnMainThread( [] {
			for( int i = 0; i < 1000; i++ )
			{
				ContinueOnMainThread( [] { total++; } );
			}
		} );

		ExecuteMainThreadActions();
	} );
	ContinueOnMainThread( [] { total++; } );

	ExecuteMainThreadActions();
	ExecuteMainThreadActions();

	EXPECT_EQ( 1002, total );
}
