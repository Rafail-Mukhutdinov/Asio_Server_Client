#ifndef COMMON_INCLUDE_FILE
#define COMMON_INCLUDE_FILE


#define LOGGING_SOURCES(a,b) logging_sources((a),(b),__FILE__, __FUNCTION__,__LINE__)

#endif

#define LOGGING_INIT() init_logging()

//#define BOOST_LOG_DYN_LINK 1
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_LOG_WITHOUT_EVENT_LOG


#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;



 enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

inline std::ostream& operator<< (std::ostream& strm, severity_level level)
{
    static const char* strings[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

inline void init_logging()
{
    boost::shared_ptr< logging::core > core = logging::core::get();
  
    boost::shared_ptr< sinks::text_ostream_backend > backend =boost::make_shared< sinks::text_ostream_backend >();
    //backend->add_stream(boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));
    backend->add_stream(boost::shared_ptr< std::ostream >(new std::ofstream("server.log")));
   
    backend->auto_flush(true);

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t(backend)); 
    core->add_sink(sink);
}



inline void logging_sources(enum severity_level _enum, const std::string text, const std::string& _file, const std::string& _func, unsigned _line)
{
   
    boost::filesystem::path p(_file);    
    auto _filepars = p.filename().string();
    src::severity_logger< severity_level > slg; 
    BOOST_LOG_SEV(slg, _enum) << "["<< _enum << "] " << "(" << _func << "()) "<<_filepars << "(" << _line << "): " << text;
}