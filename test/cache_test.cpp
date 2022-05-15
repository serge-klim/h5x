#include "h5x/detail/read_cache.hpp"
#include <boost/test/unit_test.hpp>
#include <vector>

//#include "test.hpp"

namespace {

struct read_cache : h5x::detail::read_cache<int, read_cache>
{
   using size_type = typename h5x::detail::read_cache<int, read_cache>::size_type;
   constexpr size_type bucket_size() const noexcept { return 16; }
   void load_bucket(size_type ix, std::vector<int>& data);
};


void read_cache::load_bucket(size_type ix, std::vector<int>& data)
{
   data.resize(bucket_size());
   std::iota(begin(data), end(data), static_cast<int>(ix));
}

}


BOOST_AUTO_TEST_SUITE(read_cache_tests)

BOOST_AUTO_TEST_CASE(read_cache_test)
{
   auto cache = read_cache{};

   BOOST_CHECK_EQUAL(cache.get(20), 20);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 1);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(21), 21);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 1);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(22), 22);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 1);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(5), 5);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 2);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(0), 0);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 2);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(700), 700);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 3);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(715), 715);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 3);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(1), 1);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 3);
#endif // !NDEBUG
   BOOST_CHECK_EQUAL(cache.get(15), 15);
#ifndef NDEBUG
   BOOST_CHECK_EQUAL(cache.reads, 3);
#endif // !NDEBUG
}

BOOST_AUTO_TEST_SUITE_END()