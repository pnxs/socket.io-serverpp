#pragma once

#include <uuid/uuid.h>

namespace SOCKETIO_SERVERPP_NAMESPACE
{
namespace lib
{

namespace uuid
{
    static string uuid1()
    {
        uuid_t uuid;
        char strId[38];
        uuid_generate_time(uuid);
        uuid_unparse(uuid, strId);
        strId[37] = 0;
        return strId;
    }
}

}
}
