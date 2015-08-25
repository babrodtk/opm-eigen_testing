/*
 * timing_utils.hpp
 *
 *  Created on: Aug 24, 2015
 *      Author: babrodtk
 */

#ifndef SRC_TIMING_UTILS_HPP_
#define SRC_TIMING_UTILS_HPP_


#include <boost/chrono.hpp>


typedef boost::chrono::high_resolution_clock::time_point time_point;
typedef boost::chrono::high_resolution_clock::duration duration;

inline time_point getCurrentTime() {
    return boost::chrono::high_resolution_clock::now();
}

unsigned long long toMicroSeconds(const duration& tp) {
    return boost::chrono::duration_cast<boost::chrono::microseconds>(tp).count();
}

double toSeconds(const duration& tp) {
    return toMicroSeconds(tp)/1000000.0;
}



#endif /* SRC_TIMING_UTILS_HPP_ */
