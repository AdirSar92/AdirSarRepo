import java.time.Duration;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

public class Puzz15 {
    private int dim;
    private final int[] puzzleBoard;
    private boolean hasStarted = false;
    private int tileToMoveIndex = -1;
    private int emptyTilePos;
    private final Scanner input = new Scanner(System.in);
    private  long timeStartedPlaying ;

    public Puzz15() {
        do {
            System.out.println("Please enter dim number between 4-10 ");
            this.dim = input.nextInt();
        }
        while (!isValidDimension(dim));
        puzzleBoard = new int[dim * dim];
        initBoard();
        startPuzzle();
    }

    private void initBoard() {
        int tileNum = 1;
        for (int i = 0; i < puzzleBoard.length ; i++) {
            puzzleBoard[i] = tileNum;
            ++tileNum;
        }
        puzzleBoard[puzzleBoard.length - 1] = -1;
        emptyTilePos = dim * dim - 1;
        shuffleBoard();
        printBoard();
    }

    private void shuffleBoard() {
        System.out.println("Shuffling Puzzle");
        int shuffleCounter = 0;
        for (int i = 0; i < puzzleBoard.length - 1  ; i++) {
            int rand = ThreadLocalRandom.current().nextInt(0,puzzleBoard.length - 1);
            swapTile(shuffleCounter,rand);
            ++shuffleCounter;
        }
        timeStartedPlaying = System.currentTimeMillis();

    }

    private void moveTile(){
        printInstruction();
        int tileSelected = input.nextInt();
        if (0 == tileSelected){
            PressedStart();
        } else {
            while(! isValidMove(tileSelected)){
                if (0 == tileSelected){
                    initBoard();
                }else {
                    System.out.println("Wrong tile, pick again !");
                    printBoard();
                    tileSelected = input.nextInt();

                }

            }
            if(0 != tileSelected)
            swapTile(tileToMoveIndex,emptyTilePos);
            printBoard();
        }
    }

    private void printInstruction(){
        System.out.println();
        System.out.println("press key 0 to restart the game");
        System.out.println("please enter tile number to move ");
        System.out.println("only tile the are neighbors of the empty tile");
    }

    private boolean isValidMove(int tileNumToMove){
        if(isNotOutOfBound(emptyTilePos + 1) && puzzleBoard[emptyTilePos + 1] == tileNumToMove){
            tileToMoveIndex = emptyTilePos + 1;
            return true;
        }
        if(isNotOutOfBound(emptyTilePos + - 1) && puzzleBoard[emptyTilePos -1] == tileNumToMove){
            tileToMoveIndex = emptyTilePos - 1;
            return true;
        }
        if(isNotOutOfBound(emptyTilePos + dim) && puzzleBoard[emptyTilePos + dim] == tileNumToMove){
            tileToMoveIndex = emptyTilePos + dim;
            return true;
        }
        if(isNotOutOfBound(emptyTilePos - dim ) && puzzleBoard[emptyTilePos - dim] == tileNumToMove){
            tileToMoveIndex = emptyTilePos - dim;
            return true;
        }
        return  false;
    }

    private boolean isNotOutOfBound(int index){
        return (index < dim * dim ) && (index >= 0);
    }

    private void swapTile(int firstIndex,int secondIndex){
        int temp = puzzleBoard[firstIndex];
        puzzleBoard[firstIndex] = puzzleBoard[secondIndex];
        puzzleBoard[secondIndex] = temp;
    }
    private boolean isWinner() {
        int tileNum = 1;

        for (int i = 0; i < puzzleBoard.length - 1 ; i++) {
            if (puzzleBoard[i] != tileNum) {
                return false;
            }
            ++tileNum;
        }
        System.out.println("Success!!! time for completing ");
        System.out.println(Duration.ofSeconds(TimeUnit.MILLISECONDS.toSeconds(timeStartedPlaying - System.currentTimeMillis())));
        return true;
    }

    private boolean isValidDimension(int dimension) {
        return dim > 1 && dim < 11 ;
    }

    private void confirmShuffle() {
        System.out.println("Are you sure you want to shuffle?");
        System.out.println("press 0 to shuffle or a ney key to continue");
        if (0 == input.nextInt()){
            initBoard();
        }
    }

    private void startPuzzle() {
        boolean isRematch = false;
        while(! isRematch){
            hasStarted = true;
            while(! isWinner()){
                moveTile();
            }
        }


    }
    private void PressedStart() {
        if (!hasStarted) {
            initBoard();
        } else {
            confirmShuffle();
            hasStarted = true;
        }
    }

    private void printBoard(){
        int counter = 0;
        for (int i = 0; i < puzzleBoard.length ; i++) {
            if(i > 1 && i %  dim == 0){
                System.out.println();
            }
            if(-1 == puzzleBoard[i]) {
                System.out.printf("%3s | ","X");
            }else {
                System.out.printf("%3d | ", puzzleBoard[i]);
            }
        }
        System.out.println();
    }

    private int convertIndexToArray(int xPos,int yPos){
        return (xPos / dim +  yPos % dim);
    }

    public static void main(String[] args) {
        Puzz15 puzz15 = new Puzz15();
    }


}