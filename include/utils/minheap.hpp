#pragma once

template <typename T>
struct min_heap_node_t
{
        T elem {};
        size_t arr_index = 0;
        size_t next_elem_index = 0;
};

template <typename T>
class min_heap_t
{
        private:
                std::vector<min_heap_node_t<T>> nodes {};

        public:
                min_heap_t () = default;
                min_heap_t (std::vector<min_heap_node_t<T>> &&nodes_):
                        nodes(nodes_)
                {
                        initial_heapify();        
                }

                void initial_heapify ()
                { 
                        for (size_t i = (nodes.size() - 1) / 2; i >= 0; --i)
                                heapify(i);
                }

                size_t get_left_child_ind (size_t index)  { return 2 * index + 1; }
                size_t get_right_child_ind (size_t index) { return 2 * index + 2; }

                void push (min_heap_node_t<T> &&node) { nodes.push_back(node); }

                min_heap_node_t<T> get_min () const { return nodes[0]; }

                void replace_min (min_heap_node_t<T> &node) 
                { 
                        nodes[0] = node; 
                        heapify(0);
                }
        private:
                void heapify (size_t root_index);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void min_heap_t<T>::heapify (size_t root_index)
{
        size_t left  = get_left_child_ind(root_index);
        size_t right = get_right_child_ind(root_index);
        size_t smallest = root_index;

        if (left < nodes.size() && nodes[left].elem < nodes[root_index].elem)
                smallest = left;

        if (right < nodes.size() && nodes[right].elem < nodes[smallest].elem)
                smallest = right;

        if (smallest != root_index) {
                std::swap(nodes[root_index], nodes[smallest]);
                heapify(smallest);
        }
}