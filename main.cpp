#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>

using namespace std;

void addClause(pair<int, int>, map<int, vector<pair<int, int>>>&, map<int, int>&);
void setTruthValue(int, map<int, int>&);

int MULTIPLIER[] = {-1, 1};
int MULTIPLIER_SIZE = 2;

int main() {
    srand(time(0)); // note that this is pseudo-random: has slight bias towards lower numbers
    ifstream input("test.txt");
    if (input.fail()) {
        cerr << "Failed to open test.txt file." << endl;
        return -1;
    }

    int numClauses, numVariables, num, firstVal, secondVal;
    int i = 0;
    map<int, int> variableCount;
    map<int, vector<pair<int, int>>> clauseMap;
    map<int, int> truthValues; // key: variable (1, 2, etc), value: -1 (false) or 1 (true)
    string line;
    bool firstLine = true;

    while (!input.eof()) {
        getline(input, line);
        stringstream ss(line);

        if (!firstLine) {
            ss >> firstVal;
            ss >> secondVal;
            pair<int, int> clause = make_pair(firstVal, secondVal);
            addClause(clause, clauseMap, truthValues);
        }
        else {
            ss >> numClauses;
            ss >> numVariables;
            firstLine = false;
        }
    }
    input.close();

    cout << "Checking clause map" << endl;
    for (i = 0; i < clauseMap.at(1).size(); i++) {
        cout << clauseMap.at(1).at(i).first << " " << clauseMap.at(1).at(i).second << endl;
    }
    for (i = 0; i < clauseMap.at(-1).size(); i++) {
        cout << clauseMap.at(-1).at(i).first << " " << clauseMap.at(-1).at(i).second << endl;
    }
    cout << endl << "Checking truth values" << endl;
    cout << "There are " << truthValues.size() << " variables" << endl;
    cout << "1 : " << truthValues.at(1) << endl;
    cout << "2 : " << truthValues.at(2) << endl;

    return 0;
}

/* addClause: Adds a clause to the clauseMap and sets variable's truth value if needed
 * Case 1: if the current clause variable is already mapped, add the clause to the vector
 * Case 2: else create a vector, map the variable, and call setTruthValue
*/
void addClause(pair<int, int> clause, map<int, vector<pair<int, int>>>& clauseMap, map<int, int>& truthValues) {
    // first variable
    if (clauseMap.count(clause.first)) { // if the variable is already mapped, add clause
        clauseMap.at(clause.first).push_back(clause);
    }
    else { // else map the variable and call setTruthValue
        vector<pair<int, int>> clauseList;
        clauseList.push_back(clause);
        clauseMap.emplace(clause.first, clauseList);
        if (clause.first > 0) {
            setTruthValue(clause.first, truthValues);
        }
    }

    // second variable
    if (clauseMap.count(clause.second)) { // if the variable is already mapped, add clause
        clauseMap.at(clause.second).push_back(clause);
    }
    else { // else map the variable and call setTruthValue
        vector<pair<int, int>> clauseList;
        clauseList.push_back(clause);
        clauseMap.emplace(clause.second, clauseList);
        if (clause.second > 0) {
            setTruthValue(clause.second, truthValues);
        }
    }
}

// setTruthValue: uses rng to randomly set the truth value for the given variable
void setTruthValue(int variable, map<int, int>& truthValues) {
    //get a random truth value and map it
    int truthValue = variable * MULTIPLIER[rand() % MULTIPLIER_SIZE];
    truthValues.emplace(variable, truthValue);
}
