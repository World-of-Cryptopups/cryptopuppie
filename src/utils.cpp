#include <cryptopuppie.hpp>

/**
 * Checks if the game / system is in maintenance mode.
*/
void cryptopuppie::check_maintenance() {
    auto _sysconfig = sysconfig.get();
    check(!_sysconfig.maintenance, "System is currently in maintenance mode.");
}