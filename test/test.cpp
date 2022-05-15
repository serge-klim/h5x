#define BOOST_TEST_MODULE h5x_tests
#include <boost/test/unit_test.hpp>
#include "test.hpp"
#include "H5Cpp.h"


H5::CompType test::raw_compound_type_h5_type() {
   H5::CompType h5type{sizeof(raw_compound_type)};
   h5type.insertMember("a", HOFFSET(raw_compound_type, a), H5::PredType::NATIVE_INT);
   h5type.insertMember("c", HOFFSET(raw_compound_type, c), H5::PredType::NATIVE_DOUBLE);
   h5type.insertMember("b", HOFFSET(raw_compound_type, b), H5::PredType::NATIVE_FLOAT);
   return h5type;
}

void test::create_hdf5(std::filesystem::path const& file_name, char const* dataset_name, std::vector<raw_compound_type> const& data) {

   H5::H5File file(file_name.string().c_str(), H5F_ACC_TRUNC);

   hsize_t dim = data.size() /*LENGTH*/; /* Dataspace dimensions */
   H5::DataSpace space{1 /*RANK*/, &dim};

   auto h5type = raw_compound_type_h5_type();
   auto dataset = file.createDataSet(dataset_name, h5type, space);
   dataset.write(data.data(), h5type);
}