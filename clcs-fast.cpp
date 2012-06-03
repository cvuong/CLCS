#include <algorithm>
#include <iostream>
#include <string>

#define MAX_SIZE 2048
//#define USEDEBUG 1
#ifdef USEDEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x) 
#endif

using namespace std;

void printDPTable(string first, string second);
void printBounds(int path);

int arr[MAX_SIZE][MAX_SIZE];
char walk[MAX_SIZE][MAX_SIZE];

int m, n;
int lower_bounds[MAX_SIZE][MAX_SIZE];
int upper_bounds[MAX_SIZE][MAX_SIZE];
int path_lengths[MAX_SIZE];

string A, B, C, doubleA, perm;

int LCS() {
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
  
  return arr[m][n];
}

void fillDP(int mid, int low, int up) {
  int i, j;
	int start_col, end_col;

	int shift = 0;//m / 2;
	
	//perm = A.substr(mid) + A.substr(0, mid);
	
  for (i = mid; i <= mid + m; i++) {	// valid rows to look in 
		
		if (i < up + shift) {
			start_col = 1;
		} else {
			start_col = max(upper_bounds[up][i - (up + shift)], 1);
		}
		//start_col = max(upper_bounds[up][i - mid] - 1, 1);
		//end_col = min(lower_bounds[low][i - mid] + 1, n);
		
		if (i > low + m - shift) {
			end_col = n;
		} else {
			end_col = min(lower_bounds[low][i - (low - shift)], n);
		}
		
		//start_col = 0;
		//end_col = n;
		
		//cout << " row: " << i << " start: " << start_col << " end: " << end_col << endl;

    for (j = start_col; j <= end_col; j++) {
			//cout << perm[i-1] << B[j-1] << endl;
			
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
      if (doubleA[i-1] == B[j-1]) {
				//cout << "diag " << doubleA[i-1] << B[j-1] << endl;
				arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
			}
			//cout << i << " " << j << " " << arr[i][j] << endl;
    }
  }
  
  return;
}

void clearTable() {
	for (int i = 0; i <= 2*m; i++) {
		for (int j =0; j <= n; j++) {
			arr[i][j] = 0;
		}
	}
}

void traceBack(int mid) {
	lower_bounds[mid][m] = n;
	upper_bounds[mid][m] = n;
	
	int length = 0;
	//cout << "traceback " << start_row << endl;
	
	int row = mid + m, col = n;
	int path_row = row - mid;
	while (true) {
		//cout << "row: " << row << " col: " << col << endl;
		//printBounds(mid);
		//cout << "lower bound: " << lower_bounds[mid][path_row] << " upper bound: " << upper_bounds[mid][path_row] << endl;
		upper_bounds[mid][path_row] = col;
			
		if (row == mid && col == 0) {
			break;
		}
		
		length++;
		
		if (row == mid) {	// move left
			col--;
			continue;
		} else if (col == 0) {	// move up
			row--; path_row--;
			lower_bounds[mid][path_row] = col;
			continue;
		}
		
		if (doubleA.at(row-1) == B.at(col-1)) {	
			row--; col--; path_row--;
			lower_bounds[mid][path_row] = col;
		} else if(arr[row-1][col] > arr[row][col-1]) {	// up
			row--; path_row--;
			lower_bounds[mid][path_row] = col;
		} else {	// left
			//row--;
			col--;
			
		}
	}
	path_lengths[mid] = length;
	/*
	cout << "path just created: " << endl;
	printBounds(mid);
	cout << " ***** " << endl;
	*/
	return;
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


void findShortestPaths(int low, int up) {
	if ((up - low) <= 1) return;
	int mid = (low + up) / 2;
	perm = A.substr(mid) + A.substr(0, mid);
	Debug("mid: " << mid  << " low: " << low << " up: " << up << " " << perm << endl);
	clearTable();
	//printBounds(low);
	//printBounds(up);
	fillDP(mid, low, up);
	//printDPTable(doubleA, B);
	traceBack(mid);
	
	/*
	cout << "-----------" << endl;
	cout << "lower: " << low << endl;
	printBounds(low);
	cout << "mid: " << mid << endl;
	printBounds(mid);
	cout << "upper: " << up << endl;
	printBounds(up);
	*/
	findShortestPaths(low, mid);
	findShortestPaths(mid, up);
}



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
		//printDPTable(doubleA,B);
		traceBack(0);
		// shift paths downs
		for (int i = 0; i <= m; i++) {
			upper_bounds[m][i] = upper_bounds[0][i];
			lower_bounds[m][i] = lower_bounds[0][i];
		}
		path_lengths[m] = path_lengths[0];
	
		//printBounds(0);
		
		//fillDP(0, m);
		
		findShortestPaths(0, m);

		int min_path, min_length = n + m;
		for (int i = 0; i <= m; i++) {
			if (path_lengths[i] < min_length) {
				min_path = i;
				min_length = path_lengths[i];
			}
		}
		
		//cout << "clcs lengths" << endl;
		for (int i = 0; i <= m; i++) {
			Debug("path: " << i << " length: " << path_lengths[i] << " diags: " << (m+n) - path_lengths[i] << " DP: " << arr[i + m][n] << endl);
			/*
			if (path_lengths[i] == min_length) {
				cout << arr[min_path + m][n] << endl;
			}*/
		}
		//cout << endl;
		//cout << arr[min_path + m][n] << endl;
		cout << (m+n) - path_lengths[min_path] << endl;
		//cout << min_path << endl;

  }
  return 0;
}
