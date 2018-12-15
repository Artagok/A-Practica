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

void print_map() {

    for (map<string,set<string> >:: iterator it = m.begin(); it != m.end(); ++it) {
        for (set<string>::iterator at = it->second.begin(); at != it->second.end(); ++at) {
            cout << *at << "|";
        }
        cout << endl;
        cout << endl;
    }
}


/* Reads the directory path where all documents to compare are stored */
bool get_dir(string& path_dir) {

    cout << "Comparison of N documents using LSH (Locality-Sensitive Hashing) with MinHash" << endl;
    cout << "Enter the path of the directory where all the documents to compare are stored: ";
    if (cin >> path_dir)
        return true;
    return false;
}

void imprimirBuckets(vector<vector<int> > &buckets){
	cout << "Buckets content:" << endl;
	for (int i = 0; i < buckets.size(); i++){
		cout << "[ " << i << " ] :";
		for (int j = 0; j < buckets[i].size(); j++){
			cout << " " << buckets[i][j];
		}
		cout << endl;
	}
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

    /*
    for (int i = 0; i < dir_entries.size(); ++i)
        cout << dir_entries[i] << endl;
    cout << endl;
    */
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


//AQUI COMENSA EL LSH

void union_sets(set<string> s1_set, set<string> s2_set, set<string> &s3_set) {
    for(set<string>::iterator itA = s1_set.begin(); itA != s1_set.end(); ++itA)
        s3_set.insert(*itA);
    for(set<string>::iterator itB = s2_set.begin(); itB != s2_set.end(); ++itB)
        s3_set.insert(*itB);
}

void fill_n_index(set<string> s3_set, vector< vector<int> > &s){

    int index_s;
    index_s = 0;


    vector< set<string>::iterator>  v_it;

    for(map < string, set<string> >:: iterator it = m.begin(); it != m.end(); ++it){
        set<string>::iterator it_aux = (it -> second).begin();
        v_it.push_back(it_aux);
    }

    for (set<string>::iterator it3 = s3_set.begin(); it3 != s3_set.end(); ++it3) {

        for(int i = 0; i < v_it.size(); ++i){
            if(*it3 == *v_it[i]){
                s[i].push_back(index_s);
                ++v_it[i];
            }

        }
        index_s++;
    }
}

double compute_min_hash(int tam, const vector<pair<int,int> >& h, vector<int> s1, vector<int> s2){

    ///////////////////////////////
    /////COMPUTAR MINHASHES ///////
    ///////////////////////////////

    	vector <vector<int> > t (h.size(), vector<int>(2,-1));
    	for (int i = 0; i < tam; i++) //per cada shingle
    	{
    		for (int j = 0; j < 2; j++) //documents
    		{
    			for (int k = 0; k < h.size() ; k++) //hash function
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

    ////////////////////////////////////////////////////
    /////VEURE SIMILITUDS ENTRE ELS DOS MINHASHES///////
    ////////////////////////////////////////////////////

    	int eq = 0;
    	for (int i = 0; i < h.size(); i++){
    		if (t[i][0] == t[i][1]) eq++;
    	}
        return (double) eq/ (double) h.size();

}



void compute_band_lsh(int ith_band, int tam, const vector<vector<int> >& S, int r, vector<vector<int> >& buckets, double treshold){


	vector<pair<int,int> > h;
	int m,n;
	for (int i = 0; i < r; i++){
		//eq de la forma m*x + n = y
		m = rand() % tam;
		n = rand() % tam;
		h.push_back(make_pair(m,n));
	}

/*	cout << "contingut d'H:" << endl << endl;

	for (int i = 0; i < h.size(); i++){
		cout << h[i].first << " " << h[i].second << endl;
	}
	cout << endl;
*/
	double similarity;
	for (int i = 0; i < S.size(); i++)
	{
		if ( i + 1 < S.size())
		{
			for (int j = i + 1; j < S.size(); j++)
			{
				similarity = compute_min_hash(tam,h,S[i],S[j]);

				//cout << i << j <<" "<<similarity<<endl;

				if (similarity > treshold){

					//posar i
					bool esta = false;
					for (int k = 0; k<buckets[ith_band].size() and not esta; k++){
						if (buckets[ith_band][k]==i) esta = true;
					}
					if (not esta) buckets[ith_band].push_back(i);
					//posar j
					esta = false;
					for (int k = 0; k<buckets[ith_band].size() and not esta; k++){
						if (buckets[ith_band][k]==j) esta = true;
					}
					if (not esta) buckets[ith_band].push_back(j);
				}
			}
		}
	}
}

// Case 1 in Switch
// We only compare 2 documents given their paths
void compare_n_docs() {

    set<string> s3_set;
    cout << "Documents to compare:" << endl;
    cout << "******************************************" << endl;
    for(map < string, set<string> >:: iterator it = m.begin(); it != m.end(); ++it){
        cout << it -> first << "   " << endl;
        union_sets(it->second, s3_set, s3_set);
    }
    cout << "******************************************" << endl;



/*
    cout << "Resultat d'unir els dos sets" << endl << endl;
    for (set<string>::iterator it = s3_set.begin(); it != s3_set.end(); ++it) {
        cout << *it << "|";
    }
    cout << endl;
*/

    vector< vector<int> > s(m.size());        //s = matriu de sets
    fill_n_index(s3_set, s);


    int num_b;
    int num_r;
    double treshold;
    int tam = s3_set.size();

    cout << "How many bands do you want the Signature Matrix to be split in? ";
    cin >> num_b;
    cout << "How many Hash Function do you want for each band? ";
    cin >> num_r;
    cout << "Establish the threshold t meaning that if two documents have a simulated Jsim less than t they will not be considered as candidates: ";
    cin >> treshold;

    vector<vector<int> > buckets (num_b);
    for (int b = 0; b < num_b; b++){
		compute_band_lsh(b, tam, s, num_r, buckets, treshold);
    }
    imprimirBuckets(buckets);


/*
    cout << "Aquests son els index " << endl;

    for(int i = 0; i < s.size(); i++){
        for(int j = 0; j < s[i].size(); j++){
            cout << s[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
*/

}









int main() {
    srand (time(NULL)); //set seed

    m.clear();
    cout << "Enter the desired value for k (the length of the shingles [k > 0])" << endl;
    cin >> k;
    string path_dir;
    vector<string> dir_entries;

    while ( not (get_dir(path_dir) and open_dir(path_dir, dir_entries) and fill_map_general(dir_entries))) {
        cout << "ERROR: cannot find/read " << path_dir << endl;
    }

    compare_n_docs();


}
