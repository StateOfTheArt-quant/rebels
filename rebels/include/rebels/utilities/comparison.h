#pragma once

#define _TOL_ 10000

#include <cmath>

// template with non-type parameter and default arguments
template<unsigned int TOL = _TOL_>
struct FloatComparator{
    FloatComparator() = default;

    bool operator()(double a, double b) const{
        return rint(a*TOL) < rint(b*TOL);
    }
};


template<unsigned int TOL=_TOL_>
struct ReverseFloatComparator{
    ReverseFloatComparator() = default;

    bool operator()(double a, double b) const{
        return rint(b*TOL) < rint(a*TOL);
    }
};

template<unsigned int TOL = _TOL_>
bool approx_equal(double a, double b){
    /*
     * a=3,b=3; true
     * a=3.01, b=3
     */
    return rint(a*TOL) == rint(b*TOL);
}
