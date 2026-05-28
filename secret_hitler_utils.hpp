#ifndef SECRET_HITLER_UTILS
#define SECRET_HITLER_UTILS

typedef enum SH_error_code
{
    sh_err_not_initialized = -1,
    sh_err_no_error,
    sh_err_invalid_name,
    sh_err_invalid_game_state,
    sh_err_invalid_player_size  
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
    sh_game_state_president_discarding,
    sh_game_state_chancellor_discarding,
    sh_game_state_president_killing,
    sh_game_state_president_investigating_roles,
    sh_game_state_president_examining_cards,
    sh_game_state_liberal_win,
    sh_game_state_faschist_win,
    sh_game_state_max
}SH_game_state;

typedef enum SH_policy
{
    sh_policy_undefined = -1,
    sh_policy_liberal,
    sh_policy_faschist,
    sh_policy_max
}SH_policy;

typedef enum SH_player_status
{
    sh_player_status_undefined = -1,
    sh_player_status_active,
    sh_player_status_pasive,
    sh_player_status_max
}SH_player_status;

#endif
