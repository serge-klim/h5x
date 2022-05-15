#pragma once
#include "h5x/h5_types.hpp"
#include "H5Cpp.h"
#include <vector>
#include <limits>
#include <type_traits>
#include <memory>
#include <cassert>

namespace h5x { inline namespace v0_1 {
namespace detail{

template<typename T>
auto h5_data_type(H5::DataSet const& dataset, double)
{
   auto type = dataset.getDataType();
   assert(type.getSize() == sizeof(T));
   return type;
}
    
template <typename T>
auto h5_data_type(H5::DataSet const& dataset, int) -> decltype(std::declval<h5x::h5_type<T>>()())
{
   return h5x::h5_type<T>{}();
}

template <typename T, std::size_t Rank = 1> class View;

template <typename T>
class View<T, 1>
{
   struct DataSet
   {
      DataSet(H5::DataSet const& dataset) 
          : ds{dataset}
          , type{h5_data_type<T>(dataset, 0)}
          , space{dataset.getSpace()}
          , npoints{static_cast<hsize_t>(space.getSimpleExtentNpoints())}
      {        
         assert(type.getSize() == sizeof(T));
      }
      H5::DataSet ds;
      H5::DataType type;
      H5::DataSpace space;
      hsize_t npoints;
   };
 public:
   View(H5::DataSet const& dataset, std::size_t capacity = 1024) : dataset_{std::make_shared<DataSet>(dataset)}, offset_{(std::numeric_limits<hsize_t>::max)() /*dataset_->npoints*/}, buffer_(capacity) {}
   View(View& existing, hsize_t offset) : dataset_{existing.dataset_}, offset_{offset}, buffer_(existing.capacity())
   {
       auto size = static_cast<hsize_t>(buffer_.size());
       if (offset_ + size > dataset_->npoints)
          size = dataset_->npoints - offset_;
        H5::DataSpace memspace{1, &size};
        dataset_->space.selectHyperslab(H5S_SELECT_SET, &size, &offset_ /*, stride, block*/);
        dataset_->ds.read(buffer_.data(), dataset_->type, memspace, dataset_->space);     
   }

   constexpr auto data_space_size() const noexcept { return dataset_->npoints; }
   constexpr hsize_t offset() const noexcept { return offset_; }
   std::size_t capacity() const noexcept { return buffer_.size(); }
   T const* data() const noexcept { return buffer_.data(); }
   constexpr bool in(hsize_t offset) const noexcept { return offset >= offset_ && offset - offset_ < static_cast<hsize_t>(capacity()) /*std::min(static_cast<hsize_t>(capacity()), data_space_size())*/; }
   T const& at(hsize_t offset)  const noexcept
   {
      assert(in(offset));
      return buffer_[offset - offset_];
   }
 private:
   std::shared_ptr<DataSet> dataset_;
   hsize_t offset_ /*= 0*/;
   std::vector<T> buffer_;
};

}/*namespace detail*/}


/*inline namespace v0_1*/} // namespace h5x


