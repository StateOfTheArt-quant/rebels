#include <rebels/environment/environment.h>
#include <rebels/utilities/config.h>
#include <vector>
#include <iostream>

void print_observation(std::vector<double> v){
    std::cout << "the current observation is: ";
    for (int n = 0; n <= v.size() - 1; n++){
        std::cout << v[n] << " ";
    }
    std::cout << std::endl;
}



int main(){

    Config config;
    environment::Environment env(config);

    std::vector<double> observation = env.reset();


    int action = 5;

    for (int step =0; step < 20; step++){

        print_observation(observation);

        auto [new_observation, reward, done] = env.step(action);
        std::cout << "the current reward is: " <<  reward << std::endl;

        if (not done){
            observation = new_observation;
        } else {
            break;
        }
    }

    return 0;

}






