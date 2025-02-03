#include "data_sink.hpp"
#include <stdexcept>

namespace {
constexpr auto rank() noexcept { return 1; }
} // namespace

std::size_t h5x::v0_1::detail::chunk_size(H5::DSetCreatPropList const& props) {
   hsize_t chunk_size;
   if (H5Pget_chunk(props.getId(), 1 /*oly single dimension supported at this moment*/, &chunk_size) != 1)
      throw std::runtime_error{"single dimension chunked storage layout expected"};
   //{
   //   chunk_size = static_cast<hsize_t>(default_chunk_size());
   //   props.setChunk(rank(), &chunk_size);
   //}
   return chunk_size;
}

H5::DataSet h5x::v0_1::detail::create_single_dim_dataset(H5::H5Location const& location, char const* name, H5::DataType const& data_type, H5::DSetCreatPropList const& props) try {
   // adjust_chunk_size(props);
   props.setFillValue(data_type, nullptr);
   hsize_t dim[rank()] = {0};
   hsize_t dim_max[rank()] = {H5S_UNLIMITED};
   auto space = H5::DataSpace{1, dim, dim_max};
   return location.createDataSet(name, data_type, space, props);
} catch (...) {
   std::throw_with_nested(std::runtime_error{std::string{"unable to create dataset: "} + name});
}

H5::DataSet h5x::v0_1::detail::create_single_dim_dataset(H5::H5Location const& location, char const* name, H5::DataType const& data_type, std::size_t chunk_size) try {
   H5::DSetCreatPropList props;
   props.setChunk(rank(), &chunk_size);
   props.setFillValue(data_type, nullptr);
   return create_single_dim_dataset(location, name, data_type, props);
} catch (...) {
   std::throw_with_nested(std::runtime_error{std::string{"unable to create dataset: "} + name});
}
