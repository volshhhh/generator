#pragma once

struct Object {
  virtual ~Object() = 0;
};

inline Object::~Object() {} // NOLINT