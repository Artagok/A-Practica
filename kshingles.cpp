#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
using namespace std;

/************GLOBAL******************/
int k;                              //
// Key  = id_document               //
// Info = set of unique k-shingles  //
map < string, set<string> > m;      //
/************************************/


/*** CITAR GeeksForGeeks al link --> https://www.geeksforgeeks.org/remove-extra-spaces-string/ ***/
// Function to in-place trim all spaces in the 
// string such that all words should contain only 
// a single space between them. 
void remove_spaces(string &str) { 
    // n is length of the original string 
    int n = str.length(); 
  
    // i points to next postion to be filled in 
    // output string/ j points to next character 
    // in the original string 
    int i = 0, j = -1; 
  
    // flag that sets to true is space is found 
    bool spaceFound = false; 
  
    // Handles leading spaces 
    while (++j < n && str[j] == ' '); 
  
    // read all characters of original string 
    while (j < n) 
    { 
        // if current characters is non-space 
        if (str[j] != ' ') 
        { 
            // remove preceding spaces before dot, 
            // comma & question mark 
            if ((str[j] == '.' || str[j] == ',' || 
                 str[j] == '?') && i - 1 >= 0 && 
                 str[i - 1] == ' ') 
                str[i - 1] = str[j++]; 
  
            else
                // copy current character at index i 
                // and increment both i and j 
                str[i++] = str[j++]; 
  
            // set space flag to false when any 
            // non-space character is found 
            spaceFound = false; 
        } 
        // if current character is a space 
        else if (str[j++] == ' ') 
        { 
            // If space is encountered for the first 
            // time after a word, put one space in the 
            // output and set space flag to true 
            if (!spaceFound) 
            { 
                str[i++] = ' '; 
                spaceFound = true; 
            } 
        } 
    } 
  
    // Remove trailing spaces 
    if (i <= 1) 
        str.erase(str.begin() + i, str.end()); 
    else
        str.erase(str.begin() + i - 1, str.end()); 
}

void remove_newlines(string& str) {

    for(int i = 0; i < str.size(); ++i)
        if(str[i] == '\n')
            str[i] = ' ';
}

void fill_map(const string& path, const string& content) {
    set<string> aux;
    int i = 0;
    while (i + k <= content.size()) {
        aux.insert(content.substr(i,k));
        ++i;
    }
    m.insert(make_pair(path,aux));
}

void obtain_Jsim_AB(const string& pathA, const string& pathB) {
    
    int intersection = 0;

    for (set<string>::iterator itA = m[pathA].begin(); itA != m[pathA].end(); ++itA) {
        // Meaning we either have found the word of A in B (MATCH) or we can stop searching in B because we reached 
        // wordA < wordB and due to the fact that set<string> are ordered alphabetically (char by char by its ASCII code)
        // we've guaranteed that wordA will not be in B from this point onwards.
        bool match_or_gt = false; 
        for (set<string>::iterator itB = m[pathB].begin(); itB != m[pathB].end() and (not match_or_gt); ++itB) {

            // Shingle MATCH --> Shingle in A∩B
            if (*itA == *itB) {
                intersection++;
                match_or_gt = true;
            }
            // WordA < WordB --> WordA is NOT in B
            else if (*itA < *itB) {
                match_or_gt = true;
            }
        }
    }

    int reunion =   (m[pathA].size() - intersection) + 
                    (m[pathB].size() - intersection) + 
                    intersection;

    cout << endl;
    double Jsim = (double) intersection / (double) reunion;
    cout << "Jsim(A,B) = " << Jsim << endl;
}

// Case 1 in Switch
// We only compare 2 documents given their paths
void compare_2_docs() {
    
    cout << "Please, enter the paths for both documents." << endl;
    string pathA, pathB;
    cout << "Path for first document  (A): "; cin >> pathA;
    cout << "Path for second document (B): "; cin >> pathB;

    ifstream ifsA(pathA.c_str());
    ifstream ifsB(pathB.c_str());

    stringstream bufferA;
    stringstream bufferB;

    if (ifsA.fail()) {
        cout << "ERROR: cannot find/read first file" << endl;
        return;
    }
    if (ifsB.fail()) {
        cout << "ERROR: cannot find/read second file" << endl;
        return;
    }

    bufferA << ifsA.rdbuf();
    bufferB << ifsB.rdbuf();
    string contentsA = bufferA.str();
    string contentsB = bufferB.str();

    remove_newlines(contentsA);
    remove_newlines(contentsB);

    remove_spaces(contentsA);
    remove_spaces(contentsB);

    fill_map(pathA, contentsA);
    fill_map(pathB, contentsB);

    
    for (set<string>::iterator it = m[pathA].begin(); it != m[pathA].end(); ++it) {
        cout << *it << "|";
    }
    cout << endl << endl << endl;


    for (set<string>::iterator it = m[pathB].begin(); it != m[pathB].end(); ++it) {
        cout << *it << "|";
    }
    cout << endl;

    obtain_Jsim_AB(pathA,pathB);
}


int main() {

    m.clear();
    cout << "Enter the desired value for k (the length of the shingles [k > 0])" << endl;
    cin >> k;

    int mode;
    cout << "Select the comparation mode to obtain Jaccard Similarity:" << endl;
    cout << "\t1 -> Compare 2 documents given their paths in the file system." << endl;
    cout << "\t2 -> Compare ALL files in a directory among them given a path to the said directory." << endl;
    
    while (cin >> mode and (mode != 1 and mode != 2)) {
        cout << "Choose the desired mode [1/2]" << endl;
    }

    switch(mode) {
        case 1:
            compare_2_docs();
            break;
        
        case 2:
            //compare_whole_directory();
            break;

        default:
            cout << "ERROR: Not a valid mode [1/2]" << endl;
            break;
    }
}