#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
/* Directories and files */
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
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

/* Reads the directory path where all documents to compare are stored */
bool get_dir(string& path_dir) {

    cout << "Comparison of N documents using LSH (Locality-Sensitive Hashing) with MinHash" << endl;
    cout << "Enter the path of the directory where all the documents to compare are stored: ";
    if (cin >> path_dir)
        return true;
    return false;
}

/* Tries to open the directory and fills a vector with all its entries */
bool open_dir(string& path_dir, vector<string>& dir_entries) {

    DIR* d = opendir(path_dir.c_str());

    if (d == NULL)
        return false;

    if (path_dir[path_dir.length()-1] != '/')
        path_dir += "/";
    
    dirent* dir_entry;
    while ( (dir_entry = readdir(d)) != NULL ) {
        string entry_name = dir_entry->d_name;
        if (entry_name != "." and entry_name != "..")
            dir_entries.push_back(path_dir + entry_name);
    }
    closedir(d);

    for (int i = 0; i < dir_entries.size(); ++i)
        cout << dir_entries[i] << endl;
    cout << endl;
    return true;
}

bool fill_map_general(const vector<string>& dir_entries) {

    for (int i = 0; i < dir_entries.size(); ++i) {

        ifstream ifs(dir_entries[i].c_str());

        if (ifs.fail()) {
            cout << "ERROR: cannot find/read " << dir_entries[i] << endl;
        }
        else {
            stringstream buffer;
            buffer << ifs.rdbuf();
            string contents = buffer.str();
            remove_newlines(contents);
            remove_spaces(contents);
            fill_map(dir_entries[i],contents);
        }
    }
    return true;
}

void print_map() {

    for (map<string,set<string> >:: iterator it = m.begin(); it != m.end(); ++it) {
        for (set<string>::iterator at = it->second.begin(); at != it->second.end(); ++at) {
            cout << *at << "|";
        }
        cout << endl;
        cout << endl;
    }
}

int main() {

    m.clear();
    cout << "Enter the desired value for k (the length of the shingles [k > 0])" << endl;
    cin >> k;

    /************************ AIXO ES NOU *****************************************/
    
    string path_dir;
    vector<string> dir_entries;

    while ( not (get_dir(path_dir) and open_dir(path_dir, dir_entries) and fill_map_general(dir_entries))) {
        cout << "ERROR: cannot find/read " << path_dir << endl;
    }

    print_map();
    /******************************************************************************/
}