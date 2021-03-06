#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace utilities::string::match {
        /// the rabin-karp string matching algorithm
        /// The idea behind the algorithm is using **hashing** for faster
        /// matching of hash values. It computes the hash of the pattern and
        /// matches it with the hashes of **all possible prefixes** of the
        /// text with length *M*.
        ///
        /// If the length of @pattern is 4 and the length of @text is 4, then the
        /// possible prefixes would be the substrings given by the following indice
        /// ranges:
        ///     [ [0,4), [1, 5), [2, 6) ]
        ///
        /// If a pair of hashes match, this does **not** guarantee an exact match,
        /// but a probable one due to **collisions** in the hash function.
        /// An exact match is the performed to check for *false positives*.
        /// The algorithm naturally handles the case pattern.length() > text.length(),
        /// and returns false.
        auto rabin_karp(std::string_view pattern, std::string_view text) -> bool {
            using std::size_t;
            const int ALPHABET    = 256;
            const int Q           = 101;
            const size_t t_length = text.length();
            const size_t p_length = pattern.length();

            // used to compute the polynomial to store the hash in.
            int p = 0;
            int t = 0;
            int h = 1;

            // Calculate the hash value of pattern
            for (int i = 0; i < p_length; i += 1) {
                h = (h * ALPHABET) % Q;  // precompute
                p = (ALPHABET * p + pattern[i]) % Q;
                t = (ALPHABET * t + text[i]) % Q;
            }

            // Slide the 'pattern' over 'text' by one
            for (int i = 0; i <= t_length - p_length; i += 1) {
                // check the hash values
                if (p == t) {

                    // extra check in case of false positive
                    std::cout << "text substring = " << text.substr(i, i + p_length) << "\tpattern = " << pattern << std::endl;
                    if (text.substr(i, i + p_length) == pattern) return true;
                }

                // Calculate hash value of next window of 'text'
                // Remove leading digit, add trailing digit.
                if (i < t_length - p_length) {
                    t = (ALPHABET * t - text[i] * h + text[i + p_length]) % Q;  // clever update
                    // we might get negative values of t, converting it to positive.
                    if (t < 0) { t = t + Q; }
                }
            }

            return false;
        }


        /* void prepare_match(const std::string& pattern, std::vector<int>& b) { */
        /*     int i = 0; */
        /*     int j = -1; */
        /*     size_t m = pattern.length(); */

        /*     b.resize(m); */
        /*     b[0] = -1;   // starting values */

        /*     while (i < m) {  // pre-process the pattern string */
        /*         while (j >= 0 && pattern[i] != pattern[j]) { */
        /*             j = b[j];  // if different, reset j using b */
        /*         } */

        /*         i += 1; */
        /*         j += 1;          // if same advance both pointers */
        /*         b[i] = j; */
        /*     } */
        /* } */

        /* std::vector<int> prefix_table(const std::string& pattern) { */
        /*     size_t len = pattern.length(); */
        /*     std::vector<int> table(len, 0); */

        /*     int k = 0; */
        /*     for (int i = 1; i < len; i++) { */

        /*         while (k > 0 && pattern[k+1] != pattern[i]) { */
        /*             k = table[k]; */
        /*         } */

        /*         if (pattern[k+1] == pattern[i]) { */
        /*             k += 1; // the longest prefix str_k is also a proper suffix of str_i */
        /*         } */
        /*         table[i] = k; */
        /*     } */

        /*     return table; */
        /* } */
}  // namespace utilities::string::match

