
#include "secret_hitler.hpp"

int main()
{
    SH_game game_instance;
    game_instance.add_player("semih");
    game_instance.add_player("arif");
    game_instance.add_player("bugra");
    game_instance.add_player("demokan");
    game_instance.add_player("ferdi");
    game_instance.print();
    game_instance.remove_player("semih");
    game_instance.print();

}