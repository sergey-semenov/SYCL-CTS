/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2017-2022 Codeplay Software LTD. All Rights Reserved.
//  Copyright (c) 2022 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
*******************************************************************************/

#include "../../util/sycl_exceptions.h"
#include "../common/common.h"

#define TEST_NAME device_selector_predefined

namespace device_selector_predefined__ {
using namespace sycl_cts;

/** tests predefined selectors for sycl::device_selector
 */
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
    {
      bool gpuAvailable = false;
      bool cpuAvailable = false;
      bool acceleratorAvailable = false;
      auto devices = sycl::device::get_devices();
      for (auto device : devices) {
        if (device.is_gpu()) {
          gpuAvailable = true;
        }
        if (device.is_cpu()) {
          cpuAvailable = true;
        }
        if (device.is_accelerator()) {
          acceleratorAvailable = true;
        }
      }

      /** check default_selector
      */
      if (!cpuAvailable && !gpuAvailable && !acceleratorAvailable) {
        sycl::default_selector defaultSelector;
        try {
          auto defaultDevice = util::get_cts_object::device(defaultSelector);
          if (defaultDevice.get_info<sycl::info::device::device_type>() !=
              sycl::info::device_type::host) {
            std::string errorMsg =
                "a SYCL exception occured when default_selector tried to "
                "select a device when no opencl devices available";
            FAIL(log, errorMsg.c_str());
          }
        } catch (const sycl::exception &e) {
          log_exception(log, e);
          FAIL(log,
               "default_selector failed to select a host device when no opencl "
               "devices available");
        }
      }

      /** check cpu_selector
      */
      if (cpuAvailable) {
        sycl::cpu_selector cpuSelector;
        auto cpuDevice = util::get_cts_object::device(cpuSelector);
        if (!(cpuDevice.is_cpu())) {
          FAIL(log, "cpu_selector failed to select an appropriate device");
        }
      }

      /** check gpu_selector
      */
      if (gpuAvailable) {
        sycl::gpu_selector gpuSelector;
        auto gpuDevice = util::get_cts_object::device(gpuSelector);
        if (!(gpuDevice.is_gpu())) {
          FAIL(log, "gpu_selector failed to select an appropriate device");
        }
      }

      /** check accelerator_selector
      */
      if (acceleratorAvailable) {
        sycl::accelerator_selector acceleratorSelector;
        auto acceleratorDevice = util::get_cts_object::device(acceleratorSelector);
        if (!(acceleratorDevice.is_accelerator())) {
          FAIL(log, "accelerator_selector failed to select an appropriate device");
        }
      }
    }
  }
};

// register this test with the test_collection
util::test_proxy<TEST_NAME> proxy;

} /* namespace device_selector_predefined__ */
