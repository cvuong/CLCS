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

#define DOWN 'a'
#define RIGHT 'b'
#define DIAG 'c'

string A, B, C;

int LCS() {
  m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
			
			if (arr[i-1][j] > arr[i][j-1]) {	// down
				arr[i][j] = arr[i-1][j];
				
			} else if (arr[i-1][j] < arr[i][j-1]) {	// right
				arr[i][j] = arr[i][j-1];
			}
      if (A[i-1] == B[j-1]) {	// diag
				arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
			}
    }
  }
  
  return arr[m][n];
}


void initBounds() {
	upper_bounds[0][n-1] = m-1;
	lower_bounds[0][n-1] = m-1;
	
	//find upper bound
	int row = m-1, col = n-1;
	while (col > 0) {
		if (A.at(row) == B.at(col)) {
			upper_bounds[0][col] = row;
			row--; col--;
		} else if(arr[row-1][col] == arr[row][col-1]) {	// moving left
			upper_bounds[0][col] = row;
			col--;
		} else {	// have to move up
			row--;
		}
	}

	// find lower bound
	row = m-1, col = n-1;
	while (col > 0) {
		if (A.at(row) == B.at(col)) {
			row--; col--;
			lower_bounds[0][col] = row;
		} else if(arr[row-1][col] < arr[row][col-1]) {	// can't move up, move left
			col--;
			lower_bounds[0][col] = row;
		} else {	// have to move up
			row--;
		}
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
  }
  return 0;
}
