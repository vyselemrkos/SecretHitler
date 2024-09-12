#include "secret_hitler.hpp"

extern "C"
{
    __declspec(dllexport) SH_game * create_game()
    {
        return new SH_game;
    }

    __declspec(dllexport) void add_players(SH_game * game_instance, char * new_player)
    {
        std::cout<<"new player: "<<new_player<<std::endl;
        game_instance->add_player(new_player);
    }
    
    __declspec(dllexport) void print(SH_game * game_instance)
    {
        game_instance->print();
    }
}