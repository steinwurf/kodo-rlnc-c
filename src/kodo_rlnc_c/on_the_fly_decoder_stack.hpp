// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <kodo_core/basic_factory.hpp>
#include <kodo_core/basic_proxy_stack.hpp>
#include <kodo_core/coefficient_storage_layers.hpp>
#include <kodo_core/common_decoder_layers.hpp>
#include <kodo_core/final_layer.hpp>
#include <kodo_core/finite_field.hpp>
#include <kodo_core/mutable_shallow_storage_layers.hpp>
#include <kodo_core/nested_set_seed.hpp>
#include <kodo_core/nested_write_payload.hpp>
#include <kodo_core/partial_decoding_tracker.hpp>
#include <kodo_core/payload_info.hpp>
#include <kodo_core/payload_rank_decoder.hpp>
#include <kodo_core/plain_symbol_id_reader.hpp>
#include <kodo_core/plain_symbol_id_size.hpp>
#include <kodo_core/proxy_args.hpp>
#include <kodo_core/rank_symbol_decoding_status_updater.hpp>
#include <kodo_core/symbol_decoding_status_updater.hpp>
#include <kodo_core/symbol_id_decoder.hpp>
#include <kodo_core/systematic_decoder_layers.hpp>
#include <kodo_core/trace_layer.hpp>

#include <kodo_rlnc/on_the_fly_recoding_stack.hpp>

namespace kodo_rlnc_c
{
using on_the_fly_decoder_stack = kodo_core::nested_write_payload<
    kodo_core::nested_set_seed<
    kodo_core::basic_proxy_stack<
    kodo_core::proxy_args<>, kodo_rlnc::on_the_fly_recoding_stack,
    kodo_core::partial_decoding_tracker<
    kodo_core::rank_symbol_decoding_status_updater<
    kodo_core::payload_rank_decoder<
    kodo_core::payload_info<
    // Codec Header API
    kodo_core::systematic_decoder_layers<
    kodo_core::symbol_id_decoder<
    // Symbol ID API
    kodo_core::plain_symbol_id_reader<
    kodo_core::plain_symbol_id_size<
    // Decoder API
    kodo_core::symbol_decoding_status_updater<
    kodo_core::common_decoder_layers<
    // Coefficient Storage API
    kodo_core::coefficient_storage_layers<
    // Storage API
    kodo_core::mutable_shallow_storage_layers<
    // Finite Field API
    kodo_core::finite_field<
    // Trace Layer
    kodo_core::trace_layer<
    // Final Layer
    kodo_core::final_layer
    >>>>>>>>>>>>>>>>>;
}
