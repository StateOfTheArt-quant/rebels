#include <rebels/utilities/accumulator.h>
#include <iostream>



int main(){

    //Accumulator<double> f_vwap(2);
    RollingMean<double> f_volatility{5};//(5);

    for (int i=0; i<30; i++){
        double value = (double) i*1.1;
        f_volatility.push(value);

        std::cout << i << std::endl;
        std::cout <<"current mean: " << f_volatility.mean() << std::endl;
        std::cout <<"current std: " << f_volatility.std() << std::endl;
    }

    return 0;
}
