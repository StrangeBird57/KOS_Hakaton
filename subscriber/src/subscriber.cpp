#include <iostream>
#include <string_view>
#include "general.h"
#include "subscriber.h"
#include "gpio.h"
#include "myparcer.h"

using namespace std::literals;

constexpr auto Topic = "MGOTU/Command"sv;

/*  active save mode with authentication (directory Security) */

//const char* user = "mysender";
//const char* passwd = "$6$+LQsO+QVu1cfhKdq$LpSsTzJZtdLmlWrcwz1WRwN1eZ3bX8woYPbiUia30pqhnoO0pnz2rLucfo141TrFvGjj/P4CJHWccgh06G9w8A==";


Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    /*  active save mode with authentication*/

    //   username_pw_set(user,passwd);
    //   std::cout << user << " : " << passwd << std::endl;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::string my_command;             
        my_command = payload;
        

		/*  parcer command */

		parcermain(my_command);
    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
