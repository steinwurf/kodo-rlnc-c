// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <kodo_rlnc/coders.hpp>

inline fifi::finite_field c_field_to_krlnc_field(int32_t field)
{
    switch (field)
    {
    case krlnc_binary:
        return fifi::finite_field::binary;
    case krlnc_binary4:
        return fifi::finite_field::binary4;
    case krlnc_binary8:
        return fifi::finite_field::binary8;
    case krlnc_binary16:
        return fifi::finite_field::binary16;
    default:
        assert(false && "Unknown field");
        return fifi::finite_field::binary;
    }
}

inline kodo_rlnc::coding_vector_format c_format_to_krlnc_format(int32_t format)
{
    switch (format)
    {
    case krlnc_full_vector:
        return kodo_rlnc::coding_vector_format::full_vector;
    case krlnc_seed:
        return kodo_rlnc::coding_vector_format::seed;
    case krlnc_sparse_seed:
        return kodo_rlnc::coding_vector_format::sparse_seed;
    default:
        assert(false && "Unknown coding vector format");
        return kodo_rlnc::coding_vector_format::full_vector;
    }
}
