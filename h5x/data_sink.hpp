#pragma once
#include "H5Cpp.h"
#include "h5x/h5_types.hpp"
#include <cstddef>
#include <exception>
#include <span>
#include <stdexcept>
#include <vector>

namespace h5x { inline namespace v0_1 {

template <typename T>
class dataset_writer {
 public:
   dataset_writer(H5::H5Location const& location, char const* dataset_name, std::size_t chunk_size = 1024);
   void push_back(T value) { push_back({&value, 1}); }
   void push_back(std::span<T> data);

 private:
   H5::DataType h5_type_;
   H5::DataSet dataset_;
   hsize_t pos_ = 0;
};

template <typename T>
class data_sink {
 public:
   data_sink(H5::H5Location const& location, char const* dataset_name, std::size_t chunk_size = 1024);
   ~data_sink() { flush(); }
   void flush();
   void push_back(T t) { return emplace_back(std::move(t)); }
   template <typename... Args>
   void emplace_back(Args&&... args);

 private:
   std::size_t chunk_size_;
   std::vector<T> buffer_;
   dataset_writer<T> writer_;
};

template <int Rank = 1>
H5::DataSet create_dataset(H5::H5Location const& location, char const* name, H5::DataType const& data_type, hsize_t chunk_size /*= 1024*/) try {
   static_assert(Rank == 1, "TODO: extend");
   H5::DSetCreatPropList props;
   props.setChunk(Rank, &chunk_size);
   props.setFillValue(data_type, nullptr);
   hsize_t dim[Rank] = {0};
   hsize_t dim_max[Rank] = {H5S_UNLIMITED};
   auto space = H5::DataSpace{1, dim, dim_max};
   return location.createDataSet(name, data_type, space, props);
} catch (...) {
   std::throw_with_nested(std::runtime_error{std::string{"unable to create dataset: "} + name});
}

}} // namespace h5x::v0_1

template <typename T>
h5x::v0_1::dataset_writer<T>::dataset_writer(H5::H5Location const& location, char const* dataset_name, std::size_t chunk_size /*= 1024*/)
    : h5_type_{h5_type<T>{}().getId()}, dataset_{create_dataset<1>(location, dataset_name, h5_type_, static_cast<hsize_t>(chunk_size))} {
}

template <typename T>
void h5x::v0_1::dataset_writer<T>::push_back(std::span<T> data) {
   if (auto size = static_cast<hsize_t>(data.size())) {
      auto ds_new_size = pos_ + size;
      dataset_.extend(&ds_new_size);
      auto space = dataset_.getSpace();
      space.selectHyperslab(H5S_SELECT_SET, &size, &pos_);
      auto mem_space = H5::DataSpace{1, &size};
      dataset_.write(data.data(), h5_type_, mem_space, space);
      pos_ += size;
   }
}

template <typename T>
h5x::v0_1::data_sink<T>::data_sink(H5::H5Location const& location, char const* name, std::size_t chunk_size /*= 1024*/)
    : chunk_size_{chunk_size}, writer_(location, name, chunk_size) {
}

template <typename T>
void h5x::v0_1::data_sink<T>::flush() {
   writer_.push_back(buffer_);
   buffer_.clear();
}

template <typename T>
template <typename... Args>
void h5x::v0_1::data_sink<T>::emplace_back(Args&&... args) {
   buffer_.emplace_back(std::forward<Args>(args)...);
   if (buffer_.size() == chunk_size_)
      flush();
}
