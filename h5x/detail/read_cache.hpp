#pragma once
#include <vector>
#include <array>
#include <string>
#include <numeric>
#include <stdexcept>
#include <cstdint>
#include <cassert>

namespace h5x { inline namespace v0_1 { namespace detail {

template <typename T, typename BucketLoader>
class read_cache {
   using data_type = std::vector<T>; 
   using bucket_ix_type = std::uint8_t;
public:
   using size_type = typename data_type::size_type;
   constexpr bool check_offset(size_type) const noexcept { return true; }
   constexpr size_type bucket_size() const noexcept { return 1024; }
   T const& get(size_type ix);
 private:
   bucket_ix_type load_bucket(size_type ix, bool below);
   void load_bucket(size_type, data_type&) { throw std::runtime_error{"please provide implementation"}; }
   struct bucket{
		size_type offset = 0;
		data_type data;
   };
private:  
   size_type bucket_size_ = 1024;
   std::array<bucket, 2> buckets_;
   bucket_ix_type stale_ = 0;
 public:
#ifndef NDEBUG
   std::size_t reads = 0;
#endif // !NDEBUG
};
}}} // namespace h5x::v0_1::detail

template <typename T, typename BucketLoader>
T const& h5x::v0_1::detail::read_cache<T, BucketLoader>::get(size_type ix)
{
   auto below = buckets_[0].offset > ix;
   if (!below) {
      if (buckets_[0].data.size() > ix - buckets_[0].offset) {
         stale_ = 1;
         return buckets_[0].data[ix - buckets_[0].offset];
      }
      below = buckets_[1].offset > ix;
      if (!below && buckets_[1].data.size() > ix - buckets_[1].offset) {
            stale_ = 0;
            return buckets_[1].data[ix - buckets_[1].offset];
      }
   }
   auto const& bucket = buckets_[load_bucket(ix, below)];
   return bucket.data[ix - bucket.offset];
}

template <typename T, typename BucketLoader>
typename h5x::v0_1::detail::read_cache<T, BucketLoader>::bucket_ix_type h5x::v0_1::detail::read_cache<T, BucketLoader>::load_bucket(size_type offset, bool below) {
   if (!static_cast<BucketLoader*>(this)->check_offset(offset))
      throw std::out_of_range{std::to_string(offset) + " out of range"};
   if (below)
      offset = offset > static_cast<BucketLoader*>(this)->bucket_size() ? offset - static_cast<BucketLoader*>(this)->bucket_size() + 1 : 0;
   static_cast<BucketLoader*>(this)->load_bucket(offset, buckets_[stale_].data);
   buckets_[stale_].offset = offset;
#ifndef NDEBUG
   ++reads;
#endif // !NDEBUG
   auto res = stale_;
   stale_ = stale_ ^ 1;
   if (buckets_[0].offset > buckets_[1].offset) {
      std::swap(buckets_[0].data, buckets_[1].data);
      std::swap(buckets_[0].offset, buckets_[1].offset);
      std::swap(stale_, res);
   } 
   return res;
}


