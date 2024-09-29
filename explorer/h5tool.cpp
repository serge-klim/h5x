//#include "h5x/iterator.hpp"
#include "h5x/h5_info.hpp"
#include "H5Cpp.h"
#include "hdf5.h"
#include <boost/format.hpp>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <locale>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <cassert>

template<typename T>
std::string dump_dims(std::vector<T> const& values)
{
   assert(!values.empty());
   auto res = std::accumulate(cbegin(values), cend(values), std::make_pair(std::string{}, T{1}), [](auto const& v, const auto& dim) 
       {
            return std::make_pair(str(boost::format("%1%[%2%]")%v.first%dim), v.second * dim);
       });
   return str(boost::format("%1% = %2%") % res.first % res.second);
}

std::ostream& dump_chunks(H5::DataSet const& dataset, H5::DataSpace const& dataspace, H5::DSetCreatPropList const& ds_plist, std::ostream& out)
{
   auto dims = std::vector<hsize_t>(H5S_MAX_RANK);
   dims.resize(ds_plist.getChunk(static_cast<int>(dims.size()), dims.data()));
   out << "chunk dims : " << dump_dims(dims);
   auto nchunks = hsize_t{0};
   if(auto res = H5Dget_num_chunks(dataset.getId(), dataspace.getId(), &nchunks); res<0)
      throw std::runtime_error{str(boost::format("H5Dget_num_chanks filed %1% ") % res)};
   out << "\nchunks : " << nchunks;
   auto whole_size = std::size_t{0};
   for (decltype(nchunks) i = 0; i<nchunks; ++i) 
   {
      auto offset = std::vector<hsize_t>(dims.size());
      auto mask = unsigned{0};
      auto address = haddr_t{0};
      auto size = hsize_t{0};
      if(auto res = H5Dget_chunk_info(dataset.getId(), dataspace.getId(), i, offset.data(), &mask, &address, &size); res < 0)
         throw std::runtime_error{str(boost::format("H5Dget_chunk_info filed %1% ") % res)};
      out <<'\n' << i << " offset : [ "; 
      std::copy(cbegin(offset), cend(offset), std::ostream_iterator<hsize_t>(out, " "));
      out << "], mask : " << mask << ", address : " << address << ", size : " << size;
      whole_size += size;
   }
   out << "\n------------\nsize:"<< whole_size;
   return out;
}

std::ostream& dump_type(H5::DataType const& type, std::ostream& out);
std::ostream& dump_compount_type(H5::DataType const& type, std::ostream& out)
{
   auto n = H5Tget_nmembers(type.getId());
   for (decltype(n) i = 0; i!=n; ++i) 
   {
      auto name = std::unique_ptr<char, decltype(&H5free_memory)>{H5Tget_member_name(type.getId(), i), &H5free_memory};
      out << "\n\t " << i << " : "<< name.get() << " offset : " << H5Tget_member_offset(type.getId(), i);
      dump_type(H5::DataType{H5Tget_member_type(type.getId(),i)}, out);
   }
   return out;
}

//H5Tget_member_name(type.getId(),
//H5Tget_member_type(type.getId(),
//H5Tget_member_offset(type.getId(),

std::ostream& dump_type(H5::DataType const& type, std::ostream& out)
{
   auto const cls = type.getClass();
   out << " size : " << type.getSize() << ' ' << h5x::to_string(cls);
   switch (cls) {
      case H5T_COMPOUND:
         dump_compount_type(type, out);
         break;
      case H5T_ARRAY:
         dump_type(type.getSuper(), out);
         break;
   }
   return out;
}

std::ostream& dump(H5::DataSet const& dataset, std::ostream& out)
{
   auto space = dataset.getSpace();
   out << "dataset size in memory : " << dataset.getInMemDataSize();
   out << "\ndataset storage size : " << dataset.getStorageSize();
   out << "\ndataset type: ";
   auto type = dataset.getDataType();
   dump_type(type, out);

   //out << "\nhyperslab nblocks : " << space.getSelectHyperNblocks();
   auto npoints = space.getSelectNpoints();
   out << "\nnpoints : " << npoints;

   auto rank = space.getSimpleExtentNdims();
   out << "\nrank : " << rank;
   if (rank != 0) {
      auto dims = std::vector<hsize_t>(rank);
      if (auto res = space.getSimpleExtentDims(dims.data(), nullptr); res != dims.size())
         throw std::runtime_error{str(boost::format("unexpected simple extent dims %1% (!=%2%)") % res % dims.size())};
      out << ' ' << dump_dims(dims);
   }
   out << '\n';
   auto ds_plist = dataset.getCreatePlist();
   auto layout = ds_plist.getLayout();
   out << h5x::to_string(layout) << '\n';
   if (layout == H5D_CHUNKED)
      dump_chunks(dataset, space, ds_plist, out);

  //auto range = h5x::make_iterator_range<std::uint8_t>(dataset);
  //std::distance(range.begin(), range.end());
   if (npoints == 1) 
   {
      //auto data = std::vector<char>(npoints * type.getSize());
      auto data = std::string(npoints * type.getSize(), '\0');
      dataset.read(data.data(), type);
      auto locale = std::locale{};
      if (std::all_of(cbegin(data), cend(data), std::bind(&std::isprint<char>, std::placeholders::_1, locale)))
         out << "\nsingle value : " << data;
   }
   return out;
}

std::ostream& dump(std::filesystem::path const& filename, std::string const& dataset, std::ostream& out)
{
   out << "filename : " << filename;
   out << "\ndataset : \"" << dataset << "\"\n";
   //H5::Exception::dontPrint();
    H5::H5File file(filename.string().c_str(), H5F_ACC_RDONLY);
    dump(file.openDataSet(dataset.c_str()), out);
    return out;
}

std::ostream& dump(std::string path, H5::H5Location const& location, std::ostream& out) {
   auto n = location.getNumObjs();
   for (auto i : std::views::iota(decltype(n){0}, n)) {
      H5std_string type_name;
      auto type = location.getObjTypeByIdx(i, type_name);
      auto name = location.getObjnameByIdx(i);
      out << i << ':' << path << name << '-' << type_name << ":\n";
      //out << i << ':' << name << ':' << type_name(type) << '\n';
      switch (type) {
         case H5G_DATASET:
            dump(location.openDataSet(name), out);
            break;
         case H5G_GROUP: {
            path += name;
            path += '/';
            dump(path + name, location.openGroup(name), out);
            break;
         }
      }
   }
   return out;
}

std::ostream& dump(std::filesystem::path const& filename, std::ostream& out) {
   out << "filename : " << filename << std::endl;
//   out << "\ndataset : \"" << dataset << "\"\n";
   // H5::Exception::dontPrint();
   H5::H5File file(filename.string().c_str(), H5F_ACC_RDONLY);
   dump("/", file, out);

   return out;
}

