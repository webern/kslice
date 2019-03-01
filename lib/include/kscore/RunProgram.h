#pragma once
#include "IFrameStrategy.h"
#include "kscore/Args.h"
#include <cstdint>
#include <iostream>

namespace kscore {

    int RunProgram( const Args &args, IFrameStrategyUPtr &&iframeStrategy, std::ostream &stdout, std::ostream &stderr );

}