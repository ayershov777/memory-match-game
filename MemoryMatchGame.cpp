#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include "MemoryMatchGame.h"

using namespace std;

MemoryMatchGame::MemoryMatchGame() {
    initGame();
}

void MemoryMatchGame::play() {
    startTime = time(NULL);
    int points = 0;
    int index1, index2;
    while(points != numWords) {
        drawGrid();
        cout << "guess num1: ";
        index1 = guessNumber();
        display[index1] = answer[index1];
        drawGrid();
        cout << "guess num2: ";
        index2 = guessNumber();
        display[index2] = answer[index2];
        drawGrid();
        if (answer[index1] == answer[index2]) {
            cout << "Correct!" << endl;
            points++;
            Sleep(2000);
        }
        else {
            cout << "Incorrect!" << endl;
            Sleep(sleepMS);
            display[index1] = "";
            display[index2] = "";
        }
    }

    int endTime = time(NULL);
    int winTime = endTime - startTime;
    int winMinutes = winTime / 60;
    int winSeconds = winTime % 60;
    cout << "You Won in " << winMinutes << " minutes and " << winSeconds << " seconds!" << endl;
    system("PAUSE");
}

void MemoryMatchGame::initGame() {
    cout << "Welcome to the Memory Match Game!" << endl;
    int level = initLevel();
    size = 4 + level * 2;
    numWords = size * size / 2;
    display = new string[numWords * 2];
    int speed = initSpeed();
    sleepMS = (speed + 1) * 2000;
    int theme = initTheme();
    initAnswer(theme);
}

int MemoryMatchGame::initLevel() {
    cout << "Pick a difficulty level" << endl;
    cout << "easy, medium, or hard" << endl;
    string input;
    while (true) {
        cin >> input;
        if (input == "easy") return 0;
        if (input == "medium") return 1;
        if (input == "hard") return 2;
        cout << "invalid input!" << endl;
    }
}

int MemoryMatchGame::initSpeed() {
    cout << "Pick a speed level" << endl;
    cout << "slow, medium, or fast" << endl;
    string input;
    while (true) {
        cin >> input;
        if (input == "fast") return 0;
        if (input == "medium") return 1;
        if (input == "slow") return 2;
        cout << "invalid input!" << endl;
    }
}

int MemoryMatchGame::initTheme() {
    cout << "Pick a theme" << endl;
    cout << "animals, cars, or names" << endl;
    string input;
    while (true) {
        cin >> input;
        if (input == "animals") return 0;
        if (input == "cars") return 1;
        if (input == "names") return 2;
        cout << "invalid input!" << endl;
    }
}

void MemoryMatchGame::initAnswer(int theme) {
    string fileName;
    if (theme == 0) fileName = "animals.txt";
    else if (theme == 1) fileName = "cars.txt";
    else if (theme == 2) fileName = "names.txt";

    //read words from file
    ifstream file(fileName);
    string line;
    vector<string> allWords;
    while (getline(file, line)) {
        allWords.push_back(line);
    }

    //randomly pick 50 unique words, always O(n)
    srand(time(NULL));
    int randIndex;
    string* randomWords = new string[numWords];
    for (int i = 0; i < numWords; i++) {
        randIndex = rand() % allWords.size();
        randomWords[i] = allWords[randIndex];
        allWords.erase(allWords.begin() + randIndex);
    }

    //create a list to keep track of unused indices, to keep everything O(n)
    vector<int> indices(numWords * 2);
    for (int i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }

    //randomly place each word in 2 unique indices of the answer array
    int randNum;
    answer = new string[numWords * 2];
    for (int i = 0; i < numWords; i++) {
        for (int j = 0; j < 2; j++) {
            randNum = rand() % indices.size();
            randIndex = indices[randNum];
            indices.erase(indices.begin() + randNum);
            answer[randIndex] = randomWords[i];
        }
    }
    delete[] randomWords;
}

void MemoryMatchGame::drawGrid() {
    system("CLS");
    for (int i = 0; i <= size*2; i++) {
        for (int j = 0; j <= size*9; j++) {
            char c;
            if (i == 0) {
                if (j == 0) c = 201;
                else if (j == size*9) c = 187;
                else if (j % 9 == 0) c = 203;
                else c = 205;
            }
            else if (i == size*2) {
                if (j == 0) c = 200;
                else if (j == size*9) c = 188;
                else if (j % 9 == 0) c = 202;
                else c = 205;
            }
            else if (i % 2 == 0) {
                if (j == 0) c = 204;
                else if (j == size*9) c = 185;
                else if (j % 9 == 0) c = 206;
                else c = 205;
            }
            else {
                if (j % 9 == 0) c = 186;
                else c = getCharacter(i, j);
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

char MemoryMatchGame::getCharacter(int i, int j) {
    int wordIndex = findIndex(i, j);
    j--;
    j %= 9;
    string word = display[wordIndex];
    if (j < word.length()) {
        return word.at(j);
    }

    if (word == "") {
        if (j == 3)
            return to_string(wordIndex)[0];
        if (j == 4 && wordIndex > 9)
            return to_string(wordIndex)[1];
    }

    return ' ';
}

int MemoryMatchGame::findIndex(int i, int j) {
    i--;
    i /= 2;
    j--;
    j /= 9;
    return i * size + j;
}

int MemoryMatchGame::guessNumber() {
    int number;
    while(true) {
        cin >> number;
        if (number >= size * size) {
            cout << "invalid number!" << endl;
            cout << "try again: ";
        } 
        else if (display[number] != "") {
            cout << "You already discovered that card!" << endl;
            cout << "try again: ";
        }
        else break;
    }
    return number;
}