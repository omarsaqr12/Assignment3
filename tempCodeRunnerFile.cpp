
#include <cmath>
#include "BinaryTree.h"
#include "Stackt.cpp"
#include "Queuet.cpp"
#include "binaryTree.cpp"
#include "Stackt.h"
#include "Queuet.h"
using namespace std;
vector<string> unco(string data){  // this function is used to undo the concatenation of  the data that was extracted from the resulting search from the binary search tree
    vector<string>vec(3);
    string a,b,c; int count =0;
    for (int i = 0; i < data.size(); ++i) {
if(count==0){ // we take the first string which is latitude
    if(data[i]==' '){count++;}
    else{a.push_back(data[i]);}}
else if(count ==1){// we take the second string which is longitude
    if(data[i]==' '){count++;}
    else{b.push_back(data[i]);}}
else{ // we take the last component of the data which is country
c.push_back(data[i]);
}
    }
    vec[0]=a;vec[1]=b;vec[2]=c;
return vec;}
double haversine(double lat1, double lon1,
                 double lat2, double lon2)
{

    //this function is used to calculate the distance between two latitudes and two longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371.137;
    double c = 2 * atan2(sqrt(a),sqrt(1-a));
    return rad * c;
}
double  converttorad(string s){ // we convert the string latitude, and longitude to numbers to be able to use it in haversine formula
    bool deg=true;  bool min=true; string degree; string minute; string second;

for(int i=0; i<=s.length()-2;i++){

    if(deg==true){
        if(s[i]=='d'){deg=false;}
        else{degree.push_back(s[i]);}
    }
    else if(min==true){

        if(s[i]=='l'){min=false;}
        else{minute.push_back(s[i]);}
    }
    else{
        second.push_back(s[i]);
    }
}
if(second==""){second="0";}
    int result=stoi(degree)+ stoi(minute)/60.0+stoi(second)/3600.0;
if(s[s.size()-1]=='N'||s[s.size()-1]=='S'=='W'){result*=-1;}
    return result;
}
void read(ifstream &inputFile, binaryTree<string,string> &tree){ // we take the data from the file and input it into the tree
    string line=""; int x=0;
    while (getline(inputFile, line)) {
        string  number;
        string city;
        string Latitude;
        string Longitude;
        string Country;
        if(x!=0){
            stringstream inputString(line);

            //StudentId, Last Name, FirstName, Age, Phone Number, GPA
            getline(inputString, number, ',');
            getline(inputString, city, ',');
            getline(inputString, Latitude, ',');
            getline(inputString, Longitude, ',');
            getline(inputString, Country);
            line = "";
             const string key=city;
             const string data =Latitude+" "+Longitude+" "+Country;
            tree.insert(key,data);

        }
        x++;