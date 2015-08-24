#ifndef UTIL_SWITCH_H
#define UTIL_SWITCH_H

#include <functional>
#include <cassert>
#include <map>

namespace util
{
	template
	<
		class _KeyT,
		class _FuncT,
		class _LessT = std::less< _KeyT >,
		class _AllocT = std::allocator
		<
			std::pair< _KeyT, std::function< _FuncT > >
		>
	>
	class Switch
	{
		public:

		typedef _KeyT KeyT;
		typedef std::function< _FuncT > FuncT;
		typedef _LessT LessT;
		typedef _AllocT AllocT;
		typedef typename FuncT::result_type ReturnT;

		private:

		typedef std::map< KeyT, FuncT, LessT, AllocT > FuncMap;
		typedef typename FuncMap::value_type FuncMapEntry;

		public:

		class Case
		{
			public:

			Case( KeyT const & key, FuncT const & func ):
				m_key( key ),
				m_func( func )
			{
				// empty
			}

			KeyT	m_key;
			FuncT	m_func;
		};

		class Default
		{
			public:

			Default( FuncT const & func ):
				m_func( func )
			{
				// empty
			}
			
			FuncT m_func;
		};
		
		template< class ... _ArgTs >
		Switch( _ArgTs const & ... args )
		{
			addCases( args... );
		}

		template< class ... _ArgTs >
		ReturnT operator ()
		(
			KeyT const & key,
			_ArgTs const & ... args
		)
		const
		{
			return doCall< ReturnT >( key, args... );
		}

		ReturnT operator () ( KeyT const & key ) const
		{
			return doCall< ReturnT >( key );
		}

		private:
		
		void addCases( Case const & entry )
		{
			m_funcs.insert( FuncMapEntry( entry.m_key, entry.m_func ) );
		}

		void addCases( Default const & entry )
		{
			// User can specify only one default case
			
			assert( !m_default );
			
			m_default = entry.m_func;
		}

		template< class ... _ArgTs >
		void addCases( Case const & entry, _ArgTs const & ... args )
		{
			addCases( entry );
			addCases( args... );
		}
		
		template< class ... _ArgTs >
		void addCases( Default const & entry, _ArgTs const & ... args )
		{
			// User can specify only one default case
			
			assert( !m_default );
			
			m_default = entry.m_func;
			addCases( args... );
		}
		
		template< class _ReturnT, class ... _ArgsT >
		_ReturnT doCall( KeyT const & key, _ArgsT const & ... args ) const
		{
			typename FuncMap::const_iterator it = m_funcs.find( key );

			if( it != m_funcs.end() )
			{
				return ( it->second )( args... );
			}
			else if( m_default )
			{
				return m_default( args... );
			}

			return ReturnT();
		}

		template< class ... _ArgsT >
		void doCall( KeyT const & key, _ArgsT const & ... args ) const
		{
			typename FuncMap::const_iterator it = m_funcs.find( key );

			if( it != m_funcs.end() )
			{
				return ( it->second )( args... );
			}
			else if( m_default )
			{
				return m_default( args... );
			}
		}

		template< class _ReturnT >
		_ReturnT doCall( KeyT const & key ) const
		{
			typename FuncMap::const_iterator it = m_funcs.find( key );

			if( it != m_funcs.end() )
			{
				return ( it->second )();
			}
			else if( m_default )
			{
				return m_default();
			}

			return ReturnT();
		}

		template<>
		void doCall( KeyT const & key ) const
		{
			typename FuncMap::const_iterator it = m_funcs.find( key );

			if( it != m_funcs.end() )
			{
				return ( it->second )();
			}
			else if( m_default )
			{
				return m_default();
			}
		}

		FuncMap m_funcs;
		FuncT	m_default;
	};
}

#endif
