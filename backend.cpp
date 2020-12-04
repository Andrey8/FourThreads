#include "backend.h"
#include "helper.h"
#include "shared_queue.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <random>
#include <thread>



namespace Backend
{
	const std::string Backend::sm_TITLE = "BACKEND : ";

	void Backend::LogInfoToFile( int & val1, int & val2, bool const & isExit )
	{
		std::ofstream outFile;
		outFile.open( "output.txt" );

		if ( !outFile.is_open() ) {
			throw std::runtime_error( "File has not been opened for writing.\n" );
		}

		std::thread thr1( [ & ]() { GenerateValueAndNotifyLogging( val1, 1, sharedBuffer, isExit ); } );
		
		std::thread thr2( [ & ]() { GenerateValueAndNotifyLogging( val2, 2, sharedBuffer, isExit ); } );
		
		while ( !isExit ) {
			Helper::ValueHolder const * pvh = sharedBuffer.ExtractFront();

			if ( pvh == nullptr ) {
				break;
			}

			outFile << Helper::GetGeneratedInfo( *pvh );
		}

		thr1.join();
		thr2.join();

		outFile << "And the rest :\n";
		size_t const size = sharedBuffer.GetSize();
		if ( size == 0 ) {
			outFile << "Nothing.\n";
		}
		for ( size_t i = 0; i < size; ++i ) {
			outFile << sharedBuffer.ExtractFront() << '\n';
		}

		outFile.close();

		std::cout << sm_TITLE << "Logging process FINISHED\n";
	}

	void Backend::GenerateValueAndNotifyLogging( int & val, int valueId, Helper::SharedQueue< Helper::ValueHolder > & sharedBuffer, bool const & isExit )
	{
		while ( true ) {
			if ( isExit ) {
				NotifyLoggingToTerminate();

				break;
			}

			val = Helper::GetNextRandomInteger( 999 );

			Helper::ValueHolder vh( val, valueId );

			sharedBuffer.PushBack( vh );

			//SetValueHolderAndNotifyLogging( val, valueId, vh, cv1 );

			std::this_thread::sleep_for( std::chrono::milliseconds( 400 ) );
		}

		std::cout << sm_TITLE << "Value generating (id = " << valueId << ") process FINISHED\n";
	}



	void Backend::NotifyLoggingToTerminate()
	{
		std::lock_guard< std::mutex > lg( mtx );

		if ( !loggingIsTerminated ) {
			//cv1.notify_one();
			sharedBuffer.Close();

			loggingIsTerminated = true;
		}
	}

	// void Backend::SetValueHolderAndNotifyLogging( int value, int id, ValueHolder & vh, std::condition_variable & cv )
	// {
	// 	std::lock_guard< std::mutex > lg( mtx );

	// 	vh.SetValue( value );
	// 	vh.SetId( id );

	// 	cv.notify_one();
	// }

}