/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */
// Copyright 2012 <EB/Stanford>

#include <iostream>
#include <string>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"
#include "set.h"
#include "random.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

static int getMazeDimension(string prompt,
                            int minDimension = 3, int maxDimension = 50) {
	while (true) {
		int response = getInteger(prompt);
		if (response == 0) return response;
        if (response >= minDimension
            && response <= maxDimension) return response;
		cout << "Please enter a number between "
			 << minDimension << " and "
		     << maxDimension << ", inclusive." << endl;
	}
}

class MazeManager {
 public:
    MazeManager(MazeGeneratorView &view, int dimension) {
        this->view = view;
        this->dimension = dimension;
    }

    void eliminateWalls() {
        for (int i = 0; i < (this->dimension*this->dimension - 10); i++) {
            wall considerEliminating =
            this->getRandomWallToConsiderForElimination();
            if (this->doesWallSeparateTwoChambers(considerEliminating))
                this->removeWall(considerEliminating);
        }
    }

    bool doesWallSeparateTwoChambers(wall wallToTest) {
        return true;
    }

    bool removeWall(wall wallToRemve) {
        for (int i = 0; i < this->walls.size(); i++) {
            if (this->walls.get(i) == wallToRemve) {
                this->view.removeWall(wallToRemve);
                this->walls.remove(i);
                return true;
            }
        }
        return false;
    }

    void populateBoardInitially() {
        // create rows
        for (int colNum = 1; colNum <= this->dimension; colNum++) {
            for (int rowNum = 0; rowNum < this->dimension; rowNum++) {
                cell newCell1;
                newCell1.row = rowNum;
                newCell1.col = colNum;

                cell newCell2;
                newCell2.row = rowNum + 1;
                newCell2.col = colNum;

                wall newWall1;
                newWall1.one = newCell1;
                newWall1.two = newCell2;
                this->addWallToMaze(newWall1);
            }
        }

        // create columns
        for (int colNum = 1; colNum <= this->dimension; colNum++) {
            for (int rowNum = 0; rowNum < this->dimension; rowNum++) {
                cell newCell3;
                newCell3.row = rowNum;
                newCell3.col = colNum;

                cell newCell4;
                newCell4.row = rowNum;
                newCell4.col = colNum + 1;

                wall newWall2;
                newWall2.one = newCell3;
                newWall2.two = newCell4;
                this->addWallToMaze(newWall2);
            }
        }
    }

    bool isWallAtLocation(double colNum, double rowNum) {
        for (int i = 0; i < this->walls.size(); i++) {
            double wallColNum = this->getFullColNumOfWall(this->walls.get(i));
            double wallRowNum = this->getFullRowNumOfWall(this->walls.get(i));
            if (wallColNum == colNum
                && wallRowNum == rowNum)
                return true;
        }
        return false;
    }

    wall getWallAtLocation(double colNum, double rowNum) {
        for (int i = 0; i < this->walls.size(); i++) {
            double wallColNum = this->getFullColNumOfWall(this->walls.get(i));
            double wallRowNum = this->getFullRowNumOfWall(this->walls.get(i));
            if (wallColNum == colNum
                && wallRowNum == rowNum) {
                return this->walls.get(i);
            }
        }
    }

    double getFullColNumOfWall(wall myWall) {
        return ((static_cast<double>(myWall.one.col) +
                 static_cast<double>(myWall.two.col)) / 2.0);
    }

    double getFullRowNumOfWall(wall myWall) {
        return ((static_cast<double>(myWall.one.row) +
                 static_cast<double>(myWall.two.row)) / 2.0);
    }

    bool addWallToMaze(wall &wallToAdd) {
        this->walls.add(wallToAdd);
        this->view.drawWall(wallToAdd);
        return true;
    }

    wall getRandomWallToConsiderForElimination() {
        int wallToRemove = randomInteger(0, this->walls.size());
        return this->walls.get(wallToRemove);
    }

 private:
    MazeGeneratorView view;
    int dimension;
    Vector<wall> walls;
    Set<wall> wallsConsideredForElimination;
};

int main()  {
    //  CATCH UNIT TESTS
    //  Create a default config object
    Catch::Config config;

    //  Configure CATCH to run all tests starting with "mytests"
    config.addTestSpec("MazeGenerator/*");

    //  Forward on to CATCH's main using our custom config.
    //  This overload doesn't take command line arguments
    //  So the config object must be fully set up
    Catch::Main(config);

    //  NORMAL CODE
	while (true) {
		int dimension = getMazeDimension("What should the dimension of "
                                         "your maze be [0 to exit]? ");
		if (dimension == 0) break;

        //  generate a maze object
        MazeGeneratorView mazeView = MazeGeneratorView();

        //  set dimension
        mazeView.setDimension(dimension);

        //  draw border
        mazeView.drawBorder();

        //  create mazemanager
        MazeManager manager(mazeView, dimension);

        //  create n*n Wall objects
        manager.populateBoardInitially();

        //  remove walls
        manager.eliminateWalls();
	}
	return 0;
}

TEST_CASE("MazeGenerator/getFullColNumOfWall1", "") {
    cell testCell1;
    testCell1.col = 1;
    testCell1.row = 1;
    cell testCell2;
    testCell2.col = 2;
    testCell2.row = 1;
    wall testWall1;
    testWall1.one = testCell1;
    testWall1.two = testCell2;

    //  generate a maze object
    MazeGeneratorView tMazeView = MazeGeneratorView();

    //  set dimension
    tMazeView.setDimension(7);

    //  draw border
    tMazeView.drawBorder();

    //  create mazemanager
    MazeManager tManager(tMazeView, 7);

    REQUIRE(tManager.getFullColNumOfWall(testWall1) == 1.5);
    REQUIRE(tManager.getFullRowNumOfWall(testWall1) == 1);
}

TEST_CASE("MazeGenerator/getFullColNumOfWall2", "") {
    cell testCell1;
    testCell1.col = 1;
    testCell1.row = 1;
    cell testCell2;
    testCell2.col = 1;
    testCell2.row = 2;
    wall testWall1;
    testWall1.one = testCell1;
    testWall1.two = testCell2;

    //  generate a maze object
    MazeGeneratorView tMazeView = MazeGeneratorView();

    //  set dimension
    tMazeView.setDimension(7);

    //  draw border
    tMazeView.drawBorder();

    //  create mazemanager
    MazeManager tManager(tMazeView, 7);

    REQUIRE(tManager.getFullColNumOfWall(testWall1) == 1);
    REQUIRE(tManager.getFullRowNumOfWall(testWall1) == 1.5);
}

TEST_CASE("MazeGenerator/getWallAtLocation", "") {
    cell testCell1;
    testCell1.col = 0;
    testCell1.row = 0;
    cell testCell2;
    testCell2.col = 0;
    testCell2.row = 1;
    wall testWall1;
    testWall1.one = testCell1;
    testWall1.two = testCell2;

    //  generate a maze object
    MazeGeneratorView tMazeView = MazeGeneratorView();

    //  set dimension
    int dimension = 3;
    tMazeView.setDimension(dimension);

    //  draw border
    tMazeView.drawBorder();

    //  create mazemanager
    MazeManager tManager(tMazeView, dimension);
    tManager.populateBoardInitially();
    tManager.addWallToMaze(testWall1);
    tMazeView.removeWall(testWall1);

    REQUIRE(tManager.getWallAtLocation(0, 0.5) == testWall1);
    REQUIRE_FALSE(tManager.getWallAtLocation(0, 1.5) == testWall1);
}

TEST_CASE("MazeGenerator/removeWall", "") {
    cell testCell1;
    testCell1.col = 0;
    testCell1.row = 0;
    cell testCell2;
    testCell2.col = 0;
    testCell2.row = 1;
    wall testWall1;
    testWall1.one = testCell1;
    testWall1.two = testCell2;

    //  generate a maze object
    MazeGeneratorView tMazeView = MazeGeneratorView();

    //  set dimension
    int dimension = 3;
    tMazeView.setDimension(dimension);

    //  draw border
    tMazeView.drawBorder();

    //  create mazemanager
    MazeManager tManager(tMazeView, dimension);
    tManager.populateBoardInitially();
    tManager.addWallToMaze(testWall1);

    REQUIRE(tManager.getWallAtLocation(0, 0.5) == testWall1);

    tManager.removeWall(testWall1);

    REQUIRE_FALSE(tManager.getWallAtLocation(0, 0.5) == testWall1);
}
