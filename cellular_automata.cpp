#include<cstdlib>
#include<ctime>
#include<iostream>
#include<vector>

#include "./bmp/image.cpp"

using namespace std;

// TODO: the image created is inverted

// Used to determine the proportion of the none default tile.
int NOISE_DENSITY = 72;
int NUM_NEIGHBORS = 5;

// Height of grid
const int GRID_HEIGHT = 100;
// Width of grid
const int GRID_WIDTH = 100;

// const int WATER_COLOR[3] = {17, 54, 240};
// const int GRASS_COLOR[3] = {17, 240, 54};

const Color WATER_COLOR = Color(239, 202, 18);
const Color GRASS_COLOR = Color(239, 18, 202);

// TODO: make grid global

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

void print_grid(vector<vector<char>>& grid) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid[i][j] == 'g') {
                cout << "\033[42m \033[0m";
            } else if (grid[i][j] == 'w') {
                cout << "\033[44m \033[0m";
            } else {
                cout << " " << grid[i][j] << " "; 
            }
        }
        cout << endl;
    }   
}

void print_grid_text(vector<vector<char>>& grid) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
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
    // TODO: update function to run multiple times
    vector<vector<char>> new_grid (GRID_HEIGHT, std::vector<char> (GRID_WIDTH, '-'));

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

void create_grid_image(int num, int width, int height, vector<vector<char>>& grid) {
    Image image(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == 'g') {
                image.SetColor(GRASS_COLOR, x, y);
            } else {
                image.SetColor(WATER_COLOR, x, y);
            }
        }
    }

    string file_name = "./pics/" + to_string(num) + "_passes.bmp";

    image.Export(file_name);
}


void upscale_grid(vector<vector<char>>& grid) {
    vector<vector<char>> new_grid (GRID_HEIGHT * 4, std::vector<char> (GRID_WIDTH * 4, '-'));

    cout << new_grid.size() << " " << new_grid[0].size() << endl;

    // TODO: work on this
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    new_grid[(y * 4) + i][(x * 4) + j] = grid[y][x];    
                }
            }
        }
    }


    print_grid(grid);

    cout << "\n";

    print_grid(new_grid);

    print_grid_text(grid);

    cout << "\nNew Grid:\n";
    print_grid_text(new_grid);
}

int main() {
    srand(time(NULL));

    vector<vector<char>> grid (GRID_HEIGHT, std::vector<char> (GRID_WIDTH, '-'));

    populate_grid(grid);

    create_grid_image(0, GRID_WIDTH, GRID_HEIGHT, grid);
    for(int i = 1; i < 11; i++) {
        grid = process_grid(grid);
        create_grid_image(i, GRID_WIDTH, GRID_HEIGHT, grid);
    }
    
    for (int i = 0; i < 10; i++) {
        grid = process_grid(grid); 
    }

    // for (int i = 0; i < 8; i++) {
    //     process_grid(grid);
    // }

    // cout << "\nAfter 10 Passes:\n";
    // print_grid(grid);

    return 0;
}