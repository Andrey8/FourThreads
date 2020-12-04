#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>



namespace Helper
{
	template < typename T >
	class SharedQueue
	{
	public:
		void PushBack( T const & item );
		T const * ExtractFront();

		size_t GetSize();

		void Close();
	private:
		std::queue< T > m_data;
		std::mutex m_mtx;
		std::condition_variable m_cv;
		bool m_isClosed = false;
	};

	template < typename T >
	void SharedQueue< T >::PushBack( T const & item )
	{
		std::unique_lock< std::mutex > ul( m_mtx );
		m_data.push( item );
		m_cv.notify_one();
	}

	template < typename T >
	T const * SharedQueue< T >::ExtractFront()
	{
		std::unique_lock< std::mutex > ul( m_mtx );

		while ( m_data.empty() ) {
			m_cv.wait( ul );

			if ( m_isClosed ) {
				return nullptr;
			}
		}

		T const * result = &( m_data.front() );
		m_data.pop();

		return result;			
	}

	template < typename T >
	size_t SharedQueue< T >::GetSize()
	{
		std::unique_lock< std::mutex > ul( m_mtx );

		return m_data.size();
	}

	template < typename T >
	void SharedQueue< T >::Close()
	{
		if ( !m_isClosed ) {
			m_isClosed = true;

			m_cv.notify_one();
		}
	}
}