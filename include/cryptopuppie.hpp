#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT cryptopuppie : public contract {
   public:
    using contract::contract;

    // the only way for players to transfer their nft is by creating teams / staking it
    [[eosio::on_notify("atomicassets::transfer")]] void manageasset(name from, name to, vector<uint64_t> asset_ids, string memo);

   private:
    /**
    * Pup teams created by each player.
    * Scoped by each player.
   */
    TABLE teams_s {
        uint64_t teamid;
        name player;  // for safety
        vector<uint64_t> pups;

        auto primary_key() const { return teamid; };
    };

    typedef multi_index<name("teams"), teams_s> teams_t;

    teams_t get_teams(name player) {
        return teams_t(_self, player.value);
    }
};
