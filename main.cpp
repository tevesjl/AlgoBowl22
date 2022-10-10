#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

// function definitions
void SimulatedAnnealing(int, map<int, vector<pair<int, int>>>&, map<int, int>&);
int getNumberOfCorrectClauses(map<int, vector<pair<int, int>>>&, map<int, int>&, vector<pair<int, int>>&);
int getVariableCorrectClauses(int, map<int, vector<pair<int, int>>>&, map<int, int>&);
int checkClause(pair<int, int>, map<int, int>&);
void addClause(pair<int, int>, map<int, vector<pair<int, int>>>&, map<int, int>&, vector<pair<int, int>>&);
void setTruthValue(int, map<int, int>&);

// global variables
int MULTIPLIER[] = {-1, 1};
int MULTIPLIER_SIZE = 2;
double T_FACTOR = 5;
double ALPHA = 1.65;
int ITERATION_FACTOR = 5;
int TEMP_DECREASE = 5;

int main() {
    // set a seed
    srand(time(0)); // note that this is pseudo-random: has slight bias towards lower numbers

    // input stream
    ifstream input("prototype_input_fixed.txt");
    //ifstream input("mini_input.txt");
    if (input.fail()) {
        cerr << "Failed to open input file." << endl;
        return -1;
    }

    // variable declarations
    int numClauses, numVariables, firstVal, secondVal;
    bool firstLine = true;
    string line;
    vector<pair<int, int>> clauses; // vector that simply contains all clauses
    map<int, vector<pair<int, int>>> clauseMap; // key: variable, value: all clauses that contain the key
    map<int, int> truthValues; // key: variable (1, 2, etc), value: -1 (false) or 1 (true)

    // read the input file
    while (!input.eof()) {
        getline(input, line);
        stringstream ss(line);

        if (!firstLine) {
            ss >> firstVal;
            ss >> secondVal;
            pair<int, int> clause = make_pair(firstVal, secondVal);
            addClause(clause, clauseMap, truthValues, clauses);
        }
        else {
            ss >> numClauses;
            ss >> numVariables;
            firstLine = false;
        }
    }
    input.close();

    // do simulated annealing and get number of correct clauses after
    SimulatedAnnealing(numVariables, clauseMap, truthValues);
    int numCorrectClauses = getNumberOfCorrectClauses(clauseMap, truthValues, clauses);
    cout << numClauses << endl;
    cout << numCorrectClauses;

    // output stream
    ofstream output("output.txt");
    if (output.fail()) {
        cerr << "Failed to create output file." << endl;
        return -1;
    }
    // writing to output.txt
    output << numCorrectClauses << endl;
    for (int i = 1; i <= truthValues.size(); i++) {
        // on all other lines, add newlines
        if (i < truthValues.size()) {
            // output wants '-1' to be 0 for false, so manually add '0' instead of '-1'
            if (truthValues.at(i) < 0) {
                output << "0" << endl;
            }
            else {
                output << truthValues.at(i) << endl;
            }
        }
        // on the last line, do not do a newline
        else {
            if (truthValues.at(i) < 0) {
                output << "0";
            }
            else {
                output << truthValues.at(i);
            }
        }
    }
    output.close();

    return 0;
}

// SimulatedAnnealing: implements the process of simulated annealing to get close to the optimal solution
void SimulatedAnnealing(int numVariables, map<int, vector<pair<int, int>>>& clauseMap, map<int, int>& truthValues) {
    //double temperature = T_FACTOR * numVariables;
    double temperature = 1000;
    int variable = 1;
    int numIterations = 0;

    while (temperature > 0) {
        int currentState = getVariableCorrectClauses(variable, clauseMap, truthValues);
        // change truth value and get the number of correct clauses
        truthValues.at(variable) = truthValues.at(variable) * -1;
        int nextState = getVariableCorrectClauses(variable, clauseMap, truthValues);

        double stateChange = nextState - currentState;
        if (stateChange < 0) {
            truthValues.at(variable) = truthValues.at(variable) * -1;
            /*
            double probability = exp(stateChange / temperature);
            // if we do not change truth value
            if (rand() / RAND_MAX > probability) {
                truthValues.at(variable) = truthValues.at(variable) * -1;
            } */
        }

        // temperature change: determines how much we move on the "curve"
        //temperature -= TEMP_DECREASE;
        // iterate through all the variables on the first run, then every ITERATION_FACTOR variables afterwards
        if (numIterations % ITERATION_FACTOR == 0 && numIterations > numVariables) {
            temperature = temperature / (1 + ALPHA*(log(1+numIterations)));
        }
        else {
            temperature = temperature / (1 + ALPHA*(log(1+numIterations)));
        }

        // if haven't gone over all the variables, keep going
        if (variable < numVariables) {
            variable++;
        }
        // else restart at the first variable
        else {
            variable = 1;
        }
        numIterations++;
    }
}

// getVariableCorrectClauses: gets the current number of correct clauses based on the variable's truth value
int getVariableCorrectClauses(int variable, map<int, vector<pair<int, int>>>& clauseMap, map<int, int>& truthValues) {
    int numCorrectClauses = 0;
    vector<pair<int, int>> clauseList = clauseMap.at(variable);
    for (pair<int, int> clause : clauseList) {
        if (checkClause(clause, truthValues) > 0) {
            numCorrectClauses++;
        }
    }
    return numCorrectClauses;
}

// getNumberOfCorrectClauses: gets the current number of correct clauses based on the current truth values
int getNumberOfCorrectClauses(map<int, vector<pair<int, int>>>& clauseMap, map<int, int>& truthValues, vector<pair<int, int>>& clauses) {
    int numCorrectClauses = 0;
    for (pair<int, int> clause : clauses) {
        if (checkClause(clause, truthValues) > 0) {
            numCorrectClauses++;
        }
    }
    return numCorrectClauses;
}

// checkClause: returns whether the current clause is true or not (returns -1 or 1)
int checkClause(pair<int, int> clause, map<int, int>& truthValues) {
    int firstVal = truthValues.at(abs(clause.first));
    int secondVal = truthValues.at(abs(clause.second));

    if ((firstVal * clause.first) > 0 || (secondVal * clause.second) > 0) {
        return 1;
    }
    return -1;
}

/* addClause: Adds a clause to the clauseMap and sets variable's truth value if needed
 * Case 1: if the current clause variable is already mapped, add the clause to the vector
 * Case 2: else create a vector, map the variable, and call setTruthValue
*/
void addClause(pair<int, int> clause, map<int, vector<pair<int, int>>>& clauseMap, map<int, int>& truthValues, vector<pair<int, int>>& clauses) {
    clauses.push_back(clause);

    // first variable
    if (clauseMap.count(abs(clause.first))) { // if the variable is already mapped, add clause
        clauseMap.at(abs(clause.first)).push_back(clause);
    }
    else { // else map the variable and call setTruthValue
        vector<pair<int, int>> clauseList;
        clauseList.push_back(clause);
        clauseMap.emplace(abs(clause.first), clauseList);
        if (!truthValues.count(abs(clause.first))) {
            setTruthValue(abs(clause.first), truthValues);
        }
    }

    // second variable
    if (clauseMap.count(abs(clause.second))) { // if the variable is already mapped, add clause
        clauseMap.at(abs(clause.second)).push_back(clause);
    }
    else { // else map the variable and call setTruthValue
        vector<pair<int, int>> clauseList;
        clauseList.push_back(clause);
        clauseMap.emplace(abs(clause.second), clauseList);
        if (!truthValues.count(abs(clause.second))) {
            setTruthValue(abs(clause.second), truthValues);
        }
    }
}

// setTruthValue: uses rng to randomly set the truth value for the given variable
void setTruthValue(int variable, map<int, int>& truthValues) {
    //get a random truth value and map it
    int truthValue = MULTIPLIER[rand() % MULTIPLIER_SIZE];
    truthValues.emplace(variable, truthValue);
}
