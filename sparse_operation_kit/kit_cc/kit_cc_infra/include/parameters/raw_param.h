/*
 * Copyright (c) 2021, NVIDIA CORPORATION.
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

#ifndef RAW_PARAM_H
#define RAW_PARAM_H

#include "parameters/param_interface.h"
#include "tensor_buffer/tensor2.hpp"
#include "tensor_buffer/general_buffer2.hpp"
#include "resources/manager.h"
#include "initializer/initializer_interface.h"
#include <memory>
#include <vector>

namespace SparseOperationKit {

class RawParam : public ParamInterface {
    using NvHashTable = ParamInterface::NvHashTable;
    template <typename T>
    using Tensor2 = HugeCTR::Tensor2<T>; 
    template <typename T>
    using Tensors2 = HugeCTR::Tensors2<T>;
    
public:
    ~RawParam();
    static std::shared_ptr<RawParam> create(const std::string& initializer, const bool use_hashtable,
                                            const std::vector<size_t> shape,
                                            const std::shared_ptr<ResourcesManager>& resource_mgr,
                        const std::vector<std::shared_ptr<HugeCTR::GeneralBuffer2<HugeCTR::CudaAllocator>>>& buffers,
                                            const std::string var_name, const bool trainable);

    size_t get_max_vocabulary_size_per_gpu() const override;
    size_t get_embedding_vec_size() const override;
    void init(const size_t global_replica_id) override;
    bool trainable() const override;
    void set_user(std::shared_ptr<EmbeddingLayer>& embedding) override;
    std::shared_ptr<NvHashTable>& get_hashtable(const size_t local_replica_id) override;
    std::shared_ptr<Tensor>& get_embedding_table_tensor(const size_t local_replica_id) override;
    virtual std::string get_var_name() const override;
    void dump_to_file(const std::string filepath) override;
    void let_user_dump_to_file(const std::string filepath) override;
    void restore_from_file(const std::string filepath) override;
    void let_user_restore_from_file(const std::string filepath) override;
    void load_embedding_values(const std::vector<std::shared_ptr<Tensor>>& tensor_list) override;
    void let_user_load_embedding_values(const std::vector<std::shared_ptr<Tensor>>& tensor_list) override;
    
private:
    RawParam(const std::string& initializer, const std::vector<size_t> shape,
             const std::shared_ptr<ResourcesManager>& resource_mgr,
             const std::vector<std::shared_ptr<HugeCTR::GeneralBuffer2<HugeCTR::CudaAllocator>>>& buffers,
             const std::string var_name, const bool trainable); 
            
    std::shared_ptr<ResourcesManager> resource_mgr_;
    std::vector<std::shared_ptr<NvHashTable>> hashtables_; // hashtables for all GPUs on this worker.
    Tensors2<float> emb_table_tensors_; // embedding vectors for all GPUs on this worker.
    std::vector<std::shared_ptr<Tensor>> emb_table_tensors_interface_;
    const size_t max_vocabulary_size_per_gpu_;
    const size_t embedding_vector_size_;
    const std::string var_name_;
    const bool trainable_;
    std::shared_ptr<Initializer> initializer_;
    const bool has_hashtable_ = false;
    std::shared_ptr<EmbeddingLayer> user_; // which embedding used this param
};

} // namespace SparseOperationKit
 
#endif // RAW_PARAM_H