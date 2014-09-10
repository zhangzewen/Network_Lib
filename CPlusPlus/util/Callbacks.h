#ifndef __NETWORKLIB_UTIL_CALLBACKS_HPP_INCLUDED_
#define __NETWORKLIB_UTIL_CALLBACKS_HPP_INCLUDED_

#include <boost/function.hpp>

namespace NetworkLib{
	typedef boost::function<void()> ReadEventCallback;
	typedef boost::function<void()> WriteEventCallback;
	typedef boost::function<void()> ErrorEventCallback;
	typedef boost::function<void()> TimeoutEventCallback;
}

#endif


