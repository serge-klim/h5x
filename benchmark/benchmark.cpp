#include "benchmark/benchmark.h"
#include "bizarre/bizarre.hpp"
#include <vector>
#include <array>
#include <random>
#include <numeric>
#include <functional>
#include <cstdint>


using dump_reason_t = std::uint64_t;
static constexpr std::size_t instruments_number = 4;

struct Fixture : public benchmark::Fixture
{

    Fixture() : dump_reasons{bizarre::generate_random_reasons(instruments_number)}
    {
    }

    std::vector<dump_reason_t> dump_reasons;
};

void streight_loop(std::vector<dump_reason_t> const& dump_reasons, dump_reason_t* buy, dump_reason_t* sell)
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

struct DumpReasons
{
   std::array<std::array<dump_reason_t, instruments_number>, instruments_number> buy;
   std::array<std::array<dump_reason_t, instruments_number>, instruments_number> sell;
};

#pragma pack(push,1)
struct PackedDumpReasons
{
   std::array<std::array<dump_reason_t, instruments_number>, instruments_number> buy;
   std::array<std::array<dump_reason_t, instruments_number>, instruments_number> sell;
};
#pragma pack(pop)

BENCHMARK_DEFINE_F(Fixture, packed_streight_loop) (benchmark::State& state)
{
   auto reasons = PackedDumpReasons{};
   for (auto _ : state) {
      bizarre::populate_dump_reasons(dump_reasons, reasons.buy[0].data(), reasons.sell[0].data());
   }
}

BENCHMARK_DEFINE_F(Fixture, streight_loop) (benchmark::State& state)
{
   auto reasons = DumpReasons{};
   for (auto _ : state) {
      bizarre::populate_dump_reasons(dump_reasons, reasons.buy[0].data(), reasons.sell[0].data());
   }
}

template <class T, T... i>
constexpr auto meta_loop(std::vector<dump_reason_t> const& dump_reasons, std::integer_sequence<T, i...>) noexcept
{
   auto f = [&dump_reasons](auto ix) { return dump_reasons[ix / instruments_number] | dump_reasons[ix % instruments_number]; };
   return std::array<dump_reason_t, instruments_number * instruments_number>{(f(i), ...)};
}

BENCHMARK_DEFINE_F(Fixture, meta_loop) (benchmark::State& state)
{
   auto reasons = DumpReasons{};
   for (auto _ : state) {
      auto res = bizarre::generate_dump_reasons<4>(dump_reasons);
      std::memcpy(reasons.buy[0].data(), res.data(), res.size() * sizeof(dump_reason_t));
      std::memcpy(reasons.sell[0].data(), res.data(), res.size() * sizeof(dump_reason_t));
   }
}

static constexpr auto nitterations = 1000;
BENCHMARK_REGISTER_F(Fixture, packed_streight_loop)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, streight_loop)->Iterations(nitterations);
BENCHMARK_REGISTER_F(Fixture, meta_loop)->Iterations(nitterations);

BENCHMARK_MAIN();

