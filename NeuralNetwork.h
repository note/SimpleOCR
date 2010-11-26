#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

namespace NN{
    using namespace std;
    
    class IncorrectArguments{
	
    };

    /*
    Function returns a pseudo-random number in the range -1.0 to 1.0 (including -1.0 and excluding 1.0)
    */
    template <class T>
    T random(){
	T val = rand(); /*No need of explicit cast*/
	    /*For val==RAND_MAX function should return the biggest possible value smaller than 1. For val==0 function should return -1. RAND_MAX is a constant defined in <cstdlib>. Because of assymetry of range of integer RAND_MAX must be odd number. So:*/
	return(val-RAND_MAX/2-1)/(RAND_MAX/2+1);
    }

    template <class T>
    struct Neuron : public vector<T>{
	T output;
        T error;
	
	Neuron() : output(0){}
	
	void setSize(int size){
	    this->resize(size);
	    for(int i=0; i<size; ++i)
                this->at(i) = random<T>(); //2
	}
	
	void computeOutput(vector<Neuron> & inputs){
            output = 0; //it's neccessary when calling computerOutput many times
	    for(int i=0, sz=this->size(); i<sz; ++i)
		output += this->at(i) * inputs[i].output;
            output = Neuron<T>::sigmoidal(output);
	}
	
	//prev actually is next layer, but in this phase we're moving backwards
	void backpropagation(vector<Neuron> & prev, int weightIndex){
            error = 0;
	    for(int i=0; i<prev.size(); ++i)
                error += prev[i].error * prev[i][weightIndex];
	}
	
	//http://en.wikipedia.org/wiki/Sigmoid_function
	T static sigmoidal(T val){
            return 1.0/(1.0+exp(-val));
	}

        void update(vector<Neuron> & prev, T learningRate){
            for(int i=0; i<this->size(); ++i)
                this->at(i) += learningRate*error*sigmoidal(output)*(1-sigmoidal(output))*prev[i].output;
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
	//parallel
	void backpropagation(NeuronsLayer<T> & prev){
            for(int i=0, sz=this->size(); i<sz; ++i)
		this->at(i).backpropagation(prev, i);
	}

        void update(NeuronsLayer<T> & prev, T learningRate){
            for(int i=0, sz=this->size(); i<sz; ++i)
                this->at(i).update(prev, learningRate);
        }
    };

    template <class T>
    struct NeuralNetwork : public vector<NeuronsLayer<T> >{
        int debug;
	//First parameter specifies how many layers network will contain. Second is vector with sizes of each layer (so vector size must equal layersNumber)
	NeuralNetwork(int layersNumber, vector<int> sizes) throw(IncorrectArguments){
	    srand(time(NULL));
            debug = 0;
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
	
	//todo. so far let assume that there is only one neuron in last layer
	//complexity o(n*n*l), where n - size of the biggest layer, l number of layers
        bool backpropagation(T expectedResult, T epsilon, T learningRate){
            debug++;
	    NeuronsLayer<T> & lastLayer = this->at(this->size()-1);
           // if(abs(expectedResult-lastLayer[0].output) > epsilon){
                lastLayer[0].error = expectedResult-lastLayer[0].output; //todo cosider more sophisticated method of computing Error
                for(int i=this->size()-2; i>0; --i) //this->size()-2 because we've already handle last layer by setting lastLayer[0].error, i>0 because we don't want to compute error for first layer
		    this->at(i).backpropagation(this->at(i+1));
                for(int i=1; i<this->size(); ++i){
                    this->at(i).update(this->at(i-1), learningRate);
                    debug++;
                }
                return true;
            //}
            return false;
	}
    };
}
