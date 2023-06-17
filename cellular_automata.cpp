#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <utility>

#include "./bmp/image.cpp"

using namespace std;

// Used to determine the proportion of the none default tile
const int NOISE_DENSITY = 72;
// Amount of neighbors required to make water tile
const int NUM_NEIGHBORS = 5;
// Iterations of the processing to run
const int ITERATIONS = 4;

// Chance of "trees" being put on a ground piece
const int TREE_PERCENTAGE = 7;
// True if you want trees to be generated
const bool TREES = false;

// How much to upscale the grid for the .bmp
const int UPSCALE_FACTOR = 10;
// True if you want the .bmp to be upscaled
const bool UPSCALE = true;

// Dimensions of grid
const int GRID_HEIGHT = 100;
const int GRID_WIDTH = 100;
// 4k    -> 3840 x 2160
// 1440p -> 2560 x 1440
// 1080p -> 1920 x 1080

// !!!! -> Can change it to be "lava" instead of water and "stone" instead of grass
const Color WATER_COLOR = Color(17, 54, 240);
const Color GRASS_COLOR = Color(17, 240, 54);
const Color TREE_COLOR = Color(105, 53, 16);
// lava  -> 255, 102, 0
// stone -> 38, 38, 38
// water -> 17, 54, 240
// grass -> 17, 240, 54

// This upscales the grid so we have higher resolution pictures
vector<vector<char>> upscale_grid(vector<vector<char>>& grid) {
    vector<vector<char>> new_grid (GRID_HEIGHT * UPSCALE_FACTOR, 
                                   std::vector<char> (GRID_WIDTH * UPSCALE_FACTOR, '-'));

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            for (int a = 0; a < UPSCALE_FACTOR; a++) {
                for (int b = 0; b < UPSCALE_FACTOR; b++) {
                    int x = (i * UPSCALE_FACTOR) + a;
                    int y = (j * UPSCALE_FACTOR) + b;
                    new_grid[(i * UPSCALE_FACTOR) + a][(j * UPSCALE_FACTOR) + b] = grid[i][j];
                }
            }
        }
    }

    return new_grid;
}

// This creates the Bitmap of the generated grid
void create_grid_image(string name, int width, int height, vector<vector<char>> grid) {
    vector<vector<char>> print_grid = grid;
    int w = width;
    int h = height;

    if (UPSCALE) {
        print_grid = upscale_grid(grid);
        w *= UPSCALE_FACTOR;
        h *= UPSCALE_FACTOR;
    }
    Image image(w, h);

    // Need to double check x's and y's
    for (int y = 0; y < print_grid.size(); y++) {
        for (int x = 0; x < print_grid[y].size(); x++) {
            if (print_grid[y][x] == 'g') {
                image.SetColor(GRASS_COLOR, x, y);
            } else if (print_grid[y][x] == 'w') {
                image.SetColor(WATER_COLOR, x, y);
            } else if (print_grid[y][x] == 't') {
                image.SetColor(TREE_COLOR, x, y);
            }
        }
    }

    image.Export(name + ".bmp");
}

// Prints the grid as colors on the console
// !!!! Note, might change how many horizontal spaces there are to account for different !!!!
// !!!! terminals vertical spacing.                                                      !!!!
void print_grid(vector<vector<char>>& grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'g') {
                cout << "\033[42m   \033[0m";
            } else if (grid[i][j] == 'w') {
                cout << "\033[44m   \033[0m";
            } else if (grid[i][j] == 't') {
                cout << "\033[33m   \033[0m";
            } else {
                cout << " " << grid[i][j] << " "; 
            }
        }
        cout << endl;
    }   
}

// Prints the grid in text format
void print_grid_text(vector<vector<char>>& grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            cout << " " << grid[i][j] << " "; 
        }
        cout << endl;
    }  
}

// Checks if a cell is inbounds
bool is_inbounds(int row, int col) {
    if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH)
        return false;
    return true;
}

// Runs the cellular automata over the grid
vector<vector<char>> process_grid(vector<vector<char>> grid) {
    vector<vector<char>> new_grid (grid.size(), std::vector<char> (grid[0].size(), '-'));

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            int water_count = 0;
            for (int row = (i - 1); row <= (i + 1); row++) {
                for (int col = (j - 1); col <= (j + 1); col++) {
                    if (is_inbounds(row, col)) {
                        if (row != i || col != j) {
                            if (grid[row][col] == 'w') {
                                water_count++;
                            }
                        }
                    } else {
                        water_count++;
                    }
                }
            }
            if (water_count > NUM_NEIGHBORS) {
                new_grid[i][j] = 'w';
            } else {
                new_grid[i][j] = 'g';
            }
        } // end GRID_WIDTH for
    } // end GRID_HEIGHT for

    return new_grid;
} // end process_grid

// Adds "trees" (spots) to the grid
vector<vector<char>> add_trees(vector<vector<char>>& grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'g') {
                int rand = std::rand() % 100;
                bool valid = true;

                if (rand < TREE_PERCENTAGE) {
                    for (int row = (i - 1); row <= (i + 1); row++) {
                        for (int col = (j - 1); col <= (j + 1); col++) {
                            if (is_inbounds(row, col)) {
                                if (grid[row][col] == 't') {
                                    valid = false;
                                }
                            }
                        }
                    }
                    
                    if (valid)
                        grid[i][j] = 't';
                }
            }
        }
    }

    return grid;
}

// Creates the initial noisy grid
void populate_grid(vector<vector<char>>& grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            int rand = std::rand() % 100;
            if (rand > NOISE_DENSITY) {
                grid[i][j] = 'g';
            } else {
                grid[i][j] = 'w';
            }
        }
    }    
}

int main() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    srand(time(NULL));

    std::vector<std::thread> threads;
    vector<vector<char>> grid (GRID_HEIGHT, std::vector<char> (GRID_WIDTH, '-'));

    populate_grid(grid);

    string file_name = "./pics/" + to_string(0) + "_passes";
    threads.push_back(std::thread(create_grid_image, file_name, GRID_WIDTH, GRID_HEIGHT, std::ref(grid)));
    for(int i = 1; i <= ITERATIONS; i++) {
        grid = process_grid(grid);

        file_name = "./pics/" + to_string(i) + "_passes";
        threads.push_back(std::thread(create_grid_image, file_name, GRID_WIDTH, GRID_HEIGHT, std::ref(grid)));
    }
    
    if (TREES) {
        grid = add_trees(grid);
        threads.push_back(std::thread(create_grid_image, "./pics/with_trees", GRID_WIDTH, GRID_HEIGHT, std::ref(grid)));
    }

    for (int i= 0; i < threads.size(); i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    long micro_seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    double seconds = (float)micro_seconds / 1000000;

    std::cout << "Time difference = " << micro_seconds << " [µs]" << std::endl;
    std::cout << "Time difference = " << seconds << " [s]" << std::endl;

    return 0;
}