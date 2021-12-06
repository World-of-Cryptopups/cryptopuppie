#include <cryptopuppie.hpp>

/**
 * Handle asset transfers in here.
*/
[[eosio::on_notify("atomicassets::transfer")]] void cryptopuppie::manageasset(name from, name to, vector<uint64_t> asset_ids, string memo) {
}