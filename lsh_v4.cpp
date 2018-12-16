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
map < pair<int,int>, double > m_jac;//
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

double compute_min_hash(int tam, const vector<pair<int,int> >& h,const vector<int> &s1,const vector<int> &s2){

    ///////////////////////////////
    /////COMPUTAR MINHASHES ///////
    ///////////////////////////////
	int eq = 0;

	for (int i = 0; i < h.size(); i++){
		int sig1 = -1;
		for (int j = 0; j < s1.size(); j++){
			int res = (h[i].first * s1[j] + h[i].second) % (tam);
			if (sig1 == -1) sig1 = res;
			else if(res < sig1) sig1 = res;
		}
		int sig2 = -1;
		for (int j = 0; j < s2.size(); j++){
			int res = (h[i].first * s2[j] + h[i].second) % (tam);
			if (sig2 == -1) sig2 = res;
			else if(res < sig2) sig2 = res;
		}
		if (sig1 == sig2) eq++;
        //cout << sig1 << "  " << sig2 << endl;
	}

	return ((double) eq / (double) h.size());

}


void compute_band_lsh(int ith_band, int tam, const vector<vector<int> >& S, int r, vector<vector<int> >& buckets, double treshold){

	vector<int> ids (S.size(),-1);

	vector<pair<int,int> > h;
	int m,n;
	for (int i = 0; i < r; i++){
		//eq de la forma m*x + n = y
		m = rand() % tam;
		n = rand() % tam;
		h.push_back(make_pair(m,n));
	}

	double similarity;
	for (int i = 0; i < S.size(); i++)
	{
		if ( i + 1 < S.size())
		{
			for (int j = i + 1; j < S.size(); j++)
			{

				if (ids[i] == -1 and ids[j] == -1){
					similarity = compute_min_hash(tam,h,S[i],S[j]);
					if (similarity > treshold){
                        vector<int> v_aux;
                        v_aux.push_back(i);
                        v_aux.push_back(j);
						buckets.push_back(v_aux);
                        ids[i] = buckets.size()-1;
                        ids[j] = buckets.size()-1;

					}
				}

				else if (ids[i] != -1 and ids[j] == -1){
					similarity = compute_min_hash(tam,h,S[i],S[j]);
					if (similarity > treshold){
						buckets[ids[i]].push_back(j);
                        ids[j] = ids[i];
					}
				}
				else if (ids[i] == -1 and ids[j] != -1){
					similarity = compute_min_hash(tam,h,S[i],S[j]);
					if (similarity > treshold){
						buckets[ids[j]].push_back(i);
                        ids[i] = ids[j];
					}
				}
			}
		}
	}
}



double obtain_Jsim_AB(set<string> s1_set, set<string> s2_set) {

    int intersection = 0;

    for (set<string>::iterator itA = s1_set.begin(); itA != s1_set.end(); ++itA) {
        // Meaning we either have found the word of A in B (MATCH) or we can stop searching in B because we reached
        // wordA < wordB and due to the fact that set<string> are ordered alphabetically (char by char by its ASCII code)
        // we've guaranteed that wordA will not be in B from this point onwards.
        bool match_or_gt = false;
        for (set<string>::iterator itB = s2_set.begin(); itB != s2_set.end() and (not match_or_gt); ++itB) {

            // Shingle MATCH --> Shingle in AnB
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

    int reunion =   (s1_set.size() - intersection) +
                    (s2_set.size() - intersection) +
                    intersection;


    return (double) intersection / (double) reunion;

}

set<string> obtenir_set(string &path, int n){
    map<string,set<string> >:: iterator it = m.begin();
    for(int i = 0; i < n; ++i){
        ++it;
    }
    path = it -> first;
    size_t pos_barra = path.rfind("/");
    if(pos_barra != string::npos){
        path = path.substr(pos_barra+1, path.size());
    }
    return it -> second;
}

void minim_jacard(vector<vector<int> > buckets) {
    double max_jac, jac_s1_s2;
    max_jac = 0;
    pair<string,string> mes_similars;

    for(int k = 0; k < buckets.size(); ++k){

        if(buckets[k].size() != 0){
            cout << "These are the (not repeated in a previous bucket) Jaccard Similarities for the bucket " << k << ":" << endl;
            for(int i = 0; i < buckets[k].size()-1;++i) {
                for(int j = i+1; j < buckets[k].size(); ++j){

                    if(m_jac.find( make_pair(buckets[k][i],buckets[k][j]) ) == m_jac.end() ) {

                        string A, B;
                        set<string> s1 = obtenir_set(A,buckets[k][i]);
                        set<string> s2 = obtenir_set(B,buckets[k][j]);
                        jac_s1_s2 = obtain_Jsim_AB(s1,s2);


                        cout << "Jsim(" << A <<"," << B << ") = " << jac_s1_s2 << endl << endl;


                        m_jac[make_pair(buckets[k][i],buckets[k][j])] = jac_s1_s2;

                        if(jac_s1_s2 > max_jac) {
                            mes_similars.first = A;
                            mes_similars.second = B;
                            max_jac = jac_s1_s2;

                        }
                    }

                }
            }
            cout  << endl;
        }
    }

    cout << "The pair of documents " << mes_similars.first << " and " << mes_similars.second << ", are the most similar of all. Jsim = " << max_jac << endl;
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

    vector<vector<int> > buckets;
    for (int b = 0; b < num_b; b++){
		compute_band_lsh(b, tam, s, num_r, buckets, treshold);
    }
    imprimirBuckets(buckets);

    minim_jacard(buckets);


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
    m_jac.clear();
    cout << "Enter the desired value for k (the length of the shingles [k > 0])" << endl;
    cin >> k;
    string path_dir;
    vector<string> dir_entries;

    while ( not (get_dir(path_dir) and open_dir(path_dir, dir_entries) and fill_map_general(dir_entries))) {
        cout << "ERROR: cannot find/read " << path_dir << endl;
    }

    compare_n_docs();


}
