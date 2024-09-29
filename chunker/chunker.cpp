#include "loggers.hpp"
#include "h5x/h5_info.hpp"
#include "H5Cpp.h"
#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>
#include <string>
#include <filesystem>

namespace {

void chunk_dataset(H5::H5Location const& location, /*H5std_string*/ std::string const& name, hsize_t chunk_size) {
   auto dataset = location.openDataSet(name);
   auto type = dataset.getDataType();
   auto space = dataset.getSpace();
   auto rank = space.getSimpleExtentNdims();
   if (rank != 1) {
      BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "dataset \"" << name << "\" has rank " << rank << " skipping...";
      return;
   }
   auto ds_plist = dataset.getCreatePlist();
   auto layout = ds_plist.getLayout();
   if (layout != H5D_CHUNKED) {
      BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "dataset \"" << name << "\" layout is " << h5x::to_string(layout) << " skipping...";
      return;
   } else {
      hsize_t current_chunk_size;
      ds_plist.getChunk(1, &current_chunk_size);
      if (current_chunk_size == chunk_size) {
         BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "dataset \"" << name << "\" already chunked and chunk size is " << current_chunk_size << " skipping...";
         return;
      }
   }

   auto npoints = static_cast<decltype(chunk_size)>(space.getSelectNpoints());
   if (npoints < chunk_size) {
      BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "dataset \"" << name << "\" size is : " << npoints 
          << " which less than requested chunk size (" << npoints <<" )skipping...";
      return;
   }
   auto const type_size = type.getSize();
   BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "chunking dataset \""<< name<<"\" size :" << npoints 
   << "\n\tsize in memory : " << dataset.getInMemDataSize()
   << "\n\tstorage size : " << dataset.getStorageSize()
   << "\n\ttype size : " << type_size
   << "\n...";

   auto buffer = std::vector<char>(npoints * type_size);
   dataset.read(buffer.data(), type);
   dataset.close();
   //auto backup = name;
   //backup += '.';
   //backup = to_iso_string(boost::posix_time::second_clock::local_time());
   //location.move(name, backup);
   location.unlink(name);

   H5::DSetCreatPropList ds_creatplist;      
   ds_creatplist.setChunk(rank, &chunk_size);

   H5::DataSpace dataspace{rank, &npoints};
   auto nu_dataset = location.createDataSet(name, type, dataspace, ds_creatplist); 
   auto offset = decltype(chunk_size){0};
   H5::DataSpace memspace{rank, &chunk_size};
   while (offset + chunk_size <= npoints) {
      dataspace.selectHyperslab(H5S_SELECT_SET, &chunk_size, &offset /*, stride, block*/);
      nu_dataset.write(buffer.data() + offset, type, memspace, dataspace);
      offset += chunk_size;
   }
   if (offset != npoints) {
      assert(offset < npoints);
      auto size = npoints - offset;
      H5::DataSpace memspace_last{rank, &size};
      dataspace.selectHyperslab(H5S_SELECT_SET, &size, &offset);
      nu_dataset.write(buffer.data() + offset, type, memspace_last, dataspace);
   }
}

void chunk(H5::H5Location const& location, std::size_t chunk_size) {
   auto datasets = std::vector<std::string>{};
   auto const n = location.getNumObjs();
   for (auto i = decltype(n){0}; i != n; ++i) {
      switch (auto type = location.getObjTypeByIdx(i)) {
         case H5G_DATASET:
            // chunk_dataset(location, location.getObjnameByIdx(i), static_cast<hsize_t>(chunk_size));
            datasets.push_back(location.getObjnameByIdx(i));
            break;
         case H5G_GROUP: {
            auto name = location.getObjnameByIdx(i);
            chunk(location.openGroup(name), chunk_size);
            break;
         }
      }
   }
   for (auto const& dataset : datasets) {
       try {
          chunk_dataset(location, dataset, static_cast<hsize_t>(chunk_size));
       } catch (H5::Exception& e) {
          BOOST_LOG_SEV(log::get(), boost::log::trivial::error) << "unable to chunk dataset \"" << dataset << "\" " << e.getDetailMsg();
       }
   }
}
} // namespace

void chunk_file(boost::program_options::variables_map const& options)
{
   auto filename = options["filename"].as<std::filesystem::path>();
   H5::H5File file(filename.string().c_str(), H5F_ACC_RDWR);
   chunk(file, options["chunk-size"].as<std::size_t>());
}

