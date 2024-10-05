# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: dots-and-boxes.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='dots-and-boxes.proto',
  package='dab',
  syntax='proto3',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x14\x64ots-and-boxes.proto\x12\x03\x64\x61\x62\"Y\n\rGameParameter\x12\"\n\x1anumber_of_vertical_columns\x18\x01 \x01(\r\x12$\n\x1cnumber_of_horizontal_columns\x18\x02 \x01(\r\"G\n\x08GameTurn\x12\x10\n\x08vertical\x18\x01 \x01(\x08\x12\x15\n\rtarget_column\x18\x02 \x01(\r\x12\x12\n\ntarget_gap\x18\x03 \x01(\r\"s\n\tGameState\x12\x18\n\x10vertical_columns\x18\x01 \x01(\r\x12\x1a\n\x12horizontal_columns\x18\x02 \x01(\r\x12\x16\n\x0evertical_lines\x18\x03 \x01(\x0c\x12\x18\n\x10horizontal_lines\x18\x04 \x01(\x0c\x62\x06proto3'
)




_GAMEPARAMETER = _descriptor.Descriptor(
  name='GameParameter',
  full_name='dab.GameParameter',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='number_of_vertical_columns', full_name='dab.GameParameter.number_of_vertical_columns', index=0,
      number=1, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='number_of_horizontal_columns', full_name='dab.GameParameter.number_of_horizontal_columns', index=1,
      number=2, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=29,
  serialized_end=118,
)


_GAMETURN = _descriptor.Descriptor(
  name='GameTurn',
  full_name='dab.GameTurn',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='vertical', full_name='dab.GameTurn.vertical', index=0,
      number=1, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='target_column', full_name='dab.GameTurn.target_column', index=1,
      number=2, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='target_gap', full_name='dab.GameTurn.target_gap', index=2,
      number=3, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=120,
  serialized_end=191,
)


_GAMESTATE = _descriptor.Descriptor(
  name='GameState',
  full_name='dab.GameState',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='vertical_columns', full_name='dab.GameState.vertical_columns', index=0,
      number=1, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='horizontal_columns', full_name='dab.GameState.horizontal_columns', index=1,
      number=2, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='vertical_lines', full_name='dab.GameState.vertical_lines', index=2,
      number=3, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='horizontal_lines', full_name='dab.GameState.horizontal_lines', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=193,
  serialized_end=308,
)

DESCRIPTOR.message_types_by_name['GameParameter'] = _GAMEPARAMETER
DESCRIPTOR.message_types_by_name['GameTurn'] = _GAMETURN
DESCRIPTOR.message_types_by_name['GameState'] = _GAMESTATE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

GameParameter = _reflection.GeneratedProtocolMessageType('GameParameter', (_message.Message,), {
  'DESCRIPTOR' : _GAMEPARAMETER,
  '__module__' : 'dots_and_boxes_pb2'
  # @@protoc_insertion_point(class_scope:dab.GameParameter)
  })
_sym_db.RegisterMessage(GameParameter)

GameTurn = _reflection.GeneratedProtocolMessageType('GameTurn', (_message.Message,), {
  'DESCRIPTOR' : _GAMETURN,
  '__module__' : 'dots_and_boxes_pb2'
  # @@protoc_insertion_point(class_scope:dab.GameTurn)
  })
_sym_db.RegisterMessage(GameTurn)

GameState = _reflection.GeneratedProtocolMessageType('GameState', (_message.Message,), {
  'DESCRIPTOR' : _GAMESTATE,
  '__module__' : 'dots_and_boxes_pb2'
  # @@protoc_insertion_point(class_scope:dab.GameState)
  })
_sym_db.RegisterMessage(GameState)


# @@protoc_insertion_point(module_scope)
