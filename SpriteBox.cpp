
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <vector>
//#include <SpriteBox.h>
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
/*
vector<attackboxes>  AttackBoxArray(){
    vector<attackboxes> ReturnArray;
    attackboxes TempBoxes;
    BoxRect TempRect;
    ifstream InFile("input.txt");
    int temp = 0;
    int BoxIndex = 0;
    InFile >> temp;
    while(!InFile.eof()){
        InFile >> temp;
        if(temp != 1111){
            TempRect.mX = temp;
            for(int i = 0; i <3; i++){
                switch(i){
                case 0: InFile>> TempRect.mY; break;
                case 1: InFile>> TempRect.mW; break;
                case 2: InFile>> TempRect.mH; break;
                        }
                    }
                    TempBoxes.AttackRects.push_back(TempRect);
                }

        else{
            TempBoxes.Index = BoxIndex;
            BoxIndex++;
            ReturnArray.push_back(TempBoxes);
            TempBoxes.AttackRects.clear();

        }



            }
        return ReturnArray;
        }


*/

/*
int main()
{
  /*
    vector<attackboxes> TestArray;

    TestArray = AttackBoxArray();
   //

 for(int i = 0; i < TestArray.size(); i++){
                cout<<TestArray.at(i).Index<<" ";
                cout<<TestArray.at(i).AttackRects.at(j).mW<<" ";
                cout<<TestArray.at(i).AttackRects.at(j).mH<<" ";
        for(int j = 0; j<TestArray.at(i).AttackRects.size(); j++){

                cout<<TestArray.at(i).AttackRects.at(j).mX<<" ";
                cout<<TestArray.at(i).AttackRects.at(j).mY<<" ";
                cout<<" NextRect ";
        }
        cout<<endl;
   }



*/

  //  return 0;
//}
