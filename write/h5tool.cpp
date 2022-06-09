//#include "h5x/iterator.hpp"
#include "h5x/h5x.hpp"
#include <boost/fusion/include/adapt_struct.hpp>
//#include "hdf5.h"
//#include <boost/format.hpp>
#include <filesystem>
//#include <algorithm>
//#include <vector>
//#include <string>
//#include <numeric>
//#include <algorithm>
//#include <locale>
//#include <functional>
#include <stdexcept>
#include <cassert>

struct CompoundData
{
   short short10 = 10;
   unsigned int uint10 = 10;
   double double10 = 10.;
};

BOOST_FUSION_ADAPT_STRUCT(CompoundData,
                          short10,
                          uint10,
                          double10)

void write(std::filesystem::path const& h5_filename)
{ 
   H5::H5File file{h5_filename.string().c_str(), H5F_ACC_TRUNC};

   constexpr auto rank = 1;
   H5::DSetCreatPropList ds_creatplist;       // create dataset creation prop list
   //auto chunk_size = static_cast<hsize_t>((size + n_chunks - 1) / n_chunks);
   //ds_creatplist.setChunk(rank, &chunk_size); // then modify it for compression
   // ds_creatplist.setDeflate(6);


   auto type = h5x::h5_type<CompoundData>{}();
   ds_creatplist.setFillValue(type, nullptr);
   auto file_dims = std::vector<hsize_t>{3,4,5};
   H5::DataSpace dataspace{static_cast<int>(file_dims.size()), file_dims.data()};
   auto dataset = file.createGroup("/test").createDataSet("compound", type, dataspace, ds_creatplist);

   H5::DataSpace memspace{rank, file_dims.data()};

   //auto attribute_dims = hsize_t{1};
   //H5::DataSpace attribute_dataspace{1, &attribute_dims};
   //dataset.createAttribute("whole_crc", H5::PredType::NATIVE_UINT64, attribute_dataspace).write(H5::PredType::NATIVE_UINT64, &compound_crcs.whole_crc);
   //dataset.createAttribute("tenth_crc", H5::PredType::NATIVE_UINT64, attribute_dataspace).write(H5::PredType::NATIVE_UINT64, &compound_crcs.tenth_crc);
}