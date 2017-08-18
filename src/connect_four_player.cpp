#include <iostream>

#include "GameSession.h"
#include "ConnectFour.h"
#include "TicTacToe.h"
#include "PickRandomActionAgent.h"
#include "MinimaxAgent.h"
#include "TemporalDifferenceAgent.h"
#include "MonteCarloTreeSearchAgent.h"
#include "TestGame.h"
#include "Stopwatch.h"

int main(int argc, char* argv[])  {
    std::srand ( unsigned ( std::time(0) ) );

    int x_wins=0, o_wins=0, draws=0;
    int num_games = 1;

    ConnectFour game;
    // MinimaxAgent<ConnectFour> player1;
    MonteCarloTreeSearchAgent<ConnectFour> player1;
    player1.SetIterationLimit(1000);
    MonteCarloTreeSearchAgent<ConnectFour> player2;
    player2.SetIterationLimit(1000000);

    // game.ApplyAction({0});
    // game.ApplyAction({3});
    // game.ApplyAction({0});
    // game.ApplyAction({4});
    // game.ApplyAction({0});
    // game.ApplyAction({4});

    GameSession<ConnectFour, MonteCarloTreeSearchAgent, MonteCarloTreeSearchAgent> session(game, player1, player2);
    Stopwatch sw;

    sw.Start();
    for(int i = 0; i < num_games ; i++) {
        switch(session.PlayOnce()) {
        case ConnectFourStatus::X_WINS:
            x_wins++;
            break;
        case ConnectFourStatus::O_WINS:
            o_wins++;
            break;
        case ConnectFourStatus::DRAW:
            draws++;
            break;
        }
        session.Reset();
    }
    sw.Stop();

    std::cout << "Played "
    << num_games/sw.ElapsedMillis()*1000
    << " games per second." << std::endl;

    std::cout << "x_wins: "
    << x_wins/(float)num_games*100
    << std::endl;

    std::cout << "o_wins: "
    << o_wins/(float)num_games*100
    << std::endl;

    std::cout << "draws: "
    << draws/(float)num_games*100
    << std::endl;
}
