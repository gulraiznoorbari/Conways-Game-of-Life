#include <iostream>
#include <fstream>

#include "UnInitDynamicArray.h"
#include "Update.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

bool simulationRunning = false;
int currentGeneration = 0;
int maxGenerations = 0;
bool** grid = nullptr;
UnInitDynamicArray<Position> cells;

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "Conway's Game of Life");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Event event;
    sf::Clock deltaClock;

    static int rows = 32;
    static int columns = 32;
    static char fileNameInput[1000] = "";

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // Grid generation:
        ImGui::InputInt("Rows", &rows);
        ImGui::InputInt("Columns", &columns);

        // Clamp values to prevent issues
        if (rows < 1) rows = 1;
        if (rows > 1000) rows = 1000;
        if (columns < 1) columns = 1;
        if (columns > 1000) columns = 1000;

        //File name based on user input
        //std::cout << "Enter the name of the file: " << std::endl;
        //std::cin >> fileNameInput;
        ImGui::InputText("Input File Name", fileNameInput, IM_ARRAYSIZE(fileNameInput));

        // based on pre-defined file name:
        //std::string path = "data/config.txt";
        //std::ifstream inputFile(path);

        if (ImGui::Button("Start"))
        {
            std::string fileName = "data/" + std::string(fileNameInput) + ".txt";
            //std::fstream inputFile(fileName, std::ios::in);
            //StartSimulation(window, fileName, rows, columns);

            std::ifstream inputFile(fileName);
            if (!inputFile)
            {
                std::cout << "Error! File could not be opened: " << fileName << std::endl;
                return 0;
            }

            // initialize grid
            grid = new bool* [rows];
            for (int i = 0; i < rows; i++)
            {
                grid[i] = new bool[columns];
            }
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    grid[i][j] = 0;
                }
            }

            int maxCells;
            inputFile >> maxGenerations >> maxCells;
            cells = UnInitDynamicArray<Position>(maxCells);

            for (int i = 0; i < maxCells; i++)
            {
                int x, y;
                inputFile >> x >> y;
                if (x >= 0 && x < rows && y >= 0 && y < columns)
                {
                    Position temp(x, y);
                    cells.Insert(temp);
                    grid[x][y] = 1;
                }
            }

            inputFile.close();
            simulationRunning = true;
            currentGeneration = 0;
        }

        window.clear();

        if (grid != nullptr)
        {
            float cellSize = 20.0f;
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < columns; c++)
                {
                    sf::RectangleShape rectangle;
                    rectangle.setSize(sf::Vector2f(cellSize - 2, cellSize - 2));
                    rectangle.setOutlineColor(sf::Color::Cyan);
                    if (grid[r][c])
                    {
                        rectangle.setFillColor(sf::Color::Cyan);
                    }
                    else
                    {
                        rectangle.setFillColor(sf::Color::Black);
                    }
                    rectangle.setOutlineThickness(1.0f);
                    rectangle.setPosition(cellSize * r, cellSize * c);
                    window.draw(rectangle);
                }
                std::cout << std::endl;
            }
        }

        ImGui::SFML::Render(window);
        window.display();

        if (simulationRunning && currentGeneration < maxGenerations)
        {
            sf::sleep(sf::milliseconds(200));
            Update(grid, cells, rows, columns);
            currentGeneration++;
            if (currentGeneration >= maxGenerations)
            {
                simulationRunning = false;
            }
        }
    }

    if (grid != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] grid[i];
        }
        delete[] grid;
    }

    ImGui::SFML::Shutdown();
}
