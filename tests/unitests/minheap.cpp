#include "config.hpp"

using namespace std;

TEST (min_heap, merge_vectors_same_len)
{
        std::vector<int> vec1 {1, 3, 5, 7, 9};
        std::vector<int> vec2 {0, 2, 4, 6, 8};
        std::vector vec_vec {vec1, vec2};
        std::vector<int> merged {};
        std::vector<int> ans {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        min_heap_t<int> min_heap {};
        min_heap.push(min_heap_node_t {vec1[0], 0, 1});
        min_heap.push(min_heap_node_t {vec2[0], 1, 1});
        min_heap.initial_heapify();

        for (size_t i = 0; i < vec1.size() * 2; i++) {
                min_heap_node_t root = min_heap.get_min();
                merged.push_back(root.elem);

                if (root.next_elem_index < vec1.size()) {
                        root.elem = vec_vec[root.arr_index][root.next_elem_index];
                        root.next_elem_index++;
                } else {
                        root.elem = INT_MAX;
                }
                min_heap.replace_min(root);
        }

        ASSERT_TRUE(ans == merged);
}

TEST (min_heap, merge_vectors_diff_len)
{
        std::vector<int> vec1 {1, 3, 5, 7, 9};
        std::vector<int> vec2 {0, 2, 4, 6, 8, 10};
        std::vector<int> vec3 {0, 2, 4, 6, 8, 10};
        std::vector vec_vec {vec1, vec2, vec3};
        std::vector<int> merged {};
        std::vector<int> ans {0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10};

        min_heap_t<int> min_heap {};
        min_heap.push(min_heap_node_t {vec3[0], 0, 1});
        min_heap.push(min_heap_node_t {vec2[0], 1, 1});
        min_heap.push(min_heap_node_t {vec1[0], 2, 1});
        min_heap.initial_heapify();

        for (size_t i = 0; i < vec1.size() + vec2.size() + vec3.size(); i++) {
                min_heap_node_t root = min_heap.get_min();
                merged.push_back(root.elem);

                if (root.next_elem_index < vec_vec[root.arr_index].size()) {
                        root.elem = vec_vec[root.arr_index][root.next_elem_index];
                        root.next_elem_index++;
                } else {
                        root.elem = INT_MAX;
                }
                min_heap.replace_min(root);
        }

        ASSERT_TRUE(ans == merged);
}
