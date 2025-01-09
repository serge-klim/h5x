#pragma once
#include "h5_types.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/describe.hpp>
#include <type_traits>
#include <cstddef>
#include <cassert>

namespace h5x { inline namespace v0_1 {

template<auto T, typename Enable = std::true_type> struct h5_member_type;
template<typename T, typename U, U T::* P> struct h5_member_type<P> : h5_type<U> {};

template <typename T>
struct h5_type<T, typename boost::describe::has_describe_members<T>::type>
{
   H5::CompType operator()() const noexcept
   {
      H5::CompType type{sizeof(T)};
      boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto member) {
          auto constexpr dummy_offset = alignof(max_align_t);
          auto offset = reinterpret_cast<std::size_t>(&(reinterpret_cast<T const*>(dummy_offset)->*member.pointer)) - dummy_offset;
          type.insertMember(member.name, offset, h5_member_type<decltype(member)::pointer>{}());
      });
      assert(type.getSize() == sizeof(T));
      return type;
   }
};
}/*inline namespace v0_1*/} // namespace h5x


