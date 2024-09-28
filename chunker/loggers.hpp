#pragma once
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <cstdint>

using logger = boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string>;
BOOST_LOG_GLOBAL_LOGGER(log, logger)

//BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
void configure_default_logger(boost::program_options::variables_map const& options);

