/*
 * Copyright (c) 2020, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <layer.hpp>
#include <vector>

namespace HugeCTR {

/**
 * Layer which does element-wise add by input tensors.
 * All the input tensors should have the same shape.
 */
template <typename T>
class FusedReshapeConcatGeneralLayer : public Layer {
  /*
   * stores the weight gradient tensors of this layer.
   */
  Tensors2<T> wgrad_;
  /*
   * stores the references to the input tensors of this layer.
   */
  Tensors2<T> in_tensors_;
  /*
   * stores the references to the output tensors of this layer.
   */
  Tensor2<T> out_tensor_;

 public:
  /**
   * Ctor of FusedReshapeConcatGeneralLayer.
   * @param in_tensor the input tensor
   * @param out_tensor the resulting output tensor
   * @param device_id the id of GPU where this layer belongs
   */
  FusedReshapeConcatGeneralLayer(const Tensors2<T>& in_tensors, Tensor2<T>& out_tensor,
                                 const std::shared_ptr<GeneralBuffer2<CudaAllocator>>& blobs_buff,
                                 const std::shared_ptr<GPUResource>& gpu_resource);

  void initialize() override;
  ~FusedReshapeConcatGeneralLayer() override;
  /**
   * FusedReshapeConcatGeneralLayer's foward propagation
   * @param stream CUDA stream where the foward propagation is executed
   */
  void fprop(bool is_train) override;
  /**
   * FusedReshapeConcatGeneralLayer's backward propagation
   * @param stream CUDA stream where the foward propagation is executed
   */
  void bprop() override;

 private:
  size_t* vecs_size_;
  std::vector<size_t> h_vecs_size_;
  size_t new_width_ = 0;
  size_t num_;
  size_t batch_size_ = 0;
  size_t slot_num_ = 0;
  Tensor2<T*> h_inputs_;
  Tensor2<T*> d_inputs_;
};

}  // namespace HugeCTR