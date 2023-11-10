//
//  main.cpp
//  LLTemplate
//
//  Created by James Shockey on 12/6/16.
//  Copyright © 2016 James Shockey. All rights reserved.
//


/*
 *
 *	Linked List lab.
 *	- Build a library for singly linked list.
 *	- Replace the airport array in main with a Linked List.
 *  - sort the array. 
 *	
 */

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath> 
//#include "slist.h"

using namespace std;
//using namespace slist;

class Airport
{
public:
    Airport() {}
    char    code[5];
    double   longitude;
    double   latitude;
    
};

template<class T> class listNode {
public:
  T object;
  listNode *ptr;
  void kill() {

    if (ptr == nullptr) {
      free(this);
    } else {
      ptr->kill();
      free(this);
    } 

  }
};

template<class T> class SingleList {

public:
  listNode<T> *headPtr;
  int length;

  SingleList() {
    headPtr = nullptr;
    length = 0;
  }

  void add(T value) {

    listNode<T> *newPtr = (listNode<T>*)malloc(sizeof(listNode<T>));

    newPtr->object = value;
    newPtr->ptr = nullptr;

    if (headPtr == nullptr) {

      headPtr = newPtr;

      length++;

    } else {

      listNode<T> *current = headPtr;

      while (current->ptr != nullptr) {

        listNode<T> *temp = current->ptr;

        current = temp;

      }

      current->ptr = newPtr;

      length++;
    }
  }

  void clear() { 
    if (headPtr != nullptr) {
      headPtr->kill(); 
      headPtr = nullptr;
    }
  }

  //~SingleList() { clear(); }

  bool equals(SingleList<T>* obj) {

    bool isEq = true;

    if (this->length != obj->length) {
      isEq = false;
      return isEq;
    }

    listNode<T> *current = headPtr;
    listNode<T> *objCurrent = obj->headPtr;

    while (current->ptr != nullptr && objCurrent->ptr != nullptr) {

      listNode<T> *temp1 = current->ptr->ptr;
      listNode<T> *temp2 = objCurrent->ptr->ptr;

      current = temp1;
      objCurrent = temp2;

      double lat1 = ((current->object).latitude);
      double lat2 = ((objCurrent->object).latitude);

      double lon1 = ((current->object).longitude);
      double lon2 = ((objCurrent->object).longitude);

      if (lat1 != lat2 || lon1 != lon2) {

        return false;

      }

    }

    return isEq;
  }

  T get(int idx) {

    listNode<T> *current = headPtr;

    for (int i = 0; i <= idx; i++) {

      if (idx == i) {
        break;
      } else {

        listNode<T> *temp = current->ptr;

        current = temp;

      }

    }

    return current->object;

  }

  listNode<T>* getNodePtr(int idx) {

    listNode<T> *current = headPtr;

    for (int i = 0; i <= idx; i++) {

      if (idx == i) {
        break;
      } else {

        listNode<T> *temp = current->ptr;

        current = temp;

      }

    }

    return current;

  }

  void insert(T obj, int idx) {

    listNode<T> *current = headPtr;

    for (int i = 0; i < (idx-1); i++) {

      if (idx == i) {
        break;
      } else {

        listNode<T> *temp = current->ptr;

        current = temp;

      }

    }

    listNode<T> *temp = current->ptr;

    listNode<T> *newPtr = (listNode<T> *)malloc(sizeof(listNode<T>));

    if (idx == 0) {

      headPtr = newPtr;

    } else {

      current->ptr = newPtr;

    }

    newPtr->object = obj;
    newPtr->ptr = temp;

    length++;

  }

  void exchg(int idx1, int idx2) {

    listNode<T>* trg1 = getNodePtr(idx1);
    listNode<T>* trg2 = getNodePtr(idx2);

    T obj1 = get(idx1);
    T obj2 = get(idx2);

    trg1->object = obj2;
    trg2->object = obj1;

  }

  void swap(int idx1, int idx2) {

    listNode<T>* trg1 = getNodePtr(idx1);
    listNode<T>* trg2 = getNodePtr(idx2);

    listNode<T>* prior = getNodePtr(idx1-1);

    prior = trg2;
    trg1 = trg2;
    trg2 = trg1;

  }

  bool isEmpty() {

    if (headPtr == nullptr) {
      return true;
    } else {
      return false;
    }

  }

  void remove(int idx) {

    if (idx == 0) {

      listNode<T> *temp = headPtr->ptr;
      free(headPtr);
      headPtr = temp;
      length--;

    } else {

      listNode<T>* trg = getNodePtr(idx);
      listNode<T>* prior = getNodePtr(idx-1);
      prior->ptr = trg->ptr;
      free(trg);
      length--;

    }

  }

  void set(T obj, int idx) {

    listNode<T>* trg = getNodePtr(idx);

    trg->object = obj;

  }

  int size() { return length; }

};

void simpleSortTotal(SingleList<Airport> lkList, int c);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

Airport parseStringToObj(string str) {

  Airport ret = Airport();
  int stringPart = 1;
  string toFill = "";
  
  for (int i = 0; i < str.length(); i++) {

    if (str.at(i) == ',') {

      stringPart++;
      continue;
      
    }

    if (stringPart == 1) {

      ret.code[i] = str.at(i);

      if (i == 4) {
        ret.code[4] = '\0';
      }
      
    } else if (stringPart == 4) {

      toFill += str.at(i);

      if (str.at(i+1) == ',') {

        //cout << toFill << endl;
        
        ret.longitude = stof(toFill);

        toFill = "";
        
      }
      
    } else if (stringPart == 5) {

      toFill += str.at(i);

      if (str.at(i+1) == ',') {

        //cout << toFill << endl;
        
        ret.latitude = stof(toFill);

        toFill = "";

      }
      
    } else if (stringPart > 4) {

      break;
      
    }
    
  }

  return ret;
  
}

int main() {

  ifstream csv_file;
  int length;
  char *buff;

  csv_file.open("airport-codes_US.csv", ios::out);
  csv_file.seekg(0, ios::end);
  length = csv_file.tellg();
  csv_file.seekg(0, ios::beg);
  buff = new char[length];
  csv_file.read(buff, length);
  
  string str = "";
  int lineCount = 0;

  SingleList<Airport> linkedList = SingleList<Airport>();
  
  for (int i = 124; i < length; i++) {
    
    if (buff[i] != '\n') {

      str += buff[i];

    } else if (buff[i] == '\n') {

      Airport val = parseStringToObj(str);

      linkedList.add(val);
      
      str = "";

      lineCount++;

    }

    if (lineCount > 22337) {

      break;
      
    }
    
  }

  linkedList.remove(0);
  
  simpleSortTotal(linkedList, (linkedList.length-1));

  cout << "Farthest currently active airport entry: " << linkedList.get((linkedList.length-5)).code << endl;

  cout << "List of the 100 closest:" << endl;

  for (int i = 1; i < 101; i++) {

    cout << linkedList.get(i).code << endl;
    
  }
  
}



#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}


/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void simpleSortTotal(SingleList<Airport> lkList, int c) {

  double ausLat = 30.19449997;
  double ausLon = -97.66989899;

  bool sorted = false;

  while(!sorted) {

    bool thingMoved = false;
    
    listNode<Airport> *current = lkList.headPtr;
    
    for (int i = 0; i < c; i++) {

      if (distanceEarth(ausLat, ausLon, (current->object).latitude, (current->object).longitude) > distanceEarth(ausLat, ausLon, (current->ptr->object).latitude, (current->ptr->object).longitude)) {

        Airport temp = current->object;
        current->object = current->ptr->object;
        current->ptr->object = temp;

        thingMoved = true;

        //cout << "swapped" << endl;
        
      }

      listNode<Airport> *tPtr = current->ptr;
      current = tPtr;
      
    }

    if (!thingMoved) {

      sorted = true;
      
    }
    
  }
  
}

/*
	Provide sort routine on linked list
*/
/*
void simpleSortTotal()
{
}
*/
