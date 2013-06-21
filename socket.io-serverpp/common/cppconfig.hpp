#pragma once

namespace socketio-serverpp
{
namespace lib
{

#ifdef _SOCKETIO-SERVERPP_CPP11_STL_

using std::function;
using std::bind;
using std::ref;
using namespace std::placeholders;

#else

using boost::function;
using boost::bind;
using boost::ref;

#endif

}
}
