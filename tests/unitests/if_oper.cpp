#include "config.hpp"

using namespace paracl;
using namespace backend;
using namespace std;


TEST (if_oper, true_statement)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        
        auto var = new var_t<int> {1, "var1"};
        auto oper = new if_t {new number_t {50}, oper_scope};

        oper->insert(new assign_t {var, new number_t {66}});
        try { scope.insert_symbs(var, types_t::VAR); }
        catch (std::runtime_error &re) { std::cout << "catched error\n"; };

        scope.insert(oper);
        scope.execute(scope);
        EXPECT_EQ(scope.find(*var)->execute(scope), 66);
}

TEST (if_oper, false_statement)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        auto var = new var_t<int> {1, "var1"};
        auto oper = new if_t {new number_t {0}, oper_scope};

        oper->insert(new assign_t {var, new number_t {66}});
        scope.insert_symbs(var, types_t::VAR);
        scope.insert(oper);
        scope.execute(scope);

        EXPECT_EQ(scope.find(*var)->execute(scope), 1);
}

TEST (if_oper, true_statement_complex)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};
        auto oper = new if_t {new number_t {50}, oper_scope};

        oper->insert(new assign_t {var1, new number_t {66}});
        oper->insert(new assign_t {var2, new number_t {66}});
        oper->insert(new assign_t {var3, new number_t {66}});
        
        scope.insert_symbs(var1, types_t::VAR);
        scope.insert_symbs(var2, types_t::VAR);
        scope.insert_symbs(var3, types_t::VAR);
        
        scope.insert(oper);

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 66);
        EXPECT_EQ(scope.find(*var2)->execute(scope), 66);
        EXPECT_EQ(scope.find(*var3)->execute(scope), 66);
}

TEST (if_oper, false_statement_complex)
{
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};
        auto oper = new if_t {new number_t {0}, oper_scope};

        oper->insert(new assign_t {var1, new number_t {66}});
        oper->insert(new assign_t {var2, new number_t {66}});
        oper->insert(new assign_t {var3, new number_t {66}});
        
        scope.insert_symbs(var1, types_t::VAR);
        scope.insert_symbs(var2, types_t::VAR);
        scope.insert_symbs(var3, types_t::VAR);
        
        scope.insert(oper);

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 1);
        EXPECT_EQ(scope.find(*var2)->execute(scope), 2);
        EXPECT_EQ(scope.find(*var3)->execute(scope), 3);
}

TEST (if_oper, if_true_if_true_if_true)
{
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};
        
        scope_t scope {};
        auto oper_scope  = new scope_t {&scope};
        auto oper_scope2 = new scope_t {oper_scope};
        auto oper_scope3 = new scope_t{oper_scope2};

        scope.insert_symbs(static_cast<name_t*>(var1), types_t::VAR);
        oper_scope2->insert_symbs(static_cast<name_t*>(var2), types_t::VAR);
        oper_scope3->insert_symbs(static_cast<name_t*>(var3), types_t::VAR);

        auto oper1 = new if_t {new number_t {1}, oper_scope};
        oper1->insert(new assign_t {var1, new number_t {66}});

        auto oper2 = new if_t {new number_t {1}, oper_scope2}; 
        oper2->insert(new assign_t {var2, new number_t {66}});

        auto oper3 = new if_t {new number_t {1}, oper_scope3}; 
        oper3->insert(new assign_t {var3, new number_t {66}});

        oper1->insert(oper2);
        oper2->insert(oper3);

        scope.insert(oper1);

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 66);
        EXPECT_EQ(oper_scope2->find(*var2)->execute(*oper_scope2), 66);
        EXPECT_EQ(oper_scope3->find(*var3)->execute(*oper_scope3), 66);
}

TEST (if_oper, scope_symtable_test)
{
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};

        scope_t scope {};
        auto oper_scope  = new scope_t {&scope};
        auto oper_scope2 = new scope_t {oper_scope};
        auto oper_scope3 = new scope_t{oper_scope2};

        scope.insert_symbs(var1, types_t::VAR);
        oper_scope2->insert_symbs(var2, types_t::VAR);
        oper_scope3->insert_symbs(var3, types_t::VAR);

        auto oper1 = new if_t {new number_t {1}, oper_scope};
        oper1->insert(new assign_t {var1, new number_t {66}});
        auto oper2 = new if_t {new number_t {1}, oper_scope2}; 
        oper2->insert(new assign_t {var2, new number_t {66}});
        auto oper3 = new if_t {new number_t {1}, oper_scope3}; 
        oper3->insert(new assign_t {var3, new number_t {66}});

        oper1->insert(oper2);
        oper2->insert(oper3);

        scope.insert(oper1);

        ASSERT_FALSE(oper1->is_visible(var3->get_name()));
        ASSERT_FALSE(oper2->is_visible(var3->get_name()));
        ASSERT_TRUE(oper3->is_visible(var3->get_name()));

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 66);
        EXPECT_EQ(oper_scope2->find(*var2)->execute(*oper_scope2), 66);
        EXPECT_EQ(oper_scope3->find(*var3)->execute(*oper_scope3), 66);
}

TEST (if_oper, else_statement)
{
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};

        scope_t scope {};
        auto oper_scope = new scope_t {&scope};

        auto else_scope = new scope_t {&scope};
        else_scope->insert(new assign_t {var2, new number_t {42}});

        auto oper = new if_t {new number_t {0}, else_scope, oper_scope};
        oper->insert(new assign_t {var1, new number_t {66}});
        scope.insert(oper);
        scope.execute(scope);

        scope.insert_symbs(var1, types_t::VAR);
        else_scope->insert_symbs(var2, types_t::VAR);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 1);
        EXPECT_EQ(static_cast<scope_t*>(oper->get_rhs())->find(*var2)->execute(*static_cast<scope_t*>(oper->get_rhs())), 42);
}

TEST (if_oper, cond_scope_symtable_test)
{
        auto var0 = new var_t<int> {10, "var0"};
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};

        scope_t scope {};
        auto oper_scope  = new scope_t {&scope};
        auto oper_scope2 = new scope_t {oper_scope};
        auto oper_scope3 = new scope_t {oper_scope2};

        scope.insert_symbs(var0, types_t::VAR);
        scope.insert_symbs(var1, types_t::VAR);
        oper_scope2->insert_symbs(var2, types_t::VAR);
        oper_scope3->insert_symbs(var3, types_t::VAR);

        auto oper1 = new if_t {var0, oper_scope};
        oper1->insert(new assign_t {var1, new number_t {66}});
        
        auto oper2 = new if_t      {new number_t {1}, oper_scope2};
        oper2->insert(new assign_t {var2, new number_t {66}});
        auto oper3 = new if_t { new number_t {1}, oper_scope3}; 
        oper3->insert(new assign_t {var3, new number_t {66}});

        oper1->insert(oper2);
        oper1->insert(oper3);

        scope.insert(oper1);

        ASSERT_FALSE(oper1->is_visible(var3->get_name()));
        ASSERT_FALSE(oper2->is_visible(var3->get_name()));
        ASSERT_TRUE(oper3->is_visible(var3->get_name()));

        ASSERT_TRUE(oper1->is_visible(var0->get_name()));
        ASSERT_TRUE(oper2->is_visible(var0->get_name()));
        ASSERT_TRUE(oper3->is_visible(var0->get_name()));

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 66);
        EXPECT_EQ(oper_scope2->find(*var2)->execute(*oper_scope2), 66);
        EXPECT_EQ(oper_scope3->find(*var3)->execute(*oper_scope3), 66);
}

TEST (while_oper, cycle_3_iters)
{
        auto var1 = new var_t<int> {1, "var1"};
        auto var2 = new var_t<int> {2, "var2"};
        auto var3 = new var_t<int> {3, "var3"};

        auto name1 = new name_t {"var1"};
        auto name2 = new name_t {"var2"};
        auto name3_0 = new name_t {"var3"};

        auto name3_1 = new name_t {"var3"};
        auto name3_2 = new name_t {"var3"};
        
        scope_t scope {};
        auto oper_scope = new scope_t {&scope};

        scope.insert(var1);
        scope.insert(var2);
        scope.insert(var3);

        // scope.insert_symbs(name1);
        // scope.insert_symbs(name2);
        // scope.insert_symbs(name3_0);

        auto oper = new while_t {name3_0, oper_scope};
        scope.insert(oper);

        oper->insert(new assign_t {name1, new number_t {66}});
        oper->insert(new assign_t {name2, new number_t {66}});
        oper->insert(new assign_t {name3_1, new minus_t (name3_2, new number_t {1})});

        scope.execute(scope);

        EXPECT_EQ(scope.find(*var1)->execute(scope), 66);
        EXPECT_EQ(scope.find(*var2)->execute(scope), 66);
        EXPECT_EQ(scope.find(*var3)->execute(scope), 0);
}