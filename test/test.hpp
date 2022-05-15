#pragma once
#include "H5Cpp.h"
#include <boost/test/unit_test.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/describe.hpp>
#include <filesystem>
#include <vector>

namespace test{

struct raw_compound_type
{
   int a;
   float b;
   double c;
};

H5::CompType raw_compound_type_h5_type();
void create_hdf5(std::filesystem::path const& file_name, char const* dataset_name, std::vector<raw_compound_type> const& data);

namespace fused {
struct SimpleCompoundType
{
   int a;
   float b;
   double c;
};

struct VerySimpleCompoundType
{
   double c;
};

struct CompoundTypeAC
{
   float c;
   enum class e
   {
   } a;
   int b;
};

} // namespace fused

namespace describe  {
struct SimpleCompoundType
{
   int a;
   float b;
   double c;
};

struct VerySimpleCompoundType
{
   double c;
};

struct CompoundTypeAC
{
   float c;
   enum class e
   {
   } a;
   int b;
};

BOOST_DESCRIBE_STRUCT(SimpleCompoundType, (),
(
        a,
        b,
        c
))

BOOST_DESCRIBE_STRUCT(VerySimpleCompoundType, (),
(
        c
))

BOOST_DESCRIBE_STRUCT(CompoundTypeAC, (),
(
        c,
        a,
        b
))
} // namespace describe

} // namespace test


BOOST_FUSION_ADAPT_STRUCT(
    test::fused::SimpleCompoundType,
    a,
    b,
    c
)


BOOST_FUSION_ADAPT_STRUCT(
    test::fused::VerySimpleCompoundType,
    c)

BOOST_FUSION_ADAPT_STRUCT(
    test::fused::CompoundTypeAC,
                          c,
                          a,
                          b)


