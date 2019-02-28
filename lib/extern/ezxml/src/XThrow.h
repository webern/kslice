// Copyright (c) Matthew James Briggs

#pragma once
#include "src/Throw.h"

#define EASYXML_THROW_XNULL EASYXML_THROW( "The internal object is null. This probably means that XDoc has gone out of scope causing all of its children to be destroyed." );
