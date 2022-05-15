#pragma once
#include "H5Cpp.h"
#include <boost/mp11/algorithm.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/size.hpp>
#include <type_traits>

namespace h5x { inline namespace v0_1 {


template <typename T, typename Enabled = std::true_type>
struct h5_type;


template <>
struct h5_type<char, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_CHAR;
   }
};

template <>
struct h5_type<unsigned char, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_UCHAR;
   }
};

template <>
struct h5_type<short, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_SHORT;
   }
};

template <>
struct h5_type<unsigned short, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_USHORT;
   }
};

template <typename T>
struct h5_type<T, std::bool_constant<std::is_integral<T>::value && std::is_signed<T>::value && sizeof(T) == sizeof(int)>>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_INT;
   }
};

template <typename T>
struct h5_type<T, std::bool_constant<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) == sizeof(unsigned int)>>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_UINT;
   }
};

template <typename T>
struct h5_type<T, std::bool_constant<std::is_integral<T>::value && std::is_signed<T>::value && sizeof(T) == sizeof(long long)>>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_LLONG;
   }
};

template <typename T>
struct h5_type<T, std::bool_constant<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) == sizeof(unsigned long long)>>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_ULLONG;
   }
};

template <>
struct h5_type<float, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_FLOAT;
   }
};

template <>
struct h5_type<double, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_DOUBLE;
   }
};

template<>
struct h5_type<long double, std::true_type>
{
   constexpr H5::DataType const& operator()() const noexcept
   {
      return H5::PredType::NATIVE_LDOUBLE;
   }
};

template <typename T>
struct h5_type<T, typename std::is_enum<T>::type> : h5_type<typename std::underlying_type<T>::type>{};


template <typename T>
struct h5_type<T, std::bool_constant<boost::fusion::traits::is_sequence<T>::value>>
{
   H5::DataType operator()() const noexcept
   {
      H5::CompType type{sizeof(T)};
      boost::mp11::mp_for_each<boost::mp11::mp_iota_c<boost::fusion::result_of::size<T>::value>>([&type](auto I) {
           auto name = boost::fusion::extension::struct_member_name<T, decltype(I)::value>::call();
           auto offset = reinterpret_cast<std::size_t>(&boost::fusion::at_c<decltype(I)::value>(*reinterpret_cast<T const*>(0xff))) - 0xff;
           using h5_type = h5x::h5_type<typename boost::fusion::result_of::value_at_c<T, decltype(I)::value>::type>;
           type.insertMember(name, offset, h5_type{}());
        });
        assert(type.getSize() == sizeof(T));
        return type;
   }
};
}/*inline namespace v0_1*/} // namespace h5x


