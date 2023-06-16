import java.util.concurrent.ThreadLocalRandom;

public class DiamondSquare {
    static final int SIZE = 5;
    static final int MAX = 20;
    static final int MIN = 0;

    static int[][] grid;

    // TODO: implement a decaying random value

    static void printGrid() {
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                System.out.printf(" %2d ", grid[i][j]);
            }
            System.out.println();
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

    static void square(int low_i, int upp_i, int low_j, int upp_j, int mid) {
        // TODO probably want to update this to account for corners possibly missing
        // (maybe?) the corners should always be there

        int sum = grid[low_i][low_j] + grid[low_i][upp_j] + grid[upp_i][low_j] + grid[upp_i][upp_j];

        int mid_i = (upp_i + low_i) / 2;
        int mid_j = (upp_j + low_j) / 2;

        grid[mid_i][mid_j] = sum / 4;
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

    static void diamond(int low_i, int upp_i, int low_j, int upp_j, int mid) {
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
            sum += grid[mid][(low_j - mid_j)];
            count++;
        }
        grid[mid_i][low_j] = Math.round((float) sum / count);
        System.out.println();
        printGrid();

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
        grid[low_i][mid_j] = Math.round((float) sum / count);
        System.out.println();
        printGrid();

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
        grid[mid_i][upp_j] = Math.round((float) sum / count);
        System.out.println();
        printGrid();

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
        grid[upp_i][mid_j] = Math.round((float) sum / count);
        System.out.println();
        printGrid();
    }

    static void diamondSquare(int low_i, int upp_i, int low_j, int upp_j) {
        System.out.println();
        printGrid();
        float delta = ((upp_i + low_i) / 2);
        if (delta < 1 || (upp_i - low_i) <= 1) {
            return;
        }
        int mid = (int) delta;


        square(low_i, upp_i, low_j, upp_j, mid);

        System.out.println();
        printGrid();
        diamond(low_i, upp_i, low_j, upp_j, mid);

        diamondSquare(low_i, mid, low_j, mid);
        diamondSquare(mid, upp_i, mid, upp_j);
        diamondSquare(mid, upp_i, low_j, mid);
        diamondSquare(low_i, mid, mid, upp_j);
    }

    public static void main(String[] args) {
        grid = new int[SIZE][SIZE];

        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                grid[i][j] = -1;
            }
        }

        printGrid();

        populateCorners();
        System.out.println();
        printGrid();

        diamondSquare(0, SIZE - 1, 0, SIZE - 1);
        System.out.println();
        printGrid();

        // for (int i = 0; i < 12; i++) {
        //     int x = (int) Math.pow(2, i) + 1;
        //     System.out.printf("i: %d -> %d\n", i, x);
        // } 

        // for (int i = 0; i < 2051; i++) {
        //     if (isSizeValid(i)) {
        //         System.out.println(i);
        //     }
        // }
    }
}   
