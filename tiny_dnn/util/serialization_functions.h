/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <cereal/access.hpp>  // For LoadAndConstruct
#include "tiny_dnn/tiny_dnn.h"

typedef tiny_dnn::index3d<tiny_dnn::serial_size_t> shape3d_serial;

static tiny_dnn::serial_size_t cast_size_t(const size_t &src) {
  return static_cast<tiny_dnn::serial_size_t>(src);
}

static size_t cast_size_t(const tiny_dnn::serial_size_t &src) {
  return static_cast<size_t>(src);
}

static shape3d_serial cast_size_t(const tiny_dnn::shape3d &src) {
  shape3d_serial dst;
  dst.width_  = cast_size_t(src.width_);
  dst.height_ = cast_size_t(src.height_);
  dst.depth_  = cast_size_t(src.depth_);
  return dst;
}

static tiny_dnn::shape3d cast_size_t(const shape3d_serial &src) {
  tiny_dnn::shape3d dst;
  dst.width_  = cast_size_t(src.width_);
  dst.height_ = cast_size_t(src.height_);
  dst.depth_  = cast_size_t(src.depth_);
  return dst;
}

namespace cereal {

template <>
struct LoadAndConstruct<tiny_dnn::elementwise_add_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::elementwise_add_layer> &construct) {
    tiny_dnn::serial_size_t num_args, dim;

    ar(cereal::make_nvp("num_args", num_args), cereal::make_nvp("dim", dim));
    construct(cast_size_t(num_args), cast_size_t(dim));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::average_pooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::average_pooling_layer> &construct) {
    shape3d_serial in;
    tiny_dnn::serial_size_t stride_x, stride_y, pool_size_x, pool_size_y;
    tiny_dnn::padding pad_type;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("pool_size_x", pool_size_x),
       cereal::make_nvp("pool_size_y", pool_size_y),
       cereal::make_nvp("stride_x", stride_x),
       cereal::make_nvp("stride_y", stride_y),
       cereal::make_nvp("pad_type", pad_type));
    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(in.depth_), cast_size_t(pool_size_x),
              cast_size_t(pool_size_y), cast_size_t(stride_x),
              cast_size_t(stride_y), pad_type);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::average_unpooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::average_unpooling_layer> &construct) {
    shape3d_serial in;
    tiny_dnn::serial_size_t pool_size, stride;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("pool_size", pool_size),
       cereal::make_nvp("stride", stride));
    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(in.depth_), cast_size_t(pool_size),
              cast_size_t(stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::batch_normalization_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::batch_normalization_layer> &construct) {
    shape3d_serial in;
    tiny_dnn::serial_size_t in_spatial_size, in_channels;
    tiny_dnn::float_t eps, momentum;
    tiny_dnn::net_phase phase;
    tiny_dnn::vec_t mean, variance;

    ar(cereal::make_nvp("in_spatial_size", in_spatial_size),
       cereal::make_nvp("in_channels", in_channels),
       cereal::make_nvp("epsilon", eps), cereal::make_nvp("momentum", momentum),
       cereal::make_nvp("phase", phase), cereal::make_nvp("mean", mean),
       cereal::make_nvp("variance", variance));
    construct(cast_size_t(in_spatial_size), cast_size_t(in_channels), eps,
              momentum, phase);
    construct->set_mean(mean);
    construct->set_variance(variance);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::concat_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::concat_layer> &construct) {
    std::vector<shape3d_serial> in_shapes_serial;
    ar(cereal::make_nvp("in_size", in_shapes_serial));

    std::vector<tiny_dnn::shape3d> in_shapes;
    for (const auto &shape : in_shapes_serial) {
      in_shapes.push_back(cast_size_t(shape));
    }
    construct(in_shapes);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::convolutional_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::convolutional_layer> &construct) {
    tiny_dnn::serial_size_t w_width, w_height, out_ch, w_stride, h_stride;
    bool has_bias;
    shape3d_serial in;
    tiny_dnn::padding pad_type;
    tiny_dnn::core::connection_table tbl;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("window_width", w_width),
       cereal::make_nvp("window_height", w_height),
       cereal::make_nvp("out_channels", out_ch),
       cereal::make_nvp("connection_table", tbl),
       cereal::make_nvp("pad_type", pad_type),
       cereal::make_nvp("has_bias", has_bias),
       cereal::make_nvp("w_stride", w_stride),
       cereal::make_nvp("h_stride", h_stride));

    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(w_width), cast_size_t(w_height),
              cast_size_t(in.depth_), cast_size_t(out_ch), tbl, pad_type,
              has_bias, cast_size_t(w_stride), cast_size_t(h_stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::deconvolutional_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::deconvolutional_layer> &construct) {
    tiny_dnn::serial_size_t w_width, w_height, out_ch, w_stride, h_stride;
    bool has_bias;
    shape3d_serial in;
    tiny_dnn::padding pad_type;
    tiny_dnn::core::connection_table tbl;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("window_width", w_width),
       cereal::make_nvp("window_height", w_height),
       cereal::make_nvp("out_channels", out_ch),
       cereal::make_nvp("connection_table", tbl),
       cereal::make_nvp("pad_type", pad_type),
       cereal::make_nvp("has_bias", has_bias),
       cereal::make_nvp("w_stride", w_stride),
       cereal::make_nvp("h_stride", h_stride));

    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(w_width), cast_size_t(w_height),
              cast_size_t(in.depth_), cast_size_t(out_ch), tbl, pad_type,
              has_bias, cast_size_t(w_stride), cast_size_t(h_stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::dropout_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::dropout_layer> &construct) {
    tiny_dnn::net_phase phase;
    tiny_dnn::float_t dropout_rate;
    tiny_dnn::serial_size_t in_size;

    ar(cereal::make_nvp("in_size", in_size),
       cereal::make_nvp("dropout_rate", dropout_rate),
       cereal::make_nvp("phase", phase));
    construct(cast_size_t(in_size), dropout_rate, phase);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::fully_connected_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::fully_connected_layer> &construct) {
    tiny_dnn::serial_size_t in_dim, out_dim;
    bool has_bias;

    ar(cereal::make_nvp("in_size", in_dim),
       cereal::make_nvp("out_size", out_dim),
       cereal::make_nvp("has_bias", has_bias));
    construct(cast_size_t(in_dim), cast_size_t(out_dim), has_bias);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::global_average_pooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::global_average_pooling_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_shape", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::input_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::input_layer> &construct) {
    shape3d_serial shape;

    ar(cereal::make_nvp("shape", shape));
    construct(cast_size_t(shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::linear_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::linear_layer> &construct) {
    tiny_dnn::serial_size_t dim;
    tiny_dnn::float_t scale, bias;

    ar(cereal::make_nvp("in_size", dim), cereal::make_nvp("scale", scale),
       cereal::make_nvp("bias", bias));

    construct(cast_size_t(dim), scale, bias);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::lrn_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::lrn_layer> &construct) {
    shape3d_serial in_shape;
    tiny_dnn::serial_size_t size;
    tiny_dnn::float_t alpha, beta;
    tiny_dnn::norm_region region;

    ar(cereal::make_nvp("in_shape", in_shape), cereal::make_nvp("size", size),
       cereal::make_nvp("alpha", alpha), cereal::make_nvp("beta", beta),
       cereal::make_nvp("region", region));
    construct(cast_size_t(in_shape), cast_size_t(size), alpha, beta, region);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::max_pooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::max_pooling_layer> &construct) {
    shape3d_serial in;
    tiny_dnn::serial_size_t stride_x, stride_y, pool_size_x, pool_size_y;
    tiny_dnn::padding pad_type;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("pool_size_x", pool_size_x),
       cereal::make_nvp("pool_size_y", pool_size_y),
       cereal::make_nvp("stride_x", stride_x),
       cereal::make_nvp("stride_y", stride_y),
       cereal::make_nvp("pad_type", pad_type));
    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(in.depth_), cast_size_t(pool_size_x),
              cast_size_t(pool_size_y), cast_size_t(stride_x),
              cast_size_t(stride_y), pad_type);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::max_unpooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::max_unpooling_layer> &construct) {
    shape3d_serial in;
    tiny_dnn::serial_size_t stride, unpool_size;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("unpool_size", unpool_size),
       cereal::make_nvp("stride", stride));
    construct(cast_size_t(in), cast_size_t(unpool_size), cast_size_t(stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::power_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::power_layer> &construct) {
    shape3d_serial in_shape;
    tiny_dnn::float_t factor;
    tiny_dnn::float_t scale(1.0f);

    ar(cereal::make_nvp("in_size", in_shape),
       cereal::make_nvp("factor", factor), cereal::make_nvp("scale", scale));
    construct(cast_size_t(in_shape), factor, scale);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::quantized_convolutional_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::quantized_convolutional_layer> &construct) {
    tiny_dnn::serial_size_t w_width, w_height, out_ch, w_stride, h_stride;
    bool has_bias;
    shape3d_serial in;
    tiny_dnn::padding pad_type;
    tiny_dnn::core::connection_table tbl;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("window_width", w_width),
       cereal::make_nvp("window_height", w_height),
       cereal::make_nvp("out_channels", out_ch),
       cereal::make_nvp("connection_table", tbl),
       cereal::make_nvp("pad_type", pad_type),
       cereal::make_nvp("has_bias", has_bias),
       cereal::make_nvp("w_stride", w_stride),
       cereal::make_nvp("h_stride", h_stride));

    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(w_width), cast_size_t(w_height),
              cast_size_t(in.depth_), cast_size_t(out_ch), tbl, pad_type,
              has_bias, cast_size_t(w_stride), cast_size_t(h_stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::quantized_deconvolutional_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::quantized_deconvolutional_layer> &construct) {
    tiny_dnn::serial_size_t w_width, w_height, out_ch, w_stride, h_stride;
    bool has_bias;
    shape3d_serial in;
    tiny_dnn::padding pad_type;
    tiny_dnn::core::connection_table tbl;

    ar(cereal::make_nvp("in_size", in),
       cereal::make_nvp("window_width", w_width),
       cereal::make_nvp("window_height", w_height),
       cereal::make_nvp("out_channels", out_ch),
       cereal::make_nvp("connection_table", tbl),
       cereal::make_nvp("pad_type", pad_type),
       cereal::make_nvp("has_bias", has_bias),
       cereal::make_nvp("w_stride", w_stride),
       cereal::make_nvp("h_stride", h_stride));

    construct(cast_size_t(in.width_), cast_size_t(in.height_),
              cast_size_t(w_width), cast_size_t(w_height),
              cast_size_t(in.depth_), cast_size_t(out_ch), tbl, pad_type,
              has_bias, cast_size_t(w_stride), cast_size_t(h_stride));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::quantized_fully_connected_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::quantized_fully_connected_layer> &construct) {
    tiny_dnn::serial_size_t in_dim, out_dim;
    bool has_bias;

    ar(cereal::make_nvp("in_size", in_dim),
       cereal::make_nvp("out_size", out_dim),
       cereal::make_nvp("has_bias", has_bias));
    construct(cast_size_t(in_dim), cast_size_t(out_dim), has_bias);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::slice_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::slice_layer> &construct) {
    shape3d_serial in_shape;
    tiny_dnn::slice_type slice_type;
    tiny_dnn::serial_size_t num_outputs;

    ar(cereal::make_nvp("in_size", in_shape),
       cereal::make_nvp("slice_type", slice_type),
       cereal::make_nvp("num_outputs", num_outputs));
    construct(cast_size_t(in_shape), slice_type, cast_size_t(num_outputs));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::sigmoid_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::sigmoid_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::tanh_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::tanh_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::relu_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::relu_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::softmax_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::softmax_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::leaky_relu_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::leaky_relu_layer> &construct) {
    shape3d_serial in_shape;
    tiny_dnn::float_t epsilon;

    ar(cereal::make_nvp("in_size", in_shape),
       cereal::make_nvp("epsilon", epsilon));
    construct(cast_size_t(in_shape), epsilon);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::elu_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::elu_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::tanh_p1m2_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::tanh_p1m2_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::softplus_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::softplus_layer> &construct) {
    shape3d_serial in_shape;
    tiny_dnn::float_t beta;
    tiny_dnn::float_t threshold;

    ar(cereal::make_nvp("in_size", in_shape), cereal::make_nvp("beta", beta),
       cereal::make_nvp("threshold", threshold));
    construct(cast_size_t(in_shape), beta, threshold);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::softsign_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::softsign_layer> &construct) {
    shape3d_serial in_shape;

    ar(cereal::make_nvp("in_size", in_shape));
    construct(cast_size_t(in_shape));
  }
};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::elementwise_add_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::average_pooling_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::average_unpooling_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::batch_normalization_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::concat_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::convolutional_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::deconvolutional_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::dropout_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::fully_connected_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::global_average_pooling_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::input_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::linear_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::lrn_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::max_pooling_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::max_unpooling_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::power_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::quantized_convolutional_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::quantized_deconvolutional_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::quantized_fully_connected_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::slice_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::sigmoid_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::tanh_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::relu_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::softmax_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::leaky_relu_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::elu_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::tanh_p1m2_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::softplus_layer,
                  cereal::specialization::non_member_serialize> {};

template <class Archive>
struct specialize<Archive,
                  tiny_dnn::softsign_layer,
                  cereal::specialization::non_member_serialize> {};

}  // namespace cereal

namespace tiny_dnn {

struct serialization_buddy {
#ifndef CNN_NO_SERIALIZATION
  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::elementwise_add_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("num_args", cast_size_t(layer.num_args_)),
       cereal::make_nvp("dim", cast_size_t(layer.dim_)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::average_pooling_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_)),
       cereal::make_nvp("pool_size_x", cast_size_t(layer.pool_size_x_)),
       cereal::make_nvp("pool_size_y", cast_size_t(layer.pool_size_y_)),
       cereal::make_nvp("stride_x", cast_size_t(layer.stride_x_)),
       cereal::make_nvp("stride_y", cast_size_t(layer.stride_y_)),
       cereal::make_nvp("pad_type", layer.pad_type_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::average_unpooling_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_)),
       cereal::make_nvp("pool_size", cast_size_t(layer.w_.width_)),
       cereal::make_nvp("stride", cast_size_t(layer.stride_)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::batch_normalization_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_spatial_size", cast_size_t(layer.in_spatial_size_)),
       cereal::make_nvp("in_channels", cast_size_t(layer.in_channels_)),
       cereal::make_nvp("epsilon", layer.eps_),
       cereal::make_nvp("momentum", layer.momentum_),
       cereal::make_nvp("phase", layer.phase_),
       cereal::make_nvp("mean", layer.mean_),
       cereal::make_nvp("variance", layer.variance_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::concat_layer &layer) {
    layer.serialize_prolog(ar);
    ar(layer.in_shapes_);
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::convolutional_layer &layer) {
    layer.serialize_prolog(ar);

    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in)),
       cereal::make_nvp("window_width", cast_size_t(params_.weight.width_)),
       cereal::make_nvp("window_height", cast_size_t(params_.weight.height_)),
       cereal::make_nvp("out_channels", cast_size_t(params_.out.depth_)),
       cereal::make_nvp("connection_table", params_.tbl),
       cereal::make_nvp("pad_type", params_.pad_type),
       cereal::make_nvp("has_bias", params_.has_bias),
       cereal::make_nvp("w_stride", cast_size_t(params_.w_stride)),
       cereal::make_nvp("h_stride", cast_size_t(params_.h_stride)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::deconvolutional_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in)),
       cereal::make_nvp("window_width", cast_size_t(params_.weight.width_)),
       cereal::make_nvp("window_height", cast_size_t(params_.weight.height_)),
       cereal::make_nvp("out_channels", cast_size_t(params_.out.depth_)),
       cereal::make_nvp("connection_table", params_.tbl),
       cereal::make_nvp("pad_type", params_.pad_type),
       cereal::make_nvp("has_bias", params_.has_bias),
       cereal::make_nvp("w_stride", cast_size_t(params_.w_stride)),
       cereal::make_nvp("h_stride", cast_size_t(params_.h_stride)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::dropout_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_size_)),
       cereal::make_nvp("dropout_rate", layer.dropout_rate_),
       cereal::make_nvp("phase", layer.phase_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::fully_connected_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in_size_)),
       cereal::make_nvp("out_size", cast_size_t(params_.out_size_)),
       cereal::make_nvp("has_bias", params_.has_bias_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::global_average_pooling_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_shape", cast_size_t(params_.in)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::input_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("shape", cast_size_t(layer.shape_)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::linear_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.dim_)),
       cereal::make_nvp("scale", layer.scale_),
       cereal::make_nvp("bias", layer.bias_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::lrn_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_shape", cast_size_t(layer.in_shape_)),
       cereal::make_nvp("size", cast_size_t(layer.size_)),
       cereal::make_nvp("alpha", layer.alpha_),
       cereal::make_nvp("beta", layer.beta_),
       cereal::make_nvp("region", layer.region_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::max_pooling_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in)),
       cereal::make_nvp("pool_size_x", cast_size_t(params_.pool_size_x)),
       cereal::make_nvp("pool_size_y", cast_size_t(params_.pool_size_y)),
       cereal::make_nvp("stride_x", cast_size_t(params_.stride_x)),
       cereal::make_nvp("stride_y", cast_size_t(params_.stride_y)),
       cereal::make_nvp("pad_type", params_.pad_type));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::max_unpooling_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_)),
       cereal::make_nvp("unpool_size", cast_size_t(layer.unpool_size_)),
       cereal::make_nvp("stride", cast_size_t(layer.stride_)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::power_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape_)),
       cereal::make_nvp("factor", layer.factor_),
       cereal::make_nvp("scale", layer.scale_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::quantized_convolutional_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in)),
       cereal::make_nvp("window_width", cast_size_t(params_.weight.width_)),
       cereal::make_nvp("window_height", cast_size_t(params_.weight.height_)),
       cereal::make_nvp("out_channels", cast_size_t(params_.out.depth_)),
       cereal::make_nvp("connection_table", params_.tbl),
       cereal::make_nvp("pad_type", params_.pad_type),
       cereal::make_nvp("has_bias", params_.has_bias),
       cereal::make_nvp("w_stride", cast_size_t(params_.w_stride)),
       cereal::make_nvp("h_stride", cast_size_t(params_.h_stride)));
  }

  template <class Archive>
  static inline void serialize(
    Archive &ar, tiny_dnn::quantized_deconvolutional_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in)),
       cereal::make_nvp("window_width", cast_size_t(params_.weight.width_)),
       cereal::make_nvp("window_height", cast_size_t(params_.weight.height_)),
       cereal::make_nvp("out_channels", cast_size_t(params_.out.depth_)),
       cereal::make_nvp("connection_table", params_.tbl),
       cereal::make_nvp("pad_type", params_.pad_type),
       cereal::make_nvp("has_bias", params_.has_bias),
       cereal::make_nvp("w_stride", cast_size_t(params_.w_stride)),
       cereal::make_nvp("h_stride", cast_size_t(params_.h_stride)));
  }

  template <class Archive>
  static inline void serialize(
    Archive &ar, tiny_dnn::quantized_fully_connected_layer &layer) {
    layer.serialize_prolog(ar);
    auto &params_ = layer.params_;
    ar(cereal::make_nvp("in_size", cast_size_t(params_.in_size_)),
       cereal::make_nvp("out_size", cast_size_t(params_.out_size_)),
       cereal::make_nvp("has_bias", params_.has_bias_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::slice_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape_)),
       cereal::make_nvp("slice_type", layer.slice_type_),
       cereal::make_nvp("num_outputs", cast_size_t(layer.num_outputs_)));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::sigmoid_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::tanh_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::relu_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::softmax_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::leaky_relu_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])),
       cereal::make_nvp("epsilon", layer.epsilon_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::elu_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::tanh_p1m2_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::softplus_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])),
       cereal::make_nvp("beta", layer.beta_),
       cereal::make_nvp("threshold", layer.threshold_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::softsign_layer &layer) {
    layer.serialize_prolog(ar);
    ar(cereal::make_nvp("in_size", cast_size_t(layer.in_shape()[0])));
  }
#endif
};

template <class Archive>
void serialize(Archive &ar, tiny_dnn::elementwise_add_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::average_pooling_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::average_unpooling_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::batch_normalization_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::concat_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::convolutional_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::deconvolutional_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::dropout_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::fully_connected_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::global_average_pooling_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::input_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::linear_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::lrn_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::max_pooling_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::max_unpooling_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::power_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::quantized_convolutional_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::quantized_deconvolutional_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::quantized_fully_connected_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::slice_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::sigmoid_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::tanh_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::relu_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::softmax_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::leaky_relu_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::elu_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::tanh_p1m2_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::softplus_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive>
void serialize(Archive &ar, tiny_dnn::softsign_layer &layer) {
  serialization_buddy::serialize(ar, layer);
}

template <class Archive, typename T>
void serialize(Archive &ar, tiny_dnn::index3d<T> &idx) {
  ar(cereal::make_nvp("width", idx.width_),
     cereal::make_nvp("height", idx.height_),
     cereal::make_nvp("depth", idx.depth_));
}

namespace core {

template <class Archive>
void serialize(Archive &ar, tiny_dnn::core::connection_table &tbl) {
  ar(cereal::make_nvp("rows", cast_size_t(tbl.rows_)),
     cereal::make_nvp("cols", cast_size_t(tbl.cols_)));
  if (tbl.is_empty()) {
    ar(cereal::make_nvp("connection", std::string("all")));
  } else {
    ar(cereal::make_nvp("connection", tbl.connected_));
  }
}

}  // namespace core

}  // namespace tiny_dnn
