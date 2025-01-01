#include "loggers.hpp"
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <iostream>

BOOST_LOG_GLOBAL_LOGGER_INIT(log, logger)
{
   logger log{boost::log::keywords::channel = "chunker"};
   return log;
}


void configure_default_logger(boost::program_options::variables_map const& options) {
	auto sink = boost::log::add_console_log(std::clog, boost::log::keywords::format = "%Message%");
	//auto const severity_level = options["verbose"].as<bool>() ? boost::log::trivial::trace : boost::log::trivial::info;
	//sink->set_filter(
	//	boost::log::trivial::severity >= severity_level
	//	&& boost::log::expressions::attr<std::string >("Channel") != "stats"
	//);
	if(options["verbose"].as<bool>())
		sink->set_filter(
			boost::log::trivial::severity >= boost::log::trivial::info
			|| (boost::log::expressions::attr<std::string >("Channel") != "stats" && boost::log::trivial::severity >= boost::log::trivial::trace)
		);
	else
		sink->set_filter(
			boost::log::trivial::severity >= boost::log::trivial::info
		);
}
