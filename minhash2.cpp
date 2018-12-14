#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    // std::find
using namespace std;


int main() 
{ 
/////////////////////////
/////INICIALITCAIO///////
////////////////////////
	int tam = 6;
	int num_hashs = 30;
	int num_docu = 2;
	
    vector<int> s1{0, 1, 2, 3 }; 
	vector<int> s2{2, 3, 4, 5 };  
	
//////////////////////////////////////////
/////CREAR FUNCIONS HASH ALEATORIES///////
//////////////////////////////////////////
	
	
	srand (time(NULL));
	vector<pair<int,int>> h;
	int m,n;
	for (int i = 0; i < num_hashs; i++){
		//eq de la forma m*x + n = y
		m = rand() % tam;
		n = rand() % tam;
		h.push_back(make_pair(m,n));
	}
	
	cout << "contingut d'H:" << endl << endl;
		
	for (int i = 0; i < h.size(); i++){
		cout << h[i].first << " " << h[i].second << endl;
	}
	cout << endl;
	
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
	
	cout << "contingut de T:" << endl << endl;
		
	for (int i = 0; i < num_hashs; i++){
		for (int j = 0; j < num_docu; j++){
			cout << t[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	
////////////////////////////////////////////////////
/////VEURE SIMILITUDS ENTRE ELS DOS MINHASHES///////
////////////////////////////////////////////////////

	int eq = 0;
	for (int i = 0; i < num_hashs; i++){
		if (t[i][0] == t[i][1]) eq++;
	}

	cout << "Jaccard similarity by minhashing is: " << (double) eq/ (double) num_hashs << endl;
 	
		
    return 0; 
} 
