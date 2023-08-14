import re
import json
import os
import time

from absl import app
from absl import flags
from absl import logging
import numpy
import tensorflow
from tensorflow.compiler import xla
import onnx
import onnxruntime
import tf2onnx

FLAGS = flags.FLAGS

flags.DEFINE_string('model_dir', None, '')
flags.DEFINE_string('model_ver', None, '')
flags.DEFINE_string('onnx_file', None, '')

def get_input_output_names(model_conf_file):
  model_conf_json = json.loads(open(model_conf_file,'r').read())

  input_dims = [int(inputs['dim']) for inputs in model_conf_json['optimized_inputs']]
  input_names = [inputs['name']+':0' for inputs in model_conf_json['optimized_inputs']]
  output_names = [outputs+':0' for outputs in model_conf_json['outputs']]

  return input_dims, input_names, output_names


def get_model_and_conf_file(model_dir, model_ver):
  model_ver_dir = model_dir + '/' + model_ver
  model_conf_file = model_dir + '/model_conf.json'
  tf_model_pb_file = model_ver_dir + '/graph.pb'

  return model_conf_file, tf_model_pb_file

def read_ego_meta(model_dir, model_ver):
  model_conf_file, tf_model_pb_file = get_model_and_conf_file(
    model_dir, model_ver
  )
  input_dims, input_names, output_names = get_input_output_names(
    model_conf_file
  )
  return tf_model_pb_file, input_names, input_dims, output_names

def get_onnx_model(tf_model_pb_file, input_names, output_names):
  # load ego model
  tf_graph_def, inputs, outputs = tf2onnx.tf_loader.from_graphdef(
    tf_model_pb_file, input_names, output_names
  )

  # convert ego model to onnx model
  onnx_model, _ = tf2onnx.convert.from_graph_def(
    tf_graph_def,
    input_names = inputs,
    output_names = outputs,
    output_path = FLAGS.onnx_file
  )

  return onnx_model

def main(_):
  # read meta data of ego model
  tf_model_pb_file, input_names, input_dims, output_names = read_ego_meta(
    FLAGS.model_dir, FLAGS.model_ver
  )

  # Load the .pb file
  with tensorflow.io.gfile.GFile(tf_model_pb_file, "rb") as f:
    tf_graph_def = tensorflow.compat.v1.GraphDef()
    tf_graph_def.ParseFromString(f.read())

  # get onnx model
  onnx_model = get_onnx_model(tf_model_pb_file, input_names, output_names)

if __name__ == '__main__':
  flags.mark_flags_as_required(
    ['model_dir', 'model_ver', 'onnx_file']
  )
  app.run(main)