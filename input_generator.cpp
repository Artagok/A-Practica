#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
/* Directories and files */
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
using namespace std;

void clean_input_folder() {

    vector<string> v_files;

    DIR* d = opendir("kshingles_input");
    if (d == NULL) {
        cout << "ERROR: cannot find/open kshingles_input directory" << endl;
        return;
    }
    dirent* d_entry;
    while ( (d_entry = readdir(d)) != NULL ) {
        v_files.push_back(d_entry->d_name);
    }
    closedir(d);

    // Uncomment to print all entries in kshingles_input directory before removal
    //for (int i = 0; i < v_files.size(); ++i)
        //cout << v_files[i] << endl;

    string dir = "kshingles_input/";
    for (int i = 0; i < v_files.size(); ++i) {
        if (v_files[i] != "1.in" and v_files[i] != "." and v_files[i] != "..")
            remove( (dir+v_files[i]).c_str() );
    }
    cout << "Deleted ALL files in kshingles_input/ EXCEPT 1.in" << endl;
}

void get_words_in_1(vector<string>& words) {

    ifstream ifs("kshingles_input/1.in");
    stringstream buffer;

    if (ifs.fail()) {
        cout << "ERROR: cannot find/read kshingles_input/1.in file" << endl;
        return;
    }

    buffer << ifs.rdbuf();
    string content = buffer.str();

    string delim = " ";
    size_t pos = 0;
    string token;
    while ( (pos = content.find(delim)) != string::npos ) {
        token = content.substr(0,pos);
        words.push_back(token);
        content.erase(0,pos+delim.length());
    }
    // Last token extracted out of the loop
    words.push_back(content);
}

string generate_permutation(const vector<string>& original) {
    string res = "";
    vector<string> copia = original;
    random_shuffle(copia.begin(),copia.end());
    for (int i = 0; i < copia.size(); ++i) {
        if (i == 0)
            res += copia[i];
        else
            res = res + " " + copia[i];
    }
    return res;
}

void generate_n_random_permutations(const vector<string>& original, const int& n) {

    std::ofstream ofs;
    for (int i = 2; i <= n; ++i) {
        string new_file = "kshingles_input/" + to_string(i) + ".in";
        ofs.open(new_file.c_str(), std::ofstream::out | std::ofstream::trunc);
        string new_permutation = generate_permutation(original);
        ofs.write(new_permutation.c_str(),new_permutation.length());
        ofs.close();
    }
}

int main() {

    clean_input_folder();
    vector<string> words;
    get_words_in_1(words);

    /* Uncomment to print all words from kshingles_input/1.in one by one
    for (int i = 0; i < words.size(); ++i)
        cout << words[i] << endl; */

    cout << "kshingles_input/1.in contains 50 words of Lorem Ipsum and from these words you can generate " <<
            "n files each containing a random permutation of the contents in 1.in" << endl;
    cout << "[1 <= n <= 100]" << endl;
    cout << "Meaning kshingles_input/ will contain n files afther this, so we generate (n-1) files since 1.in already exists. " <<
            "Files will be named as following: 2.in 3.in 4.in ..." << endl;

    cout << "Enter desired value for n:" << endl;
    int n;
    while (cin >> n and not(n >= 1 and n <= 100)) {
        cout << "[1 <= n <= 100]" << endl;
    }

    generate_n_random_permutations(words,n);
}
