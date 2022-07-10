#include "bizarre.hpp"
#include <random>
#include <functional>
#include <numeric>

void bizarre::populate_dump_reasons(std::vector<dump_reason_t> const& dump_reasons, dump_reason_t* buy, dump_reason_t* sell)
{
   auto primary_security_ids = std::vector<int>(instruments_number);
   auto table_dump_reason_size_ = dump_reasons.size();
   std::iota(begin(primary_security_ids), end(primary_security_ids), 0);
   //////////////
   auto sec_id_idx = std::size_t{0};
   for (auto const& security_id : primary_security_ids) {
      for (auto i = std::size_t{0}; i != primary_security_ids.size(); ++i) {
         auto idx1 = i + sec_id_idx * table_dump_reason_size_;
         auto idx2 = i * table_dump_reason_size_ + sec_id_idx;
         auto dump_reason = dump_reasons[security_id];
         buy[idx1] |= dump_reason;
         sell[idx1] |= dump_reason;
         buy[idx2] |= dump_reason;
         sell[idx2] |= dump_reason;
      }
      ++sec_id_idx;
   }
}

std::vector<bizarre::dump_reason_t> bizarre::generate_random_reasons(std::size_t n)
{
   std::random_device rd;
   std::mt19937 gen{rd()}; // Standard mersenne_twister_engine seeded with rd()
   std::uniform_int_distribution<dump_reason_t> distrib{1};
   auto res = std::vector<dump_reason_t>(n);
   std::generate(begin(res), end(res), std::bind(distrib, gen));
   return res;
}
