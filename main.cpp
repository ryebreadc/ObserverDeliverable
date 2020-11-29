#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

// base class used by classes that do OBSERVING (i.e., a "display")!
class Observer {
  public:
    virtual void update(int runs, int hits, float win_average, int HR, int GS, int W, int SO)=0;
};

class Subject {
  public:
    virtual void registerObserver(Observer* o)=0;
    virtual void unregisterObserver(Observer* o)=0;
    virtual void notifyObservers()=0;
};

class SportsData:public Subject {
  private:
    int runs;
    int hits;
    float win_average;
    int HR;
    int GS;
    int W;
    int SO;
    vector<Observer*> observerList;

    void notifyObservers() {
      for (int i = 0; i < observerList.size(); i++ ) {
        if (observerList.at(i) != nullptr) {
          observerList.at(i)->update(runs,hits,win_average, HR, GS, W, SO);
        }
      }
    }
    
    int getLatestRuns() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }

    int getLatestHits() {
      return (rand() % 10);
    }

    float getLatestWinAverage() {
      return floorf(((float) rand() / (RAND_MAX)) * 1000) / 1000;
    }

    int getHomeRuns() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }
    int getGrandSlams() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }
    int getStrikeOuts() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }
    int getWalks() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }

  public:
    SportsData() {
        srand(time(0));  // Initialize random number generator.
    }

    void registerObserver(Observer* o) {
      observerList.push_back(o);
    }

    void unregisterObserver(Observer* o) {
      for (int i = 0; i < observerList.size(); i++ ) {
        if (observerList.at(i) == o) {
          observerList.erase(observerList.begin()+i);
        }
      }
    }

    // This method is used update data and displays
    void dataChanged() {
      
      //get latest data
      hits = getLatestHits();
      runs = getLatestRuns();
      win_average = getLatestWinAverage();
      HR = getHomeRuns();
      GS = getGrandSlams();
      W = getWalks();
      SO = getStrikeOuts();
      notifyObservers();
    }

};

class CalcScoreDisplay: public Observer {
  private:
    float runRate;

  public:
    void update(int runs, int hits, float win_average, int HR, int GS, int W, int SO) {
                         
      runRate = (hits > 0)
       ? (ceilf(((float)runs /  (float)hits)*1000)/1000) 
       : 0;
      display();
    }

    void display() {
      cout << "\nRuns Per Hit: \n";
      cout << runRate << "\n";
    }
};

class CurrentScoreDisplay: public Observer {
  private:
    int runs, hits;
    float win_average;

  public:
    void update(int new_runs, int new_hits,
                       float new_win_average, int HR, int GS, int W, int SO) {
      runs = new_runs;
      hits = new_hits;
      win_average = new_win_average;
      display();
    }

    void display() {
      cout << "\nCurrent Score Display:\n";
      cout << "Runs: " << runs;
      cout << "\nHits:" << hits;
      cout << "\nWin Average: " << win_average << "\n";
    }
};

class StatisticsDisplay: public Observer {

  private:
    int HR, GS, W, SO;
    int runs, hits;
    float win_average;
  public:
    void update(int new_runs, int new_hits,
                       float new_win_average, int HR, int GS, int W, int SO) {
      runs = new_runs;
      hits = new_hits;
      win_average = new_win_average;
      updateStats(HR, GS, W, SO);
    }
    void updateStats(int new_HR, int new_GS, int new_W, int new_SO) {
      HR = new_HR;
      GS = new_GS;
      W = new_W;
      SO = new_SO;
      display();
    }

    void display() {
      cout << "|----------------------------" << endl;
      cout << "|Home Runs:" << HR << endl;
      cout << "|Grand Slams:" << GS << endl;
      cout << "|Walks:" << W << endl;
      cout << "|Strike Outs:" << SO << endl;
      cout << "|----------------------------" << endl;
    }

};

int main() {
  // create objects for display
  CalcScoreDisplay calcScoreDisplay = CalcScoreDisplay();
  CurrentScoreDisplay currentScoreDisplay = CurrentScoreDisplay();
  StatisticsDisplay SD = StatisticsDisplay();

  SportsData sportsData = SportsData();

  // register display elements
  sportsData.registerObserver(&calcScoreDisplay);
  sportsData.registerObserver(&currentScoreDisplay);
  sportsData.registerObserver(&SD);

  sportsData.dataChanged();

  //remove an observer and re-run dataChanged()
  sportsData.unregisterObserver(&currentScoreDisplay);
  sportsData.dataChanged();
} 