#include "kscore/RunProgram.h"
#include "Extract.h"
#include "Write.h"
#include "kscore/Args.h"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace kscore {

    int RunProgram( const Args &args, IFrameStrategyUPtr &&iframeStrategy, std::ostream &stdout,
                    std::ostream &stderr ) {

        if ( args.isHelp() ) {
            return 0;
        }

        if ( !iframeStrategy ) {
            throw std::runtime_error{"iframe strategy is null"};
        }

        const auto frameIndices = iframeStrategy->getIFrames();
        const auto frames = extract( args.input(), frameIndices, Size{args.x(), args.y()} );

        if ( args.output().empty() ) {
            writeFrames( frames, std::cout );
        } else {
            std::ofstream of;
            of.open( args.output().c_str(), std::ofstream::out );

            if ( !of.is_open() ) {
                stderr << "could not open the output file\n";
                return 11;
            }

            writeFrames( frames, of );
            of.close();
        }

        return 0;
    }
} // namespace kscore