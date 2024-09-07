#include "secret_hitler.hpp"

#include <algorithm>
#include <random>

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
    auto rng = std::default_random_engine {};
    std::shuffle(this->policies.begin(), this->policies.end(), rng);

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