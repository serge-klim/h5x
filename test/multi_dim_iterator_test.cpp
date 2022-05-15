#include "test.hpp"
#include "h5x/detail/view.hpp"
#include "h5x/iterator.hpp"
#include "H5Cpp.h"
#include <boost/multi_array.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/test_tools.hpp>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <algorithm>
#include <cassert>

namespace {

struct Fixture
{
   struct StoredCompoundType
   {
      int a;
      float b;
      double c;
   };

	Fixture()
       : stored_compound_type(sizeof(StoredCompoundType))
	{
      expected_content.reserve(100);
      std::generate_n(std::back_inserter(expected_content), expected_content.capacity(), [i = 0]() mutable {
         return StoredCompoundType{i, float(i * i), 1. / (i + 1)};
      });

      stored_compound_type.insertMember("a", HOFFSET(StoredCompoundType, a), H5::PredType::NATIVE_INT);
      stored_compound_type.insertMember("c", HOFFSET(StoredCompoundType, c), H5::PredType::NATIVE_DOUBLE);
      stored_compound_type.insertMember("b", HOFFSET(StoredCompoundType, b), H5::PredType::NATIVE_FLOAT);

      if (auto filename = h5_filename(); !exists(filename)) 
	  {
         H5::H5File file(filename.string().c_str(), H5F_ACC_TRUNC);

         /*
          * Create the memory datatype.
          */
         
         //hsize_t dim[] = {expected_content.size() /*LENGTH*/}; /* Dataspace dimensions */
         auto dim = std::vector<hsize_t>{10,5,2};
         H5::DataSpace space{static_cast<int>(dim.size()) /*RANK*/, dim.data()};

         /*
          * Create the dataset.
          */
         auto dataset = file.createDataSet(stored_dataset_name(), stored_compound_type, space);
         dataset.write(expected_content.data(), stored_compound_type);
         assert(exists(filename));
	  }
      BOOST_TEST_MESSAGE("h5 file " << h5_filename());
   }
   
   static std::filesystem::path h5_filename() { return "multi_dim_compound.h5"; }
   static constexpr char const* stored_dataset_name() noexcept { return "ArrayOfStoredCompoundType"; }
   H5::CompType stored_compound_type;
   std::vector<StoredCompoundType> expected_content;
};

constexpr bool compare(Fixture::StoredCompoundType const& l, Fixture::StoredCompoundType const& r) noexcept
{
   return l.a == r.a && l.b == r.b && l.c == r.c;
}

}

BOOST_FIXTURE_TEST_SUITE(H5xMultiDimIteratorSuite, Fixture)

BOOST_AUTO_TEST_CASE(read_flat_back_test)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto dataspace = dataset.getSpace();
   auto data = std::vector<StoredCompoundType>(dataspace.getSimpleExtentNpoints());
   BOOST_REQUIRE_EQUAL(data.size(), expected_content.size());
   dataset.read(data.data(), stored_compound_type);
   BOOST_CHECK(std::equal(cbegin(expected_content), cend(expected_content), cbegin(data), &compare));
}

BOOST_AUTO_TEST_CASE(read_multi_array_back_test)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
   auto dataspace = dataset.getSpace();
   auto rank = dataspace.getSimpleExtentNdims();
   BOOST_REQUIRE_EQUAL(rank, 3);
   auto dims = std::vector<hsize_t>(rank);
   BOOST_REQUIRE_EQUAL(dataspace.getSimpleExtentDims(dims.data(), nullptr), dims.size());

   //auto data = std::vector<StoredCompoundType>(dataspace.getSimpleExtentNpoints());

   auto data = boost::multi_array<StoredCompoundType, 3>{dims/*boost::extents[10][5][2]*/};
   BOOST_REQUIRE_EQUAL(data.num_elements(), expected_content.size());
   BOOST_REQUIRE_EQUAL(data.num_elements(), dataspace.getSimpleExtentNpoints());
   dataset.read(data.data(), stored_compound_type);
   auto i = cbegin(expected_content);
   for (auto const& top : data)
      for (auto const& middle : top)
         for (auto const& bottom : middle)
           BOOST_CHECK(compare(bottom,*i++));
}

std::size_t view_sizes[] = {1, 5, 7, 9, 20, 21, 50, 51, 99, 100, 101, 1024};

BOOST_DATA_TEST_CASE(next_view_test, boost::unit_test::data::make(view_sizes), view_size)
{
   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
   auto dataset = file.openDataSet(stored_dataset_name());
//   auto view = h5x::detail::View<StoredCompoundType, 5>{dataset, view_size};
   //BOOST_REQUIRE_EQUAL(view.data_space_size(), expected_content.size());
   //BOOST_REQUIRE_EQUAL(view.size(), view_size);
   //for (auto i = decltype(view.size()){0};;) {
   //   auto size = std::min(view.size(), view.data_space_size() - i);
   //   while (size-- != 0) {
   //      BOOST_CHECK(compare(view[size], expected_content[i + size + view.offset()]));
   //   }
   //   i += view.size();
   //   if (i >= expected_content.size())
   //      break;
   //   view.next();
   //}
}

//BOOST_DATA_TEST_CASE(forawrd_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
//{
//   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
//   auto dataset = file.openDataSet(stored_dataset_name());
//   auto range = h5x::make_iterator_range<StoredCompoundType>(dataset, view_size);
//   auto n = std::distance(range.begin(), range.end());
//   BOOST_REQUIRE_EQUAL(n, expected_content.size());
//   BOOST_CHECK(std::equal(range.begin(), range.end(), cbegin(expected_content),&compare));
//}
//
//BOOST_DATA_TEST_CASE(reverse_iterator_test, boost::unit_test::data::make(view_sizes), view_size)
//{
//   H5::H5File file(h5_filename().string().c_str(), H5F_ACC_RDONLY);
//   auto dataset = file.openDataSet(stored_dataset_name());
//   auto range = h5x::make_iterator_range<StoredCompoundType>(dataset, view_size);
//   BOOST_REQUIRE_EQUAL(std::distance(range.begin(), range.end()), expected_content.size());
//   auto begin = std::reverse_iterator(range.end());
//   auto end = std::reverse_iterator(range.begin());
//   auto n = std::distance(begin, end);
//   BOOST_REQUIRE_EQUAL(n, expected_content.size());
//   BOOST_CHECK(std::equal(begin, end, cbegin(expected_content), &compare));
//}

BOOST_AUTO_TEST_SUITE_END()
