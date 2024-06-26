#include <node.h>
 
namespace {

  // Function to allocate memory
  void AllocMem(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    if (!args[0]->IsNumber()) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Argument must be a number").ToLocalChecked()));
      return;
    }

    size_t amount = args[0]->IntegerValue(isolate->GetCurrentContext()).ToChecked();

    // Allocate memory
    void* ptr = malloc(amount);

    // Return the memory address
    args.GetReturnValue().Set(v8::BigInt::New(isolate, reinterpret_cast<uintptr_t>(ptr)));
  }

  // Function to free memory
  void FreeMem(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    if (!args[0]->IsBigInt()) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Argument must be a bigint").ToLocalChecked()));
      return;
    }

    void* ptr = reinterpret_cast<void*>(args[0].As<v8::BigInt>()->Uint64Value());

    // Free memory
    free(ptr);
  }

  // Function to get the memory location of a Node.js object
  void GetLocation(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    v8::Local<v8::Object> obj = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();

    // Get the memory address of the object
    uintptr_t address = reinterpret_cast<uintptr_t>(*obj);

    // Return the memory address
    args.GetReturnValue().Set(v8::BigInt::New(isolate, address));
  }

  // Function to set the value at a memory location
  void Set(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    if (!args[0]->IsBigInt()) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "First argument must be a bigint").ToLocalChecked()));
      return;
    }

    v8::Local<v8::Object> obj = args[1]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();

    void* ptr = reinterpret_cast<void*>(args[0].As<v8::BigInt>()->Uint64Value());

    // Set the value at the memory location
    *(reinterpret_cast<v8::Local<v8::Value>*>(ptr)) = obj;
  }

  // Function to get the object at a memory location
  void Get(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    if (!args[0]->IsBigInt()) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Argument must be a bigint").ToLocalChecked()));
      return;
    }

    void* ptr = reinterpret_cast<void*>(args[0].As<v8::BigInt>()->Uint64Value());

    // Get the object at the memory location
    v8::Local<v8::Value> value = *(reinterpret_cast<v8::Local<v8::Value>*>(ptr));

    // Unwrap the value if it's a local value
    if (!value.IsEmpty() && value->IsObject()) {
      args.GetReturnValue().Set(value.As<v8::Object>());
    } else {
      args.GetReturnValue().SetNull();
    }
  }

  // Initialize the module
  void Initialize(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "allocMem", AllocMem);
    NODE_SET_METHOD(exports, "freeMem", FreeMem);
    NODE_SET_METHOD(exports, "getLocation", GetLocation);
    NODE_SET_METHOD(exports, "set", Set);
    NODE_SET_METHOD(exports, "get", Get);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}
