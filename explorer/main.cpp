#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/stacktrace.hpp>
#include <filesystem>
#include <format>
#include <thread>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <csignal>
#include <cassert>

namespace {

void setup_common_options(boost::program_options::options_description& description)
{
   // clang-format off
   description.add_options()
       ("filename,f", boost::program_options::value<std::vector<std::filesystem::path>>()->multitoken()->required(), "\thdf5 file to chunk")
       ;
   // clang-format on
}

std::filesystem::path config_path(std::filesystem::path app_path)
{
   app_path += ".config";
   auto local = std::filesystem::current_path() / app_path.filename();
   return std::filesystem::is_regular_file(local) ? local : app_path;
}

bool load_config(char const* app_name, boost::program_options::variables_map& vm)
{
   auto res = false;
   auto specified = vm.count("config") !=0 ;
   auto config_filename = specified ? vm["config"].as<std::string>() : config_path(app_name).string();
   if (auto ifs = std::ifstream{config_filename}) {
      auto description = boost::program_options::options_description{"configuration"};
      setup_common_options(description);

      boost::program_options::options_description config_file_options;
      config_file_options.add(description);
      auto parsed = parse_config_file(ifs, config_file_options, true);
      store(parsed, vm);

//      logging::load_syncs_from_unrecognized_program_options(collect_unrecognized(parsed.options, boost::program_options::include_positional));

      notify(vm); // check config file options sanity
      res = true;
   } else if (specified)
      throw std::runtime_error{std::format("can't open configuration file \"%{}%\"", config_filename)};
   return res;
}

std::ostream& help(std::ostream& out, boost::program_options::options_description& description)
{
   // clang-format off
   out << description << "\neg:\n"
                         "runner ... TODO!";
   // clang-format on
   return out;
}
} // namespace


int main(int argc, char* argv[])
{
   auto description = boost::program_options::options_description{"options"};
   try {
      // clang-format off
        description.add_options()
            ("help,h", "\tprint usage message")
            ("config,c", boost::program_options::value<std::string>())
            ;
      // clang-format on
      setup_common_options(description);
      boost::program_options::positional_options_description positional;
      positional.add("filename", -1);

      boost::program_options::variables_map vm;
      store(boost::program_options::command_line_parser(
                argc, argv)
                .options(description).positional(positional).run(),
            vm);

      if (vm.count("help") != 0) {
         help(std::cout, description);
         return 0;
      }
      assert(argc > 0 && "oops! that is totally unexpected!!!");
      if(!load_config(argv[0], vm))
         notify(vm); 
      
      std::ostream& dump(std::filesystem::path const& filename, std::ostream& out);
      auto filenames = vm["filename"].as<std::vector<std::filesystem::path>>();
      for (auto const& filename : filenames)
         dump(filename, std::clog) << "\n\n\n";

      std::cout << "...done!" << std::endl;
   } catch (boost::program_options::error& e) {
      std::cerr << "error : " << e.what() << "\n\n";
      help(std::cerr, description);
   } catch (std::exception& e) {
      std::cerr << "error : " << e.what() << std::endl;
   } catch (...) {
      std::cerr << "miserably failed:(" << std::endl;
   }
   return 0;
}



