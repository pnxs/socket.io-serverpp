#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

#ifdef _SOCKETIO_SERVERPP_CPP11_STL_
#define BOOST_BIND_NO_PLACEHOLDERS
#include <functional>
#include <memory>
#endif

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{
using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;

#ifdef _SOCKETIO_SERVERPP_CPP11_STL_

using std::function;
using std::bind;
using std::ref;
using namespace std::placeholders;

using std::shared_ptr;
using std::make_shared;

#else

using boost::function;
using boost::bind;
using boost::ref;

#endif

}
}
