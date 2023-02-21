/*******************************************************************************
 * Created by Robert Fraczkiewicz, 01/2018
 * New signal processing methodology for obtaining heart rate and SpO2 data 
 * from the MAX30102 sensor manufactured by MAXIM Integrated Products, Inc.
 *
 * Modified: 06/11/2020 JLC Very slight mods to port from Arduino to RaspPi.
 *                            No intention to modify RF's original contribution.
 * 
 *******************************************************************************/

/*******************************************************************************
 * Copyright (C) 2017 Robert Fraczkiewicz, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL ROBERT FRACZKIEWICZ BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Robert Fraczkiewicz retains all
 * ownership rights.
 *******************************************************************************/
#ifndef ALGORITHM2_BY_RF_H_
#define ALGORITHM2_BY_RF_H_

#include "MAX30101.h"

/*
 * Settable parameters 
 * Leave these alone if your circuit and hardware setup match the defaults 
 * described in this code's Instructable. Typically, different sampling rate
 * and/or sample length would require these paramteres to be adjusted.
 */

//#define ST 4      // Sampling time in s. WARNING: if you change ST,
                  //   then you MUST recalcuate the sum_X2 parameter below!
//#define FS 25     // Sampling frequency in Hz. WARNING: if you change FS,
                  //   then you MUST recalcuate the sum_X2 parameter below!
// Sum of squares of ST*FS numbers from -mean_X (see below) to +mean_X
//   incremented be one. For example, given ST=4 and FS=25,
//   the sum consists of 100 terms: (-49.5)^2 + (-48.5)^2 + (-47.5)^2 +
//     ... + (47.5)^2 + (48.5)^2 + (49.5)^2
// The sum is symmetrc, so you can evaluate it by multiplying its positive
//   half by 2. It is precalcuated here for enhanced performance.

// WARNING: you MUST recalculate this sum if you changed either ST or FS above!
const double sum_X2 = 83325;  

// WARNING: The two parameters below are CRUCIAL! Proper HR evaluation
//            depends on these.
#define MAX_HR 180  // Maximal heart rate. To eliminate erroneous signals,
                    //   calculated HR should never be greater than this number.
#define MIN_HR 40   // Minimal heart rate. To eliminate erroneous signals,
                    //   calculated HR should never be lower than this number.
// Minimal ratio of two autocorrelation sequence elements:
//   one at a considered lag to the one at lag 0.
// Good quality signals must have such ratio greater than this minimum.
const double min_autocorrelation_ratio = 0.5;

// Pearson correlation between red and IR signals.
// Good quality signals must have their correlation coefficient greater
//   than this minimum.
const double min_pearson_correlation = 0.8;

/*
 * Derived parameters 
 * Do not touch these! 
 * 
 */
//Chengsen FS=25
const int32_t FS60 = 25*60;                        // Conversion factor for HR from bps to bpm
const int32_t LOWEST_PERIOD = FS60/MAX_HR;         // Minimal distance between peaks
const int32_t HIGHEST_PERIOD = FS60/MIN_HR;        // Maximal distance between peaks
//Chengsen BUFFER_SIZE=100
const double mean_X = (double)(100-1)/2.0; // Mean value of the set of integers from
                                                   //   0 to BUFFER_SIZE-1.
                                                   //   For ST=4 and FS=25 it's equal to 49.5.

void rf_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer, int32_t n_ir_buffer_length,
					 uint32_t *pun_red_buffer, double *pn_spo2,
					 int8_t *pch_spo2_valid, int32_t *pn_heart_rate, 
                                         int8_t *pch_hr_valid, double *ratio, double *correl);
double rf_linear_regression_beta(double *pn_x, double xmean, double sum_x2);
double rf_autocorrelation(double *pn_x, int32_t n_size, int32_t n_lag);
double rf_rms(double *pn_x, int32_t n_size, double *sumsq);
double rf_Pcorrelation(double *pn_x, double *pn_y, int32_t n_size);
void rf_initialize_periodicity_search(double *pn_x, int32_t n_size, int32_t *p_last_periodicity,
				      int32_t n_max_distance, double min_aut_ratio, double aut_lag0);
void rf_signal_periodicity(double *pn_x, int32_t n_size, int32_t *p_last_periodicity,
			   int32_t n_min_distance, int32_t n_max_distance, double min_aut_ratio,
			   double aut_lag0, double *ratio);

#endif /* ALGORITHM2_BY_RF_H_ */

