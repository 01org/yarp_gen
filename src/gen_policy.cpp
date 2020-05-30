/*
Copyright (c) 2015-2020, Intel Corporation
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//////////////////////////////////////////////////////////////////////////////

#include "gen_policy.h"

using namespace yarpgen;

size_t GenPolicy::leaves_prob_bump = 30;

GenPolicy::GenPolicy() {
    stmt_num_lim = 1000;

    loop_seq_num_lim = 4;
    uniformProbFromMax(loop_seq_num_distr, loop_seq_num_lim, 1);

    loop_nest_depth_lim = 3;
    uniformProbFromMax(loop_nest_depth_distr, loop_nest_depth_lim, 2);

    loop_depth_limit = 5;

    if_else_depth_limit = 5;

    scope_stmt_min_num = 2;
    scope_stmt_max_num = 5;
    uniformProbFromMax(scope_stmt_num_distr, scope_stmt_max_num,
                       scope_stmt_min_num);

    min_iters_num = 1;
    max_iters_num = 1;
    uniformProbFromMax(iters_num_distr, max_iters_num, min_iters_num);

    iters_end_limit_min = 10;
    iter_end_limit_max = 25;
    iters_step_distr.emplace_back(Probability<size_t>{1, 10});
    iters_step_distr.emplace_back(Probability<size_t>{2, 10});
    iters_step_distr.emplace_back(Probability<size_t>{4, 10});

    stmt_kind_struct_distr.emplace_back(
        Probability<IRNodeKind>{IRNodeKind::LOOP_SEQ, 10});
    stmt_kind_struct_distr.emplace_back(
        Probability<IRNodeKind>{IRNodeKind::LOOP_NEST, 10});
    stmt_kind_struct_distr.emplace_back(
        Probability<IRNodeKind>{IRNodeKind::IF_ELSE, 10});
    stmt_kind_struct_distr.emplace_back(
        Probability<IRNodeKind>{IRNodeKind::STUB, 70});

    else_br_distr.emplace_back(Probability<bool>{true, 20});
    else_br_distr.emplace_back(Probability<bool>{false, 80});

    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::BOOL, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::SCHAR, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::UCHAR, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::SHORT, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::USHORT, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::INT, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::UINT, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::LLONG, 10));
    int_type_distr.emplace_back(Probability<IntTypeID>(IntTypeID::ULLONG, 10));

    min_inp_vars_num = 10;
    max_inp_vars_num = 20;

    stmt_kind_pop_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::ASSIGN, 20));

    unary_op_distr.emplace_back(Probability<UnaryOp>(UnaryOp::NEGATE, 20));

    min_new_arr_num = 2;
    max_new_arr_num = 4;
    uniformProbFromMax(new_arr_num_distr, max_new_arr_num, min_new_arr_num);

    out_kind_distr.emplace_back(Probability<DataKind>(DataKind::VAR, 20));
    out_kind_distr.emplace_back(Probability<DataKind>(DataKind::ARR, 20));

    max_arith_depth = 3;

    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::CONST, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::SCALAR_VAR_USE, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::SUBSCRIPT, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::TYPE_CAST, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::UNARY, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::BINARY, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::CALL, 20));
    arith_node_distr.emplace_back(
        Probability<IRNodeKind>(IRNodeKind::TERNARY, 20));

    unary_op_distr.emplace_back(Probability<UnaryOp>(UnaryOp::PLUS, 25));
    unary_op_distr.emplace_back(Probability<UnaryOp>(UnaryOp::NEGATE, 25));
    unary_op_distr.emplace_back(Probability<UnaryOp>(UnaryOp::LOG_NOT, 25));
    unary_op_distr.emplace_back(Probability<UnaryOp>(UnaryOp::BIT_NOT, 25));

    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::ADD, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::SUB, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::MUL, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::DIV, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::MOD, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::LT, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::GT, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::LE, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::GE, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::EQ, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::NE, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::LOG_AND, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::LOG_OR, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::BIT_AND, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::BIT_OR, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::BIT_XOR, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::SHL, 10));
    binary_op_distr.emplace_back(Probability<BinaryOp>(BinaryOp::SHR, 10));

    foreach_distr.emplace_back(Probability<bool>(true, 20));
    foreach_distr.emplace_back(Probability<bool>(false, 80));

    cxx_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::MAX, 20));
    cxx_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::MIN, 20));

    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::MAX, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::MIN, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::SELECT, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::ANY, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::ALL, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::NONE, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::RED_MIN, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::RED_MAX, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::RED_EQ, 20));
    ispc_lib_call_distr.emplace_back(
        Probability<LibCallKind>(LibCallKind::EXTRACT, 20));

    loop_end_kind_distr.emplace_back(
        Probability<LoopEndKind>(LoopEndKind::CONST, 30));
    loop_end_kind_distr.emplace_back(
        Probability<LoopEndKind>(LoopEndKind::VAR, 30));
    loop_end_kind_distr.emplace_back(
        Probability<LoopEndKind>(LoopEndKind::EXPR, 30));
}

template <typename T>
void GenPolicy::uniformProbFromMax(std::vector<Probability<T>> &distr,
                                   size_t max_num, size_t min_num) {
    distr.reserve(max_num - min_num);
    for (size_t i = min_num; i <= max_num; ++i)
        distr.emplace_back(i, (max_num - i + 1) * 10);
}
