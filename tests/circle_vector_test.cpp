#include "testx/testx.hpp"
#include "utilx/circle_vector.hpp"




TESTX_AUTO_TEST_CASE(check_circle_buffer_push)
{
	utilx::circle_vector<int> vec;

	BOOST_CHECK_EQUAL(vec.empty(), true);

	vec.push(0);
	vec.push(1);
	vec.push(2);
	vec.push(3);
	vec.push(4);

	BOOST_CHECK_EQUAL(vec[0], 0);
	BOOST_CHECK_EQUAL(vec[1], 1);
	BOOST_CHECK_EQUAL(vec[2], 2);
	BOOST_CHECK_EQUAL(vec[3], 3);
	BOOST_CHECK_EQUAL(vec[4], 4);

	BOOST_CHECK_EQUAL(vec.empty(), false);
	BOOST_CHECK_EQUAL(vec.size(), 5);
	BOOST_CHECK_EQUAL(vec.front(), 0);
	BOOST_CHECK_EQUAL(vec.back(), 4);
}

TESTX_AUTO_TEST_CASE(check_circle_buffer_push_pop)
{
	utilx::circle_vector<int> vec;

	vec.push(0);
	vec.push(1);
	vec.push(2);
	vec.push(3);
	vec.push(4);

	vec.pop_and_push(5);
	vec.pop_and_push(6);

	BOOST_CHECK_EQUAL(vec[0], 2);
	BOOST_CHECK_EQUAL(vec[1], 3);
	BOOST_CHECK_EQUAL(vec[2], 4);
	BOOST_CHECK_EQUAL(vec[3], 5);
	BOOST_CHECK_EQUAL(vec[4], 6);

	BOOST_CHECK_EQUAL(vec.empty(), false);
	BOOST_CHECK_EQUAL(vec.size(), 5);
	BOOST_CHECK_EQUAL(vec.front(), 2);
	BOOST_CHECK_EQUAL(vec.back(), 6);
}

TESTX_AUTO_TEST_CASE(check_circle_buffer_initializer_list)
{
	utilx::circle_vector<int> vec = { 0, 1, 2, 3, 4 };

	BOOST_CHECK_EQUAL(vec[0], 0);
	BOOST_CHECK_EQUAL(vec[1], 1);
	BOOST_CHECK_EQUAL(vec[2], 2);
	BOOST_CHECK_EQUAL(vec[3], 3);
	BOOST_CHECK_EQUAL(vec[4], 4);
}