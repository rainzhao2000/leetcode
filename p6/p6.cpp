#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#ifdef DEBUG
#include <iomanip>
#include <thread>
// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#endif
using namespace std;

class Strategy {
 private:
  string name;

 public:
  Strategy(string name) : name{name} {}
  virtual ~Strategy() {}
  virtual string zigzag(const string &s, int numRows) const = 0;
  friend ostream &operator<<(ostream &out, const Strategy &strat);
};

ostream &operator<<(ostream &out, const Strategy &strat) {
  return out << strat.name;
}

class Context {
 private:
  Strategy *strat;

 public:
  Context(Strategy *strat = nullptr) : strat{strat} {}
  ~Context() { delete this->strat; }
  void setStrategy(Strategy *strat) {
    delete this->strat;
    this->strat = strat;
  }
  void testZigzag(const string &s, int numRows) {
    if (!strat) {
      cout << "Strategy not set" << endl;
      return;
    }
    cout << "\nProblem: Given a string s and numRows, return zigzag "
            "conversion.\n";
    cout << "Input (size " << s.size() << "): " << s << endl;
    auto start = chrono::steady_clock::now();
    string soln = this->strat->zigzag(s, numRows);
    auto end = chrono::steady_clock::now();
    cout << "Output: " << soln << endl;
    cout << "Strategy: " << (*this->strat) << endl;
    cout << "Time (ns): ";
    cout << chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << endl;
  }
};

class Naive : public Strategy {
 public:
  Naive(string name = "Naive") : Strategy{name} {}

  // n = s.size(), m = numRows
  // T(n) = Θ(nm)
  string zigzag(const string &s, int numRows) const override {
    if (numRows == 1) {
      return s;
    }
    // initialize 2D matrix of sentinel index values in Θ(nm) time
    int n = s.size();
    int matrix[numRows][n];
    for (int row = 0; row < numRows; ++row) {
      for (int col = 0; col < n; ++col) {
        matrix[row][col] = -1;
      }
    }
    // fill matrix in diagonal fashion in Θ(n) time
    bool onDiag = false;
    for (int i = 0, row = 0, col = 0; i < n; ++i) {
      matrix[row][col] = i;
#ifdef DEBUG
      this_thread::sleep_for(chrono::milliseconds(200));
      for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < n; ++c) {
          if (r == row && c == col) {
            cerr << YELLOW;
          }
          int index = matrix[r][c];
          cerr << setw(3) << (index > -1 ? s.at(index) : '-');
          cerr << RESET;
        }
        cerr << endl;
      }
      cerr << endl;
#endif
      if (row == 0) {
        onDiag = false;
        ++row;
      } else if (row == numRows - 1) {
        onDiag = true;
        --row;
        ++col;
      } else if (onDiag) {
        --row;
        ++col;
      } else {
        ++row;
      }
    }
    // read matrix row by row in Θ(nm) time
    string result = "";
    for (int row = 0; row < numRows; ++row) {
      for (int col = 0; col < n; ++col) {
        int index = matrix[row][col];
        if (index > -1) {
          result += s.at(index);
        }
      }
    }
    return result;
  }
};

class SortByRow : public Strategy {
 public:
  SortByRow(string name = "Sort by Row") : Strategy{name} {}

  // n = s.size()
  // T(n) = Θ(n)
  string zigzag(const string &s, int numRows) const override {
    if (numRows == 1) {
      return s;
    }
    int n = s.size();
    size_t minRows = n > numRows ? numRows : n;
    vector<string> rows{minRows};
    bool onDiag = false;
    for (int i = 0, row = 0; i < n; ++i) {
      rows.at(row) += s.at(i);
      if (row == 0) {
        onDiag = false;
        ++row;
      } else if (row == numRows - 1) {
        onDiag = true;
        --row;
      } else if (onDiag) {
        --row;
      } else {
        ++row;
      }
    }
    string result = "";
    for (string row : rows) {
      result += row;
    }
    return result;
  }
};

int main() {
  string input;
  int numRows;
  cin >> input;
  cin >> numRows;
  Context c;
  try {
    c.setStrategy(new Naive);
    c.testZigzag(input, numRows);
    c.setStrategy(new SortByRow);
    c.testZigzag(input, numRows);
  } catch (...) {
    c.~Context();
    throw;
  }
  return 0;
}