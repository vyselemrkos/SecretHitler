
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
    //game_instance.remove_player("semih");
    game_instance.print();
    
    std::vector<std::string> fasc, lib;
    game_instance.start_game(&fasc, &lib);

    std::cout<<"faschists: ";
    for (int i=0;i<fasc.size();i++)
    {
        std::cout<<fasc[i]<<", ";
    }

    std::cout<<std::endl<<"libs: ";
    for (int i=0;i<lib.size();i++)
    {
        std::cout<<lib[i]<<", ";
    }

}