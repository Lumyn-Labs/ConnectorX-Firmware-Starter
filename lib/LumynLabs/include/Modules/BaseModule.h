#pragma once

#include <cstring>

#include "BaseModuleImpl.h"

namespace Modules {

template <typename T>
class BaseModule : public BaseModuleImpl {
 public:
  BaseModule(const Configuration::Module& config) : BaseModuleImpl(config) {}
  virtual ~BaseModule() = default;

  virtual ModuleError_t initModule() = 0;

  ModuleError_t read(std::vector<uint8_t>& dataOut) override {
    T data;
    ModuleError_t err = readData(&data);

    if (err.errorType == ModuleErrorType_t::None) {
      dataOut.resize(sizeof(T));
      std::memcpy(dataOut.data(), &data, sizeof(T));
    }

    return err;
  }

 protected:
  virtual ModuleError_t readData(T* dataOut) = 0;

  using BaseModuleImpl::SpiInstance;
  using BaseModuleImpl::UartInstance;
  using BaseModuleImpl::WireInstance;
};

}  // namespace Modules