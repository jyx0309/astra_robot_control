// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

#include "robot_interfaces/msg/detail/robot_state__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_robot_interfaces
const rosidl_type_hash_t *
robot_interfaces__msg__RobotState__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xc3, 0x24, 0xf2, 0x42, 0x36, 0x93, 0x3a, 0x69,
      0x56, 0xd5, 0xd7, 0xf4, 0x29, 0x70, 0xde, 0x19,
      0x68, 0xfb, 0x17, 0x40, 0x52, 0x01, 0x00, 0xf8,
      0xc7, 0x5a, 0x31, 0xee, 0xdd, 0xf0, 0x74, 0x5a,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "geometry_msgs/msg/detail/pose__functions.h"
#include "geometry_msgs/msg/detail/point__functions.h"
#include "geometry_msgs/msg/detail/quaternion__functions.h"
#include "std_msgs/msg/detail/header__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Point__EXPECTED_HASH = {1, {
    0x69, 0x63, 0x08, 0x48, 0x42, 0xa9, 0xb0, 0x44,
    0x94, 0xd6, 0xb2, 0x94, 0x1d, 0x11, 0x44, 0x47,
    0x08, 0xd8, 0x92, 0xda, 0x2f, 0x4b, 0x09, 0x84,
    0x3b, 0x9c, 0x43, 0xf4, 0x2a, 0x7f, 0x68, 0x81,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Pose__EXPECTED_HASH = {1, {
    0xd5, 0x01, 0x95, 0x4e, 0x94, 0x76, 0xce, 0xa2,
    0x99, 0x69, 0x84, 0xe8, 0x12, 0x05, 0x4b, 0x68,
    0x02, 0x6a, 0xe0, 0xbf, 0xae, 0x78, 0x9d, 0x9a,
    0x10, 0xb2, 0x3d, 0xaf, 0x35, 0xcc, 0x90, 0xfa,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Quaternion__EXPECTED_HASH = {1, {
    0x8a, 0x76, 0x5f, 0x66, 0x77, 0x8c, 0x8f, 0xf7,
    0xc8, 0xab, 0x94, 0xaf, 0xcc, 0x59, 0x0a, 0x2e,
    0xd5, 0x32, 0x5a, 0x1d, 0x9a, 0x07, 0x6f, 0xff,
    0xf3, 0x8f, 0xbc, 0xe3, 0x6f, 0x45, 0x86, 0x84,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char robot_interfaces__msg__RobotState__TYPE_NAME[] = "robot_interfaces/msg/RobotState";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";
static char geometry_msgs__msg__Pose__TYPE_NAME[] = "geometry_msgs/msg/Pose";
static char geometry_msgs__msg__Quaternion__TYPE_NAME[] = "geometry_msgs/msg/Quaternion";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char robot_interfaces__msg__RobotState__FIELD_NAME__header[] = "header";
static char robot_interfaces__msg__RobotState__FIELD_NAME__connected[] = "connected";
static char robot_interfaces__msg__RobotState__FIELD_NAME__enabled[] = "enabled";
static char robot_interfaces__msg__RobotState__FIELD_NAME__busy[] = "busy";
static char robot_interfaces__msg__RobotState__FIELD_NAME__simulated[] = "simulated";
static char robot_interfaces__msg__RobotState__FIELD_NAME__state_valid[] = "state_valid";
static char robot_interfaces__msg__RobotState__FIELD_NAME__flange_pose[] = "flange_pose";
static char robot_interfaces__msg__RobotState__FIELD_NAME__gripper_opening_m[] = "gripper_opening_m";
static char robot_interfaces__msg__RobotState__FIELD_NAME__gripper_force_n[] = "gripper_force_n";
static char robot_interfaces__msg__RobotState__FIELD_NAME__controller_state[] = "controller_state";
static char robot_interfaces__msg__RobotState__FIELD_NAME__active_command_id[] = "active_command_id";
static char robot_interfaces__msg__RobotState__FIELD_NAME__fault[] = "fault";
static char robot_interfaces__msg__RobotState__FIELD_NAME__joint_positions[] = "joint_positions";

static rosidl_runtime_c__type_description__Field robot_interfaces__msg__RobotState__FIELDS[] = {
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__connected, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__enabled, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__busy, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__simulated, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__state_valid, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__flange_pose, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {geometry_msgs__msg__Pose__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__gripper_opening_m, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__gripper_force_n, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__controller_state, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__active_command_id, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__fault, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_interfaces__msg__RobotState__FIELD_NAME__joint_positions, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription robot_interfaces__msg__RobotState__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Pose__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Quaternion__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_interfaces__msg__RobotState__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_interfaces__msg__RobotState__TYPE_NAME, 31, 31},
      {robot_interfaces__msg__RobotState__FIELDS, 13, 13},
    },
    {robot_interfaces__msg__RobotState__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Pose__EXPECTED_HASH, geometry_msgs__msg__Pose__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = geometry_msgs__msg__Pose__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Quaternion__EXPECTED_HASH, geometry_msgs__msg__Quaternion__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = geometry_msgs__msg__Quaternion__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# \\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe6\\x9c\\xba\\xe5\\x99\\xa8\\xe4\\xba\\xba\\xe6\\x95\\xb4\\xe4\\xbd\\x93\\xe7\\x8a\\xb6\\xe6\\x80\\x81\\xe3\\x80\\x82\n"
  "std_msgs/Header header\n"
  "# \\xe6\\x98\\xaf\\xe5\\x90\\xa6\\xe5\\xb7\\xb2\\xe7\\xbb\\x8f\\xe8\\xbf\\x9e\\xe6\\x8e\\xa5\\xe5\\x88\\xb0\\xe6\\x8e\\xa7\\xe5\\x88\\xb6\\xe5\\x99\\xa8\\xe3\\x80\\x82\n"
  "bool connected\n"
  "# \\xe6\\x98\\xaf\\xe5\\x90\\xa6\\xe5\\x85\\x81\\xe8\\xae\\xb8\\xe6\\x89\\xa7\\xe8\\xa1\\x8c\\xe8\\xbf\\x90\\xe5\\x8a\\xa8\\xe3\\x80\\x82\n"
  "bool enabled\n"
  "# \\xe6\\x98\\xaf\\xe5\\x90\\xa6\\xe6\\xad\\xa3\\xe5\\x9c\\xa8\\xe6\\x89\\xa7\\xe8\\xa1\\x8c\\xe4\\xb8\\x80\\xe4\\xb8\\xaa\\xe5\\x8a\\xa8\\xe4\\xbd\\x9c\\xe3\\x80\\x82\n"
  "bool busy\n"
  "# \\xe6\\x98\\xaf\\xe5\\x90\\xa6\\xe4\\xb8\\xba\\xe6\\xa8\\xa1\\xe6\\x8b\\x9f\\xe5\\x90\\x8e\\xe7\\xab\\xaf\\xe3\\x80\\x82\n"
  "bool simulated\n"
  "# \\xe7\\x8a\\xb6\\xe6\\x80\\x81\\xe6\\x95\\xb0\\xe6\\x8d\\xae\\xe6\\x98\\xaf\\xe5\\x90\\xa6\\xe5\\xae\\x8c\\xe6\\x95\\xb4\\xe3\\x80\\x81\\xe6\\x9c\\x80\\xe6\\x96\\xb0\\xe4\\xb8\\x94\\xe9\\x80\\x9a\\xe8\\xbf\\x87\\xe5\\x9f\\xba\\xe7\\xa1\\x80\\xe6\\xa0\\xa1\\xe9\\xaa\\x8c\\xe3\\x80\\x82\n"
  "bool state_valid\n"
  "# base_link \\xe5\\x9d\\x90\\xe6\\xa0\\x87\\xe7\\xb3\\xbb\\xe4\\xb8\\x8b\\xe7\\x9a\\x84\\xe6\\xb3\\x95\\xe5\\x85\\xb0\\xe7\\xbb\\x9d\\xe5\\xaf\\xb9\\xe4\\xbd\\x8d\\xe5\\xa7\\xbf\\xe3\\x80\\x82\n"
  "geometry_msgs/Pose flange_pose\n"
  "# \\xe5\\xa4\\xb9\\xe7\\x88\\xaa\\xe5\\xbc\\x80\\xe5\\xba\\xa6\\xef\\xbc\\x8c\\xe5\\x8d\\x95\\xe4\\xbd\\x8d\\xef\\xbc\\x9a\\xe7\\xb1\\xb3\\xe3\\x80\\x82\n"
  "float64 gripper_opening_m\n"
  "# \\xe5\\xa4\\xb9\\xe7\\x88\\xaa\\xe5\\x8a\\x9b\\xef\\xbc\\x8c\\xe5\\x8d\\x95\\xe4\\xbd\\x8d\\xef\\xbc\\x9a\\xe7\\x89\\x9b\\xe9\\xa1\\xbf\\xe3\\x80\\x82\n"
  "float64 gripper_force_n\n"
  "# \\xe6\\x8e\\xa7\\xe5\\x88\\xb6\\xe5\\x99\\xa8\\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe7\\x8a\\xb6\\xe6\\x80\\x81\\xe7\\xa0\\x81\\xe3\\x80\\x82\n"
  "int32 controller_state\n"
  "# \\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe5\\x8a\\xa8\\xe4\\xbd\\x9c\\xe7\\x9a\\x84\\xe5\\x94\\xaf\\xe4\\xb8\\x80 ID\\xef\\xbc\\x9b\\xe6\\xb2\\xa1\\xe6\\x9c\\x89\\xe5\\x8a\\xa8\\xe4\\xbd\\x9c\\xe6\\x97\\xb6\\xe4\\xb8\\xba\\xe7\\xa9\\xba\\xe3\\x80\\x82\n"
  "string active_command_id\n"
  "# \\xe6\\x95\\x85\\xe9\\x9a\\x9c\\xe7\\xa0\\x81\\xe5\\x92\\x8c\\xe6\\x95\\x85\\xe9\\x9a\\x9c\\xe6\\x96\\x87\\xe6\\x9c\\xac\\xef\\xbc\\x9b\\xe6\\x97\\xa0\\xe6\\x95\\x85\\xe9\\x9a\\x9c\\xe6\\x97\\xb6\\xe4\\xb8\\xba\\xe7\\xa9\\xba\\xe3\\x80\\x82\n"
  "string fault\n"
  "# \\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe5\\x85\\xb3\\xe8\\x8a\\x82\\xe4\\xbd\\x8d\\xe7\\xbd\\xae\\xe5\\x88\\x97\\xe8\\xa1\\xa8\\xe3\\x80\\x82\n"
  "float64[] joint_positions";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
robot_interfaces__msg__RobotState__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_interfaces__msg__RobotState__TYPE_NAME, 31, 31},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 463, 463},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_interfaces__msg__RobotState__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_interfaces__msg__RobotState__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *geometry_msgs__msg__Pose__get_individual_type_description_source(NULL);
    sources[4] = *geometry_msgs__msg__Quaternion__get_individual_type_description_source(NULL);
    sources[5] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
