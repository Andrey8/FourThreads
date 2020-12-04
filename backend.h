#pragma once

#include "shared_queue.h"
#include "helper.h"

#include <string>
#include <mutex>

//namespace Helper { template < typename T > class SharedQueue; }
//namespace Helper { class ValueHolder; }



namespace Backend
{
	class Backend
	{
	public:
		void LogInfoToFile( int & val1, int & val2, bool const & isExit );
		void GenerateValueAndNotifyLogging( int & val, int valueId, Helper::SharedQueue< Helper::ValueHolder > & sharedBuffer, bool const & isExit );

	private:
		std::mutex mtx;		
		bool loggingIsTerminated = false;
		Helper::SharedQueue< Helper::ValueHolder > sharedBuffer;
		//std::condition_variable cv1;

		static const std::string sm_TITLE;

		void NotifyLoggingToTerminate();
		//void SetValueHolderAndNotifyLogging( int value, int id, ValueHolder & vh, std::condition_variable & cv );		
	};

	
	
}