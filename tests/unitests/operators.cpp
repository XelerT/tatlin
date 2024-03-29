#include "config.hpp"

using namespace backend;
using namespace std;

//---------------------------------------------------~~~~~~Plus~~~~~~--------------------------------------------------------------------

TEST (plus, execution)
{
        scope_t scope {};
        
        plus_t sum {new number_t {77}, new number_t {33}};
        EXPECT_EQ(sum.execute(scope), 110);
}

TEST (plus, ref_ctor)
{
        scope_t scope {};

        plus_t sum1 {new number_t {77}, new number_t {33}};
        plus_t sum2 {sum1};
        EXPECT_NE(sum1.get_lhs(), sum2.get_lhs());
        EXPECT_NE(sum1.get_rhs(), sum2.get_rhs());

        EXPECT_EQ(sum1.execute(scope), 110);
        EXPECT_EQ(sum2.execute(scope), 110);
}

TEST (plus, ref_assignment)
{
        scope_t scope {};

        plus_t sum1 {new number_t {77}, new number_t {33}};
        plus_t sum2 {new number_t {47}, new number_t {43}};
        sum2 = sum1;
        EXPECT_EQ(sum1.execute(scope), 110);
        EXPECT_EQ(sum2.execute(scope), 110);
}

TEST (plus, move_ctor)
{
        scope_t scope {};

        plus_t sum1 {new number_t {77}, new number_t {33}};
        plus_t sum2 {std::move(sum1)};

        EXPECT_EQ(sum1.get_lhs(), nullptr);
        EXPECT_EQ(sum1.get_rhs(), nullptr);
        EXPECT_EQ(sum2.execute(scope), 110);
}

TEST (plus, move_assignment)
{
        scope_t scope {};

        plus_t sum1 {new number_t {77}, new number_t {33}};
        plus_t sum2 {new number_t {47}, new number_t {43}};
        sum2 = std::move(sum1);
        
        // EXPECT_EQ(sum1.execute(scope), 90);
        EXPECT_EQ(sum2.execute(scope), 110);
}

TEST (plus_plus_plus, execution)
{
        scope_t scope {};

        plus_t sum {new plus_t {new number_t {77}, new number_t {33}}, new plus_t {new number_t {50}, new number_t {1}}};
        EXPECT_EQ(sum.execute(scope), 161);
}

TEST (plus, dump)
{
        scope_t scope {};

        plus_t sum {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        sum.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 + 2");
}

//---------------------------------------------------~~~~~~Minus~~~~~~--------------------------------------------------------------------

TEST (minus, execution)
{
        scope_t scope {};

        minus_t sub {new number_t {77}, new number_t {33}};
        EXPECT_EQ(sub.execute(scope), 44);
}

TEST (minus, ref_ctor)
{
        scope_t scope {};

        minus_t sub1 {new number_t {77}, new number_t {33}};
        minus_t sub2 {sub1};
        EXPECT_NE(sub1.get_lhs(), sub2.get_lhs());
        EXPECT_NE(sub1.get_rhs(), sub2.get_rhs());

        EXPECT_EQ(sub1.execute(scope), 44);
        EXPECT_EQ(sub2.execute(scope), 44);
}

TEST (minus, ref_assignment)
{
        scope_t scope {};

        minus_t sub1 {new number_t {77}, new number_t {33}};
        minus_t sub2 {new number_t {47}, new number_t {43}};
        sub2 = sub1;
        EXPECT_EQ(sub1.execute(scope), 44);
        EXPECT_EQ(sub2.execute(scope), 44);
}

TEST (minus, move_ctor)
{
        scope_t scope {};

        minus_t sub1 {new number_t {77}, new number_t {33}};
        minus_t sub2 {std::move(sub1)};
        EXPECT_EQ(sub1.get_lhs(), nullptr);
        EXPECT_EQ(sub1.get_rhs(), nullptr);
        EXPECT_EQ(sub2.execute(scope), 44);
}

TEST (minus, move_assignment)
{
        scope_t scope {};

        minus_t sub1 {new number_t {77}, new number_t {33}};
        minus_t sub2 {new number_t {47}, new number_t {43}};
        sub2 = std::move(sub1);
        
        // EXPECT_EQ(sub1.execute(scope), 4);
        EXPECT_EQ(sub2.execute(scope), 44);
}

TEST (minus_minus_minus, execution)
{
        scope_t scope {};

        minus_t sub {new minus_t {new number_t {77}, new number_t {33}},
                     new minus_t {new number_t {50}, new number_t {1}}};
        EXPECT_EQ(sub.execute(scope), -5);
}

TEST (minus, dump)
{
        scope_t scope {};

        minus_t sub {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        sub.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 - 2");
}

//---------------------------------------------------~~~~~~Mul~~~~~~--------------------------------------------------------------------

TEST (mul, execution)
{
        scope_t scope {};

        mul_t mul {new number_t {77}, new number_t {2}};
        EXPECT_EQ(mul.execute(scope), 154);
}

TEST (mul, ref_ctor)
{
        scope_t scope {};

        mul_t mul1 {new number_t {77}, new number_t {2}};
        mul_t mul2 {mul1};
        EXPECT_NE(mul1.get_lhs(), mul2.get_lhs());
        EXPECT_NE(mul1.get_rhs(), mul2.get_rhs());

        EXPECT_EQ(mul1.execute(scope), 154);
        EXPECT_EQ(mul2.execute(scope), 154);
}

TEST (mul, ref_assignment)
{
        scope_t scope {};

        mul_t mul1 {new number_t {77}, new number_t {2}};
        mul_t mul2 {new number_t {47}, new number_t {43}};
        mul2 = mul1;
        EXPECT_EQ(mul1.execute(scope), 154);
        EXPECT_EQ(mul2.execute(scope), 154);
}

TEST (mul, move_ctor)
{
        scope_t scope {};

        mul_t mul1 {new number_t {77}, new number_t {2}};
        mul_t mul2 {std::move(mul1)};
        EXPECT_EQ(mul1.get_lhs(), nullptr);
        EXPECT_EQ(mul1.get_rhs(), nullptr);
        EXPECT_EQ(mul2.execute(scope), 154);
}

TEST (mul, move_assignment)
{
        scope_t scope {};

        mul_t mul1 {new number_t {77}, new number_t {0}};
        mul_t mul2 {new number_t {47}, new number_t {-1}};
        mul2 = std::move(mul1);

        // EXPECT_EQ(mul1.execute(scope), -47);
        EXPECT_EQ(mul2.execute(scope), 0);
}

TEST (mul_mul_mul, execution)
{
        scope_t scope {};

        mul_t mul {new mul_t {new number_t {77}, new number_t {2}},
                   new mul_t {new number_t {3}, new number_t {4}}};
        EXPECT_EQ(mul.execute(scope), 1848);
}

TEST (mul, dump)
{
        scope_t scope {};

        mul_t mul {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        mul.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 * 2");
}

//---------------------------------------------------~~~~~~Div~~~~~~--------------------------------------------------------------------

TEST (div, execution)
{
        scope_t scope {};

        backend::div_t div {new number_t {77}, new number_t {2}};
        EXPECT_EQ(div.execute(scope), 38);
}

TEST (div, ref_ctor)
{
        scope_t scope {};

        backend::div_t div1 {new number_t {77}, new number_t {2}};
        backend::div_t div2 {div1};
        EXPECT_NE(div1.get_lhs(), div2.get_lhs());
        EXPECT_NE(div1.get_rhs(), div2.get_rhs());

        EXPECT_EQ(div1.execute(scope), 38);
        EXPECT_EQ(div2.execute(scope), 38);
}

TEST (div, ref_assignment)
{
        scope_t scope {};

        backend::div_t div1 {new number_t {77}, new number_t {2}};
        backend::div_t div2 {new number_t {47}, new number_t {43}};
        div2 = div1;
        EXPECT_EQ(div1.execute(scope), 38);
        EXPECT_EQ(div2.execute(scope), 38);
}

TEST (div, move_ctor)
{
        scope_t scope {};

        backend::div_t div1 {new number_t {77}, new number_t {2}};
        backend::div_t div2 {std::move(div1)};
        EXPECT_EQ(div1.get_lhs(), nullptr);
        EXPECT_EQ(div1.get_rhs(), nullptr);
        EXPECT_EQ(div2.execute(scope), 38);
}

TEST (div, move_assignment)
{
        scope_t scope {};

        backend::div_t div1 {new number_t {77}, new number_t {77}};
        backend::div_t div2 {new number_t {47}, new number_t {-1}};
        div2 = std::move(div1);

        // EXPECT_EQ(div1.execute(scope), -47);
        EXPECT_EQ(div2.execute(scope), 1);
}

TEST (div_div_div, execution)
{
        scope_t scope {};

        backend::div_t div {new backend::div_t {new number_t {77}, new number_t {2}}, 
                            new backend::div_t {new number_t {4}, new number_t {3}}};
        EXPECT_EQ(div.execute(scope), 38);
}

TEST (div, dump)
{
        scope_t scope {};

        backend::div_t div {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        div.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 / 2");
}

//---------------------------------------------------~~~~~~Modulo~~~~~~--------------------------------------------------------------------

TEST (modulo, execution)
{
        scope_t scope {};

        backend::modulo_t modulo {new number_t {77}, new number_t {2}};
        EXPECT_EQ(modulo.execute(scope), 1);
}

TEST (modulo, ref_ctor)
{
        scope_t scope {};

        modulo_t modulo1 {new number_t {77}, new number_t {2}};
        modulo_t modulo2 {modulo1};
        EXPECT_NE(modulo1.get_lhs(), modulo2.get_lhs());
        EXPECT_NE(modulo1.get_rhs(), modulo2.get_rhs());

        EXPECT_EQ(modulo1.execute(scope), 1);
        EXPECT_EQ(modulo2.execute(scope), 1);
}

TEST (modulo, ref_assignment)
{
        scope_t scope {};

        modulo_t modulo1 {new number_t {77}, new number_t {2}};
        modulo_t modulo2 {new number_t {47}, new number_t {43}};
        modulo2 = modulo1;
        EXPECT_EQ(modulo1.execute(scope), 1);
        EXPECT_EQ(modulo2.execute(scope), 1);
}

TEST (modulo, move_ctor)
{
        scope_t scope {};

        backend::modulo_t modulo1 {new number_t {77}, new number_t {2}};
        backend::modulo_t modulo2 {std::move(modulo1)};
        EXPECT_EQ(modulo1.get_lhs(), nullptr);
        EXPECT_EQ(modulo1.get_rhs(), nullptr);
        EXPECT_EQ(modulo2.execute(scope), 1);
}

TEST (modulo, move_assignment)
{
        scope_t scope {};

        backend::modulo_t modulo1 {new number_t {77}, new number_t {77}};
        backend::modulo_t modulo2 {new number_t {47}, new number_t {-1}};
        modulo2 = std::move(modulo1);

        // EXPECT_EQ(modulo1.execute(scope), 0);
        EXPECT_EQ(modulo2.execute(scope), 0);
}

TEST (modulo_modulo_modulo, execution)
{
        scope_t scope {};

        backend::modulo_t modulo {new backend::modulo_t {new number_t {77}, new number_t {2}}, 
                                  new backend::modulo_t {new number_t {4}, new number_t {3}}};
        EXPECT_EQ(modulo.execute(scope), 0);
}

TEST (modulo, dump)
{
        scope_t scope {};

        backend::modulo_t modulo {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        modulo.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 % 2");
}

//---------------------------------------------------~~~~~~Assignment~~~~~~--------------------------------------------------------------------

TEST (assign, execution_name_number)
{
        scope_t scope {};

        auto name = new var_t {77, "var1"};
        auto ass = new assign_t {name, new number_t {33}};
        scope.insert(ass);
        scope.insert_symbs(name, types_t::VAR);

        EXPECT_EQ(scope.execute(scope), 0);
        EXPECT_EQ(scope.find(*name)->execute(scope), 33);
}

TEST (assign, execution_name_name)
{
        scope_t scope {};

        auto name1 = new var_t {77, "var1"};
        auto name2 = new var_t {33, "var2"};

        auto ass = new assign_t {name1, name2};

        scope.insert(ass);
        scope.insert_symbs(name1, types_t::VAR);
        scope.insert_symbs(name2, types_t::VAR);

        EXPECT_EQ(scope.execute(scope), 0);
        EXPECT_EQ(scope.find(*name1)->execute(scope), 33);
        EXPECT_EQ(scope.find(*name2)->execute(scope), 33);
}

TEST (assign, ref_ctor)
{
        scope_t scope {};

        auto name = new var_t {77, "var1"};
        assign_t ass1 {name, new number_t {33}};
        assign_t ass2 {ass1};

        EXPECT_NE(ass1.get_lhs(), ass2.get_lhs());
        EXPECT_NE(ass1.get_rhs(), ass2.get_rhs());

        EXPECT_EQ(ass1.execute(scope), 33);
        EXPECT_EQ(ass2.execute(scope), 33);
}

TEST (assign, ref_assignment)
{
        scope_t scope {};

        auto name = new var_t {77, "var1"};
        assign_t ass1 {name, new number_t {33}};
        auto name2 = new var_t {77, "var2"};
        assign_t ass2 {name2, new number_t {33}};
        ass2 = ass1;

        EXPECT_EQ(ass1.execute(scope), 33);
        EXPECT_EQ(ass2.execute(scope), 33);
}

TEST (assign, move_ctor)
{
        scope_t scope {};

        auto name = new var_t {77, "var1"};
        assign_t ass1 {name, new number_t {33}};
        assign_t ass2 {std::move(ass1)};

        EXPECT_EQ(ass1.get_lhs(), nullptr);
        EXPECT_EQ(ass1.get_rhs(), nullptr);
        EXPECT_EQ(ass2.execute(scope), 33);
}

TEST (assign, move_assignment)
{
        scope_t scope {};

        auto name1 = new var_t {77, "var1"};
        auto name2 = new var_t {66, "var1"};

        assign_t ass1 {name1, new number_t {33}};
        assign_t ass2 {name2, new number_t {55}};
        ass2 = std::move(ass1);

        // EXPECT_EQ(ass1.execute(scope), 55);
        EXPECT_EQ(ass2.execute(scope), 33);
}

TEST (assign_assign_assign, execution)
{
        scope_t scope {};

        assign_t ass {new assign_t {new number_t {77}, new number_t {33}}, new assign_t {new number_t {50}, new number_t {1}}};
        EXPECT_ANY_THROW(ass.execute(scope));
}

TEST (assign, dump)
{
        scope_t scope {};

        assign_t ass {new number_t {40}, new number_t {2}};

        testing::internal::CaptureStdout();
        ass.dump();
        string output = testing::internal::GetCapturedStdout();

        EXPECT_EQ(output, "40 = 2");
}