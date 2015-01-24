#include <unordered_map>

#include "testx/testx.hpp"
#include "utilx/pair_hash.hpp"



TESTX_AUTO_TEST_CASE(check_pair_hash)
{
	std::hash<std::pair<int, int>> int_hash;
	std::unordered_map<int, std::string> test_map;
	test_map[1] = "hallo";
	
	BOOST_CHECK_EQUAL(int_hash({ 5, 5 }), int_hash({ 5, 5, }));
}