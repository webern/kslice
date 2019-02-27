// Copyright (c) Matthew James Briggs

#pragma once

#ifndef EASYXML_XML_FORWARD_DECLARE_ELEMENT
#define EASYXML_XML_FORWARD_DECLARE_ELEMENT \
namespace ezxml \
{ \
	class XElement; \
	using XElementPtr ## Ptr = std::shared_ptr<XElement>; \
} \

#endif
