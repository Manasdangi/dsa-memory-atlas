// DSA Memory Atlas: 80 interview problems, 195 commented C++ solutions.
// Every solution compiles with g++ -std=c++17 and passed LeetCode sample + edge-case tests.
// Each solution sits in its own namespace so the whole file compiles as one unit;
// to submit on LeetCode, copy the function (or class) out of its namespace.
#include <bits/stdc++.h>
using namespace std;

// LeetCode's node definitions
struct ListNode { int val; ListNode* next; ListNode() : val(0), next(nullptr) {} ListNode(int x) : val(x), next(nullptr) {} ListNode(int x, ListNode* n) : val(x), next(n) {} };
struct TreeNode { int val; TreeNode *left, *right; TreeNode() : val(0), left(nullptr), right(nullptr) {} TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} TreeNode(int x, TreeNode* l, TreeNode* r) : val(x), left(l), right(r) {} };

// ======================================================================
// ARRAYS & HASHING
// ======================================================================

// ---------- #1  LC 1  Two Sum  (Easy)
// Story: Every number walks into a locker room and shouts: "Is my partner (target − me) already inside?"

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. try every pair (i, j)
//   2. if nums[i] + nums[j] == target → return {i, j}
namespace lc1_naive {
vector<int> twoSum(vector<int>& nums, int target) {
    int n = nums.size();
    // pick the first number of the pair
    for (int i = 0; i < n; i++)
        // pair it with every number after it
        for (int j = i + 1; j < n; j++)
            // found the two that add up to target
            if (nums[i] + nums[j] == target) return {i, j};
    return {};  // problem guarantees an answer, so we never get here
}
}

// [Optimised]  Time O(n log n)  ·  Space O(n)
//   1. store (value, index) pairs, sort by value
//   2. two pointers: sum too small → l++, too big → r--
namespace lc1_optimised {
vector<int> twoSum(vector<int>& nums, int target) {
    // keep the original index next to each value, because sorting moves things
    vector<pair<int,int>> a;
    for (int i = 0; i < (int)nums.size(); i++) a.push_back({nums[i], i});
    sort(a.begin(), a.end());               // sort by value
    int l = 0, r = a.size() - 1;            // smallest and largest
    while (l < r) {
        int s = a[l].first + a[r].first;    // current pair sum
        // match: return original indices in ascending order
        if (s == target) return {min(a[l].second, a[r].second), max(a[l].second, a[r].second)};
        if (s < target) l++;                // need a bigger sum → move left pointer right
        else r--;                           // need a smaller sum → move right pointer left
    }
    return {};
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. one pass: is (target − x) already in the map? → answer
//   2. otherwise store x → index
namespace lc1_best {
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int,int> seen;            // value → index of numbers seen so far
    for (int i = 0; i < (int)nums.size(); i++) {
        // look for the partner that would complete the sum
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) return {it->second, i};  // partner was seen earlier
        seen[nums[i]] = i;                  // remember this number for later ones
    }
    return {};
}
}

// ---------- #2  LC 242  Valid Anagram  (Easy)
// Story: Two bags of Scrabble tiles — do they hold exactly the same tiles? Just count them.

// [Naive]  Time O(n log n)  ·  Space O(1)
//   1. sort both strings
//   2. equal after sorting → anagram
namespace lc242_naive {
bool isAnagram(string s, string t) {
    // anagrams become identical once their letters are sorted
    sort(s.begin(), s.end());
    sort(t.begin(), t.end());
    return s == t;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. one count[26]: +1 for s, −1 for t
//   2. all zeros → anagram
namespace lc242_best {
bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;  // different lengths can't match
    int cnt[26] = {0};                        // one counter per letter a..z
    for (int i = 0; i < (int)s.size(); i++) {
        cnt[s[i] - 'a']++;                    // letter from s adds a tile
        cnt[t[i] - 'a']--;                    // letter from t removes a tile
    }
    // any non-zero counter means the bags differ
    for (int c : cnt) if (c) return false;
    return true;
}
}

// ---------- #3  LC 49  Group Anagrams  (Medium)
// Story: Sort a word's letters and it becomes its family surname ("eat" → "aet"). Same surname → same house.

// [Naive]  Time O(n²·k log k)  ·  Space O(n)
//   1. for each unused word, compare it with every later word
//   2. sorted-equal → same group
namespace lc49_naive {
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    int n = strs.size();
    vector<bool> used(n, false);             // word already placed in a group?
    vector<vector<string>> res;
    for (int i = 0; i < n; i++) {
        if (used[i]) continue;                // already grouped
        string a = strs[i]; sort(a.begin(), a.end());   // surname of word i
        vector<string> g = {strs[i]}; used[i] = true;    // start a new group
        for (int j = i + 1; j < n; j++) {
            if (used[j]) continue;
            string b = strs[j]; sort(b.begin(), b.end()); // surname of word j
            // same surname → same group
            if (a == b) { g.push_back(strs[j]); used[j] = true; }
        }
        res.push_back(g);
    }
    return res;
}
}

// [Optimised]  Time O(n·k log k)  ·  Space O(n·k)
//   1. key = sorted word
//   2. map[key].push_back(word)
namespace lc49_optimised {
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> mp;   // surname → words
    for (auto& s : strs) {
        string key = s;
        sort(key.begin(), key.end());            // "eat" → "aet"
        mp[key].push_back(s);                    // drop the word in its house
    }
    vector<vector<string>> res;
    for (auto& [k, v] : mp) res.push_back(v);    // each house is one group
    return res;
}
}

// [Best]  Time O(n·k)  ·  Space O(n·k)
//   1. key = 26 letter counts (no sorting)
//   2. map[key].push_back(word)
namespace lc49_best {
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> mp;
    for (auto& s : strs) {
        string key(26, 0);                       // 26 slots, one per letter
        for (char c : s) key[c - 'a']++;         // count letters → same counts = same key
        mp[key].push_back(s);
    }
    vector<vector<string>> res;
    for (auto& [k, v] : mp) res.push_back(v);
    return res;
}
}

// ---------- #4  LC 347  Top K Frequent Elements  (Medium)
// Story: Election night: count the votes, then put each candidate on the shelf numbered by their vote count. Read shelves from the top.

// [Naive]  Time O(n log n)  ·  Space O(n)
//   1. count frequencies
//   2. sort (freq, value) descending, take first k
namespace lc347_naive {
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> f;
    for (int x : nums) f[x]++;                       // count votes
    vector<pair<int,int>> v;
    for (auto& [x, c] : f) v.push_back({c, x});      // (votes, candidate)
    sort(v.rbegin(), v.rend());                      // most votes first
    vector<int> res;
    for (int i = 0; i < k; i++) res.push_back(v[i].second);  // take the top k
    return res;
}
}

// [Optimised]  Time O(n log k)  ·  Space O(n)
//   1. count frequencies
//   2. min-heap of size k: push, pop when size > k
namespace lc347_optimised {
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> f;
    for (int x : nums) f[x]++;
    // min-heap by frequency: the weakest of the current top k sits on top
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    for (auto& [x, c] : f) {
        pq.push({c, x});
        if ((int)pq.size() > k) pq.pop();            // too many → drop the weakest
    }
    vector<int> res;
    while (!pq.empty()) { res.push_back(pq.top().second); pq.pop(); }
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. bucket[freq] = list of numbers (freq ≤ n)
//   2. walk buckets from n down, collect k
namespace lc347_best {
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> f;
    for (int x : nums) f[x]++;
    // shelf c holds every number that appears exactly c times (c ≤ n)
    vector<vector<int>> bucket(nums.size() + 1);
    for (auto& [x, c] : f) bucket[c].push_back(x);
    vector<int> res;
    // read shelves from the highest count down until we have k numbers
    for (int c = nums.size(); c >= 0 && (int)res.size() < k; c--)
        for (int x : bucket[c]) {
            res.push_back(x);
            if ((int)res.size() == k) break;
        }
    return res;
}
}

// ---------- #5  LC 238  Product of Array Except Self  (Medium)
// Story: Each person's gift = product of everyone on their LEFT × everyone on their RIGHT. One sunrise pass, one sunset pass.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. for each i multiply every j ≠ i
namespace lc238_naive {
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j) res[i] *= nums[j];   // multiply everyone except myself
    return res;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. pre[i] = product of everything left of i
//   2. suf[i] = product of everything right of i; answer = pre × suf
namespace lc238_optimised {
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> pre(n, 1), suf(n, 1), res(n);
    // pre[i] = nums[0] × … × nums[i-1]
    for (int i = 1; i < n; i++) pre[i] = pre[i-1] * nums[i-1];
    // suf[i] = nums[i+1] × … × nums[n-1]
    for (int i = n - 2; i >= 0; i--) suf[i] = suf[i+1] * nums[i+1];
    // everything except i = left part × right part
    for (int i = 0; i < n; i++) res[i] = pre[i] * suf[i];
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(1) extra
//   1. write prefix products into the answer
//   2. sweep back with a running suffix variable
namespace lc238_best {
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    // sunrise pass: res[i] = product of everything to the left
    for (int i = 1; i < n; i++) res[i] = res[i-1] * nums[i-1];
    int suf = 1;                          // product of everything to the right
    // sunset pass: multiply in the right side, then extend it
    for (int i = n - 1; i >= 0; i--) {
        res[i] *= suf;
        suf *= nums[i];
    }
    return res;
}
}

// ---------- #6  LC 128  Longest Consecutive Sequence  (Medium)
// Story: Only the FIRST domino of a chain (nobody at x−1) starts counting how far the chain falls.

// [Naive]  Time O(n³)  ·  Space O(1)
//   1. for each x, keep looking for x+1, x+2… with a linear search
namespace lc128_naive {
int longestConsecutive(vector<int>& nums) {
    int best = 0;
    for (int x : nums) {
        int len = 1;
        // linear search for the next number in the chain
        while (find(nums.begin(), nums.end(), x + len) != nums.end()) len++;
        best = max(best, len);
    }
    return best;
}
}

// [Optimised]  Time O(n log n)  ·  Space O(1)
//   1. sort; skip duplicates
//   2. +1 neighbour extends the run, else reset
namespace lc128_optimised {
int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;
    sort(nums.begin(), nums.end());          // chains become neighbours
    int best = 1, cur = 1;
    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] == nums[i-1]) continue;  // duplicate: doesn't break or extend
        if (nums[i] == nums[i-1] + 1) cur++; // next in the chain
        else cur = 1;                        // gap → new chain
        best = max(best, cur);
    }
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. put everything in a hash set
//   2. start counting only where x−1 is missing (chain head)
namespace lc128_best {
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> st(nums.begin(), nums.end());  // O(1) "is it here?"
    int best = 0;
    for (int x : st) {
        if (st.count(x - 1)) continue;       // not the first domino → skip
        int len = 1;
        while (st.count(x + len)) len++;     // walk the chain upward
        best = max(best, len);
    }
    return best;                              // each number is walked at most once
}
}

// ---------- #7  LC 1481  Least Number of Unique Integers after K Removals  (Medium) [Asked at Blitz]
// Story: You have k bullets. Shoot the SMALLEST gangs first — every gang wiped out is one less unique number.

// [Naive]  Time O(k·u)  ·  Space O(u)
//   1. count frequencies
//   2. k times: find the least-frequent number, remove one copy
namespace lc1481_naive {
int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
    map<int,int> f;
    for (int x : arr) f[x]++;                 // gang sizes
    while (k-- > 0) {
        // find the smallest gang by scanning all of them
        auto mn = f.begin();
        for (auto it = f.begin(); it != f.end(); ++it)
            if (it->second < mn->second) mn = it;
        // shoot one member; gang gone → erase it
        if (--mn->second == 0) f.erase(mn);
    }
    return f.size();                          // gangs still alive
}
}

// [Optimised]  Time O(n log n)  ·  Space O(n)
//   1. collect frequencies, sort ascending
//   2. while k ≥ freq: k −= freq, unique−−
namespace lc1481_optimised {
int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
    unordered_map<int,int> f;
    for (int x : arr) f[x]++;
    vector<int> freq;
    for (auto& [x, c] : f) freq.push_back(c);   // only the sizes matter
    sort(freq.begin(), freq.end());              // smallest gangs first
    int unique = freq.size();
    for (int c : freq) {
        if (k >= c) { k -= c; unique--; }        // enough bullets → wipe out this gang
        else break;                              // can't finish this one → stop
    }
    return unique;
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. cntOfFreq[f] = how many numbers appear f times
//   2. for f = 1..n remove as many whole groups as k allows
namespace lc1481_best {
int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
    unordered_map<int,int> f;
    for (int x : arr) f[x]++;
    int n = arr.size();
    vector<int> cntOfFreq(n + 1, 0);          // cntOfFreq[c] = how many gangs of size c
    for (auto& [x, c] : f) cntOfFreq[c]++;
    int unique = f.size();
    for (int c = 1; c <= n; c++) {            // sizes in increasing order, no sort needed
        int canRemove = min(cntOfFreq[c], k / c);  // whole gangs of size c we can afford
        k -= canRemove * c;
        unique -= canRemove;
        if (canRemove < cntOfFreq[c]) break;  // ran out of bullets at this size
    }
    return unique;
}
}

// ======================================================================
// PREFIX SUM & KADANE
// ======================================================================

// ---------- #8  LC 53  Maximum Subarray  (Medium)
// Story: Kadane's bank balance: the moment your running balance goes negative, tear up the passbook and start fresh.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i, extend j and keep a running sum
namespace lc53_naive {
int maxSubArray(vector<int>& nums) {
    int best = INT_MIN, n = nums.size();
    for (int i = 0; i < n; i++) {             // subarray starts at i
        int s = 0;
        for (int j = i; j < n; j++) {         // and ends at j
            s += nums[j];                     // running sum of nums[i..j]
            best = max(best, s);
        }
    }
    return best;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. prefix sums: best = pre[i] − smallest earlier prefix
namespace lc53_optimised {
int maxSubArray(vector<int>& nums) {
    int n = nums.size();
    vector<long long> pre(n + 1, 0);          // pre[i] = sum of first i numbers
    for (int i = 0; i < n; i++) pre[i+1] = pre[i] + nums[i];
    long long best = LLONG_MIN, mn = pre[0];  // mn = smallest prefix seen so far
    for (int i = 1; i <= n; i++) {
        best = max(best, pre[i] - mn);        // subarray ending at i-1 with the best start
        mn = min(mn, pre[i]);
    }
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. cur = max(x, cur + x)  (restart or extend)
//   2. best = max(best, cur)
namespace lc53_best {
int maxSubArray(vector<int>& nums) {
    int cur = 0, best = INT_MIN;
    for (int x : nums) {
        // either extend the current run or start fresh at x
        cur = max(x, cur + x);
        best = max(best, cur);                // best balance seen so far
    }
    return best;
}
}

// ---------- #9  LC 560  Subarray Sum Equals K  (Medium)
// Story: Odometer trick: if it reads P now and read P − k earlier, the stretch in between was exactly k km.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i, extend j, count sums equal to k
namespace lc560_naive {
int subarraySum(vector<int>& nums, int k) {
    int cnt = 0, n = nums.size();
    for (int i = 0; i < n; i++) {
        int s = 0;
        for (int j = i; j < n; j++) {
            s += nums[j];                     // sum of nums[i..j]
            if (s == k) cnt++;
        }
    }
    return cnt;
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. running prefix sum; map counts of earlier prefixes (start with {0:1})
//   2. answer += count[prefix − k]
namespace lc560_best {
int subarraySum(vector<int>& nums, int k) {
    // how many times each prefix sum has appeared; empty prefix (0) once
    unordered_map<int,int> seen{{0, 1}};
    int pre = 0, cnt = 0;
    for (int x : nums) {
        pre += x;                             // odometer reading now
        // every earlier reading of pre − k gives a stretch of exactly k
        if (seen.count(pre - k)) cnt += seen[pre - k];
        seen[pre]++;                          // record this reading
    }
    return cnt;
}
}

// ---------- #10  LC 121  Best Time to Buy and Sell Stock  (Easy)
// Story: Walk through the days remembering the cheapest day so far; every day ask "what if I sold today?"

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. try every buy day i and sell day j > i
namespace lc121_naive {
int maxProfit(vector<int>& prices) {
    int best = 0, n = prices.size();
    for (int i = 0; i < n; i++)               // buy on day i
        for (int j = i + 1; j < n; j++)       // sell on a later day j
            best = max(best, prices[j] - prices[i]);
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. track min price so far
//   2. profit today = price − min
namespace lc121_best {
int maxProfit(vector<int>& prices) {
    int mn = INT_MAX, best = 0;
    for (int p : prices) {
        mn = min(mn, p);                      // cheapest day so far = best day to have bought
        best = max(best, p - mn);             // profit if I sell today
    }
    return best;
}
}

// ======================================================================
// TWO POINTERS
// ======================================================================

// ---------- #11  LC 125  Valid Palindrome  (Easy)
// Story: Two friends start at opposite ends of a bridge and walk toward each other, hopping over puddles (non-letters). They must see the same letter at every step.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. build a cleaned lowercase string
//   2. compare it with its reverse
namespace lc125_naive {
bool isPalindrome(string s) {
    string t;
    // keep only letters/digits, lowercased
    for (char c : s) if (isalnum((unsigned char)c)) t += tolower((unsigned char)c);
    string r(t.rbegin(), t.rend());           // reversed copy
    return t == r;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. l from left, r from right, skip non-alphanumerics
//   2. mismatch → false
namespace lc125_best {
bool isPalindrome(string s) {
    int l = 0, r = (int)s.size() - 1;
    while (l < r) {
        // hop over puddles (non-alphanumeric) on both sides
        while (l < r && !isalnum((unsigned char)s[l])) l++;
        while (l < r && !isalnum((unsigned char)s[r])) r--;
        // the two friends must see the same letter (case-insensitive)
        if (tolower((unsigned char)s[l]) != tolower((unsigned char)s[r])) return false;
        l++; r--;                             // step toward each other
    }
    return true;
}
}

// ---------- #12  LC 167  Two Sum II – Input Array Is Sorted  (Medium)
// Story: A sorted array is a seesaw: sum too heavy → right friend steps in; too light → left friend steps in.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. try every pair
namespace lc167_naive {
vector<int> twoSum(vector<int>& numbers, int target) {
    int n = numbers.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (numbers[i] + numbers[j] == target) return {i + 1, j + 1};  // 1-indexed
    return {};
}
}

// [Optimised]  Time O(n log n)  ·  Space O(1)
//   1. for each i, binary-search target − numbers[i] to its right
namespace lc167_optimised {
vector<int> twoSum(vector<int>& numbers, int target) {
    int n = numbers.size();
    for (int i = 0; i < n; i++) {
        // the partner must be target − numbers[i], somewhere to the right
        int lo = i + 1, hi = n - 1, want = target - numbers[i];
        while (lo <= hi) {
            int m = lo + (hi - lo) / 2;
            if (numbers[m] == want) return {i + 1, m + 1};
            if (numbers[m] < want) lo = m + 1;   // partner is bigger → go right
            else hi = m - 1;                     // partner is smaller → go left
        }
    }
    return {};
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. l = 0, r = n−1
//   2. sum < target → l++; sum > target → r--
namespace lc167_best {
vector<int> twoSum(vector<int>& numbers, int target) {
    int l = 0, r = numbers.size() - 1;
    while (l < r) {
        int s = numbers[l] + numbers[r];
        if (s == target) return {l + 1, r + 1};
        if (s < target) l++;                  // too light → bigger left number
        else r--;                             // too heavy → smaller right number
    }
    return {};
}
}

// ---------- #13  LC 15  3Sum  (Medium)
// Story: Pick a captain, then run the Two-Sum seesaw on the players to his right. Skip twins so you never form the same team twice.

// [Naive]  Time O(n³)  ·  Space O(k)
//   1. try all triples, sort each, dedupe with a set
namespace lc15_naive {
vector<vector<int>> threeSum(vector<int>& nums) {
    int n = nums.size();
    set<vector<int>> st;                      // set removes duplicate triplets
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++)
                if (nums[i] + nums[j] + nums[k] == 0) {
                    vector<int> t = {nums[i], nums[j], nums[k]};
                    sort(t.begin(), t.end());     // same triplet → same order
                    st.insert(t);
                }
    return vector<vector<int>>(st.begin(), st.end());
}
}

// [Optimised]  Time O(n²)  ·  Space O(n)
//   1. fix i; for j > i look up −(a[i]+a[j]) in a hash set
namespace lc15_optimised {
vector<vector<int>> threeSum(vector<int>& nums) {
    int n = nums.size();
    set<vector<int>> st;
    for (int i = 0; i < n; i++) {             // captain
        unordered_set<int> seen;              // numbers between i and j
        for (int j = i + 1; j < n; j++) {
            int need = -nums[i] - nums[j];    // third number that makes the sum 0
            if (seen.count(need)) {
                vector<int> t = {nums[i], nums[j], need};
                sort(t.begin(), t.end());
                st.insert(t);
            }
            seen.insert(nums[j]);
        }
    }
    return vector<vector<int>>(st.begin(), st.end());
}
}

// [Best]  Time O(n²)  ·  Space O(1) extra
//   1. sort; for each captain i (skip duplicates)
//   2. two pointers l, r on the rest; skip duplicates after a hit
namespace lc15_best {
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());           // sorting enables the seesaw and dedupe
    vector<vector<int>> res;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i-1]) continue;   // twin captain → same teams
        if (nums[i] > 0) break;               // smallest is positive → no zero sum
        int l = i + 1, r = n - 1;
        while (l < r) {
            int s = nums[i] + nums[l] + nums[r];
            if (s < 0) l++;                   // too light
            else if (s > 0) r--;              // too heavy
            else {
                res.push_back({nums[i], nums[l], nums[r]});
                // skip twins on both sides so this team isn't added again
                while (l < r && nums[l] == nums[l+1]) l++;
                while (l < r && nums[r] == nums[r-1]) r--;
                l++; r--;
            }
        }
    }
    return res;
}
}

// ---------- #14  LC 11  Container With Most Water  (Medium)
// Story: The water level is set by the SHORTER wall. Moving the taller wall can never help, so always move the short one.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. try every pair of walls
namespace lc11_naive {
int maxArea(vector<int>& height) {
    int best = 0, n = height.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            // width × the shorter wall
            best = max(best, (j - i) * min(height[i], height[j]));
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. l = 0, r = n−1, area = width × shorter wall
//   2. move the pointer at the shorter wall inward
namespace lc11_best {
int maxArea(vector<int>& height) {
    int l = 0, r = height.size() - 1, best = 0;
    while (l < r) {
        best = max(best, (r - l) * min(height[l], height[r]));
        // the shorter wall limits the water; moving the taller one can't help
        if (height[l] < height[r]) l++;
        else r--;
    }
    return best;
}
}

// ---------- #15  LC 42  Trapping Rain Water  (Hard)
// Story: Each bar holds water = min(tallest wall on its left, tallest on its right) − its own height.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. for every bar scan left max and right max
namespace lc42_naive {
int trap(vector<int>& height) {
    int n = height.size(), water = 0;
    for (int i = 0; i < n; i++) {
        int lm = 0, rm = 0;
        for (int j = 0; j <= i; j++) lm = max(lm, height[j]);   // tallest on the left
        for (int j = i; j < n; j++) rm = max(rm, height[j]);    // tallest on the right
        water += min(lm, rm) - height[i];     // water above bar i
    }
    return water;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. precompute leftMax[] and rightMax[] arrays
namespace lc42_optimised {
int trap(vector<int>& height) {
    int n = height.size();
    if (n == 0) return 0;
    vector<int> L(n), R(n);
    // L[i] = tallest bar in [0..i]
    L[0] = height[0];
    for (int i = 1; i < n; i++) L[i] = max(L[i-1], height[i]);
    // R[i] = tallest bar in [i..n-1]
    R[n-1] = height[n-1];
    for (int i = n - 2; i >= 0; i--) R[i] = max(R[i+1], height[i]);
    int water = 0;
    for (int i = 0; i < n; i++) water += min(L[i], R[i]) - height[i];
    return water;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. two pointers; the side with the smaller wall is "settled"
//   2. add (sideMax − height) on that side, move it inward
namespace lc42_best {
int trap(vector<int>& height) {
    int l = 0, r = (int)height.size() - 1;
    int lm = 0, rm = 0, water = 0;            // tallest seen from each side
    while (l < r) {
        if (height[l] < height[r]) {
            // right side has a taller wall, so the left max decides the water here
            lm = max(lm, height[l]);
            water += lm - height[l];
            l++;
        } else {
            // left side is at least as tall, so the right max decides
            rm = max(rm, height[r]);
            water += rm - height[r];
            r--;
        }
    }
    return water;
}
}

// ---------- #16  LC 75  Sort Colors  (Medium)
// Story: Dutch flag: red gets pushed to the left wall, blue to the right wall, white stays in the middle. One sweeper walks through.

// [Naive]  Time O(n log n)  ·  Space O(1)
//   1. just sort
namespace lc75_naive {
void sortColors(vector<int>& nums) {
    sort(nums.begin(), nums.end());           // works, but ignores that there are only 3 values
}
}

// [Optimised]  Time O(n) two passes  ·  Space O(1)
//   1. count 0s, 1s, 2s; rewrite the array
namespace lc75_optimised {
void sortColors(vector<int>& nums) {
    int c[3] = {0};
    for (int x : nums) c[x]++;                // how many 0s, 1s, 2s
    int i = 0;
    for (int v = 0; v < 3; v++)
        while (c[v]--) nums[i++] = v;         // write them back in order
}
}

// [Best]  Time O(n) one pass  ·  Space O(1)
//   1. lo = next slot for 0, hi = next slot for 2, mid = sweeper
//   2. 0 → swap with lo; 2 → swap with hi (don't advance mid); 1 → mid++
namespace lc75_best {
void sortColors(vector<int>& nums) {
    int lo = 0, mid = 0, hi = nums.size() - 1;
    // [0, lo) = 0s, [lo, mid) = 1s, (hi, end] = 2s, [mid, hi] = unknown
    while (mid <= hi) {
        if (nums[mid] == 0) swap(nums[lo++], nums[mid++]);   // send red to the left wall
        else if (nums[mid] == 1) mid++;                      // white stays in the middle
        else swap(nums[mid], nums[hi--]);                    // send blue right; recheck what came back
    }
}
}

// ======================================================================
// SLIDING WINDOW
// ======================================================================

// ---------- #17  LC 3  Longest Substring Without Repeating Characters  (Medium)
// Story: A train window sliding over letters: the moment a duplicate passenger boards, kick people out from the back until everyone is unique again.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i, extend j until a repeat
namespace lc3_naive {
int lengthOfLongestSubstring(string s) {
    int n = s.size(), best = 0;
    for (int i = 0; i < n; i++) {             // window starts at i
        bool seen[256] = {false};
        for (int j = i; j < n; j++) {
            if (seen[(unsigned char)s[j]]) break;   // repeat → this start is done
            seen[(unsigned char)s[j]] = true;
            best = max(best, j - i + 1);
        }
    }
    return best;
}
}

// [Optimised]  Time O(n)  ·  Space O(k)
//   1. window [l, r] kept in a set
//   2. while s[r] is already inside, erase s[l++]
namespace lc3_optimised {
int lengthOfLongestSubstring(string s) {
    unordered_set<char> win;                  // letters currently in the window
    int l = 0, best = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        // duplicate boarding → kick passengers out from the back
        while (win.count(s[r])) win.erase(s[l++]);
        win.insert(s[r]);
        best = max(best, r - l + 1);
    }
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. remember last index of each char
//   2. on a repeat inside the window, jump l to last+1 directly
namespace lc3_best {
int lengthOfLongestSubstring(string s) {
    vector<int> last(256, -1);                // last index where each char was seen
    int l = 0, best = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        unsigned char c = s[r];
        // seen inside the current window → jump past its last position
        if (last[c] >= l) l = last[c] + 1;
        last[c] = r;
        best = max(best, r - l + 1);
    }
    return best;
}
}

// ---------- #18  LC 424  Longest Repeating Character Replacement  (Medium)
// Story: A window is fine if (window size − count of its most common letter) ≤ k paint strokes. Too many strokes needed → shrink from the left.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i, extend j, track max letter count
namespace lc424_naive {
int characterReplacement(string s, int k) {
    int n = s.size(), best = 0;
    for (int i = 0; i < n; i++) {
        int cnt[26] = {0}, mx = 0;            // mx = most common letter count in s[i..j]
        for (int j = i; j < n; j++) {
            mx = max(mx, ++cnt[s[j] - 'A']);
            // letters that need repainting = length − mx
            if (j - i + 1 - mx <= k) best = max(best, j - i + 1);
        }
    }
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. expand r, update maxCount
//   2. while (len − maxCount > k) shrink l
namespace lc424_best {
int characterReplacement(string s, int k) {
    int cnt[26] = {0}, l = 0, mx = 0, best = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        mx = max(mx, ++cnt[s[r] - 'A']);      // most common letter in the window
        // too many repaints needed → slide the left edge
        while (r - l + 1 - mx > k) cnt[s[l++] - 'A']--;
        best = max(best, r - l + 1);
    }
    return best;
}
}

// ---------- #19  LC 567  Permutation in String  (Medium)
// Story: Slide a stencil the size of s1 across s2. When the letters under the stencil have the same counts as s1, you found it.

// [Naive]  Time O(n·m log m)  ·  Space O(m)
//   1. every window of size m: sort and compare with sorted s1
namespace lc567_naive {
bool checkInclusion(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    sort(s1.begin(), s1.end());
    for (int i = 0; i + m <= n; i++) {
        string w = s2.substr(i, m);           // window of size m
        sort(w.begin(), w.end());
        if (w == s1) return true;             // same letters → a permutation
    }
    return false;
}
}

// [Optimised]  Time O(26·n)  ·  Space O(1)
//   1. count arrays for s1 and the current window
//   2. slide: add right char, remove left char, compare arrays
namespace lc567_optimised {
bool checkInclusion(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    if (m > n) return false;
    array<int,26> a{}, b{};                   // a = counts of s1, b = counts in the window
    for (int i = 0; i < m; i++) { a[s1[i] - 'a']++; b[s2[i] - 'a']++; }
    if (a == b) return true;
    for (int i = m; i < n; i++) {
        b[s2[i] - 'a']++;                     // letter entering on the right
        b[s2[i - m] - 'a']--;                 // letter leaving on the left
        if (a == b) return true;
    }
    return false;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. need[] = counts still missing; missing = m
//   2. add char: if it was needed, missing−−; drop char: if now needed, missing++
namespace lc567_best {
bool checkInclusion(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    if (m > n) return false;
    int need[26] = {0};
    for (char c : s1) need[c - 'a']++;        // letters still missing from the window
    int missing = m;                          // total letters still missing
    for (int r = 0; r < n; r++) {
        // entering letter fills a need → one fewer missing
        if (need[s2[r] - 'a']-- > 0) missing--;
        // leaving letter (window bigger than m) re-opens a need → one more missing
        if (r >= m && ++need[s2[r - m] - 'a'] > 0) missing++;
        if (missing == 0) return true;        // window is a permutation of s1
    }
    return false;
}
}

// ---------- #20  LC 76  Minimum Window Substring  (Hard)
// Story: Shopping-cart window: push right until the cart holds the whole list, then pop from the left while it still does. Keep the smallest cart.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i: extend j until all of t is covered
namespace lc76_naive {
string minWindow(string s, string t) {
    int need[128] = {0};
    for (char c : t) need[(int)c]++;          // shopping list
    int n = s.size(), bestL = 0, bestLen = INT_MAX;
    for (int i = 0; i < n; i++) {
        int have[128] = {0}, missing = t.size();
        for (int j = i; j < n; j++) {
            // this item was still on the list → one fewer missing
            if (have[(int)s[j]]++ < need[(int)s[j]]) missing--;
            if (missing == 0) {               // cart complete for start i
                if (j - i + 1 < bestLen) { bestLen = j - i + 1; bestL = i; }
                break;
            }
        }
    }
    return bestLen == INT_MAX ? "" : s.substr(bestL, bestLen);
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. expand r: if s[r] was needed, missing−−
//   2. while missing == 0: record, then drop s[l++] (missing++ if it becomes needed)
namespace lc76_best {
string minWindow(string s, string t) {
    int need[128] = {0};
    for (char c : t) need[(int)c]++;          // positive = still needed
    int missing = t.size(), l = 0, bestL = 0, bestLen = INT_MAX;
    for (int r = 0; r < (int)s.size(); r++) {
        // put s[r] in the cart; if it was needed, the list shrinks
        if (need[(int)s[r]]-- > 0) missing--;
        while (missing == 0) {                // cart has everything
            if (r - l + 1 < bestLen) { bestLen = r - l + 1; bestL = l; }
            // remove s[l]; if it becomes needed again, the cart is incomplete
            if (++need[(int)s[l++]] > 0) missing++;
        }
    }
    return bestLen == INT_MAX ? "" : s.substr(bestL, bestLen);
}
}

// ---------- #21  LC 239  Sliding Window Maximum  (Hard)
// Story: The bouncer's queue: a tall newcomer throws out everyone shorter standing behind him, since they can never be the tallest again. The front of the queue is the current king.

// [Naive]  Time O(n·k)  ·  Space O(1)
//   1. for every window scan its max
namespace lc239_naive {
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> res;
    // window starts at i and covers nums[i .. i+k-1]
    for (int i = 0; i + k <= (int)nums.size(); i++)
        res.push_back(*max_element(nums.begin() + i, nums.begin() + i + k));
    return res;
}
}

// [Optimised]  Time O(n log k)  ·  Space O(k)
//   1. keep the window in a multiset (sorted)
//   2. add new, erase the one leaving, max = last element
namespace lc239_optimised {
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    multiset<int> win;                        // sorted window, duplicates allowed
    vector<int> res;
    for (int i = 0; i < (int)nums.size(); i++) {
        win.insert(nums[i]);                  // newcomer enters
        if (i >= k) win.erase(win.find(nums[i - k]));   // erase ONE copy of the leaver
        if (i >= k - 1) res.push_back(*win.rbegin());   // largest = last element
    }
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(k)
//   1. deque of indices with decreasing values
//   2. drop front if it left the window; pop back while smaller than new
namespace lc239_best {
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;                            // indices; their values decrease front → back
    vector<int> res;
    for (int i = 0; i < (int)nums.size(); i++) {
        // the king at the front walked out of the window
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        // tall newcomer throws out everyone shorter: they can never be the max again
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(nums[dq.front()]);  // front = window max
    }
    return res;
}
}

// ======================================================================
// STACK & MONOTONIC STACK
// ======================================================================

// ---------- #22  LC 20  Valid Parentheses  (Easy)
// Story: A stack of plates: every opener puts a plate on top; every closer must match the TOP plate exactly or the tower falls.

// [Naive]  Time O(n²)  ·  Space O(n)
//   1. keep deleting "()", "[]", "{}" until nothing changes
//   2. empty at the end → valid
namespace lc20_naive {
bool isValid(string s) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (string p : {"()", "[]", "{}"}) {
            size_t pos;
            // delete every adjacent matching pair we can find
            while ((pos = s.find(p)) != string::npos) { s.erase(pos, 2); changed = true; }
        }
    }
    return s.empty();                         // everything paired off → valid
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. opener → push
//   2. closer → stack top must be its partner, pop
namespace lc20_best {
bool isValid(string s) {
    stack<char> st;                           // openers still waiting for a partner
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.push(c);   // put a plate on top
        else {
            if (st.empty()) return false;     // closer with nothing to close
            char o = st.top(); st.pop();
            // the closer must match the TOP plate exactly
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{')) return false;
        }
    }
    return st.empty();                        // leftover openers → invalid
}
}

// ---------- #23  LC 155  Min Stack  (Medium)
// Story: Every plate carries a sticky note saying "smallest plate at or below me". When you pop the plate, its note leaves with it.

// [Naive]  Time getMin O(n)  ·  Space O(n)
//   1. plain stack; scan for min when asked
namespace lc155_naive {
class MinStack {
    vector<int> st;                           // the stack itself
public:
    void push(int v) { st.push_back(v); }
    void pop() { st.pop_back(); }
    int top() { return st.back(); }
    // scan everything → O(n) per call
    int getMin() { return *min_element(st.begin(), st.end()); }
};
}

// [Best]  Time all O(1)  ·  Space O(n)
//   1. push (value, min(value, previous min))
namespace lc155_best {
class MinStack {
    vector<pair<int,int>> st;                 // (value, smallest value at or below it)
public:
    void push(int v) {
        // sticky note = min of this value and the note of the plate below
        st.push_back({v, st.empty() ? v : min(v, st.back().second)});
    }
    void pop() { st.pop_back(); }             // the note leaves with the plate
    int top() { return st.back().first; }
    int getMin() { return st.back().second; } // read the top plate's note
};
}

// ---------- #24  LC 150  Evaluate Reverse Polish Notation  (Medium)
// Story: A calculator built on a plate pile: numbers go on the pile; an operator eats the top two plates and puts the result back.

// [Naive]  Time O(n²)  ·  Space O(n)
//   1. find the first operator, combine the two numbers before it
//   2. splice the result back, repeat
namespace lc150_naive {
int evalRPN(vector<string>& tokens) {
    vector<string> t = tokens;
    auto isOp = [](const string& s) { return s == "+" || s == "-" || s == "*" || s == "/"; };
    while (t.size() > 1) {
        int i = 0;
        while (!isOp(t[i])) i++;              // first operator
        long long a = stoll(t[i-2]), b = stoll(t[i-1]), r;   // its two operands
        if (t[i] == "+") r = a + b;
        else if (t[i] == "-") r = a - b;
        else if (t[i] == "*") r = a * b;
        else r = a / b;
        // replace "a b op" with the result
        t.erase(t.begin() + i - 2, t.begin() + i + 1);
        t.insert(t.begin() + i - 2, to_string(r));
    }
    return stoi(t[0]);
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. number → push
//   2. operator → pop b, pop a, push a op b
namespace lc150_best {
int evalRPN(vector<string>& tokens) {
    stack<long long> st;
    for (auto& tk : tokens) {
        if (tk == "+" || tk == "-" || tk == "*" || tk == "/") {
            // the top plate is the RIGHT operand
            long long b = st.top(); st.pop();
            long long a = st.top(); st.pop();
            if (tk == "+") st.push(a + b);
            else if (tk == "-") st.push(a - b);
            else if (tk == "*") st.push(a * b);
            else st.push(a / b);              // truncates toward zero, like LeetCode
        } else st.push(stoll(tk));            // a number goes on the pile
    }
    return st.top();
}
}

// ---------- #25  LC 739  Daily Temperatures  (Medium)
// Story: Cold days wait in a line (stack). When a warm day arrives, every colder day still waiting gets its answer and leaves the line.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. for each day scan forward for the first warmer day
namespace lc739_naive {
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> res(n, 0);                    // 0 = no warmer day ahead
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (t[j] > t[i]) { res[i] = j - i; break; }   // first warmer day
    return res;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. monotonic stack of indices (decreasing temps)
//   2. warmer day pops all colder ones and fills their answers
namespace lc739_optimised {
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> res(n, 0);
    stack<int> st;                            // days still waiting for a warmer day
    for (int i = 0; i < n; i++) {
        // today is warmer than the waiting days on top → they get their answer
        while (!st.empty() && t[st.top()] < t[i]) {
            res[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);                           // today starts waiting
    }
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(1) extra
//   1. go right-to-left; hop using answers already computed
//   2. j += res[j] skips days that are known to be colder
namespace lc739_best {
vector<int> dailyTemperatures(vector<int>& t) {
    int n = t.size();
    vector<int> res(n, 0);
    for (int i = n - 2; i >= 0; i--) {        // right to left: answers to the right are known
        int j = i + 1;
        while (j < n && t[j] <= t[i]) {
            if (res[j] == 0) { j = n; break; } // nothing warmer than j ahead → none for i
            j += res[j];                       // hop to the next day warmer than j
        }
        if (j < n) res[i] = j - i;
    }
    return res;
}
}

// ---------- #26  LC 84  Largest Rectangle in Histogram  (Hard)
// Story: Each bar asks: "How far can I stretch left and right before I hit someone shorter?" Width × my height = my rectangle.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i, extend j, keep min height
namespace lc84_naive {
int largestRectangleArea(vector<int>& h) {
    int n = h.size(), best = 0;
    for (int i = 0; i < n; i++) {
        int mn = INT_MAX;                     // shortest bar in h[i..j]
        for (int j = i; j < n; j++) {
            mn = min(mn, h[j]);
            best = max(best, mn * (j - i + 1));
        }
    }
    return best;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. prev-smaller index L[i] and next-smaller index R[i] via stacks
//   2. area = h[i] × (R[i] − L[i] − 1)
namespace lc84_optimised {
int largestRectangleArea(vector<int>& h) {
    int n = h.size();
    vector<int> L(n), R(n);
    stack<int> st;
    // L[i] = nearest index on the left with a SHORTER bar (or -1)
    for (int i = 0; i < n; i++) {
        while (!st.empty() && h[st.top()] >= h[i]) st.pop();
        L[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    while (!st.empty()) st.pop();
    // R[i] = nearest index on the right with a SHORTER bar (or n)
    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && h[st.top()] >= h[i]) st.pop();
        R[i] = st.empty() ? n : st.top();
        st.push(i);
    }
    int best = 0;
    // bar i can stretch between L[i] and R[i] (exclusive)
    for (int i = 0; i < n; i++) best = max(best, h[i] * (R[i] - L[i] - 1));
    return best;
}
}

// [Best]  Time O(n) one pass  ·  Space O(n)
//   1. increasing stack; a shorter bar (or the 0 sentinel at the end) pops taller bars
//   2. popped bar's width = i − (new top) − 1
namespace lc84_best {
int largestRectangleArea(vector<int>& h) {
    stack<int> st;                            // indices with increasing heights
    int best = 0, n = h.size();
    for (int i = 0; i <= n; i++) {
        int cur = (i == n) ? 0 : h[i];        // height 0 at the end flushes the stack
        // cur is shorter → every taller bar on the stack now knows its right boundary
        while (!st.empty() && h[st.top()] >= cur) {
            int height = h[st.top()]; st.pop();
            int left = st.empty() ? -1 : st.top();      // its left boundary
            best = max(best, height * (i - left - 1));  // width between the boundaries
        }
        st.push(i);
    }
    return best;
}
}

// ---------- #27  LC 32  Longest Valid Parentheses  (Hard) [Asked at Blitz]
// Story: The stack remembers where the last "wall" is (an unmatched ')' or the −1 floor). Every match measures its distance back to that wall.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. every start i: balance counter; < 0 break; == 0 record length
namespace lc32_naive {
int longestValidParentheses(string s) {
    int n = s.size(), best = 0;
    for (int i = 0; i < n; i++) {             // substring starts at i
        int bal = 0;                          // open − close so far
        for (int j = i; j < n; j++) {
            bal += (s[j] == '(') ? 1 : -1;
            if (bal < 0) break;               // more ')' than '(' → can't be fixed
            if (bal == 0) best = max(best, j - i + 1);   // balanced → valid
        }
    }
    return best;
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. stack of indices, start with −1
//   2. '(' push i; ')' pop → empty? push i (new wall) : len = i − top
namespace lc32_optimised {
int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1);                              // the floor: a wall before index 0
    int best = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '(') st.push(i);          // remember where this '(' opened
        else {
            st.pop();                         // match the latest '(' (or knock the wall)
            if (st.empty()) st.push(i);       // unmatched ')' → it becomes the new wall
            else best = max(best, i - st.top());   // distance back to the wall
        }
    }
    return best;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. left→right: count open/close; equal → record; close > open → reset
//   2. right→left: same with roles swapped (catches "(()")
namespace lc32_best {
int longestValidParentheses(string s) {
    int open = 0, close = 0, best = 0, n = s.size();
    // left → right: catches runs where ')' would overflow
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') open++; else close++;
        if (open == close) best = max(best, 2 * close);  // balanced run
        else if (close > open) open = close = 0;         // broken → restart
    }
    open = close = 0;
    // right → left: catches runs with extra '(' like "(()"
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] == '(') open++; else close++;
        if (open == close) best = max(best, 2 * open);
        else if (open > close) open = close = 0;
    }
    return best;
}
}

// ---------- #28  LC 1249  Minimum Remove to Make Valid Parentheses  (Medium)
// Story: A bouncer with a guest list: an unmatched ')' is thrown out on the spot; '(' still waiting on the stack are thrown out at closing time.

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. stack of '(' indices; ')' with empty stack → mark drop
//   2. leftover stack indices → mark drop; rebuild string
namespace lc1249_optimised {
string minRemoveToMakeValid(string s) {
    stack<int> st;                            // indices of '(' still waiting
    vector<bool> drop(s.size(), false);       // characters to remove
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '(') st.push(i);
        else if (s[i] == ')') {
            if (st.empty()) drop[i] = true;   // no '(' to match → throw it out now
            else st.pop();                    // matched
        }
    }
    // '(' never matched → throw them out at closing time
    while (!st.empty()) { drop[st.top()] = true; st.pop(); }
    string res;
    for (int i = 0; i < (int)s.size(); i++) if (!drop[i]) res += s[i];
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(1) extra
//   1. pass 1 (left→right): skip ')' with no open '('
//   2. pass 2 (right→left): skip the extra '(' still open
namespace lc1249_best {
string minRemoveToMakeValid(string s) {
    string a;
    int open = 0;                             // '(' kept but not yet closed
    for (char c : s) {
        if (c == '(') open++;
        else if (c == ')') {
            if (open == 0) continue;          // unmatched ')' → skip it
            open--;
        }
        a += c;
    }
    // now `open` extra '(' remain; drop the rightmost ones
    string res;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        if (a[i] == '(' && open > 0) { open--; continue; }
        res += a[i];
    }
    reverse(res.begin(), res.end());          // we built it backwards
    return res;
}
}

// ======================================================================
// BINARY SEARCH
// ======================================================================

// ---------- #29  LC 704  Binary Search  (Easy)
// Story: Open the dictionary in the middle. Wrong half? Tear it off. Every guess halves the book.

// [Naive]  Time O(n)  ·  Space O(1)
//   1. scan left to right
namespace lc704_naive {
int search(vector<int>& nums, int target) {
    for (int i = 0; i < (int)nums.size(); i++)
        if (nums[i] == target) return i;      // check every page
    return -1;
}
}

// [Best]  Time O(log n)  ·  Space O(1)
//   1. lo..hi; m = lo + (hi−lo)/2
//   2. too small → lo = m+1; too big → hi = m−1
namespace lc704_best {
int search(vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo <= hi) {                        // search space [lo, hi] not empty
        int m = lo + (hi - lo) / 2;           // avoids (lo + hi) overflow
        if (nums[m] == target) return m;
        if (nums[m] < target) lo = m + 1;     // target is right → tear off left half
        else hi = m - 1;                      // target is left → tear off right half
    }
    return -1;
}
}

// ---------- #30  LC 33  Search in Rotated Sorted Array  (Medium)
// Story: A rotated clock face: cut it anywhere and at least ONE half is still in order. Ask: does the target live inside the ordered half?

// [Naive]  Time O(n)  ·  Space O(1)
//   1. linear scan
namespace lc33_naive {
int search(vector<int>& nums, int target) {
    // ignore the rotation and just check every element
    for (int i = 0; i < (int)nums.size(); i++)
        if (nums[i] == target) return i;
    return -1;                                // not present
}
}

// [Optimised]  Time O(log n), 2 searches  ·  Space O(1)
//   1. binary-search the pivot (smallest element)
//   2. normal binary search in the half that can contain target
namespace lc33_optimised {
int search(vector<int>& nums, int target) {
    int n = nums.size(), lo = 0, hi = n - 1;
    // step 1: find the pivot = index of the smallest element
    while (lo < hi) {
        int m = (lo + hi) / 2;
        if (nums[m] > nums[hi]) lo = m + 1;   // drop is to the right of m
        else hi = m;
    }
    int piv = lo;
    // step 2: ordinary binary search on a sorted range [l, r]
    auto bs = [&](int l, int r) {
        while (l <= r) {
            int m = (l + r) / 2;
            if (nums[m] == target) return m;
            if (nums[m] < target) l = m + 1; else r = m - 1;
        }
        return -1;
    };
    int a = bs(0, piv - 1);                   // left sorted run
    return a != -1 ? a : bs(piv, n - 1);      // else right sorted run
}
}

// [Best]  Time O(log n), 1 search  ·  Space O(1)
//   1. if left half sorted: target inside it? go left : go right
//   2. else right half sorted: target inside it? go right : go left
namespace lc33_best {
int search(vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int m = lo + (hi - lo) / 2;
        if (nums[m] == target) return m;
        if (nums[lo] <= nums[m]) {
            // left half [lo..m] is in order
            if (nums[lo] <= target && target < nums[m]) hi = m - 1;   // target lives there
            else lo = m + 1;
        } else {
            // right half [m..hi] is in order
            if (nums[m] < target && target <= nums[hi]) lo = m + 1;   // target lives there
            else hi = m - 1;
        }
    }
    return -1;
}
}

// ---------- #31  LC 153  Find Minimum in Rotated Sorted Array  (Medium)
// Story: Compare the middle with the RIGHT end. If the middle is bigger, the cliff (minimum) is somewhere to the right.

// [Naive]  Time O(n)  ·  Space O(1)
//   1. min_element
namespace lc153_naive {
int findMin(vector<int>& nums) {
    return *min_element(nums.begin(), nums.end());   // check everything
}
}

// [Best]  Time O(log n)  ·  Space O(1)
//   1. nums[m] > nums[hi] → lo = m+1, else hi = m
namespace lc153_best {
int findMin(vector<int>& nums) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo < hi) {
        int m = lo + (hi - lo) / 2;
        // middle bigger than the right end → the drop (minimum) is after m
        if (nums[m] > nums[hi]) lo = m + 1;
        else hi = m;                          // m could itself be the minimum
    }
    return nums[lo];                          // lo == hi → the minimum
}
}

// ---------- #32  LC 875  Koko Eating Bananas  (Medium)
// Story: Binary search on the ANSWER: guess a speed and ask "can I finish in h hours?" Yes → try slower. No → speed up.

// [Naive]  Time O(max·n)  ·  Space O(1)
//   1. try speeds 1, 2, 3… until total hours ≤ h
namespace lc875_naive {
int minEatingSpeed(vector<int>& piles, int h) {
    int mx = *max_element(piles.begin(), piles.end());
    for (int k = 1; k <= mx; k++) {
        long long hrs = 0;
        // ceil(p / k) hours for each pile
        for (int p : piles) hrs += (p + (long long)k - 1) / k;
        if (hrs <= h) return k;               // first speed that works
    }
    return mx;
}
}

// [Best]  Time O(n log max)  ·  Space O(1)
//   1. lo = 1, hi = max pile
//   2. feasible(mid) → hi = mid, else lo = mid + 1
namespace lc875_best {
int minEatingSpeed(vector<int>& piles, int h) {
    // answer lies in [1, biggest pile]: faster than that never helps
    int lo = 1, hi = *max_element(piles.begin(), piles.end());
    while (lo < hi) {
        int k = lo + (hi - lo) / 2;           // guess a speed
        long long hrs = 0;
        for (int p : piles) hrs += (p + (long long)k - 1) / k;   // hours needed
        if (hrs <= h) hi = k;                 // fast enough → try slower (keep k)
        else lo = k + 1;                      // too slow → must speed up
    }
    return lo;                                // smallest speed that works
}
}

// ---------- #33  LC 1011  Capacity To Ship Packages Within D Days  (Medium) [Logistics]
// Story: Truck-size guessing game: smallest possible truck = heaviest parcel, biggest = all parcels together. Binary-search the smallest truck that finishes in D days.

// [Naive]  Time O(sum·n)  ·  Space O(1)
//   1. try every capacity from max(w) up to sum(w)
namespace lc1011_naive {
int shipWithinDays(vector<int>& w, int days) {
    int lo = *max_element(w.begin(), w.end());      // must fit the heaviest parcel
    int hi = accumulate(w.begin(), w.end(), 0);     // ship everything in one day
    for (int cap = lo; cap <= hi; cap++) {
        int d = 1, load = 0;
        for (int x : w) {
            if (load + x > cap) { d++; load = 0; }  // truck full → next day
            load += x;
        }
        if (d <= days) return cap;            // first capacity that works
    }
    return hi;
}
}

// [Best]  Time O(n log sum)  ·  Space O(1)
//   1. binary search capacity in [max, sum]
//   2. ok(cap): greedily fill the truck, count days
namespace lc1011_best {
int shipWithinDays(vector<int>& w, int days) {
    int lo = *max_element(w.begin(), w.end());
    int hi = accumulate(w.begin(), w.end(), 0);
    // can a truck of this size ship everything in time? (fill greedily, in order)
    auto ok = [&](int cap) {
        int d = 1, load = 0;
        for (int x : w) {
            if (load + x > cap) { d++; load = 0; }
            load += x;
        }
        return d <= days;
    };
    while (lo < hi) {
        int m = lo + (hi - lo) / 2;
        if (ok(m)) hi = m;                    // works → try a smaller truck
        else lo = m + 1;                      // too small → bigger truck
    }
    return lo;
}
}

// ---------- #34  LC 162  Find Peak Element  (Medium)
// Story: A hiker in fog: if the path goes UP to the right, a peak must exist on the right. Keep walking uphill, halving the trail each time.

// [Naive]  Time O(n)  ·  Space O(1)
//   1. first i with nums[i] > nums[i+1] is a peak
namespace lc162_naive {
int findPeakElement(vector<int>& nums) {
    for (int i = 0; i + 1 < (int)nums.size(); i++)
        if (nums[i] > nums[i+1]) return i;    // first time the path goes down
    return nums.size() - 1;                   // always uphill → last element
}
}

// [Best]  Time O(log n)  ·  Space O(1)
//   1. nums[m] < nums[m+1] → uphill on right: lo = m+1, else hi = m
namespace lc162_best {
int findPeakElement(vector<int>& nums) {
    int lo = 0, hi = nums.size() - 1;
    while (lo < hi) {
        int m = lo + (hi - lo) / 2;
        // uphill to the right → a peak must exist on the right
        if (nums[m] < nums[m+1]) lo = m + 1;
        else hi = m;                          // downhill → m or something left is a peak
    }
    return lo;
}
}

// ======================================================================
// LINKED LIST
// ======================================================================

// ---------- #35  LC 206  Reverse Linked List  (Easy)
// Story: Train coaches: walk forward and flip each coupling to point backward. You need three hands: prev, cur, next.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. copy values to a vector, write them back reversed
namespace lc206_naive {
ListNode* reverseList(ListNode* head) {
    vector<int> v;
    for (auto p = head; p; p = p->next) v.push_back(p->val);   // copy values
    auto p = head;
    // write them back in reverse order (nodes stay, values move)
    for (int i = (int)v.size() - 1; i >= 0; i--) { p->val = v[i]; p = p->next; }
    return head;
}
}

// [Optimised]  Time O(n)  ·  Space O(n) call stack
//   1. reverse the rest, then make head->next point back to head
namespace lc206_optimised {
ListNode* reverseList(ListNode* head) {
    if (!head || !head->next) return head;    // 0 or 1 node: already reversed
    ListNode* nh = reverseList(head->next);   // reverse everything after head
    head->next->next = head;                  // the node after head now points back
    head->next = nullptr;                     // head becomes the tail
    return nh;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. save next, flip cur->next = prev, step prev and cur forward
namespace lc206_best {
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;                 // already-reversed part
    while (head) {
        ListNode* nxt = head->next;           // 1. save the rest of the train
        head->next = prev;                    // 2. flip this coupling
        prev = head;                          // 3. step prev forward
        head = nxt;                           // 4. step cur forward
    }
    return prev;                              // new head
}
}

// ---------- #36  LC 21  Merge Two Sorted Lists  (Easy)
// Story: A zipper: two sorted chains, always pull the smaller tooth next. A dummy head is your zipper pull.

// [Naive]  Time O((n+m) log(n+m))  ·  Space O(n+m)
//   1. dump all values, sort, build a new list
namespace lc21_naive {
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    vector<int> v;
    for (; a; a = a->next) v.push_back(a->val);
    for (; b; b = b->next) v.push_back(b->val);
    sort(v.begin(), v.end());
    ListNode dummy(0);
    ListNode* t = &dummy;
    for (int x : v) { t->next = new ListNode(x); t = t->next; }   // build a fresh list
    return dummy.next;
}
}

// [Optimised]  Time O(n+m)  ·  Space O(n+m) call stack
//   1. smaller head wins; its next = merge(rest)
namespace lc21_optimised {
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    if (!a) return b;                         // one list is empty → the other is the answer
    if (!b) return a;
    if (a->val <= b->val) {
        a->next = mergeTwoLists(a->next, b);  // a wins, merge the rest behind it
        return a;
    }
    b->next = mergeTwoLists(a, b->next);
    return b;
}
}

// [Best]  Time O(n+m)  ·  Space O(1)
//   1. dummy + tail pointer; attach the smaller node each step
//   2. attach whatever is left over
namespace lc21_best {
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);                        // fake head → no special case for the first node
    ListNode* t = &dummy;                     // tail of the merged list
    while (a && b) {
        // pull the smaller tooth
        if (a->val <= b->val) { t->next = a; a = a->next; }
        else { t->next = b; b = b->next; }
        t = t->next;
    }
    t->next = a ? a : b;                      // attach what's left (already sorted)
    return dummy.next;
}
}

// ---------- #37  LC 141  Linked List Cycle  (Easy)
// Story: Tortoise and hare on a race track: if the track is a loop, the fast hare eventually laps the slow tortoise.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. remember every visited node in a hash set
namespace lc141_naive {
bool hasCycle(ListNode* head) {
    unordered_set<ListNode*> seen;            // node addresses we've visited
    for (; head; head = head->next) {
        if (seen.count(head)) return true;    // been here before → loop
        seen.insert(head);
    }
    return false;                             // reached the end → no loop
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. slow moves 1, fast moves 2; they meet → cycle
namespace lc141_best {
bool hasCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {              // fast hits null → the track ends
        slow = slow->next;                    // tortoise: 1 step
        fast = fast->next->next;              // hare: 2 steps
        if (slow == fast) return true;        // hare lapped the tortoise
    }
    return false;
}
}

// ---------- #38  LC 19  Remove Nth Node From End of List  (Medium)
// Story: Two runners tied with an n-step rope. When the front runner crosses the finish line, the back runner is standing just before the victim.

// [Naive]  Time O(n) two passes  ·  Space O(1)
//   1. count length L, walk L−n steps from a dummy, unlink next
namespace lc19_naive {
ListNode* removeNthFromEnd(ListNode* head, int n) {
    int len = 0;
    for (auto p = head; p; p = p->next) len++;      // pass 1: length
    ListNode dummy(0, head);                        // handles removing the head
    ListNode* p = &dummy;
    for (int i = 0; i < len - n; i++) p = p->next;  // pass 2: stop before the victim
    p->next = p->next->next;                        // unlink it
    return dummy.next;
}
}

// [Best]  Time O(n) one pass  ·  Space O(1)
//   1. move fast n+1 steps ahead from the dummy
//   2. move both until fast is null; unlink slow->next
namespace lc19_best {
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode *fast = &dummy, *slow = &dummy;
    for (int i = 0; i <= n; i++) fast = fast->next; // gap of n+1 between them
    while (fast) {                                  // move together until fast falls off
        fast = fast->next;
        slow = slow->next;
    }
    slow->next = slow->next->next;                  // slow is just before the victim
    return dummy.next;
}
}

// ---------- #39  LC 146  LRU Cache  (Medium)
// Story: A party line: whoever you talk to moves to the FRONT. When the room is full, the person at the very back (least recently talked to) leaves. The hash map is the guest list for O(1) lookup.

// [Naive]  Time O(n) per op  ·  Space O(n)
//   1. vector of (key, value), front = most recent
//   2. linear search; move to front; pop back when over capacity
namespace lc146_naive {
class LRUCache {
    int cap;
    vector<pair<int,int>> v;                  // front = most recently used
public:
    LRUCache(int capacity) : cap(capacity) {}
    int get(int key) {
        for (int i = 0; i < (int)v.size(); i++)
            if (v[i].first == key) {
                auto p = v[i];
                v.erase(v.begin() + i);       // take it out…
                v.insert(v.begin(), p);       // …and move it to the front
                return p.second;
            }
        return -1;
    }
    void put(int key, int value) {
        // remove the old entry if the key exists
        for (int i = 0; i < (int)v.size(); i++)
            if (v[i].first == key) { v.erase(v.begin() + i); break; }
        v.insert(v.begin(), {key, value});    // newest goes to the front
        if ((int)v.size() > cap) v.pop_back();   // evict the least recent
    }
};
}

// [Best]  Time O(1) per op  ·  Space O(cap)
//   1. doubly linked list (order) + hash map key → list node
//   2. get/put: splice node to front; evict from back
namespace lc146_best {
class LRUCache {
    int cap;
    list<pair<int,int>> dll;                  // (key, value), front = most recent
    unordered_map<int, list<pair<int,int>>::iterator> mp;   // key → its node in dll
public:
    LRUCache(int capacity) : cap(capacity) {}
    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) return -1;
        dll.splice(dll.begin(), dll, it->second);   // move node to front in O(1)
        return it->second->second;
    }
    void put(int key, int value) {
        auto it = mp.find(key);
        if (it != mp.end()) {                 // existing key: update and mark as recent
            it->second->second = value;
            dll.splice(dll.begin(), dll, it->second);
            return;
        }
        dll.push_front({key, value});         // new key at the front
        mp[key] = dll.begin();
        if ((int)dll.size() > cap) {          // over capacity → evict from the back
            mp.erase(dll.back().first);
            dll.pop_back();
        }
    }
};
}

// ======================================================================
// TREES
// ======================================================================

// ---------- #40  LC 104  Maximum Depth of Binary Tree  (Easy)
// Story: Ask each child "how tall are you?", take the taller answer and add 1 for yourself. Recursion is just delegating to your children.

// [Recursive DFS]  Time O(n)  ·  Space O(h)
//   1. depth = 1 + max(depth(left), depth(right)); null → 0
namespace lc104_recursive_dfs {
int maxDepth(TreeNode* root) {
    if (!root) return 0;                      // empty tree has depth 0
    // ask both children, take the taller, add myself
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
}

// [Iterative BFS]  Time O(n)  ·  Space O(width)
//   1. process the queue one level at a time; count levels
namespace lc104_iterative_bfs {
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    queue<TreeNode*> q;
    q.push(root);
    int d = 0;
    while (!q.empty()) {
        d++;                                  // starting a new floor
        // pop exactly the nodes on this floor
        for (int s = q.size(); s > 0; s--) {
            auto n = q.front(); q.pop();
            if (n->left) q.push(n->left);     // children belong to the next floor
            if (n->right) q.push(n->right);
        }
    }
    return d;
}
}

// ---------- #41  LC 226  Invert Binary Tree  (Easy)
// Story: Mirror selfie: at every node swap the left and right hands, then tell both children to do the same.

// [Recursive DFS]  Time O(n)  ·  Space O(h)
//   1. swap(left, right), recurse on both
namespace lc226_recursive_dfs {
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;                // nothing to mirror
    swap(root->left, root->right);            // swap my two hands
    invertTree(root->left);                   // children do the same
    invertTree(root->right);
    return root;
}
}

// [Iterative BFS]  Time O(n)  ·  Space O(width)
//   1. queue; pop node, swap its children, push them
namespace lc226_iterative_bfs {
TreeNode* invertTree(TreeNode* root) {
    queue<TreeNode*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        auto n = q.front(); q.pop();
        swap(n->left, n->right);              // mirror this node
        if (n->left) q.push(n->left);         // then visit its children
        if (n->right) q.push(n->right);
    }
    return root;
}
}

// ---------- #42  LC 543  Diameter of Binary Tree  (Easy)
// Story: The longest road through any town = deepest road going down-left + deepest road going down-right. Every node is a possible "bend" in the road.

// [Naive]  Time O(n²)  ·  Space O(h)
//   1. at every node: height(left) + height(right), recompute heights each time
namespace lc543_naive {
int height(TreeNode* r) {
    return r ? 1 + max(height(r->left), height(r->right)) : 0;
}
int diameterOfBinaryTree(TreeNode* root) {
    if (!root) return 0;
    // longest road that bends at root (counted in edges)
    int through = height(root->left) + height(root->right);
    // or the best road lies entirely in one subtree
    return max({through, diameterOfBinaryTree(root->left), diameterOfBinaryTree(root->right)});
}
}

// [Best]  Time O(n)  ·  Space O(h)
//   1. one DFS returns height; on the way, best = max(best, L + R)
namespace lc543_best {
int diameterOfBinaryTree(TreeNode* root) {
    int best = 0;                             // longest road found anywhere
    // returns height; updates best as a side effect
    function<int(TreeNode*)> h = [&](TreeNode* r) -> int {
        if (!r) return 0;
        int L = h(r->left), R = h(r->right);
        best = max(best, L + R);              // road that bends at r
        return 1 + max(L, R);                 // what my parent needs
    };
    h(root);
    return best;
}
}

// ---------- #43  LC 102  Binary Tree Level Order Traversal  (Medium)
// Story: Floors of a building: before you start a floor, count how many people are in the queue (snapshot the size) and serve exactly that many.

// [DFS with depth]  Time O(n)  ·  Space O(h)
//   1. dfs(node, depth): res[depth].push_back(val)
namespace lc102_dfs_with_depth {
void dfs(TreeNode* r, int d, vector<vector<int>>& res) {
    if (!r) return;
    if ((int)res.size() == d) res.push_back({});   // first visit to floor d
    res[d].push_back(r->val);                 // left-to-right order is preserved
    dfs(r->left, d + 1, res);
    dfs(r->right, d + 1, res);
}
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    dfs(root, 0, res);
    return res;
}
}

// [BFS (standard)]  Time O(n)  ·  Space O(width)
//   1. size = q.size(); pop size nodes into one level; push children
namespace lc102_bfs_standard {
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    queue<TreeNode*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        vector<int> lvl;
        // snapshot: q.size() = number of people on this floor
        for (int s = q.size(); s > 0; s--) {
            auto n = q.front(); q.pop();
            lvl.push_back(n->val);
            if (n->left) q.push(n->left);     // next floor joins the back of the queue
            if (n->right) q.push(n->right);
        }
        res.push_back(lvl);
    }
    return res;
}
}

// ---------- #44  LC 98  Validate Binary Search Tree  (Medium)
// Story: Every node carries a "permitted range" ticket handed down by its ancestors. Go left → the ceiling drops to me. Go right → the floor rises to me.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. in-order traversal into an array
//   2. must be strictly increasing
namespace lc98_naive {
void inorder(TreeNode* r, vector<int>& v) {
    if (!r) return;
    inorder(r->left, v);                      // left subtree first
    v.push_back(r->val);                      // then me
    inorder(r->right, v);                     // then right subtree
}
bool isValidBST(TreeNode* root) {
    vector<int> v;
    inorder(root, v);
    // a valid BST reads strictly increasing in-order
    for (int i = 1; i < (int)v.size(); i++) if (v[i] <= v[i-1]) return false;
    return true;
}
}

// [Best]  Time O(n)  ·  Space O(h)
//   1. ok(node, lo, hi): lo < val < hi
//   2. left gets (lo, val), right gets (val, hi)
namespace lc98_best {
bool ok(TreeNode* r, long long lo, long long hi) {
    if (!r) return true;
    if (r->val <= lo || r->val >= hi) return false;   // outside my ticket's range
    // going left lowers the ceiling to my value; going right raises the floor
    return ok(r->left, lo, r->val) && ok(r->right, r->val, hi);
}
bool isValidBST(TreeNode* root) {
    return ok(root, LLONG_MIN, LLONG_MAX);    // long long so INT_MIN/INT_MAX values still fit
}
}

// ---------- #45  LC 236  Lowest Common Ancestor of a Binary Tree  (Medium)
// Story: Family reunion: each branch reports back "I found p" or "I found q". The first person who hears from BOTH sides is the common ancestor.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. find root→p path and root→q path
//   2. last common node on both paths
namespace lc236_naive {
bool path(TreeNode* r, TreeNode* t, vector<TreeNode*>& p) {
    if (!r) return false;
    p.push_back(r);                           // tentatively on the path
    if (r == t || path(r->left, t, p) || path(r->right, t, p)) return true;
    p.pop_back();                             // target not below me → step back
    return false;
}
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    vector<TreeNode*> a, b;
    path(root, p, a);                         // root → p
    path(root, q, b);                         // root → q
    TreeNode* res = nullptr;
    // walk both paths together; the last shared node is the LCA
    for (int i = 0; i < (int)min(a.size(), b.size()) && a[i] == b[i]; i++) res = a[i];
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(h)
//   1. null or p or q → return it
//   2. both sides non-null → I am the LCA; else pass up the non-null side
namespace lc236_best {
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // empty, or I am p or q → report myself upward
    if (!root || root == p || root == q) return root;
    TreeNode* L = lowestCommonAncestor(root->left, p, q);    // what the left branch found
    TreeNode* R = lowestCommonAncestor(root->right, p, q);   // what the right branch found
    if (L && R) return root;                  // heard from BOTH sides → I'm the LCA
    return L ? L : R;                         // pass up whichever side found something
}
}

// ---------- #46  LC 230  Kth Smallest Element in a BST  (Medium)
// Story: An in-order walk of a BST reads the numbers in sorted order, like reading a bookshelf left to right. Stop at the k-th book.

// [Naive]  Time O(n)  ·  Space O(n)
//   1. full in-order into a vector, return v[k−1]
namespace lc230_naive {
void ino(TreeNode* r, vector<int>& v) {
    if (!r) return;
    ino(r->left, v);
    v.push_back(r->val);                      // values come out sorted
    ino(r->right, v);
}
int kthSmallest(TreeNode* root, int k) {
    vector<int> v;
    ino(root, v);
    return v[k-1];                            // k is 1-indexed
}
}

// [Best]  Time O(h + k)  ·  Space O(h)
//   1. iterative in-order with a stack
//   2. count down k as you pop; stop at 0
namespace lc230_best {
int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st;
    TreeNode* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }   // go as far left as possible
        cur = st.top(); st.pop();             // next smallest value
        if (--k == 0) return cur->val;        // that's the k-th book
        cur = cur->right;                     // continue with the right subtree
    }
    return -1;
}
}

// ---------- #47  LC 199  Binary Tree Right Side View  (Medium)
// Story: Stand to the right of the building and look in: on each floor you only see the LAST person in that row.

// [BFS]  Time O(n)  ·  Space O(width)
//   1. level-order; record the last node of each level
namespace lc199_bfs {
vector<int> rightSideView(TreeNode* root) {
    vector<int> res;
    queue<TreeNode*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        int s = q.size();                     // people on this floor
        for (int i = 0; i < s; i++) {
            auto n = q.front(); q.pop();
            if (i == s - 1) res.push_back(n->val);   // last one = visible from the right
            if (n->left) q.push(n->left);
            if (n->right) q.push(n->right);
        }
    }
    return res;
}
}

// [DFS right-first]  Time O(n)  ·  Space O(h)
//   1. visit right before left; first node seen at each new depth is visible
namespace lc199_dfs_right_first {
void dfs(TreeNode* r, int d, vector<int>& res) {
    if (!r) return;
    // first node we reach on floor d is the rightmost one (we go right first)
    if (d == (int)res.size()) res.push_back(r->val);
    dfs(r->right, d + 1, res);
    dfs(r->left, d + 1, res);
}
vector<int> rightSideView(TreeNode* root) {
    vector<int> res;
    dfs(root, 0, res);
    return res;
}
}

// ======================================================================
// HEAP / PRIORITY QUEUE
// ======================================================================

// ---------- #48  LC 215  Kth Largest Element in an Array  (Medium)
// Story: A VIP lounge with only k seats and a bouncer who kicks out the weakest guest whenever it overflows. At closing time, the weakest VIP is the k-th largest.

// [Naive]  Time O(n log n)  ·  Space O(1)
//   1. sort descending, return nums[k−1]
namespace lc215_naive {
int findKthLargest(vector<int>& nums, int k) {
    sort(nums.rbegin(), nums.rend());         // largest first
    return nums[k-1];
}
}

// [Optimised]  Time O(n log k)  ·  Space O(k)
//   1. min-heap; push each; pop when size > k; top is the answer
namespace lc215_optimised {
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;   // min-heap = the lounge
    for (int x : nums) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();     // over capacity → kick out the weakest
    }
    return pq.top();                          // weakest of the k largest = k-th largest
}
}

// [Best]  Time O(n) average  ·  Space O(1)
//   1. quickselect: random pivot, 3-way partition
//   2. recurse only into the side that holds index n−k
namespace lc215_best {
int findKthLargest(vector<int>& nums, int k) {
    int target = nums.size() - k;             // k-th largest = this index after sorting ascending
    int lo = 0, hi = nums.size() - 1;
    while (true) {
        int pivot = nums[lo + rand() % (hi - lo + 1)];   // random pivot avoids worst cases
        // 3-way partition: [lo, lt) < pivot, [lt, gt] == pivot, (gt, hi] > pivot
        int lt = lo, i = lo, gt = hi;
        while (i <= gt) {
            if (nums[i] < pivot) swap(nums[lt++], nums[i++]);
            else if (nums[i] > pivot) swap(nums[i], nums[gt--]);
            else i++;
        }
        if (target < lt) hi = lt - 1;         // answer is in the "smaller" part
        else if (target > gt) lo = gt + 1;    // answer is in the "bigger" part
        else return nums[target];             // target landed in the pivot block
    }
}
}

// ---------- #49  LC 973  K Closest Points to Origin  (Medium) [Logistics]
// Story: Keep a max-heap of your k nearest riders. When a closer rider shows up, the farthest of your k gets dropped.

// [Naive]  Time O(n log n)  ·  Space O(1)
//   1. sort by x² + y², take first k
namespace lc973_naive {
vector<vector<int>> kClosest(vector<vector<int>>& pts, int k) {
    // compare squared distances (no sqrt needed: same order)
    sort(pts.begin(), pts.end(), [](auto& a, auto& b) {
        return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
    });
    return vector<vector<int>>(pts.begin(), pts.begin() + k);
}
}

// [Optimised]  Time O(n log k)  ·  Space O(k)
//   1. max-heap of (dist, index), size ≤ k
namespace lc973_optimised {
vector<vector<int>> kClosest(vector<vector<int>>& pts, int k) {
    priority_queue<pair<int,int>> pq;         // max-heap: farthest of my k on top
    for (int i = 0; i < (int)pts.size(); i++) {
        pq.push({pts[i][0]*pts[i][0] + pts[i][1]*pts[i][1], i});
        if ((int)pq.size() > k) pq.pop();     // drop the farthest
    }
    vector<vector<int>> res;
    while (!pq.empty()) { res.push_back(pts[pq.top().second]); pq.pop(); }
    return res;
}
}

// [Best]  Time O(n) average  ·  Space O(1)
//   1. quickselect (nth_element) by distance, take first k
namespace lc973_best {
vector<vector<int>> kClosest(vector<vector<int>>& pts, int k) {
    auto d = [](const vector<int>& p) { return p[0]*p[0] + p[1]*p[1]; };
    // put the k nearest in the first k slots (any order) in average O(n)
    nth_element(pts.begin(), pts.begin() + (k - 1), pts.end(),
                [&](auto& a, auto& b) { return d(a) < d(b); });
    return vector<vector<int>>(pts.begin(), pts.begin() + k);
}
}

// ---------- #50  LC 23  Merge k Sorted Lists  (Hard)
// Story: k checkout counters, one exit door. A min-heap always tells you whose front customer walks out next.

// [Naive]  Time O(N log N)  ·  Space O(N)
//   1. collect every value, sort, build a list
namespace lc23_naive {
ListNode* mergeKLists(vector<ListNode*>& lists) {
    vector<int> v;
    for (auto l : lists)
        for (; l; l = l->next) v.push_back(l->val);   // dump everything
    sort(v.begin(), v.end());
    ListNode dummy(0);
    ListNode* t = &dummy;
    for (int x : v) { t->next = new ListNode(x); t = t->next; }
    return dummy.next;
}
}

// [Optimised]  Time O(N log k)  ·  Space O(k)
//   1. push every list head into a min-heap
//   2. pop smallest, attach, push its next
namespace lc23_optimised {
ListNode* mergeKLists(vector<ListNode*>& lists) {
    // min-heap of nodes by value
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    for (auto l : lists) if (l) pq.push(l);   // front customer of every counter
    ListNode dummy(0);
    ListNode* t = &dummy;
    while (!pq.empty()) {
        auto n = pq.top(); pq.pop();          // smallest front customer leaves
        t->next = n; t = n;
        if (n->next) pq.push(n->next);        // that counter's next customer steps up
    }
    return dummy.next;
}
}

// [Best]  Time O(N log k)  ·  Space O(1) extra
//   1. divide & conquer: merge pairs (0,1),(2,3)… then (0,2)… like a tournament
namespace lc23_best {
ListNode* merge2(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* t = &dummy;
    while (a && b) {
        if (a->val <= b->val) { t->next = a; a = a->next; }
        else { t->next = b; b = b->next; }
        t = t->next;
    }
    t->next = a ? a : b;
    return dummy.next;
}
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    // tournament rounds: step 1 merges neighbours, step 2 merges pairs of pairs, …
    for (int step = 1; step < (int)lists.size(); step *= 2)
        for (int i = 0; i + step < (int)lists.size(); i += 2 * step)
            lists[i] = merge2(lists[i], lists[i + step]);
    return lists[0];                          // the winner holds everything
}
}

// ---------- #51  LC 621  Task Scheduler  (Medium)
// Story: The busiest task builds the skeleton: (maxFreq − 1) frames of width (n + 1), plus one slot for every task tied at the max. If the other tasks overflow the frames, there's no idle time.

// [Simulation]  Time O(T log 26)  ·  Space O(26)
//   1. max-heap of remaining counts + cooldown queue (count, readyTime)
//   2. each tick: run the biggest task, park it; release tasks whose cooldown ended
namespace lc621_simulation {
int leastInterval(vector<char>& tasks, int n) {
    int f[26] = {0};
    for (char c : tasks) f[c - 'A']++;
    priority_queue<int> pq;                   // remaining counts of tasks ready to run
    for (int x : f) if (x) pq.push(x);
    queue<pair<int,int>> cool;                // (remaining count, time it becomes ready)
    int time = 0;
    while (!pq.empty() || !cool.empty()) {
        time++;                               // one CPU tick
        if (!pq.empty()) {
            int c = pq.top() - 1; pq.pop();   // run the task with most copies left
            if (c) cool.push({c, time + n});  // it must rest for n ticks
        }
        // a resting task is ready again
        if (!cool.empty() && cool.front().second == time) {
            pq.push(cool.front().first);
            cool.pop();
        }
    }
    return time;
}
}

// [Best]  Time O(T)  ·  Space O(26)
//   1. answer = max(total tasks, (maxFreq − 1) × (n + 1) + countOfMax)
namespace lc621_best {
int leastInterval(vector<char>& tasks, int n) {
    int f[26] = {0}, mx = 0, cntMax = 0;
    for (char c : tasks) mx = max(mx, ++f[c - 'A']);   // busiest task count
    for (int x : f) if (x == mx) cntMax++;              // how many tasks tie for busiest
    // skeleton: (mx − 1) frames of width n + 1, plus the final row of busiest tasks;
    // if there are more tasks than slots, no idle is needed at all
    return max((int)tasks.size(), (mx - 1) * (n + 1) + cntMax);
}
}

// ---------- #52  LC 295  Find Median from Data Stream  (Hard)
// Story: Two sand piles: the left pile is a MAX-heap (small half), the right pile a MIN-heap (big half). Keep them the same size and the median sits on top.

// [Naive]  Time add O(n)  ·  Space O(n)
//   1. keep a sorted vector; insert at upper_bound
namespace lc295_naive {
class MedianFinder {
    vector<int> v;                            // always sorted
public:
    void addNum(int x) {
        v.insert(upper_bound(v.begin(), v.end(), x), x);   // O(log n) find, O(n) shift
    }
    double findMedian() {
        int n = v.size();
        return n % 2 ? v[n/2] : (v[n/2 - 1] + (double)v[n/2]) / 2.0;
    }
};
}

// [Best]  Time add O(log n), median O(1)  ·  Space O(n)
//   1. push to lo (max-heap), move lo.top to hi (min-heap)
//   2. if hi bigger, move hi.top back to lo
namespace lc295_best {
class MedianFinder {
    priority_queue<int> lo;                               // small half, biggest on top
    priority_queue<int, vector<int>, greater<int>> hi;    // big half, smallest on top
public:
    void addNum(int x) {
        lo.push(x);
        hi.push(lo.top()); lo.pop();          // biggest of the small half moves right
        // keep lo the same size as hi, or one bigger
        if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
    }
    double findMedian() {
        // odd count → extra element sits on top of lo; even → average the two tops
        return lo.size() > hi.size() ? lo.top() : (lo.top() + (double)hi.top()) / 2.0;
    }
};
}

// ======================================================================
// INTERVALS
// ======================================================================

// ---------- #53  LC 56  Merge Intervals  (Medium) [Logistics]
// Story: Delivery slots on a timeline: sort by start time. If the next slot starts before the current one ends, just stretch the current one.

// [Naive]  Time O(n³)  ·  Space O(n)
//   1. keep merging any overlapping pair until no pair overlaps
namespace lc56_naive {
vector<vector<int>> merge(vector<vector<int>>& in) {
    vector<vector<int>> v = in;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < (int)v.size() && !changed; i++)
            for (int j = i + 1; j < (int)v.size() && !changed; j++)
                // two slots overlap if each starts before the other ends
                if (v[i][0] <= v[j][1] && v[j][0] <= v[i][1]) {
                    v[i] = {min(v[i][0], v[j][0]), max(v[i][1], v[j][1])};
                    v.erase(v.begin() + j);
                    changed = true;           // restart the scan
                }
    }
    sort(v.begin(), v.end());
    return v;
}
}

// [Best]  Time O(n log n)  ·  Space O(n)
//   1. sort by start
//   2. overlap with last merged → extend its end, else append
namespace lc56_best {
vector<vector<int>> merge(vector<vector<int>>& in) {
    sort(in.begin(), in.end());               // by start time
    vector<vector<int>> res;
    for (auto& iv : in) {
        // no overlap with the last merged slot → start a new one
        if (res.empty() || res.back()[1] < iv[0]) res.push_back(iv);
        // overlap → stretch the end
        else res.back()[1] = max(res.back()[1], iv[1]);
    }
    return res;
}
}

// ---------- #54  LC 57  Insert Interval  (Medium)
// Story: Three zones on the calendar: slots fully BEFORE the new one (copy them), slots that OVERLAP it (swallow them), slots fully AFTER it (copy them).

// [Naive]  Time O(n log n)  ·  Space O(n)
//   1. append the new interval, sort, run Merge Intervals
namespace lc57_naive {
vector<vector<int>> insert(vector<vector<int>>& in, vector<int>& nw) {
    vector<vector<int>> v = in;
    v.push_back(nw);                          // drop the new slot in anywhere
    sort(v.begin(), v.end());                 // re-sort by start
    vector<vector<int>> res;
    // then it's exactly Merge Intervals
    for (auto& iv : v) {
        if (res.empty() || res.back()[1] < iv[0]) res.push_back(iv);   // no overlap → new slot
        else res.back()[1] = max(res.back()[1], iv[1]);                 // overlap → stretch the end
    }
    return res;
}
}

// [Best]  Time O(n)  ·  Space O(n)
//   1. copy all that end before new starts
//   2. swallow all that start before new ends (min start, max end); copy the rest
namespace lc57_best {
vector<vector<int>> insert(vector<vector<int>>& in, vector<int>& nw) {
    vector<vector<int>> res;
    int i = 0, n = in.size();
    // zone 1: ends before the new slot starts → copy as is
    while (i < n && in[i][1] < nw[0]) res.push_back(in[i++]);
    // zone 2: starts before the new slot ends → swallow into it
    vector<int> cur = nw;
    while (i < n && in[i][0] <= cur[1]) {
        cur[0] = min(cur[0], in[i][0]);
        cur[1] = max(cur[1], in[i][1]);
        i++;
    }
    res.push_back(cur);
    // zone 3: everything after → copy as is
    while (i < n) res.push_back(in[i++]);
    return res;
}
}

// ---------- #55  LC 253  Meeting Rooms II  (Medium) [Logistics]
// Story: Rooms = delivery riders. A min-heap holds "when does each rider become free". The earliest-free rider is done before the new order starts? Reuse him. Otherwise hire a new rider.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. at each start time, count intervals covering it; answer = max count
namespace lc253_naive {
int minMeetingRooms(vector<vector<int>>& in) {
    int best = 0;
    for (auto& a : in) {
        int c = 0;
        // how many meetings are running at the moment a starts?
        for (auto& b : in) if (b[0] <= a[0] && a[0] < b[1]) c++;
        best = max(best, c);
    }
    return best;
}
}

// [Optimised]  Time O(n log n)  ·  Space O(n)
//   1. sort by start; min-heap of end times
//   2. earliest end ≤ start → pop (reuse); push this end; answer = heap size
namespace lc253_optimised {
int minMeetingRooms(vector<vector<int>>& in) {
    sort(in.begin(), in.end());               // handle orders by start time
    priority_queue<int, vector<int>, greater<int>> ends;   // when each rider is free
    for (auto& iv : in) {
        // earliest-free rider is done before this order starts → reuse him
        if (!ends.empty() && ends.top() <= iv[0]) ends.pop();
        ends.push(iv[1]);                     // this rider is busy until iv end
    }
    return ends.size();                       // riders ever needed at once
}
}

// [Best]  Time O(n log n)  ·  Space O(n)
//   1. sort starts and ends separately
//   2. sweep: a start → rooms++, any end ≤ that start → rooms−−
namespace lc253_best {
int minMeetingRooms(vector<vector<int>>& in) {
    vector<int> s, e;
    for (auto& iv : in) { s.push_back(iv[0]); e.push_back(iv[1]); }
    sort(s.begin(), s.end());
    sort(e.begin(), e.end());
    int rooms = 0, best = 0, j = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        // free every room whose meeting ended by now
        while (j < (int)e.size() && e[j] <= s[i]) { j++; rooms--; }
        rooms++;                              // this meeting takes a room
        best = max(best, rooms);
    }
    return best;
}
}

// ---------- #56  LC 435  Non-overlapping Intervals  (Medium)
// Story: Greedy party planner: always keep the meeting that ENDS earliest, because it leaves the most room for everyone after it. Count the ones you cancel.

// [Naive]  Time O(n²)  ·  Space O(n)
//   1. sort by start; LIS-style DP of the longest non-overlapping chain
//   2. answer = n − longest chain
namespace lc435_naive {
int eraseOverlapIntervals(vector<vector<int>>& in) {
    int n = in.size();
    if (!n) return 0;
    sort(in.begin(), in.end());
    vector<int> dp(n, 1);                     // dp[i] = longest chain ending with interval i
    int keep = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++)
            if (in[j][1] <= in[i][0]) dp[i] = max(dp[i], dp[j] + 1);   // j fits before i
        keep = max(keep, dp[i]);
    }
    return n - keep;                          // everything not kept is removed
}
}

// [Best]  Time O(n log n)  ·  Space O(1)
//   1. sort by END
//   2. start ≥ last kept end → keep it, else remove it
namespace lc435_best {
int eraseOverlapIntervals(vector<vector<int>>& in) {
    // earliest-ending first leaves the most room for the rest
    sort(in.begin(), in.end(), [](auto& a, auto& b) { return a[1] < b[1]; });
    int removed = 0, end = INT_MIN;           // end of the last kept interval
    for (auto& iv : in) {
        if (iv[0] >= end) end = iv[1];        // fits → keep it
        else removed++;                       // overlaps → cancel it
    }
    return removed;
}
}

// ======================================================================
// GRAPHS
// ======================================================================

// ---------- #57  LC 200  Number of Islands  (Medium)
// Story: Fly over the sea with a paint bucket: land on any unpainted land, flood-paint the whole island, count +1. Repeat.

// [DFS flood fill]  Time O(m·n)  ·  Space O(m·n) stack
//   1. scan cells; on '1' → count++, DFS sinks the island to '0'
namespace lc200_dfs_flood_fill {
void dfs(vector<vector<char>>& g, int r, int c) {
    // stop at the border, at water, or at land we've already painted
    if (r < 0 || c < 0 || r >= (int)g.size() || c >= (int)g[0].size() || g[r][c] != '1') return;
    g[r][c] = '0';                            // paint (sink) this cell
    dfs(g, r + 1, c); dfs(g, r - 1, c);       // spread to 4 neighbours
    dfs(g, r, c + 1); dfs(g, r, c - 1);
}
int numIslands(vector<vector<char>>& g) {
    int cnt = 0;
    for (int r = 0; r < (int)g.size(); r++)
        for (int c = 0; c < (int)g[0].size(); c++)
            if (g[r][c] == '1') {             // unpainted land → a new island
                cnt++;
                dfs(g, r, c);                 // paint the whole island
            }
    return cnt;
}
}

// [BFS]  Time O(m·n)  ·  Space O(min(m,n))
//   1. same idea with a queue (no deep recursion)
namespace lc200_bfs {
int numIslands(vector<vector<char>>& g) {
    int m = g.size(), n = g[0].size(), cnt = 0;
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};   // 4 directions
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            if (g[i][j] != '1') continue;
            cnt++;                            // new island
            queue<pair<int,int>> q;
            q.push({i, j});
            g[i][j] = '0';                    // mark when pushed, so nothing is pushed twice
            while (!q.empty()) {
                auto [r, c] = q.front(); q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr >= 0 && nc >= 0 && nr < m && nc < n && g[nr][nc] == '1') {
                        g[nr][nc] = '0';
                        q.push({nr, nc});
                    }
                }
            }
        }
    return cnt;
}
}

// [Union-Find]  Time O(m·n·α)  ·  Space O(m·n)
//   1. every land cell starts as its own island (count++)
//   2. union with right/down land neighbours; each successful union → count−−
namespace lc200_union_find {
struct DSU {
    vector<int> p;                            // parent of each cell
    DSU(int n) : p(n) { iota(p.begin(), p.end(), 0); }   // everyone is their own root
    int f(int x) { return p[x] == x ? x : p[x] = f(p[x]); }   // find root + path compression
    bool u(int a, int b) {                    // merge; false if already together
        a = f(a); b = f(b);
        if (a == b) return false;
        p[a] = b;
        return true;
    }
};
int numIslands(vector<vector<char>>& g) {
    int m = g.size(), n = g[0].size(), cnt = 0;
    DSU d(m * n);                             // cell (i, j) → id i*n + j
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == '1') {
                cnt++;                        // assume it's a new island…
                // …and merge with land below / right; each real merge removes one island
                if (i + 1 < m && g[i+1][j] == '1' && d.u(i*n + j, (i+1)*n + j)) cnt--;
                if (j + 1 < n && g[i][j+1] == '1' && d.u(i*n + j, i*n + j + 1)) cnt--;
            }
    return cnt;
}
}

// ---------- #58  LC 994  Rotting Oranges  (Medium)
// Story: Multi-source BFS is a rumour spreading: every rotten orange starts gossiping at minute 0, and each minute the rumour moves one ring outward.

// [Naive]  Time O((m·n)²)  ·  Space O(m·n)
//   1. each minute scan the whole grid, rot fresh neighbours of rotten ones
//   2. stop when nothing changes
namespace lc994_naive {
int orangesRotting(vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size(), mins = 0;
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (true) {
        vector<pair<int,int>> nxt;            // oranges that rot this minute
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (g[i][j] == 2)
                    for (int d = 0; d < 4; d++) {
                        int r = i + dr[d], c = j + dc[d];
                        if (r >= 0 && c >= 0 && r < m && c < n && g[r][c] == 1) nxt.push_back({r, c});
                    }
        if (nxt.empty()) break;               // nothing new rotted → done
        for (auto [r, c] : nxt) g[r][c] = 2;  // rot them all at once
        mins++;
    }
    for (auto& row : g) for (int x : row) if (x == 1) return -1;   // someone never rots
    return mins;
}
}

// [Best]  Time O(m·n)  ·  Space O(m·n)
//   1. push ALL rotten cells first, count fresh
//   2. BFS level by level = minutes; fresh left → −1
namespace lc994_best {
int orangesRotting(vector<vector<int>>& g) {
    int m = g.size(), n = g[0].size(), fresh = 0, mins = 0;
    queue<pair<int,int>> q;
    // every rotten orange is a source at minute 0
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            if (g[i][j] == 2) q.push({i, j});
            else if (g[i][j] == 1) fresh++;
        }
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (!q.empty() && fresh > 0) {
        mins++;                               // one ring outward = one minute
        for (int s = q.size(); s > 0; s--) {
            auto [r, c] = q.front(); q.pop();
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr >= 0 && nc >= 0 && nr < m && nc < n && g[nr][nc] == 1) {
                    g[nr][nc] = 2;            // rumour reaches this orange
                    fresh--;
                    q.push({nr, nc});
                }
            }
        }
    }
    return fresh == 0 ? mins : -1;
}
}

// ---------- #59  LC 547  Number of Provinces  (Medium)
// Story: Friend circles: everybody shakes hands (union) with their direct friends. Count how many separate circles are left standing.

// [DFS]  Time O(n²)  ·  Space O(n)
//   1. for each unvisited city: count++, DFS marks its whole province
namespace lc547_dfs {
void dfs(vector<vector<int>>& M, vector<bool>& vis, int i) {
    vis[i] = true;
    for (int j = 0; j < (int)M.size(); j++)
        if (M[i][j] && !vis[j]) dfs(M, vis, j);   // visit every connected city
}
int findCircleNum(vector<vector<int>>& M) {
    int n = M.size(), c = 0;
    vector<bool> vis(n, false);
    for (int i = 0; i < n; i++)
        if (!vis[i]) {                        // not reached yet → a new province
            c++;
            dfs(M, vis, i);
        }
    return c;
}
}

// [Union-Find]  Time O(n² · α)  ·  Space O(n)
//   1. start with n components; union every connected pair
//   2. each union that merges two roots → components−−
namespace lc547_union_find {
int findCircleNum(vector<vector<int>>& M) {
    int n = M.size(), comps = n;              // everyone starts alone
    vector<int> p(n), rk(n, 0);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };   // find + compress
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (M[i][j]) {                    // i and j are friends → shake hands
                int a = f(i), b = f(j);
                if (a == b) continue;         // already in the same circle
                if (rk[a] < rk[b]) swap(a, b);   // union by rank: attach the shorter tree
                p[b] = a;
                if (rk[a] == rk[b]) rk[a]++;
                comps--;                      // two circles became one
            }
    return comps;
}
}

// ---------- #60  LC 207  Course Schedule  (Medium)
// Story: Getting dressed: socks before shoes, shirt before tie. If the rules loop (shoes before socks before shoes), it's impossible. Keep picking clothes that have no pending prerequisites.

// [DFS cycle check]  Time O(V+E)  ·  Space O(V+E)
//   1. colours: 0 = new, 1 = on current path, 2 = done
//   2. reaching a 1 again = cycle
namespace lc207_dfs_cycle_check {
bool hasCycle(int u, vector<vector<int>>& g, vector<int>& st) {
    st[u] = 1;                                // on the current path
    for (int v : g[u]) {
        if (st[v] == 1) return true;          // back to a node on my path → loop
        if (st[v] == 0 && hasCycle(v, g, st)) return true;
    }
    st[u] = 2;                                // fully explored, no loop through u
    return false;
}
bool canFinish(int n, vector<vector<int>>& pre) {
    vector<vector<int>> g(n);
    for (auto& e : pre) g[e[1]].push_back(e[0]);   // edge: prerequisite → course
    vector<int> st(n, 0);
    for (int i = 0; i < n; i++)
        if (st[i] == 0 && hasCycle(i, g, st)) return false;
    return true;
}
}

// [Kahn's BFS]  Time O(V+E)  ·  Space O(V+E)
//   1. indegree count; queue all with indegree 0
//   2. pop, "take" it, decrement neighbours; taken == n → possible
namespace lc207_kahn_s_bfs {
bool canFinish(int n, vector<vector<int>>& pre) {
    vector<vector<int>> g(n);
    vector<int> indeg(n, 0);                  // pending prerequisites per course
    for (auto& e : pre) { g[e[1]].push_back(e[0]); indeg[e[0]]++; }
    queue<int> q;
    for (int i = 0; i < n; i++) if (!indeg[i]) q.push(i);   // can take right away
    int done = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        done++;                               // take course u
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);   // v's last prerequisite is done
    }
    return done == n;                         // a loop leaves some courses never ready
}
}

// ---------- #61  LC 210  Course Schedule II  (Medium)
// Story: Same as getting dressed, but now write down the ORDER in which you picked clothes off the "no pending prerequisites" pile.

// [DFS post-order]  Time O(V+E)  ·  Space O(V+E)
//   1. DFS with colours; add a node when it finishes
//   2. reverse the finish order; a cycle → return {}
namespace lc210_dfs_post_order {
bool dfs(int u, vector<vector<int>>& g, vector<int>& st, vector<int>& order) {
    st[u] = 1;                                // on the current path
    for (int v : g[u]) {
        if (st[v] == 1) return false;         // loop → impossible
        if (st[v] == 0 && !dfs(v, g, st, order)) return false;
    }
    st[u] = 2;
    order.push_back(u);                       // u finishes after everything that depends on it
    return true;
}
vector<int> findOrder(int n, vector<vector<int>>& pre) {
    vector<vector<int>> g(n);
    for (auto& e : pre) g[e[1]].push_back(e[0]);   // prerequisite → course
    vector<int> st(n, 0), order;
    for (int i = 0; i < n; i++)
        if (st[i] == 0 && !dfs(i, g, st, order)) return {};
    reverse(order.begin(), order.end());      // reversed finish order = valid course order
    return order;
}
}

// [Kahn's BFS]  Time O(V+E)  ·  Space O(V+E)
//   1. queue of indegree-0 nodes; pop order = answer
//   2. fewer than n popped → cycle → {}
namespace lc210_kahn_s_bfs {
vector<int> findOrder(int n, vector<vector<int>>& pre) {
    vector<vector<int>> g(n);
    vector<int> indeg(n, 0), order;
    for (auto& e : pre) { g[e[1]].push_back(e[0]); indeg[e[0]]++; }
    queue<int> q;
    for (int i = 0; i < n; i++) if (!indeg[i]) q.push(i);   // ready right away
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);                   // take u now
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);   // v just became ready
    }
    if ((int)order.size() != n) return {};    // some courses stuck in a loop
    return order;
}
}

// ---------- #62  LC 743  Network Delay Time  (Medium) [Logistics]
// Story: Dijkstra is water flowing through pipes: always flood the NEAREST dry junction next (min-heap by distance). The last junction to get wet is the answer.

// [Bellman-Ford]  Time O(V·E)  ·  Space O(V)
//   1. relax every edge V−1 times
//   2. any node still ∞ → −1, else max distance
namespace lc743_bellman_ford {
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<long long> d(n + 1, LLONG_MAX / 2);   // "infinity" that won't overflow when added to
    d[k] = 0;
    // a shortest path uses at most n−1 edges, so n−1 rounds are enough
    for (int i = 1; i < n; i++)
        for (auto& e : times)                 // e = {from, to, time}
            d[e[1]] = min(d[e[1]], d[e[0]] + e[2]);
    long long mx = 0;
    for (int v = 1; v <= n; v++) mx = max(mx, d[v]);
    return mx >= LLONG_MAX / 2 ? -1 : mx;     // some node never reached → −1
}
}

// [Dijkstra]  Time O(E log V)  ·  Space O(V+E)
//   1. min-heap of (dist, node); skip stale entries
//   2. relax neighbours: d[u] + w < d[v] → update, push
namespace lc743_dijkstra {
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int,int>>> g(n + 1);   // g[u] = {(v, w)}
    for (auto& e : times) g[e[0]].push_back({e[1], e[2]});
    vector<int> d(n + 1, INT_MAX);
    d[k] = 0;
    // min-heap: (distance, node), nearest junction on top
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, k});
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du > d[u]) continue;              // stale entry: a shorter path was already found
        for (auto [v, w] : g[u])
            if (du + w < d[v]) {              // reaching v through u is faster
                d[v] = du + w;
                pq.push({d[v], v});
            }
    }
    int mx = 0;
    for (int v = 1; v <= n; v++) {
        if (d[v] == INT_MAX) return -1;       // never got wet
        mx = max(mx, d[v]);                   // the last junction to get wet
    }
    return mx;
}
}

// ---------- #63  LC 127  Word Ladder  (Hard)
// Story: Every word is a metro station; changing one letter is one stop. BFS finds the fewest stops. Bidirectional BFS sends two trains from both ends to meet in the middle.

// [Naive]  Time O(N²·L)  ·  Space O(N)
//   1. BFS; neighbours = every unused dictionary word that differs by exactly 1 letter
namespace lc127_naive {
int ladderLength(string begin, string end, vector<string>& words) {
    // do a and b differ in exactly one position?
    auto adj = [](const string& a, const string& b) {
        int d = 0;
        for (int i = 0; i < (int)a.size(); i++)
            if (a[i] != b[i] && ++d > 1) return false;
        return d == 1;
    };
    vector<bool> used(words.size(), false);
    queue<string> q;
    q.push(begin);
    for (int steps = 1; !q.empty(); steps++) {      // steps = words in the ladder so far
        for (int s = q.size(); s > 0; s--) {
            string w = q.front(); q.pop();
            if (w == end) return steps;
            // compare against every dictionary word → slow for big dictionaries
            for (int i = 0; i < (int)words.size(); i++)
                if (!used[i] && adj(w, words[i])) { used[i] = true; q.push(words[i]); }
        }
    }
    return 0;
}
}

// [Optimised]  Time O(N·L·26)  ·  Space O(N)
//   1. BFS; neighbours = try all 26 letters at each position, keep those in the set
//   2. erase from set when visited
namespace lc127_optimised {
int ladderLength(string begin, string end, vector<string>& words) {
    unordered_set<string> dict(words.begin(), words.end());
    if (!dict.count(end)) return 0;           // target not reachable at all
    queue<string> q;
    q.push(begin);
    dict.erase(begin);
    for (int steps = 1; !q.empty(); steps++) {
        for (int s = q.size(); s > 0; s--) {
            string w = q.front(); q.pop();
            if (w == end) return steps;
            for (int i = 0; i < (int)w.size(); i++) {
                char orig = w[i];
                // try every letter at position i
                for (char c = 'a'; c <= 'z'; c++) {
                    w[i] = c;
                    if (dict.count(w)) {      // a real word one stop away
                        dict.erase(w);        // erase = mark visited
                        q.push(w);
                    }
                }
                w[i] = orig;                  // restore before trying the next position
            }
        }
    }
    return 0;
}
}

// [Best]  Time O(N·L·26), far fewer nodes  ·  Space O(N)
//   1. two frontiers (from begin and from end); always expand the smaller one
//   2. a neighbour found in the other frontier → they met
namespace lc127_best {
int ladderLength(string begin, string end, vector<string>& words) {
    unordered_set<string> dict(words.begin(), words.end());
    if (!dict.count(end)) return 0;
    unordered_set<string> a{begin}, b{end};   // the two trains' current stations
    dict.erase(begin);
    dict.erase(end);
    for (int steps = 1; !a.empty() && !b.empty(); steps++) {
        if (a.size() > b.size()) swap(a, b);  // always grow the smaller frontier
        unordered_set<string> nxt;
        for (string w : a) {
            for (int i = 0; i < (int)w.size(); i++) {
                char orig = w[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    w[i] = c;
                    if (b.count(w)) return steps + 1;   // the two trains meet
                    if (dict.count(w)) { dict.erase(w); nxt.insert(w); }
                }
                w[i] = orig;
            }
        }
        a = nxt;
    }
    return 0;
}
}

// ---------- #64  LC 787  Cheapest Flights Within K Stops  (Medium) [Logistics]
// Story: Bellman-Ford with a stop budget: run exactly k + 1 rounds of "relax every route", reading only LAST round's prices so no route sneaks in two hops in one round.

// [Bellman-Ford (k+1 rounds)]  Time O(k·E)  ·  Space O(n)
//   1. d = ∞, d[src] = 0
//   2. k+1 times: nd = copy of d; relax every flight using d; d = nd
namespace lc787_bellman_ford_k_1_rounds {
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<int> d(n, INT_MAX);                // cheapest price using at most `round` flights
    d[src] = 0;
    // k stops = at most k + 1 flights → k + 1 rounds
    for (int i = 0; i <= k; i++) {
        vector<int> nd = d;                   // write into a copy…
        for (auto& f : flights)               // f = {from, to, price}
            if (d[f[0]] != INT_MAX)           // …but read last round's prices only
                nd[f[1]] = min(nd[f[1]], d[f[0]] + f[2]);
        d = nd;
    }
    return d[dst] == INT_MAX ? -1 : d[dst];
}
}

// [Dijkstra with stops]  Time O(E·k log(E·k))  ·  Space O(E·k)
//   1. heap of (cost, node, edgesUsed); first pop of dst = cheapest
//   2. skip if edgesUsed > k or we already reached node with fewer edges
namespace lc787_dijkstra_with_stops {
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<vector<pair<int,int>>> g(n);
    for (auto& f : flights) g[f[0]].push_back({f[1], f[2]});
    vector<int> best(n, INT_MAX);             // fewest flights used when a city was popped
    // min-heap by cost: {cost, city, flights used}
    priority_queue<array<int,3>, vector<array<int,3>>, greater<>> pq;
    pq.push({0, src, 0});
    while (!pq.empty()) {
        auto [c, u, s] = pq.top(); pq.pop();
        if (u == dst) return c;               // cheapest way to reach dst within the limit
        // over the stop budget, or reached u before more cheaply AND with fewer flights
        if (s > k || s >= best[u]) continue;
        best[u] = s;
        for (auto [v, w] : g[u]) pq.push({c + w, v, s + 1});
    }
    return -1;
}
}

// ======================================================================
// BACKTRACKING
// ======================================================================

// ---------- #65  LC 78  Subsets  (Medium)
// Story: At every item you reach a fork: TAKE it or SKIP it. n forks → 2ⁿ leaves → 2ⁿ subsets. Or think of n light switches and count in binary.

// [Backtracking]  Time O(n·2ⁿ)  ·  Space O(n)
//   1. bt(i): at the end, save cur
//   2. take nums[i] → bt(i+1) → undo; skip → bt(i+1)
namespace lc78_backtracking {
void bt(int i, vector<int>& nums, vector<int>& cur, vector<vector<int>>& res) {
    if (i == (int)nums.size()) {              // decided every item → one subset
        res.push_back(cur);
        return;
    }
    cur.push_back(nums[i]);                   // TAKE nums[i]
    bt(i + 1, nums, cur, res);
    cur.pop_back();                           // undo the choice
    bt(i + 1, nums, cur, res);                // SKIP nums[i]
}
vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> res;
    vector<int> cur;
    bt(0, nums, cur, res);
    return res;
}
}

// [Cascading]  Time O(n·2ⁿ)  ·  Space O(1) extra
//   1. start with {{}}; for each x, copy every existing subset and add x
namespace lc78_cascading {
vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> res = {{}};           // start with the empty subset
    for (int x : nums) {
        int sz = res.size();                  // only extend the subsets that existed before x
        for (int i = 0; i < sz; i++) {
            auto s = res[i];
            s.push_back(x);                   // same subset, now with x
            res.push_back(s);
        }
    }
    return res;
}
}

// [Bitmask]  Time O(n·2ⁿ)  ·  Space O(1) extra
//   1. mask from 0 to 2ⁿ−1; bit i set → include nums[i]
namespace lc78_bitmask {
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> res;
    for (int mask = 0; mask < (1 << n); mask++) {   // every on/off pattern of n switches
        vector<int> s;
        for (int i = 0; i < n; i++)
            if (mask >> i & 1) s.push_back(nums[i]);  // switch i is on → take nums[i]
        res.push_back(s);
    }
    return res;
}
}

// ---------- #66  LC 46  Permutations  (Medium)
// Story: Musical chairs: try every person in chair 1, then solve the remaining chairs. Swap someone in, recurse, swap them back.

// [used[] array]  Time O(n·n!)  ·  Space O(n)
//   1. pick any unused element, mark used, recurse, unmark
namespace lc46_used_array {
void bt(vector<int>& nums, vector<bool>& used, vector<int>& cur, vector<vector<int>>& res) {
    if (cur.size() == nums.size()) {         // every chair filled
        res.push_back(cur);
        return;
    }
    for (int i = 0; i < (int)nums.size(); i++)
        if (!used[i]) {
            used[i] = true; cur.push_back(nums[i]);    // seat nums[i] in the next chair
            bt(nums, used, cur, res);
            cur.pop_back(); used[i] = false;           // stand them back up
        }
}
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;
    vector<int> cur;
    vector<bool> used(nums.size(), false);
    bt(nums, used, cur, res);
    return res;
}
}

// [In-place swaps]  Time O(n·n!)  ·  Space O(1) extra
//   1. position i: swap each j ≥ i into i, recurse on i+1, swap back
namespace lc46_in_place_swaps {
void bt(int i, vector<int>& nums, vector<vector<int>>& res) {
    if (i == (int)nums.size()) {              // chairs 0..n-1 decided
        res.push_back(nums);
        return;
    }
    for (int j = i; j < (int)nums.size(); j++) {
        swap(nums[i], nums[j]);               // put candidate j in chair i
        bt(i + 1, nums, res);                 // fill the remaining chairs
        swap(nums[i], nums[j]);               // undo
    }
}
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;
    bt(0, nums, res);
    return res;
}
}

// ---------- #67  LC 39  Combination Sum  (Medium)
// Story: A coin jar you may reuse: at each coin either use it AGAIN (stay on the same index) or move on to the next coin. Stop as soon as you overshoot.

// [Backtracking]  Time O(2^(t/min))  ·  Space O(t/min)
//   1. bt(start, remaining): loop i from start
//   2. choose c[i] → bt(i, rem − c[i]) (same i = reuse) → undo
namespace lc39_backtracking {
void bt(int start, int rem, vector<int>& c, vector<int>& cur, vector<vector<int>>& res) {
    if (rem == 0) {                           // exact amount reached
        res.push_back(cur);
        return;
    }
    // only coins from `start` on → no duplicate combinations in a different order
    for (int i = start; i < (int)c.size(); i++) {
        if (c[i] > rem) continue;             // this coin overshoots
        cur.push_back(c[i]);
        bt(i, rem - c[i], c, cur, res);       // i, not i+1 → the same coin can be reused
        cur.pop_back();
    }
}
vector<vector<int>> combinationSum(vector<int>& c, int target) {
    vector<vector<int>> res;
    vector<int> cur;
    bt(0, target, c, cur, res);
    return res;
}
}

// [Sorted + pruning]  Time same worst case, much faster  ·  Space O(t/min)
//   1. sort first; break the loop as soon as c[i] > remaining
namespace lc39_sorted_pruning {
void bt(int start, int rem, vector<int>& c, vector<int>& cur, vector<vector<int>>& res) {
    if (rem == 0) {
        res.push_back(cur);
        return;
    }
    // sorted → once a coin overshoots, every later coin does too, so stop
    for (int i = start; i < (int)c.size() && c[i] <= rem; i++) {
        cur.push_back(c[i]);
        bt(i, rem - c[i], c, cur, res);
        cur.pop_back();
    }
}
vector<vector<int>> combinationSum(vector<int>& c, int target) {
    sort(c.begin(), c.end());
    vector<vector<int>> res;
    vector<int> cur;
    bt(0, target, c, cur, res);
    return res;
}
}

// ---------- #68  LC 79  Word Search  (Medium)
// Story: An explorer in a maze with chalk: mark the cell (#) when you step in, try all 4 directions, and wipe the chalk when you back out.

// [Backtracking DFS]  Time O(m·n·3^L)  ·  Space O(L)
//   1. from every cell: dfs(i, r, c) matches w[i]
//   2. mark, explore 4 neighbours for w[i+1], unmark
namespace lc79_backtracking_dfs {
bool dfs(vector<vector<char>>& b, const string& w, int i, int r, int c) {
    if (i == (int)w.size()) return true;      // matched every letter
    // off the board, or this cell doesn't match the letter we need
    if (r < 0 || c < 0 || r >= (int)b.size() || c >= (int)b[0].size() || b[r][c] != w[i]) return false;
    char t = b[r][c];
    b[r][c] = '#';                            // chalk: can't reuse this cell on this path
    bool ok = dfs(b, w, i + 1, r + 1, c) || dfs(b, w, i + 1, r - 1, c)
           || dfs(b, w, i + 1, r, c + 1) || dfs(b, w, i + 1, r, c - 1);
    b[r][c] = t;                              // wipe the chalk on the way back
    return ok;
}
bool exist(vector<vector<char>>& b, string w) {
    for (int r = 0; r < (int)b.size(); r++)
        for (int c = 0; c < (int)b[0].size(); c++)
            if (dfs(b, w, 0, r, c)) return true;   // try every starting cell
    return false;
}
}

// [Pruned]  Time same worst case, faster in practice  ·  Space O(L)
//   1. board lacks enough of some letter → false immediately
//   2. start from the rarer end of the word (reverse it if needed)
namespace lc79_pruned {
bool dfs(vector<vector<char>>& b, const string& w, int i, int r, int c) {
    if (i == (int)w.size()) return true;
    if (r < 0 || c < 0 || r >= (int)b.size() || c >= (int)b[0].size() || b[r][c] != w[i]) return false;
    char t = b[r][c];
    b[r][c] = '#';
    bool ok = dfs(b, w, i + 1, r + 1, c) || dfs(b, w, i + 1, r - 1, c)
           || dfs(b, w, i + 1, r, c + 1) || dfs(b, w, i + 1, r, c - 1);
    b[r][c] = t;
    return ok;
}
bool exist(vector<vector<char>>& b, string w) {
    unordered_map<char,int> cnt, need;
    for (auto& row : b) for (char ch : row) cnt[ch]++;   // letters on the board
    for (char ch : w) need[ch]++;                        // letters the word needs
    for (auto& [ch, k] : need) if (cnt[ch] < k) return false;   // impossible → skip the search
    // starting from the rarer letter cuts down the number of dead-end paths
    if (cnt[w.front()] > cnt[w.back()]) reverse(w.begin(), w.end());
    for (int r = 0; r < (int)b.size(); r++)
        for (int c = 0; c < (int)b[0].size(); c++)
            if (dfs(b, w, 0, r, c)) return true;
    return false;
}
}

// ======================================================================
// DYNAMIC PROGRAMMING
// ======================================================================

// ---------- #69  LC 70  Climbing Stairs  (Easy)
// Story: To stand on step n you came from step n−1 or n−2. Ways(n) = Ways(n−1) + Ways(n−2). It's Fibonacci wearing sneakers.

// [Naive]  Time O(2ⁿ)  ·  Space O(n)
//   1. plain recursion f(n) = f(n−1) + f(n−2)
namespace lc70_naive {
int climbStairs(int n) {
    // 1 step → 1 way, 2 steps → 2 ways; otherwise last move was 1 or 2 steps
    return n <= 2 ? n : climbStairs(n - 1) + climbStairs(n - 2);   // recomputes the same n many times
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. table dp[i] = dp[i−1] + dp[i−2]
namespace lc70_optimised {
int climbStairs(int n) {
    if (n <= 2) return n;
    vector<int> dp(n + 1);                    // dp[i] = ways to reach step i
    dp[1] = 1;
    dp[2] = 2;
    for (int i = 3; i <= n; i++) dp[i] = dp[i-1] + dp[i-2];   // each written once
    return dp[n];
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. keep only the last two values
namespace lc70_best {
int climbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;                         // ways(i−2), ways(i−1)
    for (int i = 3; i <= n; i++) {
        int c = a + b;                        // ways(i)
        a = b;                                // slide the window forward
        b = c;
    }
    return b;
}
}

// ---------- #70  LC 198  House Robber  (Medium)
// Story: A thief walking down a street: at each house, "rob it + best from two houses back" OR "skip it and keep the best so far".

// [Naive]  Time O(2ⁿ)  ·  Space O(n)
//   1. go(i) = max(go(i−1), nums[i] + go(i−2))
namespace lc198_naive {
int go(vector<int>& a, int i) {
    if (i < 0) return 0;                      // no houses left
    // skip house i, or rob it and jump back two
    return max(go(a, i - 1), a[i] + go(a, i - 2));
}
int rob(vector<int>& nums) {
    return go(nums, (int)nums.size() - 1);
}
}

// [Optimised]  Time O(n)  ·  Space O(n)
//   1. dp[i+2] = max(dp[i+1], dp[i] + nums[i])
namespace lc198_optimised {
int rob(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n + 2, 0);                 // dp shifted by 2 so dp[0], dp[1] = 0 padding
    for (int i = 0; i < n; i++)
        dp[i+2] = max(dp[i+1], dp[i] + nums[i]);   // skip vs rob
    return dp[n+1];
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. two rolling variables prev2, prev1
namespace lc198_best {
int rob(vector<int>& nums) {
    int prev2 = 0, prev1 = 0;                 // best up to two houses back / one house back
    for (int x : nums) {
        int cur = max(prev1, prev2 + x);      // skip this house, or rob it
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
}

// ---------- #71  LC 322  Coin Change  (Medium)
// Story: The shopkeeper's notebook: for every amount from 0 to target, write "fewest coins to make this". Each amount = 1 + best of (amount − coin).

// [Naive]  Time O(coinsᵃᵐᵒᵘⁿᵗ)  ·  Space O(amount)
//   1. try every coin, recurse on amount − coin, keep the min
namespace lc322_naive {
int go(vector<int>& c, int amt) {
    if (amt == 0) return 0;                   // nothing left to pay
    if (amt < 0) return -1;                   // overpaid → invalid
    int best = INT_MAX;
    for (int x : c) {
        int r = go(c, amt - x);               // use coin x first
        if (r >= 0) best = min(best, r + 1);
    }
    return best == INT_MAX ? -1 : best;
}
int coinChange(vector<int>& coins, int amount) {
    return go(coins, amount);
}
}

// [Optimised]  Time O(amount·coins)  ·  Space O(amount)
//   1. same recursion + memo[amount]
namespace lc322_optimised {
int go(vector<int>& c, int amt, vector<int>& memo) {
    if (amt == 0) return 0;
    if (amt < 0) return -1;
    if (memo[amt] != -2) return memo[amt];    // already in the notebook
    int best = INT_MAX;
    for (int x : c) {
        int r = go(c, amt - x, memo);
        if (r >= 0) best = min(best, r + 1);
    }
    return memo[amt] = (best == INT_MAX ? -1 : best);   // write it down once
}
int coinChange(vector<int>& coins, int amount) {
    vector<int> memo(amount + 1, -2);         // -2 = not computed yet
    return go(coins, amount, memo);
}
}

// [Best]  Time O(amount·coins)  ·  Space O(amount)
//   1. bottom-up: dp[0] = 0, dp[a] = min(dp[a − c] + 1)
//   2. dp[amount] still "infinite" → −1
namespace lc322_best {
int coinChange(vector<int>& coins, int amount) {
    // amount + 1 is impossible (more coins than the amount) → acts as infinity
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int a = 1; a <= amount; a++)
        for (int c : coins)
            if (c <= a) dp[a] = min(dp[a], dp[a - c] + 1);   // last coin used is c
    return dp[amount] > amount ? -1 : dp[amount];
}
}

// ---------- #72  LC 300  Longest Increasing Subsequence  (Medium)
// Story: Patience solitaire: put each card on the leftmost pile whose top is ≥ it (binary search), or start a new pile. The number of piles is the LIS length.

// [Naive]  Time O(2ⁿ)  ·  Space O(n)
//   1. take/skip each element, remembering the previous taken index
namespace lc300_naive {
int go(vector<int>& a, int i, int prev) {
    if (i == (int)a.size()) return 0;
    int best = go(a, i + 1, prev);            // skip a[i]
    if (prev == -1 || a[i] > a[prev])         // can take a[i] only if it increases
        best = max(best, 1 + go(a, i + 1, i));
    return best;
}
int lengthOfLIS(vector<int>& nums) {
    return go(nums, 0, -1);                   // -1 = nothing taken yet
}
}

// [Optimised]  Time O(n²)  ·  Space O(n)
//   1. dp[i] = 1 + max dp[j] over j < i with nums[j] < nums[i]
namespace lc300_optimised {
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size(), best = 0;
    vector<int> dp(n, 1);                     // dp[i] = longest run ending at i
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++)
            if (nums[j] < nums[i]) dp[i] = max(dp[i], dp[j] + 1);   // extend the run ending at j
        best = max(best, dp[i]);
    }
    return best;
}
}

// [Best]  Time O(n log n)  ·  Space O(n)
//   1. tails[k] = smallest tail of any increasing run of length k+1
//   2. lower_bound x: replace it, or append if x is the biggest
namespace lc300_best {
int lengthOfLIS(vector<int>& nums) {
    vector<int> tails;                        // top card of each pile (always sorted)
    for (int x : nums) {
        // leftmost pile whose top is ≥ x
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);   // bigger than every top → new pile
        else *it = x;                                // smaller top keeps future options open
    }
    return tails.size();                      // number of piles = LIS length
}
}

// ---------- #73  LC 1143  Longest Common Subsequence  (Medium)
// Story: A 2-D grid of the two words: letters match → step diagonally and add 1; otherwise take the better of "up" or "left".

// [Naive]  Time O(2^(n+m))  ·  Space O(n+m)
//   1. match → 1 + go(i+1, j+1); else max(go(i+1, j), go(i, j+1))
namespace lc1143_naive {
int go(const string& a, const string& b, int i, int j) {
    if (i == (int)a.size() || j == (int)b.size()) return 0;   // one word used up
    if (a[i] == b[j]) return 1 + go(a, b, i + 1, j + 1);     // match → take it
    return max(go(a, b, i + 1, j), go(a, b, i, j + 1));      // skip a letter in a or in b
}
int longestCommonSubsequence(string a, string b) {
    return go(a, b, 0, 0);
}
}

// [Optimised]  Time O(n·m)  ·  Space O(n·m)
//   1. dp[i][j] = match ? dp[i−1][j−1] + 1 : max(dp[i−1][j], dp[i][j−1])
namespace lc1143_optimised {
int longestCommonSubsequence(string a, string b) {
    int n = a.size(), m = b.size();
    // dp[i][j] = LCS of the first i letters of a and the first j letters of b
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = a[i-1] == b[j-1] ? dp[i-1][j-1] + 1          // diagonal + 1
                                        : max(dp[i-1][j], dp[i][j-1]); // better of up / left
    return dp[n][m];
}
}

// [Best]  Time O(n·m)  ·  Space O(m)
//   1. one row; keep the old diagonal in a variable
namespace lc1143_best {
int longestCommonSubsequence(string a, string b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m + 1, 0);                 // one row of the table
    for (int i = 1; i <= n; i++) {
        int diag = 0;                         // dp[i-1][j-1] before it gets overwritten
        for (int j = 1; j <= m; j++) {
            int up = dp[j];                   // dp[i-1][j]
            dp[j] = a[i-1] == b[j-1] ? diag + 1 : max(dp[j], dp[j-1]);
            diag = up;                        // becomes the diagonal for j+1
        }
    }
    return dp[m];
}
}

// ---------- #74  LC 139  Word Break  (Medium)
// Story: Can the ribbon be cut into dictionary words? dp[i] means "the first i letters can be cut". A cut at j works if dp[j] is true and s[j..i) is a word.

// [Naive]  Time O(2ⁿ)  ·  Space O(n)
//   1. try every prefix that is a word, recurse on the rest
namespace lc139_naive {
bool go(const string& s, int i, unordered_set<string>& d) {
    if (i == (int)s.size()) return true;      // cut the whole ribbon
    for (int j = i + 1; j <= (int)s.size(); j++)
        // s[i..j) is a word and the rest can be cut too
        if (d.count(s.substr(i, j - i)) && go(s, j, d)) return true;
    return false;
}
bool wordBreak(string s, vector<string>& dict) {
    unordered_set<string> d(dict.begin(), dict.end());
    return go(s, 0, d);
}
}

// [Optimised]  Time O(n³)  ·  Space O(n)
//   1. dp[0] = true; dp[i] = any j < i with dp[j] && s[j..i) in dict
namespace lc139_optimised {
bool wordBreak(string s, vector<string>& dict) {
    unordered_set<string> d(dict.begin(), dict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);            // dp[i] = first i letters can be cut
    dp[0] = true;                             // empty prefix: nothing to cut
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < i; j++)
            // the last word is s[j..i) and everything before it was cuttable
            if (dp[j] && d.count(s.substr(j, i - j))) { dp[i] = true; break; }
    return dp[n];
}
}

// [Best]  Time O(n·L²)  ·  Space O(n)
//   1. same DP, but only look back up to the longest word length L
namespace lc139_best {
bool wordBreak(string s, vector<string>& dict) {
    unordered_set<string> d(dict.begin(), dict.end());
    int n = s.size(), L = 0;
    for (auto& w : dict) L = max(L, (int)w.size());   // longest dictionary word
    vector<bool> dp(n + 1, false);
    dp[0] = true;
    for (int i = 1; i <= n; i++)
        // the last word can't be longer than L, so only check j ≥ i − L
        for (int j = max(0, i - L); j < i && !dp[i]; j++)
            if (dp[j] && d.count(s.substr(j, i - j))) dp[i] = true;
    return dp[n];
}
}

// ---------- #75  LC 72  Edit Distance  (Medium)
// Story: A typo-fixing grid with three keyboard moves: insert (from the left), delete (from above), replace (from the diagonal). Matching letters walk the diagonal for free.

// [Naive]  Time O(3^(n+m))  ·  Space O(n+m)
//   1. match → go(i+1, j+1); else 1 + min(delete, insert, replace)
namespace lc72_naive {
int go(const string& a, const string& b, int i, int j) {
    if (i == (int)a.size()) return (int)b.size() - j;   // insert the rest of b
    if (j == (int)b.size()) return (int)a.size() - i;   // delete the rest of a
    if (a[i] == b[j]) return go(a, b, i + 1, j + 1);    // same letter → free
    return 1 + min({go(a, b, i + 1, j),                 // delete a[i]
                    go(a, b, i, j + 1),                 // insert b[j]
                    go(a, b, i + 1, j + 1)});           // replace a[i] with b[j]
}
int minDistance(string a, string b) {
    return go(a, b, 0, 0);
}
}

// [Optimised]  Time O(n·m)  ·  Space O(n·m)
//   1. dp[i][0] = i, dp[0][j] = j
//   2. dp[i][j] = match ? diag : 1 + min(up, left, diag)
namespace lc72_optimised {
int minDistance(string a, string b) {
    int n = a.size(), m = b.size();
    // dp[i][j] = edits to turn the first i letters of a into the first j letters of b
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; i++) dp[i][0] = i;   // delete everything
    for (int j = 0; j <= m; j++) dp[0][j] = j;   // insert everything
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = a[i-1] == b[j-1]
                ? dp[i-1][j-1]                                      // free diagonal
                : 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}); // delete / insert / replace
    return dp[n][m];
}
}

// [Best]  Time O(n·m)  ·  Space O(m)
//   1. one row + a diag variable
namespace lc72_best {
int minDistance(string a, string b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m + 1);
    iota(dp.begin(), dp.end(), 0);            // row 0: 0, 1, 2, … (insert everything)
    for (int i = 1; i <= n; i++) {
        int diag = dp[0];                     // dp[i-1][0]
        dp[0] = i;                            // dp[i][0]: delete i letters
        for (int j = 1; j <= m; j++) {
            int up = dp[j];                   // dp[i-1][j], about to be overwritten
            dp[j] = a[i-1] == b[j-1] ? diag : 1 + min({up, dp[j-1], diag});
            diag = up;
        }
    }
    return dp[m];
}
}

// ---------- #76  LC 416  Partition Equal Subset Sum  (Medium)
// Story: Can a backpack hold EXACTLY half of the total weight? For each item: put it in or leave it out. A bitset is a row of bulbs, and shift-OR lights up every reachable sum at once.

// [Naive]  Time O(2ⁿ)  ·  Space O(n)
//   1. sum odd → false; take/skip recursion towards sum/2
namespace lc416_naive {
bool go(vector<int>& a, int i, int rem) {
    if (rem == 0) return true;                // backpack exactly full
    if (i == (int)a.size() || rem < 0) return false;
    // put item i in, or leave it out
    return go(a, i + 1, rem - a[i]) || go(a, i + 1, rem);
}
bool canPartition(vector<int>& nums) {
    int s = accumulate(nums.begin(), nums.end(), 0);
    if (s % 2) return false;                  // odd total can't split evenly
    return go(nums, 0, s / 2);
}
}

// [Optimised]  Time O(n·sum)  ·  Space O(sum)
//   1. dp[j] = can make sum j; loop j downward so each item is used once
namespace lc416_optimised {
bool canPartition(vector<int>& nums) {
    int s = accumulate(nums.begin(), nums.end(), 0);
    if (s % 2) return false;
    int t = s / 2;
    vector<bool> dp(t + 1, false);            // dp[j] = some subset sums to j
    dp[0] = true;                             // the empty subset
    for (int x : nums)
        // go downward so x is used at most once in this pass
        for (int j = t; j >= x; j--) dp[j] = dp[j] || dp[j - x];
    return dp[t];
}
}

// [Best]  Time O(n·sum/64)  ·  Space O(sum)
//   1. bitset b, b[0] = 1; for each x: b |= b << x
namespace lc416_best {
bool canPartition(vector<int>& nums) {
    int s = accumulate(nums.begin(), nums.end(), 0);
    if (s % 2) return false;
    bitset<10001> b;                          // bulb j on = sum j is reachable (sum/2 ≤ 10000)
    b[0] = 1;
    // shifting by x lights up "old sum + x" for every old sum at once, 64 bits per step
    for (int x : nums) b |= b << x;
    return b[s / 2];
}
}

// ======================================================================
// GREEDY
// ======================================================================

// ---------- #77  LC 55  Jump Game  (Medium)
// Story: A fuel gauge: walk forward tracking the farthest point you can reach. If you're ever standing past it, you're stranded.

// [Naive]  Time O(n²)  ·  Space O(n)
//   1. reachable[] array: from every reachable i mark i+1 … i+nums[i]
namespace lc55_naive {
bool canJump(vector<int>& nums) {
    int n = nums.size();
    vector<bool> ok(n, false);                // ok[i] = index i is reachable
    ok[0] = true;
    for (int i = 0; i < n; i++)
        if (ok[i])
            for (int j = 1; j <= nums[i] && i + j < n; j++) ok[i + j] = true;   // every landing spot
    return ok[n - 1];
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. far = max(far, i + nums[i]); i > far → false
namespace lc55_best {
bool canJump(vector<int>& nums) {
    int far = 0;                              // farthest index reachable so far
    for (int i = 0; i < (int)nums.size(); i++) {
        if (i > far) return false;            // standing past the gauge → stranded
        far = max(far, i + nums[i]);          // refuel
    }
    return true;
}
}

// ---------- #78  LC 45  Jump Game II  (Medium)
// Story: BFS in disguise: each "level" is the stretch you can reach with j jumps. When you walk past the edge of the current level, you must take one more jump.

// [Naive]  Time O(n²)  ·  Space O(n)
//   1. dp[i] = fewest jumps to reach i; relax i+1 … i+nums[i]
namespace lc45_naive {
int jump(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, INT_MAX);               // dp[i] = fewest jumps to reach i
    dp[0] = 0;
    for (int i = 0; i < n; i++)
        if (dp[i] != INT_MAX)
            for (int j = 1; j <= nums[i] && i + j < n; j++)
                dp[i + j] = min(dp[i + j], dp[i] + 1);
    return dp[n - 1];
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. far = farthest reach; at i == end of current level → jumps++, end = far
namespace lc45_best {
int jump(vector<int>& nums) {
    int jumps = 0, end = 0, far = 0;          // end = edge of the current level
    for (int i = 0; i + 1 < (int)nums.size(); i++) {   // no jump needed from the last index
        far = max(far, i + nums[i]);          // farthest the next level can reach
        if (i == end) {                       // finished this level
            jumps++;
            end = far;                        // the next level ends here
        }
    }
    return jumps;
}
}

// ---------- #79  LC 134  Gas Station  (Medium) [Logistics]
// Story: A road trip: if total gas ≥ total cost, some start works. Whenever your tank goes negative, every station you passed is a bad start, so restart from the next one.

// [Naive]  Time O(n²)  ·  Space O(1)
//   1. try every start; simulate the full loop
namespace lc134_naive {
int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
    int n = gas.size();
    for (int s = 0; s < n; s++) {             // try starting at station s
        int tank = 0, k = 0;
        for (; k < n; k++) {
            int i = (s + k) % n;              // wrap around the circle
            tank += gas[i] - cost[i];
            if (tank < 0) break;              // ran dry
        }
        if (k == n) return s;                 // completed the loop
    }
    return -1;
}
}

// [Best]  Time O(n)  ·  Space O(1)
//   1. total += diff; tank += diff
//   2. tank < 0 → start = i + 1, tank = 0; total < 0 → −1
namespace lc134_best {
int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
    int total = 0, tank = 0, start = 0;
    for (int i = 0; i < (int)gas.size(); i++) {
        int d = gas[i] - cost[i];             // net gain at station i
        total += d;                           // whole-trip balance
        tank += d;                            // balance since the current start
        if (tank < 0) {                       // can't reach i+1 from any station in [start, i]
            start = i + 1;
            tank = 0;
        }
    }
    return total >= 0 ? start : -1;           // enough gas overall → this start works
}
}

// ======================================================================
// TRIE
// ======================================================================

// ---------- #80  LC 208  Implement Trie (Prefix Tree)  (Medium)
// Story: A dictionary shaped like a tree: every road sign is a letter, and a flag on a house says "a word ends here". startsWith only needs the road to exist; search also needs the flag.

// [Naive]  Time insert O(1), prefix O(N·L)  ·  Space O(N·L)
//   1. keep a hash set of words; prefix query scans every word
namespace lc208_naive {
class Trie {
    unordered_set<string> words;
public:
    void insert(string w) { words.insert(w); }
    bool search(string w) { return words.count(w); }
    bool startsWith(string p) {
        // check every stored word → slow with many words
        for (auto& w : words)
            if (w.compare(0, p.size(), p) == 0) return true;   // w begins with p
        return false;
    }
};
}

// [Best]  Time O(L) per op  ·  Space O(total letters · 26)
//   1. node = 26 child pointers + end flag
//   2. insert creates missing children; search needs end flag; startsWith just needs the path
namespace lc208_best {
class Trie {
    struct Node {
        Node* ch[26] = {};                    // one road per letter
        bool end = false;                     // flag: a word ends here
    };
    Node* root = new Node();
    // follow the road for s; nullptr if the road breaks
    Node* walk(const string& s) {
        Node* n = root;
        for (char c : s) {
            if (!n->ch[c - 'a']) return nullptr;
            n = n->ch[c - 'a'];
        }
        return n;
    }
public:
    void insert(string w) {
        Node* n = root;
        for (char c : w) {
            if (!n->ch[c - 'a']) n->ch[c - 'a'] = new Node();   // build missing road
            n = n->ch[c - 'a'];
        }
        n->end = true;                        // plant the flag
    }
    bool search(string w) {
        Node* n = walk(w);
        return n && n->end;                   // road exists AND a word ends there
    }
    bool startsWith(string p) {
        return walk(p) != nullptr;            // the road alone is enough
    }
};
}

int main() { return 0; }
