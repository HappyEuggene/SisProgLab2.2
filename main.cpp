#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Automaton {
public:
    int numAlphabet;
    int numStates;
    int initialState;
    set<int> finalStates;
    map<pair<int, char>, set<int>> transitions;

    Automaton(const string& filename) {
        ifstream fin(filename);

        fin >> numAlphabet;
        fin >> numStates;
        fin >> initialState;

        int numFinalStates;
        fin >> numFinalStates;
        for (int i = 0; i < numFinalStates; i++) {
            int state;
            fin >> state;
            finalStates.insert(state);
        }

        int s1, s2;
        char a;
        while (fin >> s1 >> a >> s2) {
            transitions[{s1, a}].insert(s2);
        }
        fin.close();
    }

    set<int> nextStates(int state, char a) {
        if (transitions.find({state, a}) != transitions.end())
            return transitions[{state, a}];
        return {};
    }

    bool acceptsWord(const string& w) {
        set<int> currentStates = {initialState};
        for (char c : w) {
            set<int> next;
            for (int s : currentStates) {
                for (int s2 : nextStates(s, c)) {
                    next.insert(s2);
                }
            }
            swap(currentStates, next);
        }

        for (int s : currentStates)
            if (finalStates.find(s) != finalStates.end())
                return true;

        return false;
    }

    bool admitsForm(const string& w0) {
        for (int i = 0; i <= numStates; i++) {
            string w1;
            for (int s = 0; s < i; s++) {
                w1 += "a"; // just an example, can use any symbol
            }
            if (acceptsWord(w1 + w0))
                return true;
        }
        return false;
    }
};

int main() {
    Automaton automaton("test4.txt");
    char choice;

    do {
        string w0;
        cout << "Enter the word w0: ";
        cin >> w0;

        if (automaton.admitsForm(w0))
            cout << "The automaton admits words of the form w = w1w0." << endl;
        else
            cout << "The automaton doesn't admit words of the form w = w1w0." << endl;

        cout << "Do you want to enter again? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}