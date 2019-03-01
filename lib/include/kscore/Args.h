#pragma once

#include <cstdint>
#include <string>

namespace kscore {

    struct Args {
      public:
        Args( int argc, char *argv[] );
        Args( bool isHelp, std::string input, std::string output, int x,
              int y );
        bool isValid() const;
        std::string error() const;
        std::string help() const;
        bool isHelp() const;
        const std::string &input() const;
        const std::string &output() const;
        int x() const;
        int y() const;

      private:
        std::string myError;
        bool myIsHelp;
        std::string myInput;
        std::string myOutput;
        int myX;
        int myY;
    };
} // namespace kscore