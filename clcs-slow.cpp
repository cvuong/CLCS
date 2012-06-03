#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int m, n;
int arr[2048][2048];
string A, B, C;

/*
void traceBack(int start_row) {
	lower_bounds[start_row][m] = n;
	upper_bounds[start_row][m] = n;
	
	int length = 0;
	//cout << "traceback " << start_row << endl;
	
	//find lower bound
	int row = start_row + m, col = n;
	while (true) {
		
		//cout << "row: " << row << " col: " << col << endl;
		upper_bounds[start_row][row] = col;
		
		if (row == start_row && col == 0) {
			break;
		}
		
		length++;
		
		if (row == start_row) {	// move left
			col--;
			continue;
		} else if (col == 0) {	// move up
			row--;
			lower_bounds[start_row][row] = col;
			continue;
		}
		
		if (doubleA.at(row-1) == B.at(col-1)) {	
			row--; col--;
			lower_bounds[start_row][row] = col;
		} else if(arr[row-1][col] <= arr[row][col-1]) {	
			col--;
		} else {
			//cout << "up" << endl;
			row--;
			lower_bounds[start_row][row] = col;
		}
	}
	path_lengths[start_row] = length;
	return;
}
*/

void printDPTable() {
	cout << "   ";
	for (int i = 0; i < n; i++) {
		cout << " " << B.at(i);
	}
	cout << endl;
	for (int i = 0; i <= m; i++) {
		if (i == 0) {
			cout << "  ";
		} else {
			cout << A.at(i-1) << " ";
		}
		for (int j = 0; j <= n; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

int LCS() {
  //int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
      if (A[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return arr[m][n];
}

int main() {
  int T;
  cin >> T;
  for (int tc = 0; tc < T; tc++) {
    cin >> A >> B;
		m = A.length(), n = B.length();
		
		if (n < m) {
			C = A;
			A = B;
			B = C;
		}
		
		C = A;
		int max = 0, max_cut = 0;
		for (int k = 0; k < m; k++) {
			A = C.substr(k) + C.substr(0,k);
			int result = LCS();
			if (result > max) {
				max = result;
				max_cut = k;
			}
			//cout << result << endl;
		}
		
		A = C.substr(max_cut) + C.substr(0,max_cut);
		int result = LCS();
		//printDPTable();
		
    cout << max << endl;
		cout << max_cut << endl;
  }
  return 0;
}
