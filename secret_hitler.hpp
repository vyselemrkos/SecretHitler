#ifndef SECRET_HITLER_H
#define SECRET_HITLER_H

#define TEST

#include "secret_hitler_utils.hpp"
#include "secret_hitler_board.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

typedef struct SH_player
{
    SH_party p_party;
    SH_role p_role;
    std::string p_name;
    SH_player_status p_status;
}SH_player;

class SH_game
{
    private:
        SH_game_state game_state;
        std::vector<SH_player> players;
        std::vector<SH_policy> policy_deck;
        std::vector<SH_policy> policy_deck_on_the_board;
        std::vector<SH_policy> discarded_policies;
        std::vector<SH_policy> drawn_policies;
        std::vector<int> player_votes;   // -1=not voted, 0=NEIN, 1=JA
        int current_president;
        int current_chancellor;
        int chancellor_candidate;
        int bypass_count;
        SH_board* board;
        std::default_random_engine random_seed;

        int find_player(const std::string& name) const;
        void draw_policies(int count);
        void advance_president();
        void auto_enact_top_policy();

    public:
        SH_game();
        ~SH_game();

        SH_error_code add_player(std::string);
        SH_error_code remove_player(std::string);
        SH_error_code start_game(std::vector<std::string>*, std::vector<std::string>*);
        SH_error_code select_chancellor(std::string president_name, std::string chancellor_name);
        SH_error_code cast_vote(std::string player_name, bool vote_ja);
        SH_error_code president_discard(std::string president_name, SH_policy policy);
        SH_error_code chancellor_discard(std::string chancellor_name, SH_policy policy);
        void game_state_next();

        int get_player_count() const;
        const char* get_player_name(int index) const;
        int get_player_role(int index) const;
        int get_player_status(int index) const;
        int get_player_vote(int index) const;
        int get_game_state() const;
        int get_current_president() const;
        int get_current_chancellor() const;
        int get_chancellor_candidate() const;
        int get_bypass_count() const;
        int get_drawn_policy_count() const;
        int get_drawn_policy(int index) const;
        int get_fascist_policy_count() const;
        int get_liberal_policy_count() const;
        bool is_hitler_zone() const;

#ifdef TEST
        void print();
#endif
};

#endif /*SECRET_HITLER_H*/
