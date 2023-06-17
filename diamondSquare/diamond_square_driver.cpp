#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "../bmp/image.cpp"

using namespace std;

int main() {
    system("java DiamondSquare > diamond_square.txt");
    

    ifstream file_in("diamond_square.txt");

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

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid.size(); j++) {
            printf(" %2d ", grid[i][j]);
        }
        cout << endl;
    }         
}
