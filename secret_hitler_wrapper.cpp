#include "secret_hitler.hpp"
#include <vector>
#include <string>

#ifdef _WIN32
#  define SH_EXPORT __declspec(dllexport)
#else
#  define SH_EXPORT __attribute__((visibility("default")))
#endif

extern "C"
{
    SH_EXPORT SH_game* create_game()
    {
        return new SH_game;
    }

    SH_EXPORT void destroy_game(SH_game* game)
    {
        delete game;
    }

    SH_EXPORT int add_player(SH_game* game, const char* name)
    {
        return game->add_player(name);
    }

    SH_EXPORT int remove_player(SH_game* game, const char* name)
    {
        return game->remove_player(name);
    }

    SH_EXPORT int start_game(SH_game* game)
    {
        std::vector<std::string> fasc, lib;
        return game->start_game(&fasc, &lib);
    }

    SH_EXPORT int select_chancellor(SH_game* game, const char* president_name, const char* chancellor_name)
    {
        return game->select_chancellor(president_name, chancellor_name);
    }

    SH_EXPORT int cast_vote(SH_game* game, const char* player_name, int vote_ja)
    {
        return game->cast_vote(player_name, vote_ja != 0);
    }

    SH_EXPORT int president_discard(SH_game* game, const char* president_name, int policy)
    {
        return game->president_discard(president_name, (SH_policy)policy);
    }

    SH_EXPORT int chancellor_discard(SH_game* game, const char* chancellor_name, int policy)
    {
        return game->chancellor_discard(chancellor_name, (SH_policy)policy);
    }

    SH_EXPORT void game_state_next(SH_game* game)
    {
        game->game_state_next();
    }

    SH_EXPORT int get_player_count(SH_game* game)
    {
        return game->get_player_count();
    }

    SH_EXPORT const char* get_player_name(SH_game* game, int index)
    {
        return game->get_player_name(index);
    }

    SH_EXPORT int get_player_role(SH_game* game, int index)
    {
        return game->get_player_role(index);
    }

    SH_EXPORT int get_player_status(SH_game* game, int index)
    {
        return game->get_player_status(index);
    }

    SH_EXPORT int get_player_vote(SH_game* game, int index)
    {
        return game->get_player_vote(index);
    }

    SH_EXPORT int get_game_state(SH_game* game)
    {
        return game->get_game_state();
    }

    SH_EXPORT int get_current_president(SH_game* game)
    {
        return game->get_current_president();
    }

    SH_EXPORT int get_current_chancellor(SH_game* game)
    {
        return game->get_current_chancellor();
    }

    SH_EXPORT int get_chancellor_candidate(SH_game* game)
    {
        return game->get_chancellor_candidate();
    }

    SH_EXPORT int get_bypass_count(SH_game* game)
    {
        return game->get_bypass_count();
    }

    SH_EXPORT int get_drawn_policy_count(SH_game* game)
    {
        return game->get_drawn_policy_count();
    }

    SH_EXPORT int get_drawn_policy(SH_game* game, int index)
    {
        return game->get_drawn_policy(index);
    }

    SH_EXPORT int get_fascist_policy_count(SH_game* game)
    {
        return game->get_fascist_policy_count();
    }

    SH_EXPORT int get_liberal_policy_count(SH_game* game)
    {
        return game->get_liberal_policy_count();
    }

    SH_EXPORT int is_hitler_zone(SH_game* game)
    {
        return game->is_hitler_zone() ? 1 : 0;
    }

#ifdef TEST
    SH_EXPORT void print_game(SH_game* game)
    {
        game->print();
    }
#endif
}
