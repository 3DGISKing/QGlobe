// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the declarations of various string utility functions.

#include "kml/base/string_util.h"
#include <stdlib.h>  // strtod()

namespace kmlbase {

void b2a_hex(uint32_t i, char* out) {
  char map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                  'a', 'b', 'c', 'd', 'e', 'f'};
  out[0] = map[(i >> 4) & 0xf];
  out[1] = map[(i) & 0xf];
}

std::string CreateExpandedStrings(const std::string& in,
                                  const StringMap& string_map,
                                  const std::string& start,
                                  const std::string& end) {
  std::string out(in);
  StringMap::const_iterator itr = string_map.begin();
  for (itr = string_map.begin(); itr != string_map.end(); ++itr)  {
    std::string candidate(start + itr->first + end);
    size_t start_pos = out.find(candidate);
    while (start_pos != std::string::npos) {
      out.replace(start_pos, candidate.size(), itr->second);
      start_pos = out.find(candidate, start_pos + candidate.size());
    }
  }
  return out;
}

void SplitStringUsing(const std::string& input, const std::string& split_string,
                      std::vector<std::string>* output) {
  std::string iter = input;
  size_t separator;
  while ((separator = iter.find(split_string)) != std::string::npos) {
    output->push_back(iter.substr(0, separator));
    iter = iter.substr(separator+1);
  }
  output->push_back(iter);
}

template<>
void FromString(const std::string& str, bool* out) {
  if (out) {
    *out = str == "true" || str == "1";
  }
}


template<>
void FromString(const std::string& str, double* out) {
  if (out) {
    *out = strtod(str.c_str(), NULL);
  }
}

template<>
void FromString(const std::string& str, int* out) {
  if (out) {
    *out = atoi(str.c_str());
  }
}

template<>
void FromString(const std::string& str, std::string* out) {
  if (out) {
    *out = str;
  }
}

bool StringEndsWith(const std::string& str, const std::string& end) {
  if (str.empty() || end.empty()) {
    return false;
  }
  if (str.size() == end.size()) {
    return str == end;
  }
  return end.size() < str.size() && str.substr(str.size() - end.size()) == end;
}

}  // end namespace kmlbase
