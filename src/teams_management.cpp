#include <cryptopuppie.hpp>

/**
 * Handle asset transfers in here.
*/
[[eosio::on_notify("atomicassets::transfer")]] void cryptopuppie::manageasset(name from, name to, vector<uint64_t> asset_ids, string memo) {
    if (to != get_self()) return;

    check(memo.find("create_team:") == 0 || memo == "adventure_team", "Invalid memo.");

    if (memo == "adventure_team") {
        // create an adventure team
    }

    // creating a pup team

    // a team can only be composed of 5 pups
    check(asset_ids.size() == 5, "Allowed pups per team is currently 4.");

    // parse team number
    auto team_num = stoi(memo.substr(12));

    // 3 teams is only allowed
    check(team_num >= 1 && team_num <= 3, "Invalid team number name.");

    return;
}