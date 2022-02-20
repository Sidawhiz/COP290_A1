#pragma once
#include <vector>
#include <iostream>
using namespace std;

//========================TypeDefs===================================

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

typedef vector<float> vf;
typedef pair<float,float> pf;
typedef vector<pf> vpf;
typedef vector<vf> vvf;

//=========================MACROS====================================

#define pb push_back
#define fast ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define mod 1000000007


vvf mult_blas(vvf p, vvf q);