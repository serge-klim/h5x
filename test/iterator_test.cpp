#include "test.hpp"
#include "h5x/detail/view.hpp"
#include "h5x/iterator.hpp"
#include "h5x/h5_fused.hpp"
#include "h5x/h5_describe.hpp"
#include "H5Cpp.h"
#include <boost/test/data/test_case.hpp>
#include <boost/test/test_tools.hpp>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <algorithm>
#include <cstdint>
#include <cassert>

namespace {

struct fixture
{

	fixture()
       : stored_compound_type(test::raw_compound_type_h5_type())
	{
      expected_content.reserve(100);
      std::generate_n(std::back_inserter(expected_content), expected_content.capacity(), [i=0]() mutable {
         ++i;
         return test::raw_compound_type{i, float(i * i), 1. / (i + 1)};
      });

      if (auto filename = h5_filename(); !exists(filename)) 
	  {
         create_hdf5(h5_filename(), stored_dataset_name(), expected_content);
         assert(exists(filename));
	  }
      BOOST_TEST_MESSAGE("h5 file " << h5_filename());
   }
   
   static std::filesystem::path h5_filename() noexcept { return "compound.h5"; }
   static constexpr char const* stored_dataset_name() noexcept { return "ArrayOfStoredCompoundType"; }
   H5::CompType stored_compound_type;
   std::vector<test::raw_compound_type> expected_content;
};

constexpr bool compare(test::raw_compound_type const& l, test::raw_compound_type const& r) noexcept
{
   return l.a == r.a && l.b == r.b && l.c == r.c;
}

}

BOOST_FIXTURE_TEST_SUITE(h5x_iterator_suite, fixture)


BOOST_AUTO_TEST_CASE(read_back_test)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto dataspace = dataset.getSpace();
   auto data = std::vector<test::raw_compound_type>(dataspace.getSimpleExtentNpoints());
   BOOST_REQUIRE_EQUAL(data.size(), expected_content.size());
   dataset.read(data.data(), stored_compound_type);
   BOOST_CHECK(std::equal(cbegin(expected_content), cend(expected_content), cbegin(data), &compare));
}



std::size_t view_sizes[] = {1, 5, 7, 9, 20, 21, 50, 51, 99, 100, 101, 1024};

BOOST_DATA_TEST_CASE(next_view_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto view = std::make_shared<h5x::detail::View<test::raw_compound_type>>(dataset, view_size);
   BOOST_REQUIRE_EQUAL(view->data_space_size(), expected_content.size());
   BOOST_REQUIRE_EQUAL(view->capacity(), view_size);
   auto n = expected_content.size();
   for (auto i = decltype(n){0}; i < n; ++i)
   {

   }

   //for (auto i = decltype(view->capacity()){0};;) {
   //   auto size = std::min(view->capacity(), view->data_space_size() - i);
   //   while (size-- != 0) {
   //      BOOST_CHECK(compare(view->operator[](size), expected_content[i + view->offset()]));
   //   }
   //   i += view->capacity();
   //   if (i >= expected_content.size())
   //      break;
   //   view = view->next();
   //}
}

BOOST_DATA_TEST_CASE(forawrd_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto range = h5x::make_iterator_range<test::raw_compound_type>(dataset, view_size);
   auto n = std::distance(range.begin(), range.end());
   BOOST_REQUIRE_EQUAL(n, expected_content.size());
   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content),&compare));
}

BOOST_DATA_TEST_CASE(forawrd_fused_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto range = h5x::make_iterator_range<test::fused::SimpleCompoundType>(dataset, view_size);
   auto n = std::distance(range.begin(), range.end());
   BOOST_REQUIRE_EQUAL(n, expected_content.size());
   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
      return l.a == r.a && l.b == r.b && l.c == r.c;
   }));
}

BOOST_DATA_TEST_CASE(forawrd_fused_partial_type_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto range = h5x::make_iterator_range<test::fused::VerySimpleCompoundType>(dataset, view_size);
   auto n = std::distance(range.begin(), range.end());
   BOOST_REQUIRE_EQUAL(n, expected_content.size());
   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
      return l.c == r.c;
       }));
}

BOOST_DATA_TEST_CASE(forawrd_fused_conversion_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto range = h5x::make_iterator_range<test::fused::CompoundTypeAC>(dataset, view_size);
   auto n = std::distance(range.begin(), range.end());
   BOOST_REQUIRE_EQUAL(n, expected_content.size());
   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
      return static_cast<int>(l.a) == r.a && l.b == static_cast<int>(r.b) && l.c == static_cast<float>(r.c);
   }));
}

BOOST_DATA_TEST_CASE(forawrd_describe_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
    H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
    auto dataset = file.openDataSet(stored_dataset_name());
    //static_assert(boost::describe::has_describe_members<test::describe::SimpleCompoundType>::value, "Oops!");
    auto range = h5x::make_iterator_range<test::describe::SimpleCompoundType>(dataset, view_size);
    auto n = std::distance(range.begin(), range.end());
    BOOST_REQUIRE_EQUAL(n, expected_content.size());
    BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
        return l.a == r.a && l.b == r.b && l.c == r.c;
        }));
}

BOOST_DATA_TEST_CASE(forawrd_describe_partial_type_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
    H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
    auto dataset = file.openDataSet(stored_dataset_name());
    auto range = h5x::make_iterator_range<test::describe::VerySimpleCompoundType>(dataset, view_size);
    auto n = std::distance(range.begin(), range.end());
    BOOST_REQUIRE_EQUAL(n, expected_content.size());
    BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
        return l.c == r.c;
        }));
}

BOOST_DATA_TEST_CASE(forawrd_describe_conversion_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
{
    H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
    auto dataset = file.openDataSet(stored_dataset_name());
    auto range = h5x::make_iterator_range<test::describe::CompoundTypeAC>(dataset, view_size);
    auto n = std::distance(range.begin(), range.end());
    BOOST_REQUIRE_EQUAL(n, expected_content.size());
    BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), [](auto const& l, auto const& r) {
        return static_cast<int>(l.a) == r.a && l.b == static_cast<int>(r.b) && l.c == static_cast<float>(r.c);
        }));
}

//BOOST_DATA_TEST_CASE(reverse_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
//{
//   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
//   auto dataset = file.openDataSet(stored_dataset_name());
//   auto range = h5x::make_iterator_range<test::raw_compound_type>(dataset, view_size);
//   BOOST_REQUIRE_EQUAL(std::distance(range.begin(), range.end()), expected_content.size());
//   auto begin = std::reverse_iterator{range.end()};
//   auto end = std::reverse_iterator{range.begin()};
//   auto n = std::distance(begin, end);
//   BOOST_REQUIRE_EQUAL(n, expected_content.size());
//   BOOST_CHECK(std::equal(begin, end, expected_content.rbegin(), &compare));
//   ////double check
////   BOOST_REQUIRE_EQUAL(std::distance(range.begin(), range.end()), expected_content.size());
////   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content), &compare));
//}
//
//////BOOST_AUTO_TEST_CASE(h5diff_hyper1_test)
//////{
//////   H5::H5File file("C://Users//serge//Documents//projects//hdf5//build.lin//tools//test//h5diff//testfiles//h5diff_hyper1.h5", H5F_ACC_RDONLY);
//////   auto dataset = file.openDataSet("big");
//////   auto range = h5x::make_iterator_range<std::uint8_t>(dataset);
//////   BOOST_REQUIRE_EQUAL(std::distance(range.begin(), range.end()), expected_content.size());
//////}

BOOST_AUTO_TEST_SUITE_END()
