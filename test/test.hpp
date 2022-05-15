#pragma once
#include <boost/test/unit_test.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace test{

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

} // namespace test


BOOST_FUSION_ADAPT_STRUCT(
    test::SimpleCompoundType,
    a,
    b,
    c
)


BOOST_FUSION_ADAPT_STRUCT(
    test::VerySimpleCompoundType,
    c)

BOOST_FUSION_ADAPT_STRUCT(
    test::CompoundTypeAC,
                          c,
                          a,
                          b)
