#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

int main() {

    ifstream input("test.txt");
    if (input.fail()) {
        cerr << "Failed to open test.txt file." << endl;
        return -1;
    }
    string line;
    int numClauses, numVariables, num;
    int i = 0;
    map<int, int> clauses;

    while (!input.eof()) {
        getline(input, line);
        stringstream ss(line);
        while (ss >> num) {
            if (i == 0) {
                numClauses = num;
            }
            else if (i == 1) {
                numVariables = num;
            }
            else {
                // if the current variable is already mapped, add 1
                if (clauses.count(num)) {
                    int newValue = clauses.at(num) + 1;
                    clauses.erase(num);
                    clauses.emplace(num, newValue);
                }
                else {
                    clauses.emplace(num, 1);
                }
            }
            i++;
        }
    }
    input.close();

    cout << clauses.at(1) << endl;
    cout << clauses.at(2) << endl;
    cout << clauses.at(-1) << endl;
    cout << clauses.at(-2) << endl;

    return 0;
}