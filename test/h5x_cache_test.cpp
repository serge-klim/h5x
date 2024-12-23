#include "test.hpp"
#include "h5x/h5_types.hpp"
#include "h5x/h5_fused.hpp"
#include "h5x/read_cache.hpp"
#include <boost/test/unit_test.hpp>
#include <filesystem>
#include <vector>
#include <stdexcept>

namespace {

struct fixture
{
   fixture()
   {
      expected_content.reserve(100);
      std::generate_n(std::back_inserter(expected_content), expected_content.capacity(), [i = 0]() mutable {
         ++i;
         return test::raw_compound_type{i, float(i * i), 1. / (i + 1)};
      });
      if (auto filename = h5_filename(); !exists(filename)) {
         create_hdf5(h5_filename(), stored_dataset_name(), expected_content);
         assert(exists(filename));
      }
      BOOST_TEST_MESSAGE("h5 file " << h5_filename());

   }

   static std::filesystem::path h5_filename() noexcept { return "compound.h5"; }
   static constexpr char const* stored_dataset_name() noexcept { return "ArrayOfStoredCompoundType"; }
   std::vector<test::raw_compound_type> expected_content;
};

} // namespace

BOOST_FIXTURE_TEST_SUITE(h5x_read_hash_tests, fixture)


BOOST_AUTO_TEST_CASE(h5x_read_cache_test)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto cache = h5x::read_cache<test::fused::SimpleCompoundType>{dataset, 16};
   auto check_entry = [](h5x::read_cache<test::fused::SimpleCompoundType>& cache, std::size_t ix, test::raw_compound_type const& expected) {
      auto const& val = cache.get(ix);
      BOOST_CHECK_EQUAL(val.a, expected.a);
      BOOST_CHECK_EQUAL(val.b, expected.b);
      BOOST_CHECK_EQUAL(val.c, expected.c);
   };

   check_entry(cache, 20, expected_content[20]);
   check_entry(cache, 21, expected_content[21]);
   check_entry(cache, 3, expected_content[3]);
   check_entry(cache, 5, expected_content[5]);
   check_entry(cache, 0, expected_content[0]);

   BOOST_CHECK_THROW(cache.get(expected_content.size()), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()