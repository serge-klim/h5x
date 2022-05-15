#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include <cstdint>

struct Summary
{
   std::uint64_t whole_crc;
   std::uint64_t tenth_crc;
};

BOOST_FUSION_ADAPT_STRUCT(Summary,
                          whole_crc,
                          tenth_crc)

struct PartialCompoundData
{
   short short10 = 10;
   unsigned int uint10 = 10;
   double double10 = 10.;
};

BOOST_FUSION_ADAPT_STRUCT(PartialCompoundData,
                          short10,
                          uint10,
                          double10)

struct CompoundData{
short                   short0                   = 0;
unsigned int            uint0                    = 0;
double                  double0                  = 0.;
short                   short1                   = 1;
unsigned int            uint1                    = 1;
double                  double1                  = 1.;
short                   short2                   = 2;
unsigned int            uint2                    = 2;
double                  double2                  = 2.;
short                   short3                   = 3;
unsigned int            uint3                    = 3;
double                  double3                  = 3.;
short                   short4                   = 4;
unsigned int            uint4                    = 4;
double                  double4                  = 4.;
short                   short5                   = 5;
unsigned int            uint5                    = 5;
double                  double5                  = 5.;
short                   short6                   = 6;
unsigned int            uint6                    = 6;
double                  double6                  = 6.;
short                   short7                   = 7;
unsigned int            uint7                    = 7;
double                  double7                  = 7.;
short                   short8                   = 8;
unsigned int            uint8                    = 8;
double                  double8                  = 8.;
short                   short9                   = 9;
unsigned int            uint9                    = 9;
double                  double9                  = 9.;
short                   short10                  = 10;
unsigned int            uint10                   = 10;
double                  double10                         = 10.;
short                   short11                  = 11;
unsigned int            uint11                   = 11;
double                  double11                         = 11.;
short                   short12                  = 12;
unsigned int            uint12                   = 12;
double                  double12                         = 12.;
short                   short13                  = 13;
unsigned int            uint13                   = 13;
double                  double13                         = 13.;
short                   short14                  = 14;
unsigned int            uint14                   = 14;
double                  double14                         = 14.;
short                   short15                  = 15;
unsigned int            uint15                   = 15;
double                  double15                         = 15.;
short                   short16                  = 16;
unsigned int            uint16                   = 16;
double                  double16                         = 16.;
short                   short17                  = 17;
unsigned int            uint17                   = 17;
double                  double17                         = 17.;
short                   short18                  = 18;
unsigned int            uint18                   = 18;
double                  double18                         = 18.;
short                   short19                  = 19;
unsigned int            uint19                   = 19;
double                  double19                         = 19.;
short                   short20                  = 20;
unsigned int            uint20                   = 20;
double                  double20                         = 20.;
short                   short21                  = 21;
unsigned int            uint21                   = 21;
double                  double21                         = 21.;
short                   short22                  = 22;
unsigned int            uint22                   = 22;
double                  double22                         = 22.;
short                   short23                  = 23;
unsigned int            uint23                   = 23;
double                  double23                         = 23.;
short                   short24                  = 24;
unsigned int            uint24                   = 24;
double                  double24                         = 24.;
short                   short25                  = 25;
unsigned int            uint25                   = 25;
double                  double25                         = 25.;
short                   short26                  = 26;
unsigned int            uint26                   = 26;
double                  double26                         = 26.;
short                   short27                  = 27;
unsigned int            uint27                   = 27;
double                  double27                         = 27.;
short                   short28                  = 28;
unsigned int            uint28                   = 28;
double                  double28                         = 28.;
short                   short29                  = 29;
unsigned int            uint29                   = 29;
double                  double29                         = 29.;
short                   short30                  = 30;
unsigned int            uint30                   = 30;
double                  double30                         = 30.;
short                   short31                  = 31;
unsigned int            uint31                   = 31;
double                  double31                         = 31.;
short                   short32                  = 32;
unsigned int            uint32                   = 32;
double                  double32                         = 32.;
short                   short33                  = 33;
unsigned int            uint33                   = 33;
double                  double33                         = 33.;
short                   short34                  = 34;
unsigned int            uint34                   = 34;
double                  double34                         = 34.;
short                   short35                  = 35;
unsigned int            uint35                   = 35;
double                  double35                         = 35.;
short                   short36                  = 36;
unsigned int            uint36                   = 36;
double                  double36                         = 36.;
short                   short37                  = 37;
unsigned int            uint37                   = 37;
double                  double37                         = 37.;
short                   short38                  = 38;
unsigned int            uint38                   = 38;
double                  double38                         = 38.;
short                   short39                  = 39;
unsigned int            uint39                   = 39;
double                  double39                         = 39.;
short                   short40                  = 40;
unsigned int            uint40                   = 40;
double                  double40                         = 40.;
short                   short41                  = 41;
unsigned int            uint41                   = 41;
double                  double41                         = 41.;
short                   short42                  = 42;
unsigned int            uint42                   = 42;
double                  double42                         = 42.;
short                   short43                  = 43;
unsigned int            uint43                   = 43;
double                  double43                         = 43.;
short                   short44                  = 44;
unsigned int            uint44                   = 44;
double                  double44                         = 44.;
short                   short45                  = 45;
unsigned int            uint45                   = 45;
double                  double45                         = 45.;
short                   short46                  = 46;
unsigned int            uint46                   = 46;
double                  double46                         = 46.;
short                   short47                  = 47;
unsigned int            uint47                   = 47;
double                  double47                         = 47.;
short                   short48                  = 48;
unsigned int            uint48                   = 48;
double                  double48                         = 48.;
short                   short49                  = 49;
unsigned int            uint49                   = 49;
double                  double49                         = 49.;
short                   short50                  = 50;
unsigned int            uint50                   = 50;
double                  double50                         = 50.;
short                   short51                  = 51;
unsigned int            uint51                   = 51;
double                  double51                         = 51.;
short                   short52                  = 52;
unsigned int            uint52                   = 52;
double                  double52                         = 52.;
short                   short53                  = 53;
unsigned int            uint53                   = 53;
double                  double53                         = 53.;
short                   short54                  = 54;
unsigned int            uint54                   = 54;
double                  double54                         = 54.;
short                   short55                  = 55;
unsigned int            uint55                   = 55;
double                  double55                         = 55.;
short                   short56                  = 56;
unsigned int            uint56                   = 56;
double                  double56                         = 56.;
short                   short57                  = 57;
unsigned int            uint57                   = 57;
double                  double57                         = 57.;
short                   short58                  = 58;
unsigned int            uint58                   = 58;
double                  double58                         = 58.;
short                   short59                  = 59;
unsigned int            uint59                   = 59;
double                  double59                         = 59.;
short                   short60                  = 60;
unsigned int            uint60                   = 60;
double                  double60                         = 60.;
short                   short61                  = 61;
unsigned int            uint61                   = 61;
double                  double61                         = 61.;
short                   short62                  = 62;
unsigned int            uint62                   = 62;
double                  double62                         = 62.;
short                   short63                  = 63;
unsigned int            uint63                   = 63;
double                  double63                         = 63.;
short                   short64                  = 64;
unsigned int            uint64                   = 64;
double                  double64                         = 64.;
short                   short65                  = 65;
unsigned int            uint65                   = 65;
double                  double65                         = 65.;
short                   short66                  = 66;
unsigned int            uint66                   = 66;
double                  double66                         = 66.;
short                   short67                  = 67;
unsigned int            uint67                   = 67;
double                  double67                         = 67.;
short                   short68                  = 68;
unsigned int            uint68                   = 68;
double                  double68                         = 68.;
short                   short69                  = 69;
unsigned int            uint69                   = 69;
double                  double69                         = 69.;
short                   short70                  = 70;
unsigned int            uint70                   = 70;
double                  double70                         = 70.;
};


BOOST_FUSION_ADAPT_STRUCT( CompoundData,
(auto, short0)
(auto, uint0)
(auto, double0)
(auto, short1)
(auto, uint1)
(auto, double1)
(auto, short2)
(auto, uint2)
(auto, double2)
(auto, short3)
(auto, uint3)
(auto, double3)
(auto, short4)
(auto, uint4)
(auto, double4)
(auto, short5)
(auto, uint5)
(auto, double5)
(auto, short6)
(auto, uint6)
(auto, double6)
(auto, short7)
(auto, uint7)
(auto, double7)
(auto, short8)
(auto, uint8)
(auto, double8)
(auto, short9)
(auto, uint9)
(auto, double9)
(auto, short10)
(auto, uint10)
(auto, double10)
(auto, short11)
(auto, uint11)
(auto, double11)
(auto, short12)
(auto, uint12)
(auto, double12)
(auto, short13)
(auto, uint13)
(auto, double13)
(auto, short14)
(auto, uint14)
(auto, double14)
(auto, short15)
(auto, uint15)
(auto, double15)
(auto, short16)
(auto, uint16)
(auto, double16)
(auto, short17)
(auto, uint17)
(auto, double17)
(auto, short18)
(auto, uint18)
(auto, double18)
(auto, short19)
(auto, uint19)
(auto, double19)
(auto, short20)
(auto, uint20)
(auto, double20)
(auto, short21)
(auto, uint21)
(auto, double21)
(auto, short22)
(auto, uint22)
(auto, double22)
(auto, short23)
(auto, uint23)
(auto, double23)
(auto, short24)
(auto, uint24)
(auto, double24)
(auto, short25)
(auto, uint25)
(auto, double25)
(auto, short26)
(auto, uint26)
(auto, double26)
(auto, short27)
(auto, uint27)
(auto, double27)
(auto, short28)
(auto, uint28)
(auto, double28)
(auto, short29)
(auto, uint29)
(auto, double29)
(auto, short30)
(auto, uint30)
(auto, double30)
(auto, short31)
(auto, uint31)
(auto, double31)
(auto, short32)
(auto, uint32)
(auto, double32)
(auto, short33)
(auto, uint33)
(auto, double33)
(auto, short34)
(auto, uint34)
(auto, double34)
(auto, short35)
(auto, uint35)
(auto, double35)
(auto, short36)
(auto, uint36)
(auto, double36)
(auto, short37)
(auto, uint37)
(auto, double37)
(auto, short38)
(auto, uint38)
(auto, double38)
(auto, short39)
(auto, uint39)
(auto, double39)
(auto, short40)
(auto, uint40)
(auto, double40)
(auto, short41)
(auto, uint41)
(auto, double41)
(auto, short42)
(auto, uint42)
(auto, double42)
(auto, short43)
(auto, uint43)
(auto, double43)
(auto, short44)
(auto, uint44)
(auto, double44)
(auto, short45)
(auto, uint45)
(auto, double45)
(auto, short46)
(auto, uint46)
(auto, double46)
(auto, short47)
(auto, uint47)
(auto, double47)
(auto, short48)
(auto, uint48)
(auto, double48)
(auto, short49)
(auto, uint49)
(auto, double49)
(auto, short50)
(auto, uint50)
(auto, double50)
(auto, short51)
(auto, uint51)
(auto, double51)
(auto, short52)
(auto, uint52)
(auto, double52)
(auto, short53)
(auto, uint53)
(auto, double53)
(auto, short54)
(auto, uint54)
(auto, double54)
(auto, short55)
(auto, uint55)
(auto, double55)
(auto, short56)
(auto, uint56)
(auto, double56)
(auto, short57)
(auto, uint57)
(auto, double57)
(auto, short58)
(auto, uint58)
(auto, double58)
(auto, short59)
(auto, uint59)
(auto, double59)
(auto, short60)
(auto, uint60)
(auto, double60)
(auto, short61)
(auto, uint61)
(auto, double61)
(auto, short62)
(auto, uint62)
(auto, double62)
(auto, short63)
(auto, uint63)
(auto, double63)
(auto, short64)
(auto, uint64)
(auto, double64)
(auto, short65)
(auto, uint65)
(auto, double65)
(auto, short66)
(auto, uint66)
(auto, double66)
(auto, short67)
(auto, uint67)
(auto, double67)
(auto, short68)
(auto, uint68)
(auto, double68)
(auto, short69)
(auto, uint69)
(auto, double69)
(auto, short70)
(auto, uint70)
(auto, double70)
)
