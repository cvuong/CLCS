#include <algorithm>
#include <iostream>
#include <string>

#define MAX_SIZE 2048

using namespace std;

int arr[MAX_SIZE][MAX_SIZE];
char walk[MAX_SIZE][MAX_SIZE];

int m, n;
int upper_bounds[MAX_SIZE][MAX_SIZE];
int lower_bounds[MAX_SIZE][MAX_SIZE];
int path_lengths[MAX_SIZE];

#define DOWN 'a'
#define RIGHT 'b'
#define DIAG 'c'

string A, B, C, doubleA;

/*
int LCS() {
  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
			
			if (arr[i-1][j] > arr[i][j-1]) {	// down
				arr[i][j] = arr[i-1][j];
				arr[i+m][j] = -1;
			} else if (arr[i-1][j] < arr[i][j-1]) {	// right
				arr[i][j] = arr[i][j-1];
				arr[i+m][j] = -1;
			}
      if (A[i-1] == B[j-1]) {	// diag
				arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
				arr[i+m][j] = -1;
			}
    }
  }
  
  return arr[m][n];
}
*/

int LCS() {
  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
			arr[i+m][j] = -1;
      if (A[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return arr[m][n];
}

void singleShortestPath(int mid, int low, int up) {
	//int m = 2*A.length(), n = B.length();
  int i, j;
	int start_col, end_col;

  for (i = mid; i < up + m; i++) {	// valid rows to look in 
		
		if (i < up) {
			start_col = 1;
		} else {
			start_col = upper_bounds[up][i - up] + 1;
		}
		
		if (i > low + m) {
			end_col = n + 1;
		} else {
			end_col = lower_bounds[low][i - low] + 1;
		}
		
		//cout << " row: " << i << " start: " << start_col << " end: " << end_col << endl;

    for (j = start_col; j < end_col; j++) {
	
			cout << i << " " << j << endl;
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
      if (doubleA[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return;
}

void initBounds() {
	upper_bounds[0][m] = n;
	lower_bounds[0][m] = n;
	
	int length = 0;
	
	//find lower bound
	int row = m, col = n;
	while (true) {
		
		//cout << "row: " << row << " col: " << col << endl;
		lower_bounds[0][row] = col;
		
		if (row == 0 && col == 0) {
			break;
		}
		
		length++;
		
		if (row == 0) {	// move left
			col--;
			continue;
		} else if (col == 0) {	// move up
			row--;
			upper_bounds[0][row] = col;
			continue;
		}
		
		//cout << "letters: " << A.at(row-1) << " " << B.at(col-1) << endl;
		if (A.at(row-1) == B.at(col-1)) {	
			//cout << "diag" << endl;
			row--; col--;
			upper_bounds[0][row] = col;
		} else if(arr[row-1][col] <= arr[row][col-1]) {	
			//cout << "left" << endl;
			//lower_bounds[0][row] = col;
			col--;
		} else {
			//cout << "up" << endl;
			row--;
			upper_bounds[0][row] = col;
		}
	}
	path_lengths[0] = length;
	return;
}

void printBounds(int path) {
	
	cout << "length: " << path_lengths[path] << endl;
	
	cout << "upper bound path: ";
	for (int i = 0; i <= m; i++) {
		cout << upper_bounds[path][i] << ' ';
	}
	cout << endl;
	
	
	cout << "lower bound path: ";
	for (int i = 0; i <= m; i++) {
		cout << lower_bounds[path][i] << ' ';
	}
	cout << endl;
	
	return;
}

void printDPTable() {
	for (int i = 0; i <= 2*m; i++) {
		for (int j = 0; j <= n; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
  int T;
  cin >> T;
  for (int tc = 0; tc < T; tc++) {
    cin >> A >> B;
		m = A.length(), n = B.length();
		
		// fix A as shortest string
		if (n < m) {
			C = A;
			A = B;
			B = C;
		}
		
		doubleA = A + A;
		
		int result = LCS();
		initBounds();
		
		// shift paths downs
		for (int i = 0; i <= m; i++) {
			lower_bounds[m][i] = lower_bounds[0][i];
			upper_bounds[m][i] = upper_bounds[0][i];
		}
		path_lengths[m] = path_lengths[0];
	
		//printBounds(0);
		
		singleShortestPath((m)/2, 0, m);
		
		//printDPTable();
		
		//
		
		/*
		C = A;
		int max = 0;
		for (int k = 0; k < m; k++) {
			A = C.substr(k) + C.substr(0,k);
			int result = LCS();
			if (result > max) max = result;
			//cout << result << endl;
		}
    cout << max << endl;
		//cout << A << endl;
		*/
  }
  return 0;
}
