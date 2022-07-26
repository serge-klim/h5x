#pragma once 

#include <array>
#include <vector>
#include <type_traits>
#include <cstdint>
#include <cassert>

namespace bizarre {

using dump_reason_t = std::uint64_t;
static constexpr std::size_t instruments_number = 4;


void populate_dump_reasons(std::vector<dump_reason_t> const& dump_reasons, dump_reason_t* buy, dump_reason_t* sell);
std::vector<dump_reason_t> generate_random_reasons(std::size_t n);

namespace detail {

template <std::size_t row_size, std::size_t... i>
constexpr auto generate_dump_reasons(std::vector<dump_reason_t> const& dump_reasons, std::integer_sequence<std::size_t, i...>) noexcept
{
   constexpr auto size = sizeof...(i);
   static_assert(row_size * row_size == size);
   auto f = [&dump_reasons](auto ix) { 
	   return dump_reasons[ix / row_size] | dump_reasons[ix % row_size]; 
   };
   auto res = std::array<dump_reason_t, size>{f(i)...};
   return res;
}
} // namespace detail

template<std::size_t Size = 4>
#ifndef NDEBUG
constexpr
#endif
auto generate_dump_reasons(std::vector<dump_reason_t> const& dump_reasons)
{
   assert(dump_reasons.size() == 4);
   return detail::generate_dump_reasons<Size>(dump_reasons, std::make_integer_sequence<std::size_t, Size * Size>{});
}

namespace mul {

template <typename Left, typename Right>
constexpr bool same_sing(Left left, Right right) noexcept
{
   return left * right >= 0;
}

} // namespace mul

template <typename Left, typename Right>
constexpr auto same_sing(Left left, Right right) noexcept -> std::enable_if_t<std::is_signed_v<std::common_type_t<Left, Right>>, bool>
{
   using unsigned_type = std::make_unsigned_t<std::common_type_t<Left, Right>>;
   return left==0 || right == 0 ||  (static_cast<unsigned_type>(left) ^ static_cast<unsigned_type>(right)) >= 0;
}

}
