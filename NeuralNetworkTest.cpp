#include "NeuralNetwork.h"

using namespace NN;
int main(){
    vector<int> v;
    v.push_back(2);
    v.push_back(3);
    v.push_back(1);
    vector<int> inputs;
    inputs.push_back(7);
    inputs.push_back(3);
    //inputs.push_back(0);
    //inputs.push_back(1);
    try{
	NeuralNetwork<int> nn(3, v);
	nn.setInputValues(inputs);
	nn.compute();
	nn.printResult();
    }catch(IncorrectArguments e){
	cout << "Something wrong during constructing NeuralNetwork" << endl;
    }
}