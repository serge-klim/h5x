#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/stacktrace.hpp>
#include <filesystem>
#include <format>
#include <thread>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <csignal>
#include <cassert>


//namespace io = boost::asio;
void setup_common_options(boost::program_options::options_description& /*description*/);
//void run(gaz::AppContext&);
//
namespace {

//void critical_error_handler(int signal)
//{
//   std::signal(signal, SIG_DFL);
//   boost::stacktrace::safe_dump_to("./backtrace.dump");
////   std::signal(signal, SIGABORT);
//}
//
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

//#include "spdlog/sinks/stdout_color_sinks.h"
//#include "spdlog/sinks/rotating_file_sink.h"
//void extra_logging()
//{
//   auto console = spdlog::stdout_color_mt("console");
//   auto rotating_logger1 = spdlog::rotating_logger_mt("rotating_txt", "logs/rotating.txt", 1048576 * 5, 3);
//   //auto rotating_logger2 = spdlog::rotating_logger_mt("rotating_txt", "logs/rotating.txt", 1048576 * 5, 3);
//   spdlog::set_default_logger(console);
//}
//
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
      //boost::program_options::positional_options_description positional;
      //positional.add("config", -1);

      boost::program_options::variables_map vm;
      store(boost::program_options::command_line_parser(
                argc, argv)
                .options(description) /*.positional(positional)*/.run(),
            vm);

      if (vm.count("help") != 0) {
         help(std::cout, description);
         return 0;
      }
      assert(argc > 0 && "oops! that is totally unexpected!!!");
      if(!load_config(argv[0], vm))
         notify(vm); 
      std::ostream& dump(std::filesystem::path const&, std::string const&, std::ostream&);
      //dump("C:/Users/serge/Documents/projects/hdf5/build.lin/tools/test/h5diff/testfiles/h5diff_hyper1.h5", "/big", std::clog) << std::endl;
      dump("C:/Users/serge/Documents/projects/hdf5/build.lin/tools/test/h5repack/testfiles/h5repack_nested_8bit_enum.h5", "/tracks/1/trace", std::clog) << std::endl;
      dump("C:/Users/serge/Documents/projects/hdf5/build.lin/tools/test/h5diff/testfiles/compounds_array_vlen1.h5", "/dset1", std::clog) << std::endl;
      dump("c:/Users/serge/Documents/projects/hdf5/tools/testfiles/tscalarstring.h5", "/the_str", std::clog) << std::endl;
      dump("c:/Users/serge/Documents/projects/hdf5/tools/testfiles/tstring.h5", "/dset1", std::clog) << std::endl;

//      extra_logging();
//      std::signal(SIGSEGV, &critical_error_handler);
//      std::signal(SIGABRT, &critical_error_handler);
//
//      {
//         auto ctx = gaz::AppContext{std::move(vm)};
//         auto work_guard = io::executor_work_guard{ctx.io_context().get_executor()};
//         auto threads = make_thread_pool(ctx);
//         auto signals = boost::asio::signal_set{ctx.io_context(), SIGINT};
//         signals.async_wait([&ctx](boost::system::error_code /*ec*/, int /*signo*/) {
//            spdlog::info("shuting down...");
//            ctx.stop();
//         });
//         run(ctx);         
//         //std::this_thread::sleep_for(std::chrono::seconds{100});
//         //spdlog::info("...stoping");
//         //ctx.stop();
//         std::for_each(begin(threads), end(threads), std::mem_fn(&std::thread::join));
//      }
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



