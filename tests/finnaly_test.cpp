#include "testx/testx.hpp"
#include "utilx/finally.hpp"


template<typename Handler>
utilx::finally make_finally(Handler handler)
{
	return handler;
}

TESTX_AUTO_TEST_CASE(check_finally)
{
	bool destructed = false;

	{
		utilx::finally fin = [&destructed](){
			destructed = true;
		};
	}


	BOOST_CHECK_EQUAL(destructed, true);
}

TESTX_AUTO_TEST_CASE(check_finally_move)
{
	bool destructed = false;

	{
		utilx::finally fin = make_finally([&destructed](){
			destructed = true;
		});
	}


	BOOST_CHECK_EQUAL(destructed, true);
}