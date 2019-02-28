// Copyright (c) Matthew James Briggs

#include "ezxml/XFactory.h"
#include "src/PugiDoc.h"
#include "src/PugiElement.h"

namespace ezxml
{
    XDocPtr XFactory::makeXDoc()
    {
        return XDocPtr{ new PugiDoc{} };
    }
    
    XElementPtr XFactory::makeXElement()
    {
        return XElementPtr{ new PugiElement{} };
    }
}
