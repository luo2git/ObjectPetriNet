#include<iostream>
#include <chrono>
#include"Petrinet.h"
#include"read_json.h"
using namespace std;

int main() {
	read_place_json();
	read_trans_json();
	read_tokens_json();
	petrinet.dijskstra_search();
	petrinet.bestpath = petrinet.Createbestpath();
	return 0;
}