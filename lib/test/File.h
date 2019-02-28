#pragma once
#include "Path.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

namespace kstest {
    inline std::string path( const std::string &filename ) {
        return std::string{KSCORE_TEST_DATA_DIR} + std::string{"/"} + filename;
    }

    inline std::string tempDir() {
        std::string p = boost::lexical_cast<std::string>(
            boost::filesystem::temp_directory_path() );
        boost::replace_all( p, "\"", "" );
        return p;
    }

    inline std::string tempFilepath() {
        const auto dir = boost::filesystem::temp_directory_path();
        const auto fname = boost::lexical_cast<std::string>(
            boost::uuids::random_generator()() );
        const auto path = dir / fname;
        std::string str = boost::lexical_cast<std::string>( path );
        boost::replace_all( str, "\"", "" );
        return str;
    }

} // namespace kstest