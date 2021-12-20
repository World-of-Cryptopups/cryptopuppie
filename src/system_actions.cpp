#include <cryptopuppie.hpp>

ACTION cryptopuppie::initsys() {
    require_auth(_self);

    sysconfig.get_or_create(_self, sysconfig_s{});
}