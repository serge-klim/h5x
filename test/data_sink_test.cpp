#include "test.hpp"
#include "h5x/data_sink.hpp"
#include "h5x/h5_describe.hpp"
#include "h5x/iterator.hpp"
#include "H5Cpp.h"
#include <boost/test/unit_test.hpp>
#include <cstdio>

BOOST_AUTO_TEST_SUITE(data_sink_test)

BOOST_AUTO_TEST_CASE(data_sink_test_test) {
   std::vector<test::describe::SimpleCompoundType> data;
   data.reserve(111);
   std::generate_n(std::back_inserter(data), data.capacity(), [i = 0]() mutable {
      ++i;
      return test::describe::SimpleCompoundType{i, float(i * i), 1. / (i + 1)};
   });

   auto filename = std::tmpnam(nullptr);
   {
      H5::H5File file(filename, H5F_ACC_TRUNC);
      auto sink = h5x::data_sink<test::describe::SimpleCompoundType>{file, "SimpleCompoundType", 7};
      for (auto value : data)
         sink.push_back(std::move(value));
   }
   H5::H5File file(filename, H5F_ACC_RDONLY);
   auto dataset = file.openDataSet("SimpleCompoundType");
   auto range = h5x::make_iterator_range<test::describe::SimpleCompoundType>(dataset);
   auto n = std::distance(range.begin(), range.end());
   BOOST_REQUIRE_EQUAL(data.size(), n);
}

BOOST_AUTO_TEST_SUITE_END()