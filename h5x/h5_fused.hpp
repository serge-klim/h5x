#pragma once
#include "h5_types.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/size.hpp>
#include <type_traits>
#include <cstddef>

namespace h5x { inline namespace v0_1 {

template <typename T>
struct h5_type<T, std::bool_constant<boost::fusion::traits::is_sequence<T>::value>>
{
   H5::DataType operator()() const noexcept
   {
      H5::CompType type{sizeof(T)};
      boost::mp11::mp_for_each<boost::mp11::mp_iota_c<boost::fusion::result_of::size<T>::value>>([&type](auto I) {
           auto name = boost::fusion::extension::struct_member_name<T, decltype(I)::value>::call();
           auto constexpr dummy_offset = alignof(max_align_t);
           auto offset = reinterpret_cast<std::size_t>(&boost::fusion::at_c<decltype(I)::value>(*reinterpret_cast<T const*>(dummy_offset))) - dummy_offset;
           using h5_type = h5x::h5_type<typename boost::fusion::result_of::value_at_c<T, decltype(I)::value>::type>;
           type.insertMember(name, offset, h5_type{}());
        });
        assert(type.getSize() == sizeof(T));
        return type;
   }
};
}/*inline namespace v0_1*/} // namespace h5x


