/*
 * Copyright 2011-2021 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "device/device_kernel.h"

CCL_NAMESPACE_BEGIN

class Device;
class RenderBuffers;

struct KernelWorkTile;

/* Abstraction of a command queue for a device.
 * Provides API to schedule kernel execution in a specific queue with minimal possible overhead
 * from driver side.
 *
 * This class encapsulates all properties needed for commands execution. */
class DeviceQueue {
 public:
  virtual ~DeviceQueue() = default;

  /* Initialize execution of kernels on this queue.
   *
   * Will, for example, load all data required by the kernels from Device to global or path state.
   *
   * Use this method after device synchronization has finished before enqueueing any kernels. */
  virtual void init_execution() = 0;

  /* Enqueue kernel execution.
   *
   * Execute the kernel work_size times on the device.
   * Supported arguments types:
   * - int: pass pointer to the int
   * - device memory: pass pointer to device_memory.device_pointer
   * Return false if there was an error executing this or a previous kernel. */
  virtual bool enqueue(DeviceKernel kernel, const int work_size, void *args[]) = 0;

  /* Wait unit all enqueued kernels have finished execution.
   * Return false if there was an error executing any of the enqueued kernels. */
  virtual bool synchronize() = 0;

  /* Device this queue has been created for. */
  Device *device;

 protected:
  /* Hide construction so that allocation via `Device` API is enforced. */
  explicit DeviceQueue(Device *device);
};

CCL_NAMESPACE_END