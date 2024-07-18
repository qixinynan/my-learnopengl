#ifndef OBJECT_H
#define OBJECT_H

namespace nanrender {
class Object {
public:
  Object();
  int InstanceId();

private:
  int instance_id_;
};
} // namespace nanrender

#endif