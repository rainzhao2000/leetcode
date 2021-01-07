#include <iostream>
#include <string>
#include <chrono>
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
    Strategy(string name): name{name} {}
    virtual ~Strategy() {}
    virtual string longestPalindrome(const string &s) const = 0;
    friend ostream &operator<<(ostream &out, const Strategy &strat);
};

ostream &operator<<(ostream &out, const Strategy &strat) {
  return out << strat.name;
}

class Context {
  private:
    Strategy *strat;

  public:
    Context(Strategy *strat = nullptr): strat{strat} {}
    ~Context() { delete this->strat; }
    void setStrategy(Strategy *strat) {
      delete this->strat;
      this->strat = strat;
    }
    void testLongestPalindrome(const string &s) {
      if (!strat) {
        cout << "Strategy not set" << endl;
        return;
      }
      cout << "\nProblem: Given a string s, return the longest palindromic substring in s.\n";
      cout << "Input (size " << s.size() << "): " << s << endl;
      auto start = chrono::steady_clock::now();
      string soln = this->strat->longestPalindrome(s);
      auto end = chrono::steady_clock::now();
      cout << "Output: " << soln << endl;
      cout << "Strategy: " << (*this->strat) << endl;
      cout << "Time (ns): ";
      cout << chrono::duration_cast<chrono::nanoseconds>(end - start).count();
      cout << endl;
    }
};

class BruteForce: public Strategy {
  private:
    // n = length
    // T(n) = Θ(n/2)
    bool isPalindrome(const string &s, int start, int length) const {
      int m = start + length / 2;
      for (int i = start, j = start+length-1; i < m; ++i) {
        if (s.at(i) != s.at(j)) {
          return false;
        }
        --j;
      }
      return true;
    }

  public:
    BruteForce(string name = "Brute Force"): Strategy{name} {}

    // n = s.size()
    // T(n) = Θ()
    string longestPalindrome(const string &s) const override {
      int n = s.size();
      string lps = "";
      for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
          int l = j - i + 1;
          if (isPalindrome(s, i, l)) {
            if (l > lps.size()) {
              lps = s.substr(i, l);
            }
          }
        }
      }
      return lps;
    }
};

ostream &operator<<(ostream &out, const pair<bool, int> &coord) {
  return out << "(" << coord.first << "," << coord.second << ")";
}

ostream &operator<<(ostream &out, const pair<int, int> &coord) {
  return out << "(" << coord.first << "," << coord.second << ")";
}

class DynamicProgramming: public Strategy {
  public:
    DynamicProgramming(string name): Strategy{"Dynamic Programming " + name} {}
    virtual ~DynamicProgramming() {}
};

class DPSFindPalsAndStoreLengths: public DynamicProgramming {
  public:
    DPSFindPalsAndStoreLengths(): DynamicProgramming{"Find Palindromes and Store Lengths"} {}

    string longestPalindrome(const string &s) const override {
      int n = s.size();
      if (n == 0) {
        return "";
      }
      pair<bool, int> LPSLength [n][n]; // (isPalindrome, length)
      pair<int, int> LPS [n][n]; // (LPS start, LPS end)
#ifdef DEBUG
      bool visited [n][n];
#endif
      for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
          LPSLength[row][col] = { false, 0 };
          LPS[row][col] = { 0, 0 };
#ifdef DEBUG
          visited[row][col] = false;
#endif
        }
      }
      for (int l = 1; l <= n; ++l) {
        for (int i = 0; i < n-l+1; ++i) {
          int j = i + l - 1;
          if (s.at(i) == s.at(j) &&
              (i == j || j-i == 1 || LPSLength[i+1][j-1].first)) {
            LPSLength[i][j] = { true, l };
            LPS[i][j] = { i, j };
          } else {
            int sub1Length = LPSLength[i][j-1].second;
            int sub2Length = LPSLength[i+1][j].second;
            if (sub1Length > sub2Length) {
              LPSLength[i][j].second = sub1Length;
              LPS[i][j] = LPS[i][j-1];
            } else {
              LPSLength[i][j].second = sub2Length;
              LPS[i][j] = LPS[i+1][j];
            }
          }
#ifdef DEBUG
          visited[i][j] = true;
#define printState this_thread::sleep_for(chrono::milliseconds(200)); \
          cerr << "LPSLength and LPS:\n";                             \
          for (int row = 0; row < n; ++row) {                         \
            for (int col = 0; col < n; ++col) {                       \
              if (visited[row][col]) {                                \
                if (LPSLength[row][col].first) {                      \
                  cerr << GREEN;                                      \
                } else {                                              \
                  cerr << RED;                                        \
                }                                                     \
              }                                                       \
              cerr << setw(4) << LPSLength[row][col];                 \
              cerr << RESET;                                          \
            }                                                         \
            cerr << '\t';                                             \
            for (int col = 0; col < n; ++col) {                       \
              if (visited[row][col]) {                                \
                if (LPSLength[row][col].first) {                      \
                  cerr << GREEN;                                      \
                } else {                                              \
                  cerr << RED;                                        \
                }                                                     \
              }                                                       \
              cerr << setw(4) << LPS[row][col];                       \
              cerr << RESET;                                          \
            }                                                         \
            cerr << endl;                                             \
          }                                                           \
          cerr << endl;
          cerr << s.substr(i, l);
          if (LPSLength[i][j].first) {
            cerr << " is a palindrome";
          }
          cerr << endl;
          printState
#endif
        }
      }
      pair<int, int> soln = LPS[0][n-1];
      return s.substr(soln.first, soln.second - soln.first + 1);
    }
};

class DPFindPalindromes: public DynamicProgramming {
  public:
    DPFindPalindromes(): DynamicProgramming{"Find Palindromes"} {}

    string longestPalindrome(const string &s) const override {
      int n = s.size();
      if (n == 0) {
        return "";
      }
      string lps = "";
      bool isPal [n][n];
#ifdef DEBUG
      bool visited [n][n];
      for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
          isPal[row][col] = false;
          visited[row][col] = false;
        }
      }
#endif
      for (int l = 1; l <= n; ++l) {
        for (int i = 0; i < n-l+1; ++i) {
          int j = i + l - 1;
          if (s.at(i) == s.at(j) &&
              (i == j || j-i == 1 || isPal[i+1][j-1])) {
            isPal[i][j] = true;
            if (l > lps.size()) {
              lps = s.substr(i, l);
            }
          } else {
            isPal[i][j] = false;
          }
#ifdef DEBUG
          visited[i][j] = true;
#define printIsPal this_thread::sleep_for(chrono::milliseconds(200)); \
          cerr << "isPal:\n";                                         \
          for (int row = 0; row < n; ++row) {                         \
            for (int col = 0; col < n; ++col) {                       \
              if (visited[row][col]) {                                \
                if (isPal[row][col]) {                                \
                  cerr << GREEN;                                      \
                } else {                                              \
                  cerr << RED;                                        \
                }                                                     \
              }                                                       \
              cerr << setw(3) << isPal[row][col];                     \
              cerr << RESET;                                          \
            }                                                         \
            cerr << endl;                                             \
          }                                                           \
          cerr << endl;
          cerr << s.substr(i, l);
          if (isPal[i][j]) {
            cerr << " is a palindrome";
          }
          cerr << endl;
          printIsPal
#endif
        }
      }
      return lps;
    }
};

int main() {
  string input;
  cin >> input;
  Context c;
  try {
    c.setStrategy(new BruteForce);
    c.testLongestPalindrome(input);
    c.setStrategy(new DPSFindPalsAndStoreLengths);
    c.testLongestPalindrome(input);
    c.setStrategy(new DPFindPalindromes);
    c.testLongestPalindrome(input);
  } catch (...) {
    c.~Context();
    throw;
  }
  return 0;
}