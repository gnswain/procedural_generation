import java.io.PrintStream;
import java.util.concurrent.ThreadLocalRandom;

public class DiamondSquare {
    static final int SIZE = 17;
    static final int MAX = 20;
    static final int MIN = 0;

    static int[] random_range = new int[] {-4, 4};

    static int[][] grid;

    // TODO: implement a decaying random value

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

    static void addValue(int i, int j) {
        
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
        System.err.printf("square(low_i: %d, upp_i: %d, low_j: %d, upp_j: %d)\n", 
                          low_i, upp_i, low_i, upp_j);

        int sum = grid[low_i][low_j] + grid[low_i][upp_j] + grid[upp_i][low_j] + grid[upp_i][upp_j];

        int mid_i = (upp_i + low_i) / 2;
        int mid_j = (upp_j + low_j) / 2;

        grid[mid_i][mid_j] = (sum / 4) + ThreadLocalRandom.current().nextInt(random_range[0], random_range[1]);

        printGrid(System.err);
        System.err.println();
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

    static void diamond(int low_i, int upp_i, int low_j, int upp_j) {
        // TODO: check to see how the rounding/float -> int casting works
        int sum = 0; int count = 0;
        int mid_i = (upp_i + low_i) / 2;
        int mid_j = (upp_j + low_j) / 2;

        System.err.printf("diamond(low_i: %d, upp_i: %d, low_j: %d, upp_j: %d) mid_i -> %d  mid_j -> (%d)", 
                          low_i, upp_i, low_i, upp_j, mid_i, mid_j);

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
        grid[mid_i][low_j] = Math.round((float) sum / count) + ThreadLocalRandom.current().nextInt(random_range[0], random_range[1]);

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
        grid[low_i][mid_j] = Math.round((float) sum / count) + ThreadLocalRandom.current().nextInt(random_range[0], random_range[1]);

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
        grid[mid_i][upp_j] = Math.round((float) sum / count) + ThreadLocalRandom.current().nextInt(random_range[0], random_range[1]);

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
        grid[upp_i][mid_j] = Math.round((float) sum / count) + ThreadLocalRandom.current().nextInt(random_range[0], random_range[1]);

        System.err.println();
        printGrid(System.err);
    }

    static void diamondSquare(int low_i, int upp_i, int low_j, int upp_j) {
        float check_i = ((upp_i + low_i) / 2);
        float check_j = ((upp_j + low_j) / 2);
        int delta_i = upp_i - low_i;
        int delta_j = upp_j - low_j;
        
        System.err.printf("\nBegin diamondSquare(low_i: %d, upp_i: %d, low_j: %d, upp_j: %d)\n\tmid_i -> %f\n\tmid_j -> %f\n\tdelta_i -> %d\n\tdelta_j -> %d\n", 
                          low_i, upp_i, low_j, upp_j, check_i, check_j, delta_i, delta_j);
        
        // probably don't need all of these checks
        if (check_i < 1 || delta_i <= 1 || check_j < 1 || delta_j <= 1) {
            System.err.println("Exited diamondSquare");
            return;
        }
        int mid_i = (int) ((upp_i + low_i) / 2);
        int mid_j = (int) ((upp_j + low_j) / 2);

        
        printGrid(System.err);
        System.err.println();


        square(low_i, upp_i, low_j, upp_j);

        diamond(low_i, upp_i, low_j, upp_j);

        // Quad 1
        System.err.println("\n\n   -------------------- Quad 1 --------------------");
        diamondSquare(low_i, mid_i, low_j, mid_j);
        // Quad 3
        System.err.println("\n\n   -------------------- Quad 3 --------------------");
        diamondSquare(mid_i, upp_i, mid_j, upp_j);
        // Quad 4
        System.err.println("\n\n   -------------------- Quad 4 --------------------");
        diamondSquare(mid_i, upp_i, low_j, mid_j);
        // Quad 2
        System.err.println("\n\n   -------------------- Quad 2 --------------------");
        diamondSquare(low_i, mid_i, mid_j, upp_j);

        System.err.printf("\nEnd diamondSquare(low_i: %d, upp_i: %d, low_j: %d, upp_j: %d)\n\tmid_i -> %d\n\tmid_j -> %d\n\tdelta_i -> %d\n\tdelta_j -> %d\n", 
                          low_i, upp_i, low_j, upp_j, mid_i, mid_j, delta_i, delta_j);
        printGrid(System.err);
        System.err.println();
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

        printGrid(System.out);
    }
}   
