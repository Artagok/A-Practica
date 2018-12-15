#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    // std::find
using namespace std;

double compute_min_hash(int tam, const vector<pair<int,int>>& h, vector<int> s1, vector<int> s2){
		
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

void compute_band_lsh(int ith_band, int tam, const vector<vector<int>>& S, int r, vector<vector<int>>& buckets, double treshold){
		

	vector<pair<int,int>> h;
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

void imprimirBuckets(vector<vector<int>>& buckets){
	cout << "Buckets content:" << endl;
	for (int i = 0; i < buckets.size(); i++){
		cout << "[ " << i << " ] :";
		for (int j = 0; j < buckets[i].size(); j++){
			cout << " " << buckets[i][j];
		} 
		cout << endl;
	}
}

int main(){
	
	srand (time(NULL)); //set seed
	
	int tam = 6;
	int num_b = 5; //vegades que s'executa compute_b_lsh
	int num_r = 50; //funcions de hash que farà servir
	int num_docu = 2;
	double treshold = 0.7;
	
    vector<int> s1{0, 1, 2, 3 }; 
	vector<int> s2{2, 3, 4, 5 };
	vector<int> s3{4, 5, 6, 7 };
	vector<int> s4{6, 7, 8, 9 };
	
	vector <vector<int> > s;
	s.push_back(s1);
	s.push_back(s2);
	s.push_back(s3);
	s.push_back(s4);
	
	vector<vector<int>> buckets (num_b);
	
	for (int b = 0; b < num_b; b++){
		compute_band_lsh(b, tam, s, num_r, buckets, treshold);
	}
	
	imprimirBuckets(buckets);
	
}


