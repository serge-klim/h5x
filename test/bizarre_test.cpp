#include "test.hpp"
#include "bizarre/bizarre.hpp"
#include <array>

namespace {

struct Fixture
{
};
} // namespace

BOOST_FIXTURE_TEST_SUITE(bizarre_test_sute, Fixture)


BOOST_AUTO_TEST_CASE(read_back_test)
{
   struct DumpReasons
   {
      std::array<std::array<bizarre::dump_reason_t, bizarre::instruments_number>, bizarre::instruments_number> buy;
      std::array<std::array<bizarre::dump_reason_t, bizarre::instruments_number>, bizarre::instruments_number> sell;
   } reasons = {};

   auto dump_reasons = bizarre::generate_random_reasons(bizarre::instruments_number);
   bizarre::populate_dump_reasons(dump_reasons, reasons.buy[0].data(), reasons.sell[0].data());

   BOOST_CHECK_EQUAL_COLLECTIONS(reasons.buy[0].data(), reasons.buy[0].data() + reasons.buy.size() * reasons.buy[0].size(),
                                 reasons.sell[0].data(), reasons.sell[0].data() + reasons.sell.size() * reasons.sell[0].size());


   auto generated_reasons = bizarre::generate_dump_reasons<4>(dump_reasons);
   BOOST_REQUIRE_EQUAL(generated_reasons.size(), reasons.buy.size() * reasons.buy[0].size());
   BOOST_CHECK_EQUAL_COLLECTIONS(reasons.buy[0].data(), reasons.buy[0].data() + reasons.buy.size() * reasons.buy[0].size(), cbegin(generated_reasons), cend(generated_reasons));
}

BOOST_AUTO_TEST_SUITE_END()
