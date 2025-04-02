#include "Game.h"
#include <iostream>

int main() {
    try {
        // Initialize game with window size and title
        Game game(1024, 768, "Doom-like Game");
        
        // Run game loop
        game.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}