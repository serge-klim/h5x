#pragma once
#include "detail/read_cache.hpp"
#include "h5_types.hpp"
#include <vector>
#include <array>
#include <numeric>
#include <stdexcept>
#include <cassert>

namespace h5x { inline namespace v0_1 { 

template <typename T>
class read_cache : h5x::detail::read_cache<T, read_cache<T>> {
   friend class h5x::detail::read_cache<T, read_cache<T>>;
   using size_type = typename h5x::detail::read_cache<T, read_cache<T>>::size_type;
 public:
   using h5x::detail::read_cache<T, read_cache<T>>::get;
   read_cache(H5::DataSet const& dataset, size_type bucket_size = 1024) : bucket_size_{bucket_size}, dataset_{dataset}, /*type{h5_data_type<T>(dataset)}*/ type_{h5_type<T>{}()}, space_{dataset.getSpace()}
   {
      size_ = static_cast<size_type>(space_.getSimpleExtentNpoints());
   }
   constexpr auto size() const noexcept { return size_; }
   constexpr auto empty() const noexcept { return size_ == 0; }
   constexpr bool check_offset(size_type offset) const noexcept { return offset < size_; }
   constexpr size_type bucket_size() const noexcept { return bucket_size_; }
   void load_bucket(size_type ix, std::vector<T>& data);
 private:
   size_type bucket_size_;
   size_type size_;
   H5::DataSet const& dataset_;
   H5::DataType type_;
   H5::DataSpace space_;
};

}} // namespace h5x::v0_1

template <typename T>
void h5x::v0_1::read_cache<T>::load_bucket(size_type offset, std::vector<T>& buffer) {
   auto size = static_cast<hsize_t>(std::min(bucket_size(), size_ - offset));
   H5::DataSpace memspace{1, &size};
   buffer.resize(static_cast<size_type>(size));
   space_.selectHyperslab(H5S_SELECT_SET, &size, &offset /*, stride, block*/);
   dataset_.read(buffer.data(), type_, memspace, space_);     
}


