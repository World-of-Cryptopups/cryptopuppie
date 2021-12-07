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
    ACTION teamreset(name player, uint64_t teamid);
    ACTION advteamreset(name player, uint64_t advent_teamid);
    ACTION play(name player, uint32_t game_level, uint32_t team_num);
    ACTION playattack(name player);

    // the only way for players to transfer their nft is by creating teams / staking it
    [[eosio::on_notify("atomicassets::transfer")]] void manageasset(name from, name to, vector<uint64_t> asset_ids, string memo);

   private:
    /**
     * This is where the game data and configuration is stored.
     * Only admins can modify this.
    */
    TABLE gamelevels_s {
        uint32_t level;  // game level, start with level 1
        string map;      // this is an ipfs image hash of the level map
        string boss;     // this is an ipfs image hash of the boss
        uint32_t sec;    // estimated time till done

        auto primary_key() const { return (uint64_t)level; };
    };

    /**
     * Player profile.
    */
    TABLE profiles_s {
        name player;
        uint32_t player_level;        // current player level, it is different from current_game_level
        uint32_t xp;                  // experience points
        uint32_t current_game_level;  // this is to limit the playing level
        uint32_t energy;              // find ways to handle this properly

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
    TABLE adventure_teams_s {
        name player;
        vector<uint64_t> pups;

        auto primary_key() const { return player.value; };
    };

    /**
     * A data is stored if player will play.
     * This is used as basis for receiving rewards.
    */
    TABLE games_s {
        uint64_t gameid;
        name player;
        uint32_t game_level;
        uint32_t team_num;
        uint32_t end_time;
        bool success;  // could be failed due to sudden browser exit
    };

    typedef multi_index<name("gamelevels"), gamelevels_s> gamelevels_t;

    typedef multi_index<name("teams"), teams_s> teams_t;
    typedef multi_index<name("adventteam"), adventure_teams_s> adventure_teams_t;
    typedef multi_index<name("profiles"), profiles_s> profiles_t;

    typedef multi_index<name("games"), games_s> games_t;

    gamelevels_t gamelevels = gamelevels_t(_self, _self.value);
    adventure_teams_t adventure_teams = adventure_teams_t(_self, _self.value);

    // scope teams by each player for easier access
    teams_t get_teams(name player) {
        return teams_t(_self, player.value);
    }
};
