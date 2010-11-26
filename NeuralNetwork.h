/*#include <vector>

class IncorrectArguments{};

template <class T>
struct Neuron : public vector<T>{
    T output;
    
    Neuron();
    void setSize(int size);
    void computeOutput(vector<Neuron> & inputs);
    T static sigmoidal(T val);
};

template <class T>
struct NeuronsLayer : public vector<Neuron<T> >{
    void setSize(int size, int prevSize);
    void computeOutputs(NeuronsLayer<T> & prev);
};

template <class T>
struct NeuralNetwork : public vector<NeuronsLayer<T> >{
    //First parameter specifies how many layers network will contain. Second is vector with sizes of each layer (so vector size must equal layersNumber)
    NeuralNetwork(int layersNumber, vector<int> sizes) throw(IncorrectArguments);
    
    //set outputs of first layer neurons (first layer outputs are really inputs when we constider the whole NeuralNetwork consisted of a few layers.
    //size of inputs must match size of first layer
    void setInputValues(vector<T> & inputs) throw(IncorrectArguments);
    void compute();
    void printResult();
};
*/
#include <iostream>
#include <vector>
using namespace std;

class IncorrectArguments{
    
};

template <class T>
struct Neuron : public vector<T>{
    T output;
    
    Neuron() : output(0){}
    
    void setSize(int size){
	this->resize(size);
	for(int i=0; i<size; ++i)
	    this->at(i) = 2;//random();
    }
    
    void computeOutput(vector<Neuron> & inputs){
	for(int i=0, sz=this->size(); i<sz; ++i)
	    output += this->at(i) * inputs[i].output;
	output = Neuron<T>::sigmoidal(output);
    }
    
    T static sigmoidal(T val){
	return val;
    }

};

template <class T>
struct NeuronsLayer : public vector<Neuron<T> >{
    void setSize(int size, int prevSize){
	this->resize(size);
	for(int i=0; i<this->size(); ++i)
	    this->at(i).setSize(prevSize);
    }
    void computeOutputs(NeuronsLayer<T> & prev){
	for(int i=0, sz=this->size(); i<sz; ++i)
	    this->at(i).computeOutput(prev);
    }
};

template <class T>
struct NeuralNetwork : public vector<NeuronsLayer<T> >{
    //First parameter specifies how many layers network will contain. Second is vector with sizes of each layer (so vector size must equal layersNumber)
    NeuralNetwork(int layersNumber, vector<int> sizes) throw(IncorrectArguments){
	if(layersNumber!=sizes.size())
	    throw IncorrectArguments();
	this->resize(layersNumber);
	for(int i=0; i<layersNumber; ++i)
	    this->at(i).setSize(sizes[i], (i==0) ? 1 : sizes[i-1]);
	    
    }
    
    //set outputs of first layer neurons (first layer outputs are really inputs when we constider the whole NeuralNetwork consisted of a few layers.
    //size of inputs must match size of first layer
    void setInputValues(vector<T> & inputs) throw(IncorrectArguments){
	if(inputs.size() != this->at(0).size())
	    throw IncorrectArguments();
	for(int i=0, sz=this->at(0).size(); i<sz; ++i){
	    this->at(0).at(i).output = inputs[i]; //set 0 layer, i-th neuron's output
	    this->at(0).at(i).at(0) = 1; //it's the only weight of first layer neurons
	}
    }
    
    void compute(){
	for(int i=1; i<this->size(); ++i)
	    this->at(i).computeOutputs(this->at(i-1));
    }
    
    void printResult(){
	NeuronsLayer<T> & lastLayer = this->at(this->size()-1);
	for(int i=0, sz=lastLayer.size(); i<sz; ++i)
	    cout << lastLayer.at(i).output << endl;
    }
};