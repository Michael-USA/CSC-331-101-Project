// voteCounter.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define PROCEDURAL_NAME(base,extension) CONCAT(base, extension)
using namespace std;
namespace fs = std::filesystem;

class tentativeResult {
private:
    vector <int> votesPerCandidate;
    vector <string> candidates;
    string filename;
public:
    void databaseAccessor();
    tentativeResult(string n) {
        filename = n;
    }
    vector <int> getVotesPerCandidate() {
        return votesPerCandidate;
    }
    vector <string> getCandidates() {
        return candidates;
    }
    string getFilename() {
        return filename;
    }
};

void tentativeResult::databaseAccessor() {
    fstream strm;
    int candidatesSize = 0;
    int i;
    const int max_name_length = 20;
    bool contains = false;

    strm.open(filename, ios_base::in);
    if (strm.is_open()) {
        char arr[max_name_length];
        string blankChecker;
        while (!strm.eof()) {
            strm.getline(arr, max_name_length);
            blankChecker = arr;
            if (!blankChecker.size()) {
                if (candidatesSize == 0) {
                    candidates.push_back("There were no candidates.");
                    votesPerCandidate.push_back(0);
                }
                break;
            }
            if (candidatesSize == 0) {
                candidatesSize++;
                candidates.push_back(arr);
                votesPerCandidate.push_back(1);
            }
            else {
                for (i = 0; i < candidatesSize; i++) {
                    contains = (arr == candidates.at(i));
                    if (contains) {
                        votesPerCandidate.at(i) += 1;
                        break;
                    }
                }
                if (!contains) {
                    candidatesSize++;
                    candidates.push_back(arr);
                    votesPerCandidate.push_back(1);
                }
            }
        }

        strm.close();
        if (strm.is_open())
            cout << "Stream could not close!" << endl;
    }
}

class electionResults {
public:
    static bool validate(tentativeResult unvalidatedElection);
    // export as csv
    static void sendToDatabase() {}
};

bool electionResults::validate(tentativeResult unvalidatedElection) {
    for (int i = 0; i < 20; i++) {
        fstream strm;
        int candidatesSize = 0;
        const int max_name_length = 20;
        bool contains = false;
        vector <string> validateCandidates;
        vector <int> validateVotesPerCandidate;
        strm.open(unvalidatedElection.getFilename(), ios_base::in);
        if (strm.is_open()) {
            char arr[max_name_length];
            string blankChecker;
            while (!strm.eof()) {
                strm.getline(arr, max_name_length);
                blankChecker = arr;
                if (!blankChecker.size()) {
                    if (candidatesSize == 0) {
                        validateCandidates.push_back("There were no candidates.");
                        validateVotesPerCandidate.push_back(0);
                    }
                    break;
                }
                if (candidatesSize == 0) {
                    candidatesSize++;
                    validateCandidates.push_back(arr);
                    validateVotesPerCandidate.push_back(1);
                }
                else {
                    for (int i = 0; i < candidatesSize; i++) {
                        contains = (arr == validateCandidates.at(i));
                        if (contains) {
                            validateVotesPerCandidate.at(i) += 1;
                            break;
                        }
                    }
                    if (!contains) {
                        candidatesSize++;
                        validateCandidates.push_back(arr);
                        validateVotesPerCandidate.push_back(1);
                    }
                }
            }

            strm.close();
            if (strm.is_open())
                cout << "Stream could not close!" << endl;
        }
        for (int j = 0; j < unvalidatedElection.getVotesPerCandidate().size(); j++) {
            if (!(validateCandidates.at(j) == unvalidatedElection.getCandidates().at(j)) || !(validateVotesPerCandidate.at(j) == unvalidatedElection.getVotesPerCandidate().at(j))) {
                return false;
            }
        }
    }
    fstream fout;
    fout.open("validated" + unvalidatedElection.getFilename(), ios::out);
    fout << "Election Date" << ',' << "ID" << ',' << "Candidates" << ',' << "votecounts" << ',' << "Topic" << ',' << "Start Date" << ',' << "End Date" << '\n';
    fout << "Date would go here" << ',' << "ID would go here" << ',' << unvalidatedElection.getCandidates().at(0) << ',' << unvalidatedElection.getVotesPerCandidate().at(0) << ',' << "Topic would go here" << ',' << "Start Date would go here" << ',' << "End Date would go here" << '\n';

    for (int j = 1; j < unvalidatedElection.getCandidates().size(); j++) {
        fout << ",," << unvalidatedElection.getCandidates().at(j) << ", " << unvalidatedElection.getVotesPerCandidate().at(j) << '\n';
    }
    fout.close();
    return true;
}

class tabulationInterface {
public: 
    //this is a stub
    static bool flagElectionEnd() {
        return true;
    }
};

int main()
{
    // Access the raw result files
    vector <string> resultList;
    string path = "Results/";
    for (const auto& entry : fs::directory_iterator(path)) {
        string pathString{ entry.path().u8string() };
        //cout << pathString.substr(pathString.find("Results/")) << endl;
        resultList.push_back(pathString.substr(pathString.find("Results/")));
    }

    // Tabulate the result files, and add the class objects to an array
    vector <tentativeResult> tabulatedResultList;
    for (int i = 0; i < resultList.size(); i++) {
        tentativeResult PROCEDURAL_NAME(result,i)(resultList.at(i));
        PROCEDURAL_NAME(result, i).databaseAccessor();
        tabulatedResultList.push_back(PROCEDURAL_NAME(result, i));
    }

    // validate each result
    for (int i = 0; i < tabulatedResultList.size(); i++) {
        electionResults::validate(tabulatedResultList.at(i));
    }
    
    if (tabulationInterface::flagElectionEnd()) {
        electionResults::sendToDatabase();
    }

    return 0;
}
