#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

/*
 logA method seems to have an interesting error behavior,
 while weighted amplitude method is too accurate to have a reason for it.
*/

void logA() {
	int startpoint = 300;
	int endpoint = 1000;
	int numofpads = 5;
	int numofpoints = endpoint - startpoint;
	double size = 100.;
	double gap = 75.;
	double pitch = size + gap;
	double length = 1300.;
	vector<vector<double>> lookup(numofpoints, vector<double>(numofpads));
	vector<double> d(numofpads);
	vector<double> pad_center_pos(numofpads);
	vector<double> pad_left_pos(numofpads);
	vector<double> pad_right_pos(numofpads);
	vector<double> alpha(numofpads);
	vector<double> term(numofpads);

	ofstream output_test_logA;
	ofstream output_train_logA;
	ofstream output_train_logA_gap;
	ofstream output_train_logA_pad;
	output_test_logA.open("ypred_vs_yreal_test_logA.txt", ios::out);
	output_train_logA.open("ypred_vs_yreal_train_logA.txt", ios::out);

// generate lookup table

	for (int i = 0; i < pad_center_pos.size(); ++i) {
		pad_center_pos[i] = 300. + 175. * i;
		pad_left_pos[i] = pad_center_pos[i] - size / 2;
		pad_right_pos[i] = pad_center_pos[i] + size / 2;
	}
	for (int i = 0; i < lookup.size(); ++i) {
		for (int j = 0; j < numofpads; ++j) {
			double position = (double)i + startpoint;
//			double position = (double)i + startpoint + 0.01;
			double sum = 0.;
			for (int k = 0; k < numofpads; ++k) {
				double d1 = abs(position - pad_left_pos[k]);
				double d2 = abs(position - pad_right_pos[k]);
				d[k] = min(d1, d2);
				double edge = 8.00;
				double lateral = 4.00;
/*				if (position > pad_left_pos[k] - edge && position < pad_right_pos[k] + edge) {
					d[k] = edge;
				}*/
//			alpha[k] = 2 * atan((length / 2) / d[k]);
				alpha[k] = length / d[k];
				term[k] = alpha[k] / log(d[k] / lateral);
				sum += term[k];
			}
			double relativeA = term[j] / sum;
			lookup[i][j] = relativeA;
			}
	}

// print lookup table

	cout << "this is lookup table: " << "\n";
	for (int i = 0; i < lookup.size(); ++i) {
		for (int j = 0; j < numofpads; ++j) {
			cout << lookup[i][j] << "  ";
			if (j % 10 == 4) cout << i + startpoint << "\n";
		}
	}

// read data from file

	int numofdata = 550;
	int numoftestdata = 110;
	int timebin = 100;
	vector<vector<double>> waveform(numofpads, vector<double>(timebin));
	vector<vector<double>> waveform_test(numofpads, vector<double>(timebin));
	vector<vector<double>> rA(numofdata, vector<double>(numofpads, 0));
	vector<vector<double>> rA_test(numoftestdata, vector<double>(numofpads, 0));
	vector<double> yreal(numofdata);
	vector<double> yreal_test(numoftestdata);
	ifstream datafile("../data/signal_train_550.txt");
//	ifstream testdatafile("../data/signal_test_110.txt");
	ifstream testdatafile("../data/signal_np_2e13_test_110.txt");
	ifstream positionfile("../data/yreal_train_550.txt");
//	ifstream testpositionfile("../data/yreal_test_110.txt");
	ifstream testpositionfile("../data/yreal_np_2e13_test_110.txt");
	for (int i = 0; i < numofdata; ++i) {
		positionfile >> yreal[i];
		yreal[i] = 10000 * yreal[i];
		for (int j = 0; j < timebin; ++j) {
			datafile >> waveform[0][j] >> waveform[1][j] >> waveform[2][j] >> waveform[3][j] >> waveform[4][j];
			for (int k = 0; k < numofpads; ++k) {
				if (abs(waveform[k][j]) > rA[i][k]) {
					rA[i][k] = abs(waveform[k][j]);
				}
			}
		}
	}
	for (int i = 0; i < numoftestdata; ++i) {
		testpositionfile >> yreal_test[i];
		yreal_test[i] = 10000 * yreal_test[i];
		for (int j = 0; j < timebin; ++j) {
			testdatafile >> waveform_test[0][j] >> waveform_test[1][j] >> waveform_test[2][j] >> waveform_test[3][j] >> waveform_test[4][j];
			for (int k = 0; k < numofpads; ++k) {
				if (abs(waveform_test[k][j]) > rA_test[i][k]) {
					rA_test[i][k] = abs(waveform_test[k][j]);
				}
			}
		}
	}

// recompute rA

	for (int i = 0; i < numofdata; ++i) {
		double sum = 0.;
		for (int j = 0; j < numofpads; ++j) {
			sum += rA[i][j];
		}
		for (int k = 0; k < numofpads; ++k) {
			rA[i][k] = rA[i][k] / sum;
		}
	}
	for (int i = 0; i < numoftestdata; ++i) {
		double sum = 0.;
		for (int j = 0; j < numofpads; ++j) {
			sum += rA_test[i][j];
		}
		for (int k = 0; k < numofpads; ++k) {
			rA_test[i][k] = rA_test[i][k] / sum;
		}
	}

// print rA

/*	cout << "this is rA from simulation: " << "\n";
	for (int i = 0; i < numofdata; ++i) {
		for (int j = 0; j < numofpads; ++j) {
			cout << rA[i][j] << "  ";
			if (j % 10 == 4) cout << yreal[i] << "\n";
		}
	}
*/
	cout << "this is rA_test from simulation: " << "\n";
	for (int i = 0; i < numoftestdata; ++i) {
		for (int j = 0; j < numofpads; ++j) {
			cout << rA_test[i][j] << "  ";
			if (j % 10 == 4) cout << yreal_test[i] << "\n";
		}
	}

// predict the position by lookup table

	vector<double> ypred(numofdata);
	vector<double> ypred_test(numoftestdata);
	for (int i = 0; i < numofdata; ++i) {
		int index = -1;
		double min = 999.;
		for (int j = 0; j < numofpoints; ++j) {
			double loss = 0.;
			for (int k = 0; k < numofpads; ++k) {
				loss += pow(rA[i][k]-lookup[j][k], 2);
			}
			if (loss < min) {
				min = loss;
				index = j;
			}
		}
		ypred[i] = (double)index + 300.;
	}
	for (int i = 0; i < numoftestdata; ++i) {
		int index = -1;
		double min = 999.;
		for (int j = 0; j < numofpoints; ++j) {
			double loss = 0.;
			for (int k = 0; k < numofpads; ++k) {
				loss += pow(rA_test[i][k]-lookup[j][k], 2);
			}
			if (loss < min) {
				min = loss;
				index = j;
			}
		}
		ypred_test[i] = (double)index + 300.;
	}

// print ypred and yreal

	double error = 0;
	double variation = 0;
	double error_gap = 0;
	double error_pad = 0;
	double variation_gap = 0;
	double variation_pad = 0;
	int count_gap = 0;
	int count_pad = 0;

	double test_error = 0;
	double test_variation = 0;
	double test_error_gap = 0;
	double test_error_pad = 0;
	double test_variation_gap = 0;
	double test_variation_pad = 0;
	int test_count_gap = 0;
	int test_count_pad = 0;

	for (int i = 0; i < numofdata; ++i) {
	//	cout << "ypred: " << ypred[i] << "| yreal: " << yreal[i] << "\n";
		output_train_logA << ypred[i] << "  " << yreal[i] << "\n";
		double var = pow(ypred[i] - yreal[i], 2);
		variation += var;
		int flag = 0;
		for (int j = 0; j < numofpads; ++j) {
			if (yreal[i] > pad_left_pos[j] - 1 && yreal[i] < pad_right_pos[j] + 1) {
				flag = 1;
			//	cout << yreal[i] << " is between " << pad_left_pos[j] << " and " << pad_right_pos[j] << "\n";
			}
		}
	//	cout << "flag: " << flag << "\n";
		if (flag == 0) {
			variation_gap += var;
			++count_gap;
		} else {
			variation_pad += var;
			++count_pad;
		}
	}

	for (int i = 0; i < numoftestdata; ++i) {
//		cout << "ypred_test: " << ypred_test[i] << "| yreal_test: " << yreal_test[i] << "\n";
		output_test_logA << ypred_test[i] << "  " << yreal_test[i] << "\n";
		double var = pow(ypred_test[i] - yreal_test[i], 2);
		test_variation += var;
		int flag = 0;
		for (int j = 0; j < numofpads; ++j) {
			if (yreal_test[i] > pad_left_pos[j] - 1 && yreal_test[i] < pad_right_pos[j] + 1) {
				flag = 1;
			//	cout << yreal[i] << " is between " << pad_left_pos[j] << " and " << pad_right_pos[j] << "\n";
			}
		}
	//	cout << "flag: " << flag << "\n";
		if (flag == 0) {
			test_variation_gap += var;
			++test_count_gap;
			cout << "gap region ypred_test: " << ypred_test[i] << "| yreal_test: " << yreal_test[i] << "\n";
		} else {
			test_variation_pad += var;
			++test_count_pad;
			cout << "pad region ypred_test: " << ypred_test[i] << "| yreal_test: " << yreal_test[i] << "\n";
		}
	}

	output_train_logA.close();
	output_test_logA.close();

	error = sqrt(variation / numofdata);
	error_gap = sqrt(variation_gap / count_gap);
	error_pad = sqrt(variation_pad / count_pad);
	cout << "train: all " << numofdata << " region sigma is: " << error << "\n";
	cout << "train: " << count_gap << " gap region sigma is: " << error_gap << "\n";
	cout << "train: " << count_pad << " pad region sigma is: " << error_pad << "\n";
	test_error = sqrt(test_variation / numoftestdata);
	test_error_gap = sqrt(test_variation_gap / test_count_gap);
	test_error_pad = sqrt(test_variation_pad / test_count_pad);
	cout << "test: all " << numoftestdata << "  region sigma is: " << test_error << "\n";
	cout << "test: " << test_count_gap << " gap region sigma is: " << test_error_gap << "\n";
	cout << "test: " << test_count_pad << " pad region sigma is: " << test_error_pad << "\n";

// use naive weighted amplitude to predict position

	double variation_w = 0.;
	double variation_pad_w = 0.;
	double variation_gap_w = 0.;
	double variation_test_w = 0.;
	double variation_pad_test_w = 0.;
	double variation_gap_test_w = 0.;
	int count_pad_w = 0;
	int count_gap_w = 0;
	int count_pad_test_w = 0;
	int count_gap_test_w = 0;

	ofstream output_test_w;
	ofstream output_train_w;
	output_test_w.open("ypred_vs_yreal_test_w.txt", ios::out);
	output_train_w.open("ypred_vs_yreal_train_w.txt", ios::out);

	for (int i = 0; i < numofdata; ++i) {
		int flag = 0;
		double ypred_w = 0.;
		for (int j = 0; j < numofpads; ++j) {
			ypred_w += rA[i][j] * pad_center_pos[j];
			if (yreal[i] > pad_left_pos[j] && yreal[i] < pad_right_pos[j]) {
				flag = 1;
			}
		}
		double var = pow(ypred_w - yreal[i], 2);
		variation_w += var;
		if (flag == 0) {
			count_gap_w++;
			variation_gap_w += var;
		} else {
			count_pad_w++;
			variation_pad_w += var;
		}
//		cout << "train: weighted position: " << ypred_w << "| yreal is: " << yreal[i] << "\n";
		output_train_w << ypred_w << "  " << yreal[i] << "\n";
	}
	for (int i = 0; i < numoftestdata; ++i) {
		int flag = 0;
		double ypred_test_w = 0.;
		for (int j = 0; j < numofpads; ++j) {
			ypred_test_w += rA_test[i][j] * pad_center_pos[j];
			if (yreal_test[i] > pad_left_pos[j] && yreal_test[i] < pad_right_pos[j]) {
				flag = 1;
			}
		}
		double var = pow(ypred_test_w - yreal_test[i], 2);
		variation_test_w += var;
		if (flag == 0) {
			count_gap_test_w++;
			variation_gap_test_w += var;
		} else {
			count_pad_test_w++;
			variation_pad_test_w += var;
		}
//		cout << "test: weighted position: " << ypred_test_w << "| yreal is: " << yreal_test[i] << "\n";
		output_test_w << ypred_test_w << "  " << yreal_test[i] << "\n";
	}
	output_test_w.close();
	output_train_w.close();

	double error_w = sqrt(variation_w / numofdata);
	double error_pad_w = sqrt(variation_pad_w / count_pad_w);
	double error_gap_w = sqrt(variation_gap_w / count_gap_w);
	double error_test_w = sqrt(variation_test_w / numoftestdata);
	double error_pad_test_w = sqrt(variation_pad_test_w / count_pad_test_w);
	double error_gap_test_w = sqrt(variation_gap_test_w / count_gap_test_w);
	
	cout << "train: weighted method sigma: " << error_w << "\n";
	cout << "train: weighted method gap sigma: " << error_gap_w << "\n";
	cout << "train: weighted method pad sigma: " << error_pad_w << "\n";
	cout << "test: weighted method sigma: " << error_test_w << "\n";
	cout << "test: weighted method gap sigma: " << error_gap_test_w << "\n";
	cout << "test: weighted method pad sigma: " << error_pad_test_w << "\n";

}
