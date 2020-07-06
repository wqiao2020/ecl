/****************************************************************************
 *
 *   Copyright (c) 2015 Estimation and Control Library (ECL). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name ECL nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file sideslip_fusion.cpp
 * sideslip fusion methods.
 * equations generated using EKF/python/ekf_derivation/main.py
 *
 * @author Carl Olsson <carlolsson.co@gmail.com>
 * @author Paul Riseborough <p_riseborough@live.com.au>
 *
 */

#include "ekf.h"
#include <ecl.h>
#include <mathlib/mathlib.h>

void Ekf::fuseSideslip()
{
	// get latest estimated orientation
	const float &q0 = _state.quat_nominal(0);
	const float &q1 = _state.quat_nominal(1);
	const float &q2 = _state.quat_nominal(2);
	const float &q3 = _state.quat_nominal(3);

	// get latest velocity in earth frame
	const float &vn = _state.vel(0);
	const float &ve = _state.vel(1);
	const float &vd = _state.vel(2);

	// get latest wind velocity in earth frame
	const float &vwn = _state.wind_vel(0);
	const float &vwe = _state.wind_vel(1);

	// calculate relative wind velocity in earth frame and rotate into body frame
	const Vector3f rel_wind_earth(vn - vwn, ve - vwe, vd);
	const Dcmf earth_to_body = quatToInverseRotMat(_state.quat_nominal);
	const Vector3f rel_wind_body = earth_to_body * rel_wind_earth;

	// perform fusion of assumed sideslip  = 0
	if (rel_wind_body.norm() > 7.0f) {
		float Hfusion[9];  // Observation Jacobians - Note: indexing is different to state vector
		float Kfusion[24]; // Kalman gain vector
		float R_BETA = sq(_params.beta_noise); // observation noise variance

		// Intermediate Values
		const float HK0 = vn - vwn;
		const float HK1 = ve - vwe;
		const float HK2 = HK0*q0 + HK1*q3 - q2*vd;
		const float HK3 = q0*q2 - q1*q3;
		const float HK4 = 2*vd;
		const float HK5 = q0*q3;
		const float HK6 = q1*q2;
		const float HK7 = 2*HK5 + 2*HK6;
		const float HK8 = powf(q0, 2);
		const float HK9 = powf(q3, 2);
		const float HK10 = HK8 - HK9;
		const float HK11 = powf(q1, 2);
		const float HK12 = powf(q2, 2);
		const float HK13 = HK11 - HK12;
		const float HK14 = HK10 + HK13;
		const float HK15 = HK0*HK14 + HK1*HK7 - HK3*HK4;
		const float HK16 = 1.0F/HK15;
		const float HK17 = q0*q1 + q2*q3;
		const float HK18 = HK10 - HK11 + HK12;
		const float HK19 = HK16*(-2*HK0*(HK5 - HK6) + HK1*HK18 + HK17*HK4);
		const float HK20 = -HK0*q3 + HK1*q0 + q1*vd;
		const float HK21 = -HK19*HK2 + HK20;
		const float HK22 = 2*HK16;
		const float HK23 = HK0*q1 + HK1*q2 + q3*vd;
		const float HK24 = HK0*q2 - HK1*q1 + q0*vd;
		const float HK25 = -HK19*HK23 + HK24;
		const float HK26 = HK19*HK24 + HK23;
		const float HK27 = HK19*HK20 + HK2;
		const float HK28 = HK14*HK19 + 2*HK5 - 2*HK6;
		const float HK29 = HK16*HK28;
		const float HK30 = HK19*HK7;
		const float HK31 = HK17 + HK19*HK3;
		const float HK32 = HK13 + HK30 - HK8 + HK9;
		const float HK33 = 2*HK31;
		const float HK34 = 2*HK26;
		const float HK35 = 2*HK25;
		const float HK36 = 2*HK27;
		const float HK37 = 2*HK21;
		const float HK38 = HK28*P(0,22) - HK28*P(0,4) + HK32*P(0,23) - HK32*P(0,5) + HK33*P(0,6) + HK34*P(0,2) + HK35*P(0,1) - HK36*P(0,3) + HK37*P(0,0);
		const float HK39 = powf(HK15, -2);
		const float HK40 = -HK28*P(4,6) + HK28*P(6,22) - HK32*P(5,6) + HK32*P(6,23) + HK33*P(6,6) + HK34*P(2,6) + HK35*P(1,6) - HK36*P(3,6) + HK37*P(0,6);
		const float HK41 = HK32*P(5,23);
		const float HK42 = HK28*P(22,23) - HK28*P(4,23) + HK32*P(23,23) + HK33*P(6,23) + HK34*P(2,23) + HK35*P(1,23) - HK36*P(3,23) + HK37*P(0,23) - HK41;
		const float HK43 = HK32*HK39;
		const float HK44 = HK28*P(4,22);
		const float HK45 = HK28*P(22,22) + HK32*P(22,23) - HK32*P(5,22) + HK33*P(6,22) + HK34*P(2,22) + HK35*P(1,22) - HK36*P(3,22) + HK37*P(0,22) - HK44;
		const float HK46 = HK28*HK39;
		const float HK47 = -HK28*P(4,5) + HK28*P(5,22) - HK32*P(5,5) + HK33*P(5,6) + HK34*P(2,5) + HK35*P(1,5) - HK36*P(3,5) + HK37*P(0,5) + HK41;
		const float HK48 = -HK28*P(4,4) + HK32*P(4,23) - HK32*P(4,5) + HK33*P(4,6) + HK34*P(2,4) + HK35*P(1,4) - HK36*P(3,4) + HK37*P(0,4) + HK44;
		const float HK49 = HK28*P(2,22) - HK28*P(2,4) + HK32*P(2,23) - HK32*P(2,5) + HK33*P(2,6) + HK34*P(2,2) + HK35*P(1,2) - HK36*P(2,3) + HK37*P(0,2);
		const float HK50 = HK28*P(1,22) - HK28*P(1,4) + HK32*P(1,23) - HK32*P(1,5) + HK33*P(1,6) + HK34*P(1,2) + HK35*P(1,1) - HK36*P(1,3) + HK37*P(0,1);
		const float HK51 = HK28*P(3,22) - HK28*P(3,4) + HK32*P(3,23) - HK32*P(3,5) + HK33*P(3,6) + HK34*P(2,3) + HK35*P(1,3) - HK36*P(3,3) + HK37*P(0,3);
		float HK52;

		// determine if we need the sideslip fusion to correct states other than wind
		bool update_wind_only = !_is_wind_dead_reckoning;

		// innovation variance
		_beta_innov_var = (HK33*HK39*HK40 + HK34*HK39*HK49 + HK35*HK39*HK50 - HK36*HK39*HK51 + HK37*HK38*HK39 + HK42*HK43 - HK43*HK47 + HK45*HK46 - HK46*HK48 + R_BETA);
		if (_beta_innov_var >= R_BETA) {
			HK52 = HK16 / _beta_innov_var;
			_fault_status.flags.bad_sideslip = false;

		} else { // Reset the estimator
			_fault_status.flags.bad_sideslip = true;

			// if we are getting aiding from other sources, warn and reset the wind states and covariances only
			const char* action_string = nullptr;
			if (update_wind_only) {
				resetWindStates();
				resetWindCovariance();
				action_string = "wind";

			} else {
				initialiseCovariance();
				_state.wind_vel.setZero();
				action_string = "full";
			}
			ECL_ERR("sideslip badly conditioned - %s covariance reset", action_string);

			return;
		}

		// Calculate predicted sideslip angle and innovation using small angle approximation
		_beta_innov = rel_wind_body(1) / rel_wind_body(0);

		// Compute the ratio of innovation to gate size
		_beta_test_ratio = sq(_beta_innov) / (sq(fmaxf(_params.beta_innov_gate, 1.0f)) * _beta_innov_var);

		// if the innovation consistency check fails then don't fuse the sample and indicate bad beta health
		if (_beta_test_ratio > 1.0f) {
			_innov_check_fail_status.flags.reject_sideslip = true;
			return;

		} else {
			_innov_check_fail_status.flags.reject_sideslip = false;
		}

		// Observation Jacobians
		// Note: indexing is different to state vector 
		Hfusion[0] = HK21*HK22;			// state index 0
		Hfusion[1] = HK22*HK25;			// state index 1
		Hfusion[2] = HK22*HK26;			// state index 2
		Hfusion[3] = -HK22*HK27;		// state index 3
		Hfusion[4] = -HK29;				// state index 4
		Hfusion[5] = HK16*(HK18 - HK30);// state index 5
		Hfusion[6] = HK22*HK31;			// state index 6
		Hfusion[7] = HK29;				// state index 22
		Hfusion[8] = HK16*HK32;			// state index 23

		// Calculate Kalman gains
		if (update_wind_only) {
			// If we are getting aiding from other sources, then don't allow the sideslip fusion to affect the non-windspeed states
			for (unsigned row = 0; row <= 21; row++) {
				Kfusion[row] = 0.0f;

			}

		} else {
			Kfusion[0] = HK38*HK52;
			Kfusion[1] = HK50*HK52;
			Kfusion[2] = HK49*HK52;
			Kfusion[3] = HK51*HK52;
			Kfusion[4] = HK48*HK52;
			Kfusion[5] = HK47*HK52;
			Kfusion[6] = HK40*HK52;
			Kfusion[7] = HK52*(-HK28*P(4,7) + HK28*P(7,22) - HK32*P(5,7) + HK32*P(7,23) + HK33*P(6,7) + HK34*P(2,7) + HK35*P(1,7) - HK36*P(3,7) + HK37*P(0,7));
			Kfusion[8] = HK52*(-HK28*P(4,8) + HK28*P(8,22) - HK32*P(5,8) + HK32*P(8,23) + HK33*P(6,8) + HK34*P(2,8) + HK35*P(1,8) - HK36*P(3,8) + HK37*P(0,8));
			Kfusion[9] = HK52*(-HK28*P(4,9) + HK28*P(9,22) - HK32*P(5,9) + HK32*P(9,23) + HK33*P(6,9) + HK34*P(2,9) + HK35*P(1,9) - HK36*P(3,9) + HK37*P(0,9));
			Kfusion[10] = HK52*(HK28*P(10,22) - HK28*P(4,10) + HK32*P(10,23) - HK32*P(5,10) + HK33*P(6,10) + HK34*P(2,10) + HK35*P(1,10) - HK36*P(3,10) + HK37*P(0,10));
			Kfusion[11] = HK52*(HK28*P(11,22) - HK28*P(4,11) + HK32*P(11,23) - HK32*P(5,11) + HK33*P(6,11) + HK34*P(2,11) + HK35*P(1,11) - HK36*P(3,11) + HK37*P(0,11));
			Kfusion[12] = HK52*(HK28*P(12,22) - HK28*P(4,12) + HK32*P(12,23) - HK32*P(5,12) + HK33*P(6,12) + HK34*P(2,12) + HK35*P(1,12) - HK36*P(3,12) + HK37*P(0,12));
			Kfusion[13] = HK52*(HK28*P(13,22) - HK28*P(4,13) + HK32*P(13,23) - HK32*P(5,13) + HK33*P(6,13) + HK34*P(2,13) + HK35*P(1,13) - HK36*P(3,13) + HK37*P(0,13));
			Kfusion[14] = HK52*(HK28*P(14,22) - HK28*P(4,14) + HK32*P(14,23) - HK32*P(5,14) + HK33*P(6,14) + HK34*P(2,14) + HK35*P(1,14) - HK36*P(3,14) + HK37*P(0,14));
			Kfusion[15] = HK52*(HK28*P(15,22) - HK28*P(4,15) + HK32*P(15,23) - HK32*P(5,15) + HK33*P(6,15) + HK34*P(2,15) + HK35*P(1,15) - HK36*P(3,15) + HK37*P(0,15));

			// Only update the magnetometer states if we are airborne and using 3D mag fusion
			if (_control_status.flags.mag_3D && _control_status.flags.in_air) {
				Kfusion[16] = HK52*(HK28*P(16,22) - HK28*P(4,16) + HK32*P(16,23) - HK32*P(5,16) + HK33*P(6,16) + HK34*P(2,16) + HK35*P(1,16) - HK36*P(3,16) + HK37*P(0,16));
				Kfusion[17] = HK52*(HK28*P(17,22) - HK28*P(4,17) + HK32*P(17,23) - HK32*P(5,17) + HK33*P(6,17) + HK34*P(2,17) + HK35*P(1,17) - HK36*P(3,17) + HK37*P(0,17));
				Kfusion[18] = HK52*(HK28*P(18,22) - HK28*P(4,18) + HK32*P(18,23) - HK32*P(5,18) + HK33*P(6,18) + HK34*P(2,18) + HK35*P(1,18) - HK36*P(3,18) + HK37*P(0,18));
				Kfusion[19] = HK52*(HK28*P(19,22) - HK28*P(4,19) + HK32*P(19,23) - HK32*P(5,19) + HK33*P(6,19) + HK34*P(2,19) + HK35*P(1,19) - HK36*P(3,19) + HK37*P(0,19));
				Kfusion[20] = HK52*(HK28*P(20,22) - HK28*P(4,20) + HK32*P(20,23) - HK32*P(5,20) + HK33*P(6,20) + HK34*P(2,20) + HK35*P(1,20) - HK36*P(3,20) + HK37*P(0,20));
				Kfusion[21] = HK52*(HK28*P(21,22) - HK28*P(4,21) + HK32*P(21,23) - HK32*P(5,21) + HK33*P(6,21) + HK34*P(2,21) + HK35*P(1,21) - HK36*P(3,21) + HK37*P(0,21));

			} else {
				for (int i = 16; i <= 21; i++) {
					Kfusion[i] = 0.0f;

			}
		}

		Kfusion[22] = HK45*HK52;
		Kfusion[23] = HK42*HK52;

		// synthetic sideslip measurement sample has passed check so record it
		_time_last_beta_fuse = _time_last_imu;

		// apply covariance correction via P_new = (I -K*H)*P
		// first calculate expression for KHP
		// then calculate P - KHP
		SquareMatrix24f KHP;
		float KH[9];

		for (unsigned row = 0; row < _k_num_states; row++) {

			for (unsigned index = 0; index < 9; index++) {
				KH[index] = Kfusion[row] * Hfusion[index];
			}

			for (unsigned column = 0; column < _k_num_states; column++) {
				float tmp = KH[0] * P(0,column);
				tmp += KH[1] * P(1,column);
				tmp += KH[2] * P(2,column);
				tmp += KH[3] * P(3,column);
				tmp += KH[4] * P(4,column);
				tmp += KH[5] * P(5,column);
				tmp += KH[6] * P(6,column);
				tmp += KH[7] * P(22,column);
				tmp += KH[8] * P(23,column);
				KHP(row,column) = tmp;
			}
		}

		// if the covariance correction will result in a negative variance, then
		// the covariance matrix is unhealthy and must be corrected
		bool healthy = true;
		_fault_status.flags.bad_sideslip = false;

		for (int i = 0; i < _k_num_states; i++) {
			if (P(i,i) < KHP(i,i)) {
				P.uncorrelateCovarianceSetVariance<1>(i, 0.0f);

				healthy = false;

				_fault_status.flags.bad_sideslip = true;
			}
		}

		if (healthy) {
			// apply the covariance corrections
			P -= KHP;

			fixCovarianceErrors(true);

			// apply the state corrections
			fuse(Kfusion, _beta_innov);
		}
	}
}
