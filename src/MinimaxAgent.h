#pragma once

#include <iostream>
#include <unordered_map>

#include "Stopwatch.h"

template <class Game>
class MinimaxAgent {
public:
  typename Game::Action GetAction(const Game& state) {
    if(minimax_tree.find(state.GetStateString()) == minimax_tree.end()) {
      Stopwatch sw;
      sw.Start();
      MiniMax(state, true);
      sw.Stop();
      std::cout << "Building full game tree took " << sw.ElapsedMillis() << " ms." << std::endl;
      std::cout << "Nodes after tree expansion:" << minimax_tree.size() << std::endl;
    }

    typename Game::Action best_action;
    double best_score = -10;
    for(auto const& action : state.GetAvailableActions()) {      
      Game result_of_action = state.ForwardModel(action);
      std::string state_string = result_of_action.GetStateString();
      double score = minimax_tree[state_string]; 
      if(score >= best_score) {
        best_score = score;
        best_action = action;
      }
    }
    return best_action;
  }

  double MiniMax(const Game& state, bool maximizing_player) {
   std::string state_string = state.GetStateString();
   if(state.GameOver()) {
      if(state.Draw()) {
        minimax_tree[state_string] = 0;
        return 0;
      }
      if(!maximizing_player) {
        minimax_tree[state_string] = 1;
        return 1;
      } else {
        minimax_tree[state_string] = -1;
        return -1;
      } 
    }

    if(maximizing_player) {
      double best_value = -10;
      for(auto const& action : state.GetAvailableActions()) {
        Game result_of_action = state.ForwardModel(action);
        best_value = std::max(best_value, MiniMax(result_of_action, false));
      }
      minimax_tree[state_string] = best_value;
      return best_value;
    } else {
      double best_value = 10;
      for(auto const& action : state.GetAvailableActions()) {
        Game result_of_action = state.ForwardModel(action);
        best_value = std::min(best_value, MiniMax(result_of_action, true));
      }
      minimax_tree[state_string] = best_value;
      return best_value;
    }
  }

    std::unordered_map<std::string, double> minimax_tree;
    void Reset() { }
};