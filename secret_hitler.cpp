#include "secret_hitler.hpp"

#include <algorithm>
#include <numeric>

#define default_faschist_policy 11
#define default_liberal_policy 6
#define default_policy_count (default_liberal_policy + default_faschist_policy)


SH_game::SH_game()
{
    /* Initialize policy cards. */
    for(int i=0; i<default_faschist_policy; i++)
    {
        this->policies.push_back(sh_policy_faschist);
    }
    
    for(int i=0; i<default_liberal_policy; i++)
    {
        this->policies.push_back(sh_policy_liberal);
    }

    this->discarded_policies.clear();
    this->players.clear();
    
    /* Shuffle policy cards. */
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    this->random_seed=e;
    std::shuffle(this->policies.begin(), this->policies.end(), this->random_seed);

    /* Initialize game state */
    this->game_state = sh_game_state_initializing;
    
}

SH_error_code SH_game::add_player(std::string g_p_name)
{
    if( sh_game_state_initializing == this->game_state )
    {
        if(!g_p_name.empty())
        {
            SH_player new_player;
            new_player.p_name = g_p_name;
            new_player.p_party = sh_party_undefined;
            new_player.p_role = sh_role_undefined;

            players.push_back(new_player);
            return sh_err_no_error;
        }
        else
        {
            return sh_err_invalid_name;
        }
    }
    else
    {
        return sh_err_invalid_game_state;
    }
}

SH_error_code SH_game::remove_player(std::string g_p_name)
{
    if( sh_game_state_initializing == this->game_state )
    {
        if(!g_p_name.empty())
        {
            auto it = std::find_if(this->players.begin(), this->players.end(), [&](const SH_player& player_to_be_kicked) {
                return player_to_be_kicked.p_name == g_p_name;
                });
            
            if (it != this->players.end()) 
            {
                this->players.erase(it);
                return sh_err_no_error;
            } 
            else
            {
                return sh_err_invalid_name;
            }
        }
        else
        {
            return sh_err_invalid_name;
        }
    }
    else
    {
        return sh_err_invalid_game_state;
    }
}

static void calc_players(int player_count, int * fasc, int * lib)
{
    if(0 == player_count % 2)
    {
        *fasc = (player_count / 2) - 1;
        *lib = player_count - *fasc; 
    }
    else if (1 == player_count % 2)
    {
        *fasc = (player_count / 2);
        *lib = player_count - *fasc; 
    }
}

SH_error_code SH_game::start_game(std::vector<std::string>* ret_fasc, std::vector<std::string>* ret_lib)
{
    SH_error_code return_value;
    if( sh_game_state_initializing == this->game_state )
    {
        if(4 < this->players.size() && 11 > this->players.size())
        {
            /* select faschists and liberals */
            int lib_count, fasc_count;
            calc_players(this->players.size(), &lib_count, &fasc_count);

            std::vector<int> rand_player_index(this->players.size());
            iota(rand_player_index.begin(), rand_player_index.end(), 0);

            std::shuffle(rand_player_index.begin(), rand_player_index.end(), this->random_seed);

            //first one is Hitler, other <fasc_count - 1> ones are faschists and other <lib_count> ones are liberals
            for( int i = 0; i < this->players.size(); i++)
            {
                if (0 == i)
                {
                    this->players[rand_player_index[i]].p_party=sh_party_faschist;
                    this->players[rand_player_index[i]].p_role=sh_role_Hitler;
                    ret_fasc->push_back(this->players[rand_player_index[i]].p_name);
                }
                else if(0 < i && fasc_count>i)
                {
                    this->players[rand_player_index[i]].p_party=sh_party_faschist;
                    this->players[rand_player_index[i]].p_role=sh_role_faschist;
                    ret_fasc->push_back(this->players[rand_player_index[i]].p_name);
                }
                else
                {
                    this->players[rand_player_index[i]].p_party=sh_party_liberal;
                    this->players[rand_player_index[i]].p_role=sh_role_liberal;
                    ret_lib->push_back(this->players[rand_player_index[i]].p_name);
                }
            }
            this->current_president=0;
            game_state = sh_game_state_select_chancellor;
           
        }
        else
        {
            return_value = sh_err_invalid_player_size;
        }
    }
    else
    {
        return_value = sh_err_invalid_game_state;
    }
    return return_value;
}

#ifdef TEST
void SH_game::print()
{
    std::cout<<"Card deck:"<<std::endl;
    for(int i=0;i<default_policy_count; i++)
    {
        std::cout<<this->policies[i]<<", ";
    }
    std::cout<<std::endl;

    std::cout<<"Players:"<<std::endl;
    for(int i=0;i<players.size(); i++)
    {
        std::cout << this->players[i].p_name <<", ";
    }
    std::cout<<std::endl;
}
#endif