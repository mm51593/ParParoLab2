#ifndef INPUT_METHOD_IN
#define INPUT_METHOD_IN

/*#include "game.hpp"
#include "player.hpp"*/

class Game;
class Player;

class InputMethod
{
    protected:
        Game *game = nullptr;
        Player *player = nullptr;

    public:
        virtual int return_input() = 0;
        void set_game(Game *game)
        {
            this->game = game;
        }
        void set_player(Player *player)
        {
            this->player = player;
        }
};

#endif