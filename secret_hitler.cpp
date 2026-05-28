#include "secret_hitler.hpp"

#include <algorithm>
#include <numeric>

#define default_faschist_policy 11
#define default_liberal_policy 6
#define default_policy_count (default_liberal_policy + default_faschist_policy)

SH_game::SH_game()
{
    for(int i = 0; i < default_faschist_policy; i++)
        this->policy_deck.push_back(sh_policy_faschist);
    for(int i = 0; i < default_liberal_policy; i++)
        this->policy_deck.push_back(sh_policy_liberal);

    this->discarded_policies.clear();
    this->players.clear();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    this->random_seed = e;
    std::shuffle(this->policy_deck.begin(), this->policy_deck.end(), this->random_seed);

    this->game_state = sh_game_state_initializing;
    this->current_president = -1;
    this->current_chancellor = -1;
    this->chancellor_candidate = -1;
    this->bypass_count = 0;
    this->board = nullptr;
}

SH_game::~SH_game()
{
    delete board;
}

SH_error_code SH_game::add_player(std::string g_p_name)
{
    if(sh_game_state_initializing != this->game_state)
        return sh_err_invalid_game_state;
    if(g_p_name.empty())
        return sh_err_invalid_name;

    SH_player new_player;
    new_player.p_name = g_p_name;
    new_player.p_party = sh_party_undefined;
    new_player.p_role = sh_role_undefined;
    new_player.p_status = sh_player_status_undefined;
    players.push_back(new_player);
    return sh_err_no_error;
}

SH_error_code SH_game::remove_player(std::string g_p_name)
{
    if(sh_game_state_initializing != this->game_state)
        return sh_err_invalid_game_state;
    if(g_p_name.empty())
        return sh_err_invalid_name;

    auto it = std::find_if(this->players.begin(), this->players.end(),
        [&](const SH_player& p) { return p.p_name == g_p_name; });

    if(it == this->players.end())
        return sh_err_invalid_name;

    this->players.erase(it);
    return sh_err_no_error;
}

static void calc_players(int player_count, int* fasc, int* lib)
{
    if(0 == player_count % 2)
    {
        *fasc = (player_count / 2) - 1;
        *lib = player_count - *fasc;
    }
    else
    {
        *fasc = (player_count / 2);
        *lib = player_count - *fasc;
    }
}

SH_error_code SH_game::start_game(std::vector<std::string>* ret_fasc, std::vector<std::string>* ret_lib)
{
    if(sh_game_state_initializing != this->game_state)
        return sh_err_invalid_game_state;
    if(this->players.size() <= 4 || this->players.size() >= 11)
        return sh_err_invalid_player_size;

    int fasc_count, lib_count;
    calc_players((int)this->players.size(), &fasc_count, &lib_count);

    std::vector<int> rand_player_index(this->players.size());
    iota(rand_player_index.begin(), rand_player_index.end(), 0);
    std::shuffle(rand_player_index.begin(), rand_player_index.end(), this->random_seed);

    for(int i = 0; i < (int)this->players.size(); i++)
    {
        this->players[rand_player_index[i]].p_status = sh_player_status_active;
        if(0 == i)
        {
            this->players[rand_player_index[i]].p_party = sh_party_faschist;
            this->players[rand_player_index[i]].p_role = sh_role_Hitler;
            ret_fasc->push_back(this->players[rand_player_index[i]].p_name);
        }
        else if(fasc_count > i)
        {
            this->players[rand_player_index[i]].p_party = sh_party_faschist;
            this->players[rand_player_index[i]].p_role = sh_role_faschist;
            ret_fasc->push_back(this->players[rand_player_index[i]].p_name);
        }
        else
        {
            this->players[rand_player_index[i]].p_party = sh_party_liberal;
            this->players[rand_player_index[i]].p_role = sh_role_liberal;
            ret_lib->push_back(this->players[rand_player_index[i]].p_name);
        }
    }

    delete board;
    board = new SH_board((int)this->players.size());

    std::uniform_int_distribution<int> pres_dist(0, (int)this->players.size() - 1);
    this->current_president = pres_dist(this->random_seed);
    this->current_chancellor = -1;
    this->chancellor_candidate = -1;
    this->bypass_count = 0;
    this->player_votes.assign(this->players.size(), -1);

    game_state = sh_game_state_select_chancellor;
    return sh_err_no_error;
}

int SH_game::find_player(const std::string& name) const
{
    for(int i = 0; i < (int)players.size(); i++)
        if(players[i].p_name == name) return i;
    return -1;
}

void SH_game::draw_policies(int count)
{
    if((int)policy_deck.size() < count)
    {
        for(auto& p : discarded_policies)
            policy_deck.push_back(p);
        discarded_policies.clear();
        std::shuffle(policy_deck.begin(), policy_deck.end(), random_seed);
    }
    drawn_policies.clear();
    for(int i = 0; i < count && !policy_deck.empty(); i++)
    {
        drawn_policies.push_back(policy_deck.back());
        policy_deck.pop_back();
    }
}

void SH_game::advance_president()
{
    int n = (int)players.size();
    for(int i = 1; i < n; i++)
    {
        int next = (current_president + i) % n;
        if(players[next].p_status == sh_player_status_active)
        {
            current_president = next;
            return;
        }
    }
}

void SH_game::auto_enact_top_policy()
{
    if(policy_deck.empty())
    {
        for(auto& p : discarded_policies)
            policy_deck.push_back(p);
        discarded_policies.clear();
        std::shuffle(policy_deck.begin(), policy_deck.end(), random_seed);
    }
    if(!policy_deck.empty())
    {
        SH_policy p = policy_deck.back();
        policy_deck.pop_back();
        policy_deck_on_the_board.push_back(p);
        board->enact_policy(p); // abilities not triggered on chaos enactment
    }
}

SH_error_code SH_game::select_chancellor(std::string president_name, std::string chancellor_name)
{
    if(game_state != sh_game_state_select_chancellor)
        return sh_err_invalid_game_state;

    int pres_idx = find_player(president_name);
    if(pres_idx < 0)
        return sh_err_invalid_name;
    if(pres_idx != current_president)
        return sh_err_invalid_game_state;

    int chanc_idx = find_player(chancellor_name);
    if(chanc_idx < 0)
        return sh_err_invalid_name;
    if(players[chanc_idx].p_status != sh_player_status_active)
        return sh_err_invalid_game_state;
    if(chanc_idx == current_president)
        return sh_err_invalid_game_state;

    chancellor_candidate = chanc_idx;
    player_votes.assign(players.size(), -1);
    game_state = sh_game_state_voting;
    return sh_err_no_error;
}

SH_error_code SH_game::cast_vote(std::string player_name, bool vote_ja)
{
    if(game_state != sh_game_state_voting)
        return sh_err_invalid_game_state;

    int idx = find_player(player_name);
    if(idx < 0)
        return sh_err_invalid_name;
    if(players[idx].p_status != sh_player_status_active)
        return sh_err_invalid_game_state;
    if(player_votes[idx] != -1)
        return sh_err_invalid_game_state;

    player_votes[idx] = vote_ja ? 1 : 0;

    // Wait for all active players to vote
    for(int i = 0; i < (int)players.size(); i++)
    {
        if(players[i].p_status == sh_player_status_active && player_votes[i] == -1)
            return sh_err_no_error;
    }

    int ja_count = 0, nein_count = 0;
    for(int i = 0; i < (int)players.size(); i++)
    {
        if(player_votes[i] == 1) ja_count++;
        else if(player_votes[i] == 0) nein_count++;
    }

    if(ja_count > nein_count)
    {
        current_chancellor = chancellor_candidate;

        // Hitler elected as chancellor while >= 3 fascist policies are on the board
        if(board->is_Hitler_zone() && players[current_chancellor].p_role == sh_role_Hitler)
        {
            game_state = sh_game_state_faschist_win;
            return sh_err_no_error;
        }

        draw_policies(3);
        bypass_count = 0;
        game_state = sh_game_state_president_discarding;
    }
    else
    {
        bypass_count++;
        advance_president();

        if(bypass_count >= 3)
        {
            auto_enact_top_policy();
            bypass_count = 0;

            SH_game_state win = board->check_win();
            if(win != sh_game_state_undefined)
            {
                game_state = win;
                return sh_err_no_error;
            }
        }

        game_state = sh_game_state_select_chancellor;
    }

    return sh_err_no_error;
}

SH_error_code SH_game::president_discard(std::string president_name, SH_policy policy)
{
    if(game_state != sh_game_state_president_discarding)
        return sh_err_invalid_game_state;

    int idx = find_player(president_name);
    if(idx < 0)
        return sh_err_invalid_name;
    if(idx != current_president)
        return sh_err_invalid_game_state;

    for(auto it = drawn_policies.begin(); it != drawn_policies.end(); ++it)
    {
        if(*it == policy)
        {
            drawn_policies.erase(it);
            discarded_policies.push_back(policy);
            game_state = sh_game_state_chancellor_discarding;
            return sh_err_no_error;
        }
    }

    return sh_err_invalid_game_state; // policy not in drawn set
}

SH_error_code SH_game::chancellor_discard(std::string chancellor_name, SH_policy policy)
{
    if(game_state != sh_game_state_chancellor_discarding)
        return sh_err_invalid_game_state;

    int idx = find_player(chancellor_name);
    if(idx < 0)
        return sh_err_invalid_name;
    if(idx != current_chancellor)
        return sh_err_invalid_game_state;
    if(drawn_policies.size() != 2)
        return sh_err_invalid_game_state;

    SH_policy enacted = sh_policy_undefined;
    for(auto it = drawn_policies.begin(); it != drawn_policies.end(); ++it)
    {
        if(*it == policy)
        {
            drawn_policies.erase(it);
            discarded_policies.push_back(policy);
            enacted = drawn_policies[0];
            drawn_policies.clear();
            break;
        }
    }

    if(enacted == sh_policy_undefined)
        return sh_err_invalid_game_state;

    policy_deck_on_the_board.push_back(enacted);
    SH_game_state triggered = board->enact_policy(enacted);

    SH_game_state win = board->check_win();
    if(win != sh_game_state_undefined)
    {
        game_state = win;
        return sh_err_no_error;
    }

    if(triggered != sh_game_state_undefined)
    {
        game_state = triggered;
        return sh_err_no_error;
    }

    advance_president();
    current_chancellor = -1;
    chancellor_candidate = -1;
    game_state = sh_game_state_select_chancellor;
    return sh_err_no_error;
}

void SH_game::game_state_next()
{
    advance_president();
    current_chancellor = -1;
    chancellor_candidate = -1;
    game_state = sh_game_state_select_chancellor;
}

int SH_game::get_player_count() const { return (int)players.size(); }
const char* SH_game::get_player_name(int index) const { return players[index].p_name.c_str(); }
int SH_game::get_player_role(int index) const { return (int)players[index].p_role; }
int SH_game::get_player_status(int index) const { return (int)players[index].p_status; }
int SH_game::get_player_vote(int index) const { return player_votes.empty() ? -1 : player_votes[index]; }
int SH_game::get_game_state() const { return (int)game_state; }
int SH_game::get_current_president() const { return current_president; }
int SH_game::get_current_chancellor() const { return current_chancellor; }
int SH_game::get_chancellor_candidate() const { return chancellor_candidate; }
int SH_game::get_bypass_count() const { return bypass_count; }
int SH_game::get_drawn_policy_count() const { return (int)drawn_policies.size(); }
int SH_game::get_drawn_policy(int index) const { return (int)drawn_policies[index]; }
int SH_game::get_fascist_policy_count() const { return board ? board->get_fascist_count() : 0; }
int SH_game::get_liberal_policy_count() const { return board ? board->get_liberal_count() : 0; }
bool SH_game::is_hitler_zone() const { return board ? board->is_Hitler_zone() : false; }

#ifdef TEST
void SH_game::print()
{
    std::cout << "Card deck:" << std::endl;
    for(int i = 0; i < default_policy_count; i++)
        std::cout << this->policy_deck[i] << ", ";
    std::cout << std::endl;

    std::cout << "Players:" << std::endl;
    for(int i = 0; i < (int)players.size(); i++)
        std::cout << this->players[i].p_name << ", ";
    std::cout << std::endl;
}
#endif
