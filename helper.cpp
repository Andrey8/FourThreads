#include "helper.h"

#include <sstream>
#include <chrono>
#include <random>



namespace Helper
{
	unsigned GetNextRandomInteger( unsigned maxValue )
	{
		static std::mt19937 engine( time( nullptr ) );

		return engine() % ( maxValue + 1 );
	}

	std::string GetGeneratedInfo( ValueHolder const & vh )
	{
		std::stringstream ss;
		ss << 
			"Logging : VAL" <<
			vh.GetId() <<
			" = " <<
			vh.GetValue() <<
			" : " <<
			GetCurrentTime();

		return ss.str();
	}

	char * GetCurrentTime()
	{
		std::chrono::system_clock::time_point current_tp = std::chrono::system_clock::now();
		time_t tt = std::chrono::system_clock::to_time_t( current_tp );

		return ctime( &tt );
	}
}