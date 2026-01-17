#include <iostream>
#include <thread>
#include <chrono>
#include <string_view>
#include <cmath>
#include <iomanip>
#include <limits>
#include <atomic>
#include "random.h"

std::atomic<bool> running(true);

template <typename T>
T getInput(std::string_view prompt)
{
    std::cout << prompt << '\n';

    T input{};
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return input;
}

bool gameMenu()
{
    while (true)
    {
        char start{getInput<char>("Type 's' to start, or 'q' to exit!")};
        switch(start)
        {
        case 's':
            return false;
        case 'q':
            return true;
        }
    }
}

void runTimer()
{
    auto start{std::chrono::high_resolution_clock::now()};

    while (running)
    {
        auto now{std::chrono::high_resolution_clock::now()};
        std::chrono::duration<double> elapsedTime = now - start;

        std::cout << '\r' << std::fixed << std::setprecision(3) << elapsedTime.count() << "s (Press ENTER!)" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void resetDifficulty(int& difficulty)
{
    if (difficulty < 1)
        ++difficulty;
}

void startGame()
{
    using namespace std::chrono;

    int points{0};
    int totalPoints{0};

    for (int i{1}; i <= 10; ++i)
    {
        static int difficulty{3};
        
        const double startRange{static_cast<double>(Random::get(90, 400)) / 100};
        const double endRange{(1 / std::pow(2, (static_cast<double>(difficulty) - 2))) + startRange};

        std::cout << "Round " << i << ": Click 'ENTER' between " << startRange << " and " << endRange << '\n';

        std::cout << "Click ENTER to start: ";
        std::cin.get();

        running = true;
        auto startTime{high_resolution_clock::now()};
        std::thread t(runTimer);

        std::cin.get();
        auto stopTime{high_resolution_clock::now()};
        running = false;
        
        t.join();

        duration<double> finalElapsedTime = stopTime - startTime; 

        const double userResult{finalElapsedTime.count()};

        if (userResult >= startRange && userResult <= endRange)
        {
            if (userResult == startRange || userResult == endRange)
            {
                std::cout << "\nYou just about made it. Close one!\n";
                --difficulty;
            }
                
            else
                std::cout << "\nNice! You were able to click in time!\n";

            ++difficulty;
            ++points;
        } 
        else 
        {
            std::cout << "Sorry, you were ";

            double startDis{std::abs(static_cast<double>(startRange) - userResult)};
            double endDis{std::abs(static_cast<double>(endRange) - userResult)};

            std::cout << ((startDis < endDis) ? startDis : endDis); 
            std::cout << "s from the range :(\n";

            --difficulty;
        }

        ++totalPoints;
        std::cout << "Score: " << points << '/' << totalPoints << '\n';
        
    }

    std::cout << "Your final score was " << points << '/' << totalPoints << '\n';
}

int main()
{
    bool exitProgram{false};

    std::cout << "This is a timer game!\n";
    std::cout << "Click 'ENTER' in the time frame shown to get a point. The more you get right, the harder it gets!\n";

    while (!exitProgram)
    {
        exitProgram = gameMenu();

        if (!exitProgram)
        {
            startGame();
        }
    } 

    return 0;
}


