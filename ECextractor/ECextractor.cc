#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
#include<sstream>
#include<cmath>
#include<numeric>
#include<algorithm>
#include<stack>
#include<queue>
#include<set>
#include<assert.h>

#include"output_cycles_general.h"
#include"output_adjacency_matrix_screenshot.h"
#include"output_stat.h"

using namespace std;

int strong_component;
int elementary_cycle;
int total_elementary_cycle;
int  number_time_points; 
int  EC_length_max ;

string INPUT_FILE_NAME = "input_data/";
string OUTPUT_FILE_NAME = "output_data/";
string TIME_SERIE_DATA = INPUT_FILE_NAME +"TIME_SERIES_DATA.txt";
string DIGRAPH_DATA = INPUT_FILE_NAME +"DIGRAPH_MATRIX.txt";

void tarjan_algorithm(int*,int**, bool**,bool*,int*,stack<int>&, int* ,int& ,int& ,int& );
void Tiernan_algorithm(int** , int*, int&, int*,int** , int** , int* , int& , int&, double* );
void Tiernan_algorithm_iterative(int** , int*, int&, int*,int** , int** , int* , int& , int&, double* );
void general_list_cycles(int**,vector<int>& );
void cycle_classification(vector<int>&, vector<int>&, vector<int>&,int&,int&,vector<int>&);



int main(){
  
clock_t start, end=0; 
assert( (start = clock()) != -1);
cout <<"                            ****************************************************                "<< endl;
cout <<"                            *                                                  *           "<< endl;
cout <<"                            *                  CODE ECextractor                *        "<< endl;
cout <<"                            *                                                  *     "<< endl;
cout <<"                            ****************************************************           "<< endl;
int EC_length_max =0;
int tp_start; 
int tp_end; 
int  number_vertices; 
int  nbre_colonne; 
int couverture_analyse;
bool analyse_cycle_SCC = true; 
bool recursive_function = false; 
                                 

vector<int> time_point_used;
vector<int>:: iterator time_point_used_it;
vector<int> liste_cycles;
vector<int>::iterator liste_cycles_it;
vector<int> liste_cycles_size;
vector<int>::iterator liste_cycles_size_it;
vector<int> nbre_elementary_cycle; 
vector<int>::iterator nbre_elementary_cycle_it;
vector<string>  ID_screenshot;
vector<string> ::iterator ID_screenshot_it;
vector<int>  ID_screenshot_exit;
vector<int> ::iterator ID_screenshot_exit_it;


double previous_time = 0.0;
int number_analyzed_tp = 0;
int etat_initial;
int etat_final;
int SCC_previous = 0;
int nbre_SCC_nul_previous = 0;
double duration_total = 0.0;

cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                               INPUT REQUIRED FROM THE USER                                "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;

cout << ""<< endl;
cout << "                                     1/3) PLEASE ENTER THE NUMBER OF NODES OF THE DIGRAPH:   "; 
cin >> number_vertices;
nbre_colonne =    number_vertices +1;

cout << ""<< endl;
cout << "                                     2/3) PLEASE ENTER THE NUMBER OF TIME POINTS IN THE TIME SERIES: "; 
cin >> number_time_points;
cout << ""<< endl;
cout << "                                     3/3) DO YOU WANT TO  PERFORM THE ANALYSIS FOR ALL THE TIME POINTS? (TYPE 1 IF YES, 0 OTHERWISE):  "; 
cin >> couverture_analyse;

if (couverture_analyse ==  1){
 tp_start = 1;
 tp_end = number_time_points;
}else{
 cout << ""<< endl;
cout << "                                    PLEASE ENTER THE STARTING POINT FOR THE SIMULATION. IT MUST BE AN INTEGER > 1 AND <  "<<number_time_points << endl; 
 cin >>  tp_start ;
cout << "                                    PLEASE ENTER THE ENDING POINT FOR THE SIMULATION. IT MUST BE AN INTEGER > "<< tp_start<<" AND <=  " <<number_time_points << endl; 
 cin >>  tp_end  ;
}

 
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "              ALL INFORMATION REQUIRED FROM THE USER ARE ENCODED - STARTING LOADING INPUT FILES"<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;

   string line, tempnum;
   int end_line = 0;
   vector<double>  input_vector;
   vector<double>::iterator  input_vector_it;
   double brol;
   ifstream name_file(  TIME_SERIE_DATA.c_str(), ios::in);
 if (name_file.is_open()){
    while(!name_file.eof()){
       getline(name_file, line);
     while((end_line=line.find('	',0))!=string::npos){
       tempnum = line.substr(0,end_line);
       stringstream(tempnum) >> brol;
       input_vector.push_back(brol);
       line.erase(0,end_line+1);
     }
     tempnum = line.substr(0,line.length());
     stringstream(tempnum) >>brol;
     input_vector.push_back(brol);
  }
 name_file.close();
 }else{cout << "file can not be opened"  << endl;}
 
    end_line = 0;
   vector<double>  input_matrix_H;
   vector<double>::iterator  input_matrix_H_it;
   ifstream name_file3(  DIGRAPH_DATA.c_str(), ios::in);
   if (name_file3.is_open()){
    while(!name_file3.eof()){
     getline(name_file3, line);
     while((end_line=line.find('	',0))!=string::npos){
       tempnum = line.substr(0,end_line);
       stringstream(tempnum) >> brol;
       input_matrix_H.push_back(brol);
       line.erase(0,end_line+1);
    }
    tempnum = line.substr(0,line.length());
    stringstream(tempnum) >>brol; 
    input_matrix_H.push_back(brol);
   }
   name_file3.close();
 }else{cout << "file can not be opened"  << endl;}
 
 
  
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                      INPUT FILES LOADED - VARIABLE  INITIALISATION               "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;

   double*  X;
   X= new double[nbre_colonne+1];
   for (int i=1; i<=nbre_colonne; ++i){X[i] = 0.0;}
   
  
   
   double** adjacency_matrix=NULL;
   adjacency_matrix = new double*[ number_vertices+1];
   for (int i=1;i<= number_vertices; ++i){adjacency_matrix[i] = new double[ number_vertices+1];}
  
   
   int** Summary=NULL;
   Summary = new int*[ number_time_points+1];
   for (int i=1;i<=  number_time_points; ++i){Summary[i] = new int[ 8+1];}
   for (int i=1; i<= number_time_points; ++i){ for (int j=1;j<= 6;++j){Summary[i][j] = 0;}}
	
   int** Auto_reg_summary=NULL;
   Auto_reg_summary = new int*[ number_time_points+1];
   for (int i=1;i<=  number_time_points; ++i){Auto_reg_summary[i] = new int[ number_vertices+1];}
    
   

  

cout << "   "<< endl; 
cout << "            ****                  INITIALISATION COMPLETED  - BEGINNING OF THE ANALYSIS                **** "<< endl; 
cout << "   "<< endl; 

if (tp_start > 1)
{
   for (input_vector_it = (input_vector.begin()+(tp_start-1 - 1)*nbre_colonne);input_vector_it!= (input_vector.begin()+(tp_start-1)*nbre_colonne);++input_vector_it)
  {
   previous_time =*input_vector_it;
  }
}

int tp_switch;
if (analyse_cycle_SCC){ tp_switch = 1;}else{ tp_switch = 1;} 


for ( int time_point=tp_start; time_point<= tp_end ;time_point +=tp_switch ){

 int nbre_disjoint_parts = 0;
 strong_component = 0;
 total_elementary_cycle=0;
 double delta_time = 0.0;
 int label_colonne=0;
 

 
 for (input_vector_it = (input_vector.begin()+(time_point - 1)*nbre_colonne);input_vector_it!= (input_vector.begin()+time_point*nbre_colonne);++input_vector_it)
 {
   label_colonne++;
   X[label_colonne] =*input_vector_it;
 }

  
  for (int i=1; i<=(nbre_colonne-1);++i){ if (X[i] <= 1.e-12){X[i] = 0.0;}}
  delta_time = X[ nbre_colonne] - previous_time;
//  if (delta_time != 0.0)
//  {
      //  time_point_used est aussi utilisé pour le classement des cycles élémentaires.  
     time_point_used.push_back(time_point);
     duration_total+=delta_time;
  cout << "      begining treatment of time point No. "<<  time_point << " Total number of timepoints to be treated is "<< tp_end-tp_start+1<< " Last timepoint is No. "<< tp_end<< endl;
  cout << ""<< endl;
  number_analyzed_tp =number_analyzed_tp+1;
  
   int label_ligne_H;
   int label_colonne_H;

   label_ligne_H=1;
   label_colonne_H=0;  

 for (input_matrix_H_it = input_matrix_H.begin();input_matrix_H_it!= input_matrix_H.end();++input_matrix_H_it)
 {
  label_colonne_H++;
  if (label_ligne_H<=number_vertices){ adjacency_matrix[label_ligne_H][label_colonne_H] =*input_matrix_H_it/*;cout <<  adjacency_matrix[label_ligne_H][label_colonne_H] <<" "*/;}
  if ( label_colonne_H == number_vertices){label_ligne_H++;label_colonne_H = 0/*;cout << ""<< endl*/;}
  }
 
 for (int i=1; i<=number_vertices;++i)
 {
	if(X[i] ==0)
    {
		for (int j=1; j<=number_vertices;++j){adjacency_matrix[i][j] == 0 ;adjacency_matrix[j][i] == 0;}	
	}		
 }
 
 int count = 0;
 stack<int> S;
 
 int* nbre_node_SCC;
 nbre_node_SCC = new int[number_vertices+1];
 int* Number;
 Number = new int[number_vertices+1];
 int* lower;
 lower = new int[number_vertices+1];
 bool* N_free;
 N_free = new bool[number_vertices+1];
 int* check_node_cp;
 check_node_cp = new int[number_vertices+1];
 
 int** Liste_SCC=NULL;
 Liste_SCC= new int*[ number_vertices+1];
 for (int i=1;i<= number_vertices; ++i){Liste_SCC[i] = new int[ number_vertices+1];}
 
 bool** adjacent_matrix=NULL;
 adjacent_matrix = new bool*[number_vertices+1];
 for (int i=1;i<=number_vertices; ++i){adjacent_matrix[i] = new bool[number_vertices+1];}
 
 for (int i=1; i<=number_vertices; ++i){
    N_free[i] = true;
    Number[i] = 0; 
    nbre_node_SCC[i]=0; 
    check_node_cp[i] = 0;
    for (int j=1; j<=number_vertices; ++j){
      Liste_SCC[i][j] = 0;
      if (adjacency_matrix[i][j] != 0.0 && i!=j && X[i] != 0.0 && X[j] !=0.0 ){ adjacent_matrix[j][i] = true; }else{adjacent_matrix[j][i] = false;}
    }
 }
 
  for(int djp=1;djp<=number_vertices; ++djp){ 
   if (check_node_cp[djp] != 1 ){
   nbre_disjoint_parts =  nbre_disjoint_parts+1;
   int x = djp;
   tarjan_algorithm(nbre_node_SCC, Liste_SCC,adjacent_matrix, N_free, lower, S, Number, number_vertices, count,  x);
    for (int i=1;i<=strong_component; ++i){
     for (int j=1;j<=nbre_node_SCC[i]; ++j){
       check_node_cp[Liste_SCC[i][j]] = 1;
     }
    }
   
    } // if sur check pc 
  } // fin du  for djp
  delete[] check_node_cp ;
  delete[] N_free;
  delete[] Number;
  delete[] lower;


int positive = 0;
int negative = 0;

 
 
for (int nbre_SCC = 1; nbre_SCC<=strong_component;++nbre_SCC){

if (nbre_node_SCC[nbre_SCC] > 1){
int number_vertices_SCC = nbre_node_SCC[nbre_SCC];

  bool** adjacent_matrix_SCC=NULL;
  adjacent_matrix_SCC = new bool*[number_vertices_SCC];
  for (int i=0;i<number_vertices_SCC; ++i){adjacent_matrix_SCC[i] = new bool[number_vertices_SCC];}

  int k= 1;
  int* label_convert;
  label_convert = new int[number_vertices_SCC+1];
  int* P;
  P = new int[number_vertices_SCC+1];
  
  int** outdegree_matrix=NULL;
  outdegree_matrix = new int*[number_vertices_SCC+1];
  for (int i=1;i<=number_vertices_SCC; ++i){outdegree_matrix[i] = new int[number_vertices_SCC+1];}
 
  int** H=NULL;
  H = new int*[number_vertices_SCC+1];
  for (int i=1;i<=number_vertices_SCC; ++i){H[i] = new int[number_vertices_SCC+1];}
  elementary_cycle = 0;

// EC1 initialization 
for (int i=1; i<=number_vertices_SCC; ++i){
 P[i] = 0;
 label_convert[i] = Liste_SCC[nbre_SCC][i];
}
P[1] = 1;
   
 
  for (int i=1;  i<=number_vertices_SCC; ++i){
      for (int j=1; j<=number_vertices_SCC; ++j){
	   adjacent_matrix_SCC[i-1][j-1]=  false;
	   outdegree_matrix[i][j] = 0;
	   H[i][j] = 0;
       if (adjacent_matrix[ label_convert[i]][label_convert[j]]  ){
         if(X[ label_convert[i]] != 0.0 && X[ label_convert[j]] != 0.0){outdegree_matrix[i][j] = j;}
		 adjacent_matrix_SCC[i-1][j-1] =  true;
       }
      } 
  }
int nbre_EC_expected = 2000*number_vertices_SCC;

 // call  subroutine for computing the elementary cycles
int** list_elementary_cycle=NULL;
list_elementary_cycle = new int*[nbre_EC_expected+1];
for (int i=1;i<=nbre_EC_expected; ++i){list_elementary_cycle[i] = new int[number_vertices_SCC+1];}
int* length_elementary_cycle;
length_elementary_cycle = new int[nbre_EC_expected+1];
double* norme_elementary_cycle;
norme_elementary_cycle = new double[nbre_EC_expected+1];
for (int i=1;i<=nbre_EC_expected;++i){
   for (int j=1;j<=number_vertices_SCC;++j){
    list_elementary_cycle[i][j] = 0;
   }
   length_elementary_cycle[i] = 0;
   norme_elementary_cycle[i] = 1.0;
}
if (analyse_cycle_SCC){

 if (recursive_function){
 Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert, outdegree_matrix, H,  P,  k,  number_vertices_SCC,X);
 }else{Tiernan_algorithm_iterative(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert, outdegree_matrix, H,  P,  k,  number_vertices_SCC,X);
 }
} // fin du  if si analyse_cycle_SCC = true

total_elementary_cycle +=elementary_cycle;
for (int i=1;i<=elementary_cycle; ++i){
  //----------------------------------------------------------------------------------
  int**  matrice_cycle_temp=NULL;
  matrice_cycle_temp =  new int*[4];
  for (int k=0; k< 4;++k){matrice_cycle_temp[k]  = new int[length_elementary_cycle[i]+1];}
  //----------------------------------------------------------------------------------

  for (int j=1; j<=length_elementary_cycle[i]; ++j){
     if (j != length_elementary_cycle[i]){
      if (adjacency_matrix[list_elementary_cycle[i][j+1]][list_elementary_cycle[i][j]] > 0 ){
       matrice_cycle_temp[0][j] = 1;}else{matrice_cycle_temp[0][j] = -1;}
       matrice_cycle_temp[1][j] = list_elementary_cycle[i][j];
       matrice_cycle_temp[2][j] = list_elementary_cycle[i][j+1];
       matrice_cycle_temp[3][j] = 0;
      norme_elementary_cycle[i] =norme_elementary_cycle[i]*adjacency_matrix[list_elementary_cycle[i][j+1]][list_elementary_cycle[i][j]]; 
     }else{ 
      if (adjacency_matrix[list_elementary_cycle[i][1]][list_elementary_cycle[i][j]]> 0 ){
       matrice_cycle_temp[0][j] = 1;}else{matrice_cycle_temp[0][j] = -1;}
       matrice_cycle_temp[1][j] = list_elementary_cycle[i][j];
       matrice_cycle_temp[2][j] = list_elementary_cycle[i][1];
       matrice_cycle_temp[3][j] = 0;
      norme_elementary_cycle[i] =norme_elementary_cycle[i]*adjacency_matrix[list_elementary_cycle[i][1]][list_elementary_cycle[i][j]]; 
     }
  }
  if (norme_elementary_cycle[i] >0){ matrice_cycle_temp[2][0] =1;positive++ ;}else{ matrice_cycle_temp[2][0] = -1;negative++;}
  //----------------------------------------------------------------------------------
  matrice_cycle_temp[0][0] = 0;
  matrice_cycle_temp[1][0] = length_elementary_cycle[i];
  matrice_cycle_temp[3][0] = time_point;
 
  general_list_cycles(matrice_cycle_temp, liste_cycles );
  for (int k=0; k< 4;++k){delete[] matrice_cycle_temp[k];}
  delete [] matrice_cycle_temp;
 //----------------------------------------------------------------------------------
} // fin for sur les elementary cycle

for (int i=1;i<=nbre_EC_expected;++i){delete[] list_elementary_cycle[i];}
delete [] list_elementary_cycle;
delete [] length_elementary_cycle;
delete [] norme_elementary_cycle ;
for (int i=1;i<=number_vertices_SCC;++i){delete[] outdegree_matrix[i];}
delete []  outdegree_matrix;
for (int i=0;i< number_vertices_SCC;++i){delete[] adjacent_matrix_SCC[i];}
delete []  adjacent_matrix_SCC;
for (int i=1;i<=number_vertices_SCC;++i){delete[] H[i];}
delete [] H;
delete [] P;
delete [] label_convert;
}else{} 
} // end of the loop over SCCs

////////////////////////////////////////////
liste_cycles_size.push_back(liste_cycles.size());
nbre_elementary_cycle.push_back(total_elementary_cycle);


int auto_reg_negative = 0;
int auto_reg_positive = 0;
Auto_reg_summary[time_point][0] =  time_point;
for (int i=1; i<=  number_vertices;++i){
  Auto_reg_summary[time_point][i] = 0;
  if (adjacency_matrix[i][i] < 0.0 && X[i] > 0.0){auto_reg_negative++; Auto_reg_summary[time_point][i] = -1;}
  if (adjacency_matrix[i][i] > 0.0 && X[i] > 0.0){auto_reg_positive++;Auto_reg_summary[time_point][i] = 1;}
}
/////////////////////////////////////////////////////
bool SCC_nul = false;
int  nbre_SCC_nul = 0;
for(int i=1; i<= strong_component; ++i)
   { int count1 =0;
     while( SCC_nul ==false && count1<nbre_node_SCC[i])
     {  
        count1++;
        if(X[Liste_SCC[i][count1]] == 0.0){SCC_nul = true;}
     }
     if (SCC_nul){nbre_SCC_nul++;}
}  // for sur strong_component
int SCC_active =  strong_component - nbre_SCC_nul;
Summary[time_point][3] =total_elementary_cycle;
Summary[time_point][2] =SCC_active;
Summary[time_point][8] = auto_reg_negative;
Summary[time_point][7] = auto_reg_positive;
Summary[time_point][6] = time_point;
Summary[time_point][4] = positive;
Summary[time_point][5] = negative;
Summary[time_point][1] = nbre_disjoint_parts;

if (SCC_previous != strong_component || nbre_SCC_nul_previous !=nbre_SCC_nul)
{ 
 
 if (analyse_cycle_SCC){ output_adjacency_matrix_screenshot(adjacency_matrix,strong_component,time_point, number_vertices,SCC_active, X);}
 

std::ostringstream tp_shot;
tp_shot << time_point;
std::string label_name1 = tp_shot.str();
std::ostringstream part_nbre;
part_nbre << strong_component;
std::string label_name2 = part_nbre.str();
std::ostringstream SCC_zero ;
SCC_zero << SCC_active;
std::string label_name3 = SCC_zero.str();

  int  max_length = 0; 
   for(int i=1; i<= strong_component; ++i)
   {
     if (nbre_node_SCC[i] >=max_length){ max_length = nbre_node_SCC[i];}
   }  
 max_length = max(max_length, strong_component);
 
if (analyse_cycle_SCC){
string  filename =OUTPUT_FILE_NAME+ "screenshot/"+label_name1 +"-"+label_name2+"-"+label_name3+"-DIGRAPH-SCC";
ofstream output_file(filename.c_str());
if (output_file.is_open()){
 
  for(int i=1; i<= strong_component; ++i)
   {
     for (int count1 = 1; count1<=max_length ;++count1)
     {
       if(count1<=nbre_node_SCC[i]){ output_file <<  Liste_SCC[i][count1]<< "  ";}else{output_file << 0 << "  ";}
     }
     output_file << " "<<endl;
   } 
 
     for(int i=1; i<= max_length; ++i)
     {
       if(i<= strong_component){ output_file <<  nbre_node_SCC[i]<< "  "; }else{output_file <<  0 << "  ";} 
     }
     output_file << " "<<endl;
      for(int i=1; i<= max_length; ++i)
     {
       if(i<= 1){ output_file <<  strong_component<< "  "; }else{output_file <<  0 << "  ";} 
     }
     output_file <<  " "<<endl;
output_file.close();
}else{cout << "file can not be opened"  << endl;}
} // end if (analyse_cycle_SCC)
ID_screenshot.push_back(label_name1 +"-"+label_name2+"-"+label_name3);
ID_screenshot_exit.push_back(time_point);
ID_screenshot_exit.push_back(strong_component);
ID_screenshot_exit.push_back(SCC_active);
} // end if (SCC_previous != strong_component || nbre_SCC_nul_previous !=nbre_SCC_nul)
SCC_previous = strong_component;
nbre_SCC_nul_previous = nbre_SCC_nul;
for (int i=1;i<= number_vertices;++i){delete[] Liste_SCC[i];}
delete []  Liste_SCC;
delete []  nbre_node_SCC;
for (int i=1;i<=number_vertices;++i){delete[] adjacent_matrix[i];}
delete []  adjacent_matrix;
//}  // FIN BOUCLE SUR CONDITION DE DELTA TIME DIFFERENT DE ZERO
previous_time = X[ nbre_colonne];

} // FIN  DE LA BOUCLE GENERALE SUR LES TIME POINTS
 if (analyse_cycle_SCC){
  string filename =OUTPUT_FILE_NAME+ "screenshot/LIST-SCREENSHOT-ID";
  ofstream output_file2(filename.c_str());
  if (output_file2.is_open()){
   int tpID1 =0;
   for (ID_screenshot_exit_it = ID_screenshot_exit.begin(); ID_screenshot_exit_it !=ID_screenshot_exit.end();++ID_screenshot_exit_it)
    {
      output_file2 <<  *ID_screenshot_exit_it << " "; tpID1++;
	if ( tpID1 == 3){ tpID1 =0;  output_file2 <<""<< endl;}
    } 
    output_file2.close();
  }else{cout << "file can not be opened"  << endl;}
} // end if (analyse_cycle_SCC)


if (analyse_cycle_SCC){ output_cycles_general(number_analyzed_tp, tp_start,  tp_end, number_vertices , Summary , Auto_reg_summary);}
if (analyse_cycle_SCC){cycle_classification(liste_cycles, liste_cycles_size,  nbre_elementary_cycle,tp_start, tp_end, time_point_used);}


 
cout <<"=============================================================================================="<< endl;
cout << "          PROCEDURE COMPLETED - ALL RESULTS SAVED - END OF THE PROGRAM "<< endl;
cout <<"=============================================================================================="<< endl;
end = clock(); 
double duration =  (end-start)/CLOCKS_PER_SEC;
output_stat(tp_start,tp_end ,duration );

delete []  X;
input_vector.clear();
input_matrix_H.clear();
time_point_used.clear();
liste_cycles.clear();
liste_cycles_size.clear();
nbre_elementary_cycle.clear();
ID_screenshot.clear();
ID_screenshot_exit.clear();
for (int i=1;i<= number_vertices;++i){delete[] adjacency_matrix[i];}
delete []  adjacency_matrix;
for (int i=1;i<=  number_time_points;++i){delete[] Summary[i];}
delete []  Summary ;
for (int i=1;i<=  number_time_points;++i){delete[] Auto_reg_summary[i];}
delete []  Auto_reg_summary ;
return 0;
} // end main function
//**************************************************************************************************

void tarjan_algorithm(int* nbre_node_SCC,int**  Liste_SCC, bool** adjacent_matrix,bool* N_free,int* lower,stack<int>& S, int* Number,int& number_vertices,int& count,int&  x){
 S.push(x);
 Number[x] = count;
 count++;
 lower[x] =  Number[x];
 for (int i=1; i<=number_vertices; ++i){
   if (  N_free[i] && adjacent_matrix[x][i]){
     if (Number[i] == 0 ){
      tarjan_algorithm(nbre_node_SCC, Liste_SCC,adjacent_matrix, N_free, lower, S, Number, number_vertices, count,  i );
      lower[x] =  min(lower[x],lower[i]);
     }else{ lower[x] = min(lower[x],  Number[i]);}
   }
 }
 if (Number[x]==lower[x]){
 int iss ;
 strong_component++;
 int node= 0;
// cout << " Composant fortement connecté No.  "<<  strong_component << ":  "; 

 do{
   node++;
   iss = S.top();
   S.pop();
   Liste_SCC[strong_component][node] = iss;
   N_free[iss] = false;
 }while(x !=iss);
 nbre_node_SCC[strong_component] = node; 
  for (int i=1;i<=nbre_node_SCC[strong_component];++i){
      //  cout << Liste_SCC[strong_component][i] << "  "; 
  }
//  cout <<""<<endl;
 }
 return; 
 }
//**************************************************************************************************


void Tiernan_algorithm(int** list_elementary_cycle,  int* length_elementary_cycle, int& nbre_SCC,int* label_convert, int** outdegree_matrix, int** H, int* P, int& k, int& number_vertices_SCC, double* X){

for (int j=1;j<=number_vertices_SCC; ++j){
 if (outdegree_matrix[P[k]][j] > P[1]){
   bool step1 = true;
   for(int l=1;l<=number_vertices_SCC; ++l){ if (outdegree_matrix[P[k]][j] == P[l]){ step1 = false;}} 
   if (step1){
    bool  step2 =  true;
    for (int m=1; m<=number_vertices_SCC; ++m){if (outdegree_matrix[P[k]][j] == H[P[k]][m]){step2 = false;}}
    if (step2){
     k++;
     P[k] = outdegree_matrix[P[k-1]][j]; 
     Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC, X);
    }  // fin if sur step2  
   } // if sur step  1
 } // fin du  if sur P[1]
} // boucle for sur les j

// step EC3;
bool  step3 = false;
for(int j=1;  j<= number_vertices_SCC; ++j){if (outdegree_matrix[P[k]][j] == P[1]){step3 = true;}}
if (step3){
 elementary_cycle++;
 int longueur = 0;
 bool Mol_non_nul = true;
// cout << " Cycle élémentaire  "<< elementary_cycle  <<" trouvé dans le composant fortement connecté No. "<< nbre_SCC << ", Les vertex sont : " << endl;
 for(int j=1;  j<= number_vertices_SCC; ++j){
   if (P[j] != 0 && Mol_non_nul){
     longueur++;
     list_elementary_cycle[elementary_cycle][longueur] = label_convert[P[j]];
     if (X[label_convert[P[j]]] == 0.0 ){Mol_non_nul = false;}
//   cout << label_convert[P[j]] << "  ";
   }
  }
 // cout << ""<< endl;
if (Mol_non_nul){length_elementary_cycle[elementary_cycle] =   longueur; if(longueur >= EC_length_max){EC_length_max = longueur;}}else{elementary_cycle--;}
} // end of step3 if
// EC4
if (k  != 1){
  // step 8, upgrade;
  for (int m=1; m<=number_vertices_SCC; ++m){H[P[k]][m] = 0;}
  int m =1;
  for (int j=1; j<=number_vertices_SCC; ++j){if(H[P[k-1]][j] != 0){m++;}}
  H[P[k-1]][m+1] = P[k];
  P[k] = 0;
  k= k-1;
  Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC,X);
}else{
// EC5
if (P[1] != number_vertices_SCC){
P[1] = P[1] +1;
k = 1;
for (int i=1; i<=number_vertices_SCC; ++i){
 for (int j=1; j<=number_vertices_SCC; ++j){
   H[i][j] = 0;
 }
}
Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC,X);
}else{
// EC6;
}
} // fin if k!= 1 loop
return;
} // end of the tarnian  algorithm
//**************************************************************************************************
//**************************************************************************************************

void Tiernan_algorithm_iterative(int** list_elementary_cycle,  int* length_elementary_cycle, int& nbre_SCC,int* label_convert, int** outdegree_matrix, int** H, int* P, int& k, int& number_vertices_SCC, double* X){

bool not_finished = true;
 
while(not_finished){

int jec2 = 0;
bool find_it;
do{
 find_it = false;
bool next_k =false;
while(jec2< number_vertices_SCC && next_k == false){
 jec2++;
// cout << jec2<<"  "<< number_vertices_SCC << endl;
 if (outdegree_matrix[P[k]][jec2] > P[1]){
   bool step1 = true;
   int l=0;
   while(l<number_vertices_SCC && step1 == true){l++;if (outdegree_matrix[P[k]][jec2] == P[l]){ step1 = false;}}
 //  for(int l=1;l<=number_vertices_SCC; ++l){ if (outdegree_matrix[P[k]][jec2] == P[l]){ step1 = false;}} 
   if (step1){
    bool  step2 =  true;
	int m=0;
	while(m<number_vertices_SCC && step2 == true){m++;if (outdegree_matrix[P[k]][jec2] == H[P[k]][m]){step2 = false;}}
 //   for (int m=1; m<=number_vertices_SCC; ++m){if (outdegree_matrix[P[k]][jec2] == H[P[k]][m]){step2 = false;}}
    if (step2){
     k++;
     P[k] = outdegree_matrix[P[k-1]][jec2];// cout <<  P[k] <<"  ";
    // Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC, X);
	 find_it = true;
     jec2=0;
     next_k = true;
	 }  // fin if sur step2  
   } // if sur step  1
 } // fin du  if sur P[1]
} // while(jec2< number_vertices_SCC && next_k == false)
}while(find_it == true);
//cout << ""<< endl;
// step EC3;
bool  step3 = false;
for(int j=1;  j<= number_vertices_SCC; ++j){if (outdegree_matrix[P[k]][j] == P[1]){step3 = true;}}
if (step3){
 elementary_cycle++;
 int longueur = 0;
 bool Mol_non_nul = true;
// cout << " Cycle élémentaire  "<< elementary_cycle  <<" trouvé dans le composant fortement connecté No. "<< nbre_SCC <<  endl;
 for(int j=1;  j<= number_vertices_SCC; ++j){
   // symbol || shit+alt +l
   if (P[j] != 0 && Mol_non_nul){
	 longueur++;
     list_elementary_cycle[elementary_cycle][longueur] = label_convert[P[j]];
     if (X[label_convert[P[j]]] == 0.0 ){Mol_non_nul = false; }
  //cout << label_convert[P[j]] << "  ";
   }
  }
// cout <<""<< endl;
if (Mol_non_nul){length_elementary_cycle[elementary_cycle] =   longueur; if(longueur >= EC_length_max){EC_length_max = longueur;}}else{elementary_cycle--;}
} // end of step3 if

// EC4

if (k  != 1){
  // step 8, upgrade;
  for (int m=1; m<=number_vertices_SCC; ++m){H[P[k]][m] = 0;}
  int m =1;
  for (int j=1; j<=number_vertices_SCC; ++j){if(H[P[k-1]][j] != 0){m++;}}
  H[P[k-1]][m+1] = P[k];
  P[k] = 0;
  k= k-1;
//  Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC,X);
}else{
// EC5
if (P[1] != number_vertices_SCC){
P[1] = P[1] +1;
k = 1;
for (int i=1; i<=number_vertices_SCC; ++i){
 for (int j=1; j<=number_vertices_SCC; ++j){
   H[i][j] = 0;
 }
}
//Tiernan_algorithm(list_elementary_cycle,length_elementary_cycle,nbre_SCC,label_convert,outdegree_matrix,  H,  P,  k, number_vertices_SCC,X);
}else{
// EC6;
not_finished = false;
}
} // fin if k!= 1 loop
} // end // end while(not finished)
return;
} // end of the tarnian  algorithm_iterative form
 //**************************************************************************************************
 //**************************************************************************************************
void general_list_cycles(int** matrice_cycle_temp,vector<int>& liste_cycles ){


//  ETAPE I: LA MATRICE CONTENANT LE CYCLE ELEMENTAIRE EST REORDONNEE (ORDRE CROISSANT DES SOMMETS)
int i = 1;
int* intermediaire = new int[4]; 
while (i<  matrice_cycle_temp[1][0]){
  int j = i+1;
  while (j <=  matrice_cycle_temp[1][0]){
    if (matrice_cycle_temp[1][j] <  matrice_cycle_temp[1][i]){
       for (int k=0;  k<4;++k){
          intermediaire[k] =  matrice_cycle_temp[k][j] ;
          matrice_cycle_temp[k][j] =  matrice_cycle_temp[k][i];
          matrice_cycle_temp[k][i] = intermediaire[k];
        }
        j = i+1;
    }else{j++;}
  } // boucle while sur les j
  i++;
} // boucle while sur les  i

// ETAPE II: LE CYCLE REORDONNE QUI  SE TROUVE DANS LA MATRICE TEMPORAIRE EST RECOPIER DANS LA 
// LISTE_CYCLES COMPLETE: LES 4 PREMIERS NOMBRES SONT LES CARACTERISTIQUES, ENSUITE LE CYCLE 
// ET LES PARAMETRES SUR LA STABILITES DES ARRETES ET DES SOMMETS VIENNENT  A LA FIN.

// parametre de la premiere colonne
for (int p1 = 0; p1< 4;++p1){liste_cycles.push_back(matrice_cycle_temp[p1][0]);}
// liste sommets superieurs
for (int p1=0; p1<matrice_cycle_temp[1][0];++p1){liste_cycles.push_back(matrice_cycle_temp[1][p1+1]);}
// liste sommets inferieurs
for (int p1=0; p1<matrice_cycle_temp[1][0];++p1){liste_cycles.push_back(matrice_cycle_temp[2][p1+1]);}
// parametres
for (int p1=0; p1<matrice_cycle_temp[1][0];++p1){liste_cycles.push_back(matrice_cycle_temp[0][p1+1]);}
// parametres
for (int p1=0; p1<matrice_cycle_temp[1][0];++p1){liste_cycles.push_back(matrice_cycle_temp[3][p1+1]);}

delete [] intermediaire;
return;
}
//**************************************************************************************************
//**************************************************************************************************
void cycle_classification(vector<int>& liste_cycles, vector<int>& liste_cycles_size, vector<int>& nbre_elementary_cycle,int& tp_start,int& tp_end,vector<int>&  time_point_used){
int selected_more_once = 0;
int selected_only_once = 0;
vector<int>* map_cycle_elementaire_critique =new vector<int>[number_time_points+1];

for (int time_point=1;time_point<=number_time_points;++time_point){
  map_cycle_elementaire_critique[ time_point].push_back(time_point);
}


vector<int>::iterator liste_cycles_size_it;
vector<int>::iterator liste_cycles_it;
vector<int> cycle_en_test;
vector<int>::iterator cycle_en_test_it;
vector<int>::iterator nbre_elementary_cycle_it;// nombre de cycle dans un pas de temps
vector<int>::iterator voyageur1_it;
vector<int>::iterator voyageur2_it;
vector<int>::iterator voyageur3_it;

vector<int> time_distribution_cycle_selected;
vector<int>::iterator time_distribution_cycle_selected_it;
vector<int>::iterator nbre_elementary_cycle_it_bis;
int nouveau_cycle_critique = 0;
int number_time_selected = 0;
int longueur_cycle;
int longueur_cycle2;
int nbre_tp_parcourus = 0;
bool not_found;


// initialisation des pointeurs
 nbre_elementary_cycle_it = nbre_elementary_cycle.begin();
 nbre_elementary_cycle_it_bis = nbre_elementary_cycle.begin();
 liste_cycles_it = liste_cycles.begin();
 voyageur1_it = liste_cycles.begin();
 voyageur2_it = liste_cycles.begin();
 voyageur3_it = liste_cycles.begin();
 
string  filename =OUTPUT_FILE_NAME+ "RESULTS-LIST-ECS.csv";
 ofstream output_file(filename.c_str());
if (output_file.is_open()){
// for (int time_point = tp_start; time_point<= tp_end;++time_point){
  for (vector<int>::iterator it32 =time_point_used.begin();it32 !=time_point_used.end();++it32 ){
 /* cout << "******************************" << endl;
  cout << "time point nbre:  " <<*it32<< endl;
  cout << "******************************" << endl;
  */
  for (int nbre_cycle_tp = 1;nbre_cycle_tp<=*nbre_elementary_cycle_it;++nbre_cycle_tp){
     nbre_elementary_cycle_it_bis = nbre_elementary_cycle_it;
   //  cout << "----------------------------------------------------------------------------"<<endl;
   //  cout << "cycle no. "<<nbre_cycle_tp <<"  sur un total de  "<< *nbre_elementary_cycle_it  << endl;
   //  cout << "----------------------------------------------------------------------------"<<endl;
       voyageur1_it = liste_cycles_it;
       voyageur1_it++;
       longueur_cycle = *voyageur1_it;
       voyageur1_it--;
    if (*liste_cycles_it ==  0){
        time_distribution_cycle_selected.push_back(*(voyageur1_it+3));
        number_time_selected++;
       *liste_cycles_it = 1;
       cycle_en_test.clear();
        
       for (int  k1 = 1; k1<= 4*(longueur_cycle+1);++k1)
       {
          cycle_en_test.push_back(*voyageur1_it);
          voyageur1_it++;
       }
     
      for (liste_cycles_size_it = liste_cycles_size.begin()+nbre_tp_parcourus; liste_cycles_size_it!=liste_cycles_size.end()-1;++liste_cycles_size_it){
        voyageur2_it= liste_cycles.begin()+ *liste_cycles_size_it;
        nbre_elementary_cycle_it_bis++;
      //  cout << *voyageur2_it << "   "<< *(voyageur2_it+1) <<"   "<<  *(voyageur2_it+2)  << "  "<< *(voyageur2_it+3) << endl;
        not_found =  true;
        int nbre_max_elementary_cycle = 0;
        int limit = *(nbre_elementary_cycle_it_bis);
        
       // cout << " nbre de cycle max à parcourir  =  " << limit << endl;
        while (not_found && (nbre_max_elementary_cycle < limit))
         {nbre_max_elementary_cycle++;
     //    cout << "---------cycle numéro  "<< nbre_max_elementary_cycle <<endl;
          voyageur3_it = voyageur2_it;
          voyageur3_it++;
          longueur_cycle2 = *voyageur3_it;
         // cout << "on vérifie si  on est déjà passé " << *voyageur2_it << " la longueur du cycle est =  " <<  longueur_cycle2<< endl;
          if (*voyageur2_it == 0)
          {
          // check  la longueur des deux cycles
           cycle_en_test_it = cycle_en_test.begin()+1;
          if (*cycle_en_test_it == *voyageur3_it){
          // et la norme des deux cycles
             cycle_en_test_it++; voyageur3_it++;
            if (*cycle_en_test_it == *voyageur3_it){
            // on compare les deux cycles pour voir si  c'est isomorphe
              if (equal(voyageur3_it+2,voyageur3_it+2+longueur_cycle2,cycle_en_test_it+2)){
                 not_found = false;
                 time_distribution_cycle_selected.push_back(*(voyageur3_it+1));
                 number_time_selected++;
                 *voyageur2_it = 1;
                // cout << number_time_selected << " times isomorphic cycle found " << endl;
              }
            }//  fin if sur la norme
          } // fin  if sur la longueur
         }else{ voyageur2_it =  voyageur2_it + 4*(longueur_cycle2+1);} //  if  (*voyageur2_it == 0)
         
         } // fin du  while not found
      } // end for liste_cycles_size
    // number_time_selected < floor(0.7*( time_point_used.size())) &&
     if (number_time_selected > 0 ){
     //  cout << "sauvegarde d'un cycle!!!! de longueur ------------  "<< longueur_cycle << endl;
       //  sauvegarde du  cycle comme cycle critique.........
       nouveau_cycle_critique++;
       for (vector<int>::iterator it2 = time_distribution_cycle_selected.begin();it2!=time_distribution_cycle_selected.end();++it2){
         map_cycle_elementaire_critique[*it2].push_back(nouveau_cycle_critique);
       }
       int** triage_tableau = NULL;
       triage_tableau = new int*[4];
       for (int i=0; i< 4;++i){triage_tableau[i]  = new int[longueur_cycle+1];}
       cycle_en_test_it=cycle_en_test.begin();
       triage_tableau[0][0] =  nouveau_cycle_critique;
       triage_tableau[1][0] =  *(cycle_en_test_it+1)  ;
       triage_tableau[2][0] = *(cycle_en_test_it+2) ;
       triage_tableau[3][0] = number_time_selected; 
       if (number_time_selected > 1){selected_more_once++;}else{selected_only_once++;}
       for (int label_ligne = 1;label_ligne<=4;++label_ligne){
         int b = 0;
         for (cycle_en_test_it=cycle_en_test.begin()+4+(label_ligne-1)*longueur_cycle;cycle_en_test_it!=cycle_en_test.begin()+4+(label_ligne)*longueur_cycle;++cycle_en_test_it)
         { b++;
           if (label_ligne == 1){triage_tableau[1][b] = *cycle_en_test_it;}
           if (label_ligne == 2){triage_tableau[2][b] = *cycle_en_test_it;}
           if (label_ligne == 3){triage_tableau[0][b] = *cycle_en_test_it;}
           if (label_ligne == 4){triage_tableau[3][b] = *cycle_en_test_it;}
         }
       } //  fin for label_ligne
       // ecriture dans le fichier exterieur des cycles élémentaires critiques
       for (int label = 0;label<4;++label){
         for (int col=0;col<= longueur_cycle;++col){
          if (col !=  longueur_cycle)
		  {  output_file<< triage_tableau[label][col]<<",";}else{
		   if (longueur_cycle == EC_length_max){output_file<< triage_tableau[label][col]<< endl;}else{
		    output_file<< triage_tableau[label][col]<<",";
			for (int kk = col;kk<= EC_length_max;++kk){if (kk != EC_length_max){output_file<<",";}else{output_file<<""<<endl;}}
			} // fin if sur  EC_max
		  } // fin if sur col!= longeuer_cycle
         } // for int col
       //  output_file<< ""<< endl;
       }
       for (int i=0; i< 4;++i){delete[] triage_tableau[i];}
       delete [] triage_tableau;
     } // fin if (number_time_selected < 0.5*number_time_point )
     liste_cycles_it =  liste_cycles_it + ( longueur_cycle+1)*4;
     time_distribution_cycle_selected.clear();
     number_time_selected = 0;
    }else{
     // le cycle à déjà été sélectionné, on passe au suivant dans le même tp
     liste_cycles_it =  liste_cycles_it + ( longueur_cycle+1)*4;
    
    }// fin du if sur selectionnné ou pas
  } // fin for sur le nbre_cycle_tp
  nbre_elementary_cycle_it++;
  nbre_tp_parcourus++;
 } // fin du for sur les time points
  output_file.close();
}else{cout << "file can not be opened"  << endl;}


// extraction de la liste des cycles élémentaires dynamique en fonction du  temps
 int** cycles_dynamiques_time = NULL;
  cycles_dynamiques_time = new int*[time_point_used.size()+1];
 for (int i=0; i< (time_point_used.size()+1);++i){cycles_dynamiques_time[i]  = new int[nouveau_cycle_critique+1];}
 int label_inter = 0;
 for (int i = 0;i < (time_point_used.size()+1);++i)
 {
   for (int j = 0;j < (nouveau_cycle_critique+1);++j)
   {
      cycles_dynamiques_time[i][j] = 0;
   }
 }
 //for (int i=tp_start;i<=tp_end;++i)
 for (vector<int>::iterator i = time_point_used.begin();i !=time_point_used.end();++i)
 { vector<int>::iterator itin =  map_cycle_elementaire_critique[*i].begin();
   label_inter++; cycles_dynamiques_time[label_inter][0] =*itin;
   for (vector<int>::iterator it =  map_cycle_elementaire_critique[*i].begin()+1;it!=map_cycle_elementaire_critique[*i].end();++it){
         cycles_dynamiques_time[label_inter][*it] =1;
   }  
 }


 filename =OUTPUT_FILE_NAME+ "ECS-TEMPORAL-EVOLUTION.csv";
 ofstream output_file2(filename.c_str());
 if (output_file2.is_open()){
   label_inter = 0;
  // for (int i=tp_start;i<=tp_end;++i){
  for (vector<int>::iterator i = time_point_used.begin();i !=time_point_used.end();++i){
      label_inter++;
      for (int j=0;j<=nouveau_cycle_critique;++j){
        if(j != nouveau_cycle_critique){ output_file2 << cycles_dynamiques_time[label_inter][j] <<",";}else{output_file2 << cycles_dynamiques_time[label_inter][j] << endl;}
      }
    //  output_file2 << ""<< endl;
   }
  output_file2.close();
 }else{cout << "file can not be opened"  << endl;}

 for (int i=0; i< (time_point_used.size()+1);++i){delete[]  cycles_dynamiques_time[i];}
 delete []  cycles_dynamiques_time;

time_distribution_cycle_selected.clear();
cycle_en_test.clear();
for (int i=1;i<=number_time_points;++i){map_cycle_elementaire_critique[i].clear();}
delete [] map_cycle_elementaire_critique;
return;}