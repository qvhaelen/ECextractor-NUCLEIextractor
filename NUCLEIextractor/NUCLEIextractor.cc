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

#include "Initial_set_up_1.h"
#include "Initial_set_up_2.h"

using namespace std;

string INPUT_FILE_NAME = "input_data/";
string OUTPUT_FILE_NAME = "output_data/";
string MATRIX_B = INPUT_FILE_NAME + "Matrix_B.txt";
string TABLEAU_C = INPUT_FILE_NAME +"tableau_C.txt";
string  filename = OUTPUT_FILE_NAME+"DETAILED_LIST_OF_NUCLEI.csv";
ofstream output_file(filename.c_str());
string filename2 = OUTPUT_FILE_NAME+ "TOTAL_NUMBER_OF_NUCLEI_PER_TIMEPOINTS.csv";
ofstream output_file2(filename2.c_str());

void noyau_finder_algorithm(int&, int& , int**,int&, int&, int&, int&, int&,int&,int*, int*, int**, int&);

int main(){
 
cout <<"                            ****************************************************                "<< endl;
cout <<"                            *                                                  *           "<< endl;
cout <<"                            *        NUCLEIextractor: NUCLEI OF DIGRAPHS       *        "<< endl;
cout <<"                            *                                                  *     "<< endl;
cout <<"                            ****************************************************           "<< endl;

int nbre_species;
int nbre_EC;
int nbre_auto_reg;
int nbre_screenshot;
Initial_set_up_1(nbre_species, nbre_EC, nbre_auto_reg, nbre_screenshot);
int dimension_B = nbre_auto_reg+nbre_EC;


int*    nbre_species_screenshot;
nbre_species_screenshot= new int[nbre_screenshot+1];
Initial_set_up_2(nbre_species_screenshot);

int total_length;
int total_number_cycle;
int*  vector_length_cycle_selected;
vector_length_cycle_selected = new int[dimension_B+1];
int*  vector_label_cycle_selected;
vector_label_cycle_selected = new int[dimension_B+1];

 int**  matrix_B=NULL;
 matrix_B =  new int*[dimension_B+1]; 
 for (int k=1; k<= dimension_B;++k){matrix_B[k]  = new int[dimension_B+1];} 

 int**  tableau_C=NULL;
 tableau_C =  new int*[nbre_screenshot+1+1]; 
 for (int k=1; k<= nbre_screenshot+1;++k){tableau_C[k]  = new int[dimension_B+2];} 

cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                               LOADING INPUTS                               "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;    
  
   string line, tempnum;
   int end_line = 0;
   vector<double>  input_tableau_C;
   vector<double>::iterator  input_tableau_C_it;
   double brol;
   ifstream name_file(TABLEAU_C.c_str(), ios::in);
 if (name_file.is_open()){
    while(!name_file.eof()){
       getline(name_file, line);
     while((end_line=line.find('	',0))!=string::npos){
       tempnum = line.substr(0,end_line);
       stringstream(tempnum) >> brol;
       input_tableau_C.push_back(brol);
       line.erase(0,end_line+1);
     }
     tempnum = line.substr(0,line.length());
     stringstream(tempnum) >>brol;
     input_tableau_C.push_back(brol);
  }
 name_file.close();
 }else{cout << "file can not be opened"  << endl;}
	
 
  
   end_line = 0;
   vector<double>  input_matrix_B;
   vector<double>::iterator  input_matrix_B_it;
   ifstream name_file2(  MATRIX_B.c_str(), ios::in);
 if (name_file2.is_open()){
  while(!name_file2.eof()){
     getline(name_file2, line);
     while((end_line=line.find('	',0))!=string::npos){
       tempnum = line.substr(0,end_line);
       stringstream(tempnum) >> brol;
       input_matrix_B.push_back(brol);
       line.erase(0,end_line+1);
    }
    tempnum = line.substr(0,line.length());
    stringstream(tempnum) >>brol; 
    input_matrix_B.push_back(brol);
  }
 name_file2.close();
 }else{cout << "file can not be opened"  << endl;}
 
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                               INITIALISATION OF MATRICES                               "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl; 

int label_ligne;
int label_colonne;

label_ligne=1;
label_colonne=0;  

 for (input_matrix_B_it = input_matrix_B.begin();input_matrix_B_it!= input_matrix_B.end();++input_matrix_B_it)
 {
  label_colonne++;
  if (label_ligne<=dimension_B){ matrix_B[label_ligne][label_colonne] =*input_matrix_B_it;}
  if ( label_colonne == dimension_B){label_ligne++;label_colonne = 0;}
  }

 /*
cout << "matrix_B" << endl;   
  for (int i=1; i<= dimension_B; ++i){
    for (int j=1;j<= dimension_B;++j){
     cout << matrix_B[i][j] << "  ";
    }
    cout << "" << endl;
   }
*/
label_ligne=1;
label_colonne=0;  

 for (input_tableau_C_it = input_tableau_C.begin();input_tableau_C_it!= input_tableau_C.end();++input_tableau_C_it)
 {
  label_colonne++;
  if (label_ligne<=nbre_screenshot+1){ tableau_C[label_ligne][label_colonne] =*input_tableau_C_it;}
  if ( label_colonne == dimension_B+1){label_ligne++;label_colonne= 0;}
  }
/*
 cout << "tableau_C" << endl;   
  for (int i=1; i<= nbre_screenshot+1; ++i){
    for (int j=1;j<= dimension_B+1;++j){
     cout << tableau_C[i][j] << "  ";
    }
    cout << "" << endl;
   }
*/
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                MATRICES INTIALIZED - PROCEDURE STARTS                "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl; 
output_file << "TIMEPOINT "<< "," << "NUMBER OF ACTIVE NODES "<< "," << "NUMBER OF CYCLES IN THE NUCLEUS"<< "," << "THIS COLUMN  AND THE FOLLOWING CONTAIN THE CYCLES" << endl;
output_file2 << "TIMEPOINT"<< ","<< " TOTAL NUMBER OF NUCLEI" <<endl;
for (int screenshot_loop = 1; screenshot_loop<=nbre_screenshot;++screenshot_loop ) 
{
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                                 TIMEPOINT NO. " << screenshot_loop << endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;
int nbre_species_integer = nbre_species_screenshot[screenshot_loop];
int nbre_noyau_found = 0;
int cycle_selected; 
int k1;
int**  matrix_B_inter=NULL;
 matrix_B_inter =  new int*[dimension_B+1]; 
 for (int k=1; k<= dimension_B;++k){matrix_B_inter[k]  = new int[dimension_B+1];} 

   for (int i=1; i<= dimension_B; ++i){
    for (int j=1;j<= dimension_B;++j){
       matrix_B_inter[i][j] =  matrix_B[i][j];
    }
   }

  for (int i=2; i<= dimension_B+1;++i)
  {
     if (tableau_C[screenshot_loop+1][i] == 0)
	 {
	   for (int j=1;j<=dimension_B;++j){ matrix_B_inter[i-1][j] = 0; matrix_B_inter[j][i-1] = 0;}
	 } 
  } 
  /*
  for (int i=1; i<= dimension_B; ++i){
    for (int j=i+1;j<= dimension_B;++j){
     if (matrix_B_inter[i][j] != matrix_B_inter[j][i]){cout << i<< "  " << j<< "  ";}
    }
   }

*/
for (int cycle_1 = 1; cycle_1<=dimension_B;++cycle_1)
{
   if (tableau_C[screenshot_loop+1][cycle_1+1] == 1)
   {
     total_length  = 0;
	 total_number_cycle = 0;
	 cycle_selected  =  cycle_1;
	 k1 = cycle_1;
	 total_length = total_length + tableau_C[1][cycle_selected +1];
	 total_number_cycle++;
	 vector_length_cycle_selected[total_number_cycle] = tableau_C[1][cycle_selected +1] ;
	 vector_label_cycle_selected[total_number_cycle] = cycle_selected;
    
	 noyau_finder_algorithm(screenshot_loop, cycle_1,  matrix_B_inter, dimension_B, k1,cycle_selected,nbre_noyau_found,total_length,total_number_cycle,vector_length_cycle_selected,vector_label_cycle_selected,tableau_C,nbre_species_integer);
    } 
} 
 
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "               NUMBER OF THE FOUND NUCLEI FOR TIMEPOINT NO. " << screenshot_loop << ": "<<  nbre_noyau_found<<  endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;
output_file2 <<tableau_C[screenshot_loop+1][1] << "," << nbre_noyau_found << endl;

 for (int k=1; k<= dimension_B;++k){delete[] matrix_B_inter[k];}
 delete [] matrix_B_inter;

} // end boucle principale  screenshot_loop
 output_file.close();
 output_file2.close();

 input_tableau_C.clear();
 input_matrix_B.clear();
 
 for (int k=1; k<= dimension_B;++k){delete[] matrix_B[k];}
 delete [] matrix_B;
 for (int k=1; k<= nbre_screenshot+1;++k){delete[] tableau_C[k];}
 delete [] tableau_C;
 delete [] nbre_species_screenshot;
 delete [] vector_length_cycle_selected;
 delete [] vector_label_cycle_selected;
 
cout << ""<< endl;
cout << "            ======================================================================================"<< endl; 
cout << "                            END OF THE PROCEDURE - RESULTS HAVE BEEN SAVED -  END OF THE PROGRAM      "<< endl; 
cout << "            ======================================================================================"<< endl; 
cout << ""<< endl;  
 
return 0;
} // end main function


void noyau_finder_algorithm(int& screenshot_loop, int& cycle_1, int** matrix_B_inter, int& dimension_B, int& k1,int& cycle_selected, int& nbre_noyau,int& total_length,int& total_number_cycle,int* vector_length_cycle_selected,int* vector_label_cycle_selected, int** tableau_C,int& nbre_species_integer)
{
 
  bool  noyau_found = false;
  while(matrix_B_inter[cycle_selected][k1] == 0 && k1< dimension_B){k1++;}
  
  if ( matrix_B_inter[cycle_selected][k1] == 0){noyau_found = true;}else{
   // cycle suivant trouvŽ, on teste si on continue ou  si on ferme le noyau
     int somme =0;
     for (int i=1;i<= total_number_cycle;++i){somme = somme +  matrix_B_inter[vector_label_cycle_selected[i]][k1];}
	 if(somme == total_number_cycle )
	 {
      total_number_cycle++;
	  vector_label_cycle_selected[total_number_cycle] =k1;
	  vector_length_cycle_selected[total_number_cycle] = tableau_C[1][k1 +1] ;
      total_length = total_length +vector_length_cycle_selected[total_number_cycle];
   	  cycle_selected = k1;/* APPEL RECURSIVE*/
	  noyau_finder_algorithm(screenshot_loop, cycle_1, matrix_B_inter, dimension_B,  k1, cycle_selected,  nbre_noyau, total_length, total_number_cycle, vector_length_cycle_selected, vector_label_cycle_selected, tableau_C, nbre_species_integer);
	 }else{
	  if (k1< dimension_B)
	  {
      k1++;/* APPEL RECURSIVE*/
	  noyau_finder_algorithm(screenshot_loop, cycle_1, matrix_B_inter, dimension_B,  k1, cycle_selected,  nbre_noyau, total_length, total_number_cycle, vector_length_cycle_selected, vector_label_cycle_selected, tableau_C, nbre_species_integer);
	  }else{noyau_found = true;}
	 }
  } // end  if ( matrix_B_inter([cycle_selected][k1] == 0)
  if(noyau_found == true){
    // test si le noyau  est complet
	   if (total_length ==nbre_species_integer )
       {
	    nbre_noyau++;
	  //  cout << "Nucleus No. " << nbre_noyau  << " containing   "<<  total_number_cycle <<endl;// " cycles. La longueur totale est bien egale a "<<  total_length<< " cycle initial = "<<  cycle_1<< endl; 
	  //  cout << "Labels of the correspondng cycles within this nucleus are the following (cycle length):" << endl;
	    output_file<< tableau_C[screenshot_loop+1][1]<< "," <<nbre_species_integer<< "," << total_number_cycle << "," ;
	    for (int ll=1;ll< total_number_cycle;++ll)
	    {
	    //  cout << vector_label_cycle_selected[ll]<< "( " << vector_length_cycle_selected[ll] << " ); ";
		  output_file  <<vector_label_cycle_selected[ll]<< "," ;
	    } // for (int ll=1;ll<= total_number_cycle;++ll)
	    output_file  <<vector_label_cycle_selected[total_number_cycle]<<endl; 
	 //   cout << vector_label_cycle_selected[total_number_cycle]<< "( " << vector_length_cycle_selected[total_number_cycle] <<")"<< endl; 
	   } // end if (total_length ==nbre_species_integer ) 
	   if (cycle_selected == cycle_1){return;
	   }else{
	     while(cycle_selected > cycle_1 && k1 == dimension_B)
		   {
		    total_length = total_length -vector_length_cycle_selected[total_number_cycle];
		    k1 = cycle_selected;
			total_number_cycle--;
		    cycle_selected =vector_label_cycle_selected[total_number_cycle];
			}	
		 if(k1 < dimension_B){
		 k1++;
		 /* APPEL RECURSIVE*/
		 noyau_finder_algorithm(screenshot_loop, cycle_1, matrix_B_inter, dimension_B,  k1, cycle_selected,  nbre_noyau, total_length, total_number_cycle, vector_length_cycle_selected, vector_label_cycle_selected, tableau_C, nbre_species_integer);
		 }else{return;}
	   }
   }// end if(noyau_found == true) 

} //  function end