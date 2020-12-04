#pragma once

#include <string>



namespace Helper
{
	class ValueHolder
	{
	public:
		ValueHolder( int value, int id )
			: m_value( value ), m_id( id ) {}

		int GetValue() const { return m_value; }
		int GetId() const { return m_id; }

		void SetValue( int value ) { m_value = value; }
		void SetId( int id ) { m_id = id; }
	private:
		int m_value;
		int m_id;
	};

	unsigned GetNextRandomInteger( unsigned maxValue );
	std::string GetGeneratedInfo( ValueHolder const & vh );
	char * GetCurrentTime();
}