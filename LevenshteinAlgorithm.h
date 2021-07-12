#pragma once

#include <stdlib.h>

#include <string>

using namespace std;

/** Levenshtein±‡º≠æ‡¿ÎÀ„∑®
    @Article: https://blog.csdn.net/ddupd/article/details/19759209
 **/
class LevenshteinAlgorithm {
 private:
  static bool compare_char_(char c1, char c2);

  static size_t ins_(char c);

  static size_t del_(char c);

  static size_t sub_(char c1, char c2);

  static size_t compare_(const std::string& ref_s, const std::string& ref_l);

 public:
  static float compare(const std::string& ref1, const std::string& ref2);
};
