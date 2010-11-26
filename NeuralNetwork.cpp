#include <iostream>
#include <vector>
using namespace std;

class IncorrectArguments{
    
};

template <class T>
struct Neuron{
    T output;
    vector<T> * weights;
    
    Neuron() : output(0){}
    
    void setSize(int size){
	weights = new vector<T>(size);
	for(int i=0; i<size; ++i)
	    weights->at(i) = 2;//random();
    }
    
    void computeOutput(vector<Neuron> & inputs){
	for(int i=0, sz=weights->size(); i<sz; ++i)
	    output += weights->at(i)*inputs[i].output;
	output = sigmoidal(output);
    }
    
    T static sigmoidal(T val){
	return val;
    }

};

template <class T>
struct NeuronsLayer{
    vector<Neuron<T> > * neurons;
    void setSize(int size, int prevSize){
	neurons = new vector<Neuron<T> >(size); //it creates vector with the size of size. For each of them default constructor of Neuron is called.
	for(int i=0; i<neurons->size(); ++i)
	    neurons->at(i).setSize(prevSize);
    }
    void computeOutputs(NeuronsLayer & prev){
	for(int i=0, sz=neurons->size(); i<sz; ++i)
	    neurons->at(i).computeOutput(*prev.neurons);
    }
    ~NeuronsLayer(){
	delete neurons;
    }
};

template <class T>
struct NeuralNetwork{
    vector<NeuronsLayer<T> > * layers;
    
    //First parameter specifies how many layers network will contain. Second is vector with sizes of each layer (so vector size must equal layersNumber)
    NeuralNetwork(int layersNumber, vector<int> sizes) throw(IncorrectArguments){
	if(layersNumber!=sizes.size())
	    throw IncorrectArguments();
	layers = new vector<NeuronsLayer<T> >(layersNumber); //it creates vector with the size of layersNumber. for each of them default constructor of NeuralsLayer is called (it does nothing interesting)
	for(int i=0; i<layersNumber; ++i)
	    layers->at(i).setSize(sizes[i], (i==0) ? 1 : sizes[i-1]);
	    
    }
    
    ~NeuralNetwork(){
	delete layers;
    }
    
    //set outputs of first layer neurons (first layer outputs are really inputs when we constider the whole NeuralNetwork consisted of a few layers.
    //size of inputs must match size of first layer
    void setInputValues(vector<T> & inputs) throw(IncorrectArguments){
	if(inputs.size() != layers->at(0).neurons->size())
	    throw IncorrectArguments();
	for(int i=0, sz=layers->at(0).neurons->size(); i<sz; ++i){
	    layers->at(0).neurons->at(i).output = inputs[i];
	    layers->at(0).neurons->at(0).weights->at(0) = 1; //it's the only weight of first layer neurons
	}
    }
    
    void compute(){
	for(int i=1; i<layers->size(); ++i)
	    layers->at(i).computeOutputs(layers->at(i-1));
    }
    
    void printResult(){
	NeuronsLayer<T> & lastLayer = layers->at(layers->size()-1);
	for(int i=0, sz=lastLayer.neurons->size(); i<sz; ++i)
	    cout << lastLayer.neurons->at(i).output << endl;
    }
};

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