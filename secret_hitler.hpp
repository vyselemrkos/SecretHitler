#ifndef SECRET_HITLER_H
#define SECRET_HITLER_H

#include <iostream>

typedef enum SH_role
{
    sh_role_liberal = 0,
    sh_role_faschist,
    sh_role_max
}SH_role;

typedef enum SH_special_role
{
    sh_special_role_liberal = 0,
    sh_special_role_faschist,
    sh_special_role_Hitler,
    sh_special_role_max
}SH_special_role;

typedef enum SH_policy
{
    sh_policy_liberal = 0,
    sh_policy_faschist,
    sh_policy_max
}SH_special_role;

class SH_player
{
    private:
        SH_role p_role;
        SH_special_role special_p_role;
        std::string p_name;
    public:
        SH_player(std::string);
};

class SH_game
{

};


#endif /*SECRET_HITLER_H*/