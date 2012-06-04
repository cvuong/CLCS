#include <algorithm>
#include <iostream>
#include <string>

#define MAX_SIZE 2048

using namespace std;

int arr[2*MAX_SIZE][MAX_SIZE];							// DP table

int m, n;
int lower_bounds[MAX_SIZE][MAX_SIZE];			// bounds
int upper_bounds[MAX_SIZE][MAX_SIZE];
int path_lengths[MAX_SIZE];								// path lengths

string A, B, C, doubleA, perm;						// copies of the strings


/* 
 * DEBUGGING CODE
 *
 */

void printDPTable(string first, string second) {
	cout << "   ";
	for (int i = 0; i < n; i++) {
		cout << " " << second.at(i);
	}
	cout << endl;
	for (int i = 0; i <= 2*m; i++) {
		if (i == 0) {
			cout << "  ";
		} else {
			cout << first.at(i-1) << " ";
		}
		for (int j = 0; j <= n; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

void printBounds(int path) {
	
	//cout << "length: " << path_lengths[path] << endl;
	cout << "path: " << path << endl;
	cout << "lower bound path: ";
	for (int i = 0; i <= m; i++) {
		cout << lower_bounds[path][i] << ' ';
	}
	cout << endl;
	
	
	cout << "upper bound path: ";
	for (int i = 0; i <= m; i++) {
		cout << upper_bounds[path][i] << ' ';
	}
	cout << endl;
	
	return;
}


/* 
 * WORK CODE
 *
 */

void clearTable() {
	for (int i = 0; i <= 2*m; i++) {
		for (int j =0; j <= n; j++) {
			arr[i][j] = 0;
		}
	}
}

void initDP() {
  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
			arr[i+m][j] = 0;
      if (A[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return;
}

bool canUp(int i, int j, int mid, int low, int up) {
	if (i <= mid) return false;
	if ((i <= low + m) && (j > lower_bounds[low][i - low - 1])) return false;
	return true;
}

bool canLeft(int i, int j, int mid, int low, int up) {
	if (i >= up && (j-1 < upper_bounds[up][i - up])) return false;
	return true;
}

bool canDiag(int i, int j, int mid, int low, int up) {
	return canUp(i, j-1, mid, low, up);
}

void fillDP(int mid, int low, int up) {
  int i, j;
	int start_col, end_col;

	// valid rows to look in
  for (i = mid; i <= mid + m; i++) {	 
		
		// set start col using upper bounds if needed
		if (i < up) {
			start_col = 1;
		} else {
			start_col = max(upper_bounds[up][i - up], 1);
		}
		
		// set end col using lower bounds if needed
		if (i > low + m) {
			end_col = n;
		} else {
			end_col = min(lower_bounds[low][i - low], n);
		}

		// loop over cols
    for (j = start_col; j <= end_col; j++) {
      
			bool can_left = canLeft(i,j,mid,low,up);
			bool can_up = canUp(i,j,mid,low,up);
			bool can_diag = canDiag(i,j,mid,low,up);
			
			if (can_diag && doubleA[i-1] == B[j-1]) {
				// take from diag
				arr[i][j] = arr[i-1][j-1]+1;
				
			} else if (can_up && can_left) {
				// max up and left
				arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
				
			} else if (can_diag && can_left && !can_up) {
				// max left and diag
				arr[i][j] = max(arr[i][j-1], arr[i-1][j-1]);
				
			} else if (can_diag && can_up && !can_left) {
				// max up and diag
				arr[i][j] = max(arr[i-1][j], arr[i-1][j-1]);
				
			} else if (can_left && !can_up && !can_diag){
				arr[i][j] = arr[i][j-1];

			}
    }
  }
  
  return;
}


void traceBack(int mid, int low, int up) {

	// do we need to respect the bounds?
	bool bounded = (low != -1);
	
	// init end of path to n
	lower_bounds[mid][m] = n;
	upper_bounds[mid][m] = n;
	
	int length = 0;								// track path length
	int row = mid + m, col = n;		// track row and col
	int path_row = row - mid;			// track corresponding index into path
	while (true) {

		// save upper bounds (same as saving on exiting a row)
		upper_bounds[mid][path_row] = col;
			
		if (row == mid && col == 0) {										// we reached the upper left, end
			break;
		}
		
		length++;
		
		if (row == mid) {																// left on top edge
			col--;
			continue;
		} else if (col == 0) {													// up on left edge
			row--; path_row--;
			lower_bounds[mid][path_row] = col;
			continue;
		}
		
		if (doubleA.at(row-1) == B.at(col-1)) {					// diag
			row--; col--; path_row--;
			lower_bounds[mid][path_row] = col;
			
			// no up? go left
		} else if (bounded && (row <= low + m) && (col > lower_bounds[low][row - low - 1])) {
			col--;

			// no left? go up
		} else if (bounded && (row >= up) && (col-1 < upper_bounds[up][row - up])) {
			row--; path_row--;
			lower_bounds[mid][path_row] = col;

		} else if(arr[row-1][col] > arr[row][col-1]) {	// up
			row--; path_row--;
			lower_bounds[mid][path_row] = col;
		} else {																				// left
			col--;
		}
	}
	
	// save path length and return
	path_lengths[mid] = length;
	return;
}


void findShortestPaths(int low, int up) {
	if ((up - low) <= 1) return;
	int mid = (low + up) / 2;

	// compute the DP table with bounds
	fillDP(mid, low, up);
	
	//trace p_mid
	traceBack(mid, low, up);

	// recurse
	findShortestPaths(low, mid);
	findShortestPaths(mid, up);
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
		 
		// make doubled copy
		doubleA = A + A;
		
		// create initial DP table and compute p_0 and p_m
		initDP();
		traceBack(0, -1, -1);
		
		// shift paths downs
		for (int i = 0; i <= m; i++) {
			upper_bounds[m][i] = upper_bounds[0][i];
			lower_bounds[m][i] = lower_bounds[0][i];
		}
		path_lengths[m] = path_lengths[0];

		// recurse with bounds
		findShortestPaths(0, m);

		// select shortest path
		int min_path, min_length = n + m;
		for (int i = 0; i <= m; i++) {
			if (path_lengths[i] < min_length) {
				min_path = i;
				min_length = path_lengths[i];
			}
		}
		
		// print clcs length
		cout << (m+n) - path_lengths[min_path] << endl;
		//cout << min_path << endl;

  }
  return 0;
}
