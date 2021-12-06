#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT cryptopuppie : public contract {
   public:
    using contract::contract;

    // SYSTEM ACTIONS
    ACTION initgamelevel();
    ACTION setlevelmap(uint32_t level, string map);
    ACTION setlevelboss(uint32_t level, string boss);

    // USER ACTIONS
    ACTION teamreset(uint64_t teamid);

    // the only way for players to transfer their nft is by creating teams / staking it
    [[eosio::on_notify("atomicassets::transfer")]] void manageasset(name from, name to, vector<uint64_t> asset_ids, string memo);

   private:
    /**
     * This is where the game data and configuration is stored.
     * Only admins can modify this.
    */
    TABLE gamelevel_s {
        uint32_t level;  // game level, start with level 1
        string map;      // this is an ipfs image hash of the level map
        string boss;     // this is an ipfs image hash of the boss

        auto primary_key() const { return (uint64_t)level; };
    };

    /**
     * Player profile.
    */
    TABLE profile_s {
        name player;
        uint64_t level;
        uint64_t energy;  // find ways to handle this properly

        auto primary_key() const { return player.value; };
    };

    /**
     * Pup teams created by each player.
     * Scoped by each player.
    */
    TABLE teams_s {
        uint8_t teamid;
        name player;  // for safety
        vector<uint64_t> pups;

        auto primary_key() const { return (uint64_t)teamid; };
    };

    /**
     * Pups for adventure.
     * Only one team per player is allowed.
    */
    TABLE adventure_team_s {
        name player;
        vector<uint64_t> pups;

        auto primary_key() const { return player.value; };
    };

    typedef multi_index<name("gamelevels"), gamelevel_s> gamelevel_t;

    typedef multi_index<name("teams"), teams_s> teams_t;
    typedef multi_index<name("adventteam"), adventure_team_s> adventure_team_t;

    gamelevel_t gamelevels = gamelevel_t(_self, _self.value);
    adventure_team_t adventure_teams = adventure_team_t(_self, _self.value);

    // scope teams by each player for easier access
    teams_t get_teams(name player) {
        return teams_t(_self, player.value);
    }
};
