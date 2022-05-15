#pragma once
#include "hdf5.h"

namespace h5x { inline namespace v0_1 {

constexpr const char* to_string(H5D_layout_t type) noexcept {
   switch (type) {
      case H5D_LAYOUT_ERROR:
         return "error";
      case H5D_COMPACT:
         return "H5D_COMPACT raw data is small(< 64KB)";
      case H5D_CONTIGUOUS:
         return "H5D_CONTIGUOUS contiguous layout";
      case H5D_CHUNKED:
         return "H5D_CHUNKED chunked or tiled layout";
      case H5D_VIRTUAL:
         return "H5D_VIRTUAL actual data is stored in other datasets";
      //case H5D_NLAYOUTS: // this one must be last!"
      //   [[fallthrough]];
      //default:
      //   out << "unknow layout" << layout;
   }
   return "unknown layout type";
}

constexpr char const* to_string(H5G_obj_t type) noexcept {
   switch (type) {
      case H5G_UNKNOWN: return "unknown";
      case H5G_GROUP: return "group";
      case H5G_DATASET: return "dataset";
      case H5G_TYPE: return "named data type";
      case H5G_LINK: return "symbolic link";
      case H5G_UDLINK: return "user-defined link";
      case H5G_RESERVED_5: return "Reserved 5";
      case H5G_RESERVED_6: return "Reserved 6";
      case H5G_RESERVED_7: return "Reserved 7";
   }
   return "unknown-unknown:)";
}

constexpr char const* to_string(H5T_class_t cls) noexcept {
   switch (cls) {
      case H5T_NO_CLASS: return "H5T_NO_CLASS"; //<error        
      case H5T_INTEGER: return "H5T_INTEGER"; //<integertypes         
      case H5T_FLOAT: return "H5T_FLOAT"; //<floating-pointtypes
      case H5T_TIME: return "H5T_TIME"; //<dateandtimetypes
      case H5T_STRING: return "H5T_STRING"; //<characterstringtypes
      case H5T_BITFIELD: return "H5T_BITFIELD"; //<bitfieldtypes
      case H5T_OPAQUE: return "H5T_OPAQUE"; //<opaquetypes
      case H5T_COMPOUND: return "H5T_COMPOUND"; //<compoundtypes
      case H5T_REFERENCE: return "H5T_REFERENCE"; //<referencetypes
      case H5T_ENUM: return "H5T_ENUM"; //<enumerationtypes
      case H5T_VLEN: return "H5T_VLEN"; //<variable-Lengthtypes
      case H5T_ARRAY: return "H5T_ARRAY"; //<arraytypes        
   }
   return "<unknown type class>";
}

}/*inline namespace v0_1*/} // namespace h5x


