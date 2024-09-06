#ifndef SECRET_HITLER_H
#define SECRET_HITLER_H

#include <iostream>
#include <vector>

typedef enum SH_error_code
{
    sh_err_not_initialized = -1,
    sh_err_no_error,
    sh_err_invalid_name,
    sh_err_invalid_game_state
}SH_error_code;

typedef enum SH_party
{
    sh_party_undefined = -1,
    sh_party_liberal,
    sh_party_faschist,
    sh_party_max
}SH_party;

typedef enum SH_role
{
    sh_role_undefined = -1,
    sh_role_liberal,
    sh_role_faschist,
    sh_role_Hitler,
    sh_role_max
}SH_role;

typedef enum SH_game_state
{
    sh_game_state_undefined = -1,
    sh_game_state_initializing,
    sh_game_state_select_president,
    sh_game_state_select_chancellor,
    sh_game_state_voting,
    sh_game_state_max
}SH_game_state;

typedef enum SH_policy
{
    sh_policy_undefined = -1,
    sh_policy_liberal,
    sh_policy_faschist,
    sh_policy_max
}SH_policy;

typedef struct SH_player
{
    /* Player Party */
    SH_party p_party;
    /* Player Role */
    SH_role p_role;
    /* Player Name */
    std::string p_name;
}SH_player;

class SH_game
{
    private:
        /* Game State */
        SH_game_state game_state;
        /* Players container */
        std::vector<SH_player> players;
        /* Policy Cards */
        std::vector<SH_policy> policies;
        /* Discarded Policy Cards */
        std::vector<SH_policy> discarded_policies;

    public:
        /* Constructor */
        SH_game();

        /* Add a player with given name */
        SH_error_code add_player(std::string);

        /* Remove a player with given name*/
        SH_error_code remove_player(std::string);

#ifdef TEST
        SH_error_code print();
#endif
};


#endif /*SECRET_HITLER_H*/