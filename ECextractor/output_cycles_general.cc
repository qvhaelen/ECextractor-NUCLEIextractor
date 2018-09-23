#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
#include<sstream>
#include"output_cycles_general.h"

using namespace std;
void output_cycles_general(int& number_analyzed_tp, int& tp_start, int& tp_end,int& number_vertices ,int** Summary, int**  Auto_reg_summary ){
 string OUTPUT_FILE_NAME ="output_data/";
string  filename = OUTPUT_FILE_NAME+ "SUMMARY-SCC-CYCLES.csv";
ofstream output_file(filename.c_str());
if (output_file.is_open()){
  for ( int j=tp_start; j<= tp_end ;++j){
    if (Summary[j][6] != 0){output_file <<  Summary[j][1] << "," <<  Summary[j][2] << "," <<Summary[j][3] << ","<< Summary[j][4] << ","<< Summary[j][5] <<","<< Summary[j][6]  <<endl;}
  }
  output_file.close();
}else{cout << "file can not be opened"  << endl;}
 
string  filename6 =OUTPUT_FILE_NAME+  "TIME_EVOLUTION_AUTO_REGULATIONS.csv";
ofstream output_file6(filename6.c_str());
if (output_file6.is_open()){
 for ( int j=tp_start; j<= tp_end ;++j){
  if (Summary[j][6] != 0){
     for (int i=1;i<=number_vertices ;++i){if (i != number_vertices){output_file6 << Auto_reg_summary[j][i] <<",";}else{output_file6 << Auto_reg_summary[j][i]<< endl; }}
   }
 }
output_file6.close();
}else{cout << "file can not be opened"  << endl;} 
string  filename7 =OUTPUT_FILE_NAME+  "SUMMARY_AUTO_REGULATIONS.csv";
ofstream output_file7(filename7.c_str());
if (output_file7.is_open()){
 for ( int j=tp_start; j<= tp_end ;++j){
  if (Summary[j][6] != 0){
   output_file7 << Summary[j][6] <<"," <<Summary[j][7]<<","<< Summary[j][8] << endl;
  }
 }
output_file7.close();
}else{cout << "file can not be opened"  << endl;} 
return;}




