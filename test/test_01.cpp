#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
using namespace std;
class Solution {
public:
    bool isValid(string s) {
        stack<char> stak;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(' || s[i] == '{' || s[i] == '[') {
                stak.push(s[i]);
            }
            else if (!stak.empty() && s[i] == ')' && stak.top() =='(') {
                stak.pop();
            }
            else if (!stak.empty() && s[i] == '}' && stak.top() == '{') {
                stak.pop();
            }
            else if (!stak.empty() && s[i] == ']' && stak.top() == '[') {
                stak.pop();
            }
            else {
                return false;
            }
        }
        if (stak.empty()) {
            return true;
        }
        else {
            return false;
        } 
    }
};

class Solution2 {
public:
    bool isValid(string s) {
        unordered_map<char, char> mp =
        {
            {'}', '{'},
            {')', '('},
            {']', '['}
        };
        stack<char> stak;

        for (char ch : s) {
            if (mp.count(ch)) {
                if (stak.empty() || stak.top() != mp[ch])
                {
                    return false;
                }
                else 
                {
                    stak.pop();
                }
            }    
            else 
            {
                stak.push(ch);
            }
        }

        return stak.empty();
    }
};

int main() {
    Solution2 sln;
    string s = "()[]{}";
    std::cout << s << " is " << sln.isValid(s);
}