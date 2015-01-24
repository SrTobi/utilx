#include "testx/testx.hpp"
#include "utilx/finally.hpp"




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