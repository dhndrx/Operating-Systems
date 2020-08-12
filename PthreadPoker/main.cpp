#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <pthread.h>

using namespace std;

//defining globals for all of our shared data
vector<string> playingDeck = {"A","K","Q","J","10","9","8","7","6","5","4","3","2",
                              "A","K","Q","J","10","9","8","7","6","5","4","3","2",
                              "A","K","Q","J","10","9","8","7","6","5","4","3","2",
                              "A","K","Q","J","10","9","8","7","6","5","4","3","2",};
vector<string> hand1;
vector<string> hand2;
vector<string> hand3;
int turn = 0;
bool winner = false;
bool dealt = false;
ofstream myfile;

//prototypes for functions
int randomNumber(int);
void *runSim(void*);
void *runDealer(void*);
void displayDeck(int);

int main(){
    srand(clock());
    myfile.open ("log.txt");

    pthread_t handle[4];

    for(int j = 1; j <= 3; j++) {
        turn = j; //variable to say who's turn it is
        pthread_create(&handle[0], NULL, runDealer, (void*)j); //starts the dealer thread
        //pthread_join(handle[0], NULL);

        while(!dealt); //waits till all the players are dealt a hand

        //Has the players start playing the game
        for(int i = 1; i < 4; i++)
            pthread_create(&handle[i], NULL, runSim, (void*)i);

        //waits for all the player/dealer threads to join back together before going to the next round
        for(int i = 0; i < 4; i++)
            pthread_join(handle[i], NULL);

        //resets the flags for a new round
        winner = false;
        dealt = false;
    }

    myfile.close();
    return 0;
}

void *runSim(void *p)
{
    long player = (long) p;
    string temp;
    int discard = -1;

    while(true)
    {
        while(turn != player)
        {
            ; //busy wait until their turn
            if(winner)
            {
                myfile << "Player " << player << ": exits round";
                goto end;
            }
        }
        //this is to have the player draw a card
        temp = playingDeck.front();
        playingDeck.erase(playingDeck.begin());
        //after the player draws the card they are going to compare cards and
        //react accordingly
        if(player == 1)
        {
            myfile << "Player 1: hand " << hand1.at(0) << endl;
            myfile << "Player 1: draws " << temp << endl;
            hand1.push_back(temp);
            if(hand1[0] == hand1[1])
            {
                //this is to a log file
                myfile << "Player 1: hand " << hand1.at(0) << " " << hand1.at(1) << endl;
                myfile << "Player 1: wins" << endl;
                myfile << "Player 1: exits round" << endl;

                //this is the display to the command line when a winner happens
                cout << "PLAYER 1: " << endl;
                cout << "HAND " << hand1.at(0) << " " << hand1.at(1) << endl;
                cout << "WIN yes" << endl;
                cout << "PLAYER 2: " << endl;
                cout << "HAND " << hand2.at(0) << endl;
                cout << "WIN no" << endl;
                cout << "PLAYER 3: " << endl;
                cout << "HAND " << hand3.at(0) << endl;
                cout << "WIN no" << endl;
                displayDeck(0);
                winner = true;

                //refill the deck
                temp = hand1.front();
                hand1.erase(hand1.begin());
                playingDeck.push_back(temp);
                temp = hand1.front();
                hand1.erase(hand1.begin());
                playingDeck.push_back(temp);
                temp = hand2.front();
                hand2.erase(hand2.begin());
                playingDeck.push_back(temp);
                temp = hand3.front();
                hand3.erase(hand3.begin());
                playingDeck.push_back(temp);

                goto end;
            } else{
                //all the output will need to be to the log file
                discard = randomNumber(hand1.size());
                myfile << "Player 1: discards " << hand1.at(discard) << endl;

                playingDeck.push_back(hand1.at(discard));
                hand1.erase(hand1.begin() + discard); //deletes card from player hand, idk why it doesn't recognize the function call now....
                displayDeck(1); //displays to the logfile
            }
        }
        else if (player == 2)
        {
            myfile << "Player 2: hand " << hand2.at(0) << endl;
            myfile << "Player 2: draws " << temp << endl;
            hand2.push_back(temp);
            if(hand2[0] == hand2[1])
            {
                //this is to a log file
                myfile << "Player 2: hand " << hand2.at(0) << " " << hand2.at(1) << endl;
                myfile << "Player 2: wins" << endl;
                myfile << "Player 2: exits round" << endl;

                //this is the display to the command line when a winner happens
                cout << "PLAYER 1: " << endl;
                cout << "HAND " << hand1.at(0) << endl;
                cout << "WIN no" << endl;
                cout << "PLAYER 2: " << endl;
                cout << "HAND " << hand2.at(0)  << " " << hand2.at(1) << endl;
                cout << "WIN yes" << endl;
                cout << "PLAYER 3: " << endl;
                cout << "HAND " << hand3.at(0) << endl;
                cout << "WIN no" << endl;
                displayDeck(0);
                winner = true;

                //refill the deck
                temp = hand1.front();
                hand1.erase(hand1.begin());
                playingDeck.push_back(temp);
                temp = hand2.front();
                hand2.erase(hand2.begin());
                playingDeck.push_back(temp);
                temp = hand2.front();
                hand2.erase(hand2.begin());
                playingDeck.push_back(temp);
                temp = hand3.front();
                hand3.erase(hand3.begin());
                playingDeck.push_back(temp);

                goto end;
            } else{
                //all the output will need to be to the log file
                discard = randomNumber(hand2.size());
                myfile << "Player 2: discards " << hand2.at(discard) << endl;

                playingDeck.push_back(hand2.at(discard));
                hand2.erase(hand2.begin() + discard); //deletes card from player hand, idk why it doesn't recognize the function call now....
                displayDeck(1); //displays to the logfile
            }
        }
        else if (player == 3)
        {
            myfile << "Player 3: hand " << hand3.at(0) << endl;
            myfile << "Player 3: draws " << temp << endl;
            hand3.push_back(temp);
            if(hand3[0] == hand3[1])
            {
                //this is to a log file
                myfile << "Player 3: hand " << hand3.at(0) << " " << hand3.at(1) << endl;
                myfile << "Player 3: wins" << endl;
                myfile << "Player 3: exits round" << endl;

                //this is the display to the command line when a winner happens
                cout << "PLAYER 1: " << endl;
                cout << "HAND " << hand1.at(0)  << endl;
                cout << "WIN no" << endl;
                cout << "PLAYER 2: " << endl;
                cout << "HAND " << hand2.at(0) << endl;
                cout << "WIN no" << endl;
                cout << "PLAYER 3: " << endl;
                cout << "HAND " << hand3.at(0) << " " << hand3.at(1) << endl;
                cout << "WIN yes" << endl;
                displayDeck(0);
                winner = true;

                //refill the deck
                temp = hand1.front();
                hand1.erase(hand1.begin());
                playingDeck.push_back(temp);
                temp = hand2.front();
                hand2.erase(hand2.begin());
                playingDeck.push_back(temp);
                temp = hand3.front();
                hand3.erase(hand3.begin());
                playingDeck.push_back(temp);
                temp = hand3.front();
                hand3.erase(hand3.begin());
                playingDeck.push_back(temp);

                goto end;
            } else{
                //all the output will need to be to the log file
                discard = randomNumber(hand3.size());
                myfile << "Player 3: discards " << hand3.at(discard) << endl;

                playingDeck.push_back(hand3.at(discard));
                hand3.erase(hand3.begin() + discard); //deletes card from player hand, idk why it doesn't recognize the function call now....
                displayDeck(1); //displays to the logfile
            }
        }

        turn = turn % 3 + 1;

    }
    end:    ;//jumps out of the loops to end the thread
}

void *runDealer(void *dud)
{
    myfile << "DEALER: shuffle" << endl;

    //shuffles the deck
    shuffle(playingDeck.begin(), playingDeck.end(), std::mt19937(std::random_device()()));
    //deals a card to player 1
    hand1.push_back(playingDeck.front());
    playingDeck.erase(playingDeck.begin());
    //deals a card to player 2
    hand2.push_back(playingDeck.front());
    playingDeck.erase(playingDeck.begin());
    //deals a card to player 3
    hand3.push_back(playingDeck.front());
    playingDeck.erase(playingDeck.begin());

    dealt = true;

    while(true)
    {
        if(winner)
            break;
    }
}

void displayDeck(int display)
{
    if(display == 0)
    {
        cout << "DECK: ";
        for(int i = 0; i < playingDeck.size(); i++)
        {
            cout << playingDeck.at(i) << " ";
        }
        cout << endl << endl;
    }else if(display == 1)
    {
        myfile << "DECK: ";
        for(int i = 0; i < playingDeck.size(); i++)
        {
            myfile << playingDeck.at(i) << " ";
        }
        myfile << endl << endl;
    }
}

int randomNumber(int size)
{
    return rand() % size;
}
