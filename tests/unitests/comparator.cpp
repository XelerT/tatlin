#include "config.hpp"

using namespace paracl;
using namespace backend;
using namespace std;

TEST (if_greater_t, true_statement)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        
        auto var = new var_t<int> {1, "var1"};
        auto cond_lhs = new var_t<int> {100, "var2"};
        auto cond_rhs = new var_t<int> {1,   "var3"};
        auto condition = new greater_t {cond_lhs, cond_rhs};
        auto oper = new if_t {condition, oper_scope};

        oper->insert(new assign_t {var, new number_t {66}});
        try { scope.insert_symbs(var, types_t::VAR); }
        catch (std::runtime_error &re) { std::cout << "catched error\n"; };

        scope.insert(oper);
        scope.execute(scope);
        EXPECT_EQ(scope.find(*var)->execute(scope), 66);
}

TEST (if_less_t, true_statement)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        
        auto var = new var_t<int> {1, "var1"};
        auto cond_lhs = new var_t<int> {100,  "var2"};
        auto cond_rhs = new var_t<int> {1000, "var3"};
        auto condition = new less_t {cond_lhs, cond_rhs};
        auto oper = new if_t {condition, oper_scope};

        oper->insert(new assign_t {var, new number_t {66}});
        try { scope.insert_symbs(var, types_t::VAR); }
        catch (std::runtime_error &re) { std::cout << "catched error\n"; };

        scope.insert(oper);
        scope.execute(scope);
        EXPECT_EQ(scope.find(*var)->execute(scope), 66);
}

TEST (if_negative_t, true_statement)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        
        auto var = new var_t<int> {1, "var1"};
        auto cond_lhs = new var_t<int> {10,  "var2"};
        auto cond_rhs = new var_t<int> {1, "var3"};
        auto condition = new less_t {cond_lhs, cond_rhs};
        auto neg_condition = new negative_t {condition};
        auto oper = new if_t {neg_condition, oper_scope};

        oper->insert(new assign_t {var, new number_t {66}});
        try { scope.insert_symbs(var, types_t::VAR); }
        catch (std::runtime_error &re) { std::cout << "catched error\n"; };

        scope.insert(oper);
        scope.execute(scope);
        EXPECT_EQ(scope.find(*var)->execute(scope), 66);
}