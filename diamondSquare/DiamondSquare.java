import java.io.PrintStream;
import java.util.concurrent.ThreadLocalRandom;

public class DiamondSquare {
    static final int SIZE = 17;
    static final int MAX = 20;
    static final int MIN = 0;
    static final double DECAY_VALUE = .9935;

    static double[] random_range = new double[] {-4, 4};

    static int[][] grid;

    static void printGridForC(PrintStream ptr) {
        System.out.println(SIZE);
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                ptr.printf(" %2d ", grid[i][j]);
            }
            ptr.println();
        }
    }

    static void printGrid(PrintStream ptr) {
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                ptr.printf(" %2d ", grid[i][j]);
            }
            ptr.println();
        }
    }

    static boolean isSizeValid(int size) {
        double x = Math.log(size - 1) / Math.log(2);

        if (x % 1 == 0)
            return true;
        return false;
    }

    static void populateCorners() {
        grid[0][0] = ThreadLocalRandom.current().nextInt(MIN, MAX);
        grid[0][SIZE - 1] = ThreadLocalRandom.current().nextInt(MIN, MAX);
        grid[SIZE - 1][0] = ThreadLocalRandom.current().nextInt(MIN, MAX);
        grid[SIZE - 1][SIZE - 1] = ThreadLocalRandom.current().nextInt(MIN, MAX);
    }

    static void square(int low_i, int upp_i, int low_j, int upp_j) {
        // TODO probably want to update this to account for corners possibly missing
        // (maybe?) the corners should always be there
        int sum = grid[low_i][low_j] + grid[low_i][upp_j] + grid[upp_i][low_j] + grid[upp_i][upp_j];

        int mid_i = (upp_i + low_i) / 2;
        int mid_j = (upp_j + low_j) / 2;

        grid[mid_i][mid_j] = addValue(sum, 4);
    }

    static boolean inBounds(int i, int j) {
        // TODO: add a check if it's -1 (we don't use it in the average)
        try {
            if  (grid[i][j] == -1)
                return false;
            return true;
        } catch (IndexOutOfBoundsException e) {
            return false;
        }
    }

    // TODO: could probably remove this
    static int addIfInBounds(int i, int j) {
        try {
            return grid[i][j];
        } catch (IndexOutOfBoundsException e) {
            return 0;
        }
    }

    static int addValue(int sum, int count) {
        int value = (int) ((sum / count) + 
                        ThreadLocalRandom.current().nextDouble(random_range[0], random_range[1]));
        
        random_range[0] *= DECAY_VALUE;
        random_range[1] *= DECAY_VALUE;

        if (value > MAX) {
            return MAX;
        } else if (value < MIN) {
            return MIN;
        }
        return value;
    }

    static void diamond(int low_i, int upp_i, int low_j, int upp_j) {
        // TODO: check to see how the rounding/float -> int casting works
        int sum = 0; int count = 0;
        int mid_i = (upp_i + low_i) / 2;
        int mid_j = (upp_j + low_j) / 2;

        // left
        if (inBounds(low_i, low_j)) {
            sum += grid[low_i][low_j];
            count++;
        }
        if (inBounds(mid_i, mid_j)) {
            sum += grid[mid_i][mid_j];
            count++;
        }
        if (inBounds(upp_i, low_j)) {
            sum += grid[upp_i][low_j];
            count++;
        }
        if (inBounds(mid_i, (low_j - mid_j))) {
            sum += grid[mid_i][(low_j - mid_j)];
            count++;
        }
        grid[mid_i][low_j] = addValue(sum, count);

        // up
        sum = 0; count = 0;
        if (inBounds(low_i, low_j)) {
            sum += grid[low_i][low_j];
            count++;
        }
        if (inBounds(mid_i, mid_j)) {
            sum += grid[mid_i][mid_j];
            count++;
        }
        if (inBounds(low_i, upp_j)) {
            sum += grid[low_i][upp_j];
            count++;
        }
        if (inBounds((mid_i - low_i), mid_j)) {
            sum += grid[(mid_i- low_i)][mid_j];
            count++;
        }
        grid[low_i][mid_j] = addValue(sum, count);

        // right
        sum = 0; count = 0;
        if (inBounds(upp_i, upp_j)) {
            sum += grid[upp_i][upp_j];
            count++;
        }
        if (inBounds(mid_i, mid_j)) {
            sum += grid[mid_i][mid_j];
            count++;
        }
        if (inBounds(low_i, upp_j)) {
            sum += grid[low_i][upp_j];
            count++;
        }
        if (inBounds(mid_i, (upp_j + mid_j))) {
            sum += grid[mid_i][(upp_j + mid_j)];
            count++;
        }
        grid[mid_i][upp_j] = addValue(sum, count);

        //down
        sum = 0; count = 0;
        if (inBounds(upp_i, upp_j)) {
            sum += grid[upp_i][upp_j];
            count++;
        }
        if (inBounds(mid_i, mid_j)) {
            sum += grid[mid_i][mid_j];
            count++;
        }
        if (inBounds(upp_i, low_j)) {
            sum += grid[upp_i][low_j];
            count++;
        }
        if (inBounds((upp_i + mid_i), mid_j)) {
            sum += grid[(upp_i + mid_i)][mid_j];
            count++;
        }
        grid[upp_i][mid_j] = addValue(sum, count);
    }

    static void diamondSquare(int low_i, int upp_i, int low_j, int upp_j) {
        float check_i = ((upp_i + low_i) / 2);
        float check_j = ((upp_j + low_j) / 2);
        int delta_i = upp_i - low_i;
        int delta_j = upp_j - low_j;
        
        // probably don't need all of these checks
        if (check_i < 1 || delta_i <= 1 || check_j < 1 || delta_j <= 1) {
            return;
        }
        int mid_i = (int) ((upp_i + low_i) / 2);
        int mid_j = (int) ((upp_j + low_j) / 2);

        square(low_i, upp_i, low_j, upp_j);

        diamond(low_i, upp_i, low_j, upp_j);

        // Quad 1
        diamondSquare(low_i, mid_i, low_j, mid_j);
        // Quad 3
        diamondSquare(mid_i, upp_i, mid_j, upp_j);
        // Quad 4
        diamondSquare(mid_i, upp_i, low_j, mid_j);
        // Quad 2
        diamondSquare(low_i, mid_i, mid_j, upp_j);


    }

    public static void main(String[] args) {
        grid = new int[SIZE][SIZE];

        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                grid[i][j] = -1;
            }
        }
        populateCorners();

        diamondSquare(0, SIZE - 1, 0, SIZE - 1);

        // for (int i = 0; i < 12; i++) {
        //     int x = (int) Math.pow(2, i) + 1;
        //     System.err.printf("i: %d -> %d\n", i, x);
        // } 

        // for (int i = 0; i < 2051; i++) {
        //     if (isSizeValid(i)) {
        //         System.out.println(i);
        //     }
        // }

        printGridForC(System.out);
    }
}   
