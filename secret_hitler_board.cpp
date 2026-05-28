#include "secret_hitler_board.hpp"

SH_board::SH_board(int player_count)
    : sh_board_fascist_policy_count(0), sh_board_liberal_policy_count(0)
{
    if(player_count < 5 || player_count > 10)
        player_count = 5;

    if(player_count == 5 || player_count == 6)
    {
        fascist_abilities = {
            sh_game_state_undefined,
            sh_game_state_undefined,
            sh_game_state_president_examining_cards,
            sh_game_state_president_killing,
            sh_game_state_president_killing,
            sh_game_state_faschist_win
        };
    }
    else if(player_count == 7 || player_count == 8)
    {
        fascist_abilities = {
            sh_game_state_undefined,
            sh_game_state_president_investigating_roles,
            sh_game_state_select_president,
            sh_game_state_president_killing,
            sh_game_state_president_killing,
            sh_game_state_faschist_win
        };
    }
    else
    {
        fascist_abilities = {
            sh_game_state_president_investigating_roles,
            sh_game_state_president_investigating_roles,
            sh_game_state_select_president,
            sh_game_state_president_killing,
            sh_game_state_president_killing,
            sh_game_state_faschist_win
        };
    }

    liberal_abilities = {
        sh_game_state_undefined,
        sh_game_state_undefined,
        sh_game_state_undefined,
        sh_game_state_undefined,
        sh_game_state_liberal_win
    };
}

SH_game_state SH_board::enact_policy(SH_policy policy)
{
    if(policy == sh_policy_faschist)
    {
        sh_board_fascist_policy_count++;
        return fascist_abilities[sh_board_fascist_policy_count - 1];
    }
    else if(policy == sh_policy_liberal)
    {
        sh_board_liberal_policy_count++;
        return liberal_abilities[sh_board_liberal_policy_count - 1];
    }
    return sh_game_state_undefined;
}

SH_game_state SH_board::check_win() const
{
    if(sh_board_fascist_policy_count >= 6) return sh_game_state_faschist_win;
    if(sh_board_liberal_policy_count >= 5) return sh_game_state_liberal_win;
    return sh_game_state_undefined;
}

int SH_board::get_fascist_count() const { return sh_board_fascist_policy_count; }
int SH_board::get_liberal_count() const { return sh_board_liberal_policy_count; }

bool SH_board::is_Hitler_zone() const
{
    return sh_board_fascist_policy_count >= 3;
}
