/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ArgumentParser.h"
#include "../TensorHostObject.h"
#include "helpers.h"

namespace torchlive {
namespace utils {

using namespace facebook;

ArgumentParser::ArgumentParser(
    jsi::Runtime& runtime,
    const jsi::Value& thisVal,
    const jsi::Value* args,
    size_t count)
    : runtime_(runtime), thisVal_(thisVal), args_(args), count_(count) {}

const jsi::Value& ArgumentParser::operator[](size_t idx) const {
  if (idx + 1 > count_) {
    requireNumArguments(idx + 1);
  }
  return args_[idx];
}

size_t ArgumentParser::count() const {
  return count_;
}

std::vector<int64_t> ArgumentParser::dimsVarArgs(size_t idx, size_t* nextArgIdx)
    const {
  std::vector<int64_t> dims;
  auto numParsed = helpers::parseSize(runtime_, args_, idx, count_, &dims);
  assert(numParsed == dims.size());
  if (nextArgIdx != nullptr) {
    *nextArgIdx = idx + numParsed;
  }
  return dims;
}

jsi::Value ArgumentParser::keywordValue(size_t idx, const std::string& key)
    const {
  return helpers::parseKeywordArgument(
      runtime_, args_, idx, count_, key.c_str());
}

void ArgumentParser::requireNumArguments(size_t minArgCount) const {
  if (count_ < minArgCount) {
    throw jsi::JSError(
        runtime_,
        "This function requires at least " + std::to_string(minArgCount) +
            " argument" + (minArgCount == 1 ? "" : "s"));
  }
}

::torch::TensorOptions ArgumentParser::tensorOptions(size_t idx) const {
  return helpers::parseTensorOptions(runtime_, args_, idx, count_);
}

} // namespace utils
} // namespace torchlive
