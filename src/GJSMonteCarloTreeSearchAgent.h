#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "utils.h"
#include "Stopwatch.h"
#include "MonteCarloTreeSearchAgent.h"

template <class Game>
class GJSMonteCarloTreeSearchAgent {
public:
  GJSMonteCarloTreeSearchAgent() : exploration_rate(2), iteration_limit(100) { }
  using TreeNodePtr = TreeNode<Game>*;

  typename Game::Action GetAction(const Game& state) {
   auto root_node = std::unique_ptr<TreeNode<Game>>(new TreeNode<Game>(state, true, nullptr, typename Game::Action(), exploration_rate));
   search_tree = root_node.get();
   nodes.clear();
   nodes.push_back(std::move(root_node));

   SearchForIterations(iteration_limit);

   double best_value = 0;
   typename Game::Action best_action;
   for(auto const& child : search_tree->children) {
     double value = 0.5 + 0.5 * child->stats.wins/child->stats.plays;
     if(value >= best_value) {
       best_value = value;
       best_action = child->action;
     }
     /*
     std::cout << to_string(child->action) << " " << child->stats.plays << " " <<
         0.5 + (0.5 * child->stats.wins) / child->stats.plays << std::endl;
     */
   }

   return best_action;
  }


  void Reset() { }

  void SetIterationLimit(size_t iterations) {
    iteration_limit = iterations;
  }

  void SetExplorationRate(float rate) {
    exploration_rate = rate;
  }

private:
  void SearchForTime(double ms) {
   const int batch_size = 1000;
   Stopwatch sw;
   sw.Start();
   int iterations = 0;
   while(sw.ElapsedMillis() < ms) {
    SearchForIterations(batch_size);
    iterations += batch_size;
   }
   sw.Stop();
  }

  void SearchForIterations(int n) {
    for(int i = 0; i < n; i++) {
       MonteCarloTreeSearch(search_tree);
     }   
  }

  TreeNodePtr Selection(TreeNodePtr node) {
    if(node == nullptr) {
      return node;
    }
  
    //check for unexplored actions
    size_t num_unexplored_actions = node->unexplored_actions.size();
    if(num_unexplored_actions != 0) {
      return node;
    }

    //treat as bandit problem
    double best_value = -std::numeric_limits<double>::infinity();
    TreeNodePtr best_child = nullptr;
    for(auto const& child : node->children) {
      double ucb = child->UpperConfidenceBound();
      if(ucb > best_value) {
        best_value = ucb;
        best_child = child;
      }
    }
    
    if(!best_child) {
     int score = GetScore(node);
     Backpropagation(node, score);
     return nullptr;
    }
     
    return Selection(best_child); 
  }

  TreeNodePtr Expansion(TreeNodePtr node) {
    if (node->state.GameOver()) {
      return node;
    }
    auto action = node->unexplored_actions.back();
    node->unexplored_actions.pop_back();
    Game next_state = node->state.ForwardModel(action);
    
    auto child_node = std::unique_ptr<TreeNode<Game>>(new TreeNode<Game>(next_state, !node->our_turn, node, action, exploration_rate));
    node->children.push_back(child_node.get());
    nodes.push_back(std::move(child_node));
    return node->children.back();
  }

  int Simulation(TreeNodePtr node) {
    Game simulated_game = node->state;
    bool our_turn = node->our_turn;
    
    while(!simulated_game.GameOver()) {
      auto actions = simulated_game.GetAvailableActions();
      simulated_game.ApplyAction(*select_randomly(actions.begin(), actions.end()));
      our_turn = !our_turn;
    }

    int score;
    if(simulated_game.Draw()) {
      score = 0;
    } else {
      if (our_turn == node->our_turn) {
        score = +1;
      } else {
        score = -1;
      }
    }
    return score;
  }

  int GetScore(TreeNodePtr const& node) {
    int score;
    if(node->state.Draw()) {
      score = 0;
    } else {
      score = 1;
    }
    return score;
  }

  void Backpropagation(TreeNodePtr node, int score) {
    node->stats.plays++;
    node->stats.wins += score;

    if(node->parent) {
      // Invert the score after every iteration
      Backpropagation(node->parent, -score);
    }
  }

  void MonteCarloTreeSearch(TreeNodePtr node) {
    TreeNodePtr unexpanded_child = Selection(node);

    if(unexpanded_child) {
      TreeNodePtr expanded_node = Expansion(unexpanded_child);
      double reward = Simulation(expanded_node);
      Backpropagation(expanded_node, reward);
    }
  }

  size_t iteration_limit;
  float exploration_rate;
  std::vector<std::unique_ptr<TreeNode<Game>> > nodes;
  TreeNodePtr search_tree;
};
