#pragma once
#include "detail/view.hpp"
#include "H5Cpp.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <memory>
#include <type_traits>
#include <cstddef>
#include <cassert>

namespace h5x { inline namespace v0_1 {

template<typename T>
class ConstIterator
    : public boost::iterator_facade<ConstIterator<T>, T, boost::bidirectional_traversal_tag, T const&>
{
 public:
   ConstIterator(std::shared_ptr<detail::View<T>> view, hsize_t offset) noexcept(std::is_nothrow_move_constructible<std::shared_ptr<detail::View<T>>>::value) : view_{std::move(view)}, offset_{offset} {}
   explicit ConstIterator(std::shared_ptr<detail::View<T>> view) noexcept(std::is_nothrow_move_constructible<std::shared_ptr<detail::View<T>>>::value) : view_{std::move(view)}, offset_{view.offset()} {}

   void increment() noexcept
   {
      assert(offset_ < view_->data_space_size());
      ++offset_;
   }

   void decrement() noexcept
   {
      assert(offset_ != 0);
      --offset_ ;
   }

   T const& dereference() const
   {
       if (!view_->in(offset_))
         view_ = std::make_shared<detail::View<T>>(*view_, offset_);
       return view_->at(offset_); 
   }

   constexpr bool equal(ConstIterator<T> const& other) const noexcept { return offset_ == other.offset_; }
 private:
   mutable std::shared_ptr<detail::View<T>> view_;
   hsize_t offset_ ;    
};

//Produces so called "stashing iterators"
template <typename T>
[[nodiscard]] boost::iterator_range<ConstIterator<T>> make_iterator_range(H5::DataSet const& dataset, std::size_t capacity = 1024)
{
   auto view = std::make_shared<detail::View<T>>(dataset, capacity);
   auto size = view->data_space_size();
   auto begin = ConstIterator<T>{view, 0};
   return boost::make_iterator_range( std::move(begin), ConstIterator<T>{std::move(view), size});
}

}/*inline namespace v0_1*/} // namespace h5x


