#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>
#include <vector>

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



void union_sets(set<string> s1_set, set<string> s2_set, set<string> &s3_set) {
    for(set<string>::iterator itA = s1_set.begin(); itA != s1_set.end(); ++itA)
        s3_set.insert(*itA);
    for(set<string>::iterator itB = s2_set.begin(); itB != s2_set.end(); ++itB)
        s3_set.insert(*itB);
}

void fill_index(set<string> s1_set, set<string> s2_set, set<string> s3_set, vector<int> &s1, vector<int> &s2){
    int index_s;
    index_s = 0;
    set<string>::iterator it1 = s1_set.begin();
    set<string>::iterator it2 = s2_set.begin();

    for (set<string>::iterator it3 = s3_set.begin(); it3 != s3_set.end(); ++it3) {

        if(*it3 == *it1){
            s1.push_back(index_s);
            it1++;
        }

        if(*it3 == *it2){
            s2.push_back(index_s);
            it2++;
        }

        index_s++;
    }
}

double compute_min_hash(int tam, int num_hashs, int num_docu, vector<int> s1, vector<int> s2){


    	srand (time(NULL));
    	vector<pair<int,int> > h;
    	int m,n;
    	for (int i = 0; i < num_hashs; i++){
    		//eq de la forma m*x + n = y
    		m = rand() % tam;
    		n = rand() % tam;
    		h.push_back(make_pair(m,n));
    	}

    	//cout << "contingut d'H:" << endl << endl;

    	/*for (int i = 0; i < h.size(); i++){
    		cout << h[i].first << " " << h[i].second << endl;
    	}
    	cout << endl;
        */
    ///////////////////////////////
    /////COMPUTAR MINHASHES ///////
    ///////////////////////////////

    	vector <vector<int> > t (num_hashs, vector<int>(num_docu,-1));
    	for (int i = 0; i < tam; i++) //per cada shingle
    	{
    		for (int j = 0; j < num_docu; j++) //documents
    		{
    			for (int k = 0; k < num_hashs ; k++) //hash function
    			{
    				vector<int>::iterator it;
    				if (j == 0){
    					it = find (s1.begin(), s1.end(), i);
    				}
    				else { //j == 1
    					it = find (s2.begin(), s2.end(), i);
    				}
     	 			if ((j==0 and it != s1.end()) or (j==1 and it != s2.end())){

     	 				int res = (h[k].first * i + h[k].second) % tam;

    					if (t[k][j] == -1){
    						t[k][j] = res;
    					}
    					else if(res < t[k][j]){
    						t[k][j] = res;
    					}
    				}
    			}
    		}
    	}
        /*
    	cout << "contingut de T:" << endl << endl;

    	for (int i = 0; i < num_hashs; i++){
    		for (int j = 0; j < num_docu; j++){
    			cout << t[i][j] << " ";
    		}
    		cout << endl;
    	}
    	cout << endl;
        */
    ////////////////////////////////////////////////////
    /////VEURE SIMILITUDS ENTRE ELS DOS MINHASHES///////
    ////////////////////////////////////////////////////

    	int eq = 0;
    	for (int i = 0; i < num_hashs; i++){
    		if (t[i][0] == t[i][1]) eq++;
    	}
        return (double) eq/ (double) num_hashs;

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

    cout << "Do you want to print all " << k <<"-shingles from documents A & B? [Y/N] ";
    string resposta;
    cin >> resposta;
    cout << endl;

    if (resposta == "Y" or resposta == "y") {
        for (set<string>::iterator it = m[pathA].begin(); it != m[pathA].end(); ++it) {
            cout << *it << "|";
        }
        cout << endl << endl << endl;


        for (set<string>::iterator it = m[pathB].begin(); it != m[pathB].end(); ++it) {
            cout << *it << "|";
        }
        cout << endl;
    }

    //Minhash
    set<string> s3_set;

    union_sets(m[pathA], m[pathB], s3_set);
/*
    cout << "Resultat d'unir els dos sets" << endl << endl;
    for (set<string>::iterator it = s3_set.begin(); it != s3_set.end(); ++it) {
        cout << *it << "|";
    }
*/
    vector<int> s1;
    vector<int> s2;

    fill_index(m[pathA], m[pathB], s3_set, s1,s2);


/*
    cout << "Aquests son els index de s1 i s2" << endl;

    for(int i = 0; i < s1.size(); i++) cout << s1[i] << " ";
    cout << endl;
    for(int i = 0; i < s2.size(); i++) cout << s2[i] << " ";
*/

    cout << "Determina quantes funcions de hash vols fer servir: " << endl;

    int num_hashs;
    cin >> num_hashs;
    int num_docu = 2;
    double jacard_sim = compute_min_hash(s3_set.size(), num_hashs, num_docu, s1, s2);
    cout << "Jaccard similarity by minhashing is: " << jacard_sim << endl;
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
