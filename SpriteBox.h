
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

struct BoxRect{
    float mX;
    float mY;
    float mW;
    float mH;
};
struct attackboxes{
    int Index;
    vector<BoxRect> AttackRects;
    };

vector<attackboxes>  AttackBoxArray();
