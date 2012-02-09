#ifndef TWODIM_H
#define TWODIM_H
#include <iostream>

class TwoDim{
        public:
        TwoDim(int rows, int columns) : rows(rows), columns(columns){
            values = new double[rows * columns];
            for(int i=0; i<rows; ++i)
                for(int j=0; j<columns; ++j)
                    get(i, j) = 0;
        }

        ~TwoDim(){
            delete [] values;
        }

        double & operator() (int row, int column) const {
            return values[row*columns + column];
        }

        void print() const{
            for(int i=0; i<rows; ++i){
                for(int j=0; j<columns; ++j)
                    std::cout << get(i, j) << " ";
                std::cout << std::endl;
            }
        }

        double * values;
        int rows, columns;

        double & get(int row, int column) const{
            return values[row*columns + column];
        }
};

#endif // TWODIM_H
