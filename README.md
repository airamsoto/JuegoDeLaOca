# Game of the Goose - FP1 Project

## Introduction

This project is part of the FP1 course in the Software Engineering degree at Complutense University of Madrid. It involves creating a simulation of the traditional board game, "Game of the Goose," using C++. This implementation allows for multiple players and features a variety of game scenarios, leveraging basic and advanced programming techniques to manage game state, player movements, and game rules.

## Features

- **Multiple Player Support**: The game supports up to 4 players in a competitive format.
- **Dynamic Board Setup**: Users can load different board configurations from files, allowing for customizable gameplay.
- **Penalty System**: Special board squares like the "Inn," "Prison," and "Well" penalize players by skipping their turns.
- **Debug Mode**: Includes a debug mode for step-by-step execution and easier debugging.
- **File Management**: Ability to save and load game states, including board configurations and player statuses.

## Getting Started

### Prerequisites

Ensure you have a C++ compiler installed on your system. This project is developed using standard C++ libraries only.

### Installation

1. Clone the repository to your local machine:
git clone https://yourrepositorylink.com/game-of-the-goose.git

2. Navigate to the cloned directory:
cd game-of-the-goose

3. Compile the project using your C++ compiler. For example, if you are using `g++`:
g++ -o GameOfTheGoose main.cpp

4. Run the executable:
./GameOfTheGoose


### Playing the Game

Follow the on-screen instructions to start a new game. Players will choose their mode (normal or debug), roll dice, and navigate through the board as dictated by the rules of the Game of the Goose.

## Project Structure

- `main.cpp`: Contains the main function and game loop.
- `game_logic.cpp`: Includes functions for game mechanics like moving players, handling special squares, and checking game end conditions.
- `io_manager.cpp`: Manages file input/output operations for loading and saving games.

## Contributing

We welcome contributions to this project. Please fork the repository and submit pull requests with your enhancements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Thanks to the FP1 teaching team at UCM for their guidance and support throughout the development of this project.
- Inspired by the traditional European board game, "Game of the Goose".
