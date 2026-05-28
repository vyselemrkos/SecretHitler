#ifndef SECRET_HITLER_BOARD_HPP
#define SECRET_HITLER_BOARD_HPP

#include <vector>
#include "secret_hitler_utils.hpp"

class SH_board
{
    private:
        int sh_board_fascist_policy_count;
        int sh_board_liberal_policy_count;
        std::vector<SH_game_state> fascist_abilities;
        std::vector<SH_game_state> liberal_abilities;
    public:
        SH_board(int player_count);
        SH_game_state enact_policy(SH_policy policy);
        SH_game_state check_win() const;
        int get_fascist_count() const;
        int get_liberal_count() const;
        bool is_Hitler_zone() const;
};

#endif
