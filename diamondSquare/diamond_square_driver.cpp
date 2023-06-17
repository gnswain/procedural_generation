#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

#include "../bmp/image.cpp"

using namespace std;

const char *FILE_NAME = "diamond_square.txt";

// How much to upscale the grid for the .bmp
const int UPSCALE_FACTOR = 10;
// True if you want the .bmp to be upscaled
const bool UPSCALE = false;

const Color SEVEN_COLORS[] = {Color(148, 0,  211),
                              Color(75, 0, 130),
                              Color(0, 0, 225),
                              Color(0, 255, 0),
                              Color(255, 255, 0),
                              Color(255, 127, 0),
                              Color(255, 0, 0)};

// TODO: probably want to put this in a helper file
// This upscales the grid so we have higher resolution pictures
vector<vector<int>> upscale_grid(vector<vector<int>>& grid) {
    vector<vector<int>> new_grid (grid.size() * UPSCALE_FACTOR, 
                                   std::vector<int> (grid.size() * UPSCALE_FACTOR, '-'));

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
void create_grid_image(string name, int width, int height, vector<vector<int>> grid) {
    vector<vector<int>> print_grid = grid;
    int w = width;
    int h = height;

    if (UPSCALE) {
        print_grid = upscale_grid(grid);
        w *= UPSCALE_FACTOR;
        h *= UPSCALE_FACTOR;
    }
    Image image(w, h);

    for (int i = 0; i < print_grid.size(); i++) {
        for (int j = 0; j < print_grid[i].size(); j++) {
            image.SetColor(SEVEN_COLORS[(int) (print_grid[i][j] / 6)], i, j);
        }
    }

    image.Export(name + ".bmp");
}

int main() {
    system("java DiamondSquare > diamond_square.txt");
    
    ifstream file_in(FILE_NAME);

    int SIZE;

    string my_string;

    file_in >> SIZE;

    vector<std::vector<int>> grid (SIZE, vector<int> (SIZE, '-'));

    if ( file_in.is_open() ) {     
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid.size(); j++) {
                file_in >> grid[i][j];
            }
        }
    }       
    remove(FILE_NAME);

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid.size(); j++) {
            printf(" %2d ", grid[i][j]);
        }
        cout << endl;
    }  

    create_grid_image("../pics/diamondSquare", SIZE, SIZE, std::ref(grid));       
}
