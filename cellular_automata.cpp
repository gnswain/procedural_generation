#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>

#include "./bmp/image.cpp"

using namespace std;

// TODO: the image created is inverted

// Used to determine the proportion of the none default tile
const int NOISE_DENSITY = 72;
// Amount of neighbors required to make water tile
const int NUM_NEIGHBORS = 5;
// Iterations of the processing to run
const int ITERATIONS = 4;

// Chance of tree being put on a ground piece
const int TREE_PERCENTAGE = 7;
// True if you want trees to be generated
const bool TREES = false;

// How much to upscale the grid for the .bmp
const int UPSCALE_FACTOR = 4;
// True if you want the .bmp to be upscaled
const bool UPSCALE = true;

// Height of grid
const int GRID_HEIGHT = 4;
// Width of grid
const int GRID_WIDTH = 4;

// !!!! -> Can change it to be "lava" instead of water and "stone" instead of grass
const Color WATER_COLOR = Color(255, 102, 0); // lava
const Color GRASS_COLOR = Color(38, 38, 38); // stone
// const Color WATER_COLOR = Color(17, 54, 240); // actually water
// const Color GRASS_COLOR = Color(17, 240, 54); // actually grass
const Color TREE_COLOR = Color(105, 53, 16);

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

void create_grid_image(string name, int width, int height, vector<vector<char>>& grid) {
    vector<vector<char>> print_grid = grid;
    int w = width;
    int h = height;

    if (UPSCALE) {
        print_grid = upscale_grid(grid);
        w *= UPSCALE_FACTOR;
        h *= UPSCALE_FACTOR;
    }
    Image image(w, h);

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
    srand(time(NULL));

    vector<vector<char>> grid (GRID_HEIGHT, std::vector<char> (GRID_WIDTH, '-'));

    populate_grid(grid);

    string file_name = "./pics/" + to_string(0) + "_passes";
    create_grid_image(file_name, GRID_WIDTH, GRID_HEIGHT, grid);
    for(int i = 1; i <= ITERATIONS; i++) {
        grid = process_grid(grid);

        file_name = "./pics/" + to_string(i) + "_passes";
        create_grid_image(file_name, GRID_WIDTH, GRID_HEIGHT, grid);
    }
    
    if (TREES) {
        grid = add_trees(grid);
        create_grid_image("./pics/with_trees", GRID_WIDTH, GRID_HEIGHT, grid);
    }

    return 0;
}