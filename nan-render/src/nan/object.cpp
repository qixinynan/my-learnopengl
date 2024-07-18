#include "nan/object.h"
#include "nan/application.h"

nanrender::Object::Object() {
  instance_id_ = GetApplication()->FetchInstanceId();
}

int nanrender::Object::InstanceId() { return instance_id_; }