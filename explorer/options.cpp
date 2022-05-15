#include <boost/program_options.hpp>
//#include <boost/date_time/local_time/local_time.hpp>
//#include <boost/lexical_cast.hpp>
#include <string>

void setup_cmd_line_options(boost::program_options::options_description& description)
{
    // clang-format off
    description.add_options()
        ("help,h", "\tprint usage message")
        ("config,c", boost::program_options::value<std::string>())
        ;
    // clang-format on
}


void setup_common_options(boost::program_options::options_description& description)
{
   // clang-format off
   description.add_options()
       ("threads,t", boost::program_options::value<std::size_t>()->default_value(0), "\tnumber of threads in the pool")
       ("execute,e", boost::program_options::value<std::string>(), "\texecute process")
       ("shutdown,s", boost::program_options::value<unsigned int>(), "\tauto shutdown in ...seconds.")
       //("dump,d", boost::program_options::value<std::string>()->default_value(boost::lexical_cast<std::string>(boost::local_time::local_sec_clock::local_time()) + ".dump"), "\tdump file name")
//       ("source,s", boost::program_options::value<std::string>(/*&params.source*/), "\tsource")
       ;
   // clang-format on
}


