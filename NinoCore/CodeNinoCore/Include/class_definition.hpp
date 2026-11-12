#pragma once

#define NINO_NONCOPYABLE(Class)\
	Class(const Class&) = delete; \
	Class& operator=(const Class&) = delete; \
	Class(Class&&) = default; \
	Class& operator=(Class&&) = default;