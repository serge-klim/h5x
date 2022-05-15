#include "data.hpp"
#include "benchmark/benchmark.h"
#include "h5x/h5x.hpp"
#include <boost/crc.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <cstdlib>


template <typename T>
bool try_to_allocate(std::size_t size)
{
   auto res = false;
   try {
      auto data= std::vector<T>(size);
      benchmark::DoNotOptimize(data[size - 1]);
      res = true;
   } catch (std::exception const&) {
   }
   return res;
}

template<typename T>
auto unallocatable(std::size_t initial_size = 1000000/*00*/)
{
   //auto data = std::vector<char>{};
   while(auto ptr = std::malloc(initial_size * sizeof(T)))
   {
      benchmark::DoNotOptimize(static_cast<char*>(ptr)[initial_size - 1] = 0xAB);
      std::free(ptr);
      initial_size *= 100;
   }
   return initial_size;
}

template<typename T>
constexpr T enhance(T const& v, std::size_t) noexcept { return v; }

constexpr unsigned int enhance(unsigned int v, std::size_t n) noexcept
{   
   return (v << (sizeof(v) / sizeof(char) - 1) * 8) + static_cast<unsigned int>(n);
}

constexpr double enhance(double v, std::size_t n) noexcept
{
   return v + n;
}


struct Fixture : public benchmark::Fixture
{
   Fixture() : h5_filename{"test_huge.h5"}, dataset_name{"/test/compound"} {}
   void SetUp(const ::benchmark::State& state)
   {
      constexpr auto size = std::size_t{10000000};
      //auto size = unallocatable<CompoundData>() * 2;
      //std::clog << "initializing..." << size << (!try_to_allocate<CompoundData>(size) ? "..." : "...oops might be wrong") << std::endl;

      constexpr auto n_chunks = 1000;
      auto chunk_size = static_cast<hsize_t>((size + n_chunks - 1) / n_chunks);
      if (!std::filesystem::exists(h5_filename) || !is_ok(h5_filename, sizeof(CompoundData), size)) {
         std::clog << "preparing \"" << h5_filename.lexically_normal() << " " << size << "b ..." << std::endl;
         H5::H5File file{h5_filename.string().c_str(), H5F_ACC_TRUNC};


         constexpr auto rank = 1;
         H5::DSetCreatPropList ds_creatplist;  // create dataset creation prop list
         ds_creatplist.setChunk(rank, &chunk_size); // then modify it for compression
         //ds_creatplist.setDeflate(6);


         auto type = h5x::h5_type<CompoundData>{}();
         auto file_size = static_cast<hsize_t>(size);
         H5::DataSpace dataspace{rank, &file_size};
         auto dataset = file.createGroup("/test").createDataSet("compound", type, dataspace, ds_creatplist);

         H5::DataSpace memspace{rank, &chunk_size};

         struct
         {
            boost::crc_32_type whole;
            boost::crc_32_type tenth;
         } crc;
         auto n = std::size_t{0};
         for (auto offset = decltype(chunk_size)(0); offset != chunk_size * n_chunks; offset += chunk_size) {            
             dataspace.selectHyperslab(H5S_SELECT_SET, &chunk_size, &offset /*, stride, block*/);
             auto data = std::vector<CompoundData>(chunk_size);
             for (auto& value: data) 
             {
                boost::mp11::mp_for_each<boost::mp11::mp_iota_c<boost::fusion::result_of::size<CompoundData>::value>>([&value, &crc, n](auto I) {
                   auto&& v = boost::fusion::at_c<I>(value);
                   v = enhance(v, n);
                   crc.whole.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
                   if constexpr (I/3 == 10)
                      crc.tenth.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
                });
             }
             dataset.write(data.data(), type, memspace, dataspace);
             std::cout << '.';
         }
         std::cout << "done";

         compound_crcs.whole_crc = crc.whole.checksum(); 
         compound_crcs.tenth_crc = crc.tenth.checksum();
         auto attribute_dims = hsize_t{1};
         H5::DataSpace attribute_dataspace{1, &attribute_dims};
         dataset.createAttribute("whole_crc", H5::PredType::NATIVE_UINT64, attribute_dataspace).write(H5::PredType::NATIVE_UINT64 , & compound_crcs.whole_crc);
         dataset.createAttribute("tenth_crc", H5::PredType::NATIVE_UINT64, attribute_dataspace).write(H5::PredType::NATIVE_UINT64, &compound_crcs.tenth_crc);
      }
      std::clog << std::hex << compound_crcs.whole_crc << '\n' << compound_crcs.tenth_crc << std::endl;
   }
   bool is_ok(std::filesystem::path& filename, std::size_t expected_type_size, std::size_t expected_min_npoints) noexcept
   {
      auto res = false;
      try {
         H5::H5File file{h5_filename.string().c_str(), H5F_ACC_RDONLY};
         auto dataset = file.openDataSet(dataset_name.c_str());
         auto space = dataset.getSpace();
         res = space.getSimpleExtentNpoints() >= expected_min_npoints;
         if (res) {
            //auto type = dataset.getDataType();
            //res = type.getSize() == sizeof(expected_type_size);
            //if (res) {
               dataset.openAttribute("whole_crc").read(H5::PredType::NATIVE_UINT64, &compound_crcs.whole_crc);
               dataset.openAttribute("tenth_crc").read(H5::PredType::NATIVE_UINT64, &compound_crcs.tenth_crc);
            //}
         }
      }
      catch(...) {}
      return res;
   }
   std::filesystem::path h5_filename;
   std::string dataset_name;
   Summary compound_crcs;
};

//template<typename T>
//Summary calc_checksums(T begin, T end)
//{
//   struct
//   {
//      boost::crc_32_type whole;
//      boost::crc_32_type tenth;
//   } crc;   
//   for (;begin!=end; ++begin) 
//   {
//      auto const& value = *begin;
//      boost::mp11::mp_for_each<boost::mp11::mp_iota_c<boost::fusion::result_of::size<typename T::value_type>::value>>([&value, &crc](auto I) {
//         auto const& v = boost::fusion::at_c<I>(value);
//         crc.whole.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
//         if constexpr (I / 3 == 10)
//            crc.tenth.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
//      });
//   }
//   return {crc.whole.checksum(), crc.tenth.checksum()};
//}

template <typename T>
auto calc_checksums(T begin, T end)
{
   boost::crc_32_type crc;
   for (; begin != end; ++begin) {
      auto const& value = *begin;
      boost::mp11::mp_for_each<boost::mp11::mp_iota_c<boost::fusion::result_of::size<typename std::iterator_traits<T>::value_type>::value>>([&value, &crc](auto I) {
         if constexpr (I / 3 == 10) {
            auto const& v = boost::fusion::at_c<I>(value);
            crc.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
         }
      });
   }
   return crc.checksum();
}

template <typename T>
auto calc_tenth_checksums(T begin, T end)
{
   boost::crc_32_type crc;
   for (; begin != end; ++begin) {
      auto const& value = *begin;
      boost::mp11::mp_for_each < boost::mp11::mp_iota_c<boost::fusion::result_of::size<typename T::value_type>::value>> ([&value, &crc](auto I) {
         auto const& v = boost::fusion::at_c<I>(value);
         crc.process_bytes(&v, sizeof(std::remove_reference_t<decltype(v)>));
      });
   }
   return crc.checksum();
}


BENCHMARK_DEFINE_F(Fixture, iterators) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto range = h5x::make_iterator_range<CompoundData>(dataset, 1024*10);
      auto crc = calc_checksums(range.begin(), range.end());
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
      //if (crc.whole_crc != compound_crcs.whole_crc)
      //   std::cerr << "Oops! whole crc is different!!!" << std::endl;
      //if (crc.tenth_crc != compound_crcs.tenth_crc)
      //   std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

BENCHMARK_DEFINE_F(Fixture, iterators_partial) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto range = h5x::make_iterator_range<PartialCompoundData>(dataset, 1024 * 10);
      auto crc = calc_tenth_checksums(range.begin(), range.end());
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

BENCHMARK_DEFINE_F(Fixture, mem_mapped) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto space = dataset.getSpace();
      auto npoints = space.getSimpleExtentNpoints();
      auto filename = boost::filesystem::unique_path();
      auto params = boost::iostreams::mapped_file_params{filename.string()};
      params.flags = boost::iostreams::mapped_file::readwrite /*priv*/;
      params.length = params.new_file_size = npoints * sizeof(CompoundData);
      params.offset = 0;

      auto memfile = boost::iostreams::mapped_file{params/*filename, boost::iostreams::mapped_file::readwrite, npoints * sizeof(CompoundData)*/};
      auto type = h5x::h5_type<CompoundData>{}();
      auto begin = reinterpret_cast<CompoundData*> (memfile.data());
      dataset.read(begin, type);
      auto crc = calc_checksums(begin, begin + npoints);
      // if (crc.whole_crc != compound_crcs.whole_crc)
      //    std::cerr << "Oops! whole crc is different!!!" << std::endl;
      // if (crc.tenth_crc != compound_crcs.tenth_crc)
      //    std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

BENCHMARK_DEFINE_F(Fixture, inmemory) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto space = dataset.getSpace();
      auto npoints = space.getSimpleExtentNpoints();
      auto buffer = std::vector<CompoundData>(npoints);
      auto type = h5x::h5_type<CompoundData>{}();
      dataset.read(buffer.data(),type);
      auto crc = calc_checksums(buffer.begin(), buffer.end());
      //if (crc.whole_crc != compound_crcs.whole_crc)
      //   std::cerr << "Oops! whole crc is different!!!" << std::endl;
      //if (crc.tenth_crc != compound_crcs.tenth_crc)
      //   std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

BENCHMARK_DEFINE_F(Fixture, inmemory_raw) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto space = dataset.getSpace();
      auto npoints = space.getSimpleExtentNpoints();
      auto buffer = std::vector<CompoundData>(npoints);
      auto type = dataset.getDataType();
      dataset.read(buffer.data(),type);
      auto crc = calc_checksums(buffer.begin(), buffer.end());
      //if (crc.whole_crc != compound_crcs.whole_crc)
      //   std::cerr << "Oops! whole crc is different!!!" << std::endl;
      //if (crc.tenth_crc != compound_crcs.tenth_crc)
      //   std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

BENCHMARK_DEFINE_F(Fixture, inmemory_partial) (benchmark::State& state)
{
   for (auto _ : state) {
      H5::H5File file(h5_filename.string().c_str(), H5F_ACC_RDONLY);
      auto dataset = file.openDataSet(dataset_name);
      auto space = dataset.getSpace();
      auto npoints = space.getSimpleExtentNpoints();
      auto buffer = std::vector<PartialCompoundData>(npoints);
      auto type = h5x::h5_type<PartialCompoundData>{}();
      dataset.read(buffer.data(),type);
      auto crc = calc_tenth_checksums(buffer.begin(), buffer.end());
      if (crc != compound_crcs.tenth_crc)
         std::cerr << "Oops! 10nth crc is different!!!" << std::endl;
   }
}

static constexpr auto nitterations = 10;
BENCHMARK_REGISTER_F(Fixture, iterators_partial)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, iterators)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, mem_mapped)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, inmemory)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, inmemory_raw)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, inmemory_partial)->Iterations(nitterations);

                                              


BENCHMARK_MAIN();

