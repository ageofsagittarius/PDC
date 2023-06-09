/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Matrix multiplication with vector:
		Time is calculated using clock() function
	
	Method Used: Sequential
		Number of threads used : 2, 4, 8, 16
*/

#include <bits/stdc++.h>
using namespace std;

int main(){

    // starting time
    clock_t begin = clock();

    ifstream fileinput("inputfile.mtx");
    int rows, columns, lines;

    while(fileinput.peek() == '%')
        fileinput.ignore(2048, '\n');

    // reading first line that stores num of rows, num of columns and num of lines
    fileinput>>rows>>columns>>lines;

    // Create your matrix:
    double mat[rows][columns];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            mat[i][j] = 0.0;
        }
    }

    // reading data from lines
    int count = 0;
    for (int i = 0; i < lines; i++)
    {
        int r, c; double value;
        fileinput>>r>>c>>value;
        mat[r - 1][c - 1] = value;
        count++;
    }

    fileinput.close();

    // for (int r = 0; r < rows; r++){
    //     for(int c = 0; c < columns; c++){
    //         // cout << mat[m + n * columns]<< ' ';
    //         cout<<mat[r][c]<<" ";
    //     }
    //     cout<<endl;
    // }

    ifstream file("vector.txt");
    string data = "";
    int vec[columns];
    
    int idx = 0;

    while(getline(file, data, ','))
    {
        int value = stoi(data);
        vec[idx] = value;
        idx++;
    }

    file.close();

    printf("The input vector is: \n");
    for(int i = 0; i < columns; i++)
    {
        cout<<vec[i]<<", ";
    }
    cout<<endl<<endl;

    int nonZero = 0;

    double A[count];
    int IA[rows];
    int JA[count];
    int p = 0, q = 0, r = 0;
 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mat[i][j] != 0) {
                A[p] = mat[i][j];
                p++;
                
                JA[r] = j;
                r++;

                nonZero++;
            }
        }
        IA[q] = nonZero;
        q++;
    }

    printf("The CSR format : \n");
    printf("The element array is: \n");
    for(int i = 0; i < count; i++){
        cout<<A[i]<<", ";
    }
    cout<<endl<<endl;
    
    printf("The column array is: \n");
    for(int i = 0; i < count; i++){
        cout<<JA[i]<<", ";
    }
    cout<<endl<<endl;
    
    printf("The non-zero array is: \n");
    for(int i = 0; i < count; i++){
        cout<<IA[i]<<", ";
    }
    cout<<endl<<endl;

    double ans[rows];
    for(int i = 0; i < rows; i++){
        ans[i] = 0.0;
    }

    for(int i = 0; i < count; i++)
    {
      ans[JA[i]] = vec[JA[i]] * mat[JA[i]][JA[i]];
    }

    for(int i = 0; i < rows; i++)
    {
        cout<<ans[i]<<" ";
    }

    // ending time
  	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to multiply using sequential  :%.6f Seconds\n",time_spent);
}

/*
The input vector is: 
4015, -1727, 57, 3293, 2250, -2527, -4667, 4109, 2092, -2470, -443, -4043, 2828, -4226, -1498, 697, 1021, -4428, 706, 802, -2293, -395, 2031, 3338, 1790, -4845, 3396, -4888, 1214, 3746, 1209, 3438, -4637, 1189, -3016, 918, 3512, 2615, 4777, -4831, -2960, 2412, -2489, 4508, 3347, -2821, -1263, 139, -3746, 3865, 4656, 1250, 3295, -3668, -2654, 4148, -2451, 1878, 1080, 844, 272, -2, 3223, -923, 397, 1194, -3108, -1062, 19, 3374, 125, -1286, 4506, 1048, -4791, -215, 1605, 3175, 194, 670, 4430, -3938, -3795, -2096, 1231, -2480, 1354, -2309, -1664, -1636, -3755, 660, -960, 1638, -1615, -1132, -4230, 3065, 2782, 1545, -784, 3371, 1110, -3366, 3682, 1602, -4006, 4297, 924, 2833, -1629, -4837, 3255, 4400, 787, 1386, -224, 4588, 2782, 1545, -784, 3371, 1110, -3366, 3682, 1602, -4006, 4297, 924, 2833, -1629, -4837, 3255, 4400, 787, 1386, -224, 4588,

The CSR format :
The element array is:
0.000188273, 0.000188273, 1.80732e-005, 0.000383364, 0.000112112, 0.000592835, 0.00145678, 0.000582232, 0.00151317, 0.00060399, 0.00148348, 0.000397742, 0.000133803, 0.000177026, 1.0345e-005, 0.00065641, 0.00065641, 0.00310442, 0.000672693, 0.000672693, 0.00331513, 0.000177026, 1.0345e-005, 0.000401814, 0.000136186, 0.000605967, 0.00153098, 0.000582232, 0.00151317, 0.000594812, 0.00150427, 0.000383364, 0.000112112, 0.000188273, 0.000188273, 1.80732e-005, 0.000577098, 0.000577098, 0.00290332, 0.000563128, 0.000563128, 0.00266326, 0.00120028, 0.00120028, 0.00795417, 0.00159036, 0.00159036, 0.00973767, 0.00129136, 0.00129136, 0.00973767, 0.00144096, 0.00144096, 0.00973767, 0.00144096, 0.00144096, 0.00973767, 0.00129136, 0.00129136, 0.00973767, 0.00159036, 0.00159036, 0.00973767, 0.00121249, 0.00121249, 0.00816249, 0.000219277, 0.000219277, 0.00112498, 0.000218178, 0.000218178, 0.00118515, 0.000218178, 0.000218178, 0.00118515, 0.000218178, 0.000218178, 0.00118515, 0.000218178, 0.000218178, 0.00118515, 0.000218178, 0.000218178, 0.00118515, 0.000218178, 0.000218178, 0.00118515, 0.000221264, 0.000221264, 0.00116783, 0.0010297, 0.0010297, 0.00682381, 0.00140684, 0.00140684, 0.00835385, 0.00110784, 0.00110784, 
0.00835385, 0.00125744, 0.00125744, 0.00835385, 0.00125744, 0.00125744, 0.00835385, 0.00110784, 0.00110784, 0.00835385, 0.00140684, 0.00140684, 0.00835385, 0.00104367, 0.00104367, 0.00706387, 0.000219276, 0.000219276, 0.00112498, 0.000218176, 0.000218176, 0.00118515, 0.000218176, 0.000218176, 0.00118515, 0.000218176, 0.000218176, 0.00118515, 0.000218176, 0.000218176, 0.00118515, 0.000218176, 0.000218176, 0.00118515, 0.000218176, 0.000218176, 0.00118515, 0.000221262, 0.000221262, 0.00116783,

The column array is:
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 
58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 
133, 134, 135, 136, 137,

The non-zero array is:
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,

0.755918 -0.325148 0.00103017 1.26242 0.252252 -1.49809 -6.79878 2.39239 3.16555 -1.49186 -0.657184 -1.60807 0.378394 -0.748113 -0.0154968 0.457518 0.670194 -13.7464 0.474922 0.5395 -7.60159 -0.0699254 0.0210106 1.34126 0.243773 -2.93591 5.1992 -2.84595 1.83699 2.22816 1.81867 1.31801 -0.519863 0.223857 -0.567833 0.0165912 2.02677 
1.50911 13.8692 -2.72047 -1.66686 6.42378 -2.98748 5.41084 26.6226 -4.4864 -2.00862 1.35354 -4.83743 4.9911 45.3386 1.8012 4.74796 -35.7178 -3.82431 5.9771 -23.867 2.42517 1.39467 8.21859 0.432578 -0.00318072 31.3845 -1.11912 0.481357 9.74601 -0.681514 -0.232873 0.0213747 0.736132 0.0272722 -1.5241 0.983109 0.22865 -5.67805 -0.0469082 0.350175 3.76285 0.0423265 0.146179 5.25021 -0.859184 -0.827984 -2.48407 0.268577 -0.541081 1.60469 -0.510899 -0.368184 -1.91057 -3.86654 0.679605 -6.55086 2.30441 -2.27205 -9.45656 -4.68618 3.39554 23.2404 1.94275 -0.985837 28.1608 1.39576 -4.23256 30.7589 1.77477 -4.43803 35.8965 1.29992 3.98559 -13.6084 -5.04825 3.39716 31.081 0.17257 0.303916 -0.251996 1.00099 0.606965 1.83105 -0.17105 0.735471 1.31551 -0.73438 0.803324 1.89861 -0.874013 0.937502 1.09508 0.618092 -0.355409 -5.73256 0.710163 0.959974 0.932712 0.30667 -0.0495628 5.35799       
Time taken to multiply using sequential  :0.139000 Seconds

*/
