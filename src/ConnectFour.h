#pragma once

#include <string>
#include <vector>
#include <Eigen/Dense>

#define CONNECT_FOUR_NUM_ROWS 6
#define CONNECT_FOUR_NUM_COLS 7

enum class ConnectFourStatus {
  X_WINS,
  O_WINS,
  DRAW,
  IN_PROGRESS
};

std::string to_string(ConnectFourStatus status) {
  switch (status) {
    case ConnectFourStatus::X_WINS:
        return "X_WINS";
    case ConnectFourStatus::O_WINS:
        return "O_WINS";
    case ConnectFourStatus::DRAW:
        return "DRAW";
    case ConnectFourStatus::IN_PROGRESS:
        return "IN_PROGRESS";
    }
    return "UNKNOWN";
}

struct ConnectFourAction {
  int row_index;
  int column_index;
};

std::string to_string(ConnectFourAction const& action) {
    return "{"
           + std::to_string(action.row_index) + ", "
           + std::to_string(action.column_index) + "}";
}

class ConnectFour {
 public:
  using Action = ConnectFourAction;
  using Status = ConnectFourStatus;
  using BoardStateType = Eigen::Matrix<char,
                                       CONNECT_FOUR_NUM_ROWS,
                                       CONNECT_FOUR_NUM_COLS>;

  ConnectFour(std::string const& state) { }

  ConnectFour() {
    Reset();
  }

  BoardStateType GetBoardState() const {
    return board_state_;
  }

  void Reset() {
    game_status_ = ConnectFourStatus::IN_PROGRESS;
    board_state_.fill('-');
    x_turn = true;
  }

  std::vector<ConnectFourAction> GetAvailableActions() const;
  void ApplyAction(ConnectFourAction const & action);
  ConnectFour ForwardModel(ConnectFourAction const& action) const;

  ConnectFourStatus GetGameStatus() const {
    return game_status_;
  }

  bool GameOver() const {
    return game_status_ != ConnectFourStatus::IN_PROGRESS;
  }

  bool Draw() const {
    return game_status_ == ConnectFourStatus::DRAW;
  }

  std::string GetStateString() const {
    return std::string(board_state_.data(), string_size);
  }

 private:
  BoardStateType board_state_;
  ConnectFourStatus game_status_;
  const int num_rows = CONNECT_FOUR_NUM_ROWS;
  const int num_cols = CONNECT_FOUR_NUM_COLS;
  const int string_size = num_rows * num_cols;
  bool x_turn;
};
